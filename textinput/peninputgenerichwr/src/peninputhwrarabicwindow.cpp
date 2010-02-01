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
#include <peninputdropdownlist.h>
#include <AknIconUtils.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknsUtils.h>
#include <peninputlayoutchoicelist.h>
#include <AknFepGlobalEnums.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputlayoutmultilineicf.h>
#include <coemain.h>
#include <peninputdragbutton.h>
#include <peninputrepeatbutton.h>
#include <peninputdataconverter.h>
#include <peninputpluginutils.h>

// User includes
#include "peninputlayoutchoicelist.h"
#include "peninputhwrarabicwindow.h"
#include "peninputhwrarabicdatamgr.h"
#include "peninputhwrarabic.hrh"
#include "peninputhwrarabicengine.h"
//#include "peninputhwrevent.h"
#include "peninputhwrarabiclayout.h"
// Constants
_LIT( KHwrWindowResourceFile, 
      "z:\\resource\\plugins\\peninputhwrwindow.RSC" );
_LIT( KConfigurationResourceFile, 
      "z:\\resource\\plugins\\peninputhwrwindowconfiginfo_" );
_LIT( KResourceFileExtName, ".RSC" );

_LIT(KStandardPuncExclamatoryPoint, "\x0021");
_LIT(KStandardPuncDotPoint, "\x002E");
_LIT(KArabicPuncComma, "\x060C");
_LIT(KArabicPuncSemicolon, "\x061B");
_LIT(KArabicPuncQuestion, "\x061F");
_LIT(KArabicPuncWaw, "\x0648");

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
// CPeninputHwrBxAbWnd::CPeninputHwrBxAbWnd
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBxAbWnd::CPeninputHwrBxAbWnd( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    : CPeninputLayoutWindow( aUiLayout, aLayoutContext ),
      iNormalTransFactor(KNormalFactor),
      iWriteTransFactor(KWrittingFactor),
      iGuideLineSet(EFalse),
      iBackspaceMirrored(EFalse)
    {
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBxAbWnd* CPeninputHwrBxAbWnd::NewL( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    {
    CPeninputHwrBxAbWnd* self = 
        new ( ELeave ) CPeninputHwrBxAbWnd( aUiLayout, aLayoutContext );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::~CPeninputHwrBxAbWnd
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBxAbWnd::~CPeninputHwrBxAbWnd()
    {
    for ( TInt i = 0; i < iRecogResult.Count(); i++ )
        {
        delete iRecogResult[i];
        }
    iRecogResult.Close();
    iStrokeArray.Close();
    delete iLastResult;
    delete iBmpRotator;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::ConstructL()
    {
    iBmpRotator = CPeninputSyncBitmapRotator::NewL();    
    CPeninputLayoutWindow::ConstructL();   
    CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
        ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
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
// CPeninputHwrBxAbWnd::HandleControlEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::HandleControlEvent( TInt aEventType,
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
            CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
                ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
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
            CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
                ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
                iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );

            iHwBox->HideGuideLine( penData->iGuideLineOn == 0 ? EFalse : ETrue );
            iHwBox->RefreshUI();
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
            else
                {// handle dropdownlist when user tapped switch inputmethods popup window without selecting other inputmethods.
                TInt dataType = CPeninputDataConverter::AnyToInt(iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ));
                if (dataType == ERangeNative)
                    {
                    ResetAndShowDropdownList();
                    }
                else
                    {
                    ClearAndCloseDropdownList();
                    }
                }
            }
            break;
        case EPeninputLayoutEventToVkb:
            {
            iHwBox->CancelCharWriting();
            TInt curRange = CPeninputDataConverter::AnyToInt(iLayoutContext->RequestData(EPeninputDataTypeCurrentRange) );
            if (curRange == ERangeNative)
                {
                iDropdownList->ResetAndClear(CFepCtrlDropdownList::EListExpandableMultiRowWithoutIcon );
                }
            PopupSwitchWindow();
            }
            break; 
        case EPeninputLayoutEventRange:
            {
            iHwBox->CancelCharWriting();
            ClearAndCloseDropdownList();
            SizeChanged(ETrue);
            }
            break;
        case EPeninputLayoutEventBack:
            {
            HandleBackspaceKeyEvent();
            }
            break;
            
        default:
            break;
        }
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::GetWindowConfigResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputHwrBxAbWnd::GetWindowConfigResId()
    {
    return R_PENINPUT_LAYOUT_CONFIG_INFO;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::GetWindowResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputHwrBxAbWnd::GetWindowResId()
    {
    return R_PENINPUT_LAYOUT_HWR_WINDOW;        
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::GetWindowConfigResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TDesC& CPeninputHwrBxAbWnd::GetWindowConfigResFileName( 
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
// CPeninputHwrBxAbWnd::GetWindowResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TDesC& CPeninputHwrBxAbWnd::GetWindowResFileName()
    {
    return KHwrWindowResourceFile();
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::CreateAllControlsL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::CreateAllControlsL()
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
    AddControlL( iModeSwitchChoice );    
    iModeSwitchChoice->AddEventObserver( UiLayout() );             


    //add dropdown list to client pane.
    TPixelsTwipsAndRotation ptSize;     
    CCoeEnv* coeEnv = CCoeEnv::Static();    
    coeEnv->ScreenDevice()->GetDefaultScreenSizeAndRotation(ptSize);

    TBool boxWndLandscape;
    
    //set proper screen style for box window
    if (ptSize.iPixelSize.iWidth < ptSize.iPixelSize.iHeight)
        {
        // protrait screen
        boxWndLandscape = EFalse;
        }
    else
        {
        // landscape screen
        boxWndLandscape = ETrue;
        }
    AddDropdownListL(boxWndLandscape);
//    TRect rect = iHwBox->Rect();
//    rect.iTl.iY = rect.iTl.iY + 2*iUnitHeight;
//    
//    iHwBox->SetRect(rect);
    for ( TInt i = 0; i < iCtrlPool->ControlCount(); i++ )
        {
        if ( iCtrlPool && iCtrlPool->ControlByIndex( i ))
            {
            iCtrlPool->ControlByIndex( i )->AddEventObserver( UiLayout() );
            }
        }
    }
        
// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::ChangeUnitSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TRect CPeninputHwrBxAbWnd::ChangeUnitSize()
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
// CPeninputHwrBxAbWnd::ChangeClientSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::ChangeClientSize()
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
    //mirror backspace arrow
    TRAP_IGNORE(MirrorBackspaceButtonResL());    
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::ReorganizeControls
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::ReorganizeControls( TInt aClientLayoutId, TBool /*aNeedReset*/ )
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
    CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
        ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
        iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
    penData->iRecognizer->SetCaseSensitive( ConfigInfo()->CaseSensitive() );

    
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
                TInt language = penData->iRecognizer->GetLanguage();
                TInt curRange = CPeninputDataConverter::AnyToInt( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
                if (language == ELangArabic && curRange
                                > ERangeInvalid)
                            {
                            if (language == ELangArabic && curRange
                                    == ERangeNative)
                                {
                                iHwBox->SetRect(rect);
                                DrawGuideLine();
                                }
                            else
                                {
                                //recalculate rect size to include dropdown list area.
                                rect.iTl = point + TPoint(
                                        controlList[i]->BeginColumn()
                                                * unitWidth,
                                        (controlList[i]->BeginRow() - 2)
                                                * unitHeight);
                                cols = controlList[i]->EndColumn()
                                        - controlList[i]->BeginColumn();
                                rows = controlList[i]->EndRow()
                                        - controlList[i]->BeginRow() + 2;
                                rect.SetSize(TSize(cols * unitWidth, rows
                                        * unitHeight) );
                                iHwBox->SetRect(rect);
                                DrawGuideLine();
                                }
                            }
                        else
                            {
                            iHwBox->SetRect(rect);
                            DrawGuideLine();
                            }
                }
            else if(ctrl->ControlId() == EPentinputHwrCandidateList)
                {
                TInt language = penData->iRecognizer->GetLanguage();
                TInt curRange = CPeninputDataConverter::AnyToInt( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
                if(language == ELangArabic && curRange > ERangeInvalid)
                    {
                if(language == ELangArabic && curRange == ERangeNative)
                    {
                    iDropdownList->SetRect(rect);
                    iDropdownList->Hide(EFalse);
                    }
                else
                    {
                    iDropdownList->SetRect(TRect(0,0,0,0));
                    }
                    }
                else
                    {
                    iDropdownList->SetRect(rect);
                    iDropdownList->Hide(EFalse);                    
                    }
                }
            }
        }   
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::CalculateGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::CalculateGuideLinePos()
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
// CPeninputHwrBxAbWnd::DrawGuideLine
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::DrawGuideLine()
    {
    // Set guide line
    if ( ConfigInfo() )
        {
        TInt style = ConfigInfo()->GuideLine();
        
        CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
    ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
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
// CPeninputHwrBxAbWnd::SetControlsFont
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::SetControlsFont()
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
// CPeninputHwrBxAbWnd::AddRangeBarL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::AddRangeBarL()
    {
    // Range bar
    iRangeBar = CAknFepCtrlRangeBar::NewL( UiLayout(), 
        EPeninutWindowCtrlIdRangeBar, 
        CAknFepCtrlRangeBar::EBtnGroupVertical );
          
    // Set event id
    iRangeBar->SetEventIdForRange( EPeninputLayoutEventRange );
    iRangeBar->SetEventIdForCase( EPeninputLayoutEventRangeLoop );
//    ClientPane()->AddControlL(iRangeBar);
//    iRangeBar->AddEventObserver(UiLayout());      
    if (iCtrlPool)
        {
        iCtrlPool->AddControl(iRangeBar);
        }
    }
        
// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::AddButtonL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputHwrBxAbWnd::AddButtonL( 
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
void CPeninputHwrBxAbWnd::AddBackspaceButtonL()
    {
    iBackspaceButton = CAknFepCtrlRepeatButton::NewL( 
                                      UiLayout(), EPeninutWindowCtrlIdBackspaceBtn,
                                      EPeninputLayoutEventBack, EKeyBackspace, 
                                      KAknsIIDQsnFrFunctionButtonNormal,
                                      KAknsIIDQsnFrFunctionButtonPressed,
                                      KAknsIIDQsnFrFunctionButtonInactive );
//    iBackspaceButton->AddEventObserver( UiLayout() );
    // Read resource
    TResourceReader reader;
    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_LAYOUT_HWR_BACKSPACE );
    iBackspaceButton->SetResourceId( R_PENINPUT_LAYOUT_HWR_BACKSPACE );
    iBackspaceButton->ConstructFromResourceL();
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );    
    }
// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::AddHwBoxL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::AddHwBoxL()
    {
    CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
        ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
        iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
    iHwBox = CTransparentHwrWndExt::NewL( Rect(), UiLayout(), 
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
// ---------------------------------------------------------
// Add dropdown list to window
// ---------------------------------------------------------
//
void CPeninputHwrBxAbWnd::AddDropdownListL(TBool aLandscapeStyle)
    {   
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader,R_PENINPUT_LAYOUT_DROP_DOWN_LIST);
    
//    iSecondRangeColor = TRgb(0xFF0000);

    ReadLafInfo(aLandscapeStyle);
    TAknTextLineLayout candTextLayout = 
                      AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1(0).LayoutLine();

    TInt textMargin = (iUnitHeight - 
                      AknLayoutUtils::FontFromId(candTextLayout.iFont, NULL)->HeightInPixels())/2;

    TDropdownListDrawInfo candiDrawInfo(KAknsIIDQsnFrFunctionButtonInactive, 
                                        KAknsIIDQsnFrFunctionButtonNormal,
                                        KAknsIIDQsnFrFunctionButtonPressed,
                                        KAknsIIDQsnFrItutButtonCandiSideL,
                                        KAknsIIDQsnFrItutButtonCandiMiddle,
                                        KAknsIIDQsnFrItutButtonCandiSideR,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideL,
                                        KAknsIIDQsnFrItutButtonCandiPressedMiddle,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideR,
                                        TRgb(194, 221, 242),
                                        ETrue);
    CFont* listfont = (CFont *)AknLayoutUtils::FontFromId(candTextLayout.iFont, NULL);
    
    iDropdownList = CFepCtrlDropdownList::NewL(UiLayout(),
                                               EPentinputHwrCandidateList,
                                               R_PENINPUT_LAYOUT_DROP_DOWN_LIST,                                            
                                               TPoint(Rect().iTl.iX + 3, Rect().iTl.iY + 3 + iUnitHeight),                                               
                                               listfont ,
                                               TSize(iUnitWidth,iUnitHeight),
                                               iUnitWidth,
                                               6,2,
                                               iUnitHeight,
                                               0,
                                               0,
                                               CFepCtrlDropdownList::ECandFromRToL,
                                               EFalse);
    if ( iCtrlPool )
        {
        iCtrlPool->AddControl( iDropdownList );       
        }
    iDropdownList->SetFont(listfont);    
    iDropdownList->SetEventIdForCandidateSelected(EPeninputLayoutHwrEventCandidateSelected);
    //iDropdownList->SetEventIdForNextPageCandidate(EPeninputLayoutHwrEventGetNextCandidatePage);
    //iDropdownList->SetEventIdForPreviousPageCandidate(EPeninputLayoutHwrEventGetPreviousCandidatePage);
    iDropdownList->SetEventIdForCandidateExisted(EPeninputLayoutHwrEventCandidateExisted);
    iDropdownList->SetCandTruncateType(CFepCtrlDropdownList::ECandTruncateFromBeginning);    
    iDropdownList->SetDropdownListImgID(candiDrawInfo);
    
    CleanupStack::PopAndDestroy(); // reader  
    }       
    // ---------------------------------------------------------
// CAknFepHwrBxWnd::ReadUnitSize
// ---------------------------------------------------------
//
void CPeninputHwrBxAbWnd::ReadLafInfo(TBool aLandscapeStyle)
	{
	//if (iDataMgr->SizeChanging())// || (!iUnitShadowSizeSet))
		{
        // if size is changing, or unit size, shadow size have not been set yet
        TAknWindowLineLayout appWnd;
        TAknWindowLineLayout mainPane;
        TAknWindowLineLayout wndLayout;
        TAknWindowLineLayout unitLayout;
        TAknWindowLineLayout shadowPane;
        TAknWindowLineLayout shadowTl;
        TAknWindowLineLayout shadowBr;
        TAknLayoutRect mainPaneRect;
        TAknLayoutRect wndRect;
        TAknLayoutRect unitRect;
        TAknLayoutRect shadowRect;
        TAknLayoutRect shadowTlRect;
        TAknLayoutRect shadowBrRect;

        // Get hwr window layout
        appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
        
        if (aLandscapeStyle)
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
    
        TRect hwrWndRect = TRect(0, 
                                 0,
                                 wndRect.Rect().Width(),
                                 wndRect.Rect().Height());
                             
        iUnitWidth = unitRect.Rect().Width();
        iUnitHeight = unitRect.Rect().Height();

        shadowPane = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane(2).LayoutLine();
        shadowRect.LayoutRect(hwrWndRect, shadowPane);
    
        shadowTl = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g1().LayoutLine();
        shadowTlRect.LayoutRect(shadowRect.Rect(), shadowTl);
    
        shadowBr = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g8().LayoutLine();
        shadowBrRect.LayoutRect(shadowRect.Rect(), shadowBr);
    
        iShadowTl = shadowTlRect.Rect().Size();
        iShadowBr = shadowBrRect.Rect().Size();
        
        //iUnitShadowSizeSet = ETrue;
		}
 	}
 
// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::SetHwBoxFrameBackColor
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::SetHwBoxFrameBackColor()
    {
    TRgb frameColor;
    TRgb backColor;
    TAknsItemID id;
    
    id.Set(KWriteBoxFrameBackColorMajor, KWriteBoxFrameBackColorGrp);

    TInt error = AknsUtils::GetCachedColor(AknsUtils::SkinInstance(),
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
// CPeninputHwrBxAbWnd::PopupChoiceList
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::PopupChoiceList()
    {
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::DoCaseChange
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::DoCaseChange( TInt aNewCase )
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
// CPeninputHwrBxAbWnd::OnStrokeStarted
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::OnStrokeStarted()
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
// CPeninputHwrBxAbWnd::OnStrokeFinished
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::OnStrokeFinished()
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
    
    CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
        ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
        iLayoutContext->RequestData( EPeninputDataTypeReserve_1 );
    
    TInt err = penData->iRecognizer->Recognize( iStrokeArray, iRecogResult );
    
    if ( err == KErrNone && iRecogResult.Count() > 0 )
        {
        TInt dataType = CPeninputDataConverter::AnyToInt(iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ));
        if (dataType == ERangeNative)
            {
            if( iRecogResult[0]->Locate( EKeyBackspace ) == KErrNotFound 
              && iRecogResult[0]->Locate( EKeyFormFeed ) == KErrNotFound
              && iRecogResult[0]->Locate( EKeySpace ) == KErrNotFound 
              && iRecogResult[0]->Locate( EKeyEnter ) == KErrNotFound
              && iRecogResult[0]->Locate(EKeyDelete) == KErrNotFound)
                {
                for (TInt i = 1; i < iRecogResult.Count(); i++)
                    {
                    if((iRecogResult[i]->Locate( EKeyBackspace ) != KErrNotFound)
                            ||(iRecogResult[i]->Locate( EKeySpace ) != KErrNotFound)
                            ||(iRecogResult[i]->Locate( EKeyLineFeed ) != KErrNotFound)
                            ||(iRecogResult[i]->Locate( EKeyEnter ) != KErrNotFound))
                        {
                        delete iRecogResult[i];
                        iRecogResult.Remove(i);
                        }
                    }
                iDropdownList->Hide(EFalse);
                TRAP_IGNORE(iDropdownList->SetCandidatesL(iRecogResult,
                        CFepCtrlDropdownList::EListExpandableMultiRowWithoutIcon ));
                iDropdownList->SetHighlightCell(0);
                }
            else
                {
                ResetAndShowDropdownList();                
                }
            }
        else
            {
            ClearAndCloseDropdownList();
            }
                
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
// CPeninputHwrBxAbWnd::OnStrokeCharacterTimerOut
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::OnStrokeCharacterTimerOut()
    {
    iDisableCaseChange = EFalse;
    DoCaseChange(iCachedCase);
    iHwBox->SetWndTransparencyFactor(iNormalTransFactor);
#ifdef _NOT_USING_DECUMA_MCR_    
    // similar as S90     
    CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
    ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
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
            TInt dataType = CPeninputDataConverter::AnyToInt(iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ));
            if (dataType == ERangeNative)
                {
                iDropdownList->Hide(EFalse);
                TRAP_IGNORE(iDropdownList->SetCandidatesL(iRecogResult, CFepCtrlDropdownList::EListExpandableMultiRowWithoutIcon ));
                }
            else
                {
                ClearAndCloseDropdownList();
                }
            SubmitRecognitionResult( iRecogResult[0] );
            }
        }

    delete iLastResult;
    iLastResult = NULL;     
        
#else

    CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
        ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
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
// CPeninputHwrBxAbWnd::OnStrokeCanceled
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::OnStrokeCanceled()
    {
    iDisableCaseChange = EFalse;
    iHwBox->SetWndTransparencyFactor(iNormalTransFactor);
#ifdef _NOT_USING_DECUMA_MCR_    
    // similar as S90     
    CPeninputHwrBxAbDataMgr::THandWritingPenData* penData = 
    ( CPeninputHwrBxAbDataMgr::THandWritingPenData* ) 
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
// CPeninputHwrBxAbWnd::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::ConstructFromResourceL()
    {               
    CPeninputLayoutWindow::ConstructFromResourceL();
    
    if (iLangSwitchBtn)
        {
        SetSwitchBtnTextColor(*iLangSwitchBtn);
        } 
    iBackspaceMirrored = EFalse;
    if(!iBackspaceButton)
        {
        AddBackspaceButtonL();
        // Add into the group   
        CControlGroup* headerPane = HeaderPane();            
        headerPane->AddControlL(iBackspaceButton);
        iBackspaceButton->AddEventObserver(UiLayout());            
        }        
    SetHwBoxFrameBackColor();    
    
    TRgb* penColor =
                    ( TRgb* ) iLayoutContext->RequestData(EPeninputDataTypePenTailColor);
    if (iHwBox)
        {
        iHwBox->SetPenColor( *penColor);
        }  
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::PopupSwitchWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::PopupSwitchWindow()
    {
    CFepUiBaseCtrl* modeSwitchBtn = Control(EPeninutWindowCtrlIdSwitchToVkbBtn);

    if ( modeSwitchBtn )
        {
        TRect rect = modeSwitchBtn->Rect();   
        TRAP_IGNORE(iModeSwitchChoice->PopUpSwitchListL(rect));
        }      
    }
// ---------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::OnDeActivate
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBxAbWnd::OnDeActivate()
    {
    //reset range
    TInt range = -1;
    iLayoutContext->SetData( EPeninputDataTypeCurrentRange, &range );
    CPeninputLayoutWindow::OnDeActivate();
    iDropdownList->ResetAndClear(CFepCtrlDropdownList::EListExpandableMultiRowWithoutIcon );
    iDropdownList->Hide(ETrue);
    }


TBool CPeninputHwrBxAbWnd::IsMultiLineIcf()
    {
    return ETrue;   
    }
    
TBool CPeninputHwrBxAbWnd::IsCanChangeRange(TInt aRange)
    {
    if (aRange == ERangeSymbol)
        {
        UiLayout()->SignalOwner(ESignalLaunchSCT);
        
        return EFalse;  
        }
        
    return ETrue;
    }  
     
void CPeninputHwrBxAbWnd::ChangeToPreviousRange(TInt aRange)
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
void CPeninputHwrBxAbWnd::OnLanguageChange()
    {
    TBool bidi = IsLanguageBidi( ConfigInfo()->Language() );
    iHwBox->SetEnableFade( !bidi );                                           
    }

TBool CPeninputHwrBxAbWnd::IsAddingSpace(HBufC* aRes)
    {
    TInt curRange = CPeninputDataConverter::AnyToInt(iLayoutContext->RequestData(EPeninputDataTypeCurrentRange) );
    if (curRange != ERangeNative)
        {
        return EFalse;
        }
    if (aRes->Des().Length() == 1)
        {
        if ( ( *aRes ).Compare(KStandardPuncDotPoint) == 0 || 
             ( *aRes ).Compare(KStandardPuncExclamatoryPoint) == 0 || 
             ( *aRes ).Compare(KArabicPuncComma) == 0 || 
             ( *aRes ).Compare(KArabicPuncSemicolon) == 0 || 
             ( *aRes ).Compare(KArabicPuncQuestion) == 0 ||
             ( *aRes ).Compare(KArabicPuncWaw) == 0)
            {
            return ETrue;
            }
        }
    else if (aRes->Des().Length() > 1)
        {
        if ( ( *aRes ).Locate(EKeyBackspace) == KErrNotFound &&
             ( *aRes ).Locate(EKeyFormFeed) == KErrNotFound &&
             ( *aRes ).Locate(EKeySpace) == KErrNotFound &&
             ( *aRes ).Locate(EKeyEnter) == KErrNotFound &&
             ( *aRes ).Locate(EKeyDelete) == KErrNotFound )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

void CPeninputHwrBxAbWnd::SubmitRecognitionResult(HBufC* aRes )
    {
    _LIT(KSpaceChar, " ");
    TBuf<1> buf;
    buf.Zero();
    buf.Append(KSpaceChar); 
    
    if( !iLastResult )
        {
        if (!IsAddingSpace(aRes))
            {
            iLayoutContext->Sendkey(ESignalKeyEvent, *aRes);
            }
        else
            {
            HBufC* p = HBufC::New(aRes->Length() + buf.Length());
            p->Des().Copy(*aRes);
            p->Des().Append(buf);
            iLayoutContext->Sendkey(ESignalKeyEvent, *p);
            delete p;
            }
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
                HBufC* p = NULL;
                if (IsAddingSpace(aRes))
                    {
                    p = HBufC::New(aRes->Length() + 2 + buf.Length());
                    }
                else
                    {
                    p = HBufC::New(aRes->Length() + 2);
                    }
                if( p )
                    {
                    TInt len = iLastResult->Length();
                    if (IsAddingSpace(aRes))
                        {
                        len += buf.Length();
                        }
                    p->Des().Append((TUint16*)&len, 2);
                    p->Des().Append(*aRes);
                    if (IsAddingSpace(aRes))
                        {
                        p->Des().Append(buf);
                        }                        
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
void CPeninputHwrBxAbWnd::Move(const TPoint& aOffset)
    {
    CPeninputLayoutWindow::Move(aOffset);
    DrawGuideLine();
    }

// ---------------------------------------------------------
// Get the candidate
// ---------------------------------------------------------
//
void CPeninputHwrBxAbWnd::GetCandidate(TInt aIndex, TDes& aCharCode) const
    {
    aCharCode.Zero();
    if (aIndex >=0 && aIndex < iRecogResult.Count())
        {
        aCharCode = *iRecogResult[aIndex];
        }
    }

// ---------------------------------------------------------
// Cancel writing
// ---------------------------------------------------------
//
void CPeninputHwrBxAbWnd::CancelWriting()
    {
    if( !iHwBox )
        {
        //there is nothing to cancel
        return;
        }
    //iHwBox->SetWndTransparencyFactor(iInactiveTrans);
    iHwBox->CancelCharWriting();
    iDropdownList->ResetAndClear(CFepCtrlDropdownList::EListExpandableMultiRowWithoutIcon );
    iDropdownList->Hide(ETrue);
    }

void CPeninputHwrBxAbWnd::ClearAndCloseDropdownList()
    {
    iDropdownList->ResetAndClear(CFepCtrlDropdownList::EListExpandableMultiRowWithoutIcon );
    iDropdownList->Hide(ETrue);
    }
void CPeninputHwrBxAbWnd::ResetAndShowDropdownList()
    {
    iDropdownList->ResetAndClear(CFepCtrlDropdownList::EListExpandableMultiRowWithoutIcon );
    iDropdownList->Hide(EFalse);
    }
void CPeninputHwrBxAbWnd::MirrorBackspaceButtonResL()
    {
    if(iBackspaceMirrored)
        return;
    CAknFepCtrlCommonButton* ctrl = static_cast<CAknFepCtrlCommonButton*>(iBackspaceButton);
    if( ctrl && iBmpRotator)
        {
        TSyncRotationAngle rotChoice = CBitmapRotator::EMirrorVerticalAxis; 
        User::LeaveIfError( iBmpRotator->Rotate( *ctrl->ForgroundBmp() ,
                                                 rotChoice ) );
        User::LeaveIfError( iBmpRotator->Rotate( *ctrl->ForgroundBmpMask() ,
                                                 rotChoice ) );
        iBackspaceMirrored = ETrue;
        }       
    }
void CPeninputHwrBxAbWnd::HandleBackspaceKeyEvent()
    {
    TBuf<1> buf;
    buf.Append( EKeyBackspace );
    iLayoutContext->Sendkey( ESignalKeyEvent, buf );      
    }
