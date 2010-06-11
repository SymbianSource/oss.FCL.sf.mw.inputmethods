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











#ifndef __AKN_FEP_UI_PINYIN_POPUP_H
#define __AKN_FEP_UI_PINYIN_POPUP_H

#include <e32std.h>

class MAknFepUICtrlPinyinPopup
	{
public:
    enum 
        {
        ESpellingChanged    = 0x0001,
        ESpellingNavigation = 0x0002,
		EDispPageChanged	= 0x0004,
		EChangeStatebyBackSpce = 0x1000//need by pinying phrase creation.
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
	*	Return Pinyin spelling candidate array 
	*/
    virtual CDesCArrayFlat* PhoneticSpellingArray(void) = 0;

    /**
	*	Split phrase pinyin spelling candidates into pages 
	*/
    virtual void SplitPhraseSpellingIntoPages(void) = 0;

    /**
	*	Split pinyin spelling candidates into pages 
	*/
    virtual void SplitSpellingIntoPages(TInt aCount) = 0;
    
    /**
	*	Set display page for deliberate selection 
	*/
    virtual void SetDisplayPage(TInt aSelection) = 0;

    /**
	 *	Return keystroke array 
	 */
    virtual CDesCArrayFlat* KeystrokeArray(void) = 0;
    
    /**
	 *	return in effect keystroke array
	 */
    virtual CDesCArrayFlat* InEffectKeystrokeArray(void) = 0;    

    /**
     *	Return show keystroke array 
     */
    virtual CDesCArrayFlat* ShowKeystrokeArray(void) = 0;
    
    /**
     *	Return the optimize spelling array 
     */
    virtual CDesCArrayFlat* OptimizeSpelling(void) = 0;
    
    /**
     *  Return the temp spelling array 
     */
    virtual CDesCArrayFlat* TempSpelling(void) = 0;
    
    /**
     *	Return show keystroke array 
     */
    virtual CDesCArrayFlat* ChooseChineseCharacterArray(void) = 0;
    
    /**
     *	Return show keystroke array 
     */
    virtual CDesCArrayFlat* ChooseChineseCharacterArrayKeystroke(void) = 0;

    /**
     *	Return the tone mark state 
     */
    virtual TBool GetTonemarkState() = 0;
    
    /**
     *	set the tone mark state 
     */
    virtual void SetTonemarkState( TBool aState ) = 0; 
    

    /**
     *	Return show keystroke array 
     */
    virtual CDesCArrayFlat* PhoneticSpellingIndexArray(void) = 0;
    
    /**
	*	Return current page start spelling index 
	*/
    virtual TInt CurrentPageStartIndex(void) = 0;
    virtual void SetFlag(TInt aFlag) = 0;
    virtual void ClearFlag(TInt aFlag) = 0;
    virtual TBool IsFlagSet(TInt aFlag) const = 0;
    virtual void PopupSizeChanged() = 0;
    /**
     *  Set all the input pane not highlight
     */
    virtual void SetNoneHighlighted() = 0;
    
    virtual CDesCArrayFlat* KeystrokeArrayForStroke() = 0;

    virtual CDesCArrayFlat* InEffectKeystrokeArrayForStroke() = 0;

    virtual CDesCArrayFlat* ShowKeystrokeArrayForStroke() = 0;

    virtual CDesCArrayFlat* ChooseChineseCharacterArrayForStroke() = 0;

    virtual CDesCArrayFlat* ChooseChineseCharacterArrayKeystrokeForStroke() = 0;
    
    /**
     * Get the current content array displayed in eep control for Zhuyin method.
     */
    virtual CDesCArrayFlat* ZhuyinSymbols() = 0;
    
    /**
     * Get the current content array displayed in eep control for Zhuyin method.
     */
    virtual CDesCArrayFlat* ZhuyinShowSymbols() = 0;
    
    /**
     * Get the current valid zhuyin symbol group.
     */
    virtual CDesCArrayFlat* FirstValidZhuyinGrp() = 0;
    
    /**
     * Get the current chinese characters displayed in the eep control.
     */
    virtual CDesCArrayFlat* ChineseCharsDisped() = 0;
    
    /**
     * Get the corresponding zhuyin symbol array for chinese characters array.
     */
    virtual CDesCArrayFlat* ZhuyinSymbolsForChineseCharsDisped() = 0;
    
    virtual void SetEEPContent ( const TDes& aDes ) = 0;
    virtual TDes& GetEEPContent ( ) = 0;
    
    virtual void SetEEPContentValid ( TBool aValid ) = 0;
    virtual TBool GetEEPContentValid ( ) = 0;
    
    virtual void ResetStrokeArray() = 0;
    
    virtual void SetChangeState(TBool aChangeState) = 0;
    
    virtual TBool IsChangeStatet() const = 0;
    
    /**
     * Set whether state change from entry to spelling editing is automatical.
     * 
     * @param aState.1 for automatical, 0 for manual.
     * @return None.
     */
    virtual void SetState( TInt aState ) = 0;
    
    /**
     * Get state change status.
     */
    virtual TInt GetState() = 0;
    
    /**
     * Set whether state change from pinyin input state to pinyin phrase creation.
     * 
     * @param aState. ETrue for state change from pinyin input to phrase creation,
     *  EFalse is not.
     * @since S60 V5.0
     * @return None.
     */
    virtual void SetPhraseCreationState( TBool aState ) = 0;
    
    /**
     * Set whether at the editing state happen editing for pinyin phrase creation.
     * 
     * @param aState. ETrue happen editing,EFalse others.
     * @since S60 V5.0
     * @return None.
     */
    virtual void SetPhraseCreationEditingState( TBool aState ) = 0;
    
    /**
     * Get whether state change from pinyin input state to pinyin phrase creation.
     * 
     * @since S60 V5.0
     * @return ETrue for state change from pinyin input to phrase creation,
     *  EFalse is not.
     */
    virtual TBool GetPhraseCreationState() = 0;
    
    /**
     * Get whether at the editing state happen editing for pinyin phrase creation.
     * 
     * @since S60 V5.0
     * @return None.
     */
    virtual TBool GetPhraseCreationEditingState() = 0;
    
    /**
     * set flag whether from candidate state change state.
     * 
     * @param aFlag  ETrue from the candidate state change state, other EFalse.
     * @since S60 V5.0
     * @return none.
     */
    virtual void SetFromCandidateChangeStateFlag( TBool aFlag ) = 0;
    
    /**
     * Get flag whether from candidate state change state.
     * 
     * @since S60 V5.0
     * @return ETrue from the candidate state change state, other EFalse.
     */
    virtual TBool GetFromCandidateChangeStateFlag() = 0;
    
    /**
     * Set count of valid keystroke.
     * 
     * @since S60 V5.0
     * @return none
     */
    virtual void SetValidKeystrokeCount( TInt aCount ) = 0;
    
    /**
     * get count of valid keystroke.
     * 
     * @since S60 V5.0
     * @return count of valid keystroke.
     */
    virtual TInt GetValidKeystrokeCount() = 0;
    
    /**
     * Set last cba resouce id.
     * 
     * @since S60 V5.0
     * @return None
     */
    virtual void SetLastResouce( TInt aResouceId ) = 0;
    
    /**
     * Get last cba resouce id.
     * 
     * @since S60 V5.0
     * @return last resouce id
     */
    virtual TInt GetLastResouce() = 0;
    
    /**
     * Set change state from Edit to Spelling flag.
     * 
     * @param aFlag,If ETrue Change from Edit state to spelling state,EFalse others. 
     * @since S60 V5.0
     * @return none.
     */
    virtual void SetFromEditToSpellingFlag( TBool aFlag = EFalse ) = 0;

    /**
     * Set change state from Edit to Spelling flag.
     * 
     * @param none. 
     * @since S60 V5.0
     * @return ETrue Change from Edit state to spelling state,EFalse others..
     */
    virtual TInt GetFromEditToSpellingFlag() = 0;
    
    /**
     * Set valid keystroke change flag.
     * 
     * @param aFlag, ETrue valid keystroke changed,EFalse others.
     * @since S60 V5.0
     * @return none
     */
    virtual void SetValidKeystrokeChange( TBool aFlag ) = 0;
    
    /**
     * Get valid keystroke change flag.
     * 
     * @since S60 V5.0
     * @return ETrue,if vallid keystroke changed, others EFalse.
     */
    virtual TBool GetValidKeystrokeChange() = 0;
    
    /**
     * Set PreStateIsPredict flag.
     * 
     * @param aFlag, ETrue PreStateIsPredict,EFalse others.
     * @since S60 V5.0
     * @return none
     */
    virtual void SetPreStateIsPredictive( TBool aFlag ) = 0;
    
    /**
     * Get PreStateIsPredict flag.
     * 
     * @since S60 V5.0
     * @return ETrue,if PreStateIsPredict, others EFalse.
     */
    virtual TBool GetPreStateIsPredictive() = 0;

    virtual void SetInputLayoutMiniQwertyZhuyin() = 0;
    
    virtual void ResetInputLayoutMiniQwertyZhuyin() = 0;
	};

#endif // __AKN_FEP_UI_PINYIN_POPUP_H

// End of file
