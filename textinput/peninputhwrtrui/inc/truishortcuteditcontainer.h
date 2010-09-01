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
* Description:  CTruiShortcutEditContainer class of TrainingUI
*
*/


#ifndef C_TRUISHORTCUTEDITCONTAINER_H
#define C_TRUISHORTCUTEDITCONTAINER_H

#include <AknLayout2Def.h>
#include "truicontainerbase.h"
#include "truihwrboxobserver.h"
#include "truiradiobuttonobserver.h"

class CAknButton;
class CTruiHwrBox;
class CAknsBasicBackgroundControlContext;
class CTruiRadioButton;
class CAknCharMapDialog;
    
/**
 *  CTruiShortcutEditContainer container control
 *  Used as the component to shortcut main view
 *
 *  @code
 *      iContainer = CTruiShortcutEditContainer::NewL( ClientRect() );
 *      iContainer->SetMopParent( this );
 *      AppUi()->AddToStackL( *this, iContainer );
 *  @endcode
 *
 */
class CTruiShortcutEditContainer : public CTruiContainerBase,
                                   public MCoeControlObserver,
                                   public MTruiHwrBoxObserver,
                                   public MTruiRadioButtonObserver
    {
public:
    
    /**
     * Two-phased constructor.
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin
     *              of its associated window.
     * @return Pointer to the component control
     */
    static CTruiShortcutEditContainer* NewL( const TRect& aRect );
    
    /**
     * Two-phased constructor.
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin
     *              of its associated window.
     * @return Pointer to the component control
     */
    static CTruiShortcutEditContainer* NewLC( const TRect& aRect );

    /**
     * Destructor.
     */
    virtual ~CTruiShortcutEditContainer();

    /**
     * Switch app's active view to ShortcutView.
     */
    void SwitchToShortcutViewL();
   
    /**
     * Go back to the previous view.
     */
    void BackToPreviousViewL();
    
    /**
     * Query if shortcut model exist
     */
    TBool HasShortcutModel();
   
    /**
     * Preview shortcut model
     */
    void PreviewShortcutModel();
    
    /**
    * Before exit, do some necessary operation
    *
    * @retrun TBool if ready to exit, true will be return,
    *               otherwise it will return false
    */
    TBool PrepareToExitL();
    
    /**
     * Clear current model
     *        
     */ 
    void ClearCurrentModelL();
    
    /**
     * Return the currently selected index, which will be set in Shortcuts view.
     *
     * @param aItemList The shortcut list array in which will locate the aTex.
     * @param aText The shortcut item which will be located in aItemList
     * @return The index of aText in aItemList. 
     *         If it doesn't exist, it will return KErrNotFound.          
     */ 
    TInt DefaultSelectedIndex( const CDesCArray* aItemList, 
                               const TDesC& aText );  
                               
    /**
     * Popup SCT dialog.
     *
     */
    void PopupSctDialogL();
    
private:

    /**
     * Constructor.
     */
    CTruiShortcutEditContainer();

    /**
     * Two-phased constructor. Second phase
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin
     *              of its associated window.
     */
    void ConstructL( const TRect& aRect );
    
    /**
     * Create controls to be displayed in this container.
     */    
    void InitializeControlsL();        
    
    /**
     * Create radio button.
     */
    void CreateRadioButtonL();
    
    /**
     * Create HWRBox
     */
    void CreateHwrBoxL();
    
    /**
     * Create buttons
     */
    void CcnstructButtonL();
    
    /**
     * Create Label
     */
    void CreateLabelL();
    
    /**
     * Popup tooltip
     *
     * @param aResourceID The numeric ID of the resource string to be read.
     */ 
    void PopupTooltipL( TInt aResourceID );    
    
    /**
     * Layout under landscape condition     
     */ 
    void DoLayoutLandscape();
    
    /**
     * Layout under portait condition     
     */ 
    void DoLayoutPortait();
    
    /**
     * Save own shortcut.
     *
     * @param aShortcut The shortcut text which to be stored.
     * @param aModel The points array assigned to the shortcut.
     * @param aErrMsg The error message for too similar.
     * Leave with code KErrAlreadyInUse
     */ 
    void SaveOwnShortcutL( const TDesC& aShortcut, 
                           RArray<TPoint>& aModel, 
                           TDes& aErrMsg );
    
    /**
     * Save preset shortcut
     *
     * @param aShortcut The shortcut text which to be stored.
     * @param aUnicode The unicode of preset symbol assigned to the shortcut.
     * Leave with code KErrAlreadyInUse
     */ 
    void SavePresetShortcutL( const TDesC& aShortcut, TUint aUnicode );
    
    /**
     * Backup shortcut model. It is probably to be restored when press Back to exit.
     *
     * @param aOriginalModel The shortcut model which to be backuped.
     * @param aOriginalUnicode The unicode of preset symbol which to be backuped.     
     */ 
    void BackupShortcutModel( const RArray<TPoint>& aOriginalModel,
                              TUint aOriginalUnicode );
                              
    /**
     * Set active char.
     *
     * @param aUnicode The unicode which to be displayed.
     */ 
    void SetActiveCharL( TUint aUnicode );
        
    /**
     * Find the first available preset symbol.
     *
     * @param aStartPos The position from which to search
     * @param aDirection The direction of how to search in preset arrays,
     *                   Toward left or toward right
     * @param aUnicode The unicode of the first available preset symbol.
     * @return ETrue: search successfully; EFalse: Fail to seach, meaning all in use
     */ 
    TBool FindFirstAvailableCharL( TInt aStartPos, TInt aEndPos, 
                                   TInt aDirection, TUint& aUnicode );
    
    /**
     * Return if the preset is in use
     *
     * @param aUnicode The preset's unicode
     * @return ETrue: in use
     */ 
    TBool CheckPresetSymbolInUseL( TUint aUnicode );
    
    /**
     * Do something to response to size changed
     *
     * @param aVarity Varity to decide which option will be retrieved from LAF data
     */ 
    void DoSizeChanged( TInt aVarity );
    
// from base class CCoeControl    
    /**
     * Draw the screen.
     * @param aRect the rectangle of this view that needs updating
     */
    void Draw( const TRect& aRect ) const;

    /**
     * From CoeControl.
     * Responds to changes to the size and position of the contents of this control.
     */
    void SizeChanged();

    /**
     * From CoeControl.
     * Responds to a change in focus.
     *
     * @param aRect Contains the value that was passed to it by SetFocus(). 
     */
    void FocusChanged( TDrawNow aDrawNow );
    
    /**
     * Handles a change to the control's resources.
     *
     * @param aType A message UID value.
     */
    void HandleResourceChange( TInt aType );
    
// from MCoeControlObserver
    /** 
     * Handles an event from an observed control.
     * This function is called when a control for which this control is the
     * observer calls CCoeControl::ReportEventL(). It should be implemented
     * by the observer control, and should handle all events sent by controls
     * it observes.
     *
     *@param aControl The control that sent the event.
     *@param aEventType The event type.
     */
    void HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType );
    
// From MTruiHWRBoxObserver 
    /**
     * From MTruiHWRBoxObserver.
     * Input times out
     * Handle the save model event
     */
    void SaveNewModelL();
    
    /**
     * From MTruiHWRBoxObserver.
     * HandleEventL
     * Handle the message event from the HWRBox
     * @param aMessage According to the aMessage to handle the event
     */
    void HandleEventL( TMessageType aMessage );
    
// from MTruiRadioButtonObserver
    /**
     * Notify that which radio button is selected.
     * @param aIndex The index of currently selected index.
     */
    void SelectChangedL( TInt aIndex );
       
private: // data
    /**
     * The Writing Box is used for drawing a character model.
     * Not own
     */
    CTruiHwrBox* iHwrBox;
    
    /**
     * Pointer to background
     * Own
     */
    CAknsBasicBackgroundControlContext*	iBackGround;
   
    /**
     * Pointer to radio button
     * Not own
     */
    CTruiRadioButton* iRadioButtonContainer; 
    
    /**
     * Pointer to the tip label
     * Not own
     */
    CEikLabel* iLabelTip;
    
    /**
     * The current tip of training shortcut
     * Own
     */
    HBufC* iLabeltext;
    
    /**
     * Store the wrapped text to give tips about own or preset.
     * Own
     */
    HBufC* iWrappedLabelText;

    /**
     * Left button.
     * Not own
     */
    CAknButton* iLeftBtn;

    /**
     * Right button.
     * Not own
     */
    CAknButton* iRightBtn;

    /**
     * Active indicator.
     * Not own
     */
    CEikLabel* iIndicator;
    
    /**
     * Current preset symbol.
     */    
    TUint iUnicode;
    
    /**
     * Current shortcut type.
     */
    TInt iShortcutType;
    
    /**
     * Store the old points of the current shortcut.
     * own
     */
    RArray<TPoint> iModelBak;
    
    /**
     * Store the old unicode of the current preset shortcut.     
     */
    TUint iUnicodeBak;
    
    /**
     * Check if need to restore old shortcut.
     */
    TBool iIsRestored;
    
    /**
     * Check if the hwrbox is empty.
     */
    TBool iIsEmptyHwrBox;
    
    /**
     * Store all presets from Engine.
     * Own
     */
    RArray<TUint> iPresets;
    
    /**
     * Which char is active in preset array.
     */
    TInt iActiveCharPos;
    
    TAknMultiLineTextLayout iMultilineLayout;
    
    /**
     * Line array.
     * Own
     */
    RArray<TInt> iLineWidthArray; 
    };

#endif // C_TRUISHORTCUTEDITCONTAINER_H

