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
* Description:  peninput VKB data manager
*
*/

#include <PeninputVkbKrWindowConfigInfo_65.rsg>
#include <PeninputVkbKrWindowKr.rsg>

#include <aknlayoutscalable_apps.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>

#include <e32keys.h>
#include <s32mem.h>
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <AknLayoutDef.h>
#include <AknIconUtils.h>
#include <AknUtils.h>
#include <AknFontAccess.h> 

#include <peninputcommonlayoutglobalenum.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputlayoutcontext.h>
#include <peninputinputcontextfield.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputrangebar.h>
#include <peninputrangebarinfo.h>
#include <peninputclientlayoutinfo.h>
#include <peninputlayoutchoicelist.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputlabel.h>

#include <peninputmultimodebutton.h>
#include <peninputeventbutton.h>
#include <peninputdragbutton.h>
#include <peninputrepeatbutton.h>

#include <peninputdataconverter.h>
#include <peninputuistatemgrinterface.h>

#include "PeninputVkbKrDbg.h"
#include "PeninputVkbKr.pan"
#include "PeninputVkbKr.hrh"
#include "PeninputVkbKrEnum.h"
#include "PeninputVkbKrUiStateType.h"
#include "PeninputVkbKrWindow.h"
#include "peninputgenericvkblafdatamgr.h"

_LIT( KWindowConfigurationResourceFile,
      "z:\\resource\\plugins\\PeninputVkbKrWindowConfigInfo_65.rsc" );
_LIT( KWindowResourceFile,
      "z:\\resource\\plugins\\PeninputVkbKrWindowKr.rsc" );
const TInt KInvalidImg = -1 ;
const TInt KPeninputVkbWndInvalidIndex = -1;
//const TUint32 KDefaultTextColor = 0x000000;
//const TUint32 KDefaultShadowTextColor = 0x000000;

struct TDummyRangeInfo // from CPeninputRangeInfo
    {
    
    void* vertual1;

    /**
     * Range id
     */
    TInt iRangeId;
    
    /**
     * Response style
     */
    TPeninputRangeResponseStyle iResponseStyle;

    /**
     * Client layout id
     */
    TInt iClientLayoutId;

    /**
     * Vkb layout id
     */
    TInt iVkbLayoutId;

    };

struct TSymbolTableInfo
    {
    TInt iVkbLayoutId;
    TInt iClientLayoutId;
    };

const TSymbolTableInfo sSymbolTableInfo[] = 
    {
        {EPeninputVkbLayoutSymbol1,EPeninputClientLayout11x4},
        {EPeninputVkbLayoutSymbol2,EPeninputClientLayout11x4},
        {EPeninputVkbLayoutSymbol3,EPeninputClientLayout11x4},
        {EPeninputVkbLayoutSymbol4,EPeninputClientLayoutMacroChars},
        {EPeninputVkbLayoutSymbol5,EPeninputClientLayoutMacroChars},
    };

const TInt KMaxSymbolTable = sizeof(sSymbolTableInfo)/sizeof(TSymbolTableInfo);

namespace SymbolTableUtil
    {
    
    LOCAL_C TInt FindIndex(TInt aVkbLayoutId)
        {
        TInt ret(KErrNotFound);
        for (TInt i=0;i<KMaxSymbolTable;i++)
            {
            if (aVkbLayoutId==sSymbolTableInfo[i].iVkbLayoutId)
                {
                ret=i;
                break;
                }
            }
        return ret;
        }

    LOCAL_C TInt GetClientLaytoutFor(TInt aVkbLayoutId)
        {
        TInt ret(0);
        TInt index(FindIndex(aVkbLayoutId));
        if (index!=KErrNotFound)
            {
            ret=sSymbolTableInfo[index].iClientLayoutId;
            }
        return ret;
        }
    
    LOCAL_C TInt GetNextVkbLayout(TInt aVkbLayoutId)
        {
        TInt ret(aVkbLayoutId);
        TInt index=FindIndex(aVkbLayoutId);
        if (index!=KErrNotFound)
            {
            index=(index+1)%KMaxSymbolTable;
            ret=sSymbolTableInfo[index].iVkbLayoutId;
            }
        return ret;
        }
    
    LOCAL_C TInt GetPrevVkbLayout(TInt aVkbLayoutId)
        {
        TInt ret(aVkbLayoutId);
        TInt index=FindIndex(aVkbLayoutId);
        if (index!=KErrNotFound)
            {
            index=(index+KMaxSymbolTable-1)%KMaxSymbolTable;
            ret=sSymbolTableInfo[index].iVkbLayoutId;
            }
        return ret;
        }
    }

// ---------------------------------------------------------------------------
// Constructor & Destructor
// ---------------------------------------------------------------------------
//

CPeninputVkbKrWindow* CPeninputVkbKrWindow::NewLC(
        CFepUiLayout* aUiLayout, 
        MPeninputLayoutContext* aLayoutContext,
        MPeninputUiStateMgr* aUiStateMgr)
    {
    CPeninputVkbKrWindow* self = 
        new (ELeave) CPeninputVkbKrWindow(aUiLayout,aLayoutContext,aUiStateMgr);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPeninputVkbKrWindow* CPeninputVkbKrWindow::NewL(
        CFepUiLayout* aUiLayout, 
        MPeninputLayoutContext* aLayoutContext,
        MPeninputUiStateMgr* aUiStateMgr)
    {
    CPeninputVkbKrWindow* self = 
        CPeninputVkbKrWindow::NewLC(aUiLayout,aLayoutContext,aUiStateMgr);
    CleanupStack::Pop(); // self;
    return self;
    }

CPeninputVkbKrWindow::CPeninputVkbKrWindow(
        CFepUiLayout* aUiLayout, 
        MPeninputLayoutContext* aLayoutContext,
        MPeninputUiStateMgr* aUiStateMgr)
    : CPeninputLayoutWindowExt(aUiLayout,aLayoutContext),
      iUiStateMgr(aUiStateMgr),
      iTableUpButton(NULL),
      iTableDownButton(NULL),
      iTablePageLabel(NULL),
      iFirstConstruct(ETrue),
      iPopupSet(EFalse),
      iPopupInited(EFalse)
    {
    // No implementation required
    }

void CPeninputVkbKrWindow::ConstructL()
    {
    CPeninputLayoutWindowExt::ConstructL();
    iLafMgr = CPeninputGenericVkbLafMgr::NewL();
    iLafMgr->SetInputModeL(TPluginInputMode(iLayoutContext->LayoutType()));
    TInt option(iLafMgr->OptionType());
    option|=EOption3CellRange;
    iLafMgr->SetLafOption((TPeninputVkbLafOption)option);  
    }

CPeninputVkbKrWindow::~CPeninputVkbKrWindow()
    {
    delete iLafMgr;
    delete iIdle;
    }

// ---------------------------------------------------------------------------
// from CPeninputLayoutWindow
// ---------------------------------------------------------------------------
//

void CPeninputVkbKrWindow::HandleControlEvent(
        TInt aEventType,
        CFepUiBaseCtrl* aCtrl, 
        const TDesC& aEventData )
    {
    TInt* data = ( TInt* ) aEventData.Ptr(); 
    // Call this function in base class
    
    if (aEventType==EPeninputLayoutEventRange)
        {
        PreProcessRange(*data);
        }
    
    CPeninputLayoutWindowExt::HandleControlEvent(
        aEventType, 
        aCtrl, 
        aEventData );
    
    switch ( aEventType )
        {   
        case EPeninputLayoutVkbEventResetShift: 
            {
            HandleShiftBtnClicked();
            }
            break;  
        case EPenInputLayoutEventClosePopup:
            {
            if ( iPopupWnd && !iPopupWnd->Hiden() )
                {
                iPopupWnd->CloseWindow();
                ResetAccentButton();
                iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
                }
            }
            break;
        case EPeninputLayoutEventOpen:
        case EPeninputLayoutEventRange:
            {
            UpdateArea( Rect(), ETrue );
            }
            break;
        case EEventChoiceSelected:
            {
            CFepLayoutChoiceList::SEvent* event = 
                ( CFepLayoutChoiceList::SEvent* ) aEventData.Ptr();

            if ( event->iIndex != KPeninputVkbWndInvalidIndex )
                {
                if (aCtrl->ControlId() == EPeninutWindowCtrlIdAccentPopupWindow)
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
                    /*
                    (currentRange == ERangeAccent) ? ChangeRange(ERangeAccent, 
                                       EPeninputVkbLayoutAccented1 + 2 * event->iIndex, EFalse) :
                                                     ChangeRange(ERangeAccent, 
                                       EPeninputVkbLayoutAccented1 + 2 * event->iIndex, ETrue);
                    */
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
                }
            else
                {
                ResetAccentButton();   
                }
            iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
            }
            break;
        case EPeninputLayoutEventToHwr:
            {
            TRAP_IGNORE(UpdateICFTextL());
            TRAP_IGNORE(PopupSwitchWindowL());
            }
            break;
        case EPeninputLayoutVkbEventTableUp:
            if (IsSymbolRange())
                {
                TInt curVkbLayout = CPeninputDataConverter::AnyToInt(
                    iLayoutContext->RequestData(EPeninputDataTypeVkbLayout));
                TInt prevVkbLayout=SymbolTableUtil::GetPrevVkbLayout(curVkbLayout);
                TRAP_IGNORE( FakeClientLayoutUpdateL(prevVkbLayout));
                ChangeRange(ERangeSymbol,prevVkbLayout);
                }
            break;
        case EPeninputLayoutVkbEventTableDown:
            if (IsSymbolRange())
                {
                TInt curVkbLayout = CPeninputDataConverter::AnyToInt(
                    iLayoutContext->RequestData(EPeninputDataTypeVkbLayout));
                TInt nextVkbLayout=SymbolTableUtil::GetNextVkbLayout(curVkbLayout);
                TRAP_IGNORE( FakeClientLayoutUpdateL(nextVkbLayout));
                ChangeRange(ERangeSymbol,nextVkbLayout);
                }
            break;
        default:
            break;
        }
    }


TInt CPeninputVkbKrWindow::GetWindowConfigResId()
    {
    return R_PENINPUT_LAYOUT_CONFIG_INFO;
    }

TInt CPeninputVkbKrWindow::GetWindowResId()
    {
    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        return R_PENINPUT_LAYOUT_FSQ_WINDOW_INFO;
        }
    return R_PENINPUT_LAYOUT_VKB_WINDOW_INFO;        
    }

TInt CPeninputVkbKrWindow::GetNumKeymappingResId()
    {
    return R_PENINPUT_LAYOUT_KEY_MAPPING_STRING_LIST;
    }

const TDesC& CPeninputVkbKrWindow::GetWindowConfigResFileName( TInt /*aLangID*/ )
    {
    return KWindowConfigurationResourceFile();
    }

const TDesC& CPeninputVkbKrWindow::GetWindowResFileName()
    {
    return KWindowResourceFile();
    }

const TRect CPeninputVkbKrWindow::ChangeUnitSize()
    {
    return TRect();
    }

void CPeninputVkbKrWindow::ChangeClientSize()
    {
    LOG("Vkb.Window.ChangeClientSize");
    if ( !iPopupWnd->Hiden() )
        {
        iPopupWnd->CloseWindow();
        ResetAccentButton();
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

    TRect rect=iVkbCtrl->Rect();
    LOG2("Vkb.Window.ChangeClientSize %d %d",rect.Width(),rect.Height());
    UpdateArea( iVkbCtrl->Rect(), EFalse );
    }

void CPeninputVkbKrWindow::CreateAllControlsL()
    {
    //LOG("Vkb.Window.CreateAllControlsL");
    CFepLayoutMultiLineIcf* icf = 
        static_cast<CFepLayoutMultiLineIcf*>(Control(EPeninputWindowCtrlIdMultiLineICF));
    
    iBubbleSize = AknLayoutScalable_Apps::popup_char_count_window().LayoutLine();
    iBubbleTextLayout = AknLayoutScalable_Apps::popup_char_count_window_t1(0).LayoutLine();
    
    icf->MsgBubbleCtrl()->SetTextFormat(iBubbleTextLayout);
    icf->MsgBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    //Change the ID when ID into release
    icf->MsgBubbleCtrl()->SetBitmapParam(
        NULL,
        NULL,
        KAknsIIDQsnFrInputPreviewSideL,
        KAknsIIDQsnFrInputPreviewMiddle,
        KAknsIIDQsnFrInputPreviewSideR);  
    
    /* Teleca change begin, 20.05.2008 ssal */									  
    icf->InfoBubbleCtrl()->SetTextFormat(iBubbleTextLayout);
    icf->InfoBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    //Change the ID when ID into release
    icf->InfoBubbleCtrl()->SetBitmapParam(NULL,
    									  NULL,
    									  KAknsIIDQsnFrInputPreviewSideL,
    									  KAknsIIDQsnFrInputPreviewMiddle,
    									  KAknsIIDQsnFrInputPreviewSideR);
    /* Teleca change end, 20.05.2008 ssal */
    
    if (iLayoutContext->LayoutType() == EPluginInputModeFSQ)
        {
        icf->SetMsgBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
         /* Teleca change begin, 20.05.2008 ssal */
        icf->SetInfoBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
        /* Teleca change end, 20.05.2008 ssal */
        }
        
    // Range bar
    iRangeBar = CAknFepCtrlRangeBar::NewL(
        UiLayout(),
        EPeninutWindowCtrlIdRangeBar, 
        //CAknFepCtrlRangeBar::EBtnGroupHorizontal );
        CAknFepCtrlRangeBar::EBtnGroupVertical );

    // Set event id    
    iRangeBar->SetEventIdForRange( EPeninputLayoutEventRange );
    iRangeBar->SetEventIdForCase( EPeninputLayoutEventRangeLoop );

    AddControlL( iRangeBar );
    iRangeBar->AddEventObserver((MEventObserver*)UiLayout());    

    // Vkb control
    iVkbCtrl = CPeninputVkbCtrlExt::NewL( 
        TRect(), 
        UiLayout(), 
        EPeninutWindowCtrlIdVkbCtrl, 
        NULL  );
    iVkbCtrl->SetResourceId( 
        R_PENINPUT_LAYOUT_VKB_KEYBOARD );
    iVkbCtrl->ConstructFromResourceL();
    iVkbCtrl->SetKeyTextColorGroup( 
        EAknsCIQsnTextColorsCG68 );
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
    
    // Switch to hwr button  
    AddButtonL( EPeninutWindowCtrlIdSwitchToHwrBtn, 
                EPeninputLayoutEventToHwr, 
                R_PENINPUT_LAYOUT_VKB_HWR );
                  
    // Add popup window
    //AddPopupWindowL();
    iPopupWnd = CPeninputCommonChoiceList::NewL( UiLayout(), 
                    EPeninutWindowCtrlIdAccentPopupWindow,
                    KAknsIIDQsnFrList,
                    KAknsIIDQsnFrPopupSub );
    
        
   
    iPopupWnd->AddEventObserver( UiLayout() );
    
    AddControlL( iPopupWnd );
    iIdle = CIdle::NewL(CActive::EPriorityIdle );
    iIdle ->Start(TCallBack(BackgroundTaskL,this));
    }

void CPeninputVkbKrWindow::SetControlsFont()
    {
    //LOG("Vkb.Window.SetControlsFont");
    }

void CPeninputVkbKrWindow::PopupChoiceList()
    {
    LOG("Vkb.Window.PopupChoiceList");
    }

void CPeninputVkbKrWindow::ReorganizeControls( 
        TInt aClientLayoutId,
        TBool aNeedReset )
    {
    LOG("Vkb.Window.ReorganizeControls");
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
    ReorgnizeTitleBar();
    ReorgnizeTableUpDown();
    
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
                iVkbCtrl->SetTextFormat( iLafMgr->KeyTextLayout() );  //Read laf 
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
    }

void CPeninputVkbKrWindow::DoCaseChange( TInt aNewCase )
    {
    LOG1("Vkb.Window.DoCaseChange %d",aNewCase);
    TInt curCase = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCase ) );            
    
    if ( aNewCase == curCase )
        {
        return;
        }
        
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) ); 
        
    TInt latchFlag = CPeninputDataConverter::AnyToInt
                     (iLayoutContext->RequestData(EAkninputDataTypeLatchedSet));
                            
    if ( latchFlag == 0 && (( curRange == ERangeAccent ) || ( ( curRange == ERangeEnglish ) 
            || ( ( curRange == ERangeNative ) && ( ConfigInfo()->CaseSensitive()) ) ) ) )
        {
        // Set shift and caplock button
        TInt shift;  
        TInt capslock;  
        
        CPeninputDataConverter::ShiftCapslockByCase( aNewCase, 
            shift, capslock );
        
        iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shift );
        //ChangeButtonStatus( shfit, EPeninutWindowCtrlIdShiftBtn );          
        iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslock );        
        //ChangeButtonStatus( capslock, EPeninutWindowCtrlIdCapslockBtn );
        iLayoutContext->SetData( EPeninputDataTypeCase, &aNewCase ); 
        ChangeMergedButtonStatus( shift, capslock );
        
        TRAP_IGNORE( iRangeBar->SetCaseL( curRange, aNewCase ) );
        
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

TBool CPeninputVkbKrWindow::CreateCustomControlL( TInt16 aControlId, TInt32 aImageId )
    {
    //LOG2("Vkb.Window.CreateCustomControlL %d %d",aControlId,aImageId);
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
            
        case EPeninutVkbCtrlIdSymbolTableUp:
            {
            if (!iTableUpButton)
                {
                
                iTableUpButton = CAknFepCtrlRepeatButton::NewL( 
                    UiLayout(), aControlId, EPeninputLayoutVkbEventTableUp, EKeyPageUp, 
                                                  KAknsIIDQsnFrFunctionButtonNormal,
                                                  KAknsIIDQsnFrFunctionButtonPressed,
                                                  KAknsIIDQsnFrFunctionButtonInactive );

                AddControlL(iTableUpButton);
                iTableUpButton->AddEventObserver(UiLayout());
                iTableUpButton->SetResourceId( aImageId );
                }

            iTableUpButton->ConstructFromResourceL();
            }
            break;
        case EPeninutVkbCtrlIdSymbolTableDown:
            {
            if (!iTableDownButton)
                {
                iTableDownButton = CAknFepCtrlRepeatButton::NewL( 
                    UiLayout(), aControlId, EPeninputLayoutVkbEventTableDown, EKeyPageDown, 
                                                  KAknsIIDQsnFrFunctionButtonNormal,
                                                  KAknsIIDQsnFrFunctionButtonPressed,
                                                  KAknsIIDQsnFrFunctionButtonInactive );

                AddControlL(iTableDownButton);
                iTableDownButton->AddEventObserver(UiLayout());
                iTableDownButton->SetResourceId( aImageId );

                }
            iTableDownButton->ConstructFromResourceL();
            }
            break;
        case EPeninutVkbCtrlIdSymbolTablePageLabel:
            {
            if (!iTablePageLabel)
                {
                /*
                iTablePageLabel=CAknFepCtrlCommonButton::NewL(
                    UiLayout(),
                    aControlId,
                    KAknsIIDQsnFrFunctionButtonNormal,
                    KAknsIIDQsnFrFunctionButtonPressed,
                    KAknsIIDQsnFrFunctionButtonInactive);
                */
                iTablePageLabel=CAknFepCtrlEventButton::NewL(
                    UiLayout(),
                    aControlId,
                    0xFFFF,
                    0,
                    KAknsIIDQsnFrFunctionButtonInactive,
                    KAknsIIDQsnFrFunctionButtonInactive,
                    KAknsIIDQsnFrFunctionButtonInactive);
                AddControlL(iTablePageLabel);
                iTablePageLabel->AddEventObserver(UiLayout());
                iTablePageLabel->Hide(EFalse);
                }
            }
            break;
        default:
            {
            create = EFalse;
            }
        }
    return create;
    }

void CPeninputVkbKrWindow::AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl)
    {
    //LOG("Vkb.Window.AddCustomControlGroupL");
    AddControlL(aCtrl);
    }

void CPeninputVkbKrWindow::SetVkbLayoutSize()
    {
    TInt vkbLayoutid = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeVkbLayout ) );
    CPeninputVkbLayoutInfo* vkbLayout = 
        ConfigInfo()->FindVkbLayoutInfo( vkbLayoutid );

    TRect outrect,innerrect;   
    iLafMgr->GetPreviewWndRect(outrect,innerrect);
    
    if (SymbolTableUtil::GetClientLaytoutFor(vkbLayoutid)==EPeninputClientLayoutMacroChars)
        {
        TInt widthOutrect=outrect.Width();
        TInt widthInnerrect=innerrect.Width();
        outrect.SetWidth(widthOutrect*3);
        innerrect.SetWidth(widthInnerrect*3);
        }
    
    iVkbCtrl->SetTextFormat(iLafMgr->PreviewWndText());  //Read laf 
    iVkbCtrl->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    iVkbCtrl->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
                           innerrect.iTl.iY - outrect.iTl.iY,
                           outrect.iBr.iX - innerrect.iBr.iX,
                           outrect.iBr.iY - innerrect.iBr.iY);
    
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

void CPeninputVkbKrWindow::ConstructFromResourceL()
    {               
    CPeninputLayoutWindowExt::ConstructFromResourceL();
    if( iVkbLayout && !iFirstConstruct )
        {
        if( iVkbCtrl->IsIrregularKeyBoard() )       
            iVkbLayout->ConstructFromIrregularResourceL();
        else
            iVkbLayout->ConstructFromNonIrregularResourceL();
        SetVkbLayoutSize();
        }
    iFirstConstruct = EFalse;
    }

// ---------------------------------------------------------------------------
// Internal Functions
// ---------------------------------------------------------------------------
//

void CPeninputVkbKrWindow::SetRangeBarSizeL(TInt x, TInt y)
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

CAknFepCtrlEventButton* CPeninputVkbKrWindow::AddButtonL( 
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
        AddControlL( button );
        button->AddEventObserver(UiLayout());
        
        }
    CleanupStack::Pop( button );  
          
    return button;
    }

void CPeninputVkbKrWindow::ReorgnizeTitleBar()
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
    bkBtn->SizeChanged( exRect, inRect, bBKSizeChanged );


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

    }

void CPeninputVkbKrWindow::ReorgnizeTableUpDown()
    {
    TPoint winPoint = Rect().iTl;
    TPoint LafPoint = iLafMgr->EntirePaneRect().iTl;
    
    TRect inRect;
    TRect exRect;
    
    TInt x = winPoint.iX - LafPoint.iX;
    TInt y = winPoint.iY - LafPoint.iY;    
    
    iLafMgr->GetButtonRect(
        (TPeninputCommonCtrlID)EPeninutVkbCtrlIdSymbolTableUp,
        exRect ,
        inRect);
    exRect.Move(x, y); 
    inRect.Move(x, y);
    if (exRect.IsEmpty())
        {
        iTableUpButton->Hide(ETrue);
        }
    else
        {
        iTableUpButton->Hide(EFalse);
        iTableUpButton->SizeChanged( exRect, inRect, ETrue );
        iTableUpButton->Draw();        
        }

    iLafMgr->GetButtonRect(
        (TPeninputCommonCtrlID)EPeninutVkbCtrlIdSymbolTablePageLabel,
        exRect ,
        inRect);
    exRect.Move(x, y);
    inRect.Move(x, y); 
    
    /* for CButtonBase
    TRgb color( KRgbBlack );  // sane default for nonskinned case
    AknsUtils::GetCachedColor( 
        UiLayout()->SkinInstance(),
        color,
        KAknsIIDQsnTextColors,
        EAknsCIQsnTextColorsCG59);
    iTablePageLabel->SetCaptionColor(color);
    TFontStyle style;
    CFbsFont* font=AknFontAccess::GetClosestFont(
        *BitmapDevice(),
        style,
        BitmapDevice()->VerticalPixelsToTwips(exRect.Height()/3),
        AknFontAccess::EAknFontFamilyNotSpecified);
    iTablePageLabel->SetFont(font);
    */
    /* for Common
    TAknTextLineLayout layout=iLafMgr->KeyTextLayout();
    layout.iFont=ELatinPlain12;
    iTablePageLabel->SetTextFormat(layout);
    iTablePageLabel->SetTextColorIndex(EAknsCIQsnTextColorsCG59);
    */
    /*
    TAknTextLineLayout langSwitch;
    TAknTextLineLayout langSwitchShadow;
    langSwitch = 
        AknLayoutScalable_Avkon::cell_vkb_side_pane_t1().LayoutLine();
    langSwitchShadow = 
        AknLayoutScalable_Avkon::cell_vkb_side_pane_t1_copy1().LayoutLine();
    iTablePageLabel->SetFont(AknLayoutUtils::FontFromId(langSwitch.iFont, NULL));
    iTablePageLabel->SetShadowFont(AknLayoutUtils::FontFromId(langSwitchShadow.iFont, NULL));
    */
    TAknTextLineLayout langSwitch;
    TAknTextLineLayout langSwitchShadow;
    langSwitch = 
        AknLayoutScalable_Avkon::cell_vkb_side_pane_t1().LayoutLine();
    TAknTextLineLayout layout=iLafMgr->KeyTextLayout();
    
    if (iLayoutContext->LayoutType()==EPluginInputModeVkb)
        {
        layout.iFont=langSwitch.iFont;        
        }
    iTablePageLabel->SetTextFormat(layout);
    iTablePageLabel->SetTextColorIndex(EAknsCIQsnTextColorsCG59);
    
    if (exRect.IsEmpty())
        {
        iTablePageLabel->Hide(ETrue);
        }
    else
        {
        iTablePageLabel->Hide(EFalse);
        iTablePageLabel->SizeChanged( exRect, inRect, ETrue );
        iTablePageLabel->Draw();
        }

    iLafMgr->GetButtonRect(
        (TPeninputCommonCtrlID)EPeninutVkbCtrlIdSymbolTableDown,
        exRect ,
        inRect);
    exRect.Move(x, y);
    inRect.Move(x, y); 
    if (exRect.IsEmpty())
        {
        iTableDownButton->Hide(ETrue);
        }
    else
        {
        iTableDownButton->Hide(EFalse);
        iTableDownButton->SizeChanged( exRect, inRect, ETrue );
        iTableDownButton->Draw();
        }
    
    TRect updateArea;
    updateArea.iTl=iTableUpButton->Rect().iTl;
    updateArea.iBr=iTableDownButton->Rect().iBr;
    UpdateArea( updateArea, ETrue );
    }

void CPeninputVkbKrWindow::ResetAccentButton()
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
        
    if ( iRangeBar )
        {
        iRangeBar->ActiveRange( curRange );
        }        
    }

void CPeninputVkbKrWindow::UpdateICFTextL()
    {
    CFepLayoutMultiLineIcf* icf = 
        static_cast<CFepLayoutMultiLineIcf*>(Control(EPeninputWindowCtrlIdMultiLineICF));
    if( icf )
        {
        icf->UpdateSecretTextL();    
        }
    }


TInt CPeninputVkbKrWindow::BackgroundTaskL(TAny* aPtr)
    {
    CPeninputVkbKrWindow* self = static_cast<CPeninputVkbKrWindow*>(aPtr);
    self->DoIdleConstructL();
    return EFalse; // only run once
    }

void CPeninputVkbKrWindow::PopupWindowSizeChange()
    {
    TAknTextLineLayout choiceTextLayout;
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
        
        choiceTextLayout = 
                         AknLayoutScalable_Avkon::list_single_choice_list_pane_t1(1).LayoutLine();
        }
        
    const CFont* font = AknLayoutUtils::FontFromId(choiceTextLayout.iFont, NULL);
    iPopupWnd->SetFont(font);
    iPopupWnd->SetItemRect(iPopupItemRect, iPopupItemRect);

    TInt leftMargin = choiceTextLayout.il;
    
    iPopupWnd->SetMarginAndBaseline(leftMargin, font->HeightInPixels());
    
    iPopupWnd->SetFontColor(AKN_LAF_COLOR_STATIC(choiceTextLayout.iC));

    AknIconUtils::SetSize(iPopupWndBgImg, TSize(iPopupItemRect.Width(), 
                          iPopupItemRect.Height()*3), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(iPopupWndFocusImg, TSize(iPopupItemRect.Width(), 
                          iPopupItemRect.Height()* 3), EAspectRatioNotPreserved);
    
    iPopupSet = ETrue;
    }

void CPeninputVkbKrWindow::DoIdleConstructL()
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

void CPeninputVkbKrWindow::GetPopupWndInfoFromResL( 
        TResourceReader aRes,
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

void CPeninputVkbKrWindow::PopupSwitchWindowL()
    {
    CFepUiBaseCtrl* switchBtn = Control(EPeninutWindowCtrlIdSwitchToHwrBtn);

    if ( switchBtn )
        {
        /*
        if (!iPopupInited)
            {
            DoIdleConstructL();
            }
        TRect rect = switchBtn->Rect();   
        */
        static_cast<CPeninputLayoutInputmodelChoice*>
                (Control(EPeninutWindowCtrlIdSwitcherPopupWindow))->PopUpSwitchListL( switchBtn->Rect() );
        }         
    }

TBool CPeninputVkbKrWindow::IsSymbolRange()
    {
    TInt currentRange = CPeninputDataConverter::AnyToInt(
        iLayoutContext->RequestData(EPeninputDataTypeCurrentRange));
    return currentRange==ERangeSymbol;
    }

void CPeninputVkbKrWindow::FakeClientLayoutUpdateL(const TInt& aVkbLayoutId)
    {
    TInt vkbLayoutToUpdate=aVkbLayoutId;
    CPeninputRangeBarInfo* rangeBarInfo = iConfigInfo->RangeBarInfo();
    CPeninputRangeInfo* rangeInfo = rangeBarInfo->FindRange(ERangeSymbol);
    if (vkbLayoutToUpdate==0)
        {
        vkbLayoutToUpdate = rangeInfo->VkbLayoutId();
        }
    
    // TODO: find better way to handle changing client layout.
    TDummyRangeInfo* dummyRangeInfo=(TDummyRangeInfo*)rangeInfo;
    dummyRangeInfo->iClientLayoutId=
        SymbolTableUtil::GetClientLaytoutFor(vkbLayoutToUpdate);

    if (iTablePageLabel)
        {
        TBuf<50> buf;
        buf.Format(_L("%d/%d"),SymbolTableUtil::FindIndex(vkbLayoutToUpdate)+1,KMaxSymbolTable);
        //iTablePageLabel->SetCaptionL(buf);
        iTablePageLabel->SetTextL(buf);
        iTablePageLabel->Draw();
        }
    }

// ---------------------------------------------------------------------------
// External Functions
// ---------------------------------------------------------------------------
//

void CPeninputVkbKrWindow::SetPromptTextL(TUint8* aData)
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
                    (Control(EPeninputWindowCtrlIdMultiLineICF))->SetPromptTextL(textBuf->Des());
        CleanupStack::PopAndDestroy(textBuf);
        }
    else
        {
        static_cast<CFepLayoutMultiLineIcf*>
                    (Control(EPeninputWindowCtrlIdMultiLineICF))->SetPromptTextL(KNullDesC);
        }

    CleanupStack::PopAndDestroy(dataBuf);
      CleanupStack::PopAndDestroy(&readStream);
    }    

void CPeninputVkbKrWindow::SetSwitchlistSecretFlag(TBool aSecret)
    {
    static_cast<CPeninputLayoutInputmodelChoice*>
            (Control(EPeninutWindowCtrlIdSwitcherPopupWindow))->SetSecretEditor(aSecret);    
    //disable or enable arrow keys of FSQ in secret editor.     
    }

void CPeninputVkbKrWindow::DimArrowKeys( TBool aDimArrow )
    {
    if(iLayoutContext->LayoutType() == EPluginInputModeFSQ 
       && iArrowLeftButton && iArrowRightButton)
        {
        iArrowLeftButton->SetDimmed( aDimArrow );
        iArrowRightButton->SetDimmed( aDimArrow );
        }           
    }

void CPeninputVkbKrWindow::DimEnterKey( TBool aDimmed )
    {
	CAknFepCtrlEventButton* enterBtn = static_cast<CAknFepCtrlEventButton*>
	                              ( Control(EPeninutWindowCtrlIdEnterBtn) );	
    if( iLayoutContext->LayoutType() == EPluginInputModeFSQ 
       && enterBtn )
        {
        enterBtn->SetDimmed( aDimmed );        
        }
    }

void CPeninputVkbKrWindow::ShowBubble(TInt aShow)
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

void CPeninputVkbKrWindow::PreProcessRange(const TInt& aRange)
    {
    CAknFepCtrlMultiModeButton* ctrlShift=
        (CAknFepCtrlMultiModeButton*)ControlById(EPeninutWindowCtrlIdShiftBtn);
    if (aRange==ERangeSymbol)
        {
        TRAP_IGNORE( FakeClientLayoutUpdateL());
        ctrlShift->SetDimmed(ETrue);
        }
    else
        {
        ctrlShift->SetDimmed(EFalse);
        }
    }

void CPeninputVkbKrWindow::UpdateLafData()
    {
    TRAP_IGNORE( iLafMgr->UpdateLafDataL() );
    }
