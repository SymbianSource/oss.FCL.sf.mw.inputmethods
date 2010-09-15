/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the  TAknFepInputStateEntryMiniQwertyPinyinPhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_MINIQWERTY_PINYINPHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_MINIQWERTY_PINYINPHRASE_H__

#include "aknfepuiinputminiqwertypinyinphrasebase.h"
/**
 *  TAknFepInputStateEntryMiniQwertyPinyinPhrase class.
 *
 *  Implementation of Pinyin phrase creation entry  state 
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepInputStateEntryMiniQwertyPinyinPhrase: public TAknFepInputMiniQwertyPinyinPhraseBase
    {
public: 
    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEntryMiniQwertyPinyinPhrase(
            MAknFepUIManagerStateInterface* aOwner,
            MAknFepUICtrlContainerChinese* aUIContainer );
    /**
     * From TAknFepInputStateEntryMiniQwertyPinyinPhrase
     * Handle system key press
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
     TBool HandleKeyL( TInt aKey, TKeyPressLength aLength );
     
     /**
      * Handling Command events.
      * This Method is called by CAknFepUIManagerJapanese.
      *
      * @param aCommandId Command ID value
      */
     void HandleCommandL( TInt aCommandId );
     
     
     void SubmitTextL( const TDesC& aText );

    };

/**
 *  TAknFepInputStateEditinigMiniQwertyPinyinPhrase class.
 *  Implementation of Pinyin phrase creation  editing state 
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */

class TAknFepInputStateEditinigMiniQwertyPinyinPhrase: public TAknFepInputMiniQwertyPinyinPhraseBase
    {
public: 

    /**
     * C++ default constructor
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEditinigMiniQwertyPinyinPhrase(
            MAknFepUIManagerStateInterface* aOwner,
            MAknFepUICtrlContainerChinese* aUIContainer );
    /**
     * From TAknFepInputStateEditinigMiniQwertyPinyinPhrase
     * Handle system key press
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL( TInt aKey ,TKeyPressLength alength );
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    
    void SubmitTextL( const TDesC& aText );
    
    };

#endif __AKN_FEP_UI_INPUT_STATE_ENTRY_MINIQWERTY_PINYINPHRASE_H__