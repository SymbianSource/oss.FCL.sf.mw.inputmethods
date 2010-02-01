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

// SYSTEM INCLUDES
#include <AknsUtils.h>
#include <AknUtils.h>
#include <peninputhwrfscn.mbg>
#include <peninputhwrfscreencn.rsg>
#include <peninputcmd.h>
#include <AknFepGlobalEnums.h>
#include <peninputlayoutinputmodechoice.h>

// USER INCLUDES
#include "peninputhwrfscnlayout.h"
#include "peninputhwrfscncontrolbar.h"
#include "peninputhwrfscncontrolid.h"
#include "peninputhwrfscndatastore.h"
#include "peninputhwrfscnevent.h"
#include "peninputhwrfscnlafmanager.h"

#include "peninputhwrfscnbkgnd.h"

// CONSTANTS

//Range defination
const TInt Ranges[3] = {ERangeNative, ERangeEnglish, ERangeNumber};

const TUint8 KControlBarWidthUnitNum  = 4;
const TUint8 KUnitNumDiffOfMoreFuncAndControlBar = 2;
const TUint8 KBackspaceWidthUintNum = 2;

// ============================ MEMBER FUNCTIONS ===============================
  
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnControlBar* CPeninputHwrfscnControlBar::NewL( 
    CPeninputHwrfscnLayout* aLayout )
    {
    CPeninputHwrfscnControlBar* self = new 
        ( ELeave ) CPeninputHwrfscnControlBar( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( );
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::CPeninputHwrfscnControlBar()
// .
// -----------------------------------------------------------------------------
//	
CPeninputHwrfscnControlBar::CPeninputHwrfscnControlBar( 
    CPeninputHwrfscnLayout* aLayout )
    :CPeninputHwrfscnBasePanel( aLayout ),
    iRangeSwitchExpanded( EFalse ),
    iCurrentActiveRange( ERangeInvalid )
    {
    // Temp use
    iPos = TPoint( 0, 0 );

    // Use the iCurrentActiveRange to record which Range is active now
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::~CPeninputHwrfscnControlBar()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnControlBar::~CPeninputHwrfscnControlBar( )
    {
    delete iFunctionPanel;
    iCurrentRangeList.Close( );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ConstructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::ConstructL( )
    {
    // First set all Range available
    iLayout->DataStore( ).SetPermittedRanges( 
        ERangeNative | ERangeEnglish | ERangeNumber );
    iLayout->DataStore( ).AddSubscriber( this );

    UpdateCurrentActiveRangeList( );
    
    ReadLAFInfo( );
    
    iBkgndWnd1 = CPeninputHwrfscnBkgnd::NewL( iLayout, EHwrCtrlIdBkgndWnd1 );
    iBkgndWnd1->SetResourceId( R_PENINPUT_LAYOUT_WINDOW_BACKGROUND );
    iBkgndWnd1->ConstructFromResourceL();
    AddToRootControlL( iBkgndWnd1 );
    
    iBkgndWnd2 = CPeninputHwrfscnBkgnd::NewL( iLayout, EHwrCtrlIdBkgndWnd2 );
    iBkgndWnd2->SetResourceId( R_PENINPUT_LAYOUT_WINDOW_BACKGROUND );
    iBkgndWnd2->ConstructFromResourceL();
    AddToRootControlL( iBkgndWnd2 );
    
    CreateCloseBtnL();
    
    CreateOptionBtnL();
    
    CreateVkbSwitcherBtnL();
    
    CreateRangeSwitchBtnGroupL( );

    // Create the a button group which cover the first Range switch btn
    // It is to say, when the Range switch group expanded it use the 
    // real Range btn, when rolled-up, it will display this cover btn
    CreateRangeSwitchCoverBtnGroupL( );

    CreateMoreFuncBtnL( );

    CreateMoreFuncPanelL( );

    CreateBackSpaceBtnL( );
    
    iInputModeSwitch = CPeninputLayoutInputmodelChoice::NewL(
                                     iLayout,
                                     EHwrCtrlIdInputModeSwitcher,
                                     EPluginInputModeFSc );
    iInputModeSwitch->SetListSkinID( KAknsIIDQsnFrList, KAknsIIDQsnFrPopupSub );
    iLayout->AddControlL( iInputModeSwitch );
    iInputModeSwitch->AddEventObserver( iLayout );
    
    // resize iBkgndWnd1, cover following buttons: close, option, switcher
    TRect rect = ControlBarBtnRect( ECloseBtn );
    rect.BoundingRect( ControlBarBtnRect( EOptionBtn ) );
    rect.BoundingRect( ControlBarBtnRect( EVkbSwitcherBtn ) );
    iBkgndWnd1->SetRect( rect );

    // resize iBkgndWnd2, cover following buttons: more, backspace
    rect = MoreFuncBtnRect();
    rect.BoundingRect( BackSpaceBtnRect() );
    iBkgndWnd2->SetRect( rect );
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::Show()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::Show( TBool /*aVisible*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::Draw()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputHwrfscnControlBar::Draw( )
    {
    TRect rect( iMoreFuncBtn->Rect( ) );

    iMoreFuncBtn->Draw( );
    iLayout->LayoutOwner( )->UpdateArea( rect, EFalse,EFalse );

    
    rect = iCoverRangeSwitchBtnGroup->Rect();
    iCoverRangeSwitchBtnGroup->Draw( );
    iLayout->LayoutOwner( )->UpdateArea( rect, EFalse,EFalse );
    
    rect = iRangeSwitchBtnGroup->Rect( );
    iRangeSwitchBtnGroup->Draw( );
    iLayout->LayoutOwner( )->UpdateArea( rect, EFalse,EFalse ); 
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::HandleControlEvent()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::HandleControlEvent( 
    TInt aEventType, 
    CFepUiBaseCtrl* aCtrl, 
	const TDesC& aEventData )
    {
    switch( aEventType )
        {
        // This event is passed from the symbol list
        case EEventVirtualKeyUp:
   		    {
   		    iMoreFuncBtn->SetHighlight( EFalse );  
            ReportEvent( aEventType, aCtrl, aEventData );
   		    Draw( );  
   		    }
            break;

        case EEventButtonUp:
            {
            OnControlEvent_ButtonUp( aEventType, aCtrl, aEventData );
            }
            break;

        case EEventButtonDown:
            {
            OnControlEvent_ButtonDown( aEventType, aCtrl, aEventData );
            }
            break;
        case EHwrEventKeyBack:
            ReportEvent( aEventType, aCtrl, aEventData );
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::HandleCommand( TInt aCommand, TUint8* /*aData*/ )
    {
    if ( aCommand == EHwrEventPointerOutsideWnd )
        {
        // Hide the range list if it is extended
        if( iRangeSwitchExpanded )
           {
           CloseRangeSwitchList( );
           }
        
        // Hide the function panel if it is visible
        if( iFunctionPanel->IsVisible( ) )
    	    {
    	    ShowMoreFuncList( EFalse );
    	    }
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::BeforePropertyChangedL()
// .
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnControlBar::BeforePropertyChangedL( 
    MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty /*aProptery*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::PropertyChangedL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::PropertyChangedL( 
	MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty aProptery )
    {
    // If the permitted range changed, it should reorganized the Range switch btn
    if ( EPeninputHwrfscnPropertyPermittedRange == aProptery )
        {
        //close the range switch list if it was expended before
        if( iRangeSwitchExpanded )
            {
            CloseRangeSwitchList( );
            }
       
        //remove all buttons on range switch and cover range switch group
        RemoveAllSwitchBtns( );
        
        UpdateCurrentActiveRangeList( );
        
        UpdateRangeSwitchL( );
        
        UpdateCoverRangeSwitchL( );
        
        SetActiveCoverRangeBtn( );
        
        Draw();
        }
    else if( EPeninputHwrfscnPropertyRange == aProptery )
        {
        UpdateCurrentActiveRange( );
       
        HideCoverRangeSwitchBtns( );
		
		SetActiveCoverRangeBtn( );
		Draw();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::SizeChanged()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::SizeChanged( )
    {
    // read related LAF information    
    ReadLAFInfo( );
	
	// set the function panel position
	iFunctionPanel->SetPostion( 
	    TPoint( iPos.iX-KUnitNumDiffOfMoreFuncAndControlBar*iUnitWidth,
	    iPos.iY+iUnitHeight ) );
    
    // change the cover range buttons size
	ResetCoverRangeSwitchSize( );
	
	// change the range list buttons size
	ResetRangeSwitchSize( );
	
	// change the more function button size
	ResetMoreFuncBtnSize( );
	
	// change the backspce button size
	ResetBackSpaceBtnSize( );
	
	// change option, vkbswitcher, close buttons size
	ResetControlBarBtnSize();
	
	// resize iBkgndWnd1, cover following buttons: close, option, switcher  
    TRect rect = ControlBarBtnRect( ECloseBtn );
    rect.BoundingRect( ControlBarBtnRect( EOptionBtn ) );
    rect.BoundingRect( ControlBarBtnRect( EVkbSwitcherBtn ) );
    iBkgndWnd1->SetRect( rect );

    // resize iBkgndWnd2, cover following buttons: more, backspace
    rect = MoreFuncBtnRect();
    rect.BoundingRect( BackSpaceBtnRect() );
    iBkgndWnd2->SetRect( rect );	
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::LafChanged()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::LafChanged( )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::OnControlEvent_ButtonDown()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::OnControlEvent_ButtonDown( TInt aEventType, 
    CFepUiBaseCtrl* aCtrl, 
	const TDesC& aEventData )
    {
    // When Range switch list shown, click on btn and not release mouse until
    // move the mouse outside the btn, the display status should not changed, i.e.
    // the latched status keeps the same
    if( iRangeSwitchExpanded && ClickIsInRangeSwitchGrp( aCtrl ) )
		{
		ShowRangeSwitchBtnsLatched( );
        
        static_cast<CAknFepCtrlCommonButton*>( aCtrl )->SetHighlight( ETrue );

        Draw( );
		}
		
    ReportEvent( aEventType, aCtrl, aEventData );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::EventTypeByCtrlId()
// .
// -----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnControlBar::EventTypeByCtrlId( TInt aCtrlId )
{
	TInt eventType = 0;
    
    switch ( aCtrlId )
        {
        case EHwrCtrlIdBackspace:
            eventType = EHwrEventKeyBack;
            break;
        
        case EHwrCtrlIdEnter:
            eventType = EHwrEventKeyEnter;
            break;
        
        case EHwrCtrlIdSpace:
            eventType = EHwrEventKeySpace;
            break;
                
        case EHwrCtrlIdMoreFunc:
            eventType = EHwrEventBtnMoreFunction;
            break;                
        
        case EHwrCtrlIdVkbSwitcher:
            eventType = EHwrEventVkbSwitch;
            break;
        
        case EHwrCtrlIdLanguageOption:
            eventType = EHwrEventTouchInputOption; 
            break;
        
        case EHwrCtrlIdChineseButton:
            eventType = EHwrEventSetRangeChinese;
            break;
        
        case EHwrCtrlIdEnglishButton:
            eventType = EHwrEventSetRangeEnglish;
            break;
        
        case EHwrCtrlIdNumberButton:
            eventType = EHwrEventSetRangeNumber;
            break;
        
        case EHwrCtrlIdCoverChineseButton:
        case EHwrCtrlIdCoverEnglishButton:
        case EHwrCtrlIdCoverNumberButton:
        case EHwrCtrlIdCoverSymbolButton:
            eventType = EHwrEventRangeClick;
            break;
            
        case EHwrCtrlIdClose:
            eventType = EHwrEventClose;
            break;
        default:
            break;
        }
        
    return eventType;
}

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::OnControlEvent_ButtonUp
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::OnControlEvent_ButtonUp( 
    TInt aEventType, 
    CFepUiBaseCtrl* aCtrl, 
	const TDesC& aEventData )
	{    
    TBool handled = EFalse;
    
    // if the rangelist is expanded and then close it
    
    if( aCtrl == iBackSpaceBtn )
        {
        handled = ETrue;
        }
    
    if( iRangeSwitchExpanded )
        {
        if( aCtrl == iBackSpaceBtn )
            {
            handled = ETrue;
            }
        CloseRangeSwitchList( );
        }
    
    // check which control was pressed    
    // event sent by backspace button
    if( aCtrl == iBackSpaceBtn )
		{
		if( iFunctionPanel->IsVisible( ) )
		    {
		    handled = ETrue;
		    ShowMoreFuncList( EFalse );
		    }
		}
        		
    // event sent by the button on rang list
    else if( ClickIsInRangeSwitchGrp( aCtrl ) )
        {
        // Set clicked btn as the active control
		iRangeSwitchBtnGroup->SetActiveCtrl( aCtrl );   
        }    
    // event sent by more function button
    else if( aCtrl == iMoreFuncBtn )
	    {
	    ShowMoreFuncList( !iFunctionPanel->IsVisible( ) );    
	    }
	// event sent by the button on cover range group    
	else if( aCtrl == iCoverRangeSwitchBtnGroup->ActiveControl( ) )
		{
        // Hide the more function panel if it is visible 
		if( iFunctionPanel->IsVisible( ) )
		    {
		    ShowMoreFuncList( EFalse );
		    }
        
        // if the rang number is more than 1, then open ranglist
        if( iCurrentRangeList.Count( ) > 1 )
            {
            OpenRangeSwitchList( );
            }   
		}
    else if( aCtrl->ControlId() == EHwrCtrlIdVkbSwitcher )
        {
        iLayout->ClearScreen();
		if( iFunctionPanel->IsVisible( ) )
		    {
		    ShowMoreFuncList( EFalse );
		    }
		else
		    {
		    TRect rect = aCtrl->Rect();   
            TRAP_IGNORE( iInputModeSwitch->PopUpSwitchListL( rect ) );
		    }       
        }
    else if( aCtrl->ControlId() == EHwrCtrlIdLanguageOption )
        {
		if( iFunctionPanel->IsVisible( ) )
		    {
		    ShowMoreFuncList( EFalse );
		    }
        }
	//the events sent by function panel
	else 
	    {
	    iMoreFuncBtn->SetHighlight( EFalse );
	    Draw( );
	    }
	
	// Give the event to layout if the event was not consumed by this panel
    if( !handled )
	    {	    	
	    // convert to eventtype by the control ID
	    aEventType = EventTypeByCtrlId( aCtrl->ControlId( ) );
	    
	    //sent to layout
	    ReportEvent( aEventType, aCtrl, aEventData );
	    }
	}

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateCloseBtnL()
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CreateCloseBtnL( )
    {
   	iCloseBtn = CAknFepCtrlEventButton::NewL( iLayout, EHwrCtrlIdClose, 
   											  0xFFFF,
   											  0,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
   	iCloseBtn->SetResourceId( R_AKN_FEP_HWR_CLOSE );
   	iCloseBtn->ConstructFromResourceL( );
   	TRect innerRect;
	AknPenImageUtils::CalculateGraphicRect( ControlBarBtnRect( ECloseBtn ), innerRect );
    iCloseBtn->SizeChanged( ControlBarBtnRect( ECloseBtn ), innerRect, ETrue );
   	iCloseBtn->SetRect( ControlBarBtnRect( ECloseBtn ) );
   	iBkgndWnd1->AddControlL( iCloseBtn );
   	iCloseBtn->AddEventObserver( this );    
    }

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateOptionBtnL()
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CreateOptionBtnL( )
    {
   	iOptionBtn = CAknFepCtrlEventButton::NewL( iLayout, EHwrCtrlIdLanguageOption, 
   											  0xFFFF,
   											  0,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
   	iOptionBtn->SetResourceId( R_AKN_FEP_HWR_OPTION );
   	iOptionBtn->ConstructFromResourceL( );
   	TRect innerRect;
	AknPenImageUtils::CalculateGraphicRect( ControlBarBtnRect( EOptionBtn ), innerRect );
   	
    iOptionBtn->SizeChanged( ControlBarBtnRect( EOptionBtn ), innerRect, ETrue );
   	iOptionBtn->SetRect( ControlBarBtnRect( EOptionBtn ) );
   	iBkgndWnd1->AddControlL( iOptionBtn );
   	iOptionBtn->AddEventObserver( this );    
    }

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateVkbSwitcherBtnL()
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CreateVkbSwitcherBtnL( )
    {
   	iVkbSwitcherBtn = CAknFepCtrlEventButton::NewL( iLayout, EHwrCtrlIdVkbSwitcher, 
   											  0xFFFF,
   											  0,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
   	iVkbSwitcherBtn->SetResourceId( R_AKN_FEP_HWR_VKB );
   	iVkbSwitcherBtn->ConstructFromResourceL( );
   	TRect innerRect;
	AknPenImageUtils::CalculateGraphicRect( ControlBarBtnRect( EVkbSwitcherBtn ), innerRect );   	
    iVkbSwitcherBtn->SizeChanged( ControlBarBtnRect( EVkbSwitcherBtn ), innerRect, ETrue );
   	iVkbSwitcherBtn->SetRect( ControlBarBtnRect( EVkbSwitcherBtn ) );
   	iBkgndWnd1->AddControlL( iVkbSwitcherBtn );
   	iVkbSwitcherBtn->AddEventObserver( this );    
    }

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateRangeSwitchBtnGroupL()
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CreateRangeSwitchBtnGroupL( )
    {
    iRangeSwitchBtnGroup = CPeninputHwrfscnBkgnd::NewL( iLayout, EHwrCtrlIdRangeBar );
    iRangeSwitchBtnGroup->SetResourceId( R_PENINPUT_LAYOUT_WINDOW_BACKGROUND );
    iRangeSwitchBtnGroup->ConstructFromResourceL();
    
    UpdateRangeSwitchL( );
    AddToRootControlL( iRangeSwitchBtnGroup );
    }

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateRangeSwitchCoverBtnGroupL()
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CreateRangeSwitchCoverBtnGroupL( )
    {
    iCoverRangeSwitchBtnGroup = CPeninputHwrfscnBkgnd::NewL( iLayout, EHwrCtrlIdCoverRangeBar );
    iCoverRangeSwitchBtnGroup->SetResourceId( R_PENINPUT_LAYOUT_WINDOW_BACKGROUND );
    iCoverRangeSwitchBtnGroup->ConstructFromResourceL();
    
    UpdateCoverRangeSwitchL( );

    iCoverRangeSwitchBtnGroup->HideControl( ActiveRangeIndex( ), EFalse );
    
    iCoverRangeSwitchBtnGroup->
        SetActiveCtrl( iCoverRangeSwitchBtnGroup->At( ActiveRangeIndex( ) ) );

    AddToRootControlL( iCoverRangeSwitchBtnGroup );
    }

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateMoreFuncBtnL()
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CreateMoreFuncBtnL( )
    {
    iMoreFuncBtn = CAknFepCtrlCommonButton::NewL( iLayout, EHwrCtrlIdMoreFunc,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
	TResourceReader reader;
    
    CCoeEnv::Static()->CreateResourceReaderLC( reader,
    	 			   R_AKN_FEP_HWR_MORE_FUNC );  
    iMoreFuncBtn->SetResourceId( R_AKN_FEP_HWR_MORE_FUNC );      
    iMoreFuncBtn->ConstructFromResourceL(); 
    
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );	 
    
	TRect innerRect;
    TRect rect(MoreFuncBtnRect());
	AknPenImageUtils::CalculateGraphicRect( rect, innerRect );
    iMoreFuncBtn->SizeChanged( rect, innerRect, ETrue );
    iMoreFuncBtn->SetRect( rect );
       
    /*iMoreFuncBtn->SetBmpStretchable( );

    CFbsBitmap* svgMaskLatched = NULL;
    CFbsBitmap* svgMaskUnLatched = NULL;

    TLatchPicIndex index = ELatchPicError; 
    GetLatchedIndexByCtrlID( EHwrCtrlIdMoreFunc,index );
    CreateBitMapL( EMbmPeninputhwrfscnQgn_indi_fep_button_morefunction, 
        iLatchedBtnPic[index].iSvgLatchedBMP, 
        svgMaskLatched );

    CreateBitMapL( EMbmPeninputhwrfscnQgn_indi_fep_button_morefunction_sel, 
        iLatchedBtnPic[index].iSvgUnLatchedBMP, 
        svgMaskUnLatched );

    iMoreFuncBtn->SetLatchedBitmap( 
        iLatchedBtnPic[index].iSvgLatchedBMP, svgMaskLatched );
    iMoreFuncBtn->SetUnLatchedBitmap( 
        iLatchedBtnPic[index].iSvgUnLatchedBMP, svgMaskUnLatched );

    iMoreFuncBtn->SetLatched( ETrue );*/

    iMoreFuncBtn->AddEventObserver( this );
    
    iBkgndWnd2->AddControlL( iMoreFuncBtn );
    }

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateMoreFuncPanelL()
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CreateMoreFuncPanelL( )
    {
    iFunctionPanel = CPeninputHwrfscnFunctionPanel::NewL( iLayout );
    iFunctionPanel->Show( EFalse );
	iFunctionPanel->SetPostion( 
	    TPoint( iPos.iX-KUnitNumDiffOfMoreFuncAndControlBar*iUnitWidth, 
	    iPos.iY+iUnitHeight ) );
    iFunctionPanel->AddEventObserver( this );
    }

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateBackSpaceBtnL()
// .
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CreateBackSpaceBtnL( )
    {
   	iBackSpaceBtn = CAknFepCtrlRepeatButton::NewL( iLayout, EHwrCtrlIdBackspace, 
   	                                          EHwrEventKeyBack,
   											  EKeyBackspace,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
   	iBackSpaceBtn->SetResourceId( R_AKN_FEP_HWR_BACK_SPACE );
   	iBackSpaceBtn->ConstructFromResourceL( );
   	TRect innerRect;
	AknPenImageUtils::CalculateGraphicRect( BackSpaceBtnRect( ), innerRect );     	
    iBackSpaceBtn->SizeChanged( BackSpaceBtnRect( ), innerRect, ETrue );
   	iBackSpaceBtn->SetRect( BackSpaceBtnRect( ) );
   	iBkgndWnd2->AddControlL( iBackSpaceBtn );
   	iBackSpaceBtn->AddEventObserver( this );
    }

// ----------------------------------------------------------------------------
// CChineseFullScreenLayout::CreateLatchedBtnLC
// .
// ----------------------------------------------------------------------------
//
CAknFepCtrlCommonButton* CPeninputHwrfscnControlBar::CreateLatchedBtnLC( 
    const TRect aRect, 
    TInt aCtrlId, 
    TInt aLatchedPicID, 
    TInt /*aUnLatchedID*/ )
	{
    CAknFepCtrlMultiImageButton* rangeBtn = CAknFepCtrlMultiImageButton::NewL( iLayout, aCtrlId,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );

    rangeBtn->SetResourceId( aLatchedPicID );      
    rangeBtn->ConstructFromResourceL(); 

	TRect innerRect;
	AknPenImageUtils::CalculateGraphicRect( aRect, innerRect );   	
    rangeBtn->SizeChanged( aRect, innerRect, ETrue );
   	rangeBtn->SetRect( aRect );
   	rangeBtn->AddEventObserver( this );
    return rangeBtn;
	}

// ----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::CreateNormalBtnLC
// .
// ----------------------------------------------------------------------------
//
CAknFepCtrlCommonButton* CPeninputHwrfscnControlBar::CreateNormalBtnLC( const TRect aRect, 
    TInt aCtrlId, 
    TInt aResID )
	{
   	CAknFepCtrlCommonButton* coverRangeSwitchBtn = 
   	    CAknFepCtrlCommonButton::NewL( iLayout, aCtrlId,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
   	
   	coverRangeSwitchBtn->SetResourceId( aResID );
   	coverRangeSwitchBtn->ConstructFromResourceL( );
	TRect innerRect;
	AknPenImageUtils::CalculateGraphicRect( aRect, innerRect );   	
    coverRangeSwitchBtn->SizeChanged( aRect, innerRect, ETrue );
   	coverRangeSwitchBtn->SetRect( aRect );
   	coverRangeSwitchBtn->Hide(ETrue);
   	coverRangeSwitchBtn->AddEventObserver( this );

    return coverRangeSwitchBtn;
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::CloseRangeSwitchList()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::CloseRangeSwitchList( )
    {    
	// Hide all Range btns
	HideRangeSwitchBtns( ETrue );

    // Hide all cover range btns
	HideCoverRangeSwitchBtns( );
    
    // set the active cover range button
    SetActiveCoverRangeBtn( );
    
    // set the switch expanded state to be false
    iRangeSwitchExpanded = EFalse;
    
    // Redraw them
    Draw( );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::HideCoverRangeSwitchBtns()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnControlBar::HideCoverRangeSwitchBtns( )
    {
    TInt count = iCoverRangeSwitchBtnGroup->NumOfControls( ); 
    
    for( TInt i = 0; i < count; i++ )
        {
        iCoverRangeSwitchBtnGroup->HideControl( i ); 
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::HideCoverRangeSwitchBtns()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnControlBar::SetActiveCoverRangeBtn( )
    {
    TInt curActiveRange = ActiveRangeIndex( );
    iCoverRangeSwitchBtnGroup->HideControl( curActiveRange, EFalse );
    iCoverRangeSwitchBtnGroup->SetActiveCtrl( curActiveRange );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::HideCoverRangeSwitchBtns()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnControlBar::HideRangeSwitchBtns( TBool aHide )
    {
    TInt count = iRangeSwitchBtnGroup->NumOfControls( );
	iRangeSwitchBtnGroup->Hide( aHide );
	for( TInt i = 0; i < count; i++ )
    	{
   		iRangeSwitchBtnGroup->HideControl( i,aHide );
    	}
    }

    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::RemoveAllSwitchBtns()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnControlBar::RemoveAllSwitchBtns( )
    {
    TInt count = iRangeSwitchBtnGroup->NumOfControls( );
    
    for ( int i = 0; i < count; i++ )
        {
        iRangeSwitchBtnGroup->RemoveControl( iRangeSwitchBtnGroup->At( 0 ) );
        iCoverRangeSwitchBtnGroup->RemoveControl( 
            iCoverRangeSwitchBtnGroup->At( 0 ) );
        }
    }        

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::OpenRangeSwitchList()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnControlBar::OpenRangeSwitchList( )
    {
    // Show all items in the Range switch list
    HideRangeSwitchBtns( EFalse );
	
	// set the buttons to be latched
	ShowRangeSwitchBtnsLatched( );
	
	// Hide all items of the btns used to cover the Range switch list
	HideCoverRangeSwitchBtns( );        
    
    // Highlight the previous selected item
	static_cast<CAknFepCtrlCommonButton*>
	    ( iRangeSwitchBtnGroup->At( ActiveRangeIndex( ) ) )->SetHighlight( ETrue );
	
    iRangeSwitchExpanded = ETrue;
    
    Draw( );
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ShowRangeSwitchBtnsLatched()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnControlBar::ShowRangeSwitchBtnsLatched( )
    {
    // Show all items in the Range switch list
	TInt count = iRangeSwitchBtnGroup->NumOfControls( ); 
    
    for( TInt i = 0; i < count; i++ )
        {
        static_cast<CAknFepCtrlCommonButton*>
            ( iRangeSwitchBtnGroup->At( i ) )->SetHighlight( EFalse );
        }
    } 
   
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ShowMoreFuncList()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnControlBar::ShowMoreFuncList( TBool aShowNow )
    {
    iFunctionPanel->Show( aShowNow );
    iMoreFuncBtn->SetHighlight( aShowNow );
    Draw( );
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ClickIsInRangeSwitchGrp()
// .
// -----------------------------------------------------------------------------
//	
TBool CPeninputHwrfscnControlBar::ClickIsInRangeSwitchGrp( CFepUiBaseCtrl* aCtrl )
    {
    TInt count = iRangeSwitchBtnGroup->NumOfControls( );
    
    for ( int i = 0; i < count; i++ )
        {
        if ( aCtrl == iRangeSwitchBtnGroup->At( i ) )
            {
            return ETrue;
            }
        }
        
    return EFalse;
    }

    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::UpdateRangeSwitchL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::UpdateRangeSwitchL( )
    {
    for( int i = 0; i < iCurrentRangeList.Count( ); ++i )
    	{
    	CAknFepCtrlCommonButton* tempControl = NULL;
        TBool handled = EFalse;
        
        if ( iCurrentRangeList[i] == ERangeNative )
            {
            tempControl = CreateNormalBtnLC( 
   	            RangeSwitchBtnRect( i ), 
   	            EHwrCtrlIdChineseButton, 
   	            R_AKN_FEP_HWR_COVER_CHINESE);
   	        
   	        handled = ETrue;
            }
        else if ( iCurrentRangeList[i] == ERangeEnglish )
            {
            tempControl = CreateNormalBtnLC( 
   	            RangeSwitchBtnRect( i ), 
   	            EHwrCtrlIdEnglishButton, 
   	            R_AKN_FEP_HWR_COVER_LATIN);
   	        
   	        handled = ETrue;
            }
        else if ( iCurrentRangeList[i] == ERangeNumber )
            {
            tempControl = CreateNormalBtnLC( 
   	            RangeSwitchBtnRect( i ), 
   	            EHwrCtrlIdNumberButton, 
   	            R_AKN_FEP_HWR_COVER_NUM);
   	        
   	        handled = ETrue;
            }
            
/*		TRect innerRect;
	    TRect rect(RangeSwitchBtnRect(i));
		AknPenImageUtils::CalculateGraphicRect( rect, innerRect );
	    tempControl->SizeChanged( rect, innerRect, ETrue );
	    tempControl->SetRect( rect );  */          
            
        if ( handled )
            {
            iRangeSwitchBtnGroup->AddControlL( tempControl );
            iRangeSwitchBtnGroup->HideControl( i );
            //CleanupStack::Pop( tempControl );            
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::UpdateCoverRangeSwitchL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::UpdateCoverRangeSwitchL( )
    {     
    // When only one Range available, use the picture without small
    // triangle mark
    if ( 1 == iCurrentRangeList.Count( ) )
        {
        UpdateWithoutMarkL( );     
        }
    else
        {
        UpdateWithMarkL( );
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::UpdateWithoutMarkL
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnControlBar::UpdateWithoutMarkL( )
    {
    TBool handled = EFalse;
    CFepUiBaseCtrl* tempControl = NULL;
    if ( iCurrentActiveRange == ERangeNative )
        {
        tempControl = CreateNormalBtnLC( CoverRangeSwitchRect( ), 
            EHwrCtrlIdCoverChineseButton, 
            R_AKN_FEP_HWR_COVER_CHINESE );
        
        handled = ETrue;
        }
    else if ( iCurrentActiveRange == ERangeEnglish )
        {
       	tempControl = CreateNormalBtnLC( CoverRangeSwitchRect( ), 
            EHwrCtrlIdCoverEnglishButton, 
            R_AKN_FEP_HWR_COVER_LATIN );
        
        handled = ETrue;
        }
    else if ( iCurrentActiveRange == ERangeNumber )
        {
       	tempControl = CreateNormalBtnLC( CoverRangeSwitchRect( ), 
            EHwrCtrlIdCoverNumberButton, 
            R_AKN_FEP_HWR_COVER_NUM );
        
        handled = ETrue;                   	        
        }
        
    if ( handled )
        {
        iCoverRangeSwitchBtnGroup->AddControlL( tempControl );
        iCoverRangeSwitchBtnGroup->HideControl( 0 );
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::UpdateWithMarkL
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnControlBar::UpdateWithMarkL( )
    {
    TRect rect( CoverRangeSwitchRect( ) );
      	
    TInt rangeCount = iCurrentRangeList.Count( );
    
    for( int i = 0; i < rangeCount; ++i )
    	{
        CFepUiBaseCtrl* tempControl = NULL;
        TBool handled = EFalse;
        
        if ( iCurrentRangeList[i] == ERangeNative )
            {
            tempControl = CreateLatchedBtnLC( rect, 
                EHwrCtrlIdCoverChineseButton, 
                R_AKN_FEP_HWR_CHINESE,
                EMbmPeninputhwrfscnQgn_indi_fep_button_chinese_sel );
            
            handled = ETrue;
            }
        else if ( iCurrentRangeList[i] == ERangeEnglish )
            {
           	tempControl = CreateLatchedBtnLC( rect, 
                EHwrCtrlIdCoverEnglishButton, 
                R_AKN_FEP_HWR_LATIN,
                EMbmPeninputhwrfscnQgn_indi_fep_button_chinese_sel );
            
            handled = ETrue;
            }
        else if ( iCurrentRangeList[i] == ERangeNumber )
            {
           	tempControl = CreateLatchedBtnLC( rect, 
                EHwrCtrlIdCoverNumberButton, 
                R_AKN_FEP_HWR_NUM,
                EMbmPeninputhwrfscnQgn_indi_fep_button_chinese_sel );
        
            
            handled = ETrue;
            }
        
        if ( handled )
            {
            iCoverRangeSwitchBtnGroup->AddControlL( tempControl );
            iCoverRangeSwitchBtnGroup->HideControl( i );
            }
        }
    }
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::UpdateCoverRangeSwitchL()
// .
// -----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnControlBar::ActiveRangeIndex( )
    {
    return iCurrentRangeList.Find( iCurrentActiveRange );
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::UpdateCurrentActiveRangeList()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::UpdateCurrentActiveRangeList( )
    {
    // Use this list to store the temp permitted Range list, as the list got 
    // from DataStore will change when some functions were called
    iCurrentRangeList.Reset( );
    
    // Check whether the three Ranges we cared about were exist in the list from DataSore, 
    // If so, add corresponding Range in the iCurrentRangeList follow the sequence:
    // ERangeNative -> ERangeEnglish -> ERangeNumber
    TInt rangeCount = sizeof( Ranges ) / sizeof( TInt );
    
    const RArray<TInt>& rangeArray = iLayout->DataStore( ).PermittedRanges( );
    
    for ( int i = 0; i < rangeCount; i++ )
        {
        if( rangeArray.Find( Ranges[i] ) != KErrNotFound )
			{
			iCurrentRangeList.Append( Ranges[i] );
			}
        }
        
    UpdateCurrentActiveRange( );
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::UpdateCurrentActiveRange()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::UpdateCurrentActiveRange( )
    {
    iCurrentActiveRange = 
        static_cast<TAknFepPenInputRange>( 
        iLayout->DataStore( ).PermittedRanges( )[0] );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::GetLatchedIndexByCtrlID()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::GetLatchedIndexByCtrlID( TInt aCtrlID, 
    TLatchPicIndex& aIndex )
    {
    TLatchPicIndex latchedPicID = ELatchPicError;
    
    switch ( aCtrlID )
        {
        case EHwrCtrlIdMoreFunc:
            latchedPicID = ELatchPicMoreFunc;
            break;
        
        case EHwrCtrlIdChineseButton:
            latchedPicID = ELatchPicChineseButton;
            break;
        
        case EHwrCtrlIdEnglishButton:
            latchedPicID = ELatchPicEnglishButton;
            break;
        
        case EHwrCtrlIdNumberButton:
            latchedPicID = ELatchPicNumberButton;
            break;
        
        default:
            break;
        }
    
    aIndex = latchedPicID;
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ReadLAFInfo()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::ReadLAFInfo( )
    {
    TRect rect( PeninputHwrfscnLAFManager::ControlBarUnitRect( ) ); 	
	iUnitWidth = rect.Width( );
	iUnitHeight = rect.Height( );

	iPos = PeninputHwrfscnLAFManager::ControlBarLeftTopPosition( );
	
	// Offset lefttop position for fixing bug
	TPoint offsetPt( iUnitWidth*EControlBarBtnCount, 0 );
	iPos += offsetPt;
	
    // In case that the control bar and more function panel can not be fully shown
	if( ( iPos.iX - KUnitNumDiffOfMoreFuncAndControlBar * iUnitWidth ) < 0 )
		{
		iPos.iX = KUnitNumDiffOfMoreFuncAndControlBar * iUnitWidth;
		}
    else if( ( iPos.iX + KControlBarWidthUnitNum * iUnitWidth ) > 
        iLayout->ScreenSize( ).iWidth )
		{
		iPos.iX = iLayout->ScreenSize( ).iWidth - KControlBarWidthUnitNum * iUnitWidth;
		}
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ResetCoverRangeSwitchSize()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::ResetCoverRangeSwitchSize( )
	{
    // Get the rect
    TRect rect( CoverRangeSwitchRect( ) );
    
    // change the button group rect
    iCoverRangeSwitchBtnGroup->SetRect( rect );
    
    TInt count = iCoverRangeSwitchBtnGroup->NumOfControls( );
    
    for ( int i = 0; i < count; i++ )
        {
        CAknFepCtrlCommonButton* button = 
            static_cast<CAknFepCtrlCommonButton*>( iCoverRangeSwitchBtnGroup->At( i ) );
            
		TRect innerRect;
		AknPenImageUtils::CalculateGraphicRect( rect, innerRect );
        button->SizeChanged( rect, innerRect, ETrue );
       
        button->SetRect( rect );
        }
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ResetRangeSwitchSize()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::ResetRangeSwitchSize( )
	{
    TInt count = iRangeSwitchBtnGroup->NumOfControls( );
    
    for( int i = 0; i < count; ++i )
    	{
    	CAknFepCtrlCommonButton* button = 
                          static_cast<CAknFepCtrlCommonButton*>(iRangeSwitchBtnGroup->At( i ));
    	/*
    	TLatchPicIndex index = ELatchPicError; 
    	
    	GetLatchedIndexByCtrlID( button->ControlId( ), index );
        
        // Bitmap should be resized by the first time construct
        TSize sizeBitMap( iUnitWidth, iUnitHeight );
        
        AknIconUtils::SetSize( iLatchedBtnPic[index].iSvgLatchedBMP, 
            sizeBitMap, EAspectRatioNotPreserved );
        
        AknIconUtils::SetSize( iLatchedBtnPic[index].iSvgUnLatchedBMP, 
            sizeBitMap, EAspectRatioNotPreserved );*/
		TRect innerRect;
		AknPenImageUtils::CalculateGraphicRect( RangeSwitchBtnRect( i ), innerRect );
        button->SizeChanged(RangeSwitchBtnRect( i ), innerRect, ETrue);
        
        button->SetRect( RangeSwitchBtnRect( i ) );
    	}
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ResetMoreFuncBtnSize()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::ResetMoreFuncBtnSize( )
	{
	TLatchPicIndex index = ELatchPicError; 
	GetLatchedIndexByCtrlID( EHwrCtrlIdMoreFunc, index );
    
    // Bitmap should be resized by the first time construct
    /*TSize sizeBitMap( iUnitWidth, iUnitHeight );
    
    AknIconUtils::SetSize( iLatchedBtnPic[index].iSvgLatchedBMP, 
        sizeBitMap, EAspectRatioNotPreserved );
                           
    AknIconUtils::SetSize( iLatchedBtnPic[index].iSvgUnLatchedBMP, 
        sizeBitMap, EAspectRatioNotPreserved );*/
        
	TRect innerRect;
	AknPenImageUtils::CalculateGraphicRect( MoreFuncBtnRect(), innerRect );
    iMoreFuncBtn->SizeChanged(MoreFuncBtnRect(), innerRect, ETrue);
    iMoreFuncBtn->SetRect( MoreFuncBtnRect( ) );    
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ResetBackSpaceBtnSize()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::ResetBackSpaceBtnSize( )
	{
    TRect rect( BackSpaceBtnRect( ) );
	TRect innerRect;
	AknPenImageUtils::CalculateGraphicRect( rect, innerRect );    
    iBackSpaceBtn->SizeChanged( rect, innerRect, ETrue );
    iBackSpaceBtn->SetRect( rect );
	}
	
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ResetControlBarBtnSize()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnControlBar::ResetControlBarBtnSize( )
	{
	TRect innerRect;
    TRect rect(ControlBarBtnRect(ECloseBtn));
	AknPenImageUtils::CalculateGraphicRect( rect, innerRect );
    iCloseBtn->SizeChanged( rect, innerRect, ETrue );
    iCloseBtn->SetRect( rect );
    
	rect = ControlBarBtnRect(EOptionBtn);
    AknPenImageUtils::CalculateGraphicRect( rect, innerRect ); 
    iOptionBtn->SizeChanged(rect, innerRect, ETrue);
    iOptionBtn->SetRect(rect);
    
    rect = ControlBarBtnRect(EVkbSwitcherBtn);
    AknPenImageUtils::CalculateGraphicRect( rect, innerRect ); 
    iVkbSwitcherBtn->SizeChanged(rect, innerRect, ETrue);
    iVkbSwitcherBtn->SetRect(rect);
	}	
	

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::CoverRangeSwitchRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnControlBar::CoverRangeSwitchRect( )
	{
	return TRect ( iPos, TSize( iUnitWidth, iUnitHeight ) );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::RangeSwitchBtnRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnControlBar::RangeSwitchBtnRect( TInt aIndex )
	{	
	TPoint pos( iPos.iX, iPos.iY + iUnitHeight * aIndex );
	
	return  TRect( pos, TSize( iUnitWidth, iUnitHeight ) );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::MoreFuncBtnRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnControlBar::MoreFuncBtnRect( )
	{
	TRect coverRect( CoverRangeSwitchRect( ) );
	
	TPoint pos( coverRect.iBr.iX, coverRect.iTl.iY );
	
	return TRect ( pos, TSize( iUnitWidth,iUnitHeight ) );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::BackSpaceBtnRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnControlBar::BackSpaceBtnRect( )
	{
	TRect funcRect( MoreFuncBtnRect( ) );
	
	TPoint pos( funcRect.iBr.iX, funcRect.iTl.iY );
	
	TSize size( iUnitWidth * KBackspaceWidthUintNum, iUnitHeight );
	
	return TRect ( pos, size );
	}
	
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::ControlBarBtnRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnControlBar::ControlBarBtnRect( TInt aIndex )
	{
    TSize size( iUnitWidth, iUnitHeight );
    
    return TRect( TPoint( iPos.iX - iUnitWidth * (EControlBarBtnCount - aIndex), iPos.iY ), size );
	}
	
// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::HandleSkinChanged()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnControlBar::HandleSkinChanged()
	{
	iFunctionPanel->HandleSkinChanged();
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnControlBar::SetEnableSettingBtn()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnControlBar::SetEnableSettingBtn(TBool aEnable)
    {
    if ( aEnable )    
        {
        iOptionBtn->SetDimmed(EFalse);
        }
    else
        {
        iOptionBtn->SetDimmed(ETrue);
        iOptionBtn->SetActive(EFalse);
        }
    }
	
//End of file
