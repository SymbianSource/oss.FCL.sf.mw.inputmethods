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
* Description:  Implementation of vkb window class
*
*/


// System includes


#include <peninputvkbwindow.rsg>
#include <peninputvkbwindowconfiginfo_01.rsg>
#include <aknfeppeninputenums.h>
#include <peninputinputcontextfield.h>
#include <peninputpluginutils.h>
#include <peninputcommonlayoutext.h>
#include <peninputvkbctrllayout.h>
#include <peninputvkbctrlext.h>
#include <peninputeventbutton.h>
#include <peninputdragbutton.h>
#include <peninputrepeatbutton.h>
#include <peninputmultimodebutton.h>
#include <peninputrangebar.h>
#include <peninputdataconverter.h>
#include <peninputdataprovider.h>
#include <peninputclientlayoutinfo.h>
#include <peninputlayoutchoicelist.h>
#include <peninputlongpressbutton.h>
#include <bautils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputscrollablelist.h>

#include <e32keys.h>
#include <AknIconUtils.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknsUtils.h>
#include <AknFepGlobalEnums.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputlayoutmultilineicf.h>
#include <s32mem.h>
// User includes
#include "peninputgenericvkb.hrh"
#include "peninputgenericvkbwindow.h"
#include "peninputgenericvkbenum.h"
#include "peninputrangebarinfo.h"
#include "peninputgenericvkblafdatamgr.h"
#include "peninputgenericvkblangmeritpair.h"
#include "peninputgenericvkblayout.h"

// Constants
const TInt KPeninputVkbWndInvalidIndex = -1;
const TInt KInvalidImg = -1 ;
const TInt KKeyCodeSize = 1;

const TInt KTooltipLeftMargin = 10;
const TInt KTooltipTopMargin = 0;
const TInt KTooltipRightMargin = 10;
const TInt KTooltipBottomMargin = 0;

_LIT( KVkbWindowResourceFile, 
      "z:\\resource\\plugins\\peninputvkbwindow.RSC" );
_LIT( KConfigurationResourceFile, 
      "z:\\resource\\plugins\\peninputvkbwindowconfiginfo_" );
_LIT( KResourceFileExtName, ".RSC" );

_LIT(KEmptyString, "");

// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::CPeninputGenericVkbWindow
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericVkbWindow::CPeninputGenericVkbWindow( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext, 
    MPeninputUiStateMgr* aUiStateMgr )
    : CPeninputLayoutWindowExt( aUiLayout, aLayoutContext ),
      iUiStateMgr(aUiStateMgr),
      iPopupInited(EFalse),
      iPopupSet(EFalse),
      iLafMgr(NULL),
      iFirstConstruct(ETrue),
      iIndiWithText( EFalse ),
      iUSRscFileExist( EFalse )
    {
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
// 
CPeninputGenericVkbWindow* CPeninputGenericVkbWindow::NewL( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext, 
    MPeninputUiStateMgr* aUiStateMgr )
    {
    CPeninputGenericVkbWindow* self = 
        new ( ELeave ) CPeninputGenericVkbWindow( aUiLayout, aLayoutContext, aUiStateMgr );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::~CPeninputGenericVkbWindow
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericVkbWindow::~CPeninputGenericVkbWindow()
    {
    delete iIdle;
    iUnits.Close();
    
    delete iLafMgr;
    iLafMgr = NULL;
    delete iBmpRotator;
    iAccentCmdList.Close();
    }
void CPeninputGenericVkbWindow::ConstructL()
    {
    iHandleRangeShortPress = ETrue;
    iBmpRotator = CPeninputSyncBitmapRotator::NewL();
    iLafMgr = CPeninputGenericVkbLafMgr::NewL();
    iLafMgr->SetInputModeL(TPluginInputMode (iLayoutContext->LayoutType()));
    CPeninputLayoutWindowExt::ConstructL();

    // Save whether American rsc file exist or not
    TBuf<KMaxFileLength>  usUSRscFileName; 
    usUSRscFileName= KConfigurationResourceFile();
    usUSRscFileName.AppendNum( ELangAmerican );
    usUSRscFileName.Append( KResourceFileExtName );

    RFs fileSession;
    User::LeaveIfError( fileSession.Connect() );
    CleanupClosePushL( fileSession );
    if ( BaflUtils::FileExists( fileSession , usUSRscFileName ))
    	{
        iUSRscFileExist = ETrue;
    	}
    CleanupStack::PopAndDestroy( 1 );
    // For addition of ITI features, add a candidate list in vkb window   
    CreateCandidateListL();    
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::HandleControlEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::HandleControlEvent( TInt aEventType,
                                                    CFepUiBaseCtrl* aCtrl, 
                                                    const TDesC& aEventData )
    {
    // Call this function in base class
    CPeninputLayoutWindowExt::HandleControlEvent( aEventType, aCtrl, 
                                                  aEventData );
    
    TInt* data = ( TInt* ) aEventData.Ptr(); 
    
    switch ( aEventType )
        {
        case EPeninputLayoutEventMultiRange:
            {
            if ( aCtrl->ControlId() == EPeninutWindowCtrlIdMultiRangeBtn )
                {
                if ( iHandleRangeShortPress )
                    {
                    HandleRangeButtonShortPress();
                    }
                }
                iHandleRangeShortPress = ETrue;
            }
            break;
        case EPeninputLayoutEventMultiRangeLongPress:
            {
            if ( aCtrl->ControlId() == EPeninutWindowCtrlIdMultiRangeBtn )
                {
                // Before pop up list, cancel down event, and set range button un-highlight.
                TRAP_IGNORE( aCtrl->CancelPointerDownL());
                static_cast<CAknFepCtrlCommonButton*>(aCtrl)->SetHighlight( EFalse );
                PopupChoiceList();                
                }
            }
            break;
        case EPeninputLayoutVkbEventResetShift: 
            {
            HandleShiftAndCapslockBtnClicked();
            }
            break;  
        case EPenInputLayoutEventClosePopup:
        	{
            if ( iPopupWnd && !iPopupWnd->Hiden() )
                {
                iPopupWnd->CloseWindow();
                if ( iLayoutContext->LayoutType() != EPluginInputModeFSQ )
                    {
                    ResetAccentButton();
                    }
                iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
                }
        	}
            break;
        case EEventChoiceSelected:
            {
            CFepUiBaseCtrl* btn = Control( EPeninutWindowCtrlIdMultiRangeBtn );
            
            if ( btn )
                {
                iHandleRangeShortPress = EFalse;
                TRAP_IGNORE( btn->HandlePointerUpEventL( btn->Rect().iBr ) );
                iHandleRangeShortPress = ETrue;
                }
            CFepLayoutChoiceList::SEvent* event = 
                ( CFepLayoutChoiceList::SEvent* ) aEventData.Ptr();

            if ( event->iIndex != KPeninputVkbWndInvalidIndex )
                {
                if (aCtrl->ControlId() == EPeninutWindowCtrlIdAccentPopupWindow)
                    {
                    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
                        {
                        TRAP_IGNORE( HandleRangeListSelectedL( aCtrl, aEventData ));
                        }
                    else
                        {
                        TInt currentRange = CPeninputDataConverter::AnyToInt(
                                          iLayoutContext->RequestData(EPeninputDataTypeCurrentRange));
                        TInt currentAccent = CPeninputDataConverter::AnyToInt(
                                          iLayoutContext->RequestData(EPeninputDataTypeCurrentAccent));
                    
                        if ((currentRange == ERangeAccent) && (currentAccent == event->iIndex))
                            {
                            iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
                            return;
                            }
    
                        iLayoutContext->SetData( EPeninputDataTypeCurrentAccent, &(event->iIndex));
                                             
                        // Active range, update rangebar and vkb layout
                        //ChangeRange( ERangeAccent, EPeninputVkbLayoutAccented1 + 2 * event->iIndex);
                        (currentRange == ERangeAccent) ? ChangeRange(ERangeAccent, 
                                           EPeninputVkbLayoutAccented1 + 2 * event->iIndex, EFalse) :
                                                         ChangeRange(ERangeAccent, 
                                           EPeninputVkbLayoutAccented1 + 2 * event->iIndex, ETrue);
                        }
                    }
                else if (aCtrl->ControlId() == EPeninutWindowCtrlIdSwitcherPopupWindow)
                    {
                    if (iLayoutContext->LayoutType() != event->iCommand)
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
                else if ( aCtrl->ControlId() == EPentinputHwrCandidateList )
                    {
                    TRAP_IGNORE( HandleCandidateListSelectedL( aCtrl, aEventData ) );
                    }
                }
            else
                {
                if ( iLayoutContext->LayoutType() != EPluginInputModeFSQ )
                    {
                    ResetAccentButton();
                    }
                }
            iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
            }
            break;
        case EPeninputLayoutEventToHwr:
            {
            TRAP_IGNORE( UpdateICFTextL() );
            TRAP_IGNORE(PopupSwitchWindowL());
            }
            break; 
        case EEventVirtualKeyLatched:
            {
            HandleVirtualKeyLatchedEvent(aEventType, aCtrl, aEventData);
            }
            break;
        case EEventVirtualKeyUnLatched:
            {
            HBufC* newCharBuf = HBufC::New( 2*iNewDeadKeyBuf.Length() );
            if( newCharBuf )
                {
                CPeninputGenericVkbLayout* layout 
                        = static_cast<CPeninputGenericVkbLayout*>( UiLayout() );
                if( layout->IsEnableITIOnFSQ() )
                    {
                    // If ITI is open, double same keys should be send for one dead key,
                    // core will handle them as one key.
                    newCharBuf->Des().Append( iNewDeadKeyBuf );
                    }
                newCharBuf->Des().Append( iNewDeadKeyBuf );
                }
            
            // Submit DeadKey
            UiLayout()->SignalOwner(ESignalKeyEvent, *newCharBuf);
            delete newCharBuf;
             
            TBool deadKeyChange = EFalse;
            iLayoutContext->SetData(EAkninputDataTypeLatchedSet, &deadKeyChange);
            iNewDeadKeyBuf = KNullDesC;
            }
            break;
        case EPeninputLayoutEventBack:
            {
            TInt latchedFlag = CPeninputDataConverter::AnyToInt
                               (iLayoutContext->RequestData(EAkninputDataTypeLatchedSet));
            // If one DeadKey is latched, only unlatch the DeadKey and reset the iNewDeadKeyBuf
            // Don't delete the visible charater in ICF
            if(latchedFlag)
                {
                UnLatchDeadKey(iNewDeadKeyBuf);

                TBool deadKeyChange = EFalse;
                iLayoutContext->SetData(EAkninputDataTypeLatchedSet, &deadKeyChange);
                iNewDeadKeyBuf = KNullDesC;
                }
            }
            break;
        case EPeninputLayoutEventEnter:
        case EPeninputLayoutEventSpace:
            {
            TInt latchedFlag = CPeninputDataConverter::AnyToInt
                               (iLayoutContext->RequestData(EAkninputDataTypeLatchedSet));
            if(latchedFlag)
                {
                HBufC* newCharBuf = HBufC::New( aEventData.Length() + 2*iNewDeadKeyBuf.Length() );
                if( newCharBuf )
                    {
                    CPeninputGenericVkbLayout* layout 
                             = static_cast<CPeninputGenericVkbLayout*>( UiLayout() );
                    if( layout->IsEnableITIOnFSQ() )
                        {
                        // If ITI is open, double same keys should be send for one dead key,
                        // core will handle them as one key.                    
                        newCharBuf->Des().Append( iNewDeadKeyBuf );
                        }
                    newCharBuf->Des().Append( iNewDeadKeyBuf );
                    newCharBuf->Des().Append( aEventData );
                    }
                
                UnLatchDeadKey(iNewDeadKeyBuf);

                TBool deadKeyChange = EFalse;
                iLayoutContext->SetData(EAkninputDataTypeLatchedSet, &deadKeyChange);
                
                // Submit DeadKey + Space or Enter at the same time
                
                if( newCharBuf != NULL )
                    {
                    UiLayout()->SignalOwner(ESignalKeyEvent,*newCharBuf);
                    }
                iNewDeadKeyBuf = KNullDesC;
                delete newCharBuf;
                }
            }
            break;
            
/*        case EPeninputLayoutEventLeftArrow:
            SimulateRawEvent(EStdKeyLeftArrow,TRawEvent::EKeyDown);
            SimulateRawEvent(EStdKeyLeftArrow,TRawEvent::EKeyUp);
            break;

        case EPeninputLayoutEventRightArrow:
            SimulateRawEvent(EStdKeyRightArrow,TRawEvent::EKeyDown);
            SimulateRawEvent(EStdKeyRightArrow,TRawEvent::EKeyUp);
            break;*/
            
        default:
            break;
        }
    }
    
// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::SimulateRawEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType)
    {
    TRawEvent event; 
    event.Set(aType,aScanCode);
    
    TPtrC eventPtr(reinterpret_cast<const TUint16*>(&event),sizeof(event)/sizeof(TUint16));

    UiLayout()->SignalOwner(ESignalSimulateEvent,eventPtr);
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::GetWindowConfigResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputGenericVkbWindow::GetWindowConfigResId()
    {
    return R_PENINPUT_LAYOUT_CONFIG_INFO;
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::GetWindowResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputGenericVkbWindow::GetWindowResId()
    {
    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        return R_PENINPUT_LAYOUT_FSQ_WINDOW_INFO;
        }
    return R_PENINPUT_LAYOUT_VKB_WINDOW_INFO;        
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::GetNumKeymappingResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputGenericVkbWindow::GetNumKeymappingResId()
    {
    return R_PENINPUT_LAYOUT_KEY_MAPPING_STRING_LIST;
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::GetWindowConfigResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TDesC& CPeninputGenericVkbWindow::GetWindowConfigResFileName( 
    TInt aLangID )
    {
    if ( aLangID <= 0 ) 
        {
        return KNullDesC;
        }
        
     if ( aLangID == ELangFrench )
        {

        TBuf<KMaxFileLength>  canadianfrenchresfilename; 
        canadianfrenchresfilename= KConfigurationResourceFile();
        canadianfrenchresfilename.AppendNum( ELangCanadianFrench );
        canadianfrenchresfilename.Append( KResourceFileExtName );

        RFile file;
        TInt rs = file.Open( CCoeEnv::Static()->FsSession(),canadianfrenchresfilename, EFileRead );
        if ( rs == KErrNone )
            {
            file.Close();
            aLangID = ELangCanadianFrench;   
            }
        }
     // If US Rsc file exist, use it instead of English rsc file
     if(( aLangID == ELangEnglish ) && ( iUSRscFileExist ))
    	 {
         aLangID = ELangAmerican; 
    	 }
    iResourceFilename.Zero();
    // Generate resource file name according to language id
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
// CPeninputGenericVkbWindow::GetWindowResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TDesC& CPeninputGenericVkbWindow::GetWindowResFileName()
    {
    return KVkbWindowResourceFile();
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::CreateAllControls
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::CreateAllControlsL()
    {
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                                                   (Control(EPeninputWindowCtrlIdMultiLineICF));
    iBubbleSize = AknLayoutScalable_Apps::popup_char_count_window().LayoutLine();
    iBubbleTextLayout = AknLayoutScalable_Apps::popup_char_count_window_t1(0).LayoutLine();
    
    icf->MsgBubbleCtrl()->SetTextFormat(iBubbleTextLayout);
    icf->MsgBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    //Change the ID when ID into release
    icf->MsgBubbleCtrl()->SetBitmapParam(NULL,
    									  NULL,
    									  KAknsIIDQsnFrInputPreviewSideL,
    									  KAknsIIDQsnFrInputPreviewMiddle,
    									  KAknsIIDQsnFrInputPreviewSideR);  
    icf->MsgBubbleCtrl()->SetTextL( KEmptyString );
    icf->InfoBubbleCtrl()->SetTextFormat(iBubbleTextLayout);
    icf->InfoBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    icf->InfoBubbleCtrl()->SetBitmapParam(NULL,
    									  NULL,
    									  KAknsIIDQsnFrInputPreviewSideL,
    									  KAknsIIDQsnFrInputPreviewMiddle,
    									  KAknsIIDQsnFrInputPreviewSideR);
    if (iLayoutContext->LayoutType() == EPluginInputModeFSQ)
        {
        icf->SetMsgBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
        icf->SetInfoBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
        }
        
    if ( !( iLayoutContext->LayoutType() == EPluginInputModeFSQ ))
        {
        // Range bar
        AddRangeBarL();
        }

    // Vkb control
    iVkbCtrl = CPeninputVkbCtrlExt::NewL( TRect(), UiLayout(), 
        EPeninutWindowCtrlIdVkbCtrl, NULL  );
    iVkbCtrl->SetResourceId( R_PENINPUT_LAYOUT_VKB_KEYBOARD );
    iVkbCtrl->ConstructFromResourceL();
    iVkbCtrl->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );
    iVkbLayout->SetVkbKeyboard( iVkbCtrl );
    if (iLayoutContext->LayoutType() != EPluginInputModeFSQ)
        {
        iVkbCtrl->SetIrregularKeyBoard(ETrue);
        iVkbLayout->SetIrregularResourceId(R_AKN_FEP_VKB_IRREGULAR_BITMAP);
        iVkbLayout->ConstructFromIrregularResourceL();
        }
    else
        {
        iVkbCtrl->SetIrregularKeyBoard(EFalse);
        iVkbLayout->SetNonIrregularResourceId(R_AKN_FEP_VKB_NONIRREGULAR_BITMAP);
        iVkbLayout->ConstructFromNonIrregularResourceL();        
        }

    // Add bubble support
    iVkbCtrl->ShowBubble(ETrue);    
    iVkbCtrl->SetGowithPointerDown(ETrue);
    iVkbCtrl->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview);  //Change the ID when ID into release
    
    //set key to be 9 piece graphics
    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        iVkbCtrl->SetKeySkinId( EKeyBmpNormal, KAknsIIDQsnFrKeypadButtonFrNormal );
        iVkbCtrl->SetKeySkinId( EKeyBmpHighlight, KAknsIIDQsnFrKeypadButtonFrPressed );
        iVkbCtrl->SetKeySkinId( EKeyBmpDim, KAknsIIDQsnFrKeypadButtonFrInactive );
        }
    
    AddControlL( iVkbCtrl );
    iVkbCtrl->AddEventObserver( UiLayout() );
    
    AddButtonL( EPeninutWindowCtrlIdEnterBtn, 
                EPeninputLayoutEventEnter, 
                R_PENINPUT_LAYOUT_VKB_ENTER, 
                EKeyEnter );
       
    // Shift and capslock merged button

  	CAknFepCtrlMultiModeButton* button = 
  		CAknFepCtrlMultiModeButton::NewLC( UiLayout(), 
  										   EPeninutWindowCtrlIdShiftBtn );
    if ( button )
	    {
	    // Read resource
	    TResourceReader reader;
	    
	    CCoeEnv::Static()->CreateResourceReaderLC( reader,
	    	 			   R_PENINPUT_LAYOUT_VKB_SHIFT_CAPSLOCK );  
	    button->SetResourceId( R_PENINPUT_LAYOUT_VKB_SHIFT_CAPSLOCK );      
	    button->ConstructFromResourceL();  
	    // Pop and destroy reader
	    CleanupStack::PopAndDestroy( 1 );
	    
	    // Add into the control pool                                                   
	    AddControlL( button );
	    
	    button->AddEventObserver( UiLayout() );
	    }
    CleanupStack::Pop( button );  
    // Space button          
    CAknFepCtrlEventButton* spaceButton = static_cast<CAknFepCtrlEventButton*>
                                                   (Control(EPeninutWindowCtrlIdSpaceBtn));
    // Create 3-pieces color icon for space button
	CPenInput3PiecesColorIcons* colorIcons = 
				CPenInput3PiecesColorIcons::NewL( R_AKN_FEP_VKB_SPACE_ICON1,
												  R_AKN_FEP_VKB_SPACE_ICON2,
												  R_AKN_FEP_VKB_SPACE_ICON3 );
	spaceButton->Set3PiecesColorIcons( colorIcons );

	//set background of space button to be 9 piece graphics
	if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
	    {
	    spaceButton->SetImageFrameId( KAknsIIDQsnFrKeypadButtonFrNormal, 
	                                  KAknsIIDQsnFrKeypadButtonFrPressed,
	                                  KAknsIIDQsnFrKeypadButtonFrInactive );
	    }
	
	if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
	    {
        // Switch to hwr button  
        AddButtonL( EPeninutWindowCtrlIdMultiRangeBtn, 
                    EPeninputLayoutEventMultiRange, 
                    R_PENINPUT_LAYOUT_FSQ_MULTIRANGE );
	    }
	else
	    {
	    // Switch to hwr button  
	    AddButtonL( EPeninutWindowCtrlIdSwitchToHwrBtn, 
                    EPeninputLayoutEventToHwr, 
                    R_PENINPUT_LAYOUT_VKB_HWR );
	    }
                  
    // Add popup window
    AddPopupWindowL();
  
    }
    
// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::ChangeUnitSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TRect CPeninputGenericVkbWindow::ChangeUnitSize()
    {
    return TRect();
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::ChangeClientSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::ChangeClientSize()
    {
    if ( !iPopupWnd->Hiden() )
        {
        iPopupWnd->CloseWindow();
        if ( iLayoutContext->LayoutType() != EPluginInputModeFSQ )
            {
            ResetAccentButton();
            }
        iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
        }
        
    PopupWindowSizeChange();
            
    TInt clientLayoutId = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );
    CPeninputClientLayoutInfo* clientLayout = 
        ConfigInfo()->FindClientLayoutInfo( clientLayoutId );
        
    if ( clientLayout )
        {
        ReorganizeControls( clientLayout->LayoutID(), EFalse );
        } 

    UpdateArea( iVkbCtrl->Rect(), EFalse );
    }
void CPeninputGenericVkbWindow::SetVkbLayoutSize()
    {
    TRect outrect,innerrect;   
    iLafMgr->GetPreviewWndRect(outrect,innerrect);
    iVkbCtrl->SetTextFormat(iLafMgr->PreviewWndText());  //Read laf 
    iVkbCtrl->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    iVkbCtrl->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
                           innerrect.iTl.iY - outrect.iTl.iY,
                           outrect.iBr.iX - innerrect.iBr.iX,
                           outrect.iBr.iY - innerrect.iBr.iY);
    
    TInt vkbLayoutid = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeVkbLayout ) );
    CPeninputVkbLayoutInfo* vkbLayout = 
        ConfigInfo()->FindVkbLayoutInfo( vkbLayoutid );

    // Calculate current vkblayout according current layout
    if ( vkbLayout )
        {
        const TInt columns = vkbLayout->Columns();
        const TInt rows = vkbLayout->Rows();
        TRect keyRect;
        TRect charRect;
        
        //iLafMgr->SetLayoutType(iLafMgr->LayoutTypeFromRowsColumns(rows,columns));
        
 
        for (TInt exter = 0; exter < rows; exter ++)
            {
            for (TInt inter = 0; inter < columns; inter ++ )
                {
                iLafMgr->GetKeyRect(inter,exter, keyRect, charRect);
                vkbLayout->SetKeyInfo(exter,inter,keyRect,charRect,iLafMgr->KeyTextFont());
                }
            }
     
        if ( iVkbCtrl->IsIrregularKeyBoard() )
            {
            TSize curSize = vkbLayout->KeyInfoList()[0]->Key()->Rect().Size();
            TSize size = iVkbCtrl->IrregularKeyBitmap(EKeyLeftDown,
                                                      EKeyStateNormal)->SizeInPixels();
            if ( curSize != size )
                {
                for ( TInt i = 0; i <= EKeyRightUp; i++ )
                    {
                    for ( TInt j = 0; j <= EKeyStateDimmedMask; j++ )
                        {
                        AknIconUtils::SetSize( iVkbCtrl->IrregularKeyBitmap(
                            (TVirtualKeyIrregularKeyType)i,(TVirtualKeyIrregularKeyState)j), 
                             curSize, EAspectRatioNotPreserved ); 
                            
                        AknIconUtils::SetSize( iVkbCtrl->IrregularKeyBitmap(
                            (TVirtualKeyIrregularKeyType)i,(TVirtualKeyIrregularKeyState)j), 
                             curSize, EAspectRatioNotPreserved );
                        }
                    }
                }
            }
        else
        	{
        	TSize curSize = vkbLayout->KeyInfoList()[0]->Key()->Rect().Size();
        	TSize size = iVkbCtrl->NonIrregularKeyBitmap(EKeyBmpNormal)->SizeInPixels();
        	if( curSize != size )	
        		{
                for ( TInt i = 0; i <= EKeyBmpLastType; i++ )
                    {
                    if( iVkbCtrl->NonIrregularKeyBitmap((TVirtualKeyBmpType)i) )
                    	{
                        AknIconUtils::SetSize( 
                                           iVkbCtrl->NonIrregularKeyBitmap((TVirtualKeyBmpType)i), 
                                           curSize, EAspectRatioNotPreserved );
                    	}
                    }
        		}
        	}
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::ReorganizeControls
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::ReorganizeControls( TInt aClientLayoutId,
                                                    TBool aNeedReset )
    {
    CPeninputClientLayoutInfo* clientLayout = 
        ConfigInfo()->FindClientLayoutInfo( aClientLayoutId ); 
    RPointerArray<CPeninputControlInfo>& controlList = 
        clientLayout->ControlInfoList();
  
    const TInt keyColumns = clientLayout->Columns();
    const TInt keyRows = clientLayout->Rows();
    
    if( iLastUsedTotalColumns == keyColumns &&
    	iLastUsedTotalRows == keyRows )
		{
		return;
		}    

    iLafMgr->SetLayoutType(iLafMgr->LayoutTypeFromRowsColumns(keyRows,keyColumns));
    
    iLastUsedTotalColumns = keyColumns;
    iLastUsedTotalRows = keyRows;
    
    TRect winRect( Rect().iTl, iLafMgr->EntirePaneRect().Size());
        
    SetWindowRect(winRect);  
    //ReorgnizeTitleBar();
    ReorgnizeICFAndButtons();
    
    const TInt count = controlList.Count();
    TInt controlID = 0;
    CFepUiBaseCtrl* ctrl = NULL;
    TRect rect;
    TRect inRect;
    TRect exRect;

    TPoint winPoint = Rect().iTl;
    TPoint LafPoint = iLafMgr->EntirePaneRect().iTl;
    
    TInt x = winPoint.iX - LafPoint.iX;
    TInt y = winPoint.iY - LafPoint.iY;

    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        x = 0;
        y = 0;
        }
    for ( TInt i = 0; i < count; i++ )
        {
        controlID = controlList[i]->ControlID();
        ctrl = Control( controlID );

        if ( ctrl )
            { 
            AddNotOwnedControl( ctrl );
            if ( ( ctrl->ControlType() | ECtrlButton ) == ECtrlButton )
                {
                
                iLafMgr->GetButtonRect(TPeninputCommonCtrlID(ctrl->ControlId()),exRect ,inRect);
                
                exRect.Move(x, y);
                inRect.Move(x, y);
                if( controlID ==  EPeninutWindowCtrlIdShiftBtn)
	                {
					( static_cast<CAknFepCtrlMultiModeButton*> (ctrl) )->
						SizeChanged( exRect, inRect, ETrue );		            	                	
	                }
	            else
		            {
		            if (TPeninputCommonCtrlID(ctrl->ControlId()) == EPeninutWindowCtrlIdSpaceBtn)
		            	{
		            	// config 3 pieces icon info       
		            	TRect iconsFrameRect, middleIconRect;
		            	iLafMgr->GetSpaceButtonExLayoutInfo( iconsFrameRect, middleIconRect );
		            	iconsFrameRect.Move( x, y );
		               	middleIconRect.Move( x, y );
			            inRect = iconsFrameRect;
			            
						( static_cast<CAknFepCtrlCommonButton*> (ctrl) )->SetDraw3PieceFrameInfo(
														  TDraw3PiecesFrame(KAknsIIDQgnIndiInputSpaceL,
    													  KAknsIIDQgnIndiInputSpaceMiddle,
    													  KAknsIIDQgnIndiInputSpaceR,
    													  middleIconRect));			            
		            	}
	                ( static_cast<CAknFepCtrlCommonButton*> (ctrl) )->SizeChanged( exRect, 
                                                                                 inRect, 
                                                                                 ETrue );
		            }
                }
            else if ( ctrl->ControlId() == EPeninutWindowCtrlIdVkbCtrl )
                {
				iVkbCtrl->SetFont( iLafMgr->KeyTextFont() );
				iVkbCtrl->SetTextLineLayout( iLafMgr->KeyTextLayout() );
			    iVkbCtrl->SetTextFormat( iLafMgr->PreviewWndText() );  //Read laf 
				if (aNeedReset)
			        {
			        iVkbCtrl->Reset();
			        }
    
                rect = iLafMgr->KeypadRect();
                
                rect.Move(x, y);

                ( static_cast<CPeninputVkbCtrlExt*> (ctrl) )->SizeChanged(rect);
                }
                
            else if ( ctrl->ControlId() == EPeninutWindowCtrlIdRangeBar )
                {
                TRAP_IGNORE(SetRangeBarSizeL(x, y));
                }
            }
        }

    // Addition for ITI features, read LAF data for candidate list
    const TPeninputCandidateListLayoutData& candidateListLAF =    
                                        iLafMgr->CandidateListLayoutData();
    iCandidateList->SizeChanged( candidateListLAF.iSelectionItemSize, 
                                 candidateListLAF.iSelectionHorMargin,
                                 candidateListLAF.iSelectionVerMargin,
                                 candidateListLAF.iSelectionNaviSize,
                                 candidateListLAF.iSelectionNaviInnerSize );    
    iCandidateList->SetPageInfoTextLineLayout
                                ( candidateListLAF.iSelectionPageTextLine );
    
    
    const CFont* itemFont = candidateListLAF.iFont;    
    TRgb textColor = CandidateListTextColor();
    if ( itemFont )
        {
        // Set font defined in LAF
        iCandidateList->SetTextProperty( itemFont, textColor );
        }
    else
        {
        // If font in LAF is NULL, set ICF text font to candidate list
        iCandidateList->SetTextProperty( iLafMgr->ICFTextFont(), textColor );
        }
    
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                                   (Control(EPeninputWindowCtrlIdMultiLineICF));
    if ( icf )
        {
        // read LAF data for tooltip box
        const TPeninputTooltipBoxLayoutData& tooltipBoxLAF 
                                          = iLafMgr->TooltipBoxLayoutData();
        icf->TooltipBubbleCtrl()->SetTextFormat( tooltipBoxLAF.iTooltipTextLayout );
        icf->TooltipBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG55 );
        icf->TooltipBubbleCtrl()->SetBitmapParam(NULL,
									  NULL,
									  KAknsIIDQsnFrInputPreviewSideL,
									  KAknsIIDQsnFrInputPreviewMiddle,
									  KAknsIIDQsnFrInputPreviewSideR);
        icf->TooltipBubbleCtrl()->SetFrameDiff( KTooltipLeftMargin, 
        		                                KTooltipTopMargin,
        		                                KTooltipRightMargin,
        		                                KTooltipBottomMargin );
        // read color from skin for autocompletion part
        icf->SetAutoCompleteTextColor( AutoCompletionPartColor() );               
        }       
    }

void CPeninputGenericVkbWindow::SetRangeBarSizeL(TInt x, TInt y)
	{
	TRect exRect; 
    exRect = iLafMgr->RangbarRect();
    exRect.Move(x, y);
    // Get range bar display cell rects
	TRect rect, innerRect;
	RArray<TRect> displayCellRects, dispalyCellInnerRects;
	CleanupClosePushL( displayCellRects );
	CleanupClosePushL( dispalyCellInnerRects );
    for( TInt i = 0; i < iLafMgr->RangbarCellCount(); i++ )
    	{
    	iLafMgr->RangbarCellAt( i, rect, innerRect );
		rect.Move(x, y);
		displayCellRects.AppendL( rect );
		innerRect.Move(x, y);
		dispalyCellInnerRects.AppendL( innerRect );               	
    	}
	iRangeBar->SetDispalyCellRectsL( displayCellRects, dispalyCellInnerRects );
	CleanupStack::PopAndDestroy(2);
    iRangeBar->SizeChanged( exRect );
	}

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::SetControlsFont
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::SetControlsFont()
    {
/*    
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>(Control(EPeninputWindowCtrlIdMultiLineICF));
    
    const CFont* icfFont = iLafMgr->ICFTextFont();
    
    // set icf margin
    TPeninputICFConfig icfConfig;
    iLafMgr->GetICFConfig( icfConfig );
    icf->SetTextMargin( icfConfig.iLeftMargin,
    					icfConfig.iRightMargin,
    					icfConfig.iTopMargin,
    					0 );    			
    
    TRAP_IGNORE(icf->SizeChangedL( 
          		icf->Rect(), 
				icfConfig.iTextLineHeight,
				icfFont->FontMaxHeight(),
				icfFont));
*/    
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::AddPopupWindowL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::AddPopupWindowL()
    {
    iPopupWnd = CPeninputCommonChoiceList::NewL( UiLayout(), 
        			EPeninutWindowCtrlIdAccentPopupWindow,
        			KAknsIIDQsnFrList,
        			KAknsIIDQsnFrPopupSub );
    
        
   
    iPopupWnd->AddEventObserver( UiLayout() );
    
    AddControlL( iPopupWnd );
    iIdle = CIdle::NewL(CActive::EPriorityIdle );
    iIdle ->Start(TCallBack(BackgroundTaskL,this));
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::PopupChoiceList
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::PopupChoiceList()
    {
    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        TRAP_IGNORE( PopupRangeListL());
        }
    else
        {
        CFepUiBaseCtrl* accentBtn = Control( EPeninutWindowCtrlIdAccentedBtn );
        
        if ( accentBtn )
            {
            if(!iPopupInited)
                TRAP_IGNORE(DoIdleConstructL());
            iPopupWnd->Display( accentBtn->Rect() );
            iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStatePopup );
            }
        }
    }
// ---------------------------------------------------------------------------
// Show range list
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::PopupRangeListL()
    {
    if ( !iPopupInited )
        {
        TRAP_IGNORE( DoIdleConstructL() );
        }
    

    iPopupWnd->ClearItemsL();
    RArray<CFepLayoutChoiceList::SItem> items;
    PrepareRangeListItems( items );
    TInt count = items.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        iPopupWnd->AddItemL( items[i] );
        }
    items.Close();

    if ( iPopupWnd->ItemsCount() < 1 ) 
        {
        return;
        }
    
    if ( iPopupWnd->ItemsCount() < 2 )
        {
        const CFepLayoutChoiceList::SItem* item = iPopupWnd->ItemByIndex( 0 );
        CFepLayoutChoiceList::SEvent event;
        event.iIndex = 0;
        event.iCommand = item->iCommand;
        TPtrC ptr;
        ptr.Set(reinterpret_cast<TText*>(&event), sizeof(event));
        HandleRangeListSelectedL( iPopupWnd, ptr );
        return;
        }
    
    CFepUiBaseCtrl* btn = Control( EPeninutWindowCtrlIdMultiRangeBtn );    
    iPopupWnd->SetListColumnNumWithLafL( iPopupWnd->ItemsCount() );    
    iPopupWnd->SetCurrentFocusedIndex( - 1 ); //no active item
    iPopupWnd->Display( btn->Rect() );
    iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStatePopup );    
    }

// ---------------------------------------------------------------------------
// Handle range list event
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::HandleRangeListSelectedL( CFepUiBaseCtrl* /*aCtrl*/, 
                                                       const TDesC& aData )
    {
    CFepLayoutChoiceList::SEvent* event = 
        (CFepLayoutChoiceList::SEvent*)aData.Ptr();
    
    TInt index = -1;
    TInt range = ERangeInvalid;

    if ( event->iCommand == EPeninputVkbLayoutNativeChar )
        {
        range = ERangeNative;
        }

    else if ( event->iCommand == EPeninputVkbLayoutNativeNumber )
        {
        range = ERangeNativeNumber;
        }
    
    else if ( event->iCommand == EPeninputVkbLayoutLatinChar )
        {
        range = ERangeEnglish;
        }
    
    else if ( event->iCommand == EPeninputVkbLayoutLatinNumber )
        {
        range = ERangeNumber;
        }
    
    else if ( event->iCommand == EPeninputVkbLayoutSymbol )
        {
        range = ERangeSymbol;
        }

    else if ( event->iCommand == EPeninputVkbLayoutAccented1 )
        {
        index = 0;
        range = ERangeAccent;
        }
    
    else if ( event->iCommand == EPeninputVkbLayoutAccented2 )
        {
        index = 1;
        range = ERangeAccent;
        }
    
    else if ( event->iCommand == EPeninputVkbLayoutAccented3 )
        {
        index = 2;
        range = ERangeAccent;
        }
        
    TInt currentRange = IntContext( EPeninputDataTypeCurrentRange );
    TInt currentAccent = IntContext( EPeninputDataTypeCurrentAccent );

    if ( currentRange == range )
        {
        if ( range != ERangeAccent || currentAccent == index )
            {
            iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
            return;
            }
        }

    iLayoutContext->SetData( EPeninputDataTypeCurrentAccent, &index );

    if ( range != ERangeAccent )
        {
        ChangeRange( range );
        }
    else
        {
        ChangeRange( ERangeAccent, 
                EPeninputVkbLayoutAccented1 + 2 * index, ETrue );
        }
    UpdateRangeCtrlsL();
    }

// ---------------------------------------------------------------------------
// Update state of range button
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::UpdateRangeCtrlsL()
    {
    CAknFepCtrlCommonButton* rangeBtn = 
        static_cast<CAknFepCtrlCommonButton*>( 
                Control(EPeninutWindowCtrlIdMultiRangeBtn));
    if ( !rangeBtn )
        {
        return;
        }
    
    RArray<CFepLayoutChoiceList::SItem> items;
    PrepareRangeListItems( items );
    TInt count = items.Count();
    items.Close();
    
    TInt languageId = IntContext( EPeninputDataTypeInputLanguage );
    TInt range = IntContext( EPeninputDataTypeCurrentRange );

    TInt resId = -1;
    TInt aplhaRangeResId = -1;
    TInt numberRangeResId = -1;
    GetRangeIconResource( languageId, aplhaRangeResId, numberRangeResId );
    
    if ( range == ERangeEnglish || range == ERangeNative ) 
        {
        resId = aplhaRangeResId;
        }
    else
        {
        resId = numberRangeResId; 
        }
        
    if( rangeBtn->ResourceId() != resId )
    	{
		rangeBtn->SetResourceId( resId );
		rangeBtn->ConstructFromResourceL(); 
    	}

    rangeBtn->SetDimmed( count < 1 );    
    }

// ---------------------------------------------------------------------------
// Get resource ids of range button icon
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::GetRangeIconResource( TInt aLangId, 
                                                      TInt& aAlphaRange, 
                                                      TInt& aOtherRange  )
    {   
    switch ( aLangId )
        {
        case ELangRussian:    //Cyrillic
        case ELangBulgarian:
        case ELangUkrainian:
            {
            aAlphaRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_CYRILLIC_ALPHA;
            aOtherRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_CYRILLIC_NUMERIC;
            }
            break;
        case ELangGreek:       //Greek
        case ELangCyprusGreek:
            {
            aAlphaRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_GREEK_ALPHA;
            aOtherRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_GREEK_NUMERIC;
            }
            break;        
        case ELangHebrew:     //Hebrew
            {
            aAlphaRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_HEBREW_ALPHA;
            aOtherRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_HEBREW_NUMERIC;
            }
            break;  
        case ELangArabic:     //Arabic
        case ELangFarsi:      //Farsi
        case ELangUrdu:       //Urdu
            {
            aAlphaRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_ARABIC_ALPHA;
            aOtherRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_ARABIC_NUMERIC;
            }
            break;
        case ELangThai:       //Thai
            {
            aAlphaRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_THAI_ALPHA;
            aOtherRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_THAI_NUMERIC;
            }
            break;
        default:              //others       
            {
            aAlphaRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_LATIN_ALPHA;
            aOtherRange = R_PENINPUT_LAYOUT_VKB_RANGECTRL_LATIN_NUMERIC;
            }
        }
    }

// ---------------------------------------------------------------------------
// Prepare range list items
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::PrepareRangeListItems( 
                     RArray<CFepLayoutChoiceList::SItem>& aRangeItems )
    {
    aRangeItems.Reset();
    TInt currentRange = IntContext( EPeninputDataTypeCurrentRange );
    TInt permittedRange = IntContext( EPeninputDataTypePermittedRange );
    TInt currentAccent = IntContext( EPeninputDataTypeCurrentAccent );
    TInt currentLanguage = IntContext( EPeninputDataTypeInputLanguage );
    
    CPeninputRangeBarInfo* rbinfo= ConfigInfo()->RangeBarInfo();
    CPeninputRangeInfo* rgninfo = NULL;
   
    //add accent items
    if ( permittedRange & ERangeAccent )
        {
        rgninfo = rbinfo->FindRange( ERangeAccent );
        if ( rgninfo )
            {
            for ( TInt i = 0; i < iAccentCmdList.Count(); i++ )
                {
                if( currentRange != ERangeAccent || currentAccent != i )
                    {
                    aRangeItems.Append( iAccentCmdList[i] );
                    }
                }
            }
        }
    
    //add range items  
    CFepLayoutChoiceList::SItem item;
    
    if ( permittedRange & ERangeNative )
        {
        rgninfo = rbinfo->FindRange( ERangeNative );
        if ( rgninfo && currentRange != ERangeNative )
            {
            item.iCommand = EPeninputVkbLayoutNativeChar;
            item.iText.Copy( iRangeLabels.iLabelNativeChar );
            aRangeItems.Append( item );
            }
        }
    
    if (( permittedRange & ERangeNativeNumber ) && ( currentRange != ERangeNumber ) 
    && (( ELangArabic == currentLanguage ) || ( ELangFarsi == currentLanguage ) 
    || ( ELangUrdu == currentLanguage )))
        {
        rgninfo = rbinfo->FindRange( ERangeNativeNumber );
        if ( rgninfo && currentRange != ERangeNativeNumber )
            {
            item.iCommand = EPeninputVkbLayoutNativeNumber;
            item.iText.Copy( iRangeLabels.iLabelNativeNumber );
            aRangeItems.Append( item );
            }
        }
    else if(( permittedRange & ERangeNumber ) && ( currentRange != ERangeNumber ) 
    		&& !(( ELangThai == currentLanguage ) && ( currentRange == ERangeNativeNumber )))
       {
       rgninfo = rbinfo->FindRange( ERangeNumber );
       if ( rgninfo && currentRange != ERangeNumber )
           {
           item.iCommand = EPeninputVkbLayoutLatinNumber;
           item.iText.Copy( iRangeLabels.iLabelLatinNumber );
    	   aRangeItems.Append( item );
    	   }
        }
    else
      	{        
            // Do nothing
       }    
   
    if ( permittedRange & ERangeEnglish )
        {
        rgninfo = rbinfo->FindRange( ERangeEnglish );
        if ( rgninfo && currentRange != ERangeEnglish )
            {
            item.iCommand = EPeninputVkbLayoutLatinChar;
            item.iText.Copy( iRangeLabels.iLabelLatinChar );
            aRangeItems.Append( item );
            }
        }
    
    }

// ---------------------------------------------------------------------------
// Load range labels for range list
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::ConstructRangeLabelListL( TInt /*aLangId*/ )
    {
    iRangeLabels.iLabelNativeChar.Zero();
    iRangeLabels.iLabelNativeNumber.Zero();
    iRangeLabels.iLabelLatinChar.Zero();
    iRangeLabels.iLabelLatinNumber.Zero();
    iRangeLabels.iLabelSymbol.Zero();
    iRangeLabels.iLabelAccent.Zero();
    
    TInt resid = R_PENINPUT_LAYOUT_VKB_RANGES;
    
    TResourceReader reader;   
    CCoeEnv::Static()->CreateResourceReaderLC( reader, resid );
    TPtrC ptr1 = reader.ReadTPtrC();
    TPtrC ptr2 = reader.ReadTPtrC();
    TPtrC ptr3 = reader.ReadTPtrC();
    TPtrC ptr4 = reader.ReadTPtrC();
    TPtrC ptr5 = reader.ReadTPtrC();
    TPtrC ptr6 = reader.ReadTPtrC();
    
    iRangeLabels.iLabelNativeChar.Copy( ptr1 );
    iRangeLabels.iLabelNativeNumber.Copy( ptr2 );
    iRangeLabels.iLabelLatinChar.Copy( ptr3 );
    iRangeLabels.iLabelLatinNumber.Copy( ptr4 );
    iRangeLabels.iLabelSymbol.Copy( ptr5 );
    iRangeLabels.iLabelAccent.Copy( ptr6 );
    
    CleanupStack::PopAndDestroy( 1 ); //reader        
    }

// ---------------------------------------------------------------------------
// Utils to get context value 
// ---------------------------------------------------------------------------
//
TInt CPeninputGenericVkbWindow::IntContext( TPeninputDataType aDataIndex )
    {
    return CPeninputDataConverter::AnyToInt( 
                           iLayoutContext->RequestData( aDataIndex ));
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::ResetAccentButton
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::ResetAccentButton()
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
        
    if ( iRangeBar )
        {
        iRangeBar->ActiveRange( curRange );
        }        
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::PopupWindowSizeChange
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::PopupWindowSizeChange()
    {
    if (CPeninputDataConverter::AnyToInt(
        iLayoutContext->RequestData(EAkninputDataTypeSizeChanging)) || (!iPopupSet))
    	{
    	// if size changing, or popup window's property has not been set yet
        TAknWindowLineLayout appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
        iAppWndRect = appWnd.Rect();
    	
        TAknWindowLineLayout mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
        TAknLayoutRect mainPaneRect;
        mainPaneRect.LayoutRect(iAppWndRect, mainPane);
        
        TAknWindowLineLayout choiceWnd = 
                         AknLayoutScalable_Avkon::popup_choice_list_window(1).LayoutLine();
        TAknLayoutRect choiceRect;
        choiceRect.LayoutRect(mainPaneRect.Rect(), choiceWnd);
        
        iPopupItemRect = TRect(0, 0, choiceRect.Rect().Width(), 
                         AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine().iH);
        
        iChoiceTextLayout = 
                         AknLayoutScalable_Avkon::list_single_choice_list_pane_t1(1).LayoutLine();
    	}
    	
    const CFont* font = AknLayoutUtils::FontFromId(iChoiceTextLayout.iFont, NULL);
    iPopupWnd->SetFont(font);
    iPopupWnd->SetItemRect(iPopupItemRect, iPopupItemRect);

    TInt leftMargin = iChoiceTextLayout.il;
    
    iPopupWnd->SetMarginAndBaseline(leftMargin, font->HeightInPixels());
    
    iPopupWnd->SetFontColor(AKN_LAF_COLOR_STATIC(iChoiceTextLayout.iC));

    AknIconUtils::SetSize(iPopupWndBgImg, TSize(iPopupItemRect.Width(), 
                          iPopupItemRect.Height()*3), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(iPopupWndFocusImg, TSize(iPopupItemRect.Width(), 
                          iPopupItemRect.Height()* 3), EAspectRatioNotPreserved);
    
    iPopupSet = ETrue;
    }


// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::GetPopupWndInfoFromRes
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::GetPopupWndInfoFromResL( TResourceReader aRes,
                                                        const TRect& aRect )
    {
    TPtrC bmpFileName = aRes.ReadTPtrC();        
    TInt32 imgMajorSkinId = aRes.ReadInt32();

    TAknsItemID id;
    
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();

    TInt popWinBmpId = aRes.ReadInt16();
    TInt popWinBmpMaskId = aRes.ReadInt16();
    TInt popWinBmpSkinId = aRes.ReadInt16();
    
    id.Set( TInt( imgMajorSkinId ), popWinBmpSkinId );
    
    if ( popWinBmpId != KInvalidImg )
    	{
        AknsUtils::CreateIconL( skininstance,
                                id,
                                iPopupWndBgImg,
                                bmpFileName,
                                popWinBmpId );
            
    	AknIconUtils::SetSize( iPopupWndBgImg, TSize( aRect.Width(), 
            aRect.Height() * 3 ), EAspectRatioNotPreserved );
        iPopupWnd->SetBackgroundBitmapL( iPopupWndBgImg );
    	}
    	
    TInt focusBmpId = aRes.ReadInt16();
    TInt focusBmpMaskId = aRes.ReadInt16();
    TInt focusBmpSkinId = aRes.ReadInt16();

    id.Set( TInt( imgMajorSkinId ), focusBmpSkinId );
    
    if ( focusBmpId != KInvalidImg )
    	{
        AknsUtils::CreateIconL( skininstance,
                                id,
                                iPopupWndFocusImg,
                                bmpFileName,
                                focusBmpId );

    	AknIconUtils::SetSize( iPopupWndFocusImg, TSize( aRect.Width(), 
            aRect.Height() * 3 ), EAspectRatioNotPreserved );
        iPopupWnd->SetFocusBmp( iPopupWndFocusImg );
    	}
    }
      
// --------------------------------------------------------------------------
// CAknFepVkbClientArea::AddRangeBarL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::AddRangeBarL()
    {
    // Range bar
    iRangeBar = CAknFepCtrlRangeBar::NewL( UiLayout(),
        EPeninutWindowCtrlIdRangeBar, 
        CAknFepCtrlRangeBar::EBtnGroupHorizontal );

    // Set event id    
    iRangeBar->SetEventIdForRange( EPeninputLayoutEventRange );
    iRangeBar->SetEventIdForCase( EPeninputLayoutEventRangeLoop );

    AddControlL( iRangeBar );
    iRangeBar->AddEventObserver(UiLayout());    
    }
    
// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::AddButtonL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputGenericVkbWindow::AddButtonL( 
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
        if ( aControlId == EPeninutWindowCtrlIdMultiRangeBtn )
            {
            button = CAknFepCtrlLongPressButton::NewL( UiLayout(), aControlId, 
                aEventId, aUnicode,
                KAknsIIDQsnFrFunctionButtonNormal,
                KAknsIIDQsnFrFunctionButtonPressed,
                KAknsIIDQsnFrFunctionButtonInactive );                
            }
        else
            {
            button = CAknFepCtrlEventButton::NewL( UiLayout(), aControlId, 
                aEventId, aUnicode,
                KAknsIIDQsnFrFunctionButtonNormal,
                KAknsIIDQsnFrFunctionButtonPressed,
                KAknsIIDQsnFrFunctionButtonInactive );        
            }
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
        AddControlL( button );
        button->AddEventObserver(UiLayout());
        
        }
    CleanupStack::Pop( button );  
    	  
    return button;
    }

// --------------------------------------------------------------------------
// CPeninputGenericVkbWindow::DoCaseChange
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::DoCaseChange( TInt aNewCase )
    {
    TInt curCase = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCase ) );            
    
    if ( aNewCase == curCase )
        {
        return;
        }
        
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) ); 
                               
    if ((( curRange == ERangeAccent ) || ( ( curRange == ERangeEnglish ) 
            || ( ( curRange == ERangeNative ) && ( ConfigInfo()->CaseSensitive()) ) ) ) )
        {
        // Set shift and caplock button
        TInt shfit;  
        TInt capslock;  
        
        CPeninputDataConverter::ShiftCapslockByCase( aNewCase, 
            shfit, capslock );

        iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shfit );
        //ChangeButtonStatus( shfit, EPeninutWindowCtrlIdShiftBtn );          
        iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslock );        
        //ChangeButtonStatus( capslock, EPeninutWindowCtrlIdCapslockBtn );
        iLayoutContext->SetData( EPeninputDataTypeCase, &aNewCase ); 
        ChangeMergedButtonStatus( shfit, capslock );
        
        TInt vkblayout = ConfigInfo()->ShiftCapsSingleVkbLayoutByCase
            ( curRange, aNewCase );

        if ( curRange == ERangeAccent ) 
           {
           TInt curAccentIndex = CPeninputDataConverter::AnyToInt
               ( iLayoutContext->RequestData( EPeninputDataTypeCurrentAccent ) );
                 vkblayout = vkblayout + curAccentIndex * 2;
           }
        
        ChangeVkbLayout( vkblayout );
        }
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbWindow::BackgroundConstructL
// Do background construct.
// ---------------------------------------------------------------------------
//
TInt CPeninputGenericVkbWindow::BackgroundTaskL(TAny* aPtr)
    {
    CPeninputGenericVkbWindow* self = static_cast<CPeninputGenericVkbWindow*>(aPtr);
    self->DoIdleConstructL();
    return EFalse; // only run once
    }


void CPeninputGenericVkbWindow::DoIdleConstructL()
    {   
    if(iPopupInited)
        return;
    

    // Read resource
    TResourceReader reader;
    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, 
        R_PENINPUT_LAYOUT_VKB_POPUP_WINDOW ); 
        
    GetPopupWndInfoFromResL(reader, iPopupItemRect);    
       
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );

    }
    
// -----------------------------------------------------------------------------
// CPeninputGenericVkbWindow::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::ConstructFromResourceL()
    {     	        
    CPeninputLayoutWindowExt::ConstructFromResourceL();
    if( iVkbLayout && !iFirstConstruct )
    	{
    	if( iVkbCtrl->IsIrregularKeyBoard() )    	
    		iVkbLayout->ConstructFromIrregularResourceL();
    	else
    		iVkbLayout->ConstructFromNonIrregularResourceL();
    	
    	//SetVkbLayoutSize();
    	
    	if( IsRtoLLanguage( iLanguage ) )
    		{
			//Mirror backspace graphic
			HandleButtonResOnLangDirChange( EPeninutWindowCtrlIdBackspaceBtn );    			
    		}
    	}
    	
    iFirstConstruct = EFalse;
    }
    
// -----------------------------------------------------------------------------
// CPeninputGenericVkbWindow::SetSwitchlistLanguage
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void  CPeninputGenericVkbWindow::SetSwitchlistLanguage(TInt aLang)
    {
    ASSERT( IsValidLanguage( aLang ) );
    //Check language input direction change
    TBool isLangDirectionChange = EFalse;
    if( IsValidLanguage( iLanguage ) )
    	isLangDirectionChange =  IsRtoLLanguage( aLang ) ^ IsRtoLLanguage( iLanguage );    
    else
    	isLangDirectionChange = IsRtoLLanguage( aLang );
    
    if( aLang ==ELangHebrew || 
	    aLang == ELangGreek ||
		aLang == ELangHindi ||
		aLang == ELangMarathi ) 
		{
		iLafMgr->SetLafOption( (TPeninputVkbLafOption)
							   ( iLafMgr->OptionType() &(~EOption3CellRange) ) );	
		}
	else
		{
		iLafMgr->SetLafOption( (TPeninputVkbLafOption)
				   ( iLafMgr->OptionType() | EOption3CellRange) );	
		}
			
	if( isLangDirectionChange )
		{
		//Mirror backspace graphic
		HandleButtonResOnLangDirChange( EPeninutWindowCtrlIdBackspaceBtn );
		}
			
	iLanguage = aLang;	
    }

// -----------------------------------------------------------------------------
// CPeninputGenericVkbWindow::SetSwitchlistSecretFlag
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void  CPeninputGenericVkbWindow::SetSwitchlistSecretFlag(TBool aSecret)
    {
    CPeninputLayoutInputmodelChoice* choiceList = 
        static_cast<CPeninputLayoutInputmodelChoice*>( 
            Control(EPeninutWindowCtrlIdSwitcherPopupWindow ));
        
    if ( choiceList )
        {
        choiceList->SetSecretEditor( aSecret );
        }
	//disable or enable arrow keys of FSQ in secret editor.    	
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericVkbWindow::DimArrowKeys
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::DimArrowKeys( TBool aDimArrow )
    {
    if(iLayoutContext->LayoutType() == EPluginInputModeFSQ 
       && iArrowLeftButton && iArrowRightButton)
        {
        iArrowLeftButton->SetDimmed( aDimArrow );
        iArrowRightButton->SetDimmed( aDimArrow );
        }	    	
    }

void CPeninputGenericVkbWindow::DimEnterKey( TBool aDimmed )
    {
	CAknFepCtrlEventButton* enterBtn = static_cast<CAknFepCtrlEventButton*>
	                              ( Control(EPeninutWindowCtrlIdEnterBtn) );	
    if(iLayoutContext->LayoutType() == EPluginInputModeFSQ 
       && enterBtn )
        {
        enterBtn->SetDimmed( aDimmed );        
        }
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::PopupAccentWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::PopupSwitchWindowL()
    {
    CFepUiBaseCtrl* accentBtn = Control(EPeninutWindowCtrlIdSwitchToHwrBtn);

    if ( accentBtn )
        {
        if (!iPopupInited)
            {
        	  DoIdleConstructL();
            }
        TRect rect = accentBtn->Rect();   
		static_cast<CPeninputLayoutInputmodelChoice*>
		    	(Control(EPeninutWindowCtrlIdSwitcherPopupWindow))->PopUpSwitchListL( accentBtn->Rect() );
        }         
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbWindow::SetDeadKey
// (Set DeadKey)
// ---------------------------------------------------------------------------
//     
void CPeninputGenericVkbWindow::SetDeadKey()
    {
    // Get VKB list
    RPointerArray<CPeninputVkbLayoutInfo> vkbListInfo;
    vkbListInfo = iVkbLayout->VkbLayoutInfoList();
    TInt vkbListNum = vkbListInfo.Count();
    
    CPeninputVkbKeyInfo* pVKBKeyInfo;
    CVirtualKey* pKey;
    TInt count = sizeof(KSupportLanguages)/sizeof(TLangMeritPair);
    for(TInt i = 0; i < vkbListNum; i++)
        {
        for(TInt j = 0; j < count; j++)
            {
            if(vkbListInfo[i]->LayoutID() == EPeninputVkbLayoutNativeChar ||
               vkbListInfo[i]->LayoutID() == EPeninputVkbLayoutNativeCharShift)
                {
                pVKBKeyInfo = vkbListInfo[i]->FindKey( KSupportLanguages[j].iDeadKey );
                if(pVKBKeyInfo)
                    {
                    // Set the DeadKey as a latch button
                    pKey = pVKBKeyInfo->Key();
                    pKey->SetLatchKey(ETrue);
                    }
                }
            }
        }
    }
    
    
// ---------------------------------------------------------------------------
// CPeninputGenericVkbWindow::UnLatchDeadKey
// (Unlatch the DeadKey)
// ---------------------------------------------------------------------------
//     
void CPeninputGenericVkbWindow::UnLatchDeadKey( const TDesC& abuf )
    {
    RPointerArray<CPeninputVkbLayoutInfo> vkbListInfo;
    vkbListInfo = iVkbLayout->VkbLayoutInfoList();
    TInt vkbListNum = vkbListInfo.Count();
    
    CPeninputVkbKeyInfo* pVKBKeyInfo;
    CVirtualKey* pKey;
    TInt count = sizeof(KSupportLanguages)/sizeof(TLangMeritPair);
    for(TInt i = 0; i < vkbListNum; i++)
        {
        for(TInt j = 0; j < count; j++)
            {
            // Only NativeChar and NativeCharShift layout contain DeadKey
            if(vkbListInfo[i]->LayoutID() == EPeninputVkbLayoutNativeChar ||
               vkbListInfo[i]->LayoutID() == EPeninputVkbLayoutNativeCharShift)
                {
                // Find the DeadKey in NativeChar or NativeCharShift layout
                pVKBKeyInfo = vkbListInfo[i]->FindKey( KSupportLanguages[j].iDeadKey );
                if(pVKBKeyInfo)
                    {
                    pKey = pVKBKeyInfo->Key();
                    if(pKey->Latched())
                        {
                        if(abuf == KNullDesC)
                            {
                            // When focus change, the latched key will be released
                            TBool deadKeyChange = EFalse;
                            iLayoutContext->SetData(EAkninputDataTypeLatchedSet, &deadKeyChange);
                            pKey->SetLatched(EFalse);
                            TRect rect = pKey->Rect();
                            rect.Move( iVkbCtrl->Rect().iTl );
                            UpdateArea( rect, EFalse ); 
                            return;
                            }
                        if(abuf[0] == KSupportLanguages[j].iDeadKey)
                            {
                            // When another DeadKey is clicked, the old one will be released
                            pKey->SetLatched(EFalse);
                            TRect rect = pKey->Rect();
                            rect.Move( iVkbCtrl->Rect().iTl );
                            UpdateArea( rect, EFalse ); 
                            return;
                            }
                        }
                    }
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbWindow::HandleAccentCharEvent
// (Unlatch the DeadKey)
// ---------------------------------------------------------------------------
//     
TBool CPeninputGenericVkbWindow::HandleAccentCharEvent( TInt /*aEventType*/, 
                                                    const TDesC& aEventData, 
                                                    const TDesC& aDeadKey, 
                                                    HBufC* anewCharBuf )
    {
    // Get the unicode of virtual key char
    TBuf<16> buf;
    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
    buf.Append(event->iCode);
    TBool rs = EFalse;
    
    // Caculate the accent char number
    TInt count = sizeof(KSupportLanguages)/sizeof(TLangMeritPair);
    TInt i = 0; 
    if(aDeadKey.Compare( KNullDesC ) != 0) // One DeadKey is latched
        {
        for(i = 0; i < count; i++)
            {
            if((aDeadKey[0] == KSupportLanguages[i].iDeadKey) && 
               (buf[0] == KSupportLanguages[i].iChar))
                {
                if(anewCharBuf)
                    {                    
                    anewCharBuf->Des().Append(KSupportLanguages[i].iAccentedChar);
                    rs = ETrue;
                    }
                break;
                }
            }
        }
    return rs;
    }
    
 
// ---------------------------------------------------------------------------
// CPeninputGenericVkbWindow::HandleVirtualKeyLatchedEvent
// (Handle virtual key latched event).
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::HandleVirtualKeyLatchedEvent(TInt /*aEventType*/, 
                                                         CFepUiBaseCtrl* /*aCtrl*/, 
                                                         const TDesC& aEventData )
    {
    TBuf<16> deadKey(KNullDesC);
    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
    deadKey.Append(event->iCode);

    TInt latchedFlag = CPeninputDataConverter::AnyToInt(
                       iLayoutContext->RequestData(EAkninputDataTypeLatchedSet));
    if(latchedFlag)
        {
        // Update the new and old DeadKey buffer
        iOldDeadKeyBuf = iNewDeadKeyBuf;
        iNewDeadKeyBuf = deadKey;

        HBufC* newCharBuf = HBufC::New( 2*iOldDeadKeyBuf.Length() );
        if( newCharBuf )
            {
            CPeninputGenericVkbLayout* layout 
                    = static_cast<CPeninputGenericVkbLayout*>( UiLayout() );
            if( layout->IsEnableITIOnFSQ() )
                {
                // If ITI is open, double same keys should be send for one dead key,
                // core will handle them as one key.
                newCharBuf->Des().Append( iOldDeadKeyBuf );
                }
            newCharBuf->Des().Append( iOldDeadKeyBuf );
            }
        
        // Submit old DeadKey
        UiLayout()->SignalOwner(ESignalKeyEvent, *newCharBuf);
        delete newCharBuf;                     
     
        // Unlatch the previous DeadKey
        UnLatchDeadKey(iOldDeadKeyBuf);
        

        }
    else
        {
        iNewDeadKeyBuf = deadKey;
        
        // Set DeadKey state to latched
        TBool deadKeyChange = ETrue;
        iLayoutContext->SetData(EAkninputDataTypeLatchedSet, &deadKeyChange);
        }
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::HandleVirtualKeyUpEvent
// (Handle virtual key up event).
// ---------------------------------------------------------------------------
//
TBool CPeninputGenericVkbWindow::HandleVirtualKeyUpEvent(TInt aEventType, 
                                                    CFepUiBaseCtrl* /*aCtrl*/, 
                                                    const TDesC& aEventData )
    {
    // Get DeadKey state
    TInt latchedFlag = CPeninputDataConverter::AnyToInt(
                       iLayoutContext->RequestData(EAkninputDataTypeLatchedSet));
    if(latchedFlag)
        {
        TBool ret = EFalse;
        
        TRAP_IGNORE(ret = HandleDeadKeyL(aEventType,aEventData))
        
        if(ret)
        	{
        	return ETrue;	
        	}
        else
            {
            // When the DeadKey can not combine with current character,
            // Sumit current DeadKey
            UiLayout()->SignalOwner( ESignalKeyEvent, iNewDeadKeyBuf );
            }
        }
        
    iNewDeadKeyBuf = KNullDesC;
    return EFalse;
    } 
 
 TBool CPeninputGenericVkbWindow::HandleDeadKeyL(TInt aEventType, 
                                                 const TDesC& aEventData )
 	{
    // Set DeadKey to unlatched state
    TBool deadKeyChange = EFalse;
    iLayoutContext->SetData(EAkninputDataTypeLatchedSet, &deadKeyChange);
    UnLatchDeadKey(iNewDeadKeyBuf);

    CPeninputGenericVkbLayout* layout 
                      = static_cast<CPeninputGenericVkbLayout*>( UiLayout() );
 	if ( layout->IsEnableITIOnFSQ() )
 		{
 	    TBuf<KKeyCodeSize> buf;
 	    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
 	    buf.Append(event->iCode);

		HBufC* newCharBuf = HBufC::New( iNewDeadKeyBuf.Length() + buf.Length() );
		if( newCharBuf )
			{
			newCharBuf->Des().Append( iNewDeadKeyBuf );
			newCharBuf->Des().Append( buf );
			}
 	    
        // Submit DeadKey + Key at the same time
        UiLayout()->SignalOwner(ESignalKeyEvent,*newCharBuf);
		
        delete newCharBuf;
 		
 		return ETrue;
 		}
 	
    // Get the accent char
    HBufC* newCharBuf = HBufC::NewL(1);
    if(HandleAccentCharEvent(aEventType, 
                             aEventData, 
                             iNewDeadKeyBuf, 
                             newCharBuf))
        {
        // Send ths accent key to FEP
        UiLayout()->SignalOwner( ESignalKeyEvent, *newCharBuf);
        iNewDeadKeyBuf = KNullDesC;
        delete newCharBuf;
        return ETrue;
        }
    delete newCharBuf;
    
    return EFalse; 		
 	}
 
void CPeninputGenericVkbWindow::SetLafLayOut(TInt aRows, TInt aColumns)
    {
    iLafMgr->SetLayoutType(iLafMgr->LayoutTypeFromRowsColumns(aRows,aColumns));
    }

    
void CPeninputGenericVkbWindow::AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl)
	{
	AddControlL(aCtrl);
	}    

TBool CPeninputGenericVkbWindow::CreateCustomControlL( TInt16 aControlId, TInt32 aImageId )
	{
	TBool create = ETrue;
    switch ( aControlId )
        {
        case EPeninutWindowCtrlIdMoveBtn:
            {
            if ( !iMoveButton )
                {
                iMoveButton = CAknFepCtrlDragButton::NewL(UiLayout(), EPeninutWindowCtrlIdMoveBtn);
                AddControlL(iMoveButton);
                iMoveButton->AddPositionObserver(this);
                iMoveButton->AddEventObserver( UiLayout() );
                iMoveButton->SetResourceId( aImageId );
                iMoveButton->SetImageFrameId(KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive);
                UiLayout()->EnableLayoutMoving( static_cast<CDragBar*>( iMoveButton ) );
                }
            iMoveButton->ConstructFromResourceL();
            }
            break;
        case EPeninupWindowCtrlIdArrowLeftBtn:
            {
            if ( iLayoutContext->LayoutType() != EPluginInputModeFSQ )
                {
                break;
                }

            if (!iArrowLeftButton)
                {
                
	            iArrowLeftButton = CAknFepCtrlRepeatButton::NewL( 
	                UiLayout(), aControlId, EPeninputLayoutEventLeftArrow, EKeyLeftArrow, 
	            								  KAknsIIDQsnFrFunctionButtonNormal,
	        									  KAknsIIDQsnFrFunctionButtonPressed,
	        									  KAknsIIDQsnFrFunctionButtonInactive );

                AddControlL(iArrowLeftButton);
                iArrowLeftButton->AddEventObserver(UiLayout());
                iArrowLeftButton->SetResourceId( aImageId );
                }

            iArrowLeftButton->ConstructFromResourceL();
            }
            break;
        case EPeninupWindowCtrlIdArrowRightBtn:
            {
            if ( iLayoutContext->LayoutType() != EPluginInputModeFSQ )
                {
                break;
                }

            if (!iArrowRightButton)
                {
	            iArrowRightButton = CAknFepCtrlRepeatButton::NewL( 
	                UiLayout(), aControlId, EPeninputLayoutEventRightArrow, EKeyRightArrow, 
	            								  KAknsIIDQsnFrFunctionButtonNormal,
	        									  KAknsIIDQsnFrFunctionButtonPressed,
	        									  KAknsIIDQsnFrFunctionButtonInactive );

                AddControlL(iArrowRightButton);
                iArrowRightButton->AddEventObserver(UiLayout());
                iArrowRightButton->SetResourceId( aImageId );

                }
            iArrowRightButton->ConstructFromResourceL();
            }
            break;
        default:
            {
            create = EFalse;
            }
        }
        return create;
    }
    
void CPeninputGenericVkbWindow::SetPromptTextL(TUint8* aData)
    {
    RDesReadStream readStream;

    TPtr8 countPtr(aData, 2*sizeof(TInt), 2*sizeof(TInt));
	  readStream.Open(countPtr);
	  CleanupClosePushL(readStream);
    const TInt dataCount = readStream.ReadInt32L();
	  const TInt textCount = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy(&readStream);
    
    TPtr8 ptr( aData+2*sizeof(TInt), dataCount+textCount, dataCount+textCount );            
	  readStream.Open(ptr);
	  CleanupClosePushL(readStream);
	
	  HBufC8* dataBuf = HBufC8::NewLC(dataCount);
	  TPtr8 dataBufPtr = dataBuf->Des();
	  readStream.ReadL(dataBufPtr, dataCount);

    TFepPromptText* pIcfData = 
        reinterpret_cast<TFepPromptText*>(const_cast<TUint8*>(dataBufPtr.Ptr()));

    HBufC* textBuf;
    if (textCount > 0)
        {
        textBuf = HBufC::NewLC(textCount/2);
    	  TPtr textBufPtr = textBuf->Des();
    	  readStream.ReadL(textBufPtr, textCount/2);
        static_cast<CFepLayoutMultiLineIcf*>
            (Control(EPeninputWindowCtrlIdMultiLineICF))->SetPromptTextL
                (textBuf->Des(), pIcfData->iCleanContent);
        CleanupStack::PopAndDestroy(textBuf);
        }
    else
        {
        static_cast<CFepLayoutMultiLineIcf*>
            (Control(EPeninputWindowCtrlIdMultiLineICF))->SetPromptTextL(KNullDesC, pIcfData->iCleanContent);
        }

    CleanupStack::PopAndDestroy(dataBuf);
	CleanupStack::PopAndDestroy(&readStream);
    }    

void CPeninputGenericVkbWindow::UpdateLafData()
    {
    TRAP_IGNORE( iLafMgr->UpdateLafDataL() );
    }

void CPeninputGenericVkbWindow::ReorgnizeICFAndButtons()
	{
    TRect rect;
    
    TPoint winPoint = Rect().iTl;
    TPoint LafPoint = iLafMgr->EntirePaneRect().iTl;
    
    TInt x = winPoint.iX - LafPoint.iX;
    TInt y = winPoint.iY - LafPoint.iY;    
    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        x = 0;
        y = 0;
        }
        
    TRect inRect;
    TRect exRect;
    
   
    rect = iLafMgr->ICFRect();
    rect.Move(x, y);
    
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                                  (Control(EPeninputWindowCtrlIdMultiLineICF));

    const CFont* icfFont = iLafMgr->ICFTextFont();
    
    // set icf margin
    TPeninputICFConfig icfConfig;
    iLafMgr->GetICFConfig( icfConfig );
    icf->SetTextMargin( icfConfig.iLeftMargin,
    					icfConfig.iRightMargin,
    					icfConfig.iTopMargin,
    					0 );    			
    icf->SetLineSpace( icfConfig.iLineSpaceMargin );
    TRAP_IGNORE(icf->SizeChangedL( 
          		rect, 
				icfConfig.iTextLineHeight,
				icfFont->FontMaxHeight(),
				icfFont));

    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        iLafMgr->GetButtonRect( 
                EPeninutWindowCtrlIdMultiRangeBtn, exRect, inRect );
        exRect.Move(x, y); 
        inRect.Move(x, y);
        static_cast<CAknFepCtrlEventButton*>
            ( Control( EPeninutWindowCtrlIdMultiRangeBtn ))->SizeChanged( 
              exRect, inRect, ETrue );
            
        TRAP_IGNORE( icf->MsgBubbleCtrl()->SetTextL( KEmptyString ));
        iIndiWithText = EFalse;
        IndiBubbleWithoutText();
        }
    
    iLafMgr->GetButtonRect(EPeninutWindowCtrlIdOptionBtn,exRect ,inRect);
    exRect.Move(x, y); 
    inRect.Move(x, y);
    static_cast<CAknFepCtrlEventButton*>
        ( Control( EPeninutWindowCtrlIdOptionBtn ) )->SizeChanged( exRect, inRect, ETrue );

    iLafMgr->GetButtonRect(EPeninutWindowCtrlIdCloseBtn,exRect ,inRect);
    exRect.Move(x, y); 
    inRect.Move(x, y);
    static_cast<CAknFepCtrlCommonButton*>
        ( Control( EPeninutWindowCtrlIdCloseBtn ) )->SizeChanged( exRect, inRect, ETrue );

    iLafMgr->GetButtonRect(EPeninutWindowCtrlIdBackspaceBtn,exRect ,inRect);
    exRect.Move(x, y); 
    inRect.Move(x, y);
    
    // Check size change of bk button, need for mirroring when arabic
    CAknFepCtrlCommonButton* bkBtn = static_cast<CAknFepCtrlCommonButton*>
        ( Control( EPeninutWindowCtrlIdBackspaceBtn ) );
    TBool bBKSizeChanged = EFalse;
	if( exRect.Size() != bkBtn->Rect().Size() &&
		inRect.Size() != bkBtn->ForgroundBmpRect().Size() )
		{
		bBKSizeChanged = ETrue;
		}    
    bkBtn->SizeChanged( exRect, inRect, ETrue );


    iLafMgr->GetButtonRect(EPeninutWindowCtrlIdSpaceBtn,exRect ,inRect);
    inRect = exRect;
    inRect.Shrink(10, 0);
    
    exRect.Move(x, y); 
    inRect.Move(x, y);
    static_cast<CAknFepCtrlCommonButton*>
        ( Control( EPeninutWindowCtrlIdSpaceBtn ) )->SizeChanged( exRect, inRect, ETrue );
        
    TRect spaceIconRect = inRect;
    spaceIconRect.Shrink(10, 0);
	static_cast<CAknFepCtrlCommonButton*>
        ( Control( EPeninutWindowCtrlIdSpaceBtn ) )->SetDraw3PieceFrameInfo(
                      TDraw3PiecesFrame(KAknsIIDQgnIndiInputSpaceL,
            													  KAknsIIDQgnIndiInputSpaceMiddle,
            													  KAknsIIDQgnIndiInputSpaceR,
            													  spaceIconRect));

    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        iLafMgr->GetButtonRect(EPeninupWindowCtrlIdArrowLeftBtn,exRect ,inRect);
        exRect.Move(x, y); 
        inRect.Move(x, y);
        iArrowLeftButton->SizeChanged( exRect, inRect, ETrue );
        
        iLafMgr->GetButtonRect(EPeninupWindowCtrlIdArrowRightBtn,exRect ,inRect);
        exRect.Move(x, y);
        inRect.Move(x, y); 
        iArrowRightButton->SizeChanged( exRect, inRect, ETrue );
        }
	else
		{
	    iLafMgr->GetButtonRect(EPeninutWindowCtrlIdMoveBtn,exRect ,inRect);
	    exRect.Move(x, y); 
	    inRect.Move(x, y);
	    static_cast<CAknFepCtrlDragButton*>
	        ( Control( EPeninutWindowCtrlIdMoveBtn ) )->SizeChanged( exRect, inRect, ETrue );
		}    

    //If backspace size changed when language is RtoL mirror should be used    
    if( bBKSizeChanged && IsRtoLLanguage( iLanguage ) )
    	{
		//Mirror backspace graphic
		HandleButtonResOnLangDirChange( EPeninutWindowCtrlIdBackspaceBtn );
    	}		
		    
	}
void CPeninputGenericVkbWindow::ShowBubble(TInt aShow)
    {
    if (aShow > 0)
        {
        iVkbCtrl->ShowBubble(ETrue);    
        }
    else
        {
        iVkbCtrl->ShowBubble(EFalse);     
        }
    }
	
void CPeninputGenericVkbWindow::ConstructAccentListL(TInt aLang)
	{
	iPopupWnd->ClearItemsL();
	
	if (aLang == ELangVietnamese)
		{
		_LIT( KVietAccentList1, "\x00E0 - \x013A" );
		_LIT( KVietAccentList2, "\x013C - \x017E" );
		
	    CFepLayoutChoiceList::SItem item;

	    item.iCommand = EPeninputVkbLayoutAccented1;
	    item.iText.Copy( KVietAccentList1 );
	    iPopupWnd->AddItemL( item );
	    
	    item.iCommand = EPeninputVkbLayoutAccented2;
	    item.iText.Copy( KVietAccentList2 );
	    iPopupWnd->AddItemL( item );
		}
	// 10X3
	else if (aLang == ELangEnglish
	        || aLang == ELangAmerican
	        || aLang == ELangDutch
	        || aLang == ELangTagalog
	        || aLang == ELangIndonesian
	        || aLang == ELangMalay)
		    {
	        _LIT( KVietAccentList1, "\x00E0 - \x0121" );
	        _LIT( KVietAccentList2, "\x0123 - \x014F" );
	        _LIT( KVietAccentList3, "\x0151 - \x017E" );
	        
	        CFepLayoutChoiceList::SItem item;

	        item.iCommand = EPeninputVkbLayoutAccented1;
	        item.iText.Copy( KVietAccentList1 );
	        iPopupWnd->AddItemL( item );
	        
	        item.iCommand = EPeninputVkbLayoutAccented2;
	        item.iText.Copy( KVietAccentList2 );
	        iPopupWnd->AddItemL( item );
	        
            item.iCommand = EPeninputVkbLayoutAccented3;
            item.iText.Copy( KVietAccentList3 );
            iPopupWnd->AddItemL( item );
		    }
	//10X4
	else
		{
	    // Add item
	    TResourceReader reader;
	    CFepLayoutChoiceList::SItem item;

	    item.iCommand = EPeninputVkbLayoutAccented1;
	    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT1 );
	    TPtrC p1 = reader.ReadTPtrC();
	    item.iText.Copy( p1 );
	    iPopupWnd->AddItemL( item );
	    CleanupStack::PopAndDestroy( 1 );
	    item.iCommand = EPeninputVkbLayoutAccented2;
	    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT2 );
	    TPtrC p2 = reader.ReadTPtrC();
	    item.iText.Copy( p2 );
	    iPopupWnd->AddItemL( item );
	    CleanupStack::PopAndDestroy( 1 );
	    
		item.iCommand = EPeninputVkbLayoutAccented3;
		CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT3 );
		TPtrC p3 = reader.ReadTPtrC();
		item.iText.Copy( p3 );
		iPopupWnd->AddItemL( item );
		CleanupStack::PopAndDestroy( 1 );      	
		}
	iPopupWnd->SetListColumnNumWithLafL( iPopupWnd->ItemsCount() );
    iPopupInited = ETrue;
    iPopupSet = ETrue;
	}
// ---------------------------------------------------------------------------
// Load accent labels for range list
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::ConstructFSQAccentListL( TInt aLangId )
    {  
    iAccentCmdList.Reset();
    
    TBool is10x3 = ( aLangId == ELangEnglish ) || ( aLangId == ELangAmerican )
                   || ( aLangId == ELangDutch)  || (aLangId == ELangTagalog ) 
                   || ( aLangId == ELangIndonesian) || (aLangId == ELangMalay );
    
    if ( aLangId == ELangVietnamese )
        {
        _LIT( KVietAccentList1, "\x00E0 - \x013A" );
        _LIT( KVietAccentList2, "\x013C - \x017E" );
        
        CFepLayoutChoiceList::SItem item;

        item.iCommand = EPeninputVkbLayoutAccented1;
        item.iText.Copy( KVietAccentList1 );      
        iAccentCmdList.Append( item );
        
        item.iCommand = EPeninputVkbLayoutAccented2;
        item.iText.Copy( KVietAccentList2 );
        iAccentCmdList.Append( item );
        }
    else if( aLangId == ELangRussian || aLangId == ELangUkrainian || aLangId == ELangBulgarian )
    	{
        _LIT( KCyrillicAccent, "\x00E0 - \x017E" );
        CFepLayoutChoiceList::SItem item;

        item.iCommand = EPeninputVkbLayoutAccented1;
        item.iText.Copy( KCyrillicAccent );      
        iAccentCmdList.Append( item );        
    	}
    // 10X3
    else if ( is10x3 )
        {
        _LIT( KVietAccentList1, "\x00E0 - \x0121" );
        _LIT( KVietAccentList2, "\x0123 - \x014F" );
        _LIT( KVietAccentList3, "\x0151 - \x017E" );
        
        CFepLayoutChoiceList::SItem item;

        item.iCommand = EPeninputVkbLayoutAccented1;
        item.iText.Copy( KVietAccentList1 );
        iAccentCmdList.Append( item );
        
        item.iCommand = EPeninputVkbLayoutAccented2;
        item.iText.Copy( KVietAccentList2 );
        iAccentCmdList.Append( item );

        
        item.iCommand = EPeninputVkbLayoutAccented3;
        item.iText.Copy( KVietAccentList3 );
        iAccentCmdList.Append( item );
        }
    //10X4
    else
        {
        // Add item
        TResourceReader reader;
        CFepLayoutChoiceList::SItem item;

        item.iCommand = EPeninputVkbLayoutAccented1;
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT1 );
        TPtrC p1 = reader.ReadTPtrC();
        item.iText.Copy( p1 );
        iAccentCmdList.Append( item );
        CleanupStack::PopAndDestroy( 1 ); //reader
        
        item.iCommand = EPeninputVkbLayoutAccented2;
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT2 );
        TPtrC p2 = reader.ReadTPtrC();
        item.iText.Copy( p2 );
        iAccentCmdList.Append( item );
        CleanupStack::PopAndDestroy( 1 ); //reader
        
        item.iCommand = EPeninputVkbLayoutAccented3;
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT3 );
        TPtrC p3 = reader.ReadTPtrC();
        item.iText.Copy( p3 );
        iAccentCmdList.Append( item );
        CleanupStack::PopAndDestroy( 1 );//reader          
        }

    iPopupInited = ETrue;
    iPopupSet = ETrue;
    }
void CPeninputGenericVkbWindow::UpdateICFTextL()
    {
    CFepLayoutMultiLineIcf* icf = 
        static_cast<CFepLayoutMultiLineIcf*>(Control(EPeninputWindowCtrlIdMultiLineICF));
    if( icf )
        {
        icf->UpdateSecretTextL();    
        }
    }
    
void CPeninputGenericVkbWindow::HandleButtonResOnLangDirChange( TInt aControlId )
	{
    TRAP_IGNORE(HandleButtonResOnLangDirChangeL(aControlId));
    }
void CPeninputGenericVkbWindow::HandleButtonResOnLangDirChangeL( TInt aControlId )
	{
	CAknFepCtrlCommonButton* ctrl = static_cast<CAknFepCtrlCommonButton*>(
										Control(aControlId) );
	if( ctrl && iBmpRotator)
		{
		TSyncRotationAngle rotChoice = CBitmapRotator::EMirrorVerticalAxis; 
		User::LeaveIfError( iBmpRotator->Rotate( *ctrl->ForgroundBmp() ,
								  				 rotChoice ) );
		User::LeaveIfError( iBmpRotator->Rotate( *ctrl->ForgroundBmpMask() ,
								  				 rotChoice ) );   								  
		}		
	}    
		
// --------------------------------------------------------------------------
// Show tooltip of ICF
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::ShowTooltipL( const TDesC& aText, TInt aLangCode )
    {
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                                (Control(EPeninputWindowCtrlIdMultiLineICF));    
    CPeninputGenericVkbLayout* layout 
                      = static_cast<CPeninputGenericVkbLayout*>( UiLayout() );    
    if ( !icf || !( layout->IsEnableITIOnFSQ() ) )
        {
        return;
        }    
    
    // Calculate tooltip width
    const TPeninputTooltipBoxLayoutData& tooltipBoxLAF 
                                      = iLafMgr->TooltipBoxLayoutData();
    TRect tooltipRect = tooltipBoxLAF.iRect;
    // Calculate inner rect
    TRect innerRect = tooltipRect;
    innerRect.iTl += TPoint( KTooltipLeftMargin, KTooltipTopMargin );
    innerRect.iBr -= TPoint( KTooltipRightMargin, KTooltipBottomMargin );          
    const CFont* font = tooltipBoxLAF.iFont;
    // Set inner width in terms of text width
    innerRect.SetWidth( font->TextWidthInPixels( aText ) );    
    tooltipRect.iTl 
           = innerRect.iTl - TPoint( KTooltipLeftMargin, KTooltipTopMargin );
    tooltipRect.iBr 
           = innerRect.iBr + TPoint( KTooltipRightMargin, KTooltipBottomMargin );
    // Set tooltip rect in terms of inner rect
    icf->ShowTooltipL( aText, tooltipRect, aLangCode );
    }
	
// --------------------------------------------------------------------------
// Hide tooltip of ICF
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::HideTooltip()
    {
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                                (Control(EPeninputWindowCtrlIdMultiLineICF));
    if ( !icf  )
        {
        return;
        }
    icf->HideTooltip();
    }

// --------------------------------------------------------------------------
// Hide candidate list
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::HideCandidateList()
    {
    if ( !iCandidateList )
        {
        return;
        }
    iCandidateList->Hide( ETrue );
    }
	
// --------------------------------------------------------------------------
// Show candidate list
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::ShowCandidateListL( TInt aAlign, const CDesCArray* aItemArray,
                                                   TInt aActiveIndex )
    {
    if ( !iCandidateList )
        {
        return;
        }                
    iCandidateList->ClearItemsL();
	iCandidateList->SetTextAlignment((CGraphicsContext::TTextAlign) aAlign);

    if ( aItemArray )
        {
		CFepLayoutChoiceList::SItem item;
		item.iCommand = 0;
		
        for (TInt i = 0; i < aItemArray->Count(); ++i)
            {
            if ((*aItemArray)[i].Length() <= item.iText.MaxLength())
                {
                item.iText.Copy((*aItemArray)[i]);
                }
            else
                {
                // Given longer than maxlength, display the part of less than max
                item.iText.Copy((*aItemArray)[i].Left(item.iText.MaxLength()));
                }
			
            iCandidateList->AddItemL( item );
            }
        }
    iCandidateList->SetCurrentFocusedIndex( aActiveIndex );
    
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
         (Control(EPeninputWindowCtrlIdMultiLineICF)); 

    TPoint posBelow( 0, 0 );
    TPoint posRight( 0, 0 );
    posBelow = posRight = icf->Rect().iTl;
    icf->InlineTextPos( posBelow, posRight );
    TPoint tlPos = posRight;
    iCandidateList->Display( TRect( tlPos, tlPos ), 
                             CFepLayoutPopupWnd::EDispBottomRight );
    }

// --------------------------------------------------------------------------
// Handle candidate list selected events.
// --------------------------------------------------------------------------
//
void CPeninputGenericVkbWindow::HandleCandidateListSelectedL
                         ( CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    if ( aCtrl != iCandidateList )
        {
        return;
        }

    CFepLayoutChoiceList::SEvent* event = 
            (CFepLayoutChoiceList::SEvent*)aEventData.Ptr();
    if ( event->iIndex != KErrNotFound )
        {
        TPtrC ptr;
        ptr.Set( reinterpret_cast<TUint16*>(&( event->iIndex ) ), 
                 sizeof( TInt ) );            
        UiLayout()->SignalOwner( ESignalSelectCandidate, ptr );
        }
    }

// --------------------------------------------------------------------------
// Create a candidate list control.
// --------------------------------------------------------------------------
//    
void CPeninputGenericVkbWindow::CreateCandidateListL()
    {
    iCandidateList = CFepLayoutScrollableList::NewL( 
                            UiLayout(), 
                            EPentinputHwrCandidateList,
                            TSize( 0, 0 ),
                            0,
                            0,
                            TSize( 0, 0 ),
                            TSize( 0, 0 ) );
         
    iCandidateList->SetImgFrId(KAknsIIDQsnFrPopupSub, 
                                KAknsIIDQsnFrFunctionButtonNormal,
								KAknsIIDQsnFrFunctionButtonPressed,
								KAknsIIDQsnFrFunctionButtonInactive,
                                KAknsIIDQsnFrList,
                                KAknsIIDQsnFrPopupSub);
                                
    iCandidateList->SetResourceId(R_PENINPUT_FSQ_CANDIDATE_LIST);
    iCandidateList->ConstructFromResourceL();
    iCandidateList->SetPageInfoResID( R_PENINPUT_FSQ_CANDIDATE_LIST_PAGE_NUM );
    iCandidateList->AddEventObserver( UiLayout() );
    AddControlL(iCandidateList);    
    }

// --------------------------------------------------------------------------
// Retrieve text color from skin for candidiate list.
// --------------------------------------------------------------------------
//
TRgb CPeninputGenericVkbWindow::CandidateListTextColor()
    {
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();
    TRgb matchlistcolor = KRgbBlack;
    AknsUtils::GetCachedColor( skininstance, 
                                matchlistcolor, 
                                KAknsIIDQsnTextColors,
                                EAknsCIQsnTextColorsCG43 );
    return matchlistcolor;
    }

// --------------------------------------------------------------------------
// Retrieve text color from skin for autocompletion part.
// --------------------------------------------------------------------------
//
TRgb CPeninputGenericVkbWindow::AutoCompletionPartColor()
    {
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();
    TRgb matchlistcolor = KRgbBlack;
    AknsUtils::GetCachedColor( skininstance, 
                                matchlistcolor, 
                                KAknsIIDQsnTextColors,
                                EAknsCIQsnTextColorsCG64 );
    return matchlistcolor;
    }

void CPeninputGenericVkbWindow::SetIndiBubbleImageL( const TInt aImgID1,
                                              const TInt aMaskID1,
                                              const TInt aImgID2,
                                              const TInt aMaskID2 )
    {
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();

    CFbsBitmap* bmp1 = NULL;
    CFbsBitmap* mask1 = NULL;
    
    TInt colorIndex = EAknsCIQsnIconColorsCG30;

    AknsUtils::CreateColorIconL(skininstance,
                                KAknsIIDQsnIconColors,
                                KAknsIIDQsnIconColors,
                                colorIndex,
                                bmp1,
                                mask1,
                                AknIconUtils::AvkonIconFileName(),
                                aImgID1,
                                aMaskID1,
                                KRgbGray);
    CleanupStack::PushL( bmp1 );
    CleanupStack::PushL( mask1 );
                                
    AknIconUtils::GetContentDimensions(bmp1, iIndicatorSize);

    CFbsBitmap* bmp2 = NULL;
    CFbsBitmap* mask2 = NULL;
    AknsUtils::CreateColorIconL(skininstance,
                                KAknsIIDQsnIconColors,
                                KAknsIIDQsnIconColors,
                                colorIndex,
                                bmp2,
                                mask2,
                                AknIconUtils::AvkonIconFileName(),
                                aImgID2,
                                aMaskID2,
                                KRgbGray);
 
    CleanupStack::PushL( bmp2 );
    CleanupStack::PushL( mask2 );
    
    AknIconUtils::GetContentDimensions(bmp2, iIndicatorTextSize);
    
    TRect boundRect;
    if ( iIndiWithText )
        {
        boundRect = iLafMgr->IndiIconRectWithText();
        }
    else
        {
        boundRect = iLafMgr->IndiIconRectWithoutText();
        }
    
    TRect imgrect, textrect;
    
    CalIndicatorRect(boundRect, imgrect, textrect, EIndiAlignCenter);
    AknIconUtils::SetSize(bmp1, imgrect.Size(), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(mask1, imgrect.Size(), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(bmp2, textrect.Size(), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(mask2, textrect.Size(), EAspectRatioNotPreserved);

    CFbsBitmap* bmp3 = AknPenImageUtils::CombineTwoImagesL(bmp1, bmp2, bmp1->DisplayMode());
    CFbsBitmap* mask3 = AknPenImageUtils::CombineTwoImagesL(mask1, mask2, EGray256);
    
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                                   (Control(EPeninputWindowCtrlIdMultiLineICF));
    icf->MsgBubbleCtrl()->SetBitmapParam( bmp3, mask3, 
                    KAknsIIDQsnFrInputPreviewSideL,
                    KAknsIIDQsnFrInputPreviewMiddle,
                    KAknsIIDQsnFrInputPreviewSideR );
    
    CleanupStack::PopAndDestroy( mask2 );
    CleanupStack::PopAndDestroy( bmp2 );
    CleanupStack::PopAndDestroy( mask1 );
    CleanupStack::PopAndDestroy( bmp1 );
    }

void CPeninputGenericVkbWindow::CalIndicatorRect(const TRect& aBoundRect,
                                          TRect& aRealRect1,
                                          TRect& aRealRect2,
                                          TIndicatorAlign aAlign) 
    {
    TInt imgAspectText = iIndicatorTextSize.iWidth / iIndicatorTextSize.iHeight;
    TInt imgAspectIndi = iIndicatorSize.iWidth / iIndicatorSize.iHeight;
    TSize imgSizeText( aBoundRect.Size().iHeight * imgAspectText, 
                       aBoundRect.Size().iHeight );
    TSize imgSizeIndi( aBoundRect.Size().iHeight * imgAspectIndi, 
                               aBoundRect.Size().iHeight );
    // check if the length of img > bound rect width
    TInt nTotalWidth = imgSizeText.iWidth + imgSizeIndi.iWidth;
    if( nTotalWidth > aBoundRect.Size().iWidth )
        {
        TReal nAspect = (TReal)imgSizeText.iWidth / nTotalWidth;
        imgSizeText.iWidth = aBoundRect.Size().iWidth * nAspect;
        imgSizeIndi.iWidth = aBoundRect.Size().iWidth - imgSizeText.iWidth;
        imgSizeText.iHeight = imgSizeText.iWidth / imgAspectText;
        // make sure the height of two rect is equal
        imgSizeIndi.iHeight = imgSizeText.iHeight;
        }
    if( aAlign == EIndiAlignRight )
        {
        aRealRect2 = TRect(TPoint( aBoundRect.iBr.iX - imgSizeText.iWidth, aBoundRect.iTl.iY),
                       imgSizeText);
        aRealRect1 = TRect(TPoint(aRealRect2.iTl.iX - imgSizeIndi.iWidth, aRealRect2.iTl.iY),
                       imgSizeIndi);
        }
    else if( aAlign == EIndiAlignCenter )
        {
        TInt offsetX = ( aBoundRect.Size().iWidth - imgSizeText.iWidth - imgSizeIndi.iWidth ) / 2;
        TInt offsetY = ( aBoundRect.Size().iHeight - imgSizeText.iHeight ) / 2;
        aRealRect2 = TRect( TPoint( aBoundRect.iBr.iX - imgSizeText.iWidth - offsetX, 
                                   aBoundRect.iTl.iY + offsetY),
                            imgSizeText );
        aRealRect1 = TRect( TPoint(aRealRect2.iTl.iX - imgSizeIndi.iWidth, aRealRect2.iTl.iY),
                       imgSizeIndi );
        }
    else if( aAlign == EIndiAlignLeft )
        {
        aRealRect1 = TRect( aBoundRect.iTl, imgSizeIndi );
        aRealRect2 = TRect( TPoint( aRealRect1.iBr.iX, aRealRect1.iTl.iY ), imgSizeText );
        }
    }

void CPeninputGenericVkbWindow::IndiBubbleWithText()
    {
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                           (Control(EPeninputWindowCtrlIdMultiLineICF));
    if ( icf )
        {
        TAknTextLineLayout textLine = iLafMgr->IndiText();
        TRect bubbleRect = iLafMgr->IndiPaneRectWithText();
        TRect iconRect = iLafMgr->IndiIconRectWithText();
        TSize offset;
        offset.iHeight = iconRect.iTl.iY - bubbleRect.iTl.iY;
        offset.iWidth = iconRect.iTl.iX - bubbleRect.iTl.iX;
        TSize size( iconRect.Width(), iconRect.Height());
        
        icf->MsgBubbleCtrl()->SetRect( bubbleRect );
        icf->MsgBubbleCtrl()->SetIconOffsetAndSize( offset, size );
        icf->MsgBubbleCtrl()->SetTextFormat( textLine );
        icf->MsgBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
        }
    }

void CPeninputGenericVkbWindow::IndiBubbleWithoutText()
    {
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                               (Control(EPeninputWindowCtrlIdMultiLineICF));
    if ( icf )
        {
        TRect bubbleRect = iLafMgr->IndiPaneRectWithoutText();
        TRect iconRect = iLafMgr->IndiIconRectWithoutText();
        TSize offset;
        offset.iHeight = iconRect.iTl.iY - bubbleRect.iTl.iY;
        offset.iWidth = iconRect.iTl.iX - bubbleRect.iTl.iX;
        TSize size( iconRect.Width(), iconRect.Height());
        
        icf->MsgBubbleCtrl()->SetRect( bubbleRect );
        icf->MsgBubbleCtrl()->SetIconOffsetAndSize( offset, size );
        }
    }

void CPeninputGenericVkbWindow::SetIndiWithTextFlag( TBool aFlag )
    {
    iIndiWithText = aFlag;
    }

void CPeninputGenericVkbWindow::UpdateIndiBubbleL( TUint8* aData )
    {
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
                               (Control(EPeninputWindowCtrlIdMultiLineICF));
    
    RDesReadStream readStream;
    TFepIndicatorInfo indicatorData;

    TPtr8 countPtr( aData, 4*sizeof(TInt), 4*sizeof(TInt) );            
    readStream.Open(countPtr);
    CleanupClosePushL(readStream);

    indicatorData.iIndicatorImgID = readStream.ReadInt32L();
    indicatorData.iIndicatorMaskID = readStream.ReadInt32L();
    indicatorData.iIndicatorTextImgID = readStream.ReadInt32L();
    indicatorData.iIndicatorTextMaskID = readStream.ReadInt32L();

    CleanupStack::PopAndDestroy(&readStream);
    
    if ( indicatorData.iIndicatorImgID != 0 && 
        indicatorData.iIndicatorMaskID != 0 && 
        indicatorData.iIndicatorTextImgID != 0 &&
        indicatorData.iIndicatorTextMaskID != 0 )
        { 
        SetIndicatorData( indicatorData );
        
        if ( icf )
            {
            SetIndiBubbleImageL( indicatorData.iIndicatorImgID,
                    indicatorData.iIndicatorMaskID,
                    indicatorData.iIndicatorTextImgID,
                    indicatorData.iIndicatorTextMaskID );
                 
            TBuf<100> text;
            icf->MsgBubbleCtrl()->GetText( text );
            icf->ShowBubble( text, icf->MsgBubbleCtrl()->Rect());
            }
        }
    }

void CPeninputGenericVkbWindow::HandleRangeButtonShortPress()
    {
    const TInt range = CPeninputDataConverter::AnyToInt(
        iLayoutContext->RequestData(EPeninputDataTypeCurrentRange));
    
    const TInt langId = IntContext( EPeninputDataTypeInputLanguage );

    switch ( langId )
        {
        case ELangRussian:
        case ELangBulgarian:
        case ELangUkrainian:
            {
            range == ERangeNative ? ChangeRange( ERangeNumber ) : ChangeRange( ERangeNative );
            }
            break;

        case ELangArabic:
        case ELangFarsi:
        case ELangUrdu:
            {
            range == ERangeNative ? ChangeRange( ERangeNativeNumber ) : ChangeRange( ERangeNative );
            }
            break;
            
        case ELangThai:
            {
            range == ERangeNative ? ChangeRange( ERangeNumber ) : ChangeRange( ERangeNative ); 
            }
            break;

        case ELangGreek:
        case ELangHebrew:
            {
            range == ERangeNative ? ChangeRange( ERangeNumber ) : ChangeRange( ERangeNative ); 
            }
            break;
        default: // Latin, vietnamese, and other possible languages
            {
            if ( ConfigInfo()->RangeBarInfo()->FindRange( ERangeNative ) )
                {
                if ( range == ERangeNative )
                    {
                    if ( ConfigInfo()->RangeBarInfo()->FindRange( ERangeNativeNumber ) )
                        {
                        ChangeRange( ERangeNativeNumber );
                        }
                    else if ( ConfigInfo()->RangeBarInfo()->FindRange( ERangeNumber ) )
                        {
                        ChangeRange( ERangeNumber );
                        }
                    }
                else
                    {
                    ChangeRange( ERangeNative );
                    }
                }
            else if ( ConfigInfo()->RangeBarInfo()->FindRange( ERangeEnglish ) )
                {
                range == ERangeEnglish ? ChangeRange( ERangeNumber ) : ChangeRange( ERangeEnglish );
                }
            }
            break;
        }
    
    TRAP_IGNORE( UpdateRangeCtrlsL() );
    }
// End Of File
