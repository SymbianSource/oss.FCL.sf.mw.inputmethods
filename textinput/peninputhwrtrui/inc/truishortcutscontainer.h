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
* Description:  CTruiShortcutsContainer class of TrainingUI
*
*/


#ifndef C_TRUISHORTCUTSCONTAINER_H
#define C_TRUISHORTCUTSCONTAINER_H

#include <eiklbo.h>
#include <AknPreviewPopUpObserver.h>
#include "truicontainerbase.h"

class CEikButtonGroupContainer;
class CAknSingleGraphicStyleListBox;
class CTruiHwrBox;
class CAknPreviewPopUpController;

/**
 *  CTruiShortcutsContainer container control
 *  Used as the component to shortcut main view
 *
 *  @code
 *      iContainer = CTruiShortcutsContainer::NewL( ClientRect() );
 *      iContainer->SetMopParent( this );
 *      AppUi()->AddToStackL( *this, iContainer );
 *  @endcode
 *
 */
class CTruiShortcutsContainer : public CTruiContainerBase,
                                public MEikListBoxObserver,
                                public MAknPreviewPopUpObserver
    {
public:

    /**
     * Two-phased constructor.
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin of its associated window.
     * @return Pointer to the component control
     */
    static CTruiShortcutsContainer* NewL( const TRect& aRect );
    
    /**
     * Two-phased constructor.
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin of its associated window.
     * @return Pointer to the component control
     */
    static CTruiShortcutsContainer* NewLC( const TRect& aRect );

    /**
     * Destructor.
     */
    virtual ~CTruiShortcutsContainer();

    /**
     * Popup the TextSettingPage and added the new shortcut to ListBox.    
     */    
    void AddShortcutL();
    
    /**
     * Delete shortcut.    
     */
    void DeleteShortcutL();
    
    /**
     * Mark shortcut.    
     */
    void MarkShortcutL();
    
    /**
     * Unmark shortcut.    
     */
    void UnmarkShortcutL();
    
    /**
     * Return currently selected item's index.
     *
     * @return Currently selected item's index. 
     */
    TInt CurrentItemIndex();

    /**
     * Test if the currently selected index is marked.
     *
     * @param aSelectedIndex: Current selected item's index
     * @return ETrue if the item is marked. EFalse if it is not marked. 
     */    
    TBool IsMarked( TInt aSelectedIndex );
     
    /**
     * Return the count of being marked currently.
     *     
     * @return The count of being marked. 
     */    
    TInt MarkCount();
    
    /**
     * Edit shortcut
     * @param aOnlyEditText ETrue : Only edit shortcut text.
     *                      EFalse : Only edit shortcut model.
     */
    void EditShortcutL( TBool aOnlyEditText = EFalse );
        
    
    /**
     * Check if reset introduction
     * 
     * @return ETrue: Reset introcudtion
     *         EFalse: Do not.
     */
    TBool IsResetIntroduction();

    /**
     * Reset introduction
     * 
     */
    void ResetIntroductionL();
    
    /**
     * Set flag to indicate popup window being displayed.
     *
     * @param aDisplayPopupWindow ETrue : Displaying.
     *                            EFalse : Not displaying.
     */
    inline void SetDisplayPopupWindow( TBool aDisplayPopupWindow )
        {
        iDisplayPopupWindow = aDisplayPopupWindow;
        };

// from MEikListBoxObserver
    /**
     * Observer callback from the listbox
     * @param aListBox  ptr to listbox that is calling the method  
     * @param aEventType  type of event; defined by MEikListBoxObserver 
     */
    void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );

// from CCoeControl
    /**
     * Handles key event.
     *
     * @param aKeyEvent The key event.
     * @param aType The type of key event: EEventKey, EEventKeyUp or EEventKeyDown. 
     *
     * @return Indicates whether or not the key event was used by this control.
     */    
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                 TEventCode aType );    

    /**
     *  GetHelpContext()
     *  Identify the help context so that the framework can look up
     *  the corresponding help topic
     *  @param aContext Returns the help context
     */    
    void GetHelpContext( TCoeHelpContext& aContext ) const;
           
private:
    
   /**
    * Constructor.
    */
    CTruiShortcutsContainer();

   /**
    * Perform the second phase construction of a CTruiMainViewContainer object.
    *
    * @param aRect: The rectangle that defines the control's extent. 
    */
    void ConstructL( const TRect& aRect );
    
    /**
     * Create controls to be displayed in this container.
     */    
    void InitializeControlsL();       
    
    /**
     * Delete shortcut from ListBox.     
     */    
    void DeleteItemsL();
    
    /**
     * Create preview popup controller to show popup hwrbox.       
     */ 
    CAknPreviewPopUpController* CreatePopupHwrBoxL();
    
    /**
     * Popup HWR box
     * @param aPosition The position to display HWR box
     * @param aShowDelay The delay time interval before being shown.
     * @param aHideDelay The delay time interval after which to hide tooltip.     
     */ 
    void PopupHwrBoxL( const TPoint& aPosition, 
                       const TTimeIntervalMicroSeconds32& aShowDelay,
                       const TTimeIntervalMicroSeconds32& aHideDelay );        
    /**
     * Preview shortcut model in popup hwrbox.
     * @param aShortcutText The shortcut to be previewed
     */ 
    void PreviewShortcutModelL( const TDesC& aShortcutText, TInt aIndex );
                          
   /**
    * Check if need to preview shortcut and perform previewing necessarily.
    *
    * @param aIndex To stand for which item of the listbox
    * @return ETrue : Preview shortcut model. EFalse : Don't preview it.
    */    
    TBool CheckAndPreviewShortcutModelL( TInt aIndex );
    
   /**
    * Update buttons' status in toolbar.
    *
    * @param aIndex To stand for which item of the listbox.
    */
    void UpdateToolbarButtonStatus( TInt aIndex );

    /**
     * Create ListBox
     */
    void CreateListBoxL();
    
    /**
     * Setup text data for listbox
     */    
    void LoadTextArrayL( const CDesCArray* aTextArray );
    
    /**
     * Handle event when listbox change to another focused item.
     */
    void HandleListBoxFocusChangedL( TInt aIndex );    

    /**
     * Popup context menu.
     * @param aResourceId The resource Id of the context menu.
     */
    void PopupContextMenuL( TInt aResourceId );
    
    /**
     * Get position relative to screen origin for popup window.
     * @param aIndex The index of item from which pop up.
     */
    TPoint PopupWindowPosition( TInt aIndex );
       
// from MAknPreviewPopUpObserver
    /**    
    * Called by the preview popup when an appropriate event takes place. 
    * @param aController Pointer to the sender of the event.
    * @param aEvent Event type.
    */
    void HandlePreviewPopUpEventL(
          CAknPreviewPopUpController* aController, TPreviewPopUpEvent aEvent );

// from CoeControl.   
    /**     
     * Responds to changes to the size and position of the contents of this control.
     */
    void SizeChanged();
    
    /**     
     * Responds to a change in focus.
     * @param aDrawNow Contains the value that was passed to it by SetFocus().
     */
    void FocusChanged( TDrawNow aDrawNow );
    
    /**     
     * Handles pointer events.
     * @param aPointerEvent The pointer event.
     */
    void HandlePointerEventL( const TPointerEvent& aPointerEvent );
       
private: // data
        
    /**
     * list box to display all shortcut
     * Not own
     */
    CAknSingleGraphicStyleListBox* iListBox;
    
    /**
     * Pointer to HWR box displayed in popup controller
     * Own
     */
    CTruiHwrBox* iHwrBox;

    /**
     * Pointer to preview popup used to show tooltip
     * Own
     */
    CAknPreviewPopUpController* iPopupController;

    /**
     * Check if popup preview when focus changed
     */
    TBool iDisplayPopupWindow;
    
    TUint iShortcutUnicode;
    
    /**
     * Point of pen down on item.
     */
    TPoint iPenDownPoint;
    };

#endif // C_TRUISHORTCUTSCONTAINER_H

