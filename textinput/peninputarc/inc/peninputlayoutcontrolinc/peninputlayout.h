/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of peninput ui layout
*
*/


#ifndef C_CFEPUILAYOUT_H
#define C_CFEPUILAYOUT_H

// INCLUDES
#include <s32std.h>
#include "peninputuilayoutownerinf.h"
#include "peninputuilayoutbase.h"
#ifdef RD_TACTILE_FEEDBACK
#include <touchfeedback.h>
#endif //RD_TACTILE_FEEDBACK
#include "peninputlayoutminterface.h"
#include "peninputlayoutbasecontrol.h"
#include "peninputcmdparam.h"

class CFepUiCursor;

class CControlGroup;
class CDragBar;
class CFepUiLayoutRootCtrl;
class CCoeControl;
const TInt KMaxDragBarNum = 4;


/**
 * class CFepUiLayout
 *
 * This is main class for Fep UI framework. 
 *
 * @lib peninputlayoutcontrol.lib
 * @since S60 v4.0
 */
class CFepUiLayout : public CBase, 
                     public MPenUiLayoutBase, 
                     public MEventObserver
    {
public:   
    /**
     * Destructor.
     *
     * @since S60 v4.0
     */
    IMPORT_C virtual ~CFepUiLayout();

    /**
     * BaseConstructL
     * Does the base construction. Derivated class must call this when constructed.
     *
     * @since S60 v4.0
     */
    IMPORT_C virtual void BaseConstructL();    
    
    /**
     * AddControlL
     * Add a control to layout. 
     *
     * @since S60 v4.0
     * @param aControl The control to be added. The ownership transferred to layout
     * @param aEventObserverFlag ETrue if layout is this control's event observer. 
     *                           Otherwise not. Default value is ETrue.
     */   
    IMPORT_C void AddControlL(CFepUiBaseCtrl* aControl, 
                                            TBool aEventObserverFlag = ETrue);

    /**
     * AddControlL
     * Add a control to layout. 
     *
     * @since S60 v4.0
     * @param aControl The control to be added. The ownership transferred to layout
     * @param aEventObserverFlag ETrue if layout is this control's event observer. 
     *                           Otherwise not. Default value is ETrue.
     */   
    IMPORT_C void AddControlL(CCoeControl* aControl, 
                                            TBool aEventObserverFlag = ETrue);
    
    /**
     * RemoveControl
     * Remove a control from layout.
     *
     * @since S60 v4.0
     * @param aControl The control to be removed.     
     */          
    IMPORT_C void RemoveControl(CFepUiBaseCtrl* aControl);

    /**
     * RemoveControl
     * Remove a control from layout.
     *
     * @since S60 v4.0
     * @param aControl The control to be removed.     
     */          
    IMPORT_C void RemoveControl(CCoeControl* aControl);

   
    /**
     * Get ui layout control list
     *
     * @since S60 v4.0
     * @return The control list.
     */                        
    inline const RPointerArray<CFepUiBaseCtrl>& ControlList();
   
    /**
     * Report layout event to layout owner
     *
     * @since S60 v4.0
     * @param aEventType the event type
     * @param aEventData Event data
     */
    IMPORT_C void SignalOwner(TInt aEventType,
                                    const TDesC& aEventData = KNullDesC);
    
    
    /**
     * Test whether the position is valid. 
     * This is used to test the target position when draging controls to some place
     *
     * @since S60 v4.0
     * @param aRect The rect to be checked.
     * @param aDragbar The dragbar being dragged
     * @param aVInfo The valid information in vertial dirtection. ETrue is vertical
     *        moving is valid.It's only used when aRect is not valid but moving in 
     *        vertical direction is valid.
     * @param aHInfo The valid information in horizontal dirtection. ETrue is 
     *        horizontal moving is valid.It's only used when aRect is not 
     *        valid but moving in vertical direction is valid.
     * @param aFlag ETrue if whole layout is moving, otherwise, only components moving
     * @return ETue if aRect is a valid position. Otherwise, EFalse. 
     */                                             
    IMPORT_C virtual TBool IsValidDestination(const TRect& aRect,
                                                CDragBar* aDragBar,
                                                TBool& aVInfo,TBool& aHInfo,
                                                TBool aFlag);

    /**
     * Enable whole layout moved by a dragbar. 
     *
     * @since S60 v4.0
     * @param aDragBar The dragging bar drags the layout
     */                                             
    IMPORT_C void EnableLayoutMoving(CDragBar* aDragbar);


    /**
     * Get ui layout root control
     *
     * @since S60 v4.0
     * @param aPos The new position. 
     */                        
    inline CFepUiLayoutRootCtrl* RootControl();
    
    /**
     * Get the layout owner
     *
     * @since S60 v4.0        
     * @return The layout owner
     */
    inline MLayoutOwner* LayoutOwner();

    /**
     * Set the layout size. It will update region-aware control's valid region
     * It will call owner's LayoutSizeChanged if size changed.
     *
     * @since S60 v4.0
     * @param aRect The layout size. 
     */                    
    IMPORT_C void SetRect(const TRect& aRect);                  
        
    /**
     * Set the layout shadow size. Shadow will draw between shadow rect and 
     * layout rect. Ui layout rect is then changed to shadow rect.
     *
     * @since S60 v4.0
     * @param aRect The layout shadow size.
     */                    
    IMPORT_C void SetShadowRect(const TRect& aRect);  
    
    /**
     * Set shadow rect for a shadow area.
     * The nth shadow is drawn in this shadow rect
     *
     * @since S60 V4.0
     * @param aRect The shadow rect.The rect's coordinate is relative to layout.
     * @param aIndex The index position for shadow rect 
     */        
    IMPORT_C void SetShadowAreaRect(const TRect& aRect, 
                                                TShadowBitmapIndex aIndex);

    
    /**
     * set layout's shadow bitmap and mask bmp
     * @since S60 V4.0
     * @param The shadow background bitmap. Ownership transferred
     * @param The shadow background mask bitmap. Ownership transferred
     * @param aIndex The shadow bitmap position index. See CFepUiBaseCtrl's 
     *         SetShadowBmp the defination of TShadowBitmapIndex
     */   
    IMPORT_C void SetShadowBmp(CFbsBitmap* aBmp,CFbsBitmap* aMaskBmp,
                                TShadowBitmapIndex aIndex);
                            
    
    /**
     * Set whole screen size.
     *
     * @since S60 v4.0
     * @param aSize The screen size
     */
    inline void SetScreenSize(const TSize& aSize);
    
    /**
     * get whole screen size.
     *
     * @since S60 v4.0
     * @return The screen size
     */
    inline const TSize& ScreenSize();   

    /**
     * Lock a area
     *
     * @since S60 v4.0
     * @param aRect The locked area
     * @param aOwner The owner of the locked area
     */
    IMPORT_C void LockArea(const TRect& aRect, CFepUiBaseCtrl* aOwner);

    /**
     * unlock a area
     *
     * @since S60 v4.0
     * @param aRect The locked area
     * @param aCtrl The owner of the locked area
     */    
    IMPORT_C void UnLockArea(const TRect& aRect, CFepUiBaseCtrl* aCtrl);    
    
    /**
     * get the locked area
     *
     * @since S60 v5.0
     * @reeturn return current locked area.
     */        
    inline const TRect& LockedArea();
    
    /**
     * get the locked area owner
     *
     * @since S60 v5.0
     * @reeturn return current locked area owner.
     */            
    inline const CFepUiBaseCtrl* LockareaOwner() const;
    
     /** Update layout area, cause flush in screen
     *
     * @since S60 V4.0
     * @param aCtrl The control who requires the update
     * @param aRect the rect to be flushed in screen
     * @param aUpdateFlag ETrue if full update.
     * @param aImmedFlag flag tells whethere updating is executing immediately     
     */
    IMPORT_C void UpdateArea(const CFepUiBaseCtrl* aCtrl,const TRect& aRect,
                                             TBool aUpdateFlag,TBool aImmedFlag = EFalse);
        
    /**
     * Call back function. Called when cursor in input context field changed by pointer.
     * Just signal layout owner this event in default implementation
     *
     * @since S60 v4.0
     * @param aCursorSel Current cursor selection in input context field
     */                
    IMPORT_C virtual void SetAppEditorCursor(const TCursorSelection& aCursorSel,
                                                 TBool aReqMoreText = EFalse);   
    
    /**
     * Get layout status. Layout is only ready after initialized.
     * @since S60 v4.0
     *
     * @return The layout status
     */    
    inline TBool LayoutReady();
   
    //from base class MPenUiLayoutBase
    
    /**
     * From MPenUiLayoutBase    
     * Init. 
     * Do layout initialization. It's called by layout owner
     *
     * @since S60 v4.0
     * @return The layout rect including the shadow rect
     */
    IMPORT_C TRect Init();

    /**
     * From MPenUiLayoutBase        
     * HandleEventL. Handle UI event.
     * There are only 3 kinds of UI event right now: raw event, 
     * pointer and pointer buffer event. 
     *
     * @since S60 v4.0
     * @param aType The event type. See TEventType
     * @param aEventData The event data.
     * @return The control which processes the event
     */
    IMPORT_C TBool HandleEventL(TEventType aType, const TAny* aEventData);

    /**
     * From MPenUiLayoutBase        
     * HandleCommand. Handle layout command event.
     *
     * @since S60 v4.0
     * @param aCmd Command Id.
     * @param aData Data for command.
     * @return precessing result. Errors when return value small than 0.		
     */
    IMPORT_C TInt HandleCommand(TInt aCmd, TUint8* aData) ;
                                                    
    /**
     * From MPenUiLayoutBase        
     * Draw layout.
     *
     * @since S60 v4.0    
     */                                                          
    IMPORT_C virtual void Draw() ;
    
    /**
     * From MPenUiLayoutBase        
     * OnResourceChange. Inform layout the system resource changed.
     *
     * @since S60 v4.0
     * @param aType the event type
     * @return KErrNone if successfully size changing. 
     */
    IMPORT_C virtual TInt OnResourceChange(TInt aType);     

    /**
     * From MPenUiLayoutBase        
     * HandleAppInfoChange. Inform UI that application infomation changed.
     *
     * @since S60 v5.0
     * @param aInfo The information.
     * @param aType The information type
     */
    IMPORT_C virtual void HandleAppInfoChange(const TDesC& aInfo, TPeninputAppInfo aType);    

    /**
     * Handle editor text coming
     * Called by animation dll when app editor text is coming.
     * If a ui layout has a editor area, it must implements this function to get the text
     *
     * @since S60 v4.0
     * @param aData The fep input context field data
     * @return KErrNone is operation succeeded.
     */
    IMPORT_C virtual TInt OnAppEditorTextComing(const TFepInputContextFieldData& aData);
                          
    /*
     * From MPenUiLayoutBase        
     * Get the max editor length which the input context field can hold
     *
     * @since S60 v4.0
     * @return The editor length
     */
    IMPORT_C virtual TInt GetInputContextFieldMaxLen();    
    
    /**
     * Get layout start position
     *
     * @since S60 v4.0
     * @return The layout start position
     */
    IMPORT_C virtual TPoint Position();    
    /**
     * Get layout area
     *
     * @since S60 v4.0
     * @return The layout size in rect
     */
    IMPORT_C virtual TRect Rect();    
    
    /**
     * From MPenUiLayoutBase    
     * Destroy layout
     *
     * @since S60 v4.0
     * @return The layout size in rect
     */    
    IMPORT_C virtual void Destroy();

    /**
     * From MPenUiLayoutBase        
     * OnActivate. Called by owner when the layout is going to be shown.
     *
     * @since S60 v4.0
     */        
    IMPORT_C void OnActivate();
    
    /**
     * From MPenUiLayoutBase        
     * OnDeActivate. Called by owner when the layout is going to be hidden
     *
     * @since S60 v4.0
     */        
    IMPORT_C void OnDeActivate();
    /**
     * SemiTransparencyRequired. Tell whether this layout requires semi-transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v4.0
     * @return ETrue if layout require transparency, otherwise EFalse.
     */        
    IMPORT_C TBool SemiTransparencyRequired();        

    /**
     * Set layout position
     *
     * @since S60 v4.0
     * @param The layout position
     */        
    IMPORT_C void SetLayoutPos(const TPoint& aPt);

    IMPORT_C TInt GetEnabledPluginMode( );
    
    /**
     * RegisterFeedbackArea. Register the area which requires tactile feedback.
     * Note, if two control have the same area, you need to register twice.
     *
     * @since S60 v4.0
     * @param aId The unique id of this area
     * @param aRect The tactile area.
     */    
    IMPORT_C void RegisterFeedbackArea(TInt aId,const TRect& aRect);
    
    /**
     * DeRegisterFeedbackArea. Remove the area which has required tactile feedback.
     * Nothing happends if the area not registered before.
     *
     * @since S60 v4.0
     * @param aId The unique id of this area. Don't use negative id.
     * @param aRect The tactile area.
     */    
    IMPORT_C void DeRegisterFeedbackArea(TInt aId,const TRect& aRect);
    
    /**
     * ChangeFeedbackArea. Change the area which has required tactile feedback.
     *
     * @since S60 v4.0
     * @param aId The unique id of this area     
     * @param aNew The new registered area
     * @param aOld Previous area for tactile feedback.
     */      
    IMPORT_C void ChangeFeedbackArea(TInt aId,const TRect& aNew,const TRect& aOld);   

    /**
     * This function can be used to check, whether touch feedback is
     * supported at all in the device.
     *
     * @since S60 v4.0
     * @return ETrue if touch feedback is supported in this device.
     */      
    IMPORT_C TBool SupportTactileFeedback();
    
    /**
     * Gives instant feedback.
     *
     * Notice that the feedback might not be actually played, if 
     * for example user has disabled the feedback from the settings.
     *
     * @since S60 v4.0
     * @param aType - The logical feedback type to play.
     */    
    IMPORT_C void DoTactileFeedback(TInt aType);
    
    /**
     * Gives instant feedback.
     *
     * Notice that the feedback might not be actually played, if 
     * for example user has disabled the feedback from the settings.
     *
     * @since S60 v4.0
     * @param aType - The logical feedback type to play.
     * @param aVibraEnable - Enable Audio Feedback.
     * @param aAudioEnable - Enable Vibra Feedback.
     */    
    IMPORT_C void DoTactileFeedback(TInt aType, TBool aVibraEnable, TBool aAudioEnable);

    /**
     * Create the cursor. There is only one cursor within a pen ui.
     *
     *
     * @since S60 v4.0
     * @return The cursor instance
     */        
    IMPORT_C CFepUiCursor* CreateCursor();    
    
    /**
     * SetPositionFromOutside. Inform UI that the position is change from outside of UI.
     *
     * @since S60 v5.0
     * @param aInfo The information.
     * @param aType The information type
     */
    IMPORT_C void SetPositionFromOutside(const TPoint& aNewPos);  
    
    
    /**
     * RegisterFeedbackArea. Register the area which requires tactile feedback.
     * Note, if two control have the same area, you need to register twice.
     *
     * @since S60 v4.0
     * @param aId The unique id of this area
     * @param aRect The tactile area.
     */   
    //Advanced Tactile feedback REQ417-47932
    IMPORT_C void RegisterFeedbackArea(TInt aId,const TRect& aRect, TInt aTactileType);
    
    
    /**
	 * ChangeFeedbackType. Change the tactile feedback which has required tactile feedback.
	 *
	 * @since S60 v4.0
	 * @param aId The unique id of this feedback type     
	 * @param aRect The area
	 * @param aNewTactileType new feedback type.
	 * 
	 */      
	//Advanced Tactile feedback REQ417-47932
	IMPORT_C void ChangeFeedbackType(TInt aId, TInt aNewTactileType);  

protected:
    /**
     * Constructor
     *
     * @since S60 v4.0
     * @param aLayoutOwner The layout owner
     */
    IMPORT_C CFepUiLayout(MLayoutOwner* aLayoutOwner);

    /**
     * OnSkinChange. Called when system skin changed.
     *
     * @since S60 v4.0
     * @return KErrNone if successfully handling. 
     */
    IMPORT_C virtual TInt OnSkinChange();     
    
private:

    /**
     * Handle raw key event. Call back function. Called when raw key event happens
     * Do nothing in default implementation
     *
     * @since S60 v4.0
     * @param aKeyEvent Key event
     * @return EFalse if the key has not been processed. Otherwise ETrue. 
     *      If key is not processed, it will be passed to CONE's control stack.
     */                
    IMPORT_C virtual TBool HandleRawKeyEventL(const TRawEvent& aKeyEvent);

    /**
     * Handle pointer down/up event while this event is outside of the layout
     * Do nothing in default implementation
     *
     * @since S60 v4.0
     * @param aEvent The raw event
     * @return EFalse if the event needs forward on. Otherwise ETrue.
     */                
    IMPORT_C virtual TBool OnPointerEventOutsideLayout(const TRawEvent& aEvent);
    
    /**
     * IsSemiTransparencyRequired. Tell whether one of the contorl require transparency.
     *
     * @since S60 v4.0
     * @param aCtrlList The control list
     * @return ETrue if layout require transparency, otherwise EFalse.
     */            
    TBool IsSemiTransparencyRequired(const RPointerArray<CFepUiBaseCtrl>& aCtrlList);
    
    /**
     * SizeChanged. Called when screen size changed.
     *
     * @since S60 v4.0
     * @param aData the event data
     * @return KErrNone if successfully size changing. 
     */
    IMPORT_C virtual TInt SizeChanged(const TAny* aData);     
    
    /**
     * Send editor text and cursor pos
     *
     * @since S60 v4.0
     * @param aData Editor text and cursor pos information
     */
    void SendEditorTextAndCursorPosL(TUint8* aData);

private:  
    /**
     * Layout dragging position. Same as iLayoutPos, but only used when layout is
     * dragging.
     *
     */
    TPoint iLayoutDraggingPos;
        
    /**
     * screen size
     */
    TSize iScreenSize;
 
    /** 
     * Root control. It's a control group.
     * Own
     */
    CFepUiLayoutRootCtrl* iRootCtrl;
    
    /**
     * Layout owner.
     * Not own
     */        
    MLayoutOwner* iLayoutOwner;        
    /**
     * layout dragbar number.
     */
    TInt iDragbarNum;
    /**
     * dragbar list
     */    
    CDragBar* iDragbarList[KMaxDragBarNum];
    
    /**
     * locked area. There may be one locked area which only the locker can 
     * update the area.
     */
    TRect iLockedArea;

    /**
     * The owner of the locked area
     */    
    CFepUiBaseCtrl* iLockedAreaOwner;
    
    /**
     * State indicates layout ready to be drawn
     */
     TInt iLayoutReady;     
    
    /**
     * Reserved item1
     */
     TInt iReserved1;     
     
    /**
     * Flag for tactile feedback support
     */        
    TBool iTactileSupported;
};

#include "peninputlayout.inl"

#endif  C_CFEPUILAYOUT_H

// End of file
