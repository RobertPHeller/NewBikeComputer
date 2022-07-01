// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Sep 15 16:01:45 2013
//  Last Modified : <220701.1231>
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

#ifndef __MODES_H
#define __MODES_H

#include "Button.h"

typedef enum {TimeAndSpeed=0, TimeAndDistance, NVRAMSlot} DisplayMode;

DisplayMode operator++(DisplayMode &m);

class ModeButton : public Button {
public:
    ModeButton(int pin) : Button(pin) {currentMode = TimeAndSpeed;}
    DisplayMode CheckModeButton();
private:
    DisplayMode currentMode;
};
    
    

#endif // __MODES_H

