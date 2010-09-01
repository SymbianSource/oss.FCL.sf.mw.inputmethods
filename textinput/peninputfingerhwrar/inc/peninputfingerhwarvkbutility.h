/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of virtual key ctrl utility for arabic hwr.
*
*/


#ifndef C_PENINPUTFINGERHWARVKBUTILITY_H
#define C_PENINPUTFINGERHWARVKBUTILITY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class TResourceReader;
class CVirtualKeyboard;
class CVirtualKey;

// CLASS DECLARATION

/**
 *  The virtual key board utility which implement a set of funtions:
 *  1. create the virutal key
 *  2. load virutal key image
 *  3. load the virtual keys
 * 
 *  @lib peninputfingerhwrar.lib
 *  @since S60 v5.0
 */

class PeninputFingerHwrArVkbUtility
    {
public:
    /**
	 * Load VKB image
	 *
	 */
    static void LoadVkbKeyImageL(CVirtualKeyboard& aVkbCtrl, TInt aResId, const TSize& aKeySize);
	
	/**
	 * Load virutal Key and set key rects.
	 *
	 */
	static void LoadVirtualKeypadKeyL(CVirtualKeyboard& aVkbCtrl, const TInt aResId, const RArray<TRect>& aCellRects);

    /** 
	 * create a new virtual key
	 *
	 */
	static CVirtualKey* CreateVkbKeyL(TResourceReader& aReader, const TRect& aKeyRect);
	
	/**
	 * update the virtal key
	 *
	 */
	static void UpdateVkbKeyL( CVirtualKey* aVirtualKey, TResourceReader& aReader, const TRect& aKeyRect );
    };

#endif // C_PENINPUTFINGERHWARVKBUTILITY_H

//  End Of File
