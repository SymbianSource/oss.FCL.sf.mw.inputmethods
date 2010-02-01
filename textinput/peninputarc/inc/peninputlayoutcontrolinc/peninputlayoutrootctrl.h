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
* Description:  header file of control group and button group 
*
*/

#ifndef C_CFEPUILAYOUTROOTCTRL_H
#define C_CFEPUILAYOUTROOTCTRL_H

#include <peninputlayoutctrlgroup.h>
#include <gulgcmap.h>
class CDragBar;
class CCoeControl;
class CFepUiCursor;
/**
 *  CFepUiLayoutRootCtrl
 *
 *  Root control of the layout. It's a control group will consists of several controls.
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 V4.0
 */
NONSHARABLE_CLASS(CFepUiLayoutRootCtrl) : public CControlGroup,
                             public MEventObserver
    {    
public: 
    //constructor and destructor
    /**
     * Factory function.        
     *
     * @since S60 V4.0     
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @return An instance of CFepUiLayoutRootCtrl class        
     */                           
    static CFepUiLayoutRootCtrl* NewL(CFepUiLayout* aUiLayout);    

    /**
     * Destructor
     *
     * @since S60 V4.0        
     */                                   
    virtual ~ CFepUiLayoutRootCtrl();

    /**
     * HandleEventL. Handle UI event.
     * There are only 4 kinds of UI event right now: Initialization, raw event, 
     * pointer and pointer buffer event. 
     *
     * @since S60 v4.0
     * @param aType The event type. See TEventType
     * @param aEventData The event data.
     * @return The control which processes the event
     */
    TBool HandleEventL(TEventType aType, const TAny* aEventData);
     
     										 
    /**
     * Set the control which has pointer down event
     *
     * @since S60 v4.0
     * @param aCtrl The control to be set. NULL if pointer up 
     * happens in other control, and this will cause a canceling pointer down operation
     * in that control which has the pointer down event.
     */
    void SetPointerDownCtrlL(CFepUiBaseCtrl* aCtrl);           
    
    /**
     * Register to layout to require updaing control valid region constantly.
     *
     * @since S60 v4.0
     * @param aCtrl The contrl which want to update region.
     * @param aRequiredFlag Register or deregister. ETrue if register.
     */        
    IMPORT_C void RequireRegionUpdateL(CFepUiBaseCtrl* aCtrl,TBool aRequiredFlag);    
    
     
    /**
     * Draw contents within the rect.
     *
     * @since S60 v4.0		
     * @param aRect The rect to be drawn in.		
     * @param aDrawFrameFlag The flag which tells whether draw content of draw frame only
     */             
    void DrawRect(const TRect& aRect,TBool aDrawFrameFlag = EFalse); //internal use only

    /**
     * AddControlL
    
    
    /**
     * RemoveControl
     * Remove a control from layout.
     * Remove a control after layout initialization is not supported right now
     *
     * @since S60 v4.0
     * @param aControl The control to be removed.     
     */          
    void RemoveControl(CFepUiBaseCtrl* aControl);

    /**
     * RemoveControl
     * Remove a control from layout.
     * Remove a control after layout initialization is not supported right now
     *
     * @since S60 v4.0
     * @param aControl The control to be removed.     
     */          
    void RemoveControl(CCoeControl* aControl);    

    /**
     * Set layout's rectangle
     *
     * @since S60 V4.0
     * @param aRect The new control area
     */                                       
    void SetRect(const TRect& aRect);
       
    /**
     * Init layout control 
     * Do layout control initialization. It's called by layout when layout 
     * is initialized.
     *
     * @since S60 v4.0
     * @return The layout rect.
     */
    TRect InitControl();
     
    /**
     * Capture or release the pointer
     *
     * @since S60 v4.0
     * @param aControl The control who wants the operation.
     * @param aFlag  Flag indicates capture or release. ETure if capturing.
     * @return The control which captures the pointer before.
     */                                                    
    CFepUiBaseCtrl* CapturePointer(CFepUiBaseCtrl* aCtrl,TBool aFlag = ETrue);

    /**
     * Get the control which captures the pointer
     *
     * @since S60 v4.0
     * @return The control which captures the pointer.
     */                                                    
    inline CFepUiBaseCtrl* CtrlCapPointer();
         
    /*
     * Get the max editor length which the input context field can hold.
     * Called by CFepUiLayout
     *
     * @since S60 v4.0
     * @return The editor length
     */
    TInt GetInputContextFieldMaxLen();
    
    /**
     * Test whether the position is valid. 
     * This is used to test the target position when draging controls to some place
     *
     * @since S60 v4.0
     * @param aRect The rect to be checked.
     * @param aDragbar The dragbar being dragged
     * @param aVInfo The valid information in vertial dirtection
     * @param aHInfo The valid information in horizontal dirtection     
     * @return ETue if no confliction. Otherwise, EFalse                
     */                                             
    TBool IsValidDestination(const TRect& aRect,CDragBar* aDragBar,
                                            TBool& aVInfo,TBool& aHInfo); 
    
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
     * Get the region where user can't start the drawing.
     * 
     * @since S60 V4.0
     * @return The region required
     */    
    inline const TRegion& NonHwrStartingPtRegion();
    
    //from base class CFepUiBaseCtrl    
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */              
    virtual void Draw();
        
    /**
     * From CFepUiBaseCtrl
     * Update control's valid region when other control hiding or displaying.
     * This is usually used for HWR window to change it's writing area.
     *
     * @since S60 V4.0
     * @param aCtrl The control whose displaying status changed
     * @param bFlag ETrue if control is hiden, otherwise EFalse
     */        
     virtual void UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag);    
    
    //from base control MPositionObserver    
    /**
     * From MPositionObserver
     * Handle position change event
     *
     * @since S60 v4.0
     * @param aOffset The moving offset
     * @return The flag whether this observer has processed the event.
     *         ETrue if processed.Otherwise EFalse.
     */            
    TBool HandlePositionChange(const TPoint& aOffset);    


    //from base class MEventObserver
    /**
     * From MEventObserver
     * Handle control event 
     *
     * @since S60 v4.0
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     */            
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);    
    
    /**
     * From CFepUiBaseCtrl
     * OnActivate. Called when the layout is going to be shown.
     *
     * @since S60 v4.0
     */        
    IMPORT_C void OnActivate();
    
    /**
     * From CFepUiBaseCtrl
     * OnDeActivate. Called when the layout is going to be hidden
     *
     * @since S60 v4.0
     */        
    IMPORT_C void OnDeActivate();   
    
    CWindowToBitmapMappingGc* GetRWindowGcL();
    
    /**
     * Handle system resource change.
     *
     * @since S60 V5.0
     * @param aType The system resource change type
     */
    void HandleResourceChange(TInt aType);
    
    /**
     * Public for layout 
     *
     * @since S60 V5.0
     */
     void GraphicDeviceSizeChanged();
    virtual void BringToTopInGroup(CFepUiBaseCtrl* aCtrl);
    virtual void BringToBackInGroup(CFepUiBaseCtrl* aCtrl);

    CFepUiCursor* CreateCursor();     
protected:
    /**
     * 2nd phase constructor
     *
     * @since S60 v4.0
     */                
    void ConstructL();               

private:
    /**
     * Default constructor.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control
     */                                   
    CFepUiLayoutRootCtrl(CFepUiLayout* aUiLayout);
    //from CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl    
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */                                        
    CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl    
     * Handle pointer move event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl    
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);

    void DoRegionUpdateForCtrls();
    
    void RegisterICFEditor();
    
    void InsertToPopList(CFepUiBaseCtrl* aCtrl);  
    
    CFepUiCursor* CreateCursorL();
    TBool ClipCursorForCtrlNeeded(CFepUiBaseCtrl* aCtrl, 
								  const TBool aBringToTopFlag,
								  TInt& aCursorIndex);    
private:      

    /**
     * Indicate whether the pointer down event should be canceled.
     */
    TBool iPointerDownCanceled;
    
    TBool iShadowShown;
    /**
     * Control list for controls which require updating rect constantly
     */
    RPointerArray<CFepUiBaseCtrl> iCtrlRegionAwareList;
    
    /**
     * The control who has captured the pointer.
     * Not own
     */
    CFepUiBaseCtrl* iPointerCaptureCtrl; 
    
    /**
     * Layout position. To keep the position when layout moving
     */
    TPoint iLayoutPos;


    CFepUiCursor* iCursor;

    /**
     * Control list for controls which require to be aware any overlapping change
     * due to other controls show/hide.
     */    
    RPointerArray<CFepUiBaseCtrl> iCtrlOverlappingAwareList;        
    
    CWindowToBitmapMappingGc* iWindowGc;
    };
    
#include "peninputlayoutrootctrl.inl"    
//end of classd CFepUiLayoutRootCtrl

#endif //C_CFEPUILAYOUTROOTCTRL_H