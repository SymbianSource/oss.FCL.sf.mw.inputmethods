/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUICtrlInputPane definition.
*
*/












#ifndef __AKN_FEP_INPUT_PANE_H__
#define __AKN_FEP_INPUT_PANE_H__

#include <e32std.h>

/**
 * pane in which the user's input is shown
 */
class MAknFepUICtrlInputPane
    {
public:
    /**
     * Different layouts, can be used depending on whether used as the input pane, or as 
     * an entry in the popup pinyin window
     */
    enum TInputPaneLayout
        {
		ELayoutPhraseInputPane = 3,
		ELayoutPhrasePinyinPopupPane,
		ELayoutPhraseMiniQwertyZhuyin
        };

    /**
     * The number of characters that can be displayed in the input pane.
     * This does not include the tone mark, which is counted separately.
     */
    enum
        {
        EMaxToneMarkLength = 1,
        EMaxInputCharsInputPaneNotIncludingToneMark = 30, // need 30 in the case of stroke
        EMaxInputCharsPinyinPopupNotIncludingToneMark = 21,
        EMaxInputCharsZhuyinPopup = 16,
        EMaxInputCharsInputPane = EMaxInputCharsInputPaneNotIncludingToneMark + EMaxToneMarkLength,
        EMaxInputCharsPinyinPopup = EMaxInputCharsPinyinPopupNotIncludingToneMark + EMaxToneMarkLength,
        EMaxInputChars = EMaxInputCharsInputPane + EMaxInputCharsPinyinPopup // guaranteed big enough for either
        };
    
    enum 
        {
        EPhoneticChanged  = 0x0001
        };

public:
    virtual void SetText(const TDesC& aDes) = 0;

    /**
     * Get the text buffer of the pane
     *
     * @output aText the buffer is filled with the contents of the candidate
     *              of the pane, up to the size of the buffer
     */
    virtual void GetText(TDes& aText) const = 0;

    /**
     * This function sets the Tone Mark for the Pane.
     *
     * @param aToneMark descriptor containing the tone mark character
     */
    virtual void SetToneMark(const TDesC& aToneMark) = 0;

    /**
     * This function sets the validity of the tonemark
     *
     * @param aValid if ETrue, tonemark will be displayed as valid
     *                          if EFalse, tonemark will be displayed as not valid
     */
    virtual void SetToneMarkValidity(TBool aValid) = 0;

    /**
     * sets the font to be used, which will override the font from LAF
     *
     * @param aFontId the new font id, pass in 0 to use the one from LAF
     *
     */
    virtual void SetOverrideFontId(TInt aFontId) = 0;
    virtual void SetFlag(TInt aFlag) = 0;
    virtual void ClearFlag(TInt aFlag) = 0;
    virtual TBool IsFlagSet(TInt aFlag) const = 0;
    };

#endif //__AKN_FEP_INPUT_PANE_H__

// End of file
