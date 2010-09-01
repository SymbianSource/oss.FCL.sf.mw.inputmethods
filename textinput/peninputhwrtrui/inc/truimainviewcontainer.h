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
* Description:  CTruiMainViewContainer class of TrainingUI
*
*/


#ifndef C_TRUIMAINVIEWCONTAINER_H
#define C_TRUIMAINVIEWCONTAINER_H

#include "truicontainerbase.h"
#include "truihwrboxobserver.h"

class CAknButton;
class CAknChoiceList;
class CTruiHwrBox;
class CAknCharMapDialog;
class CAknsBasicBackgroundControlContext;
class CEikLabel;
class CAknPreviewPopUpController;

const TInt KActiveCharSize = 1;
const TInt KLabelTextLen = 100;
/**
 *  CTruiMainViewContainer container control
 *  Used as the component to training ui main view
 *
 *  @code
 *      iContainer = CTruiMainViewContainer::NewL( ClientRect() );
 *      iContainer->SetMopParent( this );
 *      AppUi()->AddToStackL( *this, iContainer );
 *  @endcode
 *
 */
class CTruiMainViewContainer : public CTruiContainerBase, 
                               public MCoeControlObserver,
                               public MTruiHwrBoxObserver
    {
public:
    
    /**
     * Two-phased constructor.
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin of its associated window.
     * @return Pointer to the component control
     */
    static CTruiMainViewContainer* NewL( const TRect& aRect );
    
    /**
     * Two-phased constructor.
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin of its associated window.
     * @return Pointer to the component control
     */
    static CTruiMainViewContainer* NewLC( const TRect& aRect );

    /**
     * Destructor.
     */
    virtual ~CTruiMainViewContainer();
    
    /**
     * Handles an event from an observed control.
     *
     * @param aControl The control that sent the event.    
     * @param aEventType The event type.     
     */    
    void HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType );
    
    /**
     * Handles language script changed.
     *
     * @param aScriptId The selected language script's Id
     */  
    void HandleLanguageScriptChangedL( TInt aScriptId );

    /**
     * Handles character range changed.     
     *          
     * @param aIndex The index of range item in Menu "character range" or choicelist
     * @param aRangeId The range Id.
     */  
    void HandleCharRangeChangedL( TInt aIndex, TInt aRangeId );
    
    /**
     * Handles choosing an active char from SCT.
     *          
     */  
    void HandleChooseCharEventL();
    
    /**
     * Preview character model
     *          
     */  
    void PreviewCharacterModel();
    
    /**
     * Set current trained character
     *
     * @param aChar the trained character         
     */ 
    void SetCurrentCharL( const TPtrC& aChar );
    
    /**
     * Clear current model
     *        
     */ 
    void ClearCurrentModel();
    
    /**
     * Delete model of current character
     *        
     */ 
    void DeleteModelL();
    
    /**
     * Delete all models of current character
     *        
     */ 
    void DeleteAllModelL();

    /**
     * Check if clear button has been dimmed
     *       
     * @return ETrue: dimmed; EFalse: undimmed  
     */    
    TBool IsButtonDimmed();
    
    /**
     * Check if all delete button has been dimmed
     *       
     * @return ETrue: dimmed; EFalse: undimmed  
     */
    TBool IsAllDelete();
    
    /**
     * Get character set Id, used to choose which characters will be displayed in SCT
     *
     * @param aRangeId Character range Id for special character table 
     *     
     * @return Id of character set. 
     */       
    TInt SctCharacterSetId( TInt aRangeId );

    /**
     * Get cyrillic lower character set Id
     *
     * @param aInputLanguage Current input language
     *     
     * @return Id of character set. 
     */       
    TInt SctCyrillicLowerCharacterSetId( TLanguage aInputLanguage );

    /**
     * Get cyrillic upper character set Id
     *
     * @param aInputLanguage Current input language
     *     
     * @return Id of character set. 
     */       
    TInt SctCyrillicUpperCharacterSetId( TLanguage aInputLanguage );
        
    /**
     * Before exit, do some necessary operation
     *
     */
    void PrepareToExitL();
    
    /**
     * Handle size changed
     *
     * @param aVarity: Use aVarity to load different resulotion's LAF data.
     */    
    void DoSizeChanged( TInt aVarity );

