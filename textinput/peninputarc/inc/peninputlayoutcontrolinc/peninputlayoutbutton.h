/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of button and dragbar
*
*/

#ifndef C_CBUTTONBASE_H
#define C_CBUTTONBASE_H

#include <peninputlayoutbasecontrol.h>
/**
 *  CButtonBase
 *
 *  Button base control. This class could be used directly by user.
 *
 *  @lib peninputlayoutcontrol.lib
 *  @since S60 V4.0
 */
class CButtonBase: public CFepUiBaseCtrl
    {
public: 
    //constructor and destructor
    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                  
    IMPORT_C virtual ~CButtonBase();

    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CButtonBase class        
     */                               
    IMPORT_C static CButtonBase* NewL(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aControlId);

    /**
     * Set button active or deactive.        
     *
     * @since S60 V4.0        
     * @param aFlag Button status flag.
     */                                       
    IMPORT_C void SetActive(TBool aFlag);
    
    /**
     * Set button dimmed.        
     *
     * @since S60 V4.0        
     * @param aFlag Dim status. Default value is ETrue.
     */                                      
    IMPORT_C void SetDimmed(TBool aFlag);           

    /**
     * Set bitmap for given status .        
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap. Ownership transferred to button
     * @param aType The bitmap type used for
     */                                 
    IMPORT_C void SetBitmapL(CFbsBitmap* aBmp, TButtonBmpType aType);

    /**
     * Set mask bitmap for given status.        
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap. Ownership transferred to button
     * @param aType The bitmap type used for
     */                                 
    IMPORT_C void SetMaskBitmapL(CFbsBitmap* aBmp, TButtonBmpType aType);

    /**
     * get bitmap for given type . 
     *
     * @since S60 V4.0        
     * @param aType The bitmap type used for
     * @param aMaskFlag EFalse if want to get the mask bitmap, otherwise, 
     *          return the bitmap. Default value is EFalse.
     * @return The bitmap
     */                                 
    IMPORT_C CFbsBitmap* Bitmap(TButtonBmpType aType,TBool aMaskFlag = EFalse);

    /**
     * Set caption.        
     *
     * @since S60 V4.0        
     * @param aText The button caption
     */                                 
    IMPORT_C void SetCaptionL(const TDesC& aText);
    
    /**
     * Get button dimmed status .
     *
     * @since S60 V4.0        
     * @return Button dimmed status
     */                                                                      
    inline TBool IsDimmed();
   

    /**
     * Enable bitmap stretchable. When it's set, all bitmap in this control
     * will be stretched to fit in the control. It's stretchable by default
     *
     * @since S60 V4.0
     * @param aFlag the bitmap stretching flag
     */
    inline void SetBmpStretchable(TBool aFlag = ETrue);

    /**
     * Get bitmap stretchable flag
     *
     * @since S60 V4.0
     * @return aFlag the bitmap stretching flag
     */
    inline TBool BmpStretchable();
    
   
    //from base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */                               
    IMPORT_C virtual void Draw();    

    /**
     * From CFepUiBaseCtrl
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V4.0
     * @param aPoint current pointer position
     */
    IMPORT_C virtual void HandlePointerLeave(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer enter event. 
     * This will be called only when it's got pointer down event before and 
     * pointer leaves but now comes back without pointer up event.
     *
     * @since S60 V4.0
     * @param aPoint current pointer position     
     */
    IMPORT_C virtual void HandlePointerEnter(const TPoint& aPoint);            
    
    /**
     * From CFepUiBaseCtrl    
     * Set control's rectangle
     *
     * @since S60 V4.0
     * @param aRect The new control area
     */                                       
    //IMPORT_C virtual void SetRect(const TRect& aRect);    

    /**
     * Hide or shoe control. A hiden control will not handle the event.
     *
     * @since S60 V4.0
     * @param aFlag ETrue if want to hide the control.
     */                                               
    //IMPORT_C virtual void Hide(TBool aFlag);
    
    /* Set the font for caption
     *
     * @since S60 V4.0
     * @param aFontSpec the font spec to be used.
     */                                                   
    IMPORT_C void SetFont(const TFontSpec& aFontSpec);
    
    /**
     * Set font information
     *
     * @since S60 V4.0          
     * @param aFont The font info for key drawing.Ownership not transferred
     */
    IMPORT_C void SetFont(const CFont* aFont);    

    /**
     * Set caption color
     *
     * @since S60 V4.0          
     * @param aCol The caption font color
     */    
    inline void SetCaptionColor(const TRgb& aCol);
    
    /**
     * Set font shadow color
     *
     * @since S60 V4.0          
     * @param aCol The font color
     */    
    inline void SetCaptionShadowColor(const TRgb& aCol);    

    /**
     * Reset the bitmap pool.
     *
     * @since S60 v4.0
     */
    IMPORT_C void ResetBmpPool();    

	IMPORT_C void Hide(TBool aFlag);
	
protected:   
    /**
     * Constructor
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     */           
    IMPORT_C CButtonBase(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aControlId);            
    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * Handle pointer move event
     * From CFepUiBaseCtrl
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
 	 * @return The control which handles the event
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);

    /**
     * Set background bitmap
     *
     * @since S60 V4.0
     * @param aBmp The background bitmap. Ownership transferred to control
     */
    IMPORT_C virtual void SetBackgroundBitmapL(CFbsBitmap* aBmp);
    
    /**
     * Set background mask bitmap
     *
     * @since S60 V4.0
     * @param aBmp The background bitmap. Ownership transferred to control
     */
    IMPORT_C virtual void SetBackgroundMaskBitmapL(CFbsBitmap* aBmp);

private:
    /**
     * Add a bitmap to bitmap pool
     *
     * @since S60 V4.0
     * @param aBmp The  bitmap to be added
     */ 
    void AddBmpToPool(CFbsBitmap* aBmp);

    
protected:
    /**
     * bitmap for active state
     * Own.
     */
    CFbsBitmap* iActiveBmp; 
    
    /**
     * bitmap for pressing in active state
     * Own.
     */
    CFbsBitmap* iActiveBmpPressed; 
    
    /**
     * bitmap for non active state
     * Own.
     */
    CFbsBitmap* iNonActiveBkBmp;    
    
    /**
     * bitmap for pressing in non active state
     * Own.
     */
    CFbsBitmap* iNonActiveBkBmpPressed;
         
    /**
     * mask bitmap for active state
     * Own.
     */
    CFbsBitmap* iActiveMaskBmp;
    
    /**
     * mask bitmap for pressing in active state
     * Own.
     */
    CFbsBitmap* iActiveMaskBmpPressed; 
    
    /**
     * mask bitmap for non active state
     * Own.
     */
    CFbsBitmap* iNonActiveBkMaskBmp; 
    
    /**
     * mask bitmap for pressing in non active state
     * Own.
     */
    CFbsBitmap* iNonActiveBkMaskBmpPressed; 
        
private:    
    /**
     * Button pressed status
     */
    TBool iPressed;    
    
    /**
     * Button dim status
     */
    TBool iDimmed;

    /**
     * Bitmap stretchable flag
     */ 
    TBool iStretchable;
    
    /**
     * bitmap for dimmed state
     * Own.
     */
    CFbsBitmap* iDimmedBmp;   
         
    /**
     * mask bitmap for dim stute
     * Own.
     */
    CFbsBitmap* iDimmedMaskBmp;
    
    /**
     * Button caption
     */
    HBufC* iCaption;
    
    /**
     * Font object for key drawing
     * Ownership depends on flag iFontOwnership.
     */          
    CFont* iFont;

    /**
     * Font information for key drawing
     */     
    TFontSpec iFontSpec;

    /**
     *  Flag ownership flag. ETrue if CFont object is owned by this control.
     */
    TBool iFontOwnership;    
    /**
     * Baseline for caption
     */
    TInt iBaseline;
    
    /**
     * Pen color for first level caption
     */     
    TRgb iCaptionColor;

    /**
     * Pen color for caption shadow
     */     
    TRgb iCaptionShadowColor;
    
    /**
     * bitmap pool for all bitmaps.
     */
    RPointerArray<CFbsBitmap> iBmpPool;
    
    /**
     * keep the bitmap for background bitmap set by SetBackgroundBitmapL in case someone used.
     * Own.
     */
    CFbsBitmap* iNoUsedBkbmp;

    /**
     * keep the bitmap for mask background bitmap set by SetBackgroundMaskBitmapL in case someone used.
     * Own.
     */
    CFbsBitmap* iNoUsedMaskBkbmp;
    
    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;

    };


