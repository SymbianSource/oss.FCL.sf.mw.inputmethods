/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            header file of zhuyin phrase candidate state
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_CANDIDATE_MINIQWERTY_ZHUYINPHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_CANDIDATE_MINIQWERTY_ZHUYINPHRASE_H__

// User includes 
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"

/**
 *  TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase class.
 *
 *  Implementation of Pinyin and Stroke phrase candidate state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase : 
    public TAknFepInputStateCandidateQwertyBaseChinesePhrase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    
protected:

    /**
     * From TAknFepInputStateCandidateBasePhrase
     * Handle system key press
     *
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    
    TBool HandleKeyForMiniQwertyL( TInt aKey,
           TKeyPressLength aLength );
       
    TBool HandleKeyForHalfAndCustomQwertyL( TInt aKey,
           TKeyPressLength aLength );
    TBool HandleKeyL( TInt aKey,TKeyPressLength aLength );
    /**
     *fill the phrase and keystorke array 
     *
     * @since S60 V3.2
     * @param aText Pointer to the spelling 
     * @param aPhrase Pointer to phrase 
     */
    void FillPhraseAndStrokArrayL(const TDesC& aText, const TDesC& aPhrase);
    /**
     * get the key stroke count
     *
     * @since S60 V3.2
     * @param aBuf Pointer to the spelling 
     * 
     */
    TInt GetInputPaneKeyStrokeCount(const TDesC& aBuf);
    /**
     * commit the pharse created to editor
     *
     * @since S60 V3.2
     * 
     */
    TBool ComitPhraseCreatedToEditorL();
    /**
     * Get the valid keystroke.
     * ETure  get the valid keystorke,else EFalse
     */
    TBool CheckToneMarkBorderUpon();
    /**
     * get the index of the tonemark 
     *
     * @since S60 V3.2
     * @param aText  Pointer to the spelling 
     * 
     */
    TInt GetIndexOfToneMark( const TDesC& aText);
    };

#endif __AKN_FEP_UI_INPUT_STATE_CANDIDATE_MINIQWERTY_ZHUYINPHRASE_H__

// End of file
