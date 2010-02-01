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
* Description:  hand writing box - control in Training UI application 
*
*/


#ifndef C_TRAININGUIHWRBOX_H
#define C_TRAININGUIHWRBOX_H

// includes 
#include <coecntrl.h>
#include <AknInfoPopupNoteController.h> // Tooltip

// forward declarations  
class MTruiHwrBoxObserver;
class CAknInfoPopupNoteController;
class CAknsBasicBackgroundControlContext;

const TPoint KTail( 0xFFFF,0 );

// Class declaration
/**
 *  CTruiHwrBox
 *  CTruiHwrBox is new component, which allows input and output a character
 *  model by strokes
 *  @lib avkon.lib
 */ 
class CTruiHwrBox : public CCoeControl, public MAknInfoPopupNoteObserver
    {   
public:

    /* Flags for hwrbox state. HwrBox must be one and only one state */   
    enum TBoxState
        {
        /* Already has model but doesn't play the animation */
        EExistModelStop = 0,
        /* Already has model and play the animation */
        EExistModelDraw,
        /* Ready to input an new character model */
        ENewModelStop,
        /* Use is inputting an new character model */
        ENewModelDraw,
        }; 
        
    enum THwrBoxFlag
        {
        ENoFrameHwrBox = 0x00000000,
        EFrameHwrBox = 0x00000001,
        EReadOnlyHwrBox = 0x00000002,
        EWritableHwrBox = 0x00000004
        };
        
public: 

    /**
     * Two-phased constructor.
     * @param aParent the parent control for choice list
     * @param aRect the extent of the control
     * @return Pointer to CTruiHwrBox's instance
     */
    static CTruiHwrBox* NewL( CCoeControl* aParent, 
                              MTruiHwrBoxObserver* aHwrBoxObserver = NULL,
                              MAknsControlContext* aBgContext = NULL );

    /**
     * Two-phased constructor.
     * @param aParent the parent control for choice list
     * @param aRect the extent of the control
     * @return Pointer to CTruiHwrBox's instance
     */
    static CTruiHwrBox* NewLC( CCoeControl* aParent,
                               MTruiHwrBoxObserver* aHwrBoxObserver = NULL,
                               MAknsControlContext* aBgContext = NULL );
    
    /**
     * Destructor
     */
    ~CTruiHwrBox(); 
    
    /**
     * Periodic Callback function
     * To save the model
     *
     * @param aObject: Transfer the pointer to CTruiHwrBox into this function
     * @return system-wide err code
     */
    static TInt TimeToSaveL( TAny* aObject );
    
    /**
     * Save the new inputting model
     */
    void SaveModelL();

    /**
     * Save the new inputting model by manual.
     */
    void SaveModelByManuallyL();
    
    /**
     * Periodic Callback function
     * To draw the next piont in the animation
     *
     * @param aObject: Transfer the pointer to CTruiHwrBox into this function
     * @return system-wide err code
     */
    static TInt TimeToDrawNextL( TAny* aObject );
    
    /**
     * Draw the next piont in the animation
     */
    void DrawNextL();
    
    /**
     * Begin to draw the existing model
     *
     * @param aIsScale ETrue scale points to the fitable size before playing.
     * @param aRefSize The size as the reference coordination for scaling.
     */
    void PlayAnimation( TBool aIsScale = EFalse, const TSize& aRefSize = TSize( 1, 1 ) );
    
    /**
     * clear the existing model
     */
    void ClearExistModel();
    
    /**
     * Set HwrBox observer
     * @param aHwrBoxObserver the observer
     */
    void SetHwrBoxObserver( MTruiHwrBoxObserver* aHwrBoxObserver );
    
    /**
     * Get the Hwrbox's model
     *
     * @return Reference to iPointArrayModel
     */
    RArray<TPoint>& Model();
    
    /**
     * Get the Hwrbox's state
     *
     * @return Current HWR box's state
     */
    TInt BoxState() const;
    
    /**
     * Pause the playing of animation
     */
    void PausePlaying();
    
    /**
     * Resume the playing of animation
     */
    void ResumePlaying();
    
    /**
     * Set guiding line
     *
     * @param aTop The vertical coordinate of top guiding line
     * @param aBottom The vertical coordinate of bottom guiding line
     * @return system-wide err code
     */
    TInt SetGuidingLine( TInt aTop, TInt aBottom );
    
    /**
     * Show trails statically
     *
     * @param aPointArray The points that will be shown in HwrBox
     */
    void ShowTrails( const RArray<TPoint>& aPointArray, 
                     TBool aIsScale = EFalse,
                     const TSize& aRefSize = TSize( 1, 1 ) );
    
    /**
     * Show tooltip
     *
     * @param aText The tooltip tex.
     * @param aPosition The tooltip's location.
     * @param aBeforeTimeout The time internal that will popup tooltip
     * @param aInViewTimeout The time internal that will hide tooltip
     */
    void ShowTooltipL( const TDesC& aText,
                       const TPoint& aPosition, 
                       const TInt aBeforeTimeout,
                       const TInt aInViewTimeout );
    
    /**
     * Set the position for tooltip
     *     
     * @param aPosition The tooltip's location.
     */
    void SetTooltipPosition( const TPoint& aPosition );
    
    /**
     * Hide tooltip
     *
     */
    void HideInfoPopupNote();
// From CCoeControl 
    
    /**
     * From CCoeControl. Handles key events.
     * @param aKeyEvent the key event    
     * @param aType The type of key event
     * @return Indicates whether or not the keyevent was used by this control   
     */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );    
    
    /**
     * From CCoeControl. Draws control to given area
     * @param aRect The rectangle that should be drawn by the control. 
     */
    void Draw( const TRect& aRect ) const;
       
    /**
     * Draw character model
     */
    void DrawModel();
    
    /**
     * Set box state from outside
     *
     * @param aReady ture means model is loaded
     *               false means no model is loaded
     */
    void ModelIsReady( TBool aReady = ETrue );
    
    TRect DrawableRect() const;
    
    inline void SetHwrBoxFlag( TUint aFlag )
        {
        iFlag = aFlag;
        };
        
    /**
     * Set flag to indicate if draw bottom guideline
     *
     * @param aBottomGuideLineDisplay ETrue means draw bottom guideline
     */
    inline void SetDisplayBottomGuideLine( TBool aBottomGuideLineDisplay )
        {
        iBottomGuideLineDisplay = aBottomGuideLineDisplay;
        };

    /**
     * Set flag to indicate if draw top guideline
     *
     * @param aTopGuideLineDisplay ETrue means draw bottom guideline
     */
    inline void SetDisplayTopGuideLine( TBool aTopGuideLineDisplay )
        {
        iTopGuideLineDisplay = aTopGuideLineDisplay;
        };
        
    /**
     * Set editing mode to readonly or writable
     *
     * @param aReadOnly ETrue means hwrbox is readonly
     *                  EFalse means hwrbox is loaded
     */
    void SetHwrBoxReadOnly( TBool aReadOnly );

