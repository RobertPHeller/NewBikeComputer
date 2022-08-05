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
#  Created       : Mon Jul 18 16:00:26 2022
#  Last Modified : <220805.0459>
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
import Mesh
import importSVG
import os
import sys
sys.path.append(os.path.dirname(__file__))

import datetime

class BoxBottomSpacer(object):
    _totalLength = 93
    _totalWidth  = 61
    _innerLength = 75
    _innerWidth  = 43
    _centerHoleSpacing = 81
    _holeWidthSpacing = 47
    _holeLengthSpacing = 44.5
    _holeOD = 6
    _holeID = 4
    _cutoutXYWL = (14,10,60,41)
    def __init__(self,name,origin,thick=.25*25.4):
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
        cyoffset = self._totalWidth/2
        cxoffset = (self._totalLength-self._centerHoleSpacing)/2
        corigin = origin.add(Base.Vector(cxoffset,cyoffset,0))
        hyoffset = (self._totalWidth-self._holeWidthSpacing)/2
        hxoffset = (self._totalLength-self._holeLengthSpacing)/2
        horigin = origin.add(Base.Vector(hxoffset,hyoffset,0))
        self.holes = {
            1 : corigin,
            2 : corigin.add(Base.Vector(self._centerHoleSpacing,0,0)),
            3 : horigin,
            4 : horigin.add(Base.Vector(self._holeLengthSpacing,0,0)),
            5 : horigin.add(Base.Vector(self._holeLengthSpacing,self._holeWidthSpacing,0)),
            6 : horigin.add(Base.Vector(0,self._holeWidthSpacing,0))
        }
        for i in [1,2,3,4,5,6]:
            holev = self.holes[i]
            hole = Part.Face(Part.Wire(Part.makeCircle(self._holeID/2.0,holev))).extrude(ZThick)
            board = board.cut(hole)
        self.board = board.cut(self._cutout(thick))
    def _cutout(self,thick):
        x,y,w,l = self._cutoutXYWL
        corigin = self.origin.add(Base.Vector(x,y,0))
        return Part.makePlane(w,l,corigin).extrude(Base.Vector(0,0,thick))
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.board
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.82352,.70588,.54901])
        self.object = obj
    def MakeSTL(self,filename):
        objs=[]
        objs.append(self.object)
        Mesh.export(objs,filename)

if __name__ == '__main__':
    App.ActiveDocument=App.newDocument("panel and board")
    doc = App.activeDocument()
    op = Base.Vector(0,0,0)
    bottomSpacer = BoxBottomSpacer("spacer",op)
    bottomSpacer.show()
    bottomSpacer.MakeSTL("BottomSpacer.stl")
    Gui.SendMsgToActiveView("ViewFit")
    Gui.activeDocument().activeView().viewIsometric()
    
