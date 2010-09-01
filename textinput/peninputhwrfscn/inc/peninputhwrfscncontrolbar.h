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
* Description:  Implementation for chinese peninput full screen hwr
*
*/


#ifndef __PENINPUTHWRFSCNCONTROLBAR_H__
#define __PENINPUTHWRFSCNCONTROLBAR_H__

#include <aknfeppeninputenums.h>


#include "peninputhwrfscnbasepanel.h"
#include "peninputbutton.h"
#include "peninputrepeatbutton.h"
#include "peninputmultiimagebutton.h"
#include "peninputlayoutlatchbutton.h"
#include "peninputhwrfscnfunctionpanel.h"
#include "peninputlayoutctrlgroup.h"

class CPeninputLayoutInputmodelChoice;
class CAknFepCtrlCommonButton;
/**
* Class CPeninputHwrfscnControlBar
*
* Define the control bar panel and contain controlbar buttongroup and function panel
*
* @lib peninputhwrfscn.lib
* @since S60 v5.0
*/
class CPeninputHwrfscnControlBar : public CPeninputHwrfscnBasePanel
	{
public:
    /**
    * The factory function
    *
    * @since Series 60 5.0
    * @param aLayout The Full screen layout
    * @return Pointer to created CPeninputHwrfscnControlBar object
    */
	static CPeninputHwrfscnControlBar* NewL( CPeninputHwrfscnLayout* aLayout );

    /**
    * The destructor function
    *
    * @since Series 60 5.0
    */
    ~CPeninputHwrfscnControlBar();

public:// From base class

    /**
    * From CPeninputHwrfscnBasePanel
    * Show controls
    *
    * @since Series 60 5.0
    * @param aVisible Show or hide
    */
	void Show( TBool aVisible );

    /**
    * From CPeninputHwrfscnBasePanel
    * Draw
    *
    * @since Series 60 5.0
    */
	virtual void Draw();

    /**
    * Handle control's event
    *
    * @since Series 60 5.0
    * @param aEventType Event type
    * @param aCtrl Event source
    * @param aEventData event data
    */
	virtual void HandleControlEvent( TInt aEventType, 
	    CFepUiBaseCtrl* aCtrl, 
	    const TDesC& aEventData );

    /**
    * Handle layout command
    *
    * @since Series 60 5.0
    * @param aCommand command type
    * @param aData Event data
    */
	virtual void HandleCommand( TInt aCommand, TUint8* aData );

    /**
    * From CPeninputHwrfscnBasePanel
    * Check if the property could be changed before changing it.
    *
    * @since S60 v5.0
    * @param aProperty One of @see TPeninputHwrfscnProperty changed
    * @return TBool ETrue if the property needed to be changed, EFalse otherwise
    */
    virtual TBool BeforePropertyChangedL( 
        MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty aProptery);

	/**
    * From CPeninputHwrfscnBasePanel
	* Notify the property subscriber the property changed.
	*
	* @since S60 v5.0
	* @param aProperty One of @see TPeninputHwrfscnProperty changed
	*/
    virtual void PropertyChangedL(
        MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty aProptery);


    /**
    * From CPeninputHwrfscnBasePanel
    * Handle size changed.
    *
    * @since S60 v5.0
    */
    void SizeChanged();
    
    /**
    * 
    * Handle skin changed.
    *
    * @since S60 v5.0
    */    
    void HandleSkinChanged();
    
    /**
    * 
    * Set enable setting button.
    *
    * @since S60 v5.0
    * @param aEnable
    */    
    void SetEnableSettingBtn(TBool aEnable);

private:
    /**
    * The latch picture index
    */
    enum TLatchPicIndex
        {
        ELatchPicError = -1,
        ELatchPicMoreFunc = 0,
        ELatchPicChineseButton,
        ELatchPicEnglishButton,
        ELatchPicNumberButton,
        ELatchPicCount        
        };
    
    /**
    * Save the latched and unlatche pictures pointers
    */
    struct TLatchedPic
        {
        CFbsBitmap* iSvgLatchedBMP;
        CFbsBitmap* iSvgUnLatchedBMP;
        };
                
private:
   /**
    * The constructor
    *
    * @since Series 60 5.0
    * @param aLayout The layout pointer.
    */
    CPeninputHwrfscnControlBar( CPeninputHwrfscnLayout* aLayout );

    /**
    * 2nd phase constructor
    *
    * @since Series 60 5.0
    */
	void ConstructL();

    /**
    * From CPeninputHwrfscnBasePanel
    * Handle laf (Look and feel) changed
    *
    * @since S60 v5.0
    */
    void LafChanged();

    /**
    * Handle control's button up event
    *
    * @since Series 60 5.0
    * @param aEventType Event type
    * @param aCtrl Event source
    * @param aEventData event data
    */
    void OnControlEvent_ButtonUp(
        TInt aEventType, 
        CFepUiBaseCtrl* aCtrl, 
	    const TDesC& aEventData );

    /**
    * Handle control's button down event
    *
    * @since Series 60 5.0
    * @param aEventType Event type
    * @param aCtrl Event source
    * @param aEventData event data
    */
    void OnControlEvent_ButtonDown(
        TInt aEventType, 
        CFepUiBaseCtrl* aCtrl, 
        const TDesC& aEventData );

    /**
    * Create close button group
    *
    * @since Series 60 5.0
    */
    void CreateCloseBtnL( );

    /**
    * Create option button group
    *
    * @since Series 60 5.0
    */
    void CreateOptionBtnL( );
    
    /**
    * Create vkb switcher button group
    *
    * @since Series 60 5.0
    */
    void CreateVkbSwitcherBtnL( );
    
    /**
    * Create Range switch button group
    *
    * @since Series 60 5.0
    */
    void CreateRangeSwitchBtnGroupL();

    /**
    * Create Range switch cover button group
    *
    * @since Series 60 5.0
    */
    void CreateRangeSwitchCoverBtnGroupL();

    /**
    * Create more function button
    *
    * @since Series 60 5.0
    */
    void CreateMoreFuncBtnL();

    /**
    * Create more function panel
    *
    * @since Series 60 5.0
    */
    void CreateMoreFuncPanelL();

    /**
    * Create backspace function button
    *
    * @since Series 60 5.0
    */
    void CreateBackSpaceBtnL();

    /**
    * Create latched button
    *
    * @since Series 60 5.0
    * @param aRect button rectangle
    * @param aCtrlId control ID
    * @param aLatchedPicID latched picture ID
    * @param aUnLatchedID unlatched picture ID
    * @return newly created button
    */
    CAknFepCtrlCommonButton* CreateLatchedBtnLC( const TRect aRect,
        TInt aCtrlId,
        TInt aLatchedPicID,
        TInt aUnLatchedID );

    /**
    * Create latched button
    *
    * @since Series 60 5.0
    * @param aRect button rectangle
    * @param aCtrlId control ID
    * @param aResID resource ID
    * @return newly created button
    */
    CAknFepCtrlCommonButton* CreateNormalBtnLC( const TRect aRect, 
        TInt aCtrlId, 
        TInt aResID );

    /**
    * Close Range switch list
    *
    * @since Series 60 5.0
    */
    void CloseRangeSwitchList();
   
   /**
    * Hide cover range swith buttons
    *
    * @since Series 60 5.0
    */
    void HideCoverRangeSwitchBtns();
    
    /**
    * Hide range swith buttons
    *
    * @since Series 60 5.0
    */
    void HideRangeSwitchBtns( TBool aHide );
    
    /**
    * Set active cover range button
    *
    * @since Series 60 5.0
    */
    void SetActiveCoverRangeBtn();
    
    /**
    * Remove all buttons on range and cover range list
    *
    * @since Series 60 5.0
    */
    void RemoveAllSwitchBtns();
    
    /**
    * Set the range switch buttons to be latched
    *
    * @since Series 60 5.0
    */
    void ShowRangeSwitchBtnsLatched();
    
    /**
    * Open Range switch list
    *
    * @since Series 60 5.0
    */
    void OpenRangeSwitchList();

    /**
    * Switch more function list
    * 
    * @since Series 60 5.0
    * @param aShowNow ETrue if the function panel need be shown or EFalse otherwise
    */
    void ShowMoreFuncList( TBool aShowNow );

    /**
    * Click is in the Range switch group
    *
    * @since Series 60 5.0
    * @param aCtrl event source control
    */
    TBool ClickIsInRangeSwitchGrp( CFepUiBaseCtrl* aCtrl );

    /**
    * Update Range swtich list
    *
    * @since Series 60 5.0
    */
    void UpdateRangeSwitchL();

    /**
    * Update convered Range swtich list
    *
    * @since Series 60 5.0
    */
    void UpdateCoverRangeSwitchL();
    
    /**
    * Update convered Range swtich without mark picture
    *
    * @since Series 60 5.0
    */
    void UpdateWithoutMarkL();
    
    /**
    * Update convered Range swtich with mark picture
    *
    * @since Series 60 5.0
    */
    void UpdateWithMarkL();
    
    /**
    * Get current active Range
    *
    * @since Series 60 5.0
    */
    TInt ActiveRangeIndex();

    /**
    * Update current active Range list
    *
    * @since Series 60 5.0
    */
    void UpdateCurrentActiveRangeList();

    /**
    * Update current active Range
    *
    * @since Series 60 5.0
    */
    void UpdateCurrentActiveRange(); 

    /**
    * Get event type by the control ID
    *
    * @since Series 60 5.0
    * @param aCtrlId control ID
    * @return Event type
    */
    TInt EventTypeByCtrlId(TInt aCtrlId);

	/**
	* Get Latched pic pointer based on the control ID
	*
	* @since Series 60 5.0
	* @param aCtrlId control ID
	* @param aIndex The latched picture index
	*/
    void GetLatchedIndexByCtrlID( TInt aCtrlID, TLatchPicIndex& aIndex );

	/**
	* Read LAF information
	*
	* @since Series 60 5.0
	*/
    void ReadLAFInfo();

	/**
	* Reset Cover Range switch button size
	*
	* @since Series 60 5.0
	*/
	void ResetCoverRangeSwitchSize();

	/**
	* Reset Range switch button size
	*
	* @since Series 60 5.0
	*/
	void ResetRangeSwitchSize();

	/**
	* Reset More function button size
	*
	* @since Series 60 5.0
	*/
	void ResetMoreFuncBtnSize();

	/**
	* Reset back space button size
	*
	* @since Series 60 5.0
	*/
	void ResetBackSpaceBtnSize();
	
	/**
	* Reset close, option, vkbswitcher button size
	*
	* @since Series 60 5.0
	*/
	void ResetControlBarBtnSize( );

	/**
	* Get more cover Range switch button rectangle
	*
	* @since Series 60 5.0
	* @return cover Range switch button rectangle
	*/
	TRect CoverRangeSwitchRect();

	/**
	* Get more Range switch buttons rectangle
	*
	* @since Series 60 5.0
	* @return Range switch buttons rectangle
	*/
	TRect RangeSwitchBtnRect(TInt aIndex);

	/**
	* Get more function button rectangle
	*
	* @since Series 60 5.0
	* @return more function button rectangle
	*/
	TRect MoreFuncBtnRect();

	/**
	* Get back space button rectangle
	*
	* @since Series 60 5.0
	* @return back space button rectangle
	*/
	TRect BackSpaceBtnRect();
	
	/**
	* Get back space button rectangle
	*
	* @since Series 60 5.0
	* @return back space button rectangle
	*/
	TRect ControlBarBtnRect( TInt aIndex );

private:
    enum TPeninputHwrfscnControlBarGrp
        {
        //This definition is used for more function btn
        ECloseBtn = 0,
        EOptionBtn,
        EVkbSwitcherBtn,
        EControlBarBtnCount
        };


private:
	/**
	* The function panel pointer
	* Own
	*/
    CPeninputHwrfscnFunctionPanel* iFunctionPanel;

	/**
	* Back space button
	* Not own
	*/
    CAknFepCtrlRepeatButton* iBackSpaceBtn;

	/**
	* More function button
	* Not own
	*/
    CAknFepCtrlCommonButton* iMoreFuncBtn;

	/**
	* Range switch button group
	* Not own
	*/
    CControlGroup* iRangeSwitchBtnGroup;    

	/**
	* Cover Range switch button group
	* Not own
	*/
    CControlGroup* iCoverRangeSwitchBtnGroup;
    
	/**
	* Close button
	* Not own
	*/
    CAknFepCtrlEventButton* iCloseBtn;
    
	/**
	* Option button
	* Not own
	*/
    CAknFepCtrlEventButton* iOptionBtn;
    
	/**
	* Vkb switcher button
	* Not own
	*/
    CAknFepCtrlEventButton* iVkbSwitcherBtn;   

	/**
	* Pen input mode switcher (own)
	*/
    CPeninputLayoutInputmodelChoice* iInputModeSwitch;

	/**
	* Range switch list shown or not
	*/
    TBool iRangeSwitchExpanded;

	/**
	* Current active Range
	*/
    TAknFepPenInputRange iCurrentActiveRange;

	/**
	* Current Range list
	* Own
	*/
    RArray<TInt> iCurrentRangeList;
    
	/**
	* Latch pictures
	*/
    TLatchedPic iLatchedBtnPic[ELatchPicCount];
    
	/**
	* Button width
	*/
    TInt iUnitWidth;

	/**
	* Button height
	*/
    TInt iUnitHeight;
    
    /**
     * background window for close,option,switch button
     * not own.
     */
    CControlGroup* iBkgndWnd1;
    
    /**
     * background window for more,backspace button.
     * not own.
     */
    CControlGroup* iBkgndWnd2;
	};

#endif //__PENINPUTHWRFSCNCONTROLBAR_H__