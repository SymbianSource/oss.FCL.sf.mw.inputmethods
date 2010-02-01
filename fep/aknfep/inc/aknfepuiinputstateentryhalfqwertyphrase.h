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
* Description:           
 *       Provides the  TAknFepInputStateEntryHalfQwerty definition.
 *
*/












#ifndef T_AKNFEPINPUTSTATEENTRYHALFQWERTYPINYINPHRASE_H
#define T_AKNFEPINPUTSTATEENTRYHALFQWERTYPINYINPHRASE_H

#include "AknFepUiInputStateEntryQwertyBaseChinesePhrase.h"
#include "aknfepuiinputstateminiqwertystrokephrase.h"
#include "AknFepUiInputStateEntryZhuyinPhrase.h"
#include "AknFepUiInputStateChineseBase.h"

// the entry of the pinyin inputmethod 
class TAknFepInputStateEntryHalfQwertyPinyinPhrase :
public TAknFepInputStateEntryQwertyBaseChinesePhrase
    {
public:
    /**
     * C++ default constructor
     *
     * @since S60 v3.2.3
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEntryHalfQwertyPinyinPhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer );
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
     * update the selection 
     *
     * @since S60 v3.2.3
     */
    void DeliberatelyUpdateSelection( );
    /**
     * update the selection  
     *
     * @since S60 v3.2.3
     */
    void ImplicitlyUpdateSelection( );
    /**
     * clear the deliberate selection
     *
     * @since S60 v3.2.3
     */
    void ClearDeliberateSelection( );
    /**
     * refresh the ui 
     *
     * @since S60 v3.2.3
     */
    void RefreshUI( TInt aSelection );
    /**
     * change the cba 
     *
     * @since S60 v3.2.3
     */
    void InitializeStateL( );
    /**
     * update the indicator  
     *
     * @since S60 v3.2.3
     */
    void UpdateIndicator( );
    /**
     * to check whether the key stroke is a character key or not 
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @return ETrue if it is a character key , otherwise not
     */
    TBool IsCharacterKey( const TInt aKey );
    };

// the entry state of the stroke inputmethod  
class TAknFepInputStateEntryHalfQwertyStrokePhrase :
public TAknFepInputStateEntryQwertyBaseChinesePhrase
    {
public:
    /**
     * C++ default constructor
     *
     * @since S60 v3.2.3
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEntryHalfQwertyStrokePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer );
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
     * change the cba 
     *
     * @since S60 v3.2.3
     */
    void InitializeStateL( );
    /**
     * to check whether the key stroke is a character key or not 
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @return ETrue if it is a character key , otherwise not
     */
    TBool IsCharacterKey( const TInt aKey );
    
    /**
     * update the indicator  
     *
     * @since S60 v3.2.3
     */
    void UpdateIndicator( );

    };
// the zhuyin inputmethod entry state 
class TAknFepInputStateEntryHalfQwertyZhuyinPhrase :
public TAknFepInputStateEntryQwertyBaseChinesePhrase
    {
public:
    /**
     * C++ default constructor
     *
     * @since S60 v3.2.3
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEntryHalfQwertyZhuyinPhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer );
    /**
     * Handle system key press event
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL( TInt aKey, TKeyPressLength aLength );

public:
    // Functions from MPtiObserver interface
    void KeyTimerExpired( );
    
private:
    /**
     * update the indicator  
     *
     * @since S60 v3.2.3
     */
    void UpdateIndicator( );
    };
#endif T_AKNFEPINPUTSTATEENTRYHALFQWERTYPINYINPHRASE_H

//end of the file 
