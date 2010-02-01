/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:            This file contains classes of phrase creation layout data management
*
*/












#ifndef C_AKNFEPINPUTSTATEMINIQWERTYZHUYINKEYMAP_H
#define C_AKNFEPINPUTSTATEMINIQWERTYZHUYINKEYMAP_H

#include <e32std.h>
#include <e32base.h>
#include "PtiEngine.h"

//class CAknFepInputStateMiniQwertyZhuyinKeyMap;
/**
 *  CAknFepInputStateMiniQwertyZhuyinKeyMap
 *  This class provides mini-qwerty zhuyin key map 
 *  zhuyin key map interface management
 *  to the program other component
 */
/**
* CAknFepInputStateMiniQwertyZhuyinKeyMap is a static fascade interface to the mini-qwerty zhuyin key map object.
* There is at most one CAknFepInputStateMiniQwertyZhuyinKeyMap per thread. This static interface ensures that this
* fact can not be changed.
*/
class TAknFepInputStateMiniQwertyZhuyinKeyMap
    {
public:
    // General public API
    
    /**
     * Get the show keystroke for the keytype.
     * 
     * @param aPtiEngine, ptiengine.
     * @param aKey, the input keystroke.
     * @param aKeystroke, the keystroke.
     * @since S60 v3.2.3.
     * @return None.
     */
    static void GetKeyStroke( CPtiEngine* aPtiEngine, TInt& aKey, TDes& aKeystroke );
    };

#endif // C_AKNFEPINPUTSTATEMINIQWERTYZHUYINKEYMAP_H

// End of File
