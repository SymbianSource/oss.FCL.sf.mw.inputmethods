/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of base controls
*
*/


#ifndef C_CFEPUIBASECTRL_H
#define C_CFEPUIBASECTRL_H

#include <e32base.h>
#include <w32std.h>
#include <w32adll.h>
#include <peninputuilayoutownerinf.h>
#include <peninputuilayoutbase.h>
#include <peninputlayoutminterface.h>

class CFepUiLayout;
class CLayoutTimer;
class CCandidateWnd;
class CFepUiLayoutRootCtrl;
class CCoeControl;

const TInt KDefaultFrameWidth = 1;
// There are 8 shadow bitmaps
const TInt KShadowBitmapTotalNum = 8;
//for mask bitmap
const TUint32 KTransparentColor = 0x0;
const TUint32 KOpaqueColor = 0xffffff;

#define KDEFAULTBASELINERATIO 3/4
//base control type
typedef TInt64 TControlType;

//UI layout leave code
enum TUiLayoutErrorCode
    {
    EUiLayoutNotReady = -2000,
    EUiLayoutOwnerNotReady,
    EUiLayoutBadControlIndex,
    EUiLayoutBadEvent,
    EUiLayoutBadControlType,
    EUiLayoutCandiateNumIsNotSet,    
    EUiLayoutFuncNotImplemented,
    EUiLayoutBadKeyBmpIndex,
    EUiLayoutNoShadowBmp,
    EUiLayoutNoKey,
    EUiGeneralIndexError,
    EUiFullscreenWithoutHWRWnd,
    EUiLayoutBadParentType,
    EUiNullParam,
    EUiCursorNoOwner,
    EUiNoparent
    };

//define base control types
//Derived class should not re-assign new ctrl type.
enum TStockBaseCtrlType
    {
    ECtrlBaseCtrl           = 0x0001,
    ECtrlTactileFeedback    = 0x0002,    
    ECtrlHwrWnd             = 0x0004 | ECtrlBaseCtrl,     //HWR writing window 
    ECtrlTransparentHwrWnd  = 0x0008 | ECtrlHwrWnd,       //HWR transparent window
    ECtrlCandidateList      = 0x0010 | ECtrlBaseCtrl,     //Candidate list
    ECtrlButton             = 0x0020 | ECtrlBaseCtrl | ECtrlTactileFeedback,     //Button
    ECtrlDragBar            = 0x0040 | ECtrlButton,       //drag bar
    ECtrlControlGroup       = 0x0080 | ECtrlBaseCtrl/* | ECtrlTactileFeedback*/,      //control group    
//    ECtrlButtonGroup        = 0x0080 | ECtrlControlGroup,       //button group    
    ECtrlEditAreaBase       = 0x0100 | ECtrlBaseCtrl | ECtrlTactileFeedback,		//editor base
    ECtrlInputContextArea   = 0x0200 | ECtrlEditAreaBase,   //input context area
    ECtrlVirtualKeyboard    = 0x0400 | ECtrlBaseCtrl,   	//virtual keyboard
    ECtrlPopupWindow		= 0x0800 | ECtrlBaseCtrl,   	//pop up window
    ECtrlPopupChoiceList	= 0x1000 | ECtrlPopupWindow | ECtrlTactileFeedback,   	//pop up choice list
    //ECtrlVirtualItutKeypad  = 0x2000 | ECtrlVirtualKeyboard,
    ECtrlVirtualKey         = 0x2000 | ECtrlBaseCtrl | ECtrlTactileFeedback,       //virtual key control
    ECtrlVirtualItutKey     = 0x4000 | ECtrlVirtualKey,		//virtual itut key control
    ECtrlBackgroundCtrl	    = 0x8000,
    ECtrlCursor             = 0x00010000 | ECtrlBaseCtrl,   //editor cursor
    
    //Add more stock controls here. E.g ECtrlOskKey for VKB key
    ECtrlWndBasedCtrl           = 0x00040000,
    EUserCtrlBase = 0x00080000
    };

//button type
enum TButtonBmpType
    {    
    EBtnBmpNonActive ,                //bitmap for non active control
    EBtnBmpNonActivePressed,      //bitmap for pressed active control, 

    EBtnBmpActive,                //bitmap for active control
    EBtnBmpActivePressed,         //bitmap for pressed active control
                                
    EBtnBmpDisabled,              //bitmap for disabled control
    EBtnBmpDimmed = EBtnBmpDisabled,    

    EBtnBmpMovingIndicator,

    EBtnBmpLastType = EBtnBmpMovingIndicator                 
    };

