/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:             A non-focusing popup menu to show candidates.
*
*/












#ifndef __AKN_FEP_CANDIDATE_POPUP_H
#define __AKN_FEP_CANDIDATE_POPUP_H

// MACROS
//To congigure the number of candidates in list
const TInt KPredictiveListCandidateMax = 16; 

// CLASS DECLARATION
class MAknFepCandidatePopup
    {
public: // methods
	
	/**
	 * destructor
	 */
    virtual ~MAknFepCandidatePopup() = 0;

    /**
    * Getter for the currently active word
    * @return   The currently active word as pointer descriptor
    */
    virtual TPtrC ActiveWord() const = 0;
    
    /**
    * Getter for the exact word (the word with ordinal 0)
    * @return   The exact word as pointer descriptor.
    */
    virtual TPtrC ExactWord() const = 0;
    
    /**
    * Makes the pop-up selection list visible and blocks until the dialog has been dismissed.
    * @param    aInlineEditorRect   Tells the place of the inline editor. If possible, the popup is opened
    *                               so that it does not hide the inline editor.
    * @param    aSelectedIdx        Output argument for the selected index.
    * @param    aLastKeyEvent       The last key event received by the popup. On return this is the event
    *                               which closed the popup.
    * @param    aRightToLeftLanguage Direction of the candidate language.ETrue is right to left. EFalse otherwise
    * @param    aKeyboard           Keyboard type in use (TPtiKeyboardType)
    *
    * @return   The command id used to close the window. EAknFepSoftkeySpell, EAknSoftkeyCancel, or EAknSoftkeySelect
    */
    virtual TInt ExecutePopupL( const TRect& aInlineEditorRect, TInt& aSelectedIdx, TKeyEvent& aLastKeyEvent, TBool aRightToLeftLanguage, TInt aKeyboard ) = 0;
    
    /**
    * Function called when the dialog is dismissed. Among other things, this hides the dialog
    * and deblocks the thread. Unlike in the base class, the instance is not destroyed here. 
    * The same popup instance can be reused by calling ExecutePopupL() again. 
    * @param    aAccept     EFalse  if the dialog was cancelled.
    *                       ETrue   otherwise.
    */
    virtual void AttemptExitL(TBool aAccept) = 0;

    /**
    * Removes the focus from the candidate list and makes the editor to be in focus.
    */
    virtual void UnFocus() = 0;
    /**
    * Changes the position where the candidate list is shown. The position of the candidate list is choosen by the
    * candidate list itself but can be controlled by the inline text rectangle.
    * @param    aRect  The inline text rect.
    */
    virtual void ShowAtNewPosition(TRect aRect) = 0;
    
    /**
     *  Revert back the candidate focus, and add the candidate list to the control stack reducing
     *  the control priority. Usually, this function can be called after void UnFocus().
     */
    virtual void SetFocusAddStackReducePriorityL() = 0;

       
    };
    
#endif // __AKN_FEP_CANDIDATE_POPUP_ H

