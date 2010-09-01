/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           
 *       Provides the TAknFepInputStateInitialChineseGenericHalfQwerty definition.
 *
*/











#ifndef AKNFEPUIINPUTSTATEINITIALCHINESEGENERICHALFQWERTY_H_
#define AKNFEPUIINPUTSTATEINITIALCHINESEGENERICHALFQWERTY_H_
#include "AknFepUiInputStateHalfQwerty.h"

class TAknFepInputStateInitialChineseGenericHalfQwerty :
public TAknFepInputStateHalfQwerty
    {
public:
    /**
     * C++ default constructor
     *
     * @since S60 v3.2.3
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateInitialChineseGenericHalfQwerty(
        MAknFepUIManagerStateInterface* aOwner, TInt aMode );
    /**
     * Handle system key press event
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL( TInt aKey, TKeyPressLength aLength );

private:
    /**
     * to check whether the key stroke is the sct key or not 
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @return ETrue if it is the sct key , otherwise not
     */
    TBool IsSCTKey( TInt aKey );
    /**
     * to check whether the key stroke is a character key or not 
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @return ETrue if it is a character key , otherwise not
     */
    TBool IsCharacter( TInt aKey );
    /**
     * Handle the shift key envent 
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyByShiftOrCharPressedL( TInt aKey, TKeyPressLength aLength );
    /**
     * to check whether the key stroke is a emotion key  or not 
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @return ETrue if it is a emotion key , otherwise not
     */
    TBool HandleEmotionKeyL( TInt aKey, TKeyPressLength aLength );
    /**
     * to check whether the key is a Multitap key or not 
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @return ETrue if it is a Multitap key , otherwise not
     */
    TBool IsMultitapkey( TInt aKey );
    };
#endif /*AKNFEPUIINPUTSTATEINITIALCHINESEGENERICHALFQWERTY_H_*/

//end of the file
