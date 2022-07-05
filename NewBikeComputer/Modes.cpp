// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Sep 15 16:05:29 2013
//  Last Modified : <220705.0924>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013 Deepwoods Software.
// 
//  All Rights Reserved.
// 
// This  document  may  not, in  whole  or in  part, be  copied,  photocopied,
// reproduced,  translated,  or  reduced to any  electronic  medium or machine
// readable form without prior written consent from Deepwoods Software.
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <Arduino.h>
#include "Modes.h"
#include "Button.h"

DisplayMode operator++(DisplayMode &m) {
    switch (m) {
    case TimeAndSpeed: m=TimeAndDistance; break;
    case TimeAndDistance: m=TimeAndSpeed; break;
    }
    return m;
}

DisplayMode ModeButton::CheckModeButton() {
    if (pressed()) ++currentMode;
    return currentMode;
}