/**
 *  CDragBar
 *
 *  Drag bar
 *
 *  @lib peninputlayoutcontrol.lib
 *  @since S60 V4.0
 */
class CDragBar: public CButtonBase,public MTimerHandler,public MEventObserver
    {
public:
    /**
     * Factory function
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return The class instance 
     */               
    IMPORT_C static CDragBar* NewL(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aControlId);
    
    /**
     * Destructor
     *
     * @since S60 V4.0
     */
    IMPORT_C virtual ~CDragBar();
    
    /**
     * Add position observer. When drag bar moved, the observer will get notified.
     *
     * @since S60 V4.0        
     * @param aObserver The event observer
     */                                                              
    IMPORT_C void AddPositionObserver(MPositionObserver* aObserver);        

    /**
     * Remove position observer. 
     *
     * @since S60 V4.0        
     * @param aObserver The event observer to be removed
     */                                                              
    IMPORT_C void RemovePositionObserver(MPositionObserver* aObserver);        

    /**
     * Set the long press time out. 
     *
     * @since S60 V4.0        
     * @param aTime The long press event time out interval
     */                                                              
    IMPORT_C void SetLongpressTimer(TTimeIntervalMicroSeconds32 aTime);        

    /**
     * Check a control is the position observer of the dragbar 
     *
     * @since S60 V4.0
     * @param aControl The control to be checked
     * @return ETrue if the control is position observer of the dragbar
     */                
    IMPORT_C TBool IsDraggingComponent(CFepUiBaseCtrl* aControl);

    /**
     * Get dragging status.        
     *
     * @since S60 V4.0        
     * @param None
     * @return ETrue if is dragging, otherwise EFalse
     */                                                      
    inline TBool IsDragging();        

    /**
     * Set the moving draw interval. 
     *
     * @since S60 V4.0        
     * @param aTime The drawing interval while moving
     */    
    inline void SetRedrawInterval(TTimeIntervalMicroSeconds32 aTime);        

    /**
     * Enable/disable dragging feature. 
     * Dragging is enabled by default when created.
     *
     * @since S60 V4.0        
     * @param aFlag dragging flag. Default value is ETrue
     */                                                              
    inline void EnableDragging(TBool aFlag = ETrue);                

    /**
     * Set bitmap for given status .        
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap. Ownership transferred to dragbar
     * @param The type of the bitmap
     */                                 
    IMPORT_C void SetBitmapL(CFbsBitmap* aBmp, TButtonBmpType aType);

    /**
     * Set mask bitmap for given status . 
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap. Ownership transferred to dragbar
     * @parma aType The type of bitmap
     */                                 
    IMPORT_C void SetMaskBitmapL(CFbsBitmap* aBmp, TButtonBmpType aType);

    //From base class CFepUiBaseCtrl
    
    /**
     * From CFepUiBaseCtrl
     * Update control and it's components rect
     *
     * @since S60 V4.0
     * @param aOffset The offset to be moved
     */                
    IMPORT_C void Move(const TPoint& aOffset);   
    
    //from base class MEventObserver
    
    /**
     * From MEventObserver
     * Handle control event 
     *
     * @since S60 V4.0
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     */            
    IMPORT_C void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl,
                                                     const TDesC& aEventData);
 
    //from base class MTimerHandler
    /**
     * From MTimerHandler
     * HandleTimerOut. Handling time out event
     *
     * @since S60 V4.0
     * @param aTimeType The timer type
     */                    
    IMPORT_C void HandleTimerOut(TInt aTimeType);

    /**
     * From CFepUiBaseCtrl
     * OnDeActivate. Called when the layout is going to be hidden
     *
     * @since S60 v4.0
     */        
    IMPORT_C void OnDeActivate();
    
    /**
     * Cancel pointer down event.
     * This will be called when a control has pointer down but pointer up 
     * event happens in other ways. If overrided, this base implementation 
     * must be called in order to report the event.
     *
     * @since S60 V4.0
     * @param aFocusedFlag ETrue if got focused, otherwise EFalse
     */                                
    IMPORT_C virtual void CancelPointerDownL(); 
    
    /**
     * Recalculate moving area. It's called when it's observer changes size.
     *
     * @since S60 v4.0
     */             
    void ReCalculateDragArea();    

protected:
    /**
     * Constructor
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control. Ownership not transferred
     * @param aControlId control Id
     */    
    IMPORT_C CDragBar(const TRect& aRect,CFepUiLayout* aUiLayout,
                                                            TInt aControlId);

    /**
     * Second phrase constructor
     *
     * @since S60 V4.0
     */
    IMPORT_C void ConstructL();

    /**
     * Test whether the moving destination is valid
     *
     * @since S60 V4.0
     * @param aPt The destionation point
     * @param aVInfo The valid information in vertial dirtection
     * @param aHInfo The valid information in horizontal dirtection
     * @return ETrue if valid, otherwise EFalse 
     */           
    IMPORT_C TBool IsValidDestination(const TPoint& aPt,TBool& aVInfo,
                                                               TBool& aHInfo);
    

    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer move event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
private:    
    /**
     * Draw control frame 
     * 
     * @since S60 V4.0
     * @param aFrameRect The rect to be drawn
     * @param aDrawFlag Draw or erase flag. ETrue if draw
     * @return The affected rect
     */            
    TRect DrawFrame(const TRect& aFrameRect, TBool aDrawFlag = ETrue);

    /**
     * Show or hide the moving indicator 
     *
     * @since S60 V4.0
     * @param aDrawFlag ETrue if show the moving indicator. Default value ETrue
     */                    
    void ShowMovingIndicator(TBool aDrawFlag = ETrue);

    /**
     * Add a control to the dragging list
     *
     * @since S60 V4.0
     * @param aCtrl The control to be added
     */                    
    void AddToDraggingCtrlList(CFepUiBaseCtrl* aCtrl);
    
    /**
     * Do preparation before dragging 
     *
     * @since S60 V4.0
     */                        
    void PrepareDragging();
    
    /**
     * Restore states after dragging
     *
     * @since S60 V4.0
     */                        
    void AfterDragging();

private:
    /**
     * dragging flag
     */
    TBool iIsDragging;

    /**
     * Moving offset when drag bar is moving.
     */
    TPoint iMoveOffset;

    /**
     * dragging start position
     */
    TPoint iDraggingStartPt;
    
    /**
     * Position change observer list
     */
    RPointerArray<MPositionObserver> iPosObserverList;
    
    /**
     * The rect to be drawn when dragging. It's the rect of whole dragging area
     */
    TRect iDragFrameRect;
    
    /** 
     * Moving flag, is it moving.
     */
    TBool iMoved;
    
    /**
     * Flag tells whether whole layout is being draged
     */
    TBool iLayoutBeingDraged;
    
    /**
     * Long time press event flag. 
     * When this is enabled, dragbar starts move only after long press time out
     */
    TBool iNeedLongtimePress;
    
    /**
     * Long time press timer interval
     */
    TTimeIntervalMicroSeconds32 iLongtimePressTimeout;
    
    /**
     * Flag tells whether long pressing happend
     */
    TBool iLongPressed;       

    /**
     * Draing enabler flag. Only this flag set can dragbar movable.
     * Default value is ETrue
     */
    TBool iDraggingEnabled;

    /**
     * The position where moving indidator is displayed
     */
    TPoint iMovingIndicatorPos;

    /**
     * Currnt pointer position. Need to kept since we use timely drawing.
     */
    TPoint iCurrentPointerPos;

    /**
     * Redraw timer interval. 
     * While moving, it's not drawn if another redraw event comes
     */
    TTimeIntervalMicroSeconds32 iDrawInterval;        
    
    /** 
     * A list keeps all the control belongs to the dragging area
     */    
    RPointerArray<CFepUiBaseCtrl> iDraggingCtrlList;    
 
    /**
     * long press timer
     * Own.
     */
    CLayoutTimer* iLongPressTimer;    
    
    /**
     * Moving indicator bitmap
     * Own.
     */
    CFbsBitmap* iMovingIndicatorBmp; 
    
    /**
     * Moving indicator mask bitmap
     * Own
     */
    CFbsBitmap* iMovingIndicatorMaskBmp;    
    
    /** 
     * Redraw timer for moving
     * Own
     */
    CLayoutTimer* iMoveTimer;

    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;	       
    };
//end of CDragBar

#include <peninputlayoutbutton.inl>
#endif //C_CBUTTONBASE_H