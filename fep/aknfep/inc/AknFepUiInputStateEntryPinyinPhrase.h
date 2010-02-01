/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryPinyinPhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INPUT_PINYIN_PHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_INPUT_PINYIN_PHRASE_H__

#include "AknFepUiInputStateChineseBase.h"

class TAknFepInputStateEntryPinyinPhrase : public TAknFepInputStateChineseBase
    {
public:
	TAknFepInputStateEntryPinyinPhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    void InitializeStateL();

    void HandleCommandL(TInt aCommandId);
private:
    void ClearDeliberateSelection();
    void DeliberatelyUpdateSelection();
    void ImplicitlyUpdateSelection();
    void RefreshUI(TInt aSelection);
    void UpdateIndicator();

    //the follow 12 functions for phrase creation
    /**
     * Optimize spelling.
     *
     * @since S60 v5.0.
     * @param aOptimizeBefore the before spelling array be Optimized.
     * @param aOptimizeAfter the after spelling array be Optimized..
     * @return None.
     */
    void OptimizeSpellingL(CDesCArrayFlat& aOptimizeBefore,
        CDesCArrayFlat& aOptimizeAfter);

    /**
     * Delete current keystroke.
     *
     * @since S60 v5.0.
     * @return None.
     */
    void DeleteCurrentKeystroke();

    /**
     * Add current keystroke.
     *
     * @since S60 v5.0.
     * @param keystroke input keystroke.
     * @return None.
     */
    void AddCurrentKeystrokeL(const TDesC& keystroke);

    /**
     * Get the keystroke.
     *
     * @since S60 v5.0.
     * @param aKey input key.
     * @param aKeystroke the aKey's keytroke.
     * @return None.
     */
    void GetKeystrokeNum(TInt aKey, TDes& aKeystroke);

    /**
     * Get the keystroke buf.
     *
     * @since S60 v5.0.
     * @param aKeystrokeArray keytroke array.
     * @param aKeystrokeBuf the keytroke buf.
     * @return None.
     */
    void GetKeystrokeBuf(const CDesCArrayFlat& aKeystrokeArray,
        TDes& aKeystrokeBuf);

    /**
     * clear keystroke.
     *
     * @since S60 v5.0.
     * @return None.
     */
    void ClearKeystrokeBuf();

    /**
     * Get tome mark.
     *
     * @since S60 v5.0.
     * @param aKeystrokeArray keytroke array.
     * @param aPinyinToneMark the tone mark.
     * @return None.
     */
    void GetPinyinToneMark(const CDesCArrayFlat& aKeystrokeArray,
        TDes& aPinyinToneMark);

    /**
     * Check keystroke.
     *
     * @since S60 v5.0.
     * @param aKeystrokeArray keytroke array.
     * @param aPinyinToneMark the tone mark.
     * @return None.
     */
    TBool CheckKeystroke(TInt aKey, const CDesCArrayFlat& aKeystrokeArray);

    /**
     * Check tome mark.
     *
     * @since S60 v5.0.
     * @return None.
     */
    void CheckTonemarkL();

    /**
     * Get change state.
     *
     * @since S60 v5.0.
     * @return ETrue change state, EFasle others.
     */
    TBool GetChangeState();

    /**
     * Append show keystroke.
     *
     * @since S60 v5.0.
     * @param aShowKeystrokeArray the show keytroke array.
     * @param aKeystrokeArray the keytroke array.
     * @param aPinyinToneMark the tome mark.
     * @return None.
     */
    void ShowKeystrokeAppendL(CDesCArrayFlat& aShowKeystrokeArray,
        const CDesCArrayFlat& aKeystrokeArray, const TDesC& aPinyinToneMark);

    /**
     * change  CBA.
     *
     * @since S60 v5.0.
     * @return None.
     */
    void ChangeCbaL();
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INPUT_PINYIN_PHRASE_H__

// End of file
