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
* Description:  Implementation of hwr window class
*
*/


// System includes
#include <peninputhwrwindow.rsg>
#include <peninputhwrwindowconfiginfo_01.rsg>
#include <aknfeppeninputenums.h>
#include <peninputinputcontextfield.h>
#include <peninputpluginutils.h>
#include <peninputcommonlayout.h>
#include <peninputeventbutton.h>
#include <peninputrepeatbutton.h>
#include <peninputlayouthwrwnd.h>
#include <peninputrangebar.h>
#include <peninputdataconverter.h>
#include <peninputdataprovider.h>
#include <peninputclientlayoutinfo.h>
#include <AknIconUtils.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknsUtils.h>
#include <peninputlayoutchoicelist.h>
#include <AknFepGlobalEnums.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputlayoutmultilineicf.h>

// User includes
#include "peninputlayoutchoicelist.h"
#include "peninputgenerichwrwindow.h"
#include "peninputgenerichwrdatamgr.h"
#include "peninputgenerichwr.hrh"
#include "peninputgenericrecognizer.h"
#include "peninputgenerichwrlayout.h"

// Constants
_LIT( KHwrWindowResourceFile, 
      "z:\\resource\\plugins\\peninputhwrwindow.RSC" );
_LIT( KConfigurationResourceFile, 
      "z:\\resource\\plugins\\peninputhwrwindowconfiginfo_" );
_LIT( KResourceFileExtName, ".RSC" );


const TInt KWrittingFactor = 0;
const TInt KNormalFactor = 26;

const TInt KWriteBoxFrameBackColorMajor = EAknsMajorSkin;
const TInt KWriteBoxFrameBackColorGrp = EAknsMinorQsnOtherColors;
const TInt KWriteBoxFrameBackColorIdx = EAknsCIQsnOtherColorsCG9; 
const TUint32 KDefaultWriteBoxFrameColor = 0x000000;
const TUint32 KDefaultWriteBoxBackColor = 0xffffff;

const TInt KPeninputHwrWndInvalidIndex = -1;
// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::CPeninputGenericHwrWindow
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericHwrWindow::CPeninputGenericHwrWindow( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    : CPeninputLayoutWindow( aUiLayout, aLayoutContext ),
      iNormalTransFactor(KNormalFactor),
      iWriteTransFactor(KWrittingFactor),
      iGuideLineSet(EFalse)
    {
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericHwrWindow* CPeninputGenericHwrWindow::NewL( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    {
    CPeninputGenericHwrWindow* self = 
        new ( ELeave ) CPeninputGenericHwrWindow( aUiLayout, aLayoutContext );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::~CPeninputGenericHwrWindow
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericHwrWindow::~CPeninputGenericHwrWindow()
    {
    for ( TInt i = 0; i < iRecogResult.Count(); i++ )
        {
        delete iRecogResult[i];
        }
    iRecogResult.Close();
    iStrokeArray.Close();
    delete iLastResult;
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::ConstructL()
    {
    CPeninputLayoutWindow::ConstructL();   
    CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
        ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
        iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );

    if ( ( penData ) && ( iHwBox ) )
        {
        iHwBox->SetCharacterDelay( penData->iWritingSpeed );
        iHwBox->SetFadingSpeed(penData->iFadingSpeed);
#ifdef _NOT_USING_DECUMA_MCR_
        // similar as S90     

        iHwBox->SetStrokeDelay( 90000 );
        
#endif
        
        iHwBox->SetPenSize( penData->iPenSize );
        iHwBox->SetPenColor( penData->iPenColor );
        iHwBox->SetWndTransparencyFactor(iNormalTransFactor);
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::HandleControlEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::HandleControlEvent( TInt aEventType,
                                                    CFepUiBaseCtrl* aCtrl, 
                                                    const TDesC& aEventData )
    {
    // Call this function in base class
    CPeninputLayoutWindow::HandleControlEvent( aEventType, 
                                               aCtrl, 
                                               aEventData );

    TInt* data = ( TInt* ) aEventData.Ptr(); 
    
    switch ( aEventType )
        {
        case EPeninputLayoutHwrEventPenColorChange:
        case EPeninputLayoutHwrEventPenSizeChange:
        case EPeninputLayoutHwrEventWritingSpeedChange:
            {
            CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
                ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
                iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );

            iHwBox->SetCharacterDelay( penData->iWritingSpeed );
            iHwBox->SetFadingSpeed(penData->iFadingSpeed);
            
#ifdef _NOT_USING_DECUMA_MCR_
        
            // similar as S90                 
            iHwBox->SetStrokeDelay( 90000 );
            
#endif

            iHwBox->SetPenSize( penData->iPenSize );
            iHwBox->SetPenColor( penData->iPenColor );
            }
            break;
        case EPeninputLayoutHwrEventGuideLineChanged:
            {
            CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
                ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
                iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );

            iHwBox->HideGuideLine( penData->iGuideLineOn == 0 ? EFalse : ETrue );
            iHwBox->RefreshUI();
            }
            break;
        case EEventHwrStrokeStarted:
            {
            OnStrokeStarted();
            }
            break;
        case EEventHwrStrokeFinished:
            {
            OnStrokeFinished();
            }
            break;
        case EEventHwrCharacterTimerOut:
            {
            OnStrokeCharacterTimerOut();
            }
            break;
        case EEventHwrStrokeCanceled:
            {
            OnStrokeCanceled();
            }
            break;  
        case EPeninputLayoutEventCloseWindow:
            {
            iStrokeArray.Reset();
            iHwBox->CapturePointer(EFalse);
            iHwBox->CancelCharWriting();
            }
            break;        
        case EEventChoiceSelected:
            {
            CFepLayoutChoiceList::SEvent* event = 
                ( CFepLayoutChoiceList::SEvent* ) aEventData.Ptr();
            
            if ( event->iIndex != KPeninputHwrWndInvalidIndex )
                {
                if (aCtrl->ControlId() == EPeninutWindowCtrlIdSwitcherPopupWindow)
                    {
                    if (EPluginInputModeHwr != event->iCommand)
                        {
                        TBool switchByMode = ETrue;
                        TInt  mode = event->iCommand;

                        TBuf<8> buf;
                        buf.Append(reinterpret_cast<TText*>(&switchByMode), 
                                   sizeof(TBool)/sizeof(TText));
                        buf.Append(reinterpret_cast<TText*>(&mode), sizeof(TInt)/sizeof(TText));
                        UiLayout()->SignalOwner(ESignalLayoutUIChanged,buf);
                        }
                    }
                }
            }
            break;
        case EPeninputLayoutEventToVkb:
            {
            iHwBox->CancelCharWriting();
            PopupSwitchWindow();
            }
            break; 
        default:
            break;
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::GetWindowConfigResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputGenericHwrWindow::GetWindowConfigResId()
    {
    return R_PENINPUT_LAYOUT_CONFIG_INFO;
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::GetWindowResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputGenericHwrWindow::GetWindowResId()
    {
    return R_PENINPUT_LAYOUT_HWR_WINDOW;        
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::GetWindowConfigResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TDesC& CPeninputGenericHwrWindow::GetWindowConfigResFileName( 
    TInt aLangID )
    {
    if ( aLangID <= 0 ) 
        {
        return KNullDesC;
        }

    iResourceFilename.Zero();
    iResourceFilename = KConfigurationResourceFile();
    
    if ( aLangID < 10 )
        {
         iResourceFilename.AppendNum( 0 );
         iResourceFilename.AppendNum( aLangID );
        }
    else
        {
         iResourceFilename.AppendNum( aLangID );
        }
    iResourceFilename.Append( KResourceFileExtName );
    
    return iResourceFilename;
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::GetWindowResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TDesC& CPeninputGenericHwrWindow::GetWindowResFileName()
    {
    return KHwrWindowResourceFile();
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::CreateAllControlsL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::CreateAllControlsL()
    {
    // Hwr box    
    AddHwBoxL();
    
    // Range bar
    AddRangeBarL();
               
    // Language switch button
    iLangSwitchBtn = AddButtonL( EPeninutWindowCtrlIdInputLangSwitcherBtn, 
                                 EPeninputLayoutEventSwitchLanguage, 
                                 R_PENINPUT_LAYOUT_HWR_SWITCH ); 
    SetSwitchBtnFont(*iLangSwitchBtn);
    SetSwitchBtnTextColor(*iLangSwitchBtn);
               
    // Switch to vkb button
    AddButtonL( EPeninutWindowCtrlIdSwitchToVkbBtn, 
                EPeninputLayoutEventToVkb, 
                R_PENINPUT_LAYOUT_HWR_VKB );
    
    iModeSwitchChoice = CPeninputLayoutInputmodelChoice::NewL( UiLayout(),
            EPeninutWindowCtrlIdSwitcherPopupWindow, EPluginInputModeHwr);
    iModeSwitchChoice->SetListSkinID( KAknsIIDQsnFrList, KAknsIIDQsnFrPopupSub );
    iModeSwitchChoice->AddEventObserver( UiLayout() );             
    AddControlL( iModeSwitchChoice );
    
    if( iCtrlPool != NULL )
        {
        for ( TInt i = 0; i < iCtrlPool->ControlCount(); i++ )
            {
            iCtrlPool->ControlByIndex( i )->AddEventObserver( UiLayout() );
            }
        }
    }
        
// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::ChangeUnitSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TRect CPeninputGenericHwrWindow::ChangeUnitSize()
    {
    if (CPeninputDataConverter::AnyToInt(
                                   iLayoutContext->RequestData(EAkninputDataTypeSizeChanging)) ||
        !(CPeninputDataConverter::AnyToInt(
                                   iLayoutContext->RequestData(EAkninputDataTypeUnitSizeSet))))
        {
        // if size changing, or unitwidth, unitheight has not been set yet
        TAknWindowLineLayout appWnd;
        TAknWindowLineLayout mainPane;
        TAknWindowLineLayout wndLayout;
        TAknWindowLineLayout unitLayout;
        TAknLayoutRect mainPaneRect;
        TAknLayoutRect wndRect;
        TAknLayoutRect unitRect;

        // Get hwr window layout
        appWnd = AknLayoutScalable_Avkon::application_window( 0 ).LayoutLine();

        if (appWnd.iW > appWnd.iH)
            {
    	      // landscape
    	      mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
            wndLayout = AknLayoutScalable_Avkon::popup_fep_hwr_window(2).LayoutLine();
            }
        else
            {
            // portrait	
            mainPane = AknLayoutScalable_Avkon::main_pane(1).LayoutLine();
            wndLayout = AknLayoutScalable_Avkon::popup_fep_hwr_window(0).LayoutLine();
            }

        mainPaneRect.LayoutRect(appWnd.Rect(), mainPane);
        wndRect.LayoutRect(mainPaneRect.Rect(), wndLayout);

        unitLayout = AknLayoutScalable_Avkon::fep_hwr_aid_pane(1).LayoutLine();
        unitRect.LayoutRect(wndRect.Rect(), unitLayout);

        iHwrWndRect = TRect(0,
                            0,
                            wndRect.Rect().Width(),
                            wndRect.Rect().Height());
                             
        TInt unitWidth = unitRect.Rect().Width();
        TInt unitHeight = unitRect.Rect().Height();

        iLayoutContext->SetData(EPeninputDataTypeUnitWidth, &unitWidth);
        iLayoutContext->SetData(EPeninputDataTypeUnitHeight, &unitHeight);
        }
    
    return iHwrWndRect;
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::ChangeClientSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::ChangeClientSize()
    {
    TInt unitWidth = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
    TInt unitHeight = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );   
        
    // Modify language switch button font
    SetSwitchBtnFont(*iLangSwitchBtn);

    TInt clientLayoutId = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );
    CPeninputClientLayoutInfo* clientLayout = 
        ConfigInfo()->FindClientLayoutInfo( clientLayoutId );
    
    if ( clientLayout )
        {
        // Change self rect
        /*TInt width = clientLayout->Columns() * unitWidth;
        TInt height = clientLayout->Rows() * unitHeight;
        TRect rect = TRect( ClientPane()->Rect().iTl, 
                            TSize( width, height ) );

        ClientPane()->SetRect( rect );*/
        // Change the postion and size of controls in current layout
        ReorganizeControls( clientLayout->LayoutID() );
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::ReorganizeControls
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::ReorganizeControls( TInt aClientLayoutId, TBool /*aNeedReset*/ )
    {
    TInt unitWidth = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
    TInt unitHeight = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );
 
    CPeninputClientLayoutInfo* clientLayout = 
                ConfigInfo()->FindClientLayoutInfo( aClientLayoutId );

    // First remove all controls in the client pane, but don't delete them
    ( const_cast<RPointerArray<CFepUiBaseCtrl>&>
        ( ClientPane()->ControlList() ) ).Reset();

    // Set case sensitive for the engine
    CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
        ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
        iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
    penData->iRecognizer->SetCaseSensitive( ConfigInfo()->CaseSensitive() );
    //Set guideline on or off
    penData->iRecognizer->EnableGuideline( penData->iGuideLineOn == 1? ETrue : EFalse );
    
    //reset window rect, titlebar rect, client rect
    TSize szClient( clientLayout->Columns() * unitWidth, 
                    clientLayout->Rows() * unitHeight );

    TSize szWnd( clientLayout->Columns() * unitWidth, 
                 clientLayout->Rows() * unitHeight + unitHeight );                       
                 
    TSize szTitle ( clientLayout->Columns() * unitWidth, unitHeight );
    
    SetWindowRect( TRect(Rect().iTl, szWnd) ); 
    
    //setwindowrect may change automactially for shadow or other things
    TPoint ptTopLeft = Rect().iTl;
    
    TitleBar()->SetRect( TRect(ptTopLeft, szTitle ) );
    ClientPane()->SetRect( TRect(ptTopLeft + TPoint(0, unitHeight),
                               szClient) );
                               
    // Get control list from client layout
    RPointerArray<CPeninputControlInfo>& controlList = 
        clientLayout->ControlInfoList();
    const TInt count = controlList.Count();
    TInt controlID = 0;
    TInt cols = 0;
    TInt rows = 0;
    CFepUiBaseCtrl* ctrl = NULL;
    
    //controls are based on client pane
    TPoint point = ptTopLeft + TPoint(0, unitHeight);
    
    for ( TInt i = 0; i < count; i++ )
        {
        controlID = controlList[i]->ControlID();
        ctrl = iCtrlPool->Control( controlID );

        if ( ctrl )
            {
            TRect rect;
            rect.iTl = point 
                + TPoint( controlList[i]->BeginColumn() * unitWidth, 
                          controlList[i]->BeginRow() * unitHeight );
            cols = controlList[i]->EndColumn() 
                - controlList[i]->BeginColumn();
            rows = controlList[i]->EndRow() 
                - controlList[i]->BeginRow();
            rect.SetSize( TSize( cols * unitWidth, rows * unitHeight ) ); 
            AddNotOwnedControl( ctrl );
            
            if ( ( ctrl->ControlType() | ECtrlButton ) == ECtrlButton )
                {
				TRect innerRect;
				AknPenImageUtils::CalculateGraphicRect( rect, innerRect );                    
                ( static_cast<CAknFepCtrlCommonButton*> ( ctrl ) )->SizeChanged
                    ( rect, innerRect, ETrue );
                }
            else if ( ctrl->ControlId() == EPeninutWindowCtrlIdRangeBar )
                {
                rect = TRect( rect.iTl, TSize( unitWidth, unitHeight ) );
                ( static_cast<CAknFepCtrlRangeBar*> ( ctrl ) )->SizeChanged( rect );
                }   
            else if ( ctrl->ControlId() == EPeninutWindowCtrlIdHwrBox )
                {
                iHwBox->SetRect( rect );
                DrawGuideLine();
                }
            }
        }   
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::CalculateGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::CalculateGuideLinePos()
	  {
	  TRect rect = iHwBox->Rect();
    TInt leftrightmargin = rect.Size().iWidth / 10;
    
    TSize size;
    size.iHeight = rect.iBr.iY - rect.iTl.iY;
    size.iWidth = rect.iBr.iX - rect.iTl.iX;
    iLayoutContext->SetData( EAkninputDataTypeInputArea, &size );
    
    TInt bottommargin = *(TInt*)iLayoutContext->RequestData( EAkninputDataTypeGuideLineBottom );
    TInt topmargin = *(TInt*)iLayoutContext->RequestData( EAkninputDataTypeGuideLineTop );
    	
    //TAknWindowLineLayout guideLineTop = AknLayoutScalable_Avkon::fep_hwr_write_pane_g5(0).LayoutLine();
    //TAknWindowLineLayout guideLineBottom = AknLayoutScalable_Avkon::fep_hwr_write_pane_g6(0).LayoutLine();
    
    iGuideLineTopTl.iX = rect.iTl.iX + leftrightmargin;
    iGuideLineTopTl.iY = rect.iTl.iY + topmargin;
    
    iGuideLineTopBr.iX = rect.iBr.iX - leftrightmargin;
    iGuideLineTopBr.iY = rect.iTl.iY + topmargin;
    
    iGuideLineBottomTl.iX = rect.iTl.iX + leftrightmargin;
    iGuideLineBottomTl.iY = rect.iTl.iY + bottommargin;
    
    iGuideLineBottomBr.iX = rect.iBr.iX - leftrightmargin;
    iGuideLineBottomBr.iY = rect.iTl.iY + bottommargin;
    
    iGuideLineSet = ETrue;
	  }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::DrawGuideLine
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::DrawGuideLine()
    {
    // Set guide line
    if ( ConfigInfo() )
        {
        TInt style = ConfigInfo()->GuideLine();
        
        CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
    ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
    iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
        TInt language = penData->iRecognizer->GetLanguage();
        
            TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );	
        
		if(language == ELangHindi && curRange == ERangeNative)
			style = EPeninputGuideLineTop;
		
        iHwBox->SetGuideLineStyle( style );

        // if size changing, or guide line pos has not been set
        CalculateGuideLinePos();
        
        if ( style == EPeninputGuideLineTop)
            {
            iHwBox->SetTopGuideLinePosition(iGuideLineTopTl, iGuideLineTopBr);
            }
        else if ( style == EPeninputGuideLineBottom)
            {
            iHwBox->SetBottomGuideLinePosition(iGuideLineBottomTl, iGuideLineBottomBr);
            }
        else if ( style == EPeninputGuideLineBoth)
            {
            iHwBox->SetTopGuideLinePosition(iGuideLineTopTl, iGuideLineTopBr);
            iHwBox->SetBottomGuideLinePosition(iGuideLineBottomTl, iGuideLineBottomBr);
            }
            
        iHwBox->RefreshUI();            
        }        
    }
// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::SetControlsFont
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::SetControlsFont()
    {
    TAknTextLineLayout textPaneTextLayout;
    TAknLayoutText txt;
    textPaneTextLayout = 
        AknLayoutScalable_Avkon::fep_hwr_top_text_pane_t1().LayoutLine();
  
    TInt leftMargin, rightMargin, topMargin, txtHeight;
    
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
    								(Control(EPeninputWindowCtrlIdMultiLineICF));
    TRect rect = icf->Rect();
  	txt.LayoutText( rect, textPaneTextLayout );
  	TRect txtRect = txt.TextRect();
  	leftMargin = txtRect.iTl.iX - rect.iTl.iX;
  	rightMargin = rect.iBr.iX - txtRect.iBr.iX;
  	topMargin = txtRect.iTl.iY - rect.iTl.iY;
  	txtHeight = txtRect.Height();
        	
    icf->SetTextMargin( leftMargin, rightMargin, topMargin, 0);
    icf->SetLineSpace( 1 );
    const CFont* icfFont = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, NULL );
        
    TRAP_IGNORE(icf->SizeChangedL( 
          static_cast<CFepLayoutMultiLineIcf*>(
                                  Control(EPeninputWindowCtrlIdMultiLineICF))->Rect(), 
                                  txtHeight,
                                  icfFont->FontMaxHeight(),
                                  icfFont));    
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::AddRangeBarL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::AddRangeBarL()
    {
    // Range bar
    iRangeBar = CAknFepCtrlRangeBar::NewL( UiLayout(), 
        EPeninutWindowCtrlIdRangeBar, 
        CAknFepCtrlRangeBar::EBtnGroupVertical );
          
    // Set event id
    iRangeBar->SetEventIdForRange( EPeninputLayoutEventRange );
    iRangeBar->SetEventIdForCase( EPeninputLayoutEventRangeLoop );
    iCtrlPool->AddControl( iRangeBar );
    }
        
// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::AddButtonL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputGenericHwrWindow::AddButtonL( 
    const TInt aControlId, const TInt aEventId, const TInt aResId,
    const TInt aUnicode, const TBool aIsRepeat )
    {
    CAknFepCtrlEventButton* button = NULL;
    
    if ( aIsRepeat )
        {
        button = CAknFepCtrlRepeatButton::NewL( UiLayout(), aControlId, 
                                                aEventId, aUnicode );
        }
    else
        {
        button = CAknFepCtrlEventButton::NewL( UiLayout(), aControlId, 
                                               aEventId, aUnicode,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
        }
        
    CleanupStack::PushL( button );
    if ( button )
        {
        // Read resource
        TResourceReader reader;
        
        CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );
        button->SetResourceId( aResId );
        button->ConstructFromResourceL();
        // Pop and destroy reader
        CleanupStack::PopAndDestroy( 1 );
        
        // Add into the control pool                                                   
        if ( iCtrlPool )
            {
            iCtrlPool->AddControl( button );
            }
        }
    CleanupStack::Pop( button );
    	     
    return button;
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::AddHwBoxL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::AddHwBoxL()
    {
    CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
        ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
        iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );

    iHwBox = CTransparentHwrWndExt::NewL( TRect(), UiLayout(), 
                                          EPeninutWindowCtrlIdHwrBox, 
                                          EFalse, penData->iGuideLineOn ); 
    iHwBox->SetEnableFade( ETrue );                                           
    iHwBox->EnableTraceOutsideWindow( EFalse );
    iHwBox->SetWndTransparencyFactor(iNormalTransFactor);

    if ( iCtrlPool )
        {
        iCtrlPool->AddControl( iHwBox );       
        }
    
    SetHwBoxFrameBackColor();
    }
    
// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::SetHwBoxFrameBackColor
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::SetHwBoxFrameBackColor()
    {
    TRgb frameColor;
    TRgb backColor;
    TAknsItemID id;
    
    id.Set(KWriteBoxFrameBackColorMajor, KWriteBoxFrameBackColorGrp);

    TInt error = AknsUtils::GetCachedColor(UiLayout()->SkinInstance(),
                                           frameColor,
                                           id,
                                           KWriteBoxFrameBackColorIdx);

    if (error != KErrNone)
        {
    	frameColor = TRgb(KDefaultWriteBoxFrameColor);
    	backColor = TRgb(KDefaultWriteBoxBackColor);
        }
    else
        {
        backColor = frameColor;	
        }
    
    if (iHwBox)
        {
    	iHwBox->SetFrameCol(frameColor);
    	iHwBox->SetBkColor(backColor);
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::PopupChoiceList
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::PopupChoiceList()
    {
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::DoCaseChange
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::DoCaseChange( TInt aNewCase )
    {
    if( iDisableCaseChange )
        {
        iCachedCase = aNewCase;
        return;
        }
        
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    TInt index = 0;
    
    if ( ( curRange == ERangeEnglish ) 
         || ( ( curRange == ERangeNative ) && ( ConfigInfo()->CaseSensitive() ) ) )
        {
        switch ( aNewCase )
            {
            case ECaseUpper:
                {
                index = 2;
                }
                break;
            case ECaseLower:
                {
                index = 1;
                }
                break;
            case ECaseText:
                {
                index = 0;
                }
                break;
            default:
                break;
            }
            
        TRAP_IGNORE( iRangeBar->SetCaseL( curRange, index ) );    
        iLayoutContext->SetData( EPeninputDataTypeCase, &aNewCase );
        }
        //to modify guideline pos.
        iHwBox->RefreshUI();
        DrawGuideLine();
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::OnStrokeStarted
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::OnStrokeStarted()
    {
    iDisableCaseChange = ETrue;
    iHwBox->SetWndTransparencyFactor(iWriteTransFactor);
    
    if( !iCharacterStart )
        {
        iCharacterStart = ETrue;
    
        TBuf<4> buf;
        buf.Append(reinterpret_cast<TText*>(&iCharacterStart), sizeof(TBool)/sizeof(TText));
        iLayoutContext->Sendkey( ESignalCharacterStart, buf );
        
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::OnStrokeFinished
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::OnStrokeFinished()
    {    
    // No trace
    if ( !iHwBox->HasNewTrace() )
        {
        return;
        }
    
    RArray<TPoint> strokes = iHwBox->StrokeList();
    
    iStrokeArray.Reset();
    
    const TInt count = strokes.Count();
    
#ifdef _NOT_USING_DECUMA_MCR_
    // similar as S90         
    
    for ( TInt i = 0; i < count; i++ )
        {
        iStrokeArray.Append( strokes[i] );
        } 
    
    CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
        ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
        iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
    TInt err = penData->iRecognizer->Recognize( iStrokeArray, iRecogResult );
	
    if ( err == KErrNone && iRecogResult.Count() > 0 )
        { 
        SubmitRecognitionResult( iRecogResult[0] );   
        }
#else          

    for ( TInt i = 0; i < count; i++ )
        {
        iStrokeArray.Append( strokes[i] );
        } 
        
#endif

    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::OnStrokeCharacterTimerOut
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::OnStrokeCharacterTimerOut()
    {
    iDisableCaseChange = EFalse;
    DoCaseChange(iCachedCase);
    iHwBox->SetWndTransparencyFactor(iNormalTransFactor);
#ifdef _NOT_USING_DECUMA_MCR_    
    // similar as S90     
    CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
    ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
    iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
   	TInt language = penData->iRecognizer->GetLanguage();
	if( language == ELangThai || language == ELangHindi || language == ELangVietnamese )
		{
        // This is a workarround to clear the recognizer.
    	RArray<TPoint> stroke_array;
    	stroke_array.Reset(); 
    	// Call recognize with a empty array of stroke is the condition
    	// to clear the context>
        TInt err = penData->iRecognizer->Recognize( stroke_array, iRecogResult );    	
        if( err == KErrNone && iRecogResult.Count() > 0 )
            {
            SubmitRecognitionResult( iRecogResult[0] );
            }
		}

	delete iLastResult;
	iLastResult = NULL;    	
    	
#else

    CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
        ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
        iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
    TInt err = penData->iRecognizer->Recognize( iStrokeArray, iRecogResult );
    if ( err == KErrNone && iRecogResult.Count() > 0 )
        {        
        const HBufC* res = iRecogResult[0];
        iLayoutContext->Sendkey( ESignalKeyEvent, *res );
        } 
    iStrokeArray.Reset();    
     
#endif    


    iCharacterStart = EFalse;
    }

// --------------------------------------------------------------------------
// CPeninputGenericHwrWindow::OnStrokeCanceled
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::OnStrokeCanceled()
    {
    iDisableCaseChange = EFalse;
    iHwBox->SetWndTransparencyFactor(iNormalTransFactor);
#ifdef _NOT_USING_DECUMA_MCR_    
    // similar as S90     
    CPeninputGenericHwrDataMgr::THandWritingPenData* penData = 
    ( CPeninputGenericHwrDataMgr::THandWritingPenData* ) 
    iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
    TInt language = penData->iRecognizer->GetLanguage();
    if( language == ELangThai || language == ELangHindi || language == ELangVietnamese )
        {
        // This is a workarround to clear the recognizer.
        RArray<TPoint> stroke_array;
        stroke_array.Reset(); 
        // Call recognize with a empty array of stroke is the condition
        // to clear the context>
        TInt err = penData->iRecognizer->Recognize( stroke_array, iRecogResult );       
//        if( err == KErrNone && iRecogResult.Count() > 0 )
//            {
//            SubmitRecognitionResult( iRecogResult[0] );
//            }
        }

    delete iLastResult;
    iLastResult = NULL;     
        

     
#endif    

    iCharacterStart = EFalse;
    } 

// -----------------------------------------------------------------------------
// CPeninputGenericHwrWindow::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::ConstructFromResourceL()
    {     	        
    CPeninputLayoutWindow::ConstructFromResourceL();
    
    if (iLangSwitchBtn)
        {
    	SetSwitchBtnTextColor(*iLangSwitchBtn);
        }
        
    SetHwBoxFrameBackColor();    
    
    TRgb* iPenColor =
                    ( TRgb* ) iLayoutContext->RequestData(EPeninputDataTypePenTailColor);
    if (iHwBox)
        {
        iHwBox->SetPenColor( *iPenColor);
        }
        
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrWindow::PopupSwitchWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::PopupSwitchWindow()
    {
    CFepUiBaseCtrl* modeSwitchBtn = Control(EPeninutWindowCtrlIdSwitchToVkbBtn);

    if ( modeSwitchBtn )
        {
        TRect rect = modeSwitchBtn->Rect();   
        TRAP_IGNORE(iModeSwitchChoice->PopUpSwitchListL(rect));
        }      
    }
// ---------------------------------------------------------------------------
// CPeninputGenericHwrWindow::OnDeActivate
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputGenericHwrWindow::OnDeActivate()
    {
    //reset range
    TInt range = -1;
    iLayoutContext->SetData( EPeninputDataTypeCurrentRange, &range );
    CPeninputLayoutWindow::OnDeActivate();
    }


TBool CPeninputGenericHwrWindow::IsMultiLineIcf()
	{
	return ETrue;	
	}
	
TBool CPeninputGenericHwrWindow::IsCanChangeRange(TInt aRange)
	{
	if (aRange == ERangeSymbol)
		{
		UiLayout()->SignalOwner(ESignalLaunchSCT);
		
		return EFalse;	
		}
		
	return ETrue;
	}  
	 
void CPeninputGenericHwrWindow::ChangeToPreviousRange(TInt aRange)
	{
	if(iRangeBar)
		{
		iRangeBar->ActiveRange(aRange);	
		}
	}
                                                                                                              
inline TBool IsLanguageBidi( TInt aLanguage )
    {
    return aLanguage == ELangArabic || aLanguage == ELangFarsi ||
           aLanguage == ELangHebrew ;    
    }
void CPeninputGenericHwrWindow::OnLanguageChange()
    {
    TBool bidi = IsLanguageBidi( ConfigInfo()->Language() );
    iHwBox->SetEnableFade( !bidi );                                           
    }

void CPeninputGenericHwrWindow::SubmitRecognitionResult(HBufC* aRes )
    {
    if( !iLastResult )
        {
        iLayoutContext->Sendkey( ESignalKeyEvent, *aRes );
        }
    else
        {
        if( iLastResult->Compare( *aRes ) != 0 )
            {
            //replace
            if ( ( *aRes ).Locate( EKeyBackspace ) != KErrNotFound )
                {
                TBuf<1> buf;
                buf.Append( EKeyBackspace );
                iLayoutContext->Sendkey( ESignalKeyEvent, buf );
                }
            else
                {
                HBufC* p = HBufC::New(aRes->Length() + 2);
                if( p )
                    {
                    TInt len = iLastResult->Length();
                    p->Des().Append((TUint16*)&len, 2);
                    p->Des().Append(*aRes);
                    iLayoutContext->Sendkey( ESignalReplaceText, *p );
                    delete p;
                    }
                }
            }
        }
        
    delete iLastResult;
    iLastResult = NULL;
    
    if ( (*aRes).Locate( EKeyBackspace ) != KErrNotFound  ||
         aRes->Length() >= 50 )
        {
        iHwBox->CancelCharWriting();
        }
    else
        {
        iLastResult = aRes->Alloc();
        }
    }
void CPeninputGenericHwrWindow::Move(const TPoint& aOffset)
    {
    CPeninputLayoutWindow::Move(aOffset);
    DrawGuideLine();
    }
