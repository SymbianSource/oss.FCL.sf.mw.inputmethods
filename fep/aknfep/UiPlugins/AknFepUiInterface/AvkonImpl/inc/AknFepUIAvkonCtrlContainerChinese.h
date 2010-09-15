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
*       Provides the CAknFepUICtrlContainerChinese definition.
*
*/











#ifndef __AKN_FEP_UI_AVKON_CONTAINER_CHINESE_H__
#define __AKN_FEP_UI_AVKON_CONTAINER_CHINESE_H__

#include <AknFepUiCtrlContainerChinese.h>
#include <coecntrl.h>
#include <AknUtils.h>

class CAknsFrameBackgroundControlContext;
class CAknFepUICtrlInputPane;
class CAknFepUICtrlCandidatePane;
class CAknFepUICtrlPinyinPopup;
class CAknFepUICtrlPinyinEEP;
class MAknFepManagerUIInterface;
class CAknFepUiLayoutDataMgr;
class MCoeFocusObserver;
class MAknFepUiStateCtrl;

class MAknFepUIEventObserver
	{
public:
    /**
     * Click close pane to close
     */
	virtual void FepUIClosePressed() = 0;
	};

class MAknFepChineseUiMgr
	{
public:
    /**
     * Submit text
     * @param aText committed text
     */
	virtual void SubmitSelectedTextL( const TDesC& aText ) = 0;
	
    /**
     * Close container
     */
	virtual void CloseContainer() = 0;
	};

class MAknFepUICtrlPinyinPopupSizeChangedObserver
    {
public:
    /// this is called when the selection popup changes size
    virtual void SelectionPopupSizeChanged(TInt aNumLines) = 0;

    /// this is called when the popup is enabled
    virtual void SelectionPopupEnabled(TBool aEnabled) = 0;
    };

NONSHARABLE_CLASS(CAknFepUICtrlContainerChinese)
    : 
    public CCoeControl, 
    public MAknFepUICtrlPinyinPopupSizeChangedObserver,
    public MAknFepUICtrlContainerChinese,
    public MAknFepChineseUiMgr
{
public:
    /**
     * first phase construction
     */
    static CAknFepUICtrlContainerChinese* NewL();

    /**
     * destructor
     */
    ~CAknFepUICtrlContainerChinese();

    /**
     * get the input pane
     *
     * @return pointer to the input pane, ownership is not passed
     */
    MAknFepUICtrlInputPane* InputPane() const;

    /**
     * get the candidate pane
     *
     * @return pointer to the candidate pane, ownership is not passed
     */
    MAknFepUICtrlCandidatePane* CandidatePane() const;

    /**
     * get the phrase pinyin popup window 
     *
     * @return pointer to the phrase popup window, ownership is not passed
     */
    MAknFepUICtrlPinyinPopup* PinyinPopupWindow() const;

    /**
     * 
     * get the pinyin phrase creation popup window 
     * 
     * @return pointer to the pinyin phrase popup window, ownership is not passed
    */
    MAknFepUICtrlEditPane* EditPaneWindow() const;    
    
    /**
     * Is the control displayed on the screen?
     *
     * @return if not enabled, returns EFalse
     *
     */
    TBool IsEnabled() const;
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
     * Arrange the constituent panes into the correct proportions for 
     * the required layout. 
     *
     * @param aPaneLayout the required layout
     *
     */
    void SetLayout(TPaneLayout aPaneLayout);
 

    /**
     * Set the Highlight to the desired control.
     *
     * @param aValue if aValue is ETrue set the focus to the Candidate Pane.
     *                          if aValue is EFalse set focus to the Chinese Input Pane 
     */
    void FocusCandidatePane(TBool aValue);

    /**
     * enable/disable the rendering of the Horizontal Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *                          if EFalse, disable the arrrows
     */
    void ShowHorizontalScrollArrows(TBool aValue);
    
    /**
     * enable/disable the rendering of the Vertical Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *                          if EFalse, disable the arrrows
     */
    void ShowVerticalScrollArrows(TBool aValue);

    /**
     * Call this method if the fep is being displayed for a control that is 
     * contained in a popup window (i.e. a Query). It changes the layout 
     * of the container.
     *
     * @param aValue if ETrue, use the layout for display with a query
     *                          if EFalse, use the normal layout
     */
    void SetIsShownWithPopupWindows(TBool aValue);

     /**
     * This function sets cursor position.
     *
     * @param aCurPos containing the cursor position 
     * @param aHeight containing font height
     */
    void SetContainerPosition(TPoint aCurPos,TInt aHeight);
    
    /**
     * Set Candidate and pinyin popup invisible
     *
     */
    void SetControlInVisible( TBool aVisible ); 
    
    /**
     * Set the state to phrase creation
     */
    void PhraseCreation( TBool aPhraseCreation );
    
    /**
     * Is the state is in phrase creation
     *
     * @return if not enabled, return EFalse
     */
    TInt& IsPhraseCreation();
    
    /**
     * Set FepMan
     *
     * @return if not enabled, return EFalse
     */
    void SetFepMan( MAknFepManagerUIInterface* aFepMan );
    
    /**
     * Handle pointer event
     * @param aPointerEvent pointer event
     * 
     */
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    
    /**
     * Set observer
     * @param aObserver Obsever object
     * 
     */
    void SetFepUiStateCtrl( MAknFepUiStateCtrl* aFepUiState );
        
    /**
     * Submit text
     * @param aText committed text
     */
    void SubmitSelectedTextL(const TDesC& aText);
    
    /**
     * Close container
     */
    void CloseContainer();	

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

    virtual void HandleResourceChange(TInt aType); 
   
	
protected: // From CCoeControl
    TTypeUid::Ptr MopSupplyObject(TTypeUid aId);

public: // from MAknFepUICtrlPinyinPopupSizeChangedObserver

    /**
     * called when the popup window changes size, so that it can be 
     * laid out correctly
     *
     * @param aNumLines the number of visible lines
     */
    void SelectionPopupSizeChanged(TInt aNumLines);

    /**
     * the popup has been enabled or disabled, so change layout where necessary 
     *
     * @param aEnabled ETrue if popup has been enabled
     *                              EFalse if popup has been disabled
     */
    void SelectionPopupEnabled(TBool aEnabled);
protected:

    /**
     * second phase construction
     */
    void ConstructL();

    /**
     * constructor
     */
    CAknFepUICtrlContainerChinese();

    /**
     * layout this control relative to the control pane (aka the CBA).
     */
    void LayoutContainer();
    /**
     * layout the rectangles
     */
    void LayoutRects();
	/**
     * layout entry pane
     */
	void LayoutInputPane();
    /**
     * layout contained controls
     */
    void LayoutContainedControls();

    /**
     * layout the popup window
     */
    void LayoutPinyinPopupWindow();
    
    /**
     * layout the EEP window
     */
    void LayoutPinyinEEPWindow();

    /**
     * set control vilisibilty, according to the current pane layout
     */
    void SetControlVisibility();

    /**
     * return the layout as an index that can be passed to the 
     * layout dll 
     *
     * @return index into layout macro
     */
    TInt ConvertLayoutToIndex() const;

    /**
     * layout the background frames
     */
	void LayoutFrames();

    /**
     * layout the background frames
	 *
	 * @param aOuterRect filled with outer rect coords
	 * @param aInnerRect filled with inner rect coords
	 *
     */
	void CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const;    

    /**
     * Close UI
	 *
     */
	void CloseUI();
	
private:
    CAknFepUiLayoutDataMgr* iLafDataMgr;
    CAknFepUICtrlInputPane* iInputPane;
    CAknFepUICtrlCandidatePane* iCandidatePane;
	CAknFepUICtrlPinyinPopup* iPinyinPopupWindow;
	CAknFepUICtrlPinyinEEP* iEditPane;
	CAknsFrameBackgroundControlContext* iBgContext; 
	
	

private:
	TBool iEnabled;
	TPaneLayout iPaneLayout;
    TBool iIsShownWithPopupWindows;
	// input pane
	TAknLayoutRect iRectCoverMainPaneInput;
    TAknLayoutRect iRectFirstShadowInput;
    TAknLayoutRect iRectOutlineFrameInput;
    TAknLayoutRect iRectInsideAreaInput;
    TAknLayoutRect iHorizLine; //line between input pane and candidate pane
    TAknLayoutRect iEEPFirstHorizLine; //line between input pane and EEP pane
    TAknLayoutRect iEEPSecondHorizLine;////line between input pane and candidate pane when EEP exist
    TPoint iContainerPosition;
    TBool isPCLPosed;
    TBool iPhraseCreationLaf;
    
    TInt iPhraseCreation;
    MAknFepManagerUIInterface* iFepMan;
    MAknFepUiStateCtrl* iFepUiState;
};
#endif  // __AKN_FEP_UI_AVKON_CONTAINER_CHINESE_H__

// End of file

