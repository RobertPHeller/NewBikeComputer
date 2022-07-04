##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sun Mar 9 11:13:09 2014
#  Last Modified : <220704.1253>
#
#  Description	
#
#  Notes
#
#  History
#	
##############################################################################
#
#  Copyright (c) 2014 Deepwoods Software.
# 
#  All Rights Reserved.
# 
#  This  document  may  not, in  whole  or in  part, be  copied,  photocopied,
#  reproduced,  translated,  or  reduced to any  electronic  medium or machine
#  readable form without prior written consent from Deepwoods Software.
#
##############################################################################


package require Tk;#                    Base GUI Toolkit
package require tile;#                  Themed widgets
package require snit
package require MainFrame
package require IconImage
package require ScrollWindow
package require snitStdMenuBar
package require HTMLHelp
package require ROText

IconImage image DeepwoodsBanner

global Main Status Log ToolBar
set Status {}
set menu [StdMenuBar MakeMenu \
          -file {&File file:menu file 0 {
            {command &Open... file:open "Open Port" {Ctrl o} -command {Port Open}} 
            {command &Close   file:close "Close Port" {Ctrl c} -command {Port Close} -state disabled}
            {command E&xit {file:exit} "Exit the application" {Ctrl q} -command CarefulExit} 
          }} \
          -help {&Help help help 0 {
            {command "On &Help..." help:help "Help on help" {} -command {HTMLHelp help Help}}
            {command "&Tutorial..." help:tutorial "Tutorial" {} -command {HTMLHelp help Tutorial}}
            {command "On &Version" help:version "Version" {} -command {HTMLHelp help Version}}
            {command "Warranty" help:warranty "Warranty" {} -command {HTMLHelp help Warranty}}
            {command "Copying" help:copying "Warranty" {} -command {HTMLHelp help Copying}}
          }}]
#puts stderr "*** BikeHost.tcl: $menu"
set Main [MainFrame .mainframe -menu $menu -textvariable Status]
pack .mainframe -expand yes -fill both
$Main showstatusbar status
set Toolbar [$Main addtoolbar]
$Main showtoolbar 0 yes
pack [ttk::button $Toolbar.settz -text "Set TimeZone" -command {Port SetTimeZone} -state disabled] -side left
pack [ttk::button $Toolbar.cleartrips -text "Clear Stored Trips" -command {Port ClearTrips} -state disabled] -side left
pack [ttk::button $Toolbar.uploadtrips -text "Upload Stored Trips" -command {Port UploadTrips} -state disabled] -side left
pack [ttk::button $Toolbar.zeronvs -text "Zero NVS" -command {Port ZeroNVS} -state disabled] -side left
set uframe [$Main getframe]
set sw [ScrolledWindow $uframe.sw -scrollbar vertical -auto vertical]
pack $sw -expand yes -fill both
set Log [ROText [$sw getframe].log]
$sw setwidget $Log
bind . <Control-q> CarefulExit
set HTMLdir [file join [file dirname \
                        [file dirname \
                         [file dirname [info script]]]] HTML]
HTMLHelp setDefaults $HTMLdir toc.html
wm protocol . WM_DELETE_WINDOW CarefulExit
wm title . "Bike Host"


proc CarefulExit {{dontask no}} {
    set ans yes
    if {!$dontask} {
        set ans [tk_messageBox -icon question -type yesno -message {Really Exit }]
    }
    switch -exact "$ans" {
        no {return}
        yes {exit}
    }
}

snit::type Port {
    pragma -hastypeinfo    no
    pragma -hastypedestroy no
    pragma -hasinstances   no

    typevariable BikeComputerTTYPattern /dev/ttyUSB*
    typecomponent portSelectDialog
    typecomponent   selectedPort
    typevariable portfd {}
    typeconstructor {
        set portSelectDialog {}
    }
    typemethod _createPortSelectDialog {} {
        if {$portSelectDialog ne "" && [winfo exists $portSelectDialog]} {
            return
        }
        set portSelectDialog [Dialog .portselect \
                              -bitmap questhead -default 0 \
                              -cancel 1 -modal local -transient yes -parent . \
                              -side bottom -title "Select a port"]
        $dialog add ok -text Select -command [mytypemethod _Select]
        $dialog add cancel -text Cancel -command [mytypemethod _Cancel]
        wm protocol [winfo toplevel $dialog] WM_DELETE_WINDOW [mytypemethod _Cancel]
        $dialog add help -text Help -command {HTMLHelp help {Select a Port}}
        set frame [$dialog getframe]
        set selectedPort [LabelComboBox $frame.selectedPort \
                          -label "Selected Port" -editable no]
        pack $selectedPort -fill x
    }
    typemethod draw {args} {
        $type _createPortSelectDialog
        set ports [from args -ports]
        $selectedPort configure -values $ports
        $selectedPort set [lindex $ports 0]
        return [$portSelectDialog draw]
    }
    typemethod _Select {} {
        $portSelectDialog withdraw
        $portSelectDialog enddialog [$portSelectDialog get]
    }
    typemethod _Cancel {} {
        $portSelectDialog withdraw
        $portSelectDialog enddialog {}
    }
    typevariable _ready no
    typemethod _ReadPort {} {
        global Log
        foreach {in out} [fconfigure $portfd -queue] {break}
        if {$in > 0} {
            set buffer [read $portfd $in]
            $Log insert end "$buffer"
            if {[string range $buffer end-1 end] eq ">>"} {
                set _ready yes
            } else {
                set _ready no
            }
        }
    }        
    typemethod Close {} {
        global Main Toolbar
        catch {close $portfd}
        $Main setmenustate file:close disabled
        foreach tb [winfo children $Toolbar] {
            $tb configure -state disabled
        }
    }    
    typemethod Open {} {
        global Main Toolbar Log
        if {$portfd ne ""} {$type Close}
        set allports [glob -nocomplain $BikeComputerTTYPattern]
        if {[llength $allports] == 0} {
            tk_messageBox -icon error -type ok -message {No ports found. Is the Bike Computer plugged in?}
            return
        }
        if {[llength $allports] == 1} {
            set port [lindex $allports 0]
        } else {
            set port [$type draw -ports $allports]
            if {$port eq {}} {return}
        }
        set portfd [open $port w+]
        fconfigure $portfd -mode 115200,n,8,2 -blocking no -buffering none \
              -handshake none -translation {crlf cr}
        $Log delete 0.0 end-1c
        fileevent $portfd readable [mytypemethod _ReadPort]
        $Main setmenustate file:close normal
        foreach tb [winfo children $Toolbar] {
            $tb configure -state normal
        }
    }
    typemethod SetTimeZone {} {
        global Log
        while {!$_ready} {update}
        set cmd [format {S %s} [exec date +%:::z]]
        $Log insert end "$cmd\n"
        set _ready no
        puts $portfd "$cmd"
    }
    typemethod ClearTrips {} {
        global Log
        while {!$_ready} {update}
        set cmd "C"
        $Log insert end "$cmd\n"
        set _ready no
        puts $portfd "$cmd"
    }
    typemethod UploadTrips {} {
        global Log
        while {!$_ready} {update}
        set cmd "U"
        set pos [$Log index end]
        #puts stderr "*** $type UploadTrips: pos is $pos"
        $Log insert end "$cmd\n"
        set _ready no
        puts $portfd "$cmd"
        while {!$_ready} {update}
        set block [$Log get $pos end]
        #puts stderr "*** $type UploadTrips: returned block is $block"
        if {[regexp $block {--Uploading File--\nLength: ([[:digit:]]+)\n(.*)--UploadComplete--\n} $block => ContentLength buffer] > 0} {
            set outfile [tk_getSaveFile -defaultextension .txt \
                         -filetypes { 
                         {{Text Files}       {.txt}    TEXT} 
                         {{All Files}        *             } 
                     } \
                           -initialdir [pwd] \
                           -initialfile trips.txt -parent . \
                           -title "File to save trips in"]
            if {$outfile eq ""} {return}
            set outfd [open $outfile w]
            puts $outfd $buffer
            close $outfd
        }
        #puts stderr "*** $type UploadTrips: leftover is $buffer"
    }
    typemethod ZeroNVS {} {
        global Log
        while {!$_ready} {update}
        set cmd "Z"
        $Log insert end "$cmd\n"
        set _ready no
        puts $portfd "$cmd"
    }
}