//control event type
enum TControlEvent
    {
    EEventButtonDown,               //for button
    EEventButtonUp,                 //for button
    EEventDraggingStart,             //for drag bar
    EEventDragging,                   //for drag bar
    EEventDraggingEnd,                //for drag bar
    EEventVirtualKeyUp,             //for virtual key
    EEventVirtualKeyDown,           //for virtual key
    EEventHwrStrokeStarted,         //for hwr window
    EEventHwrStrokeFinished,        //for hwr window
    EEventHwrCharacterTimerOut,     //for hwr window
    EEventCandidateSelected,        //for candidate window
    EEventButtonLongPress,          //for dragbar long press event  

    EEventSizeChanged,              //for base control size change
    EEventRegionUpdated,            //region has been updated

    EEventSetAppCursorSelection,    //for app cursor selection changing

    EEventControlFocusGained,
    EEventControlFocusLost,
    
    //Cancel button down event, this happends when button up in another control
    EEventButtonDownCanceled,  
    EEventHwrStrokeCanceled,     
    
    EEventControlDestroyed,
    
    EEventChoiceSelected,
    EEventPointerLeftControl,      //Pointer leaves a control after button down
    EEventPointerEnterControl,      //pointer re-enters a control after button down
    
    EEventVirtualKeyLatched,    //pointer down in a latched virtual key
    EEventVirtualKeyUnLatched,      //point down in a unlatched virtaul key
    EEventICFSelectionChanged,  //ICF selection has been changed   
    EEventRawKeyDownEvent,		//raw ITU-T key down event  
    EEventRawKeyUpEvent,		//raw ITU-T key up event  
    EEventPointerDownOnNomatch, //pointer down on ICF nomatch range
    EEventPointerDownOnInline, // pointer down on ICF inline, but not in nomatch range
    EEventCapturePointer,
    EEventIcfPointerUpEvent,		   //Point up in icfevent
    EEventTooltipPointerUp,
    EEventTooltipPointerLeave,
    EEventTooltipPointerEnter,
    EEventCommitUnderLine,
    EEventInsertSpace,
    EEventControlExtBase = 1000,    //extended event for common control
    EEventControlUserBase = 2000    //user event starts from here
    };
    
//shadow bitmap index
/****************************************************************************/
// A shadow is drawn by 8 bitmaps. Their position is defined here
//
//      tl  tm  tr
//      lm      rm
//      bl  bm  br
//
/****************************************************************************/

enum TShadowBitmapIndex
    {
    KShadowBmpTl,   //top left bitmap
    KShadowBmpTm,   //top middle bitamp     
    KShadowBmpTr,   //top right bitmap

    KShadowBmpLm,   //left middle bitmap
    KShadowBmpRm,   //right middle bitmap    
        
    KShadowBmpBl,   //bottom left bitmap
    KShadowBmpBm,   //bottom middle bitmap    
    KShadowBmpBr,   //bottom right bitmap   
    };

/**
 *  CFepUiBaseCtrl
 *
 *  Base control for all FEP UI layout controls.
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 v4.0
 */
