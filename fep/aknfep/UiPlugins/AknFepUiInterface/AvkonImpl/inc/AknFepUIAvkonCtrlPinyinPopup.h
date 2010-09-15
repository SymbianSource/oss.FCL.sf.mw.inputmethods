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











#ifndef __AKN_FEP_UI_AVKON_PINYIN_POPUP_H
#define __AKN_FEP_UI_AVKON_PINYIN_POPUP_H

#include <AknFepUICtrlPinyinPopup.h>
#include <badesca.h>
#include <coecntrl.h>
#include <AknUtils.h>
#include "AknFepUIAvkonCtrlInputPane.h"
#include "AknFepUIAvkonCtrlCandidatePane.h"

class MAknFepUICtrlPinyinPopupSizeChangedObserver;

NONSHARABLE_CLASS(CAknFepUICtrlPinyinPopup):public CCoeControl, public MAknFepUICtrlPinyinPopup
{
public:
    /**
     * First phase construction
     */
    static CAknFepUICtrlPinyinPopup* NewL(RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr);
    /**
     * destructor
     */
    ~CAknFepUICtrlPinyinPopup();
    /**
     * This function returns the index of the selected phrase item .
     *
     * @return aIndex index of entry in window, zero based
     */
    TInt CurrentSelection() const;
    /**
     * This function returns the visible index of the selected item.
     *
     * @return aIndex index of entry in window, zero based
     */
    TInt CurrentVisibleSelection() const;
	 /**
     * set the observer
     * <p> Note that only one observer is supported!
     *
     * @param aObserver pointer to the observer
     */
    void SetSizeChangedObserver(MAknFepUICtrlPinyinPopupSizeChangedObserver* aObserver);

    /**
     * Enables the display of the control. Sets the control's 
     * window postion and visibility, but only if the value has
     * changed (so it's ok to call it excessively). 
     *
     * @param aEnable if ETrue, makes visible
     *                          if EFalse, makes non-visible
     *
     */
    void Enable(TBool aEnable);
	/**
     * set the number of selections that are visible
     *
     * @param aCount the number of visible selections, must be 
     *                          0 < aCount <= KMaxNumberOfSelections 
     */
    void SetVisibleSelectionCount(TInt aCount);
	/**
     * set the number of selections that are visible
     *
     * @param aCount the number of visible selections, must be 
     *                          0 < aCount <= KMaxNumberOfSelections 
     */
    void SetVisibleCount(TInt aCount);

    /**
     * get the current visible selection count
     *
     * @return visible selection count
     */
    TInt VisibleSelectionCount() const;


    /**
     * This sets the string of the internal buffer for the item index aIndex to aText,
     * even if the index falls outside the current visible candidates.
     *
     * @param aText new contents of text item
     * @return ETrue if successful
     *              EFalse if index outside visible candidates (but value is still set)
     *
     */
    TBool SetItemText(TInt aIndex, const TDesC& aText);

    /**
    *   Sets the string of the internal buffer for visible item 
    */
    TBool SetPhraseItemTexts();
    
    /**
     * This function gets the text at the index aIndex. 
     *
     * @param aIndex index of entry in window, zero based
     * @output aText contains contents of text item
     * @return ETrue always
     *
     */
    TBool GetItemText(TInt aIndex, TDes& aText) const;

    /**
     * This function sets the Tone Mark for all entries
     * 
     * @param aToneMark the tone mark
     *
     */
    void SetToneMark(const TDesC& aToneMark);

    /**
     * This function sets the Tone Mark for a specific entry
     * 
     * @param aIndex index of entry
     * @param aToneMark the tone mark
     *
     */
    void SetToneMark(TInt aIndex, const TDesC& aToneMark);

    /**
     * This function gets the Tone Mark for a specific entry
     * 
     * @param aIndex index of entry
     * @output aToneMark the tone mark
     *
     */
    void GetToneMark(TInt aIndex, TDes& aToneMark) const;

    /**
     * This function sets the validity of the tonemark
     *
     * @param aIndex index of the entry to be affected
     * @param aValid if ETrue, tonemark will be displayed as valid
     *                          if EFalse, tonemark will be displayed as not valid
     */
    void SetItemToneMarkValidity(TInt aIndex, TBool aValid);
    /**
     * Is the control enabled for display?
     *
     * @return if not enabled, return EFalse
     */
    TBool IsEnabled() const;

    /**
     * Select the next visible item. If already on the last one, loop to the 
     * first one.
     *
     * @return ETrue if success, 
     *              EFalse if no items visible, or if selection is outside visible items
     */
    TBool SelectNextPhrase();

    /**
     * Select the previous visible item. If already on the first one, loop to the 
     * last one.
     *
     * @return ETrue if success, 
     *              EFalse if no items visible, or if selection is outside visible items
     */
    TBool SelectPrevPhrase();

    /**
     * This function selects the item at index aIndex. 
     *
     * @param aIndex index of entry in window, zero based
     * @return ETrue if success, EFalse if failure.
     */
    TBool SelectIndex(TInt aIndex);

    /**
	*	Return Pinyin spelling candidate array 
	*/
    CDesCArrayFlat* PhoneticSpellingArray(void);

    /**
	*	Return Pinyin spelling candidate array 
	*/
    CDesCArrayFlat* PhoneticSpellingIndexArray();    
    /**
	*	Split phrase pinyin spelling candidates into pages 
	*/
    void SplitPhraseSpellingIntoPages(void);

    /**
	*	Split pinyin spelling candidates into pages 
	*/
    void SplitSpellingIntoPages(TInt aCount);
    
    /**
	*	Set display page for deliberate selection 
	*/
    void SetDisplayPage(TInt aSelection);
    /**
	*	Return current page start spelling index 
	*/
    TInt CurrentPageStartIndex(void);

    /**
	*	Return kestroke array 
	*/
    CDesCArrayFlat* KeystrokeArray(void);
    
    /**
	*	Return in effect kestroke array 
	*/    
    CDesCArrayFlat* InEffectKeystrokeArray();
    
    /**
	*	Return  show kestroke array 
	*/    
    CDesCArrayFlat* ShowKeystrokeArray();

    /**
	 *	Return Optimize Spelling array 
	 */    
    CDesCArrayFlat* OptimizeSpelling();
    
    /**
     *  Return temp Spelling array 
     */    
    CDesCArrayFlat* TempSpelling();
    
    /**
     *	Return choose chinese charater array 
     */
    CDesCArrayFlat* ChooseChineseCharacterArray();    

    /**
     *	Return choose chiese charater keystroke array 
     */
    CDesCArrayFlat* ChooseChineseCharacterArrayKeystroke();    
    
    /**
     *	Return the tone mark state 
     */
    TBool GetTonemarkState();   
    
    /**
     *	set the tone mark state 
     */
     void SetTonemarkState( TBool aState );    
     
    /**
    *   Set, clear and check flags status
    */
    inline void SetFlag(TInt aFlag);
    inline void ClearFlag(TInt aFlag);
    inline TBool IsFlagSet(TInt aFlag) const;
	void PopupSizeChanged();
	
    /**
     *  Set all the input pane not highlight
     */
	void SetNoneHighlighted();
	
    /**
    *   Return kestroke array 
    */
	CDesCArrayFlat* KeystrokeArrayForStroke();

    /**
    *   Return in effect kestroke array 
    */  
	CDesCArrayFlat* InEffectKeystrokeArrayForStroke();

    /**
    *   Return  show kestroke array 
    */ 
	CDesCArrayFlat* ShowKeystrokeArrayForStroke();

    /**
     *  Return choose chinese charater array 
     */
	CDesCArrayFlat* ChooseChineseCharacterArrayForStroke();

    /**
     *  Return choose chiese charater keystroke array 
     */
	CDesCArrayFlat* ChooseChineseCharacterArrayKeystrokeForStroke();
	
    CDesCArrayFlat* ZhuyinSymbols();
    
    CDesCArrayFlat* ZhuyinShowSymbols();
    
    CDesCArrayFlat* FirstValidZhuyinGrp() ;
    
    CDesCArrayFlat* ChineseCharsDisped() ;
    
    CDesCArrayFlat* ZhuyinSymbolsForChineseCharsDisped() ;
    
    void SetEEPContent ( const TDes& aDes );
    TDes& GetEEPContent ( );
    
    void SetEEPContentValid ( TBool aValid );
    TBool GetEEPContentValid ( );
    
    /**
     *  Reset all array 
     */
	void ResetStrokeArray();
	
    /**
     *  Set the state is changed 
     */
    void SetChangeState( TBool aChangeState );
    
    /**
     *  Return if has changed state or not 
     */
    TBool IsChangeStatet() const;
    
    /**
     * Set whether state change from entry to spelling editing is automatical.
     * 
     * @param aState.1 for automatical, 0 for manual.
     * @return None.
     */
    void SetState( TInt aState );
    
    /**
     * Get state change status.
     */
    TInt GetState();
    
    /**
     * Set whether state change from pinyin input state to pinyin phrase creation.
     * 
     * @param aState. ETrue for state change from pinyin input to phrase creation,
     *  EFalse is not.
     * @return None.
     */
    void SetPhraseCreationState( TBool aState );
    
    /**
     * Set whether state change from pinyin input state to pinyin phrase creation.
     * 
     * @return ETrue for state change from pinyin input to phrase creation,
     *  EFalse is not.
     */
    TBool GetPhraseCreationState();
    
    /**
     * Set whether at the editing state happen editing for pinyin phrase creation.
     * 
     * @param aState. ETrue happen editing,EFalse others.
     * @return None.
     */
    void SetPhraseCreationEditingState( TBool aState );
    
    /**
     * Get whether at the editing state happen editing for pinyin phrase creation.
     * 
     * @param aState. ETrue happen editing,EFalse others.
     * @return None.
     */
    TBool GetPhraseCreationEditingState();
    
    /**
     * Set flag whether from candidate state change state.
     * 
     * @param aFlag  ETrue from the candidate state change state, other EFalse.
     * @since S60 V5.0
     * @return none.
     */
    void SetFromCandidateChangeStateFlag( TBool aFlag );
    
    /**
     * Get flag whether from candidate state change state.
     * 
     * @since S60 V5.0
     * @return ETrue from the candidate state change state, other EFalse.
     */
    TBool GetFromCandidateChangeStateFlag();

    /**
     * Set count of valid keystroke.
     * 
     * @since S60 V5.0
     * @return none
     */
    void SetValidKeystrokeCount( TInt aCount );
    
    /**
     * get count of valid keystroke.
     * 
     * @since S60 V5.0
     * @return count of valid keystroke.
     */
    TInt GetValidKeystrokeCount();
    
    /**
     * Set last cba resouce id.
     * 
     * @since S60 V5.0
     * @return None
     */
    void SetLastResouce( TInt aResouceId );
    
    /**
     * Get last cba resouce id.
     * 
     * @since S60 V5.0
     * @return last resouce id
     */
    TInt GetLastResouce();
    
    
    /**
     * Set change state from Edit to Spelling flag.
     * 
     * @param aFlag,If ETrue Change from Edit state to spelling state,EFalse others. 
     * @since S60 V5.0
     * @return none.
     */
    void SetFromEditToSpellingFlag( TBool aFlag = EFalse );

    /**
     * Get change state from Edit to Spelling flag.
     * 
     * @param none. 
     * @since S60 V5.0
     * @return ETrue Change from Edit state to spelling state,EFalse others..
     */
    TInt GetFromEditToSpellingFlag();
    
    
    /**
     * Set valid keystroke change flag.
     * 
     * @param aFlag, ETrue valid keystroke changed,EFalse others.
     * @since S60 V5.0
     * @return none
     */
    void SetValidKeystrokeChange( TBool aFlag );
    
    /**
     * Get valid keystroke change flag.
     * 
     * @since S60 V5.0
     * @return ETrue,if vallid keystroke changed, others EFalse.
     */
    TBool GetValidKeystrokeChange();
    
    /**
     * Set PreStateIsPredict flag.
     * 
     * @param aFlag, ETrue PreStateIsPredict,EFalse others.
     * @since S60 V5.0
     * @return none
     */
    void SetPreStateIsPredictive( TBool aFlag );
    
    /**
     * Get PreStateIsPredict flag.
     * 
     * @since S60 V5.0
     * @return ETrue,if PreStateIsPredict, others EFalse.
     */
    TBool GetPreStateIsPredictive();

    void SetInputLayoutMiniQwertyZhuyin();
    
    void ResetInputLayoutMiniQwertyZhuyin();
    
    CDesCArrayFlat* ChooseChineseCharacterArraySpelling(void);

protected: // From CCoeControl
	TTypeUid::Ptr MopSupplyObject(TTypeUid aId);

protected:
    /**
     * constructor
     */
	CAknFepUICtrlPinyinPopup(CAknFepUiLayoutDataMgr* aLafDataMgr);
    /**
     * second phase construction
     */
    void ConstructL(RWindow& aParent);

    /**
     * layout the rectangles
     */
    void LayoutRects();

    /**
     * layout contained controls
     */
    void LayoutContainedControls();

    /**
     * Layout the input panes
     */
    void LayoutInputPanes();
 
	/**
     * set the visibility of the input panes, according to the current layout
     */
    void SetInputPaneVisibility();

    /**
     * layout the background frames
     */
    void LayoutFrames();

    /**
     * just layout one of the input panes
     *
     * @param aIndex index of input pane to lay out
     *
     */
    void LayoutInputPane(TInt aIndex);

    /**
     * update input panes' visibility
     */
    void UpdateInputPaneVisibility(void);

	
public: //from CcoeControl
     /**
     * size changed
     */
    virtual void SizeChanged();

    /**
     * count component controls
     *
     * @return number of contained controls
     */
    TInt CountComponentControls() const;

    /**
     * Component control
     *
     * @param aIndex the control to return, must be between 0 and the number of controls
     * @return pointer to contained control, ownership is not passed
     * 
     */
    CCoeControl* ComponentControl(TInt aIndex) const;
    /**
     * draw
     *
     * @param aRect rectangle
     *
     */
    virtual void Draw(const TRect& aRect) const;
    /**
     * layout the background frames
     *
     * @param aOuterRect filled with outer rect coords
     * @param aInnerRect filled with inner rect coords
     *
     */
    void CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const;
 
private: // the following need resetting
    RPointerArray<CAknFepUICtrlInputPane> iInputPaneArray;
private: // the following are not owned
    CAknFepUiLayoutDataMgr* iLafDataMgr;
    MAknFepUICtrlPinyinPopupSizeChangedObserver* iSizeChangedObserver;
private: // the following are owned
    CAknsFrameBackgroundControlContext* iBgContext; 

private:
    TInt iSelected;
    TInt iEnabled;
    TToneMark iToneMark;
   	TAknLayoutRect iRectCoverMainPaneInput;
    TAknLayoutRect iRectFirstShadowInput;
    TAknLayoutRect iRectOutlineFrameInput;
    TAknLayoutRect iRectInsideAreaInput;
	TInt iVisibleCount;
    RArray<TPage> iPages;
    TInt iCurDisplayPage;
    CDesCArrayFlat* iSpellingArray;
    //Spelling index array
    CDesCArrayFlat* iSpellingIndexArray;
    //keystroke arrray
    CDesCArrayFlat* iKeystrokeArray;
    //in effect keystroke array
    CDesCArrayFlat* iInEffectKeystrokeArray;
    //show Keystroke array
    CDesCArrayFlat* iShowKeystrokeArray;
    //Optimize Keystroke array
    CDesCArrayFlat* iOptimizeSpelling;
    //temp Keystroke array
    CDesCArrayFlat* iTempSpelling;
    //Chinese array
    CDesCArrayFlat* iChooseChineseCharacterArray;
    CDesCArrayFlat* iChooseChineseCharacterArraySpelling;
    //Chinese Keystroke array
    CDesCArrayFlat* iChooseChineseCharacterArrayKeystroke;
    
    CDesCArrayFlat* iKeystrokeArrayForStroke;
    CDesCArrayFlat* iInEffectKeystrokeArrayForStroke;
    CDesCArrayFlat* iShowKeystrokeArrayForStroke;
    CDesCArrayFlat* iChooseChineseCharacterArrayForStroke;
    CDesCArrayFlat* iChooseChineseCharacterArrayKeystrokeForStroke;

    CDesCArrayFlat* iZhuyinSymbols;
    CDesCArrayFlat* iZhuyinShowSymbols;
    CDesCArrayFlat* iFirstValidZhuyinGrp;
    CDesCArrayFlat* iChineseCharsDisped;
    CDesCArrayFlat* iZhuyinSymbolsForChineseCharsDisped;
    TBuf<100> iEEPContent;
    TBool iEEPContentValid;
    
    //the tone mark state
    TInt iTonemarkState;
    
    //the stroke state
    TInt iStrokeState;
    
    // The highlight goes back to wrong position when transfer from pinyin 
    // spelling pane to pinyin candidate pane more than one time.
    TInt iHighLightFlag;
    
    TInt iFlags;
    
    TInt iValidKeystrokeCount;
    TInt iPhraseCreation;
    TInt iPhraseCreationEditingState;
    TInt iChangeState;
    /**
     * whether the state change from entry to spelling editing is automatical.
     */
    TInt iState;
    
    TInt iLastResouce;
    
    /**
     * whether the state change from Edit to spelling.
     */
    TInt iChangeStateFromEditToSpelling;
    
    /**
     * whether the valid keystroke changed.
     */
    TInt iValidKeystrokeChanged;
    
    /**
     * whether the previous state is predictive display.
     */
    TBool iPreStateIsPredictive;
};

inline void CAknFepUICtrlPinyinPopup::SetFlag(TInt aFlag)
    {
    iFlags |= aFlag;
    }

inline void CAknFepUICtrlPinyinPopup::ClearFlag(TInt aFlag)
    {
    iFlags &= ~aFlag;
    }

inline TBool CAknFepUICtrlPinyinPopup::IsFlagSet(TInt aFlag) const
    {
    return iFlags & aFlag;
    }

#endif // __AKN_FEP_UI_AVKON_PINYIN_POPUP_H

// End of file
