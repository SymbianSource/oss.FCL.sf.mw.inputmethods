/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the vkb client area
*
*/


#ifndef C_CAKNFEPVKBCLIENTAREA_H
#define C_CAKNFEPVKBCLIENTAREA_H

//  INCLUDES
#include <peninputlayoutctrlgroup.h>
#include <peninputvkbctrlext.h>

#include "peninputvkb.hrh"
#include "peninputvkbwindow.h"
#include "peninputvkbcnlafdatamgr.h"
/** class forward decalaration */
class CFepUiLayout;
class CAknFepVkbCtrlPool;
class CAknFepLayoutPool;
class CAknFepVkbImLayout;
class CPeninputVkbLayoutInfo;
class MAknFepVkbDataProvider;
class CPeninputVkbCtrlExt;
class CAknFepVkbCompositionField;
class CFepCtrlDropdownList;
class CAknFepCtrlRangeBar;
class CAknFepCtrlEventButton;
class CFepLayoutEditAreaBase;
class CAknFepVkbNumericKepMappingMgr;
class CAknFepCtrlVkbLayout;
class CPeninputCnGenericVkbLafMgr;

// class declarations
/**
 *  client area class
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */    
class CAknFepVkbClientArea : public CControlGroup
    {    
public:

    /**
     * Create one CAknFepVkbClientArea object
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return The CFepCtrlDropdownList object.
     */  
    static CAknFepVkbClientArea* NewL(CFepUiLayout* aUiLayout, 
                                      TInt aControlId,
                                      MAknFepVkbDataProvider* aDataProvider,
                                      CPeninputCnGenericVkbLafMgr* aLafMgr);

    /**
     * Create one CAknFepVkbClientArea object
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return The CFepCtrlDropdownList object.
     */      
    static CAknFepVkbClientArea* NewLC(CFepUiLayout* aUiLayout, 
                                       TInt aControlId,
                                       MAknFepVkbDataProvider* aDataProvider,
                                       CPeninputCnGenericVkbLafMgr* aLafMgr);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None.
     */   
    virtual ~CAknFepVkbClientArea();  
    
    /**
     * Size change function
     *
     * @since S60 v3.2
     * @return None.
     */       
    void SizeChanged();

    /**
     * Handle event 
     * @since Series 60 3.2
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);
    
    /**
     * Change input method layout
     *
     * @since S60 v3.2
     * @param aRange The range defined in TAknFepVkbIMLayout
     * @param aChangeVkb The flag of changing vkb or not     
     * @return None.
     */
    void ChangeImLayout(TInt aImLayout, TBool aChangeVkb = ETrue);
    
    /**
     * Change vkb layout
     *
     * @since S60 v3.2
     * @param aVkbLayout The new vkb layout
     * @return None.
     */ 
    void ChangeVkbLayout(TInt aVkbLayoutId = -1);

    /**
     * Loop vkb layout
     *
     * @since S60 v3.2
     * @param aType The flag showing shift down or capslock down
     * @return None.
     */ 
    void LoopVkbLayout(TVkbShiftType aType, TBool aNotify = EFalse);

    /**
     * Get the string length in composition field
     *
     * @since S60 v3.2
     * @return The string length in composition field.
     */     
    TInt CompositionFieldStringLength();

    /**
     * Set rect of parent, top parent, layout
     *
     * @since S60 v3.2
     * @param aRect The rect of client rect
     * @return None.
     */     
    void SetWholeWindowRect(const TRect& aRect);

    /**
     * Set top parent
     *
     * @since S60 v3.2
     * @param aParent The vkb window
     * @return None.
     */      
    void SetTopParent(CFepUiBaseCtrl* aParent);

    /**
     * Reorganize controls
     *
     * @since S60 v3.2
     * @param aImLayout The input method layout
     * @param aImLayoutChanged To indicate whether Im Layout changed
     * @return None.
     */        
    void ReorganizeControls(CAknFepVkbImLayout* aImLayout, TBool aImLayoutChanged);
    
    /**
     * Get composition field
     *
     * @since S60 v3.2
     * @return The composition field.
     */       
    CFepLayoutEditAreaBase* CompositionField();

    /**
     * Add controls into client area
     *
     * @since S60 v3.2
     * @param aControl The control which will be added into client area
     * @return The composition field.
     */       
    void AddControlL(CFepUiBaseCtrl* aControl); 
    
    /**
     * Is there need to create new word
     *
     * @since S60 v3.2
     * @return ETrue if need to create new word, otherwise EFalse
     */       
    TBool NeedCreateWord();
 
    /**
     * Get layout ID
     *
     * @since S60 v3.2
     * @param aRange The current range
     * @return The composition field.
     */   
    TAknFepVkbLayout ShiftCapsSingleVkbLayoutId( TInt aRange );
    
    /**
     * Is next candidate after last page exist?
     *
     * @since S60 v3.2
     * @return ETrue if candidate eixst, otherwise EFalse
     */       
    void SetNextCandidateExistFlag(TBool aFlag);
    
    /**
     * Set if enable switch to hwr
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    void SetEnableSwitchToHwr(TBool aEnable);
    
    /**
     * Active current range
     *
     * @since S60 v3.2
     * @return None
     */ 
    void ActiveCurrentRange();
    
    /**
     * Return the spell for new created words.
     *
     * @since S60 v3.2
     * 
     * @return The buffer for the gernated spell.
     */ 
    TPtrC GetCreatedWordSpell();

    void SetVkbLayoutSize(TInt aVkbLayoutId = -1);
    
    /**
     * Change Shift and CapsLock merged button status
     *
     * @since Series 60 3.2
     * @param aIsShiftCase The Shift case
     * @param aIsCapslockCase The CpasLock case
     * @return None
     */  
    void ChangeMergedButtonStatus( const TInt aIsShiftCase, 
    							   const TInt aIsCapslockCase ); 
    							   
    void ShowBubble(TBool aShow);
    
    void ReorganizeZhuYinControls( TBool aShowArrowKey );
    
    /**
     * Dim arrow keys when the editor type is
     * find pane or secret editor
     *
     * @since S60 v3.2
     * @param aIsSecret The flag
     * @return None
     */          
    void DimArrowKeys( TBool aDimArrow );
       
protected:

    /**
     * Constructor function
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return None.
     */ 
    CAknFepVkbClientArea(CFepUiLayout* aUiLayout, 
                         TInt aControlId,
                         MAknFepVkbDataProvider* aDataProvider,
                         CPeninputCnGenericVkbLafMgr* aLafMgr);
                         
    /**
     * 2nd construct function
     *
     * @since Series 60 3.2
     * @return None
     */
    void ConstructL();
    
    /**
     * Get default vkb layout according to range
     *
     * @since Series 60 3.2
     * @param aRange the current range
     * @return The default vkb layout.
     */    
    TAknFepVkbLayout GetDefaultVkbLayout(const TInt aRange);      

    /**
     * Add drop down list into client area
     *
     * @since Series 60 3.2
     * @return None
     */
    void AddDropdownListL();

    /**
     * Add button into client area
     *
     * @since Series 60 3.2
     * @param aControlId The button ID
     * @param aEventId The event id
     * @param aResId The resource id
     * @param aUnicode The unicode
     * @param aIsRepeat The flag showing whether it is repeat button
     * @
     */
    CAknFepCtrlEventButton* AddButtonL(const TInt aControlId,
                                       const TInt aEventId,
                                       const TInt aResId,
                                       const TInt aUnicode = 0,
                                       const TBool aIsRepeat = EFalse);

    /**
     * Add range bar into client area
     *
     * @since Series 60 3.2
     * @return None
     */    
    void AddRangeBarL();



    /**
     * Reset some data in client area
     *
     * @since Series 60 3.2
     * @param aImLayoutChanged To indicate whether Im Layout changed
     * @return None
     */      
    void Reset(TBool aImLayoutChanged);

    /**
     * Dim keys in vkb controls
     *
     * @since Series 60 3.2
     * @return None
     */  
    void DimKeys();
    
    /**
     * Clear all candidates
     *
     * @since Series 60 3.2
     * @return None
     */      
    void ClearCandidatesInList();

    /**
     * Calculate postion function
     *
     * @since S60 v3.2
     * @return None.
     */    
    void CalculatePosition( TInt aJustifyMode );

    /**
     * Change window position
     *
     * @since S60 v3.2
     * @return None.
     */    
    void TopLeftJustify();

    /**
     * Change window position
     *
     * @since S60 v3.2
     * @return None.
     */    
    void BottomRightJustify();

    /**
     * Change window position
     *
     * @since S60 v3.2
     * @return None.
     */    
    void DataQueryJustify();   
        
    /**
     * read from resource function
     *
     * @since Series 60 3.2
     * @return None
     */     
    void ConstructFromResourceL();
    
    /**
     * Set control's text property, e.g. font,
     * font color
     *
     * @since Series 60 3.2
     * @return None
     */     
    void SetTextProperty();
    
private:

    TInt FepCaseByCaseId( TInt aCaseId );

    TInt CaseByShiftCapslockStatus();
      
    TPeninputCnCellType GetCellType(TInt aColumns);
    
    void DimKeysL();
    void HandleStandby2CompositionEventL();
           
private:// data

    /**
     * The top parent
     * Not own
     */  
    CFepUiBaseCtrl* iTopParent;


    /**
     * The control pooling
     * Own
     */ 
    CAknFepVkbCtrlPool* iCtrlPool;
    
    /**
     * The layout pooling
     * Own
     */ 
    CAknFepLayoutPool* iLayoutPool;

    /**
     * The current im layout
     * Not own
     */ 
    CAknFepVkbImLayout* iCurrentImLayout;
    
    /**
     * The current vkb layout
     * Not own
     */ 
    CPeninputVkbLayoutInfo* iCurrentVkbLayout;
    
    /**
     * The data provider
     * Not own
     */
    MAknFepVkbDataProvider* iDataProvider;
    
    /**
     * The vkb ctrl
     * Not own
     */
    CPeninputVkbCtrlExt* iVkbCtrl;
            
    /**
     * Vkb layout
     * Own
     */ 
    CAknFepCtrlVkbLayout* iVkbLayout;
    
    /**
     * The composition field
     * Not own
     */
    CAknFepVkbCompositionField* iCompositionField;
    
    /**
     * The drop down list
     * Not own
     */
    CFepCtrlDropdownList* iDropdownList;
    
    /**
     * The range bar
     * Not own
     */
    CAknFepCtrlRangeBar* iRangeBar;
    


    
    /**
     * The key mapping manager
     * Own
     */    
    CAknFepVkbNumericKepMappingMgr* iKeyMappingMgr;
    
    /**
     * Indicate whether control's text property have 
     * been set, e.g. font, font color
     */    
    TBool iTextPropertySet;
    
    /**
     * Indicate whether drop down list's text property
     * and unit size etc. have been set
     */    
    TBool iDropdownPropertySet;
    
    CPeninputCnGenericVkbLafMgr* iLafMgr;
    
    CAknFepVkbWindow* iVkbCnWindow;

    TInt iCharWidth;
    TInt iCharHeight;

    TPoint iCharLeftTop;   
    
    TInt iBtnWidth;
    TInt iBtnHeight;
        
    TPoint iVkbBoardLeftTop;
    /**
     * Keep the current layout id
     */     
    TInt iVkbLayoutId;
    /**
     * Keep the control ID in ctrl pool not added to the control group
     */    
    RArray<TInt> iRemainedPoolControlIDs;
public:

    TInt iCellAidGap;
    };  

#endif // C_CAKNFEPVKBCLIENTAREA_H

// End Of File