class CFepUiBaseCtrl : public CBase, public MPositionObserver,
                        public MOnResourceChangFunctions
    {
friend class CControlGroup;    
friend class CInsertionPoint;
public:
    enum TZOrder
        {
        EOrderBackground = 1500, //order for background control
        EOrderNomal = 1000, //default z-order for control
        EOrderFront = 500,  // front order for control
        EOrderTopMost = 0  // for top most control
        };

public:
    /**
     * Constructor
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control. Ownership not transferred
     * @param aControlId control Id
     */    
    IMPORT_C CFepUiBaseCtrl(const TRect& aRect,CFepUiLayout* aUiLayout,
                                                            TInt aControlId);

    /**
     * Constructor. Used internally by CControlGroup
     *
     * @since S60 V4.0
     * @param aControl The control to be added. The control must set its extent
     *                  before added to the layout.
     * @param aUiLayout Ui layout who contains this control. Ownership not transferred               
     * @param aOwnership The ownership of the window control. ETrue if it's transfered 
     * to this control
     */                                                                    
     CFepUiBaseCtrl(CCoeControl* aControl, CFepUiLayout* aUiLayout,TBool aOwnership = ETrue);
                                                            
    /**
     * Do base contruction. Allocate event data buffer
     * Each derived control must call this in its contruction time
     * 
     * @since S60 v4.0
     */
    IMPORT_C void BaseConstructL();    

    /**
     * Destructor.
     *
     * @since S60 V4.0        
     */    
    IMPORT_C virtual ~CFepUiBaseCtrl();           
        
    /**
     * Pure virtual function, draws the control
     *
     * @since S60 V4.0
     */               
     IMPORT_C virtual void Draw();
     
    /**
     * Redraws the content in a control
     *
     * @since S60 V4.0
     * @param aRect The rect to be redrawed
     */               
     IMPORT_C virtual void ReDrawRect(const TRect& aRect);     
        
    /**
     * Capture or release the pointer. 
     * After capturing, all UI layout event will be processed by this control
     *
     * @since S60 V4.0
     * @param aFlag The flag indicates capturing or releasing.
     * @return The control which captures the pointer before.
     */               
    IMPORT_C CFepUiBaseCtrl* CapturePointer(TBool aFlag = ETrue);

    /**
     * Clear the whole control area
     *
     * @since S60 V4.0
     */        
    IMPORT_C void Clear();
   
    /**
     * Clear a area in the control
     *
     * @since S60 V4.0
     * @param aRect The rect to be cleared.
     */                        
    IMPORT_C void ClearRect(const TRect& aRect);

    /**
     * Hide or shoe control. A hiden control will not handle the event.
     *
     * @since S60 V4.0
     * @param aFlag ETrue if want to hide the control.
     */                                               
    IMPORT_C virtual void Hide(TBool aFlag);

    /**
     * Bring the control to top most in current control group and show it.
     *
     * @since S60 V5.0
     */                                               
    IMPORT_C void BringToTop();

    /**
     * Bring the control to back most in current control group and show it.
     *
     * @since S60 V5.0
     */                                                   
    IMPORT_C void BringToBack();
    /*
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

    /**
     * Check whether this control contains the point
     *
     * @since S60 V4.0
     * @param aPoint The point to be checked
     * @return ETrue if control valid region contains the point, otherwise EFalse
     */
    IMPORT_C virtual TBool Contains(const TPoint& aPt);

    /**
     * Set control's rectangle
     *
     * @since S60 V4.0
     * @param aRect The new control area
     */                                       
    IMPORT_C virtual void SetRect(const TRect& aRect);
    
    /**
     * Set shadow rect. Shadow rect defines the outside rect of the control.
     * The shadow is drawn between this rect and control's rect using bitmaps
     *
     * @since S60 V4.0
     * @param aRect The shadow rect. Shadow rect must bigger than control's
     *        rect, otherwise no effect.
     */    
    IMPORT_C void SetShadowRect(const TRect& aRect);        

    /**
     * Set shadow rect for a shadow area.
     * The nth shadow is drawn in this shadow rect
     *
     * @since S60 V4.0
     * @param aRect The shadow rect.
     * @param aIndex The index position for shadow rect 
     */        
    IMPORT_C void SetShadowAreaRect(const TRect& aRect, TShadowBitmapIndex aIndex);

    /**
     * Add event obeserver.
     * The observer will get notified when control event happens.
     *
     * @since S60 V4.0
     * @param aObserver An event obeserver. Ownership not transferred
     */                                       
    IMPORT_C void AddEventObserver(MEventObserver* aObserver);
            
    /**
     * Remove event obeserver
     *
     * @since S60 V4.0
     * @param aObserver An event obeserver to be removed.
     */                                                       
    IMPORT_C void RemoveEventObserver(MEventObserver* aObserver);
    
    /**
     * Report event to event observer
     *
     * @since S60 V4.0
     * @param aEventType the event type
     * @param aEventData Event data
     */
    IMPORT_C void ReportEvent(TInt aEventType, const TDesC& aEventData = KNullDesC);
    
    /**
     * Update control's valid region when other control hiding or displaying.
     * This is usually used for HWR window to change it's writing area.
     *
     * @since S60 V4.0
     * @param aCtrl The control whose displaying status changed
     * @param bFlag ETrue if control is hiden, otherwise EFalse
     */        
    IMPORT_C virtual void UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag);
    
    /**
     * Move conntrol
     *
     * @since S60 V4.0
     * @param aOffset The offset to be moved
     */                
    IMPORT_C virtual void Move(const TPoint& aOffset);

    /**
     * Set control active
     *
     * @since S60 V4.0
     * @param aFlag The active flag. ETrue if active, otherwise EFalse
     */                                
    IMPORT_C virtual void SetActive(TBool aFlag);   


    /**
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * Handle pointer move event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
           
    
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
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V4.0
     * @param aPoint current pointer position
     */
    IMPORT_C virtual void HandlePointerLeave(const TPoint& aPoint);
    
    /**
     * Handle pointer enter event. 
     * This will be called only when it's got pointer down event before and 
     * pointer leaves but now comes back without pointer up event.
     *
     * @since S60 V4.0
     * @param aPoint current pointer position     
     */
    IMPORT_C virtual void HandlePointerEnter(const TPoint& aPoint);    
    

    /**
     * Register to layout to require updaing control valid region constantly.
     *
     * @since S60 v4.0
     * @param aRequiredFlag Register or deregister. ETrue if register.
     */        
    IMPORT_C void RequireRegionUpdateL(TBool aRequiredFlag);
    
    /**
     * Set Tactile Feedback Type
     * Advanced Tactile feedback REQ417-47932
     * 
     * @since S60 v4.0
     * @param aTactileType.
     */
    IMPORT_C void SetTactileFeedbackType(TInt aTactileType);
    
    /**
    * Return control type
    *
    * @since S60 V4.0
    * @return The control type
    */                               
    inline TControlType ControlType();
    
    /**
    * Return tactile feedback type
    *
    * Advanced Tactile feedback REQ417-47932
    * @return the tactile feedback type
    */                               
    inline TInt TactileFeedbackType() const;

    
    /**
     * Get control Id
     *
     * @since S60 V4.0
     * @return The control id
     */                                       
    inline TInt ControlId();
    
    /**
     * Get control size in rect
     *
     * @since S60 V4.0
     * @return The control size in rect
     */                                      
    inline const TRect& Rect();    
        
    /**
     * Get valid region. A valid region is that the region responds to pointer.
     *
     * @since S60 V4.0
     * @return The control valid region
     */
    inline const RRegion& Region();    
    /**
     * Set background color
     *
     * @since S60 V4.0
     * @param aCol Background color to be used
     */                                       
    inline void SetBkColor(const TRgb& aCol);
   
    /**
     * Set mask background color
     *
     * @since S60 V4.0        
     * @param aCol The mask back ground color
     */
    inline void SetMaskBkCol(const TRgb& aCol);
   
    /**
     * Set pen size
     *
     * @since S60 V4.0
     * @param aSize New pen size
    */                                       
    inline void SetPenSize(const TSize& aSize);
    
    /**
     * Get pen size
     *
     * @since S60 V4.0
     * @return Pen size
     */                
    inline const TSize& PenSize();        
  
    /**
     * Set pen color
     *
     * @since S60 V4.0
     * @param aCol New pen color    
     */                                       
    inline void SetPenColor(const TRgb& aCol) ;

    /**
     * Set border size
     *
     * @since S60 V4.0
     * @param aSize New border size
    */                                       
    inline void SetBorderSize(const TSize& aSize);
    
    /**
     * Get border size
     *
     * @since S60 V4.0
     * @return Border size
     */                
    inline const TSize& BorderSize();    
 
    /**
     * Set pen color
     *
     * @since S60 V4.0
     * @param aCol New border color    
     */                                       
    inline void SetBorderColor(const TRgb& aCol) ;


    /**
     * Get pen color
     *
     * @since S60 V4.0
     * @return border color    
     */                                       
    inline const TRgb& BorderColor() ;
  
    /**
     * Get hide flag
     *
     * @since S60 V4.0
     * @return ETrue if control is hiden, otherwise EFalse
     */                                              
    inline TBool Hiden();
        
    /**
     * Get initialization status
     *
     * @since S60 V4.0
     * @return Control initialization status. ETrue if initialized, otherwise EFalse.
     */
    inline TBool Ready();

    /**
     * set control drawing status. It won't be drawn if not ready
     *
     * @since S60 V4.0
     * @param aReadyFlag The control ready flag
     */
    inline void SetReady(TBool aReadyFlag);
            
    /**
     * Test whether the control belongs one kind of control class
     *
     * @since S60 V4.0
     * @param aCtrlClass The control class type
     * @return ETrue if the control belongs to the class, otherwise EFalse
     */
    inline TBool IsKindOfControl(TStockBaseCtrlType aCtrlClass);       


    /**
     * Get active status
     *
     * @since S60 V4.0
     * @return active status
     */    
    inline TBool IsActive();

    /**
     * Set control type
     *
     * @since S60 V4.0
     * @param aCtrlType The control type
     */        
    inline void SetControlType(TControlType aCtrlType);

    /**
     * get control pointer down status
     *
     * @since S60 V4.0
     * @return The pointer down status. ETrue if has pointer down
     */    
    inline TBool PointerDown();

    /**
     * change control pointer down status
     *     
     * @since S60 V4.0
     * @param aFlag The pointer down flag.
     */    
    inline void SetPointerDown(TBool aFlag);
   
    
    /**
     * Set control valid region
     *     
     * @since S60 V4.0
     * @param aRegion The new control region
     */   
    inline void SetRegion(const RRegion& aRegion);
    
    /**
     * Tell whether it allows other control moving on top of it
     *
     * @since S60 V4.0
     * @return Overlap flag
     */
    inline TBool AllowOverlap();
    
    /**
     * Set overlap flag
     *
     * @since S60 V4.0
     * @param aFlag The new overlap falg
     */
    inline void SetAllowOverlap(TBool aFlag);
    
    
    // from base class MPositionObserver

    /**
     * From MPositionObserver
     * Handle position change event.
     * It's called to change the position after it's being dragging 
     *
     * @since S60 V4.0
     * @param aOffset The moving offset
     * @return The flag whether this observer has processed the event. ETrue if processed.
     */            
    IMPORT_C TBool HandlePositionChange(const TPoint& aOffset);
    
    /**
     * From MPositionObserver
     * Get observer's area to be moved when position changed
     *
     * @since S60 V4.0
     * @return The rect of observer's moving area
     */            
    IMPORT_C const TRect& MovingArea();
    
    /**
     * From MPositionObserver
     * Tell whether this observer is a fep base control
     *
     * @since S60 v4.0
     * @return ETrue if this observer is a control, otherwise, EFalse.
     */
    IMPORT_C TBool IsFepBaseControl();   

    /**
     * Get shadow rect
     * @since S60 V4.0
     * @return control shadow rect
     */                        
    inline const TRect& ShadowRect();

    /**
     * set control shadow bitmap and mask bmp
     * @since S60 V4.0
     * @param The shadow background bitmap. Ownership transferred
     * @param The shadow background mask bitmap. Ownership transferred
     * @param aIndex The shadow bitmap position index
     */   
    IMPORT_C void SetShadowBmp(CFbsBitmap* aBmp,CFbsBitmap* aMaskBmp,
                                TShadowBitmapIndex aIndex);
    
    /**
     * OnActivate. Called when the layout is going to be shown.
     *
     * @since S60 v4.0
     */        
    IMPORT_C virtual void OnActivate();
    
    /**
     * OnDeActivate. Called when the layout is going to be hidden
     *
     * @since S60 v4.0
     */        
    IMPORT_C virtual void OnDeActivate();
    /**
     * Update layout area, cause screen update. However the updating is buffered until 
     * refresh timer time out.
     *
     * @since S60 V4.0
     * @param aRect the rect to be flushed in screen
     * @param aUpdateFlag ETrue if full update.
     */
    IMPORT_C void UpdateArea(const TRect& aRect,TBool aUpdateFlag= EFalse); 

    /**
     * Update layout area immediately
     *
     * @since S60 V4.0
     * @param aRect the rect to be flushed in screen
     * @param aUpdateFlag ETrue if full update.
     */
    IMPORT_C void UpdateAreaImmed(const TRect& aRect,TBool aUpdateFlag= EFalse); 
    
    /**
     * Get shadow area rect
     *
     * @since S60 V4.0
     * @param aIndex The index for shadow rect
     */
    inline TRect ShadowAreaRect(TShadowBitmapIndex aIndex);
    
    /**
     * Call back function when layout starts dragging
     *
     * @since S60 V4.0
     */
    IMPORT_C virtual void OnLayoutDraggingStart();

    /**
     * Call back function when layout ends dragging
     *
     * @since S60 V4.0
     */
    IMPORT_C virtual void OnLayoutDraggingEnd();
    
    /**
     * Get window based control
     *
     * @since S60 V4.0
     * @return The window based control associated with this control
     */
    inline CCoeControl* WndBasedControl();
    
    // from base class MOnResourceChangFunctions
    
    /**
     * From MOnResourceChangFunctions
     * Set resource Id
     *
     * @since S60 V4.0
     * @param aResId The resource id;
     */                
    IMPORT_C void SetResourceId(TInt32 aResId);    
    
    /**
     * Get hide flag
     *
     * @since S60 V4.0
     * @return ETrue if all controls are hiden, otherwise EFalse
     */ 
    IMPORT_C TBool WholeHiden();
    
    /**
     * From MOnResourceChangFunctions
     * Construct from resource
     *
     * @since S60 V4.0
     */                
    IMPORT_C void ConstructFromResourceL();
    
    /**
     * Get resource Id
     *
     * @since S60 V4.0
     * @return The resource id;
     */                
    inline TInt32 ResourceId();
    
    IMPORT_C void SetParent(CFepUiBaseCtrl* aParent);
    
    inline CFepUiBaseCtrl* ParentCtrl();
    
    /**
     * Get the associated CCoeControl control
     *
     * @since S60 V5.0
     * @return The CCoeControl it contains;
     */                    
    inline CCoeControl* WndControl();
    
    /**
     * Simulates a internal raw event. The event is handled inside pen input ui,
     * will not go for window server.
     *
     * @since S60 V5.0
     * @param aPtEvent The simulated raw event
     */
    IMPORT_C void SimulateRawEvent(const TRawEvent& aEvent);    
    
    IMPORT_C void SetOrdinalPos(TInt aPos);
    
    inline TInt OrderPos() const;
    
    TInt AbsOrderPos();
    
    /**
     * Check whether this control is on top of another control. If a control is on top of another
     * it will draw later so it will overwrite the overlapped area.
     *
     * @since S60 V5.0
     * @param aCtrl The compared control
     * @return ETrue if this control is on top of the specified  one. 
     */    
    TBool IsOnTopOf(CFepUiBaseCtrl* aCtrl);
    
    /**
     * Check whether this control has overlapped area with another one
     *
     * @since S60 V5.0
     * @param aCtrl The compared control
     * @return ETrue if this control has overlapped area with the specified  one. 
     */        
    TBool Overlapped(CFepUiBaseCtrl* aCtrl);
    
    /**
     * Compare order pos
     *
     * @since S60 V5.0
     * @return ETrue if this control is on top of the specified one.
     */                                                   
    TBool CompareOrder(CFepUiBaseCtrl* aCtrl);
    
    
    /**
     * Compare order pos for two control within the same control grop
     *
     * @since S60 V5.0
     * @return ETrue if this control is on top of the specified one.
     */                                                       
    TBool CompareOrderInSameGroup(CFepUiBaseCtrl* aCtrl);
    
