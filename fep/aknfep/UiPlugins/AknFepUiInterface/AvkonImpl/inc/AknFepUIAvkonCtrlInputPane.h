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












#ifndef __AKN_FEP_AVKON_INPUT_PANE_H__
#define __AKN_FEP_AVKON_INPUT_PANE_H__

#include <AknFepUICtrlInputPane.h>
#include <coecntrl.h>
#include <AknUtils.h>

#include "AknFepUIAvkonCtrlContainerChinese.h"
class CEikLabel;
class CAknsFrameBackgroundControlContext;


/**
 * pane in which the user's input is shown
 */
NONSHARABLE_CLASS(CAknFepUICtrlInputPane) : public CCoeControl, public MAknFepUICtrlInputPane
    {
public:
    /**
     * first phase construction
     *
     * @param aParent parent window
     * @param aPaneLayout the required layout
     *
     */
    static CAknFepUICtrlInputPane* NewL(RWindow& aParent, 
                                        TInputPaneLayout aPaneLayout,
                                        CAknFepUiLayoutDataMgr* aLafDataMgr);

    /**
     * destructor
     */
    ~CAknFepUICtrlInputPane();
	
	 /**
     * set the observer
     * <p> Note that only one observer is supported!
     *
     * @param aObserver pointer to the observer
     */

    void SetSizeChangedObserver(MAknFepUICtrlPinyinPopupSizeChangedObserver* aObserver);

    /**
     * Set the text buffer of the pane.
     *
     * @param aDes, the label will be filled with the first N characters from aDes,
     *                              where N=EMaxInputCharsNotIncludingToneMark
     *
     */
    void SetText(const TDesC& aDes);

    /**
     * Get the text buffer of the pane
     *
     * @output aText the buffer is filled with the contents of the candidate
     *              of the pane, up to the size of the buffer
     */
    void GetText(TDes& aText) const;

    /**
     * This function sets the highlighted status of the pane for rendering purposes.
     *
     * @param aHighlighted whether or not the input pane has the highlight
     */
    void SetHighlighted(TBool aHighlighted);

    /**
     * This function returns the highlighted status of the Pane.
     *
     * @return whether the pane is highlighted
     */
    TBool Highlighted() const;

    /**
     * This function sets the Tone Mark for the Pane.
     *
     * @param aToneMark descriptor containing the tone mark character
     */
    void SetToneMark(const TDesC& aToneMark);

    /**
     * This function gets the Tone Mark for the Pane.
     *
     * @output aToneMark descriptor containing the tone mark character
     */
    void GetToneMark(TDes& aToneMark) const;

    /**
     * This function sets the validity of the tonemark
     *
     * @param aValid if ETrue, tonemark will be displayed as valid
     *                          if EFalse, tonemark will be displayed as not valid
     */
    void SetToneMarkValidity(TBool aValid);

    /**
     * sets the layout index to use when accessing the 
     * layout dll
     */
    void SetLayoutIndex(TInt aIndex);

    /**
     * sets the font to be used, which will override the font from LAF
     *
     * @param aFontId the new font id, pass in 0 to use the one from LAF
     *
     */
    void SetOverrideFontId(TInt aFontId);

    /**
     * This function determines whether the current text is shown or not
     *
     * @param aValid if ETrue, text will be displayed
     *                          if EFalse, text will be displayed
     */
    void MakeTextVisible(TBool aVisible);
	/**
	*	This  function calculate the input pane's size for phrase input layout
	*   @return input pane's size
	*   
	*/
	TInt LabelTextWidthInPixels() const;

    /**
    * Set,clear and check phonetic change status (for phrase input)
    */
    inline void SetFlag(TInt aFlag);
    inline void ClearFlag(TInt aFlag);
    inline TBool IsFlagSet(TInt aFlag) const;
    
    void SetLayout( TInputPaneLayout aPaneLayout );
    

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

protected:

    /**
     * second phase construction
     *
     * @param aParent parent window
     */
    void ConstructL(RWindow& aParent);

     /**
      * constructor
      *
      * @param aPaneLayout the required layout
      */
    CAknFepUICtrlInputPane(TInputPaneLayout aPaneLayout, CAknFepUiLayoutDataMgr* aLafDataMgr);

    /**
     * Update the contents of the label, by combining
     * the buffer with the tone mark
     */
    void UpdateLabelContents();

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
	
private:

    /**
     * Get dispalying input text
     *
     * @param aDes Input text
     */
    TInt CalculateMaxTextLength(const TDesC& aDes) const;
    
    /**
     * layout input text
     *
     * @param aDes Input text
     */
    TInt CalculateTextLength(const TDesC& aDes) const;
	
private:
    TBool iHighlighted;
    TInputPaneLayout iLayout;
    TInt iLayoutIndex;
    TInt iMaxInputChars;
    TAknLayoutRect iUnselectedStringHighlight;
    TBufC<EMaxToneMarkLength> iToneMarkBuffer;
    TBool iToneMarkValidity;
    TInt iFontId;
    TInt iFlags;
private: // the following are owned
    HBufC* iBuffer;
    CEikLabel* iLabel;
    CEikLabel* iLabelToneMark;

private: // the following are not owned
    CAknFepUiLayoutDataMgr* iLafDataMgr;
    MAknFepUICtrlPinyinPopupSizeChangedObserver* iSizeChangedObserver;
    };

inline void CAknFepUICtrlInputPane::SetFlag(TInt aFlag)
    {
    iFlags |= aFlag;
    }

inline void CAknFepUICtrlInputPane::ClearFlag(TInt aFlag)
    {
    iFlags &= ~aFlag;
    }

inline TBool CAknFepUICtrlInputPane::IsFlagSet(TInt aFlag) const
    {
    return iFlags & aFlag;
    }

#endif //__AKN_FEP_AVKON_INPUT_PANE_H__

// End of file
