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
*       Provides the CAknFepUICtrlCandidatePane definition.
*
*/












#ifndef __AKN_FEP_AVKON_CANDIDATE_PANE_H__
#define __AKN_FEP_AVKON_CANDIDATE_PANE_H__

#include <e32std.h>
#include <coecntrl.h>

#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUIAvkonCtrlContainerChinese.h"

class CAknFepUICtrlCandidateSelected;

/**
 * Pane which displays the Chinese Candidates.
 */
NONSHARABLE_CLASS(CAknFepUICtrlCandidatePane) : public CCoeControl, public MAknFepUICtrlCandidatePane
    {
public:
    /**
     * first phase construction
     */
    static CAknFepUICtrlCandidatePane* NewL(RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr);
    /**
     * destructor
     */
    ~CAknFepUICtrlCandidatePane();
public:
    /**
     * Get a buffer that can be filled with candidate characters, 
     * which will subsequently be displayed.
     *
     * @output the text buffer of the Pane
     */
    const TDesC& TextBuffer() const;

    /**
     * Allows the client to set a descriptor pointer into an existing buffer of 
     * data, avoiding unnecessary copying of data.
     * <p> Note that if the buffer contains less characters than the current
     * visible count, then the remaining characters will not be displayed. 
     * <p> Note also that if the current selection falls outside the range
     * of the size of the new buffer, it will be made not visible. 
     * However, the current highlighted state remains, so if the selection 
     * is set once more to a valid entry (or if the buffer is set to a length that 
     * includes the currently selected position), and the highlight is still enabled,
     * the highlight will reappear.
     *
     * @param aBufferPtr descriptor that contains the candidates for display,
     *                              could be a pointer into a subset of a larger descriptor 
     *                              that contains all the candidates. Note that only the 
     *                              first EMaxCandidates of aBuffer will be used. 
     *                              Length must be > 0.
     */
    void SetCandidateBuffer(const TDesC& aBuffer);

    /**
     * This function sets the highlighted status of the pane for rendering purposes. 
     * If the Pane is highlighted it displays the current selected Candidate, 
     * plus numbers with each of the other Candidates. 
     * 
     * @param aHighlighted ETrue if the highlight is to be set
     *                                  EFalse if the highlight is to be removed
     */
    void SetHighlighted(TBool aHighlighted);

    /**
     * This function returns the highlighted status of the Pane.
     *
     * @return ETrue if highlight was set
     *              EFalse if highlight was not set
     *
     */
    TBool Highlighted() const;

    /**
     * Set the candidate mode for the specified candidate. This is used for 
     * setting the colour of the font for components in stroke mode
     *
     * @param aMode the new mode
     */
    void SetCandidateMode(TInt aIndex, TCandidateMode aMode);

    /**
     * Get the candidate mode for the specified candidate.
     *
     * @return aMode the new mode
     */
    TCandidateMode CandidateMode(TInt aIndex);

    /**
     * are the Horizontal Scroll Arrows enabled?
     *
     * @return if arrows not enabled, returns EFalse
     */
    TBool IsShowHorizontalScrollArrows() const;

    /**
     * enable/disable the rendering of the Horizontal Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *                          if EFalse, disable the arrrows
     */
    void ShowHorizontalScrollArrows(TBool aValue);


    /**
     * enable/disable the rendering of the up Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *  if EFalse, disable the arrrows
     */
    void ShowUpScrollArrows(TBool aValue);

    /**
     * enable/disable the rendering of the down Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *  if EFalse, disable the arrrows
     */
    void ShowDownScrollArrows(TBool aValue);
    
     /**
     * enable/disable the rendering of the left Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *  if EFalse, disable the arrrows
     */
    void ShowLeftScrollArrows(TBool aValue);
    
        /**
     * enable/disable the rendering of the right Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *  if EFalse, disable the arrrows
     */
    void ShowRightScrollArrows(TBool aValue);

    /**
     * are the Vertical Scroll Arrows enabled?
     *
     * @return if arrows not enabled, returns EFalse
     */
    TBool IsShowVerticalScrollArrows() const;

    /**
     * enable/disable the rendering of the Vertical Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *                          if EFalse, disable the arrrows
     */
    void ShowVerticalScrollArrows(TBool aValue);

    /**
     * enable/disable the rendering of the candidate ordinal numbers
     *
     * @param aValue if ETrue, enable the ordinals
     *                          if EFalse, disable the ordinals
     *
     */
    void ShowCandidateOrdinals(TBool aValue);

    /**
     * enable/disable the rendering of the candidate ordinal numbers
     *
     * @return if not shown, return EFalse
     */
    TBool IsShowCandidateOrdinals() const;

    /**
     * This function returns the number of Candidates that are visible in the pane 
     * in its current configuration.
     *
     * @return number of candidates that are currently visible
     */
    TInt VisibleCandidateCount() const;

    /**
     * This function sets the number of candidates that are visible. 
     * This will affect the size of the control as determined by the layout. 
     *
     * @param aCount the new visible count, 
     *              must be 1 <= aCount <= EMaxCandidates
     */
    void SetVisibleCandidateCount(TInt aCount);

    /**
     * Select the first visible item in the candidate buffer
     *
     * @return ETrue if success, EFalse if no items visible
     *
     */
    TBool SelectFirst();

    /**
     * Select the first visible phrase item in the candidate buffer
     *
     * @return ETrue if success, EFalse if no items visible
     *
     */
    TBool SelectFirstPhrase();
    
    /**
     * Select the last visible item in the candidate buffer
     *
     * @return ETrue if success, EFalse if no items visible
     *
     */
    TBool SelectLast();

    /**
     * Select the last visible phrase item in the candidate buffer
     *
     * @return ETrue if success, EFalse if no items visible
     *
     */
    TBool SelectLastPhrase();

    /**
     * Select the next visible item
     *
     * @return ETrue if success, 
     *              EFalse if no items visible, or if already on last visible item
     */
    TBool SelectNext();
    /**
     * Select the previous visible item
     *
     * @return ETrue if success, 
     *              EFalse if no items visible, or if already on first visible item
     */
    TBool SelectPrev();

    /**
     * This function selects the given index. 
     * <p> Note that if the new selected index is within the visible count, 
     * but falls on a candidate that is not valid (i.e. if it falls outside the
     * length of the candidate buffer) then nothing happens and EFalse is returned
     *
     * @param aIndex If aIndex falls outside length of candidate buffer, 
     *                          then return EFalse to indicate failure. 
     * @return ETrue if index was valid, EFalse if index not valid
     */
    TBool SelectIndex(TInt aIndex, TBool selected = ETrue);

    /**
     * This function returns the selected index of the Candidate Pane. 
     * 
     * @return the index of the selected item. If the Pane is not highlighted it returns 0.
     *
     */
    TInt SelectedIndex() const;

    /**
     * This function returns the character at index of the current selection
     *
     * @return a descrptor containing the character that is currently selected.
     *              If the currect selection falls outside the length of the current buffer,
     *              then return a null descriptor
     *
     */
    TPtrC CurrentCandidate() const;
    /**
     * This function returns the phrase at index of the current selection
     *
     * @return a descrptor containing the character that is currently selected.
     *              If the currect selection falls outside the length of the current buffer,
     *              then return a null descriptor
     *
     */
    TPtrC CurrentPhraseCandidate() const;
	/**
	*	This  function calculate the input pane's size for phrase input layout
	*   @return input pane's size
	*   
	*/
	TInt LabelTextWidthInPixels(TInt aIndex);
	/**
	*	Set layout 
	*   @param aPaneLayout the new panelayout
	*   
	*/
	void SetPaneLayout(CAknFepUICtrlContainerChinese::TPaneLayout aPaneLayout);
    /**
	*	Split phrase candidates into pages 
	*/
    void SplitPhraseCandidatesIntoPages();
  
    /**
	*	Navigate to next page 
	*/
    void NextCandidatePage(void);
   
    /**
	*	Navigate to previous page 
	*/
    void PreviousCandidatePage(void);

    /**
	*	Return candidate array 
	*/
    CDesCArrayFlat* CandidateArray(void);

    /**
	*	Set phrase candidate buffer
	*/
    void SetCandidateBuffer(void);
	
	/**
	*	Reset candidate pane state and variables
	*/
	void ResetCandidateState();
	
	/**
	 *	To test whether current page is the first page
	 */
	TBool IsFirstPage();
	
	/**
	 *	To test whether current page is the last page
	 */
	TBool IsLastPage();
	
	/**
     *  To set all the cnadidate label invisible(phrase creation)
     */
    void SetInVisiable();
	/**
	 *	To test whether need to get new candidate page
	 */
	TBool NeedGetNewCandidatePage();
	
public: // from CCoeControl
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
    
    
    void Enable(TBool aEnable);

protected:
    /**
     * second phase construction
     */
    void ConstructL(RWindow& aParent);

    /**
     * Construct all the bitmap classes
     */
    void ConstructBitmapsL();

    /**
     * constructor
     */
    CAknFepUICtrlCandidatePane( CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * layout the rectangles
     */
    void LayoutRects();

    /**
     * layout contained controls
     */
    void LayoutContainedControls();

    /**
     * layout the labels for a single candidate
     */
    void LayoutLabel(TInt aIndex);

    /**
     * layout the labels for all the candidates (even those that are not currently visible)
     */
    void LayoutLabels();

    /**
     * layout selected candidate
     * <p> Note that there is only one instance, as there can 
     * only be one selected candidate at a time. Therefore, the control
     * is placed in the position that is correct based on the current selection
     *
     * @param aIndex the index of the new position of the selected candidate
     *
     */
    void LayoutCandidateSelected();

    /**
     * set label vilisibilty, according to the current pane layout
     */
    void UpdateLabelVisibility();

    /**
     * draw background color 
     *
     * @param aRect rect in which to draw
     */
    void DrawBackground(const TRect& aRect) const;

    /**
     * Internal candidate paging function,
     * Only paging once per call.
     * 
     */
    void InternalSpilitPhraseCandidate();

private: // the following are owned
    CAknFepUICtrlCandidateSelected* iCandidateSelected;
    CFbsBitmap* iNaviArrowBitmapLeft;
    CFbsBitmap* iNaviArrowBitmapLeftMask;
    CFbsBitmap* iNaviArrowBitmapRight;
    CFbsBitmap* iNaviArrowBitmapRightMask;
    CFbsBitmap* iNaviArrowBitmapUp;
    CFbsBitmap* iNaviArrowBitmapUpMask;
    CFbsBitmap* iNaviArrowBitmapDown;
    CFbsBitmap* iNaviArrowBitmapDownMask;
private: // the following need resetting
    RPointerArray<CEikLabel> iCandidateLabelArray;
    RPointerArray<CEikLabel> iOrdinalLabelArray;
    RArray<TCandidateMode> iCandidateModeArray;
private: // the following are members
    CAknFepUiLayoutDataMgr* iLafDataMgr;
    TAknLayoutRect iIndiFepArrowLeft;
    TAknLayoutRect iIndiFepArrowRight;
    TAknLayoutRect iIndiFepArrowUp;
    TAknLayoutRect iIndiFepArrowDown;
    TBufC<EMaxCandidates> iBuffer;
    CAknFepUICtrlContainerChinese::TPaneLayout iPaneLayout;
    TInt iVisibleCount;
    TInt iSelected;
    TBool iHighlighted;
    TBool iShowHorizontalScrollArrows;
    TBool iShowVerticalScrollArrows;
    TBool iShowUpScrollArrows;
    TBool iShowDownScrollArrows;
    TBool iShowLeftScrollArrows;
    TBool iShowRightScrollArrows;
    TBool iShowCandidateOrdinals;
    RArray<TPage> iPages;
    //next paging candidate index
    TInt iUnpagedIndex;
    TInt iCurDisplayPage;
    CDesCArrayFlat* iCandidateArray;
    TInt iOrdinalLableWidth;
    };

#endif //__AKN_FEP_AVKON_CANDIDATE_PANE_H__

// End of file