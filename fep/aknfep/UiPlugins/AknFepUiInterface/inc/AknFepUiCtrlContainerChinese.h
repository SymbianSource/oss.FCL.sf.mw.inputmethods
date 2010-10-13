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











#ifndef __AKN_FEP_UI_CONTAINER_CHINESE_H__
#define __AKN_FEP_UI_CONTAINER_CHINESE_H__

#include <e32std.h>

class MAknFepUICtrlInputPane;
class MAknFepUICtrlCandidatePane;
class MAknFepUICtrlPinyinPopup;
class MAknFepUICtrlEditPane;
class MAknFepManagerUIInterface;

class MAknFepUICtrlContainerChinese
{

public:
    /**
     * Different layouts, the number represents the number of candidates visible in the candidate pane
     */
    enum TPaneLayout
        {
		ELayoutInput = 6,
		ELayoutCandidate = 7,
        // the follow 2 lines be added for phrase creation
		ELayoutPhraseCreation = 8,
		ELayoutKeystroke = 9
        };
public:
    /**
     * destructor
     */
    virtual ~MAknFepUICtrlContainerChinese() = 0;

    /**
     * get the input pane
     *
     * @return pointer to the input pane, ownership is not passed
     */
    virtual MAknFepUICtrlInputPane* InputPane() const = 0;

    /**
     * get the candidate pane
     *
     * @return pointer to the candidate pane, ownership is not passed
     */
    virtual MAknFepUICtrlCandidatePane* CandidatePane() const = 0;

    /**
     * get the phrase pinyin popup window 
     *
     * @return pointer to the phrase popup window, ownership is not passed
     */
    virtual MAknFepUICtrlPinyinPopup* PinyinPopupWindow() const = 0;

    /**
     * phrase creation's EEP Ctrl
     * get the pinyin phrase creation popup window 
     * 
     * @return pointer to the pinyin phrase popup window, ownership is not passed
    */
    virtual MAknFepUICtrlEditPane* EditPaneWindow() const = 0;

    /**
     * Is the control displayed on the screen?
     *
     * @return if not enabled, returns EFalse
     *
     */
    virtual TBool IsEnabled() const = 0;
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
     * Arrange the constituent panes into the correct proportions for 
     * the required layout. 
     *
     * @param aPaneLayout the required layout
     *
     */
    virtual void SetLayout(TPaneLayout aPaneLayout) = 0;
 

    /**
     * Set the Highlight to the desired control.
     *
     * @param aValue if aValue is ETrue set the focus to the Candidate Pane.
     *                          if aValue is EFalse set focus to the Chinese Input Pane 
     */
    virtual void FocusCandidatePane(TBool aValue) = 0;

    /**
     * enable/disable the rendering of the Horizontal Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *                          if EFalse, disable the arrrows
     */
    virtual void ShowHorizontalScrollArrows(TBool aValue) = 0;
    
    /**
     * enable/disable the rendering of the Vertical Scroll Arrows. 
     *
     * @param aValue if ETrue, enable the arrows
     *                          if EFalse, disable the arrrows
     */
    virtual void ShowVerticalScrollArrows(TBool aValue) = 0;

    /**
     * Call this method if the fep is being displayed for a control that is 
     * contained in a popup window (i.e. a Query). It changes the layout 
     * of the container.
     *
     * @param aValue if ETrue, use the layout for display with a query
     *                          if EFalse, use the normal layout
     */
    virtual void SetIsShownWithPopupWindows(TBool aValue) = 0;

     /**
     * This function sets cursor position.
     *
     * @param aCurPos containing the cursor position 
     * @param aHeight containing font height
     */
    virtual void SetContainerPosition(TPoint aCurPos,TInt aHeight) = 0;

    /**
     * Set Candidate and pinyin popup invisible(phrase creation)
     *
     */
    virtual void SetControlInVisible( TBool aVisible = ETrue ) = 0; 
    
    /**
     * Set the state to phrase creation(phrase creation)
     */
    virtual void PhraseCreation( TBool aPhraseCreation ) = 0;
    
    /**
     * Is the state is in phrase creation(phrase creation)
     *
     * @return if not phrase creation, return EFalse
     */
    virtual TInt& IsPhraseCreation() = 0;
    
    /**
     * Set FepMan(phrase creation)
     *
     * @return if not enabled, return EFalse
     */
    virtual void SetFepMan( MAknFepManagerUIInterface* aFepMan ) = 0;

	};
#endif  // __AKN_FEP_UI_CONTAINER_CHINESE_H__

// End of file

