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
*       Provides the CAknFepUICtrlCandidateSelected definition.
*
*/












#ifndef __AKN_FEP_CANDIDATE_SELECTED_PANE_H__
#define __AKN_FEP_CANDIDATE_SELECTED_PANE_H__

#include <e32std.h>
#include <coecntrl.h>

#include "AknFepUICtrlCandidatePane.h"

class CEikLabel;

/**
 * Control representing the selected candidate. Is a window owning control,
 * that appears over the candidate pane such that it obscures the labels
 * beneath. Can be hidden, for example if there is no visible selection, 
 * or if highlighting is disabled.
 */
class CAknFepUICtrlCandidateSelected : public CCoeControl
    {
public:
    enum
        {
		EOneCandidate = 1,
        EMaxSelectedCandidateLength = 10 // extend buffer for phrase input
        };
public:

    /**
     * first phase construction
     */
    static CAknFepUICtrlCandidateSelected* NewL(RWindowTreeNode& aParent);

    /**
     * destructor
     */
    ~CAknFepUICtrlCandidateSelected();

    /**
     * Set the text buffer of the Pane.
     *
     * @param aCharacter, the label will be filled with the characters from aCharacter
     */
    void SetText(TPtrC aCharacter);
    
    /**
     * Set the ordinal of the Pane.
     *
     * @param aOrdinal
     */
    void SetOrdinal(TInt aOrdinal);

    /**
     * This function sets the candidate mode of the pane for rendering purposes.
     *
     * @param aMode the new mode
     */
    void SetCandidateMode(CAknFepUICtrlCandidatePane::TCandidateMode aMode);

	/**
	* Set layout
	* @param aLayout the new layout
	*/
	void SetLayout(CAknFepUICtrlContainerChinese::TPaneLayout aLayout);
	/**
	* Set ordinal status
	* @param aValue the new status
	*/
	void ShowOrdinal(TBool aValue);

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
     */
    CCoeControl* ComponentControl(TInt aIndex) const;

    /**
     * draw
     *
     * @param aRect rectangle
     */
    virtual void Draw(const TRect& aRect) const;

protected:
    /**
     * second phase construction
     */
    void ConstructL(RWindowTreeNode& aParent);

     /**
      * constructor
      */
    CAknFepUICtrlCandidateSelected();

    /**
     * layout the rectangles
     */
    void LayoutRects();

    /**
     * layout contained controls
     */
    void LayoutContainedControls();

    /**
     * layout the background frames
     *
     * @param aOuterRect filled with outer rect coords
     * @param aInnerRect filled with inner rect coords
     *
     */
    void CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const;

    /**
     * set label vilisibilty, according to the current pane layout
     */
    void UpdateLabelVisibility();

private:
    TAknLayoutRect iRectShadow;
    TAknLayoutRect iRectHighlight;
	TBufC<EMaxSelectedCandidateLength> iBuffer;
    CAknFepUICtrlCandidatePane::TCandidateMode iCandidateMode;
    TInt iIndex;
    CAknFepUICtrlContainerChinese::TPaneLayout iPaneLayout;
	TBool iShowOrdinal;
private: // the following are owned
    CEikLabel* iCandidateLabel;
    CEikLabel* iOrdinalLabel;
    };

#endif //__AKN_FEP_CANDIDATE_SELECTED_PANE_H__

// End of file