protected:
         
    /**
     * Draw bitmap to control rect
     *
     * @param aBitmap The given bitmap
     * @param aStretchFlag The flag tells whether this bitmap should be stretched
     */  	
	IMPORT_C void DrawBitmap(const CFbsBitmap* aBitmap,TBool aStretchFlag);    
     
    /**
     * Draw bitmap to control rect
     *
     * @param aRect The drawn rect
     * @param aBitmap The given bitmap
     * @param aStretchFlag The flag tells whether this bitmap should be stretched
     */  	
	IMPORT_C void DrawBitmap(const TRect aRect, const CFbsBitmap* aBitmap,TBool aStretchFlag);    
    
    /**
     * Get background color
     *
     * @since S60 V4.0
     * @return Background color
     */                        
    inline const TRgb& BkColor();

    /**
     * get mask background color
     *
     * @since S60 V4.0        
     * @return The mask back ground color
     */
    inline const TRgb& MaskBkCol();       
    
    /**
    * Get pen color
    * @since S60 V4.0
    * @return Pen colore
    */                        
    inline const TRgb& PenColor();

    /**
     * Get Ui layout
     *
     * @since S60 V4.0
     * @return The ui layout
     */        
    inline CFepUiLayout* UiLayout();  
    
    /**
     * Get ui layout root control
     *
     * @since S60 v4.0
     * @param aPos The new position. 
     */                        
    inline CFepUiLayoutRootCtrl* RootControl();        
    
    /**
     * get graphics context for sprite or window
     * @since S60 V4.0
     * @return The graphic context
     */
    inline CBitmapContext* BitGc();

    /**
     * get Bitmap device for sprite or window
     * @since S60 V4.0
     * @return The bitmap device
     */
    inline CFbsBitmapDevice* BitmapDevice();

    /**
     * get Mask bitmap device for sprite or window
     * @since S60 V4.0
     * @return The mask bitmap device
     */
    inline CFbsBitmapDevice* MaskBitmapDevice();
    
    /**
     * get control background mask bmp
     * @since S60 V4.0
     * @return The background mask bitmap
     */
    inline CFbsBitmap* BkMaskBmp();
    
    /**
     * get control background  bmp
     * @since S60 V4.0
     * @return The background bitmap
     */
    inline CFbsBitmap* BackgroundBmp();

    /**
     * set control background  bmp
     * @since S60 V4.0
     * @param The background bitmap to be set. Ownership transferred.
     */   
    inline void SetBackgroundBmp(CFbsBitmap* aBmp);

    /**
     * set control background mask bmp
     * @since S60 V4.0
     * @param The background mask bitmap to be set. Ownership transferred
     */   
    inline void SetBackgroundMaskBmp(CFbsBitmap* aBmp);  
    /**
     * Draw shadow
     *
     * @since S60 V4.0
     */   
    IMPORT_C void DrawShadow();
    
    /**
     * Do base contruction. Allocate event data buffer
     * Each derived control must call this in its contruction time
     * 
     * @since S60 v4.0
     * @param aRoot The root control.
     */
    void BaseConstructL(CFepUiLayoutRootCtrl* aRoot);
    
    /**
     * Draw mask background
     * 
     * @since S60 v4.0
     * @param aBmpStretchFlag Flag tells whether the bitmap will be stretched
     * to fit the rect. Default value is ETrue.
     */    
    IMPORT_C void DrawMaskBackground(TBool aBmpStretchFlag = ETrue);
    
    /**
     * Draw opaque mask background. Draw an opaque ground.
     * 
     * @since S60 v4.0
     * @param aBmpStretchFlag Flag tells whether the bitmap will be stretched
     * to fit the rect. Default value is ETrue.
     */        
    IMPORT_C void DrawOpaqueMaskBackground(TBool aBmpStretchFlag = ETrue);
    
    /**
     * Draw opaque mask background. Draw an opaque ground.
     * 
     * @since S60 v4.0
     * @param aBmpStretchFlag Flag tells whether the bitmap will be stretched
     * @param aRect Rect to be draw
     * to fit the rect. Default value is ETrue.
     */        
    IMPORT_C void DrawOpaqueMaskBackground(const TRect aRect, TBool aBmpStretchFlag = ETrue);
    
    /**
     * Draw transparent mask background
     * 
     * @since S60 v4.0
     * @param aBmpStretchFlag Flag tells whether the bitmap will be stretched
     * to fit the rect. Default value is ETrue.
     */           
    IMPORT_C void DrawTransparentMaskBackground(const TRect& aRect, TBool aBmpStretchFlag = ETrue);

    /**
     * Draw background.
     * 
     * @since S60 v4.0
     * @param aBmpStretchFlag Flag tells whether the bitmap will be stretched
     * to fit the rect. Default value is ETrue.
     */            
    IMPORT_C void DrawBackground(TBool aBmpStretchFlag = ETrue);   

    /**
     * Draw background.
     * 
     * @since S60 v4.0
     * @param aBmpStretchFlag Flag tells whether the bitmap will be stretched
     * @param aRect Rect to be draw
     * to fit the rect. Default value is ETrue.
     */            
    IMPORT_C void DrawBackground(const TRect aRect,TBool aBmpStretchFlag = ETrue);  
        
    /**
     * Test whether control is able to be drawn. e.g, is ready or shown?
     * 
     * @since S60 v4.0
     * @return ETrue if able to be drawn.
     */                
	IMPORT_C TBool AbleToDraw();    
	
    /**
     * Draw background to device
     * 
     * @since S60 v4.0
     * @param aRect The device rect to be filled.
     * @param aDevie The device to be drawn
     * @param aBmp The background bmp
     * @param aBrushCol The brush color
     * @param aPenCol The pen color
     * @param aBmpStretchFlag The stretch flag for drawing bitmap.
     */  	
    void DrawBackgroundToDevice(TRect aRect,CFbsDevice *aDevice, CFbsBitmap* aBmp, 
    		const TRgb& aBrushCol, const TRgb& aPenCol,TBool aBmpStretchFlag);
	
    /**
     * Enable region update
     * 
     * @since S60 v5.0     
     */	
    inline void EnableRegionUpdating();
    
    /**
     * Enable region update
     * 
     * @since S60 v5.0     
     */    
    inline void DisableRegionUpdating();
    
    /**
     * Get the clip region 
     * 
     * @since S60 v5.0     
     * @return The clip region
     */                
    IMPORT_C const TRegion& ValidClipRegion(); 
    
    //From CBase
    /**
     * Get the extension
     *
     * @since S60 v5.0
     * @param aExtensionId Extension UID
     * @param a0 The extension returned
     * @param a1 Any passed in parameter.
     * @return Operation result. Implementation dependent
     */													     
    virtual IMPORT_C TInt Extension_(TUint aExtensionId, TAny *&a0, TAny *a1);    
    /**
     * Handle system resource change
     *
     * @since S60 V5.0
     * @param aType The system resource change type
     */
    virtual IMPORT_C void HandleResourceChange(TInt aType);

    /**
     * Handle pen UI graphic device size change event
     *
     * @since S60 V5.0
     */
    virtual IMPORT_C void GraphicDeviceSizeChanged();
    
    inline void SetHidenFlag(TBool aFlag);
