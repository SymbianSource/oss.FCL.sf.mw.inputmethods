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
* Description:             Avkon implementation of non-focusing popup menu to show word candidates.
*
*/












#ifndef __AKN_FEP_AVKON_CANDIDATE_POPUP_H
#define __AKN_FEP_AVKON_CANDIDATE_POPUP_H

#include "AknFepCandidatePopup.h"
#include "AknFepCandidatePopupCallback.h"
#include <aknPopup.h>
#include <PtiDefs.h>

// FORWARD DECLARATIONS
class TAknPopupLayouts;
class TRect;


// CLASS DECLARATION
class CAknFepAvkonCandidatePopup : public CAknPopupList, public MAknFepCandidatePopup
    {
public: // construction and destruction

    /**
    * Two-phased constructor
    * @param    aOwner  Reference to the owner of the popup which provides 
    *                   the candidates to be shown
    * @return   A new instance of the class.
    */
    static CAknFepAvkonCandidatePopup* NewL( MAknFepCandidatePopupCallback& aOwner );
    
    /**
    * Destructor
    */
    ~CAknFepAvkonCandidatePopup();
    
public: // methods

    /**
    * Getter for the currently active word
    * @return   The currently active word as pointer descriptor
    */
    TPtrC ActiveWord() const;
    
    /**
    * Getter for the exact word (the word with ordinal 0)
    * @return   The exact word as pointer descriptor.
    */
    TPtrC ExactWord() const;
    
    /**
    * Makes the pop-up selection list visible and blocks until the dialog has been dismissed.
    * @param    aInlineEditorRect    Tells the place of the inline editor. If possible, the popup is opened
    *                                so that it does not hide the inline editor.
    * @param    aSelectedIdx         Output argument for the selected index.
    * @param    aLastKeyEvent        The last key event received by the popup. On return this is the event
    *                                which closed the popup.
    * @param    aRightToLeftLanguage Direction of the candidate language.ETrue is right to left. EFalse otherwise
    * @param    aKeyboard            Type of keyboard in use.
    * 
    * @return   The command id used to close the window. EAknFepSoftkeySpell, EAknSoftkeyCancel, or EAknSoftkeySelect
    */
    TInt ExecutePopupL( const TRect& aInlineEditorRect, TInt& aSelectedIdx, TKeyEvent& aLastKeyEvent, TBool aRightToLeftLanguage, TInt aKeyboard );
    
    /**
    * Function called when the dialog is dismissed. Among other things, this hides the dialog
    * and deblocks the thread. Unlike in the base class, the instance is not destroyed here. 
    * The same popup instance can be reused by calling ExecutePopupL() again. 
    * @param    aAccept     EFalse  if the dialog was cancelled.
    *                       ETrue   otherwise.
    */
    void AttemptExitL(TBool aAccept); 
    /**
    * Function called when there is a change of resource, typically when there is a layout switch.
    * Overidden the base implementation for the same. This is because the position of the popup
    * also chnages when there is a layout change.
    * @param    aType : Specifies the type of layout switch.
    */
    void HandleResourceChange(TInt aType); 

    /**
    * Removes the focus from the candidate list and makes the editor to be in focus.
    */
    void UnFocus();
    /**
    * Changes the position where the candidate list is shown. The position of the candidate list is choosen by the
    * candidate list itself but can be controlled by the inline text rectangle.
    * @param    aRect  The inline text rect.
    */
    void ShowAtNewPosition(TRect aRect);
    
    /**
     * Reduce the candidate pop up control priority and add it to stack.
     * Basically, this function is called,if after void UnFocus();
     * 
     * Whenever layout Change is happen opencandidate list, then candidatelist priority
     * should be reduce, otherwise updated inline rect 
     */
    void SetFocusAddStackReducePriorityL();
   
protected: // methods
    
    /**
    * Handler for key evemts.
    * @param    aKeyEvent   The key event to be handled.
    * @param    aType       The type of the key event to be handled.
    * @return   EKeyWasConsumed     If the offered key event is consumed.
    *           EKeyWasNotConsumed  Otherwise.
    */
    virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

    /**
    * Processing of the CBA commands
    * @param  aCommandId  The command to be handled.
    */
    virtual void ProcessCommandL(TInt aCommandId);

    /**
    * Calculates the position for the popup window and places it there
    * @param    aInlineEditorRect   The placement of the inline editor is used as reference.
    *                               The inline editor is not hidden behind the popup if possible.
    *                               The preferred location for the popup is under the inline editor.
    *                               The next best place is on the right side of the inline editor,
    *                               then on the top of the editor and last on the left side of the editor.
    */
    virtual void SetupWindowLocation( const TRect& aInlineEditorRect );
    
    /**
    * Sets up the layout of the popup window.
    * Modified from CAknPopupList::SetupWindowLayout()
    */
    virtual void SetupWindowLayout( AknPopupLayouts::TAknPopupLayouts aType );

    /**
    * Calculates the width of the popup window based on the candidates available on the list. 
    * The with is always set to as small as possible without truncation. 
    * The client application area is used as reference to ensure that the popup is never wider
    * than the application area.
    * @param    aClientRect The client application area.
    * @return   The window widt in pixels.
    */    
    TInt CalculateWindowWidth( const TRect& aClientRect );
    
    /**
    * Updates the popup window layout.
    * Modified from AknPopupLayouts::HandleSizeChanged()
    */
    void HandleSizeChanged( TAknPopupWindowLayoutDef &aDef,
                            AknPopupLayouts::TAknPopupLayouts aLayout_1,
                            TAknPopupLayoutsNode *aNode );
    
    /**
    * The C++ constructor
    * @param    aOwner  Reference to the owner of the popup which provides 
    *                   the candidates to be shown 
    */
    CAknFepAvkonCandidatePopup( MAknFepCandidatePopupCallback& aOwner );

    /**
    * Second phase constructor.
    */
    void ConstructL();
    
    
    static TInt HandleChrKeyMonitorCallback(TAny* aParam);
    
    /**
     * Judge whether phonelanguage is right to left language
     */
    void RightToLeftPhoneLanguage();
    
    /**
     * Set the correct coordinate by writing language
     */
    void SetUpLayoutCoords(TAknWindowLineLayout& aLayout);

private: // data

    //! Owner of this popup. Provides the candidates.
    MAknFepCandidatePopupCallback& iOwner;

    //! The listbox embedded into this popup
    CEikFormattedCellListBox* iList;
    
    //! Array of candidates shown in the popup list
    CDesCArray* iCandidates;
    
    //! Pointer to variable containing the index of currently selected word.
    TInt* iSelectedIdx;         // not owned
    
    //! Pointer to variable containing the last received key event.
    TKeyEvent* iLastKeyEvent;   // not owned
    
    //! Last received command. EAknSoftkeySelect, EAknSoftkeySpell, or EAknSoftkeyCancel.
    TInt iLastCommandId;
    
    //! Candidate Language direction.
    TBool iRightToLeftCandidate;
    //! Keyboard type in use.
    TPtiKeyboardType iKeyboard;
    
    CPeriodic*     iChrKeypressMonitor;
    //The inline text rectangle that is used as a reference for showing the candidate list.
    TRect iInlineTextRect;
    
    //Judge whether phonelanguage is right to left language
    TBool iIsRightToLeftPhoneLanguage;
    };
    

#endif // __AKN_FEP_AVKON_CANDIDATE_POPUP_H   

