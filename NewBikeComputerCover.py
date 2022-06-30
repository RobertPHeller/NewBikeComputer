#!/usr/local/bin/FreeCAD019
#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Jun 30 14:57:32 2022
#  Last Modified : <220630.1723>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
#*****************************************************************************


import FreeCAD as App
import Part
from FreeCAD import Base

import os
import sys
sys.path.append(os.path.dirname(__file__))

import datetime

class FLXT_2535_TopCover(object):
    _width=2.32*25.4
    _length=3.32*25.4
    _holeDiameter=(3/16)*25.4  # guess
    _holeWidthSpacing=1.86*25.4
    _holeLengthSpacing=2.86*25.4
    def __init__(self,name,origin,thick=(1/16)*25.4):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector")
        self.origin = origin
        ox = origin.x
        oy = origin.y
        oz = origin.z
        ZNorm=Base.Vector(0,0,1)
        ZThick=Base.Vector(0,0,thick)
        panel = Part.makePlane(self._length,self._width,origin,ZNorm).extrude(ZThick)
        hXoffset = (self._length-self._holeLengthSpacing)/2.0
        hYoffset = (self._width-self._holeWidthSpacing)/2.0
        horigin = origin.add(Base.Vector(hXoffset,hYoffset,0))
        self.holes = {
            1 : horigin,
            2 : horigin.add(Base.Vector(self._holeLengthSpacing,0,0)),
            3 : horigin.add(Base.Vector(self._holeLengthSpacing,self._holeWidthSpacing,0)),
            4 : horigin.add(Base.Vector(0,self._holeWidthSpacing,0))
        }
        for i in [1, 2, 3, 4]:
            mhv = self.holes[i]
            mh = Part.Face(Part.Wire(Part.makeCircle(self._holeDiameter/2.0,mhv))).extrude(ZThick)
            panel = panel.cut(mh)
        self.panel = panel
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.panel
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([1.0,1.0,1.0])
        obj.ViewObject.Transparency=90
        

class FLXT_2535_PCB(object):
    _totalLength = 3.17*25.4
    _totalWidth  = 2.17*25.4
    _innerLength = 2.56*25.4
    _innerWidth  = 1.56*25.4
    _holeDiameter = .129*25.4
    _holeLengthSpacing = 2.14*25.4
    _holeWidthSpacing  = 1.45*25.4
    def __init__(self,name,origin,thick=(1/16)*25.4):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector")
        self.origin = origin
        ox = origin.x
        oy = origin.y
        oz = origin.z
        ZNorm=Base.Vector(0,0,1)
        ZThick=Base.Vector(0,0,thick)
        board=Part.makePlane(self._totalLength,self._totalWidth,origin,ZNorm).extrude(ZThick)
        XCut = (self._totalLength-self._innerLength)/2
        YCut = (self._totalWidth-self._innerWidth)/2
        corner = Part.makePlane(XCut,YCut,origin,ZNorm).extrude(ZThick)
        board = board.cut(corner)
        corner = Part.makePlane(XCut,YCut,origin.add(Base.Vector(XCut+self._innerLength,0,0)),ZNorm).extrude(ZThick)
        board = board.cut(corner)
        corner = Part.makePlane(XCut,YCut,origin.add(Base.Vector(0,YCut+self._innerWidth,0)),ZNorm).extrude(ZThick)
        board = board.cut(corner)
        corner = Part.makePlane(XCut,YCut,origin.add(Base.Vector(XCut+self._innerLength,YCut+self._innerWidth,0)),ZNorm).extrude(ZThick)
        board = board.cut(corner)
        hxoffset = (self._totalLength-self._holeLengthSpacing)/2
        hyoffset = (self._totalWidth-self._holeWidthSpacing)/2
        horigin = origin.add(Base.Vector(hxoffset,hyoffset,0))
        self.holes = {
            1 : horigin,
            2 : horigin.add(Base.Vector(self._holeLengthSpacing,0,0)),
            3 : horigin.add(Base.Vector(self._holeLengthSpacing,self._holeWidthSpacing,0)),
            4 : horigin.add(Base.Vector(0,self._holeWidthSpacing,0))
        }
        for i in [1,2,3,4]:
            holev = self.holes[i]
            hole = Part.Face(Part.Wire(Part.makeCircle(self._holeDiameter/2.0,holev))).extrude(ZThick)
            board = board.cut(hole)
        self.board = board
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.board
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.82352,.70588,.54901])
        


if __name__ == '__main__':
    App.ActiveDocument=App.newDocument("panel and board")
    doc = App.activeDocument()
    op = Base.Vector(0,0,0)
    panel = FLXT_2535_TopCover("panel",op)
    panel.show()
    bxoffset = (FLXT_2535_TopCover._length-FLXT_2535_PCB._totalLength)/2.0
    byoffset = (FLXT_2535_TopCover._width-FLXT_2535_PCB._totalWidth)/2.0
    ob = Base.Vector(bxoffset,byoffset,25.4)
    board = FLXT_2535_PCB("board",ob)
    board.show()
    Gui.SendMsgToActiveView("ViewFit")
    Gui.activeDocument().activeView().viewIsometric()
    