private:
    /**
     * Draw shadow bitmap
     *
     * @since S60 V4.0
     * @param aRect The rect to be drawn
     * @param aIndex The shadow bitmap position index
     */   
    void DrawShadowBitmap(const TRect& aRect,TInt aIndex);
    
protected:
    /**
     * Indicate whether the pointer has leaved a control
     */    
    TBool iPointerLeft;
    /**
     * Control rect. It's relative to ui layout 
     */
    TRect iRect;        

    /**
     * Control shadow rect. It's relative to ui layout 
     */
    TRect iShadowRect;        

    /**
     * Shadow area rect
     */
    TRect iShadowAreaRect[KShadowBitmapTotalNum]; 

    /**
     * CCoeControl based control
     * Own
     */ 
    CCoeControl* iWndControl;

    /**
     * The resource Id associatied with this control
     */
    TInt32 iResourceId;

    /**
     * Control border color
     */
    TRgb iBorderColor;
    
    /**
     * Control border size
     */
    TSize iBorderSize;
    
    /** 
     * Valid region which reacts to the pointer event
     */
    RRegion iValidRegion;   

    /**
     * Parent control
     * Not own
     */
    CFepUiBaseCtrl* iParentCtrl;

    /**
     * Flag indicates whether rect changed should report to parent
     */
    TBool iUpdateFlagEnabled;

    /**
     * The actual drawing region. so drawing won't happend outside of the region.
     */    
    RRegion iClipRegion; 
    
private:    
    /**
     * Control id
     */
    TInt iControlId;
    /**
     * Control type. 
     * User derivated control should starts from EUserCtrlBase
     */
    TControlType iControlType;

    /**
     * Background color
     */
    TRgb iBackgroundCol;

    /**
     * Maks backgroud color
     */
    TRgb iMaskBkCol;

    /**
     * pen size
     */
    TSize iPenSize;

    /**
     * Pen color
     */
    TRgb iPenColor;

    /**
     * Flag indicates whether the control is hiden
     */
    TBool iHiden;
    
    /* 
     * Event type of current event
     */
    TEventType iEventType;

    /**
     * flag to show whether pen has beed pressed down in this control
     */
    TBool iPointerDown;
    
    /**
     * flag to indicate whehter this control is ready to drawn
     */
    TBool iReady;
    
    
    /**
     * control active status.
     */
    TBool iIsActive;    
    
    /**
     * Flag tells whether a draging component can be moved on top of this control
     */
    TBool iAllowOverlap;
    
    /** 
     * event observer list
     */
    RPointerArray<MEventObserver> iEventObserverList;               

    //pointer members

    /**
     * Background bitmap
     * Own.
     */
    CFbsBitmap* iBackgroundBmp;

    /**
     * Background mask bitmap
     * Own.
     */
    CFbsBitmap* iBkMaskBmp;

    /**
     * The ui layout which contains this control
     * Not own.
     */
    CFepUiLayout* iUiLayout;
    
    /** 
     * Root control. It's a control group.
     * Not Own
     */
    CFepUiLayoutRootCtrl* iRootCtrl;        
    /**
     * Shadow bitmaps.
     * Own
     */ 
    CFbsBitmap* iShadowBmp[2*KShadowBitmapTotalNum+1];

    /**
     * Layout owner
     * Not own
     */
    MLayoutOwner* iLayoutOwner;
    
    /**
     * Ownership of the window control. If true, it will delete the window control during
     * destruction.
     */
    TBool iOwnWndCtrl;    
    
    
    //When pop up shown, the locked area will be also reduced from control.
    RRegion iClipRegionWithoutLockedArea;     
    
    
    TInt iOrdinalPos; 
    /**
     * Reservered
     */
    TInt iReservered1;

    /**
     * Reservered
     */
    TInt iReservered2;   
    
    NONSHARABLE_CLASS(CFepUiBaseCtrlExtension)
    	{
    	public: 
    		
		CFepUiBaseCtrlExtension(TInt aTactileType);
		
		/**
		 * Set Tactile Feedback Type
		 * Advanced Tactile feedback REQ417-47932
		 */
		void SetTactileFeedbackType(TInt aTactileType);
		
		/**
		* Return tactile feedback type
		*
		* Advanced Tactile feedback REQ417-47932
		* @return the tactile feedback type
		*/  
		IMPORT_C TInt TactileFeedbackType();
		
    	private:
        /**
         * Tactile Feedback type
         */
        TInt iTactileType;
    	};
    };
//end of class CFepUiBaseCtrl
#include "peninputlayoutbasecontrol.inl"
#endif //C_CFEPUIBASECTRL_H