private:

   /**
    * Constructor.
    */    
    CTruiMainViewContainer();

   /**
    * Perform the second phase construction of a CTruiMainViewContainer object.
    *
    * @param aRect: The rectangle that defines the control's extent. 
    */
    void ConstructL( const TRect& aRect );
    
    /**
     * Create controls to be displayed in this container.
     *
     */    
    void InitializeControlsL();
    
    /**
     * Create label and display it in the container.
     *
     * @param aResourceID The resource ID
     * @param aParent The parent of the control.
     * @param aObserver The observer of the control.
     * @return The pointer to the label.
     */
    CEikLabel* CreateLabelL( const TInt aResourceId,
                             const CCoeControl* aParent,
                             MCoeControlObserver* aObserver );
                             
    /**
     * Create choicelist and display it in the container.
     *
     * @param aParent The parent of the control.
     * @param aObserver The observer of the control.   
     * @param aTextArray The content displayed in the control.
     * @param aButton The attached button.
     * @return The pointer to the choiclist.
     */
    CAknChoiceList* CreateChoiceListL( CCoeControl* aParent,
                                       MCoeControlObserver* aObserver,
                                       CDesCArray* aTextArray,
                                       CAknButton* aButton );
                             
    /**
     * Create HWRBox and display it in the container.
     *
     * @param aParent The parent of the control.
     * @param aObserver The observer of the control.
     * @return The pointer to the choiclist.
     */
    CTruiHwrBox* CreateHwrBoxL( CCoeControl* aParent, 
                                MTruiHwrBoxObserver* aObserver );
     
     /**
     * Return button's resourceId by Character Range Id.
     *
     * @param aRangeId The character range ID.     
     * @return Reource Id to be used to create button for choicelist.
     */                                           
    TInt ButtonResourceId( TInt aRangeId ) const;

    /**
     * Pop Sct dialog
     *
     * @param aCharCase Character case for special character table 
     *
     * @param aSpecialChar Reference to the buffer where selected special characters are put.
     * @param aCharSetResourceId The resource ID of special character table 
     * @return Id of the button used to dismiss dialog.
     * 
     */    
    TInt PopupSctDialogL( TInt aCharCase, TDes& aSpecialChars, TInt aCharSetResourceId );

    /**
     * Popup information note
     *
     * @param aText The text to be displayed in CAknInformationNote.
     */ 
    void PopupInformationNoteL( const TDesC& aText );       
                                   
    /**
     * Adjust special behavior characters to displayable characters.
     * @param aResult Return result for adjustment.
     *
     */
    void AdjustDisplayChars( TDes& aResult );
    
    /**
     * From CoeControl.
     * Handles key event.
     *
     * @param aKeyEvent The key event.
     * @param aType The type of key event: EEventKey, EEventKeyUp or EEventKeyDown. 
     * @return Indicates whether or not the key event was used by this control.
     */    
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    /**
     *  GetHelpContext()
     *  Identify the help context so that the framework can look up
     *  the corresponding help topic
     *  @param aContext Returns the help context
     */    
    void GetHelpContext( TCoeHelpContext& aContext ) const;
            
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
     * From CoeControl.
     * Draws the container.
     *
     * @param aRect The region of the control to be redrawn. Co-ordinates are relative to the control's origin (top left corner).      
     */    
    virtual void Draw( const TRect& aRect ) const;
    
    /**
     * From CoeControl.
     * Handles a change to the control's resources.
     *
     * @param aType A message UID value.
     */
    void HandleResourceChange( TInt aType );   
    
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    
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
       
private: // data

    /**
     * The current trained character
     */
    TBuf<KActiveCharSize> iCurrentChar;
    
    /**
     * The current tip of training character
     */
    HBufC* iLabeltext;
    
    /**
     * Store the wrapped text to indicate which character is being trained.
     * 
     * Own
     */
    HBufC* iWrappedLabelText;

    /**
     * This text helps a user understand that he / she is
     * teaching his / her own character models to HWR engine
     * Not own
     */
    CEikLabel* iLabel;
    
    /**
     * The Clear button in toolbar, used to clean up the Writing Box
     * Not own
     */
    CAknButton* iClearBtn;

    /**
     * The preview button in toolbar
     * Not own
     */
    CAknButton* iPreviewBtn;
    
    /**
     * The button currently associated with choicelist.
     * Not own
     */
    CAknButton* iCurChoiceListBtn;

    /**
     * Left buttons allow quick access to the previous character
     * in the selected character range.
     * Not own
     */
    CAknButton* iLeftBtn;

    /**
     * Indicates which character is currently selected.
     * Not own
     */
    CEikLabel* iIindicator;

    /**
     * Right buttons allow quick access to the Next character
     * in the selected character range.
     * Not own
     */
    CAknButton* iRightBtn;
    
    /**
     * ChoiceList is used for switching between character ranges.
     * Not own
     */
    CAknChoiceList* iChoicelist;

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
     * Rect of parent pane of label     
     */
    TRect iLabelPaneRect;
    
    /**
     * MultiLineTextLayout of Label
     */
    TAknMultiLineTextLayout iMultilineLayout;
     
    RArray<TInt> iLineWidthArray;    
    };

#endif // C_TRUIMAINVIEWCONTAINER_H

