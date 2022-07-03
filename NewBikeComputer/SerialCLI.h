// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Sep 15 17:54:59 2013
//  Last Modified : <220703.1733>
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

#ifndef __SERIALCLI_H
#define __SERIALCLI_H

#include "PersistentTripDatabase.h"
#include "BikeNVS.h"
#include "Wheelsensor.h"

void ProcessSerialCLI(PersistentTripDatabase *PTD,BikeNVS *NVS,
                      Wheelsensor *wheel);

typedef enum {SET='S', UPLOAD='U', CLEAR='C', ZERO='Z', HELP='H'} Commands;

#endif // __SERIALCLI_H

