/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef C_CCONTROLBROUP_H
#define C_CCONTROLBROUP_H

#include <peninputlayoutbasecontrol.h>
class CButtonBase;
class CCoeControl;
/**
 *  CControlGroup
 *
 *  CControl group. A CControl group will consists of several controls.
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 V4.0
 */
class CControlGroup : public CFepUiBaseCtrl
    {    
public: 
    //constructor and destructor
    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CControlGroup class        
     */                           
    IMPORT_C static CControlGroup* NewL(CFepUiLayout* aUiLayout,
    									TInt aControlId, 
    									TBool aRedrawBg = ETrue);
    
    CControlGroup* NewL(CFepUiLayout* aUiLayout,CCoeControl* aControl);
    /**
     * Destructor
     *
     * @since S60 V4.0        
     */                                   
    IMPORT_C virtual ~ CControlGroup();

    /**
     * Add a control to group
     *
     * @since S60 V4.0
     * @param aControl New control to be added.Ownership transferred
     * @param aOwnerFlag The flag tells whether transfer ownership to group. ETrue if group will be the owner
     *                   A owner is responsible for free the control. 
     *                     Deafult value is ETrue
     */                  
    IMPORT_C void AddControlL(CFepUiBaseCtrl* aControl, TBool aOwnerFlag = ETrue);   
    /**
     * Add a control to group
     *
     * @since S60 V4.0
     * @param aControl New control to be added.Ownership transferred. A CFepUiBasedCtrl 
     *                  will be created for this window based control
     * @param aOwnerFlag The flag tells whether transfer ownership to group. ETrue if group will be the owner
     *                   A owner is responsible for free the control. 
     *                     Deafult value is ETrue
     * @return The control associated to this window based control
     */                  
    IMPORT_C CFepUiBaseCtrl* AddControlL(CCoeControl* aControl);   
    
    /**
     * RemoveControl
     * Remove a control from the control group
     *
     * @since S60 v4.0
     * @param aControl The control to be removed.  
     * @return ETrue if remove successfully.   
     */          
    IMPORT_C TBool RemoveControl(CFepUiBaseCtrl* aControl);
    
    
    /**
     * Get control for an position index
     *
     * @since S60 V4.0
     * @param aIndex Control index in group. Index starts from 0
     * @return The control for position
     */                      
    IMPORT_C CFepUiBaseCtrl* At(TInt aIndex);
    
    /**
     * Get control counts
     *
     * @since S60 V4.0
     * @param None
     * @return Number of controls in group
     */                      
    IMPORT_C TInt NumOfControls();        
    
     /**
     * Find qualified control in layout whose control type is exactly the same as 
     * the given one.
     *
     * @since S60 v4.0
     * @param aType The contol type
     * @param aIndex The control index. The nth control for such type in this group.
     *               The index is counted seperated for different control type.
     * @return        The contrl found.
     */
    IMPORT_C CFepUiBaseCtrl* ControlByType(TControlType aType,TInt aIndex = 0);   
    
     /**
     * Find qualified control in layout according to the type and index.
     * Unlike ControlByType, it finds all controls which is kind of such type, 
     * includes those derived controls.
     * 
     * @since S60 v4.0
     * @param aType The contol type
     * @param aIndex The control index. The nth control for such type in this group.
     *               The index is counted seperated for different control type.
     * @return The contrl found.
     */
    IMPORT_C CFepUiBaseCtrl* ControlForKindOfType(TStockBaseCtrlType  aType,
                                                            TInt aIndex = 0);
    /**
     * Set control to be actived.
     *
     * @since S60 V4.0
     * @param aControl The control to be set to actived. Ownership not transferred.
     * @return Previous active control
     */                      
    IMPORT_C virtual CFepUiBaseCtrl* SetActiveCtrl(CFepUiBaseCtrl* aControl);
   
    /**
     * Set control to be temparory actived. When button is pressed, the control is 
     * temparary set to be actived. When button is up, it's set to be actived.
     *
     * @since S60 V4.0
     * @param aControl The control to be set to actived. Ownership not transferred.
     * @return Previous active control
     */              
    CFepUiBaseCtrl* SetTempActiveCtrl(CFepUiBaseCtrl* aCtrl);
    /**
     * Set control to be actived
     *
     * @since S60 V4.0
     * @param aIndex The control index. Ownership not transferred.
     * @return current active control
     */                      
    IMPORT_C CFepUiBaseCtrl* SetActiveCtrl(TInt aIndex);
    
    /**
     * Get active control
     *
     * @since S60 V4.0
     * @param None
     * @return The active control
     */                      
    IMPORT_C CFepUiBaseCtrl* ActiveControl();
    
    /**
     * Hide Control
     *
     * @since S60 V4.0
     * @param aIndex control index
     * @param aFlag control hide flag. ETrue if going to be hiden. Otherwise EFalse
     *                Deafult value is ETrue
     */                      
    IMPORT_C void HideControl(TInt aIndex,TBool aFlag = ETrue);
    
    /**
     * Hide Control
     *
     * @since S60 V4.0
     * @param aControl The button to be hiden
     * @param aFlag Contrl hide flag. ETrue if going to be hiden. Otherwise EFalse
     *                Default value is ETrue
     */                    
    IMPORT_C void HideControl(CFepUiBaseCtrl* aControl,TBool aFlag = ETrue);
    
    /**
     * Get the sub control for given control Id. 
     *
     * @param aCtrlId The control id
     * @return The contrl found.
     */
    IMPORT_C CFepUiBaseCtrl* ControlById(TInt aCtrlId);
       
    /**
     * Get control list
     *
     * @since S60 v4.0
     * @return The layout control list
     */    
    inline const RPointerArray<CFepUiBaseCtrl>& ControlList() const;
    
    inline const RPointerArray<CFepUiBaseCtrl>& PopCtrlList() const;    

    /**
     * Check whether this control is in the control group
     *
     * @since S60 v4.0
     * @param aCtrl The control to be found
     * @return EFalse if not included in the group. ETrue if founded
     */    
    CFepUiBaseCtrl* Find(CCoeControl* aCtrl);
    
    /**
    //From base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */              
    IMPORT_C virtual void Draw();
    
    /**
     * From CFepUiBaseCtrl    
     * Redraws the content in a control
     *
     * @since S60 V4.0
     * @param aRect The rect to be redrawed
     */               
     IMPORT_C virtual void ReDrawRect(const TRect& aRect);     
    
    /**
     * Redraws the content in a control if needed. The redraw is caused by aCtrl's updating aRect.
     *
     * @since S60 V5.0
     * @param aRect The rect to be redrawed
     * @param aCtrl The control who initiates the updating.
     */                    
     void RedrawRectIfNeeded(const TRect& aRect, CFepUiBaseCtrl* aCtrl);

    /**
     * From CFepUiBaseCtrl
     * Hide or shoe control. A hiden control will not handle the event.
     *
     * @since S60 V4.0
     * @param aFlag ETrue if want to hide the control.
     */                                               
    IMPORT_C virtual void Hide(TBool aFlag);

    /**
     * From CFepUiBaseCtrl
     * Move control
     *
     * @since S60 V4.0
     * @param aOffset The offset to be moved
     */                
    IMPORT_C void Move(const TPoint& aOffset);    
    
    /**
     * From CFepUiBaseCtrl
     * Cancel pointer down event.
     * This will be called when a control has pointer down but pointer up 
     * event happens in other ways.
     *
     * @since S60 V4.0
     * @param aFocusedFlag ETrue if got focused, otherwise EFalse
     */          
    IMPORT_C virtual void CancelPointerDownL();    
    
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
    /**
     * From CFepUiBaseCtrl
     * Call back function when layout starts dragging
     *
     * @since S60 V4.0
     */
    virtual IMPORT_C void OnLayoutDraggingStart();

    /**
     * From CFepUiBaseCtrl    
     * Call back function when layout ends dragging
     *
     * @since S60 V4.0
     */
    virtual IMPORT_C void OnLayoutDraggingEnd();
    
    /**
     * From CFepUiBaseCtrl
     * Construct from resource. Handle resource change
     *
     * @since S60 V4.0
     */                
    IMPORT_C void ConstructFromResourceL();
    
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
     * Bring the control to top most in current control group and show it.
     *
     * @since S60 V5.0
     * @aParam aCtrl The control to be shown.
     */                                               
    IMPORT_C virtual void BringToTopInGroup(CFepUiBaseCtrl* aCtrl);
    
    IMPORT_C virtual void BringToBackInGroup(CFepUiBaseCtrl* aCtrl);
    void UpdateAreaInGrp(CFepUiBaseCtrl* aControl,const TRect& aRect,TBool aUpdateFlag); // = EFalse
    
    void UpdateAreaInGrpImmed(CFepUiBaseCtrl* aControl,const TRect& aRect,TBool aUpdateFlag);
    void RedrawRectInGrp( CFepUiBaseCtrl* aControl,const TRect& aRect );

    TInt GetNestedLevel();

    TInt GetIdx(const CFepUiBaseCtrl* aCtrl);
    
    void AdjustBkCtrlPos();
    
protected:
    /**
    * Symbian second phrase constructor
    *
    * @since S60 V4.0
    */            
    IMPORT_C void ConstructL();

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

    const RRegion& CalculateNonHwrStartPtArea();
    void CalculateClipRegion();    
    
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
protected:
    /**
     * Default constructor.        
     *
     * @since S60 V4.0        
     * @param aUiLayout Ui layout who contains this control
     * @param aControlId control Id
     */                                   
    IMPORT_C CControlGroup(CFepUiLayout* aUiLayout,TInt aControlId);
    
    /**
     * Default constructor.        
     *
     * @since S60 V4.0        
     * @param aUiLayout Ui layout who contains this control
     * @param aControlId control Id
     */                                   
    IMPORT_C CControlGroup(CFepUiLayout* aUiLayout,
    					   TInt aControlId,
    					   TBool aRedrawBg);

    /*
    * ControlForPoints
    * Get the control which contains the given point
    *
    * @param aPt the point to be checked
    * @return The control which contains the point
    */       
    CFepUiBaseCtrl* ControlForPoints(const TPoint& aPt);
	
    /**
     * Draw ControlGroup Background        
     *
     * @since S60 V5.0        
     * @param aUpdateRegion The rect to be updated    
     */                                   	
	virtual IMPORT_C void DrawControlGroupBackground( const TRect& aUpdateRegion );	
private:
    /**
     * RemoveOwnedControl
     * Remove a control from group's owned control list
     *
     * @since S60 v4.0
     * @param aControl The control to be removed.  
     * @return ETrue if remove successfully.   
     */          
    TBool RemoveOwnedControl(CFepUiBaseCtrl* aControl);

    TInt FindSubRegion(CFepUiBaseCtrl* aCtrl);
    
    void RemoveCtrlFromSubCtrlInfoList(CFepUiBaseCtrl* aControl);
    void SaveSubCtrlShownState();
    void RemoveSubRegion(TInt aIdx);
    void AddSubRegionL(CFepUiBaseCtrl* aCtrl);
    void CalculateClipRegion(CFepUiBaseCtrl* aCtrl);
    TBool MoveSubCtrlToTop(CFepUiBaseCtrl* aCtrl, RPointerArray<CFepUiBaseCtrl>& aList);
    TBool MoveSubCtrlToBack(CFepUiBaseCtrl* aCtrl,RPointerArray<CFepUiBaseCtrl>& aList);
    void OnDeActivateL();
protected:
    /**
     * Control list for all controls
     */
    RPointerArray<CFepUiBaseCtrl> iCtrlList;

    /**
     * Active control
     * Not own
     */
    CFepUiBaseCtrl* iActiveCtrl;    
    
    /**
     * Control which gets the pointer down event
     * Not own
     */
    CFepUiBaseCtrl* iCtrlWithPointerDown;    

    RRegion iNonHwrStartPtRegion;
    /**
     * Control list for all owned controls. 
     * Control group will be responsible for release owned controls
     */
    RPointerArray<CFepUiBaseCtrl> iOwnedCtrlList;  
    
    /**
     * This is the control list for those control which must be shown on top, and 
     * which doesn't directly belong ( as a direct child of)
     * to current group
     */        
    RPointerArray<CFepUiBaseCtrl> iPopCtrlList;    

private:      
    //internal class to record sub control's information
    class CSubRegionArea : public CBase
        {
        public:        
            ~CSubRegionArea();        
        private:
            CSubRegionArea(const CFepUiBaseCtrl* aCtrl,const RRegion &aRegion);
        private:
            const CFepUiBaseCtrl* iSubCtrl;
            RRegion iSubRegion;
        friend class CControlGroup;
        };
    struct TSubCtrlShownState
        {
        CFepUiBaseCtrl* iSubCtrl;
        TBool           iHiden;
        };
    /**
     * temparory active control
     * Not own
     */
    CFepUiBaseCtrl* iTempActiveCtrl;
    
    RPointerArray<CSubRegionArea> iSubRegionArea;   
    
    /**
     * list to record sub control's hide/show state before control group is hidden
     */    
    RArray<TSubCtrlShownState> iSubCtrlShowStateBeforeHiden;
    
    /**
     * Reserved item1
     */
     TInt iNeedRedrawBg;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;    
    };
//end of classd CControlGroup

#include "peninputlayoutctrlgroup.inl"

#endif //C_CCONTROLBROUP_H