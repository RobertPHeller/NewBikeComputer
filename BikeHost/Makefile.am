ACLOCAL_AMFLAGS = -I m4

SUBDIRS = BuildScripts BuildData Docs Scripts
EXTRA_DIST = bootstrap
docdir = $(datadir)/$(PACKAGE)-$(VERSION)/documentation

bin_SCRIPTS = BikeHost$(EXEEXT)
doc_DATA = refman.pdf

LibraryScripts = Scripts/DynamicHelp.tcl Scripts/IconsAndImages.tcl \
		 Scripts/snitmainframe.tcl Scripts/snitscrollw.tcl \
		 Scripts/snitLFs.tcl Scripts/snitbuttonbox.tcl \
		 Scripts/snitdialog.tcl Scripts/snitrotext.tcl \
		 Scripts/snitHTMLHelp.tcl Scripts/snitlistbox.tcl \
		 Scripts/snitStdMenuBar.tcl Scripts/gettext.tcl
		 
LibraryImages = Scripts/close.gif Scripts/new.gif Scripts/print.gif \
		Scripts/DeepwoodsBanner.gif Scripts/openfold.png \
		Scripts/questhead.xbm Scripts/gray50.xbm Scripts/open.gif \
		Scripts/save.gif Scripts/info.xbm Scripts/palette.png \
		Scripts/unknown.xpm

BikeHostDATA = $(LibraryScripts) $(LibraryImages)

CLEANFILES = BikeHost$(EXEEXT) BikeHost.kit

TCLKIT = $(top_builddir)/BuildData/tclkit
BUILT_SOURCES = $(TCLKIT)
SDX = $(top_srcdir)/BuildScripts/sdx.kit
AddKitDir = $(top_srcdir)/BuildScripts/AddKitDir.kit
AddKitFile = $(top_srcdir)/BuildScripts/AddKitFile.kit
MakePkgIndex = $(top_srcdir)/BuildScripts/MakePkgIndex.kit

BikeHost$(EXEEXT): Scripts/BikeHost.tcl $(BikeHostDATA) html/index.html
	-rm -f  BikeHost.kit
	-rm -f  BikeHost$(EXEEXT)
	-rm -rf BikeHost.vfs
	$(TCLKIT) $(SDX) qwrap $(srcdir)/Scripts/BikeHost.tcl
	$(TCLKIT) $(SDX) unwrap BikeHost.kit
	-rm -f BikeHost.kit
	$(TCLKIT) $(AddKitDir) BikeHost lib $(SNITLIB)
	$(TCLKIT) $(AddKitFile) -srcdir $(srcdir) BikeHost \
		lib/Common $(BikeHostDATA)
	$(TCLKIT) $(MakePkgIndex) BikeHost Common
	$(TCLKIT) $(AddKitFile) BikeHost HTML \
		html/*.html html/*.css html/*.png
	$(TCLKIT) $(SDX) wrap BikeHost$(EXEEXT) \
		-runtime $(top_srcdir)/BuildData/$(TCLKITRUN)
	-rm -rf BikeHost.vfs


html/index.html : FORCE
	$(MAKE) -C Docs ../html/index.html
        
latex/refman.tex : FORCE
	$(MAKE) -C Docs ../latex/refman.tex
        
refman.pdf : latex/refman.tex
	$(MAKE) -C latex refman.pdf
		-cp latex/refman.pdf ./
                
BuildData/tclkit : FORCE
	$(MAKE) -C BuildData tclkit
	
FORCE:

clean-local:
	-rm -rf BikeHost.vfs
	-rm -rf html latex