// from CCoeControl
    
    /**
     * From CCoeControl. Handles pointer events
     * @param aPointerEvent the pointer event be be handled.
     */
    void HandlePointerEventL( const TPointerEvent& aPointerEvent );    
    
protected: 


    /**
     * From CoeControl.
     * Responds to changes to the size and position of the contents of this control.
     */
    void SizeChanged();
    
    /**
     * Handles a change to the control's resources.
     *
     * @param aType A message UID value.
     */
    void HandleResourceChange( TInt aType );
    
    /**
     * Handle focus changed
     *
     */
    void FocusChanged( TDrawNow aDrawNow ); 

private:

    /**
     * C++ Default Constructor 
     */
    CTruiHwrBox(); 

    /**
     * 2nd phase constructor
     * @param aParent the parent control for choice list
     * @param aRect the extent of the control
     */
    void ConstructL( CCoeControl* aParent,
                     MTruiHwrBoxObserver* aHwrBoxObserver = NULL,
                     MAknsControlContext* aBgContext = NULL );
    
    /**
     * Check if the point is contained in drawing area
     *
     * @param aPoint The point which to be checked
     * @return ETrue: The point is in drawing area
     *         EFalse: The point isn't in drawing area
     */
    TBool IsContained( const TPoint& aPoint ) const;
    
    /**
    * Adjust shortcut model's point to fit the popup hwr box. 
    * @param aOriginSize Origin size of which input character model.
    * @param aRealSize Real popup hwr box's size.
    * @param aModel The model to be adjuested.
    */    
    void ScaleModel( const TSize& aOriginSize, 
                     const TSize& aRealSize, 
                     RArray<TPoint>& aModel );
       
    /**
     * Draws all points.     
     */
    void DrawTrails( CWindowGc& aGc ) const;
    
    /**
     * Set context for this control.
     *
     * @param aGc Handle for CWindowGc
     */
    void PrepareContext( CWindowGc& aGc ) const;
    
    /**
     * Load bitmaps for border.
     *
     */
    void LoadBorderBitmapsL();
    
    /**
     * Load bitmaps for shadows.
     *
     */
    void LoadShadowBitmapsL();

    /**
     * Load bitmaps for guidelines.
     *
     */
    void LoadGuideLineBitmapsL();
    
    /**
     * Load layouts for borders from LAF.
     *
     * @param aVarity Varity for LAF
     */
    void LoadBorderLayout( TInt aVarity );

    /**
     * Load layouts for shadows from LAF.
     *
     * @param aVarity Varity for LAF
     */
    void LoadShadowLayout( TInt aVarity );
    
    /**
     * Load layouts for guidelines from LAF.
     *
     * @param aVarity Varity for LAF
     */
    void LoadGuideLineLayout( TInt aVarity );

    /**
     * Draw borders.
     *
     */
    void DrawBorders( CWindowGc& aGc ) const;
    
    /**
     * Draw shadows.
     *
     */
    void DrawShadows( CWindowGc& aGc ) const;

    /**
     * Draw guideliens.
     *
     */    
    void DrawGuideLines( CWindowGc& aGc ) const;    
    
// From MAknInfoPopupNoteObserver
    /**
    * Handles events reported from info popup note.
    * @param aController The controller that controls the note, from
    *   where the event originates.
    * @param aEvent The event.
    */
    void HandleInfoPopupNoteEvent( CAknInfoPopupNoteController* aController,
            TAknInfoPopupNoteEvent aEvent );
    
private: // data

    /**
     * When this timer time out, the inputting of new character is over.
     * Own.
     */
    CPeriodic* iTimeToSave;
    
    /**
     * To control drawing speed
     * Own.
     */
    CPeriodic* iTimeToDrawNext;

    /**
     * Contain the points need to be drawn on screen
     * Own
     */
    RArray<TPoint> iPointArrayDraw;

    /**
     * Contain the character model points
     * Own
     */
    RArray<TPoint> iPointArrayModel;
    
    /**
     * Current control state 
     */
    TBoxState iBoxState;
    
    /**
     * Observe user's input
     * Not own
     */
    MTruiHwrBoxObserver* iHwrBoxObserver;
    
    /**
     * Use this attribute to count the last animated point
     * zero implies the first point
     */
    TInt iLastAnimatedPoint;
    
    /**
     * Guideline top left point
     */
    TPoint iGuideLineTopL;
    
    /**
     * Guideline top right point
     */
    TPoint iGuideLineTopR;
    
    /**
     * Guideline bottom left point
     */
    TPoint iGuideLineBottomL;
    
    /**
     * Guideline bottom right point
     */
    TPoint iGuideLineBottomR;
    
    /**
     * Indicate pen position
     */
    TBool iPenInRect;   
          
    /**
     * Flag of training box.
     */
    TUint iFlag;
    
    /**
     * Component used to show tooltip
     * Own.
     */
    CAknInfoPopupNoteController* iTooltip;
    
    /**
     * Check if draw bottom guideline.
     */
    TBool iBottomGuideLineDisplay;
    
    /**
     * Check if draw bottom guideline.     
     */    
    TBool iTopGuideLineDisplay;
        
    /**
     * Pointer array to store pointers to bitmaps of borders
     * 
     * Own
     */    
    RPointerArray<CFbsBitmap> iBorderBitmaps;

    /**
     * Pointer array to store pointers to mask bitmaps of borders
     * 
     * Own
     */
    RPointerArray<CFbsBitmap> iBorderBitmapMasks;

    /**
     * Array to store pointers to TAknLayoutRect for borders
     * 
     * Own
     */
    RArray<TAknLayoutRect> iBorderLayout;


    /**
     * Pointer array to store pointers to bitmaps of shadows
     * 
     * Own
     */    
    RPointerArray<CFbsBitmap> iShadowBitmaps;

    /**
     * Pointer array to store pointers to mask bitmaps of shadows
     * 
     * Own
     */
    RPointerArray<CFbsBitmap> iShadowBitmapMasks;

    /**
     * Array to store pointers to TAknLayoutRect for shadows
     * 
     * Own
     */
    RArray<TAknLayoutRect> iShadowLayout;
    
    /**
     * Pointer array to store pointers to bitmaps of guideline
     * 
     * Own
     */    
    RPointerArray<CFbsBitmap> iGuideLineBitmaps;

    /**
     * Pointer array to store pointers to mask bitmaps of guideline
     * 
     * Own
     */
    RPointerArray<CFbsBitmap> iGuideLineBitmapMasks;

    /**
     * Array to store pointers to TAknLayoutRect for guidelines
     * 
     * Own
     */
    RArray<TAknLayoutRect> iGuideLineLayout;
    
    /**
     * Rect of drawable for trails;
     * 
     */
    TRect iDrawabelRect;
    
    /**
     * Color of pen;
     * 
     */
    TRgb iPenColor;
    
    
    CAknsBasicBackgroundControlContext* iBgContext;
    
    TBool iStartSave;
    };    

#endif // TRAININGUIHWRBOX_H

// end of file

