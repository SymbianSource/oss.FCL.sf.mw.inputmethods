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
*       Provides the CAknFepUICtrlPinyinPopup class definitions.
*
*/











#ifndef AKN_FEP_UI_EDIT_ENTRY_POPUP_H
#define AKN_FEP_UI_EDIT_ENTRY_POPUP_H

#include <e32std.h>

class MAknFepUICtrlEditEntryPopup
    {
public:
    enum 
        {
        ESpellingChanged    = 0x0001,
        ESpellingNavigation = 0x0002,
            EDispPageChanged    = 0x0004
        };
    /**
     * The cursor move orientation.
     */    
    enum TMoveDirect
        {
        ELeft = 0,
        ERight = 1
        }; 
    /**
     * The number of selections that can be displayed in the popup.
     */
    enum
        {
        EMaxToneMarkLength = 1,
        EMaxCandidates = 6,
        EMaxCandidateCharsNotIncludingToneMark = 6,
        EMaxCandidateChars = EMaxCandidateCharsNotIncludingToneMark + EMaxToneMarkLength,
        EMaxPhraseCandidate = 10
        };

    typedef TBufC<EMaxToneMarkLength> TToneMark;

public:
    
    /**
     * This function move the cursor in the EEP.
     *
     * @param aMoveDirect if ELeft, the cursor move left
     *                    if ERight, the cursor move right
     *
     * @return EFalse if the cursor can't move again other ETrue.
     */
    virtual TBool Move( TInt aMoveDirect ) const = 0;
      
    /**
     * This function move the cursor in the EEP.
     *
     * @param aCount the number of the verifide Key sequence.
     *
     * @return None
     */
    virtual void SetVerifiedKeySequenceCount( TInt aCount ) const = 0;
      
    /**
     * This function move the cursor in the EEP.
     *
     * @param aFlag if ETrue, the key sequence is verified
     *              if EFalse, the key sequence is not verified.
     *      
     * @return EFalse if the cursor can't move again other ETrue.
     */
      virtual TBool SetVerifiedKeySequenceFlag( TBool aFlag) const = 0;
      
    /**
     * This function returns the index of the selected phrase item .
     *
     * @return aIndex index of entry in window, zero based
     */
    virtual TInt CurrentSelection() const = 0;
    
    /**
     * This function returns the visible index of the selected item.
     *
     * @return aIndex index of entry in window, zero based
     */
    virtual TInt CurrentVisibleSelection() const = 0;

    /**
     * Enables the display of the control. Sets the control's 
     * window postion and visibility, but only if the value has
     * changed (so it's ok to call it excessively). 
     *
     * @param aEnable if ETrue, makes visible
     *                          if EFalse, makes non-visible
     *
     */
    virtual void Enable(TBool aEnable) = 0;
    /**
     * set the number of selections that are visible
     *
     * @param aCount the number of visible selections, must be 
     *                          0 < aCount <= KMaxNumberOfSelections 
     */
    virtual void SetVisibleCount(TInt aCount) = 0;

    /**
     * get the current visible selection count
     *
     * @return visible selection count
     */
    virtual TInt VisibleSelectionCount() const = 0;


    /**
     * This sets the string of the internal buffer for the item index aIndex to aText,
     * even if the index falls outside the current visible candidates.
     *
     * @param aText new contents of text item
     * @return ETrue if successful
     *              EFalse if index outside visible candidates (but value is still set)
     *
     */
    virtual TBool SetItemText(TInt aIndex, const TDesC& aText) = 0;

    /**
     *   Sets the string of the internal buffer for visible item 
     */
    virtual TBool SetPhraseItemTexts() = 0;
    
    /**
     * This function gets the text at the index aIndex. 
     *
     * @param aIndex index of entry in window, zero based
     * @output aText contains contents of text item
     * @return ETrue always
     *
     */
    virtual TBool GetItemText(TInt aIndex, TDes& aText) const = 0;

    /**
     * This function sets the Tone Mark for a specific entry
     * 
     * @param aIndex index of entry
     * @param aToneMark the tone mark
     *
     */
    virtual void SetToneMark(TInt aIndex, const TDesC& aToneMark) = 0;

    /**
     * This function sets the validity of the tonemark
     *
     * @param aIndex index of the entry to be affected
     * @param aValid if ETrue, tonemark will be displayed as valid
     *                          if EFalse, tonemark will be displayed as not valid
     */
    virtual void SetItemToneMarkValidity(TInt aIndex, TBool aValid) = 0;
    /**
     * Is the control enabled for display?
     *
     * @return if not enabled, return EFalse
     */
    virtual TBool IsEnabled() const = 0;

    /**
     * Select the next visible item. If already on the last one, loop to the 
     * first one.
     *
     * @return ETrue if success, 
     *              EFalse if no items visible, or if selection is outside visible items
     */
    virtual TBool SelectNextPhrase() = 0;

    /**
     * Select the previous visible item. If already on the first one, loop to the 
     * last one.
     *
     * @return ETrue if success, 
     *              EFalse if no items visible, or if selection is outside visible items
     */
    virtual TBool SelectPrevPhrase() = 0;

    /**
     * Return Pinyin spelling candidate array 
     */
    virtual CDesCArrayFlat* PhoneticSpellingArray(void) = 0;

    /**
     * Split phrase pinyin spelling candidates into pages 
     */
    virtual void SplitPhraseSpellingIntoPages(void) = 0;

    /**
     * Split pinyin spelling candidates into pages 
     */
    virtual void SplitSpellingIntoPages(TInt aCount) = 0;
    
    /**
     * Set display page for deliberate selection 
     */
    virtual void SetDisplayPage(TInt aSelection) = 0;

    /**
     *    Return keystroke array 
     */
    virtual CDesCArrayFlat* KeystrokeArray(void) = 0;
    
    /**
     *    return in effect keystroke array
     */
    virtual CDesCArrayFlat* InEffectKeystrokeArray(void) = 0;    

    /**
     *    Return show keystroke array 
     */
    virtual CDesCArrayFlat* ShowKeystrokeArray(void) = 0;
    
    /**
     *    Return current page start spelling index 
     */
    virtual TInt CurrentPageStartIndex(void) = 0;
    virtual void SetFlag(TInt aFlag) = 0;
    virtual void ClearFlag(TInt aFlag) = 0;
    virtual TBool IsFlagSet(TInt aFlag) const = 0;
    virtual void PopupSizeChanged() = 0;
    };

#endif // AKN_FEP_UI_EDIT_ENTRY_POPUP_H

// End of file
