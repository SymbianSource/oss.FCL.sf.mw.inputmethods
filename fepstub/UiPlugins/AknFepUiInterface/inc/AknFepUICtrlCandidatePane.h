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












#ifndef __AKN_FEP_CANDIDATE_PANE_H__
#define __AKN_FEP_CANDIDATE_PANE_H__

#include <e32std.h>
#include <coecntrl.h>

#include "AknFepUiCtrlContainerChinese.h"

/**
* TPage holds the start index, end index and start position in buffer of every display page
*/
struct TPage
    {
    TInt iStartIndex;
    TInt iEndIndex;
    };

/**
 * Pane which displays the Chinese Candidates.
 */
class MAknFepUICtrlCandidatePane
    {
public:
    enum
        {
        EOneCandidate = 1,
        EMaxCandidates = 10,
        EMaxPages = 10,
        EPhraseLength = 10
        };
    /**
     * each candidate can be in a certain mode, which may affect its layout. Note
     * that only stroke is needed at this time (for components), but more modes could be added 
     * later if the LAF spec changes.
     */
    enum TCandidateMode
        {
        ECandidateModeNormal,
        ECandidateModeStroke
        };
public:
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
    virtual void SetCandidateBuffer(const TDesC& aBuffer) = 0;

    /**
     * This function sets the highlighted status of the pane for rendering purposes. 
     * If the Pane is highlighted it displays the current selected Candidate, 
     * plus numbers with each of the other Candidates. 
     * 
     * @param aHighlighted ETrue if the highlight is to be set
     *                                  EFalse if the highlight is to be removed
     */
    virtual void SetHighlighted(TBool aHighlighted) = 0;

    /**
     * enable/disable the rendering of the Horizontal Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *                          if EFalse, disable the arrrows
     */
    virtual void ShowHorizontalScrollArrows(TBool aValue) = 0;


    /**
     * enable/disable the rendering of the up Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *  if EFalse, disable the arrrows
     */
    virtual void ShowUpScrollArrows(TBool aValue) = 0;

    /**
     * enable/disable the rendering of the down Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *  if EFalse, disable the arrrows
     */
    virtual void ShowDownScrollArrows(TBool aValue) = 0;
    
     /**
     * enable/disable the rendering of the left Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *  if EFalse, disable the arrrows
     */
    virtual void ShowLeftScrollArrows(TBool aValue) = 0;
    
        /**
     * enable/disable the rendering of the right Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *  if EFalse, disable the arrrows
     */
    virtual void ShowRightScrollArrows(TBool aValue) = 0;

    /**
     * enable/disable the rendering of the Vertical Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *                          if EFalse, disable the arrrows
     */
    virtual void ShowVerticalScrollArrows(TBool aValue) = 0;

    /**
     * enable/disable the rendering of the candidate ordinal numbers
     *
     * @param aValue if ETrue, enable the ordinals
     *                          if EFalse, disable the ordinals
     *
     */
    virtual void ShowCandidateOrdinals(TBool aValue) = 0;

    /**
     * This function returns the number of Candidates that are visible in the pane 
     * in its current configuration.
     *
     * @return number of candidates that are currently visible
     */
    virtual TInt VisibleCandidateCount() const = 0;

    /**
     * Select the first visible item in the candidate buffer
     *
     * @return ETrue if success, EFalse if no items visible
     *
     */
    virtual TBool SelectFirst() = 0;

    /**
     * Select the first visible phrase item in the candidate buffer
     *
     * @return ETrue if success, EFalse if no items visible
     *
     */
    virtual TBool SelectFirstPhrase() = 0;
    
    /**
     * Select the last visible item in the candidate buffer
     *
     * @return ETrue if success, EFalse if no items visible
     *
     */
    virtual TBool SelectLast() = 0;

    /**
     * Select the last visible phrase item in the candidate buffer
     *
     * @return ETrue if success, EFalse if no items visible
     *
     */
    virtual TBool SelectLastPhrase() = 0;

    /**
     * Select the next visible item
     *
     * @return ETrue if success, 
     *              EFalse if no items visible, or if already on last visible item
     */
    virtual TBool SelectNext() = 0;
    /**
     * Select the previous visible item
     *
     * @return ETrue if success, 
     *              EFalse if no items visible, or if already on first visible item
     */
    virtual TBool SelectPrev() = 0;

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
    virtual TBool SelectIndex(TInt aIndex, TBool selected = ETrue) = 0;

    /**
     * This function returns the selected index of the Candidate Pane. 
     * 
     * @return the index of the selected item. If the Pane is not highlighted it returns 0.
     *
     */
    virtual TInt SelectedIndex() const = 0;
    
    /**
     * This function returns the character at index of the current selection
     *
     * @return a descrptor containing the character that is currently selected.
     *              If the currect selection falls outside the length of the current buffer,
     *              then return a null descriptor
     *
     */
    virtual TPtrC CurrentCandidate() const = 0;
    /**
     * This function returns the phrase at index of the current selection
     *
     * @return a descrptor containing the character that is currently selected.
     *              If the currect selection falls outside the length of the current buffer,
     *              then return a null descriptor
     *
     */
    virtual TPtrC CurrentPhraseCandidate() const = 0;
    /**
	*	Split phrase candidates into pages 
	*/
    virtual void SplitPhraseCandidatesIntoPages() = 0;
  
    /**
	*	Navigate to next page 
	*/
    virtual void NextCandidatePage(void) = 0;
   
    /**
	*	Navigate to previous page 
	*/
    virtual void PreviousCandidatePage(void) = 0;

    /**
	*	Return candidate array 
	*/
    virtual CDesCArrayFlat* CandidateArray(void) = 0;

    /**
	*	Set phrase candidate buffer
	*/
    virtual void SetCandidateBuffer(void) = 0;

	/**
	*	Reset candidate pane state and variables
	*/
	virtual void ResetCandidateState() = 0;
	
	/**
	 *	To test whether current page is the first page
	 */
	virtual TBool IsFirstPage() = 0;
	
	/**
	 *	To test whether current page is the last page
	 */
	virtual TBool IsLastPage() = 0;

   /**
     *  To set all the cnadidate label invisible(phrase creation)
     */
    virtual void SetInVisiable() = 0;

	/**
	 *	To test whether need to get new candidate page
	 */
	virtual TBool NeedGetNewCandidatePage() = 0;
    };

#endif //__AKN_FEP_CANDIDATE_PANE_H__

// End of file
