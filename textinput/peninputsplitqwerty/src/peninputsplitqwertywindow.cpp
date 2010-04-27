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
#include <peninputssqwin.rsg>
#include <peninputssqwinconfiginfo_01.rsg>
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
#include "peninputsplitqwerty.hrh"
#include "peninputsplitqwertywindow.h"
#include "peninputsplitqwertyenum.h"
#include "peninputrangebarinfo.h"
#include "peninputsplitqwertylafdatamgr.h"
#include "peninputsplitqwertylangmeritpair.h"
#include "peninputsplitqwertylayout.h"

#include <peninputaknvkbpreviewbubblerenderer.h>

// Constants
const TInt KPeninputVkbWndInvalidIndex = -1;
const TInt KInvalidImg = -1 ;

_LIT( KVkbWindowResourceFile, "z:\\resource\\plugins\\peninputssqwin.rsc" );
_LIT( KConfigurationResourceFile, "z:\\resource\\plugins\\peninputssqwinconfiginfo_" );
_LIT( KResourceFileExtName, ".rsc" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
// 
CPeninputSplitQwertyWindow* CPeninputSplitQwertyWindow::NewL(
                                        CFepUiLayout* aUiLayout, 
                                        MPeninputLayoutContext* aLayoutContext, 
                                        MPeninputUiStateMgr* aUiStateMgr )
    {
    CPeninputSplitQwertyWindow* 
        self = new ( ELeave ) CPeninputSplitQwertyWindow( aUiLayout,
                                                          aLayoutContext, 
                                                          aUiStateMgr );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyWindow::~CPeninputSplitQwertyWindow()
    {
    delete iIdle;
    iUnits.Close();
    
    delete iLafMgr;
    iLafMgr = NULL;
    delete iBmpRotator;
    
    iAccentCmdList.Close();
    }

// ---------------------------------------------------------------------------
// Get id of window config resource
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyWindow::GetWindowConfigResId()
    {
    return R_PENINPUT_LAYOUT_CONFIG_INFO;
    }

// ---------------------------------------------------------------------------
// Get id of window resource
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyWindow::GetWindowResId()
    {
    return R_PENINPUT_LAYOUT_FSQ_WINDOW_INFO;
    }

// ---------------------------------------------------------------------------
// Get resource id of numeric keymapping
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyWindow::GetNumKeymappingResId()
    {
    return R_PENINPUT_LAYOUT_KEY_MAPPING_STRING_LIST;
    }

// ---------------------------------------------------------------------------
// Get file name of window config resource
// ---------------------------------------------------------------------------
//
const TDesC& CPeninputSplitQwertyWindow::GetWindowConfigResFileName( TInt aLangID )
    {
    if ( aLangID <= 0 ) 
        {
        return KNullDesC;
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

// ---------------------------------------------------------------------------
// Get file name of window resource
// ---------------------------------------------------------------------------
//
const TDesC& CPeninputSplitQwertyWindow::GetWindowResFileName()
    {
    return KVkbWindowResourceFile();
    }

// ---------------------------------------------------------------------------
// Change unit size
// ---------------------------------------------------------------------------
//
const TRect CPeninputSplitQwertyWindow::ChangeUnitSize()
    {
    return TRect();
    }

// ---------------------------------------------------------------------------
// Change size of client area
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::ChangeClientSize()
    {
    if ( !iPopupWnd->Hiden() )
        {
        iPopupWnd->CloseWindow();
        iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
        }
        
    HandlePopupSizeChange();
            
    TInt clientLayoutId = IntContext( EPeninputDataTypeClientLayout );
    CPeninputClientLayoutInfo* 
        clientLayout = ConfigInfo()->FindClientLayoutInfo( clientLayoutId );
        
    if ( clientLayout )
        {
        ReorganizeControls( clientLayout->LayoutID(), EFalse );
        } 

    UpdateArea( iVkbCtrl->Rect(), EFalse );
    }

// ---------------------------------------------------------------------------
// Construct all controls specified in resource
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::CreateAllControlsL()
    {
    // Range bar
    AddRangeBarL();

    // Vkb control
    iVkbCtrl = CPeninputVkbCtrlExt::NewL( TRect(), 
                                          UiLayout(), 
                                          EPeninutWindowCtrlIdVkbCtrl, 
                                          NULL  );
    
    iVkbCtrl->SetResourceId( R_PENINPUT_LAYOUT_VKB_KEYBOARD );
    iVkbCtrl->ConstructFromResourceL();
    iVkbCtrl->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );
    iVkbLayout->SetVkbKeyboard( iVkbCtrl );
    
    iVkbCtrl->SetIrregularKeyBoard( EFalse );
    iVkbLayout->SetNonIrregularResourceId( R_AKN_FEP_VKB_NONIRREGULAR_BITMAP );
    iVkbLayout->ConstructFromNonIrregularResourceL();        

    //todo split-view FSQ, bubble support
    CPeninputAknVkbPreviewBubbleRenderer* renderer = CPeninputAknVkbPreviewBubbleRenderer::NewL();
    iVkbCtrl->SetPreviewBubbleRenderer( renderer );
    iVkbCtrl->ShowBubble( ETrue );    
    iVkbCtrl->SetGowithPointerDown( ETrue );
    iVkbCtrl->SetBubbleBitmapParam( NULL, NULL, KAknsIIDQsnFrInputCharPreview ); 
    iVkbCtrl->SetBubbleSize( TSize(80,80) ); //todo preview-bubble
       
    //set key to be 9 piece graphics
    iVkbCtrl->SetKeySkinId( EKeyBmpNormal, KAknsIIDQsnFrKeypadButtonFrNormal );
    iVkbCtrl->SetKeySkinId( EKeyBmpHighlight, KAknsIIDQsnFrKeypadButtonFrPressed );
    iVkbCtrl->SetKeySkinId( EKeyBmpDim, KAknsIIDQsnFrKeypadButtonFrInactive );
    
    AddControlL( iVkbCtrl );
    iVkbCtrl->AddEventObserver( UiLayout() );
    
    AddButtonL( EPeninutWindowCtrlIdEnterBtn, 
                EPeninputLayoutEventEnter, 
                R_PENINPUT_LAYOUT_VKB_ENTER, 
                EKeyEnter );
       
    // Shift and capslock merged button

    CAknFepCtrlMultiModeButton* button = CAknFepCtrlMultiModeButton::NewLC( 
                                                UiLayout(), 
                                                EPeninutWindowCtrlIdShiftBtn );
    if ( button )
        {
        // Read resource
        TInt resid = R_PENINPUT_LAYOUT_VKB_SHIFT_CAPSLOCK;
        TResourceReader reader;
        CCoeEnv::Static()->CreateResourceReaderLC( reader, resid );
        button->SetResourceId( resid );      
        button->ConstructFromResourceL();  
        
        CleanupStack::PopAndDestroy( 1 ); //reader
        
        AddControlL( button );
        button->AddEventObserver( UiLayout() );
        }
    CleanupStack::Pop( button );  

    
    // Space button          
    CAknFepCtrlEventButton* spaceButton = EventButtonCtrl( 
                                                EPeninutWindowCtrlIdSpaceBtn ); 

    // Create 3-pieces color icon for space button
    CPenInput3PiecesColorIcons* colorIcons = NULL;
    colorIcons = CPenInput3PiecesColorIcons::NewL( R_AKN_FEP_VKB_SPACE_ICON1,
                                                   R_AKN_FEP_VKB_SPACE_ICON2,
                                                   R_AKN_FEP_VKB_SPACE_ICON3 );
    
    spaceButton->Set3PiecesColorIcons( colorIcons );
    
    //set background of space button to be 9 piece graphics
    spaceButton->SetImageFrameId( KAknsIIDQsnFrKeypadButtonFrNormal, 
                                  KAknsIIDQsnFrKeypadButtonFrPressed,
                                  KAknsIIDQsnFrKeypadButtonFrInactive );

    // Add popup window
    AddPopupWindowL();
  
    }

// ---------------------------------------------------------------------------
// Set control fonts. Fonts info comes from resource
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::SetControlsFont()
    {
    }

// ---------------------------------------------------------------------------
// Pop up choice list
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::PopupChoiceList()
    {
    TRAP_IGNORE( PopupRangeListL() );
    }

// ---------------------------------------------------------------------------
// Re-organize all controls in the assigned client area layout
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::ReorganizeControls( TInt aClientLayoutId,
                                                     TBool aNeedReset )
    {
    CPeninputClientLayoutInfo* clientLayout = 
        ConfigInfo()->FindClientLayoutInfo( aClientLayoutId ); 
    RPointerArray<CPeninputControlInfo>& controlList = 
        clientLayout->ControlInfoList();
  
    const TInt keyColumns = clientLayout->Columns();
    const TInt keyRows = clientLayout->Rows();
    
    if ( iLastUsedTotalColumns == keyColumns && iLastUsedTotalRows == keyRows )
        {
        return;
        }    

    iLafMgr->SetLayoutType(
                iLafMgr->LayoutTypeFromRowsColumns( keyRows, keyColumns )
                );
    
    iLastUsedTotalColumns = keyColumns;
    iLastUsedTotalRows = keyRows;
    
    TRect winRect( Rect().iTl, iLafMgr->EntirePaneRect().Size() );        
    SetWindowRect( winRect );    
    UiLayout()->LayoutOwner()->SetPosition( iLafMgr->EntirePaneRect().iTl );
    
    ReorgnizeTitleBar();
    
    const TInt count = controlList.Count();
    TInt controlID = 0;
    CFepUiBaseCtrl* ctrl = NULL;
    TRect rect;
    TRect inRect;
    TRect exRect;

    TPoint winPoint = Rect().iTl;
    TPoint LafPoint = iLafMgr->EntirePaneRect().iTl;
    
    for ( TInt i = 0; i < count; i++ )
        {
        controlID = controlList[i]->ControlID();
        ctrl = Control( controlID );

        if ( !ctrl )
            {
            continue;
            }
        
        AddNotOwnedControl( ctrl );
        
        if ( ( ctrl->ControlType() | ECtrlButton ) == ECtrlButton )
            {
            
            iLafMgr->GetButtonRect( 
                            TPeninputCommonCtrlID(ctrl->ControlId() ), 
                            exRect, inRect );
            
            if( controlID ==  EPeninutWindowCtrlIdShiftBtn)
                {
                CAknFepCtrlMultiModeButton* btn = NULL;
                btn = static_cast<CAknFepCtrlMultiModeButton*>(ctrl);
                btn->SizeChanged( exRect, inRect, ETrue );
                }
            else
                {
                CAknFepCtrlCommonButton* btn = NULL;
                btn = static_cast<CAknFepCtrlCommonButton*>(ctrl);
                
                if ( ctrl->ControlId() == EPeninutWindowCtrlIdSpaceBtn )
                    {
                    // config 3 pieces icon info       
                    TRect frameRect, middleRect;
                    iLafMgr->GetSpaceButtonExLayoutInfo( frameRect, middleRect );
                    inRect = frameRect;
                    
                    TDraw3PiecesFrame pic3frame( 
                            KAknsIIDQgnIndiInputSpaceL,
                            KAknsIIDQgnIndiInputSpaceMiddle,
                            KAknsIIDQgnIndiInputSpaceR,
                            middleRect);
                    
                    btn->SetDraw3PieceFrameInfo( pic3frame );
                    }
                
                btn->SizeChanged( exRect, inRect, ETrue );
                }
            }
        
        else if ( ctrl->ControlId() == EPeninutWindowCtrlIdVkbCtrl )
            {
            iVkbCtrl->SetFont( iLafMgr->KeyTextFont() );
            iVkbCtrl->SetTextLineLayout( iLafMgr->KeyTextLayout() );
            iVkbCtrl->SetTextFormat( iLafMgr->PreviewWndText() );  //Read laf 
            if ( aNeedReset )
                {
                iVkbCtrl->Reset();
                }

            rect = iLafMgr->KeypadRect();
            CPeninputVkbCtrlExt* ctrlext = static_cast<CPeninputVkbCtrlExt*>(ctrl);
            ctrlext->SizeChanged(rect);
            }  
        }  
    }

// ---------------------------------------------------------------------------
// Do when case changed
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::DoCaseChange( TInt aNewCase )
    {
    TInt curCase = IntContext( EPeninputDataTypeCase );
    
    if ( aNewCase == curCase )
        {
        return;
        }
        
    TInt curRange = IntContext( EPeninputDataTypeCurrentRange ); 
                               
    if ( ( curRange == ERangeEnglish ) 
         || ( curRange == ERangeAccent ) 
         || ( curRange == ERangeNative && ConfigInfo()->CaseSensitive() ) 
       )
        {
        // Set shift and caplock button
        TInt shfit;  
        TInt capslock;  
        
        CPeninputDataConverter::ShiftCapslockByCase( aNewCase, shfit, capslock );

        iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shfit ); 
        iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslock );        
        iLayoutContext->SetData( EPeninputDataTypeCase, &aNewCase ); 
        ChangeMergedButtonStatus( shfit, capslock );
        
        TInt vkblayout = ConfigInfo()->ShiftCapsSingleVkbLayoutByCase( curRange, 
                                                                       aNewCase );

        if ( curRange == ERangeAccent ) 
           {
           TInt curAccentIndex = IntContext( EPeninputDataTypeCurrentAccent );
           vkblayout = vkblayout + curAccentIndex * 2;
           }
        
        ChangeVkbLayout( vkblayout );
        }
    }

// ---------------------------------------------------------------------------
// Handle control event
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::HandleControlEvent( TInt aEventType,
                                                    CFepUiBaseCtrl* aCtrl, 
                                                    const TDesC& aEventData )
    {
    // Call this function in base class
    CPeninputLayoutWindowExt::HandleControlEvent( aEventType, aCtrl, 
                                                  aEventData );
    
    TInt* data = (TInt*) aEventData.Ptr(); 
    TInt ctrlId = aCtrl ? aCtrl->ControlId() : -1;
    
    switch ( aEventType )
        {
        case EEventButtonUp:
            {
            if ( ctrlId == EPeninutWindowCtrlIdRangeBtn )
                {
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
                iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
                }
            }
            break;
        case EEventChoiceSelected:
            {
            CFepLayoutChoiceList::SEvent* event = NULL;
            event = (CFepLayoutChoiceList::SEvent*) aEventData.Ptr();

            if ( event->iIndex != KPeninputVkbWndInvalidIndex )
                {
                if ( ctrlId == EPeninutWindowCtrlIdAccentPopupWindow )
                    {
                    TRAP_IGNORE( HandleRangeListSelectedL( aCtrl, aEventData ) );
                    }
                }
            iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStateStandby );
            }
            break;
        case EEventVirtualKeyLatched:
            {
            HandleVirtualKeyLatchedEvent( aEventType, aCtrl, aEventData );
            }
            break;
        case EEventVirtualKeyUnLatched:
            {
            UiLayout()->SignalOwner( ESignalKeyEvent, iNewDeadKeyBuf );
            TBool deadKeyChange = EFalse;
            iLayoutContext->SetData( EAkninputDataTypeLatchedSet, &deadKeyChange );
            iNewDeadKeyBuf = KNullDesC;
            }
            break;
        case EPeninputLayoutEventBack:
            {
            TInt latchedFlag = IntContext( EAkninputDataTypeLatchedSet );
            // If one DeadKey is latched, 
            // only unlatch the DeadKey and reset the iNewDeadKeyBuf
            // Don't delete the visible charater in ICF
            if ( latchedFlag )
                {
                UnLatchDeadKey(iNewDeadKeyBuf);

                TBool deadKeyChange = EFalse;
                iLayoutContext->SetData( EAkninputDataTypeLatchedSet, &deadKeyChange );
                iNewDeadKeyBuf = KNullDesC;
                }
            }
            break;
        case EPeninputLayoutEventEnter:
        case EPeninputLayoutEventSpace:
            {
            TInt latchedFlag = IntContext( EAkninputDataTypeLatchedSet );
            if ( latchedFlag )
                {
                TInt length = aEventData.Length() + iNewDeadKeyBuf.Length();
                HBufC* newCharBuf = HBufC::New( length );
                if ( newCharBuf )
                    {
                    newCharBuf->Des().Append( iNewDeadKeyBuf );
                    newCharBuf->Des().Append( aEventData );
                    }                
                
                UnLatchDeadKey( iNewDeadKeyBuf );

                TBool deadKeyChange = EFalse;
                iLayoutContext->SetData( EAkninputDataTypeLatchedSet, &deadKeyChange );
                
                // Submit DeadKey + Space or Enter at the same time
                
                if( newCharBuf != NULL )
                    {
                    UiLayout()->SignalOwner( ESignalKeyEvent, *newCharBuf );
                    }
                iNewDeadKeyBuf = KNullDesC;
                delete newCharBuf;
                }
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// load settings from resource
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::ConstructFromResourceL()
    {                 
    CPeninputLayoutWindowExt::ConstructFromResourceL();
    if ( iVkbLayout && !iFirstConstruct )
        {
        if ( iVkbCtrl->IsIrregularKeyBoard() )
            {
            iVkbLayout->ConstructFromIrregularResourceL();
            }
        else
            {
            iVkbLayout->ConstructFromNonIrregularResourceL();
            }
        
        SetVkbLayoutSize();
        
        if ( IsRtoLLanguage( iLanguage ) )
            {
            //Mirror backspace graphic
            HandleButtonResOnLangDirChange( EPeninutWindowCtrlIdBackspaceBtn );
            }
        }
        
    iFirstConstruct = EFalse;
    }

// ---------------------------------------------------------------------------
// Callback function to do background construction
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyWindow::BackgroundTaskL( TAny* aPtr )
    {
    CPeninputSplitQwertyWindow* self = static_cast<CPeninputSplitQwertyWindow*>(aPtr);
    self->DoIdleConstructL();
    return EFalse; // only run once
    }

// ---------------------------------------------------------------------------
// Set current language
// ---------------------------------------------------------------------------
//
void  CPeninputSplitQwertyWindow::SetSwitchlistLanguage(TInt aLang)
    {
    ASSERT( IsValidLanguage( aLang ) );
    
    //Check language input direction change
    TBool isLangDirectionChange = EFalse;
    if ( IsValidLanguage( iLanguage ) )
        {
        isLangDirectionChange =  IsRtoLLanguage( aLang ) ^ IsRtoLLanguage( iLanguage );
        }
    else
        {
        isLangDirectionChange = IsRtoLLanguage( aLang );
        }
    
    TPeninputVkbLafOption option = iLafMgr->OptionType();
    if ( aLang ==ELangHebrew || aLang == ELangGreek || aLang == ELangHindi 
                                                    || aLang == ELangMarathi ) 
        {
        option = (TPeninputVkbLafOption)( option & ( ~EOption3CellRange ) );
        iLafMgr->SetLafOption( option );    
        }
    else
        {
        option = (TPeninputVkbLafOption)( option | EOption3CellRange );
        iLafMgr->SetLafOption( option );    
        }
            
    if( isLangDirectionChange )
        {
        //Mirror backspace graphic
        HandleButtonResOnLangDirChange( EPeninutWindowCtrlIdBackspaceBtn );
        }
            
    iLanguage = aLang;    
    }

// ---------------------------------------------------------------------------
// Set current editor is secret
// ---------------------------------------------------------------------------
//    
void  CPeninputSplitQwertyWindow::SetSwitchlistSecretFlag( TBool aSecret )
    {
    CFepUiBaseCtrl* ctrl = Control( EPeninutWindowCtrlIdSwitcherPopupWindow );
    CPeninputLayoutInputmodelChoice* choice =
                        static_cast<CPeninputLayoutInputmodelChoice*>( ctrl );
    if ( choice )
        {
        choice->SetSecretEditor( aSecret );    
        }
    //disable or enable arrow keys of FSQ in secret editor.
    }

// ---------------------------------------------------------------------------
// Dim the arrow keys on qwerty when the editor is findpane or secret editor
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::DimArrowKeys( TBool aDimArrow )
    {
    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ 
         && iArrowLeftButton && iArrowRightButton )
        {
        iArrowLeftButton->SetDimmed( aDimArrow );
        iArrowRightButton->SetDimmed( aDimArrow );
        }
    }

// ---------------------------------------------------------------------------
// Handle accented char event
// ---------------------------------------------------------------------------
//     
TBool CPeninputSplitQwertyWindow::HandleAccentCharEvent( TInt /*aEventType*/, 
                                                    const TDesC& aEventData, 
                                                    const TDesC& aDeadKey, 
                                                    HBufC* anewCharBuf )
    {
    // Get the unicode of virtual key char
    TBuf<16> buf;
    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
    buf.Append( event->iCode );
    TBool rs = EFalse;
    
    // Caculate the accent char number
    TInt count = sizeof(KSupportLanguages)/sizeof(TLangMeritPair);

    if ( aDeadKey.Compare( KNullDesC ) != 0 ) // One DeadKey is latched
        {
        for ( TInt i = 0; i < count; i++ )
            {
            if ( ( aDeadKey[0] == KSupportLanguages[i].iDeadKey ) && 
                 ( buf[0] == KSupportLanguages[i].iChar) )
                {
                if ( anewCharBuf )
                    {
                    anewCharBuf->Des().Append( KSupportLanguages[i].iAccentedChar );
                    rs = ETrue;
                    }
                break;
                }
            }
        }
    return rs;
    }

// ---------------------------------------------------------------------------
// Handle virtual key latched event
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::HandleVirtualKeyLatchedEvent(TInt /*aEventType*/, 
                                                         CFepUiBaseCtrl* /*aCtrl*/, 
                                                         const TDesC& aEventData )
    {
    TBuf<16> deadKey(KNullDesC);
    TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
    deadKey.Append(event->iCode);
        
    TInt latchedFlag = IntContext( EAkninputDataTypeLatchedSet );
    if ( latchedFlag )
        {
        // Update the new and old DeadKey buffer
        iOldDeadKeyBuf = iNewDeadKeyBuf;
        iNewDeadKeyBuf = deadKey;

        // When type another DeadKey, submit the previous one.
        UiLayout()->SignalOwner( ESignalKeyEvent, iOldDeadKeyBuf );
        // Unlatch the previous DeadKey
        UnLatchDeadKey(iOldDeadKeyBuf);
        }
    else
        {
        iNewDeadKeyBuf = deadKey;
        
        // Set DeadKey state to latched
        TBool deadKeyChange = ETrue;
        iLayoutContext->SetData( EAkninputDataTypeLatchedSet, &deadKeyChange );
        }
   
    // Send the char to FEP
    // Not submit the DeadKey when latch it.
//    UiLayout()->SignalOwner( ESignalKeyEvent, iNewDeadKeyBuf );
    }

// ---------------------------------------------------------------------------
// Handle virtual key up event
// ---------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyWindow::HandleVirtualKeyUpEvent(TInt aEventType, 
                                                    CFepUiBaseCtrl* /*aCtrl*/, 
                                                    const TDesC& aEventData )
    {
    // Get DeadKey state
    TInt latchedFlag = IntContext( EAkninputDataTypeLatchedSet );
    if ( latchedFlag )
        {
        TBool ret = EFalse;
        
        TRAP_IGNORE( ret = HandleDeadKeyL( aEventType, aEventData ) );
        
        if ( ret )
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

// ---------------------------------------------------------------------------
// Set dead key
// ---------------------------------------------------------------------------
//     
void CPeninputSplitQwertyWindow::SetDeadKey()
    {
    // Get VKB list
    RPointerArray<CPeninputVkbLayoutInfo> vkbListInfo;
    vkbListInfo = iVkbLayout->VkbLayoutInfoList();
    TInt vkbListNum = vkbListInfo.Count();
    
    CPeninputVkbKeyInfo* keyinfo;
    TInt count = sizeof(KSupportLanguages)/sizeof(TLangMeritPair);
    for ( TInt i = 0; i < vkbListNum; i++ )
        {
        for ( TInt j = 0; j < count; j++ )
            {
            if ( vkbListInfo[i]->LayoutID() == EPeninputVkbLayoutNativeChar ||
                 vkbListInfo[i]->LayoutID() == EPeninputVkbLayoutNativeCharShift )
                {
                keyinfo = vkbListInfo[i]->FindKey( KSupportLanguages[j].iDeadKey );
                if ( keyinfo )
                    {
                    // Set the DeadKey as a latch button
                    CVirtualKey* key = keyinfo->Key();
                    key->SetLatchKey( ETrue );
                    }
                }
            }
        }
    }
    
    
// ---------------------------------------------------------------------------
// Unlatch a DeadKey by the key buffer
// ---------------------------------------------------------------------------
//     
void CPeninputSplitQwertyWindow::UnLatchDeadKey( const TDesC& abuf )
    {
    RPointerArray<CPeninputVkbLayoutInfo> vkbListInfo;
    vkbListInfo = iVkbLayout->VkbLayoutInfoList();
    TInt vkbListNum = vkbListInfo.Count();
    
    CPeninputVkbKeyInfo* keyinfo;
    TInt count = sizeof(KSupportLanguages)/sizeof(TLangMeritPair);
    for ( TInt i = 0; i < vkbListNum; i++ )
        {
        for ( TInt j = 0; j < count; j++ )
            {
            // Only NativeChar and NativeCharShift layout contain DeadKey
            if(vkbListInfo[i]->LayoutID() == EPeninputVkbLayoutNativeChar ||
               vkbListInfo[i]->LayoutID() == EPeninputVkbLayoutNativeCharShift)
                {
                // Find the DeadKey in NativeChar or NativeCharShift layout
                keyinfo = vkbListInfo[i]->FindKey( KSupportLanguages[j].iDeadKey );
                if ( keyinfo )
                    {
                    CVirtualKey* key = keyinfo->Key();
                    if ( key->Latched() )
                        {
                        if ( abuf == KNullDesC )
                            {
                            // When focus change, the latched key will be released
                            TBool deadKeyChange = EFalse;
                            iLayoutContext->SetData( EAkninputDataTypeLatchedSet, 
                                                     &deadKeyChange );
                            key->SetLatched( EFalse );
                            TRect rect = key->Rect();
                            rect.Move( iVkbCtrl->Rect().iTl );
                            UpdateArea( rect, EFalse ); 
                            return;
                            }
                        if ( abuf[0] == KSupportLanguages[j].iDeadKey )
                            {
                            // When another DeadKey is clicked, the old one will be released
                            key->SetLatched( EFalse );
                            TRect rect = key->Rect();
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
// Update laf data
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::UpdateLafData()
    {
    TRAP_IGNORE( iLafMgr->UpdateLafDataL() );
    }

// ---------------------------------------------------------------------------
// Load accent labels for range list
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::ConstructAccentListL( TInt aLangId )
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

// ---------------------------------------------------------------------------
// Load range labels for range list
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::ConstructRangeLabelListL( TInt /*aLangId*/ )
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
// Show or hide preview bubble
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::ShowBubble( TInt aShow )
    {
    iVkbCtrl->ShowBubble( aShow > 0 ? ETrue : EFalse ); 
    }

// ---------------------------------------------------------------------------
// Update state of range button
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::UpdateRangeCtrlsL()
    {
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
        
    iRangeButton->SetResourceId( resId );
    iRangeButton->ConstructFromResourceL();
    iRangeButton->SetDimmed( count < 1 );    
    }

// ---------------------------------------------------------------------------
// Update layout position and size
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::UpdateLayoutPosAndSize()
    {
    if ( iLafMgr )
        {
        TRect rect = iLafMgr->ClientRect();
        TPoint pos = iLafMgr->EntirePaneRect().iTl;
        
        UiLayout()->SetRect( rect );
        UiLayout()->LayoutOwner()->SetPosition( pos );
        }
    }

// ---------------------------------------------------------------------------
// Set laf layout type
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::SetLafLayOut( TInt aRows, TInt aColumns )
    {
    iLafMgr->SetLayoutType( 
                iLafMgr->LayoutTypeFromRowsColumns( aRows, aColumns )
                );
    }

// ---------------------------------------------------------------------------
// Create custom control
// ---------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyWindow::CreateCustomControlL( TInt16 aControlId, 
                                                        TInt32 aImageId )
    {
    TBool create = ETrue;
    switch ( aControlId )
        {
        case EPeninupWindowCtrlIdArrowLeftBtn:
            {
            if ( iLayoutContext->LayoutType() != EPluginInputModeFSQ )
                {
                break;
                }

            if ( !iArrowLeftButton )
                {
                iArrowLeftButton = CAknFepCtrlRepeatButton::NewL( 
                                          UiLayout(), 
                                          aControlId, 
                                          EPeninputLayoutEventLeftArrow, 
                                          EKeyLeftArrow, 
                                          KAknsIIDQsnFrFunctionButtonNormal,
                                          KAknsIIDQsnFrFunctionButtonPressed,
                                          KAknsIIDQsnFrFunctionButtonInactive );

                AddControlL( iArrowLeftButton );
                iArrowLeftButton->AddEventObserver( UiLayout() );
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

            if ( !iArrowRightButton )
                {
                iArrowRightButton = CAknFepCtrlRepeatButton::NewL( 
                                          UiLayout(), 
                                          aControlId, 
                                          EPeninputLayoutEventRightArrow, 
                                          EKeyRightArrow, 
                                          KAknsIIDQsnFrFunctionButtonNormal,
                                          KAknsIIDQsnFrFunctionButtonPressed,
                                          KAknsIIDQsnFrFunctionButtonInactive );

                AddControlL( iArrowRightButton );
                iArrowRightButton->AddEventObserver( UiLayout() );
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

// ---------------------------------------------------------------------------
// Add custom control to this group
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::AddCustomControlGroupL( CFepUiBaseCtrl* aCtrl )
    {
    AddControlL( aCtrl );
    }    

// ---------------------------------------------------------------------------
// Set layout size
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::SetVkbLayoutSize()
    {
    TRect outrect,innerrect;   
    iLafMgr->GetPreviewWndRect( outrect, innerrect );
    
    iVkbCtrl->SetTextFormat( iLafMgr->PreviewWndText() );  //Read laf 
    
    TSize bubbleSize( outrect.Width(), outrect.Height() );
    iVkbCtrl->SetBubbleSize( bubbleSize );  // Read laf
    
    iVkbCtrl->SetFrameDiff( innerrect.iTl.iX - outrect.iTl.iX,
                            innerrect.iTl.iY - outrect.iTl.iY,
                            outrect.iBr.iX - innerrect.iBr.iX,
                            outrect.iBr.iY - innerrect.iBr.iY );
    
    TInt vkbLayoutid = IntContext( EPeninputDataTypeVkbLayout );
    CPeninputVkbLayoutInfo* vkbLayout = ConfigInfo()->FindVkbLayoutInfo( 
                                                                 vkbLayoutid );

    // Calculate current vkblayout according current layout
    if ( vkbLayout )
        {
        const TInt columns = vkbLayout->Columns();
        const TInt rows = vkbLayout->Rows();
        TRect keyRect;
        TRect charRect;
        
        //iLafMgr->SetLayoutType(iLafMgr->LayoutTypeFromRowsColumns(rows,columns));
 
        for ( TInt exter = 0; exter < rows; exter++ )
            {
            for ( TInt inter = 0; inter < columns; inter++ )
                {
                iLafMgr->GetKeyRect( inter, exter, keyRect, charRect );
                vkbLayout->SetKeyInfo( exter, inter, keyRect, charRect,
                                       iLafMgr->KeyTextFont() );
                }
            }
     
        if ( iVkbCtrl->IsIrregularKeyBoard() )
            {
            CFbsBitmap* bitmap = iVkbCtrl->IrregularKeyBitmap( EKeyLeftDown,
                                                              EKeyStateNormal );
            
            TSize curSize = vkbLayout->KeyInfoList()[0]->Key()->Rect().Size();
            TSize size = bitmap->SizeInPixels();
            if ( curSize != size )
                {
                for ( TInt i = 0; i <= EKeyRightUp; i++ )
                    {
                    for ( TInt j = 0; j <= EKeyStateDimmedMask; j++ )
                        {
                        bitmap = iVkbCtrl->IrregularKeyBitmap(
                                              (TVirtualKeyIrregularKeyType)i,
                                              (TVirtualKeyIrregularKeyState)j );
                        
                        AknIconUtils::SetSize( bitmap, curSize, 
                                               EAspectRatioNotPreserved ); 

                        }
                    }
                }
            }
        else
            {
            CFbsBitmap* bitmap = iVkbCtrl->NonIrregularKeyBitmap( EKeyBmpNormal );
            TSize curSize = vkbLayout->KeyInfoList()[0]->Key()->Rect().Size();
            TSize size = bitmap->SizeInPixels();
            if( curSize != size )    
                {
                for ( TInt i = 0; i <= EKeyBmpLastType; i++ )
                    {
                    bitmap = iVkbCtrl->NonIrregularKeyBitmap( (TVirtualKeyBmpType)i );
                    if( bitmap )
                        {
                        AknIconUtils::SetSize( bitmap, curSize, 
                                               EAspectRatioNotPreserved );
                        }
                    }
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyWindow::CPeninputSplitQwertyWindow( 
                                        CFepUiLayout* aUiLayout, 
                                        MPeninputLayoutContext* aLayoutContext, 
                                        MPeninputUiStateMgr* aUiStateMgr )
    : CPeninputLayoutWindowExt( aUiLayout, aLayoutContext )
    , iUiStateMgr( aUiStateMgr )
    , iPopupInited( EFalse )
    , iPopupSet( EFalse )
    , iFirstConstruct( ETrue )
    {
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::ConstructL()
    {
    iBmpRotator = CPeninputSyncBitmapRotator::NewL();
    iLafMgr = CPeninputSplitQwertyLafMgr::NewL();
    iLafMgr->SetInputModeL( TPluginInputMode( iLayoutContext->LayoutType() ) );
    CPeninputLayoutWindowExt::ConstructL();   
    }

// ---------------------------------------------------------------------------
// Add range bar into client area
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::AddRangeBarL()
    {
    CAknFepCtrlCommonButton* button = CAknFepCtrlCommonButton::NewL(
                                        UiLayout(), 
                                        EPeninutWindowCtrlIdRangeBtn,
                                        KAknsIIDQsnFrFunctionButtonNormal,
                                        KAknsIIDQsnFrFunctionButtonPressed,
                                        KAknsIIDQsnFrFunctionButtonInactive );

    CleanupStack::PushL( button ); 
    button->SetResourceId( R_PENINPUT_LAYOUT_VKB_RANGE_DEFAULT );
    button->ConstructFromResourceL();
    AddControlL( button );
    CleanupStack::Pop( button );
    
    button->AddEventObserver( UiLayout() );  
    iRangeButton = button;  
    }

// ---------------------------------------------------------------------------
// Add button to client area
// ---------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputSplitQwertyWindow::AddButtonL( 
                                                    const TInt aControlId, 
                                                    const TInt aEventId, 
                                                    const TInt aResId, 
                                                    const TInt aUnicode, 
                                                    const TBool aIsRepeat )
    {
    CAknFepCtrlEventButton* button = NULL;
    
    if ( aIsRepeat )
        {
        button = CAknFepCtrlRepeatButton::NewL( UiLayout(), 
                                                aControlId, 
                                                aEventId, 
                                                aUnicode );
        }
    else
        {
        button = CAknFepCtrlEventButton::NewL( UiLayout(), 
                                               aControlId, 
                                               aEventId, 
                                               aUnicode );
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

// ---------------------------------------------------------------------------
// Add popup window to display accented char list
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::AddPopupWindowL()
    {
    iPopupWnd = CPeninputCommonChoiceList::NewL( 
                                        UiLayout(), 
                                        EPeninutWindowCtrlIdAccentPopupWindow,
                                        KAknsIIDQsnFrList,
                                        KAknsIIDQsnFrPopupSub );

    iPopupWnd->AddEventObserver( UiLayout() );
    
    AddControlL( iPopupWnd );
    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    iIdle ->Start( TCallBack( BackgroundTaskL, this ) );
    }

// ---------------------------------------------------------------------------
// Reorgnize position and size of function buttons
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::ReorgnizeTitleBar()
    {
    TRect inner, bound;
    
    CAknFepCtrlEventButton* evtButton = NULL;
    CAknFepCtrlCommonButton* cmmButton = NULL;
    TBool bBKSizeChanged = EFalse;
       
    evtButton = EventButtonCtrl( EPeninutWindowCtrlIdOptionBtn );
    if ( evtButton )
        {
        iLafMgr->GetButtonRect(EPeninutWindowCtrlIdOptionBtn, bound, inner );
        evtButton->SizeChanged( bound, inner, ETrue );
        }

    cmmButton = CommonButtonCtrl( EPeninutWindowCtrlIdCloseBtn );
    if ( cmmButton )
        {
        iLafMgr->GetButtonRect(EPeninutWindowCtrlIdCloseBtn, bound ,inner );
        cmmButton->SizeChanged( bound, inner, ETrue );
        }
    
    cmmButton = CommonButtonCtrl( EPeninutWindowCtrlIdBackspaceBtn );
    if ( cmmButton )
        {
        iLafMgr->GetButtonRect( EPeninutWindowCtrlIdBackspaceBtn, bound, inner );

        if( bound.Size() != cmmButton->Rect().Size() &&
            inner.Size() != cmmButton->ForgroundBmpRect().Size() )
            {
            bBKSizeChanged = ETrue;
            }    
        cmmButton->SizeChanged( bound, inner, ETrue );
        }

    cmmButton = CommonButtonCtrl( EPeninutWindowCtrlIdSpaceBtn );
    if ( cmmButton )
        {
        iLafMgr->GetButtonRect(EPeninutWindowCtrlIdSpaceBtn, bound, inner );
        inner = bound;
        inner.Shrink( 10, 0 );
        
        cmmButton->SizeChanged( bound, inner, ETrue );
            
        TRect spaceIconRect = inner;
        spaceIconRect.Shrink( 10, 0 );
        TDraw3PiecesFrame draw3pic( KAknsIIDQgnIndiInputSpaceL,
                                    KAknsIIDQgnIndiInputSpaceMiddle,
                                    KAknsIIDQgnIndiInputSpaceR,
                                    spaceIconRect);
        cmmButton->SetDraw3PieceFrameInfo( draw3pic );
        }
    
    if ( iRangeButton )
        {
        iLafMgr->GetButtonRect( EPeninutWindowCtrlIdRangeBtn, bound, inner );
        iRangeButton->SetRect( bound );
        iRangeButton->SizeChanged( bound, inner, ETrue );
        }  
    
    if ( iArrowLeftButton )
        {
        iLafMgr->GetButtonRect( EPeninupWindowCtrlIdArrowLeftBtn, bound, inner );
        iArrowLeftButton->SizeChanged( bound, inner, ETrue );
        }
    if ( iArrowRightButton )
        {
        iLafMgr->GetButtonRect( EPeninupWindowCtrlIdArrowRightBtn, bound, inner );
        iArrowRightButton->SizeChanged( bound, inner, ETrue );
        }  

    //If backspace size changed when language is RtoL mirror should be used    
    if ( bBKSizeChanged && IsRtoLLanguage( iLanguage ) )
        {
        //Mirror backspace graphic
        HandleButtonResOnLangDirChange( EPeninutWindowCtrlIdBackspaceBtn );
        }   
    }


// ---------------------------------------------------------------------------
// Show range list
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::PopupRangeListL()
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
    
    CFepUiBaseCtrl* btn = Control( EPeninutWindowCtrlIdRangeBtn );    
    iPopupWnd->SetListColumnNumWithLafL( iPopupWnd->ItemsCount() );    
    iPopupWnd->SetCurrentFocusedIndex( - 1 ); //no active item
    iPopupWnd->Display( btn->Rect() );
    iUiStateMgr->SetCurrentUiState( EPeninputVkbUiStatePopup );    
    }

// ---------------------------------------------------------------------------
// get resource info of accented chars popup window
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::GetPopupWndInfoFromResL( TResourceReader aRes,
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

        AknIconUtils::SetSize( iPopupWndFocusImg, 
                               TSize( aRect.Width(),aRect.Height() * 3 ), 
                               EAspectRatioNotPreserved );
        
        iPopupWnd->SetFocusBmp( iPopupWndFocusImg );
        }
    }

// ---------------------------------------------------------------------------
// Background constructor
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::DoIdleConstructL()
    {   
    if( iPopupInited )
    {
    return;
    }

    // Read resource
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, 
                                         R_PENINPUT_LAYOUT_VKB_POPUP_WINDOW ); 
        
    GetPopupWndInfoFromResL( reader, iPopupItemRect );
       
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );

    }

// ---------------------------------------------------------------------------
// Simulate raw event
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::SimulateRawEvent( TInt aScanCode, 
                                                   TRawEvent::TType aType )
    {
    TRawEvent event; 
    event.Set( aType, aScanCode );
    
    TPtrC eventPtr( reinterpret_cast<const TUint16*>(&event),
                    sizeof(event)/sizeof(TUint16) );

    UiLayout()->SignalOwner(ESignalSimulateEvent,eventPtr);
    }


// ---------------------------------------------------------------------------
// Handle size changed for range list
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::HandlePopupSizeChange()
    {
    if ( IntContext( EAkninputDataTypeSizeChanging ) || (!iPopupSet) )
        {
        // if size changing, or popup window's property has not been set yet
        TAknWindowLineLayout appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
        iAppWndRect = appWnd.Rect();
        
        TAknWindowLineLayout mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
        TAknLayoutRect mainPaneRect;
        mainPaneRect.LayoutRect( iAppWndRect, mainPane );
        
        TAknWindowLineLayout choiceWnd = AknLayoutScalable_Avkon::popup_choice_list_window(1).LayoutLine();
        TAknLayoutRect choiceRect;
        choiceRect.LayoutRect( mainPaneRect.Rect(), choiceWnd );
        
        iPopupItemRect = TRect( 0, 0, choiceRect.Rect().Width(), 
                         AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine().iH );
        
        iChoiceTextLayout = 
                         AknLayoutScalable_Avkon::list_single_choice_list_pane_t1(1).LayoutLine();
        }
        
    const CFont* font = AknLayoutUtils::FontFromId( iChoiceTextLayout.iFont, NULL );
    iPopupWnd->SetFont( font );
    iPopupWnd->SetItemRect( iPopupItemRect, iPopupItemRect );

    TInt leftMargin = iChoiceTextLayout.il;
    
    iPopupWnd->SetMarginAndBaseline( leftMargin, font->HeightInPixels() );
    
    iPopupWnd->SetFontColor( AKN_LAF_COLOR_STATIC( iChoiceTextLayout.iC ) );

    TSize imgSize ( iPopupItemRect.Width(), iPopupItemRect.Height() * 3 );
    AknIconUtils::SetSize( iPopupWndBgImg, imgSize, EAspectRatioNotPreserved );
    AknIconUtils::SetSize(iPopupWndFocusImg, imgSize, EAspectRatioNotPreserved );
    
    iPopupSet = ETrue;
    }


// ---------------------------------------------------------------------------
// Handle dead key event
// ---------------------------------------------------------------------------
//
 TBool CPeninputSplitQwertyWindow::HandleDeadKeyL( TInt aEventType, 
                                                   const TDesC& aEventData )
     {
    // Set DeadKey to unlatched state
    TBool deadKeyChange = EFalse;
    iLayoutContext->SetData( EAkninputDataTypeLatchedSet, &deadKeyChange );
    UnLatchDeadKey( iNewDeadKeyBuf );

    // Get the accent char
    HBufC* newCharBuf = HBufC::NewL( 1 );
    
    TBool handled = HandleAccentCharEvent( aEventType, 
                                           aEventData, 
                                           iNewDeadKeyBuf, 
                                           newCharBuf );
    
    if( handled )
        {
        // Send ths accent key to FEP
        UiLayout()->SignalOwner( ESignalKeyEvent, *newCharBuf );
        iNewDeadKeyBuf = KNullDesC;
        delete newCharBuf;
        return ETrue;
        }
    
    delete newCharBuf;
    
    return EFalse;
     }

 // --------------------------------------------------------------------------
 // Handle resource of button(base on language direction)
 // --------------------------------------------------------------------------
 //
void CPeninputSplitQwertyWindow::HandleButtonResOnLangDirChange( TInt aControlId )
    {
    TRAP_IGNORE( HandleButtonResOnLangDirChangeL( aControlId ) );
    }

// ---------------------------------------------------------------------------
// Handle resource of button(base on language direction)
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::HandleButtonResOnLangDirChangeL( TInt aControlId )
    {
    CAknFepCtrlCommonButton* ctrl = CommonButtonCtrl( aControlId ); 
    if ( ctrl && iBmpRotator )
        {
        TSyncRotationAngle rotChoice = CBitmapRotator::EMirrorVerticalAxis; 
        
        User::LeaveIfError( 
                iBmpRotator->Rotate( *ctrl->ForgroundBmp() ,rotChoice ) 
                );
        
        User::LeaveIfError( 
                iBmpRotator->Rotate( *ctrl->ForgroundBmpMask(), rotChoice ) 
                );
        }        
    }    

// ---------------------------------------------------------------------------
// Utils to get context value 
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyWindow::IntContext( TPeninputDataType aDataIndex )
    {
    return CPeninputDataConverter::AnyToInt( 
                                       iLayoutContext->RequestData( aDataIndex ) 
                                   );
    }

// ---------------------------------------------------------------------------
// Handle range list event
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::HandleRangeListSelectedL( CFepUiBaseCtrl* /*aCtrl*/, 
                                                           const TDesC& aData )
    {
    CFepLayoutChoiceList::SEvent* event = (CFepLayoutChoiceList::SEvent*)aData.Ptr();
    
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
        ChangeRange( ERangeAccent, EPeninputVkbLayoutAccented1 + 2 * index, ETrue );
        }
    UpdateRangeCtrlsL();
    }

// ---------------------------------------------------------------------------
// Get resource ids of range button icon
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyWindow::GetRangeIconResource( TInt aLangId, 
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
void CPeninputSplitQwertyWindow::PrepareRangeListItems( 
                             RArray<CFepLayoutChoiceList::SItem>& aRangeItems )
    {
    aRangeItems.Reset();
    TInt currentRange = IntContext( EPeninputDataTypeCurrentRange );
    TInt permittedRange = IntContext( EPeninputDataTypePermittedRange );
    TInt currentAccent = IntContext( EPeninputDataTypeCurrentAccent );
    
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
    
    if ( permittedRange & ERangeNativeNumber )
        {
        rgninfo = rbinfo->FindRange( ERangeNativeNumber );
        if ( rgninfo && currentRange != ERangeNativeNumber )
            {
            item.iCommand = EPeninputVkbLayoutNativeNumber;
            item.iText.Copy( iRangeLabels.iLabelNativeNumber );
            aRangeItems.Append( item );
            }
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
    
    if ( permittedRange & ERangeNumber )
        {
        rgninfo = rbinfo->FindRange( ERangeNumber );
        if ( rgninfo && currentRange != ERangeNumber )
            {
            item.iCommand = EPeninputVkbLayoutLatinNumber;
            item.iText.Copy( iRangeLabels.iLabelLatinNumber );
            aRangeItems.Append( item );
            }
        }  
    }
// End Of File
