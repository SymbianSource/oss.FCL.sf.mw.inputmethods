/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for finger hwr UiLayout
*
*/


//SYSTEM INCLUDES
#include <bautils.h>
#include <coemain.h>

//FEP Includes
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <peninputlayoutchoicelist.h>
#include <settingsinternalcrkeys.h>
#include <centralrepository.h>
#include <peninputlayoutvkb.h>
#include <AknFepGlobalEnums.h>
#include <peninputcmdparam.h>
#include <peninputdropdownlist.h>
#include <peninputlayoutmultilineicf.h>

//USER INCLUDES
#include "peninputfingerhwrdatastore.h"
#include "peninputfingerhwrstatemanager.h"
#include "peninputfingerhwrevent.h"
#include "peninputfingerhwrlayout.h"
#include "peninputfingerhwrcontrolid.h"

#include "peninputfingerhwrwnd.h"
#include "peninputfingerhwrcn.rsg"

//CONST DEFINATION
_LIT( KFSHWRRssFile, "\\resource\\peninputfingerhwrcn.rsc" );

const TUint8 KTempBufSize = 8;
const TUint8 KIntLen = sizeof(TInt)/2;
const TUint8 KIntSize = sizeof(TInt);

const TInt16 KEmotionKeyMark = 0xFFFE;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrLayout* CPeninputFingerHwrLayout::NewL( 
    MLayoutOwner* aLayoutOwner, const TAny* aInitData )
    {
    CPeninputFingerHwrLayout* self = new( ELeave ) CPeninputFingerHwrLayout( 
            aLayoutOwner );
    
    CleanupStack::PushL( self );
    self->ConstructL( aInitData );
    CleanupStack::Pop( self ); //self
    return self;
    }

// ---------------------------------------------------------------------------
// handle the command from Fep.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLayout::HandleCommand( const TInt aCmd, TUint8* aData )
    {
    TInt ret = KErrUnknown;
    TRAPD(err, ret = HandleCommandL( aCmd, aData ) );
    if ( err )
        {
        return KErrUnknown;
        }
    else
        {
        return ret;
        }
    }

// ---------------------------------------------------------------------------
// handle the command from Fep.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLayout::HandleCommandL( const TInt aCmd, TUint8* aData )
    {
    if ( CFepUiLayout::HandleCommand( aCmd, aData ) == KErrNone )
        {
        return KErrNone;
        }

    TInt ret = KErrUnknown;
    switch ( aCmd )
        {
        case ECmdPenInputEditorNumericKeyMap:
            {
            iDataStore->SetNumberMode( *(TAknEditorNumericKeymap*)aData );
            OnNumMappingChangedL();
            ret = KErrNone;
            }
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
            {
            TInt len = *( TInt* )( aData - KIntSize );
            TPtrC16 keymapRes( (TUint16*)aData, len / 2 );
            iDataStore->SetCustomNumberModeL( keymapRes );
            OnNumMappingChangedL();
            ret = KErrNone;
            }
            break;
        case ECmdPenInputPermittedRange:
            {
            TInt ranges = *(TInt*)aData;
            iDataStore->SetPermittedRanges( ranges );
            iHwrWnd->SetPermittedRanges( ranges );
            ret = KErrNone;
            }
            break;
        case ECmdPenInputLanguage:
            {
            TInt language = *( TInt* )aData;
            iDataStore->SetLanguageL( language );
            iHwrWnd->SetInputLanguage( language );

            ret = KErrNone;
            }
            break;
        case ECmdPenInputRange:
            {
            TInt range = *( TInt* )aData;
            iDataStore->SetPrimaryRange( range );
            range = iDataStore->CurrentRange();
            iHwrWnd->TryToChangeCharRange( range );

            TBuf<KIntLen> buf;
            buf.Append( reinterpret_cast<TText*>( aData ), KIntLen );

            SignalOwner( ESignalRange, buf );

            ret = KErrNone;
            }
            break;
        case ECmdPenInputPermittedCase:
            {
            TInt cs = *( TInt* )aData;
            iDataStore->SetPermittedCases( cs );
            ret = KErrNone;
            }
            break;
        case ECmdPenInputCase:
            {
            TInt cs = *( TInt* )aData;
            iDataStore->SetCase( cs );
            ret = KErrNone;
            }
            break;
        case ECmdPenInputWindowOpen:
            {
            if( !iIsOpenFingerHWR )
                {
                iDataStore->GetKeyboardType();
                iDataStore->SetKeyboardToQwerty();
                }
            
            LayoutOwner()->Hide( EFalse );
            RootControl()->UpdateValidRegion( NULL, EFalse );

            TRect hwrRect = iHwrWnd->WritingBoxRect();
            TSize hwrSize = hwrRect.Size();
            iDataStore->SetInputAreaSize(hwrSize);
            iDataStore->SetScreenSize(hwrSize);
            ChangeCurStateManagerToStandby();
            
            iIsOpenFingerHWR = ETrue;
            ret = KErrNone;
            }
            break;
        case ECmdPenInputWindowClose:
            {
            iIsOpenFingerHWR = EFalse;
            
            iDataStore->ResetKeyboardType();
            LayoutOwner()->Hide( ETrue );
            ret = KErrNone;
            }
            break;
        case ECmdPenInputSetTextAlignment:
            {
            iHwrWnd->SetTextAlignmentL( *aData, ELangPrcChinese );
            ret = KErrNone;            
            }
            break;
        case ECmdPenInputSetPromptText:
            {
            iHwrWnd->SetPromptTextL( aData );
            ret = KErrNone;
            }
            break;
        case ECmdPenInputDimArrowKeys:
            {
            TBool IsDimArrowKeys = *aData;
            iHwrWnd->DimArrowKeys( IsDimArrowKeys );
            ret = KErrNone;
            }
            break;
        case ECmdPenInputCharacterPreview:
            {
            iHwrWnd->ShowBubble(*aData);    
            }
            break;
        case ECmdPenInputEnableSettingBtn:    
            {
            TBool isEnableSetting = *aData;
            iHwrWnd->SetEnableSettingBtn(isEnableSetting);
            }
            break;  
        default:
            break;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// handle size changing event.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLayout::SizeChanged( const TAny* /*pData*/ )
    {
    //Handle size change event
    SetRect( TRect( TPoint( 0, 0 ), ScreenSize() ) );
    
    TRAP_IGNORE( iHwrWnd->SizeChangedL( EFalse ) );

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Handle editor text coming.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLayout::OnAppEditorTextComing( 
    const TFepInputContextFieldData& aData )
    {
    if ( iHwrWnd )
        {
        TRAPD( err, iHwrWnd->SetEditorTextL( aData ) );
        return err;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Handle application info change.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::HandleAppInfoChange( const TDesC& aInfo,
    TPeninputAppInfo aType )
    {
    if ( aType == EAppIndicatorMsg )
        {
        TRAP_IGNORE( iHwrWnd->SetBubbleTextL( aInfo ) );
        }
    }

// ---------------------------------------------------------------------------
// Called by owner when the system resource changed event.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLayout::OnResourceChange( TInt aType )
    {
    CFepUiLayout::OnResourceChange( aType );
    if ( aType == KAknsMessageSkinChange )
        {
        //Do other things when skin change
        TInt newValue = GetPenTrailColor();
	    SetBoxPenColor(newValue);        
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// get the peninput ui type.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLayout::PenInputType()
    {
    return EPluginInputModeFingerHwr;
    }

// ---------------------------------------------------------------------------
// Called by owner when the layout is going to be hidden.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::OnDeActivate()
    {
    iIsOpenFingerHWR = EFalse;
    iDataStore->ResetKeyboardType();
    
    //cancel writing
    iHwrWnd->CancelWriting();
    
    // Bring the expandable dropdown list back to right level
    iHwrWnd->PredictiveList()->BringToBack();
    
    //cancel range selector
    if ( iHwrWnd->RangeDropListVisible() )
        {
        iHwrWnd->CancelRangeDropList();
        }
    
    if(iHwrWnd->CurrentCharRange() != EFingerHwrSymbolRange)
        {
        iHwrWnd->UpdateLayout( ETrue );
        }
    
    CFepUiLayout::OnDeActivate();
    }

// ---------------------------------------------------------------------------
// handle control event.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::HandleControlEvent( TInt aEventType,
    CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    TRAP_IGNORE( HandleControlEventL( aEventType, aCtrl, aEventData ) );
    }

// ---------------------------------------------------------------------------
// handle control event.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::HandleControlEventL( TInt aEventType,
    CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    switch ( aEventType )
        {
        case EEventButtonDown:
            {
            OnCtrlButtonDownL( aEventType, aCtrl, aEventData );
            }
            break;
        case EEventButtonUp:
            {
            OnCtrlButtonUpL( aEventType, aCtrl, aEventData );
            }
            break;
        case EEventChoiceSelected:
            {
            OnImeSelected( aEventType, aCtrl, aEventData );
            }
            break;

        case EEventHwrCharacterTimerOut:
            {
            OnHwrCharacterFinishedL();
            }
            break;
        case EEventHwrStrokeStarted:
            {
            OnHwrStrokeStartedL();
            }
            break;
        case EEventHwrStrokeFinished:
            {
            OnHwrStrokeFinishedL();
            }
            break;
        case EHwrEventKeyBack:
            {
            OnBackspaceClickedL();
            }
            break;
        case EHwrEventNavKeyClicked:
            {
            OnRepButtonClickedL( aCtrl, aEventData );
            }
            break;
        case EEventVirtualKeyDown:
            {
            OnVirtualKeyDownL( aCtrl, aEventData );
            }
            break;
        case EEventVirtualKeyUp:
            {
            OnVirtualKeyUpL( aCtrl, aEventData );
            }
            break;
        case EHwrEventCandidateSelected:
            {
            OnCandidateSelectedL( aCtrl, aEventData );
            }
            break;
        case EHwrEventPredictiveSelected:
            {
            OnPredictSelectedL( aCtrl, aEventData );
            }
            break;
        case EHwrEventGetNextCandidatePage:
            {
            OnCandidateWndEventL();
            }
            break;
        case EEventIcfPointerUpEvent:
            {
            OnIcfClicked();
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// Get the data stroe object.
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrDataStore& CPeninputFingerHwrLayout::DataStore()
    {
    return *iDataStore;
    }

// ---------------------------------------------------------------------------
// Delete the previous charcode and submit the new one
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::Replace( const TDesC& aOldCharCode,
    const TDesC& aNewCharCode, const TBool aIsPart )
    {
    // user tries to correct the previous selection
    // Delete the previous character and append the new one
    HBufC* newCharBuf = HBufC::New( aNewCharCode.Length() + KIntLen );
    if ( newCharBuf )
        {
        TInt len = aOldCharCode.Length();
        if(len == 0 )
            {
            delete newCharBuf;
            return;
            }
            
        newCharBuf->Des().Append( ( TUint16* )&len, KIntLen );
        newCharBuf->Des().Append( aNewCharCode );
        if ( aIsPart )
            {
            SignalOwner( ESignalReplacePartText, *newCharBuf );
            }
        else
            {
            SignalOwner( ESignalReplaceText, *newCharBuf );
            }
        delete newCharBuf;
        }
    }

// ---------------------------------------------------------------------------
// Submit string to layout owner.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::SubmitStringToFep( const TDesC& aCharCode )
    {
    SignalOwner( ESignalKeyEvent, aCharCode );
    }

// ---------------------------------------------------------------------------
// Submit char to layout owner.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::SubmitCharToFep( const TInt aCharCode )
    {
    TBuf<1> buf;
    buf.Append( aCharCode );
    SignalOwner( ESignalKeyEvent, buf );
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
// ---------------------------------------------------------------------------
// Submit start position to layout owner.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::SubmitStartPosToFep( TBool aSetPosFlag )
    {
    TBuf<4> buf;
    TInt length = sizeof(TBool)/sizeof(TText);
    buf.Append( reinterpret_cast<TText*>( &aSetPosFlag ), length );
    SignalOwner( ESignalCharacterStart, buf );
    }
#endif 

// ---------------------------------------------------------------------------
// repository callback for settings.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrLayout::HandleGSRepositoryCallBack( TAny* aPtr )
    {
    CPeninputFingerHwrLayout *self = static_cast<CPeninputFingerHwrLayout*>( aPtr );

    CAknFepRepositoryWatcher* watcher = self->iGSRepositoryWatcher;
    if ( watcher )
        {
        TInt newValue = self->GetNewValue( watcher, watcher->ChangedKey() );

        switch ( watcher->ChangedKey() )
            {
            case KSettingsWritingSpeed:
                {
                self->SetWritingSpeed( newValue );
                }
                break;
            case KSettingsPenWidth:
                {
                self->SetBoxPenSize( TSize( newValue, newValue ) );
                }
                break;
            case KSettingsPenColor:
                {
                self->SetBoxPenColor( newValue );
                }
                break;
            default:
                break;
            }
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// load default settings.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::LoadAndPublishDefaultL()
    {
    if(!iRepositorySetting)
        {
        return;
        }

    TInt newValue;

    iRepositorySetting->Get( KSettingsWritingSpeed, newValue );
    SetWritingSpeed( newValue );

    iRepositorySetting->Get( KSettingsPenWidth, newValue );
    SetBoxPenSize( TSize( newValue, newValue ) );

    newValue = GetPenTrailColor();
    SetBoxPenColor( newValue );

    iRepositorySetting->Get( KSettingsPenInputLang, newValue );
    iDataStore->SetLanguageL( newValue );

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::SyncHwrStrokeEndMark( const TPoint&  aEndMark )
    {
    iHwrWnd->SetStrokeEndMark( aEndMark );
    }

#ifndef RD_INTELLIGENT_TEXT_INPUT
// ---------------------------------------------------------------------------
// hide or show fixed english symbol list.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::HideTwoRowEngSymbolList( const TBool aIsHide )
    {
    iHwrWnd->HideTwoRowEngSymbolList( aIsHide );
    }
#endif // RD_INTELLIGENT_TEXT_INPUT   

// private

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrLayout::CPeninputFingerHwrLayout( MLayoutOwner* aLayoutOwner ) 
    : CFepUiLayout( aLayoutOwner ), iLastOverlapIdx(KInvalidIndex)
    {
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrLayout::~CPeninputFingerHwrLayout()
    {
    CCoeEnv::Static()->DeleteResourceFile( iResId );

    delete iDataStore;
    iStateMgrArray.ResetAndDestroy();
    iStateMgrArray.Close();

    delete iGSRepositoryWatcher;
    delete iRepositorySetting;
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//   
void CPeninputFingerHwrLayout::ConstructL( const TAny* aInitData )
    {
    BaseConstructL();
    //load resource
    TFileName resFileName( KFSHWRRssFile );
    CCoeEnv* coeEnv = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile( coeEnv->FsSession(), resFileName );
    iResId = coeEnv->AddResourceFileL( resFileName );

    //initialze the data store
    TPeninputLayoutData* layoutData = ( TPeninputLayoutData* )aInitData;

    iDataStore = CPeninputFingerHwrDataStore::NewL( (*layoutData).iPtiEngine, this );

    //create the repository for gereral settings
    iRepositorySetting = CRepository::NewL( KCRUidPersonalizationSettings );

    //create the repository watcher for general settings
    iGSRepositoryWatcher = CAknFepRepositoryWatcher::NewL( 
            KCRUidPersonalizationSettings, 
            TCallBack( HandleGSRepositoryCallBack, this ), 
            iRepositorySetting );

    //create state manager   
    CreateStateManagerL();

    CreateHwrWindowL();

    LoadAndPublishDefaultL();
    //set screen layout extent
    SetRect( TRect( TPoint( 0, 0 ), ScreenSize() ) );
    }

// ---------------------------------------------------------------------------
// Create the hwr box window.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::CreateHwrWindowL()
    {
    iHwrWnd = CPeninputFingerHwrWnd::NewL( this, EHwrCtrlIdHwrWindow, EFalse );

    iHwrWnd->SetResourceId( R_FINGERHWR_WINDOW );
    iHwrWnd->ConstructFromResourceL();
    
    iHwrWnd->SetStrokeEndMark( iDataStore->StrokeEndMark() );

    AddControlL( iHwrWnd, EFalse );
    }

// ---------------------------------------------------------------------------
// Create the state manager.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::CreateStateManagerL()
    {
    iStateMgrArray.AppendL( CPeninputFingerHwrStateManagerCn::NewL( this ) );
    iStateMgrArray.AppendL( CPeninputFingerHwrStateManagerEn::NewL( this ) );
    iStateMgrArray.AppendL( CPeninputFingerHwrStateManagerNum::NewL( this ) );
    }

// ---------------------------------------------------------------------------
// handler of button down event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnCtrlButtonDownL( TInt /*aEventType*/,
    CFepUiBaseCtrl* /*aCtrl*/, const TDesC& /*aEventData*/ )
    {

    }

// ---------------------------------------------------------------------------
// handler of button up event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnCtrlButtonUpL( TInt /*aEventType*/,
    CFepUiBaseCtrl* aCtrl, const TDesC& /*aEventData*/ )
    {
    if ( !aCtrl )
        {
        return;
        }

    switch ( aCtrl->ControlId() )
        {
        case EHwrCtrlIdClose:
            {
            // Go to standby when button up before character timeout    
            if ( iDataStore->StartCharacter())
                {
                iDataStore->SetStartCharacter(EFalse);
                iHwrWnd->CancelWriting();
                ChangeCurStateManagerToStandby();
                iHwrWnd->UpdateLayout( ETrue );
                }
        
            SignalOwner( ESignalLayoutClosed );
            }
            break;
        case EHwrCtrlIdLanguageOption:
            {
            SignalOwner( ESignalLaunchOptionMenu );
            }
            break;
        case EHwrCtrlIdVkbSwitcher:
            {
            iHwrWnd->PopupImeSwitcher();
            }
            break;
        case EHwrCtrlIdChineseMiButton:
        case EHwrCtrlIdEnglishMiButton:
        case EHwrCtrlIdNumberMiButton:
            {
            // Go to standby when button up before character timeout    
            if ( iDataStore->StartCharacter())
                {
                iDataStore->SetStartCharacter(EFalse);
                iHwrWnd->CancelWriting();
                ChangeCurStateManagerToStandby();
                iHwrWnd->UpdateLayout( ETrue );
                }
        
            iHwrWnd->ShowRangeDropList( EPopDirAuto );            
            }
            break;
        case EHwrCtrlIdChineseButton:
            {
            if ( iDataStore->CurrentRange() != EFingerHwrNativeRange )
                {
                iDataStore->SetPrimaryRange( ERangeNative );
                iDataStore->SaveRange( ERangeNative );

                ChangeCurStateManagerToStandby();
                NotifyPrimaryRangeChanged( ERangeNative );
                }

            iHwrWnd->TryToChangeCharRange( EFingerHwrNativeRange );            
            }
            break;
        case EHwrCtrlIdEnglishButton:
            {
            if ( iDataStore->CurrentRange() != EFingerHwrEnglishRange )
                {
                iDataStore->SetPrimaryRange( ERangeEnglish );
                iDataStore->SaveRange( ERangeEnglish );

                ChangeCurStateManagerToStandby();
                NotifyPrimaryRangeChanged( ERangeEnglish );
                }

            iHwrWnd->TryToChangeCharRange( EFingerHwrEnglishRange );
            }
            break;
        case EHwrCtrlIdSymbolButton:
            {
            // Go to standby when button up before character timeout    
            if ( iDataStore->StartCharacter())
                {
                iDataStore->SetStartCharacter(EFalse);
                iHwrWnd->CancelWriting();
                ChangeCurStateManagerToStandby();
                iHwrWnd->UpdateLayout( ETrue );
                }
        
            if ( iHwrWnd->CurrentCharRange() == EFingerHwrSymbolRange )
                {
                RestoreRange();
                }
            else
                {
                ChangeCurStateManagerToStandby();
                iHwrWnd->TryToChangeCharRange( EFingerHwrSymbolRange );
                }
            }
            break;
        case EHwrCtrlIdNumberButton:
            {
            if ( iDataStore->CurrentRange() != EFingerHwrNumberRange )
                {
                iDataStore->SetPrimaryRange( ERangeNumber );
                iDataStore->SaveRange( ERangeNumber );

                ChangeCurStateManagerToStandby();
                NotifyPrimaryRangeChanged( ERangeNumber );
                }
            iHwrWnd->TryToChangeCharRange( EFingerHwrNumberRange );
            }
            break;
        case EHwrCtrlIdSctPage0Btn:
            {
            iHwrWnd->ShowSctPage( 1, EPagePosPageNo );
            }
            break;
        case EHwrCtrlIdSctPage1Btn:
            {
            iHwrWnd->ShowSctPage( 2, EPagePosPageNo );
            }
            break;
        case EHwrCtrlIdSctPage2Btn:
            {
            iHwrWnd->ShowSctPage( 0, EPagePosPageNo );
            }
            break;
        default:
            break;
        }

    }

// ---------------------------------------------------------------------------
// handler of rep-button clicked event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnRepButtonClickedL( CFepUiBaseCtrl* aCtrl,
    const TDesC& aData )
    {
    if ( !aCtrl )
        {
        return;
        }

    switch ( aCtrl->ControlId() )
        {
        case EHwrCtrlIdArrowRight:
        case EHwrCtrlIdArrowLeft:
            {
            SignalOwner( ESignalKeyEvent, aData );
            }
            break;
        case EHwrCtrlIdArrowDown:
            {
            iHwrWnd->Icf()->HandleArrowBtnEventL( CFepLayoutMultiLineIcf::EArrowDown );
            }
            break;
        case EHwrCtrlIdArrowUp:
            {
            iHwrWnd->Icf()->HandleArrowBtnEventL( CFepLayoutMultiLineIcf::EArrowUp );
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// handler of BACKSPACE button clicked event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnBackspaceClickedL()
    {
    if ( iDataStore->StartCharacter() ) // before character timer out
        {
        // goto standby and clear screen
        iDataStore->SetStartCharacter(EFalse);
        iHwrWnd->CancelWriting();
        }
    else
        {
        SubmitCharToFep( EKeyBackspace );
        }
        
        CurStateManager()->HandleEventL( EHwrEventKeyBack, KNullDesC );
        iLastOverlapIdx = iDataStore->RemoveDuplicateCand(iDataStore->Candidate(),
                                          const_cast<RPointerArray<HBufC>&>(iDataStore->Predictive()),
                                          0,
                                          0);
        
    
    // not update layou when SCT button is latched
    if ( ( iDataStore->CurrentRange() == EFingerHwrNativeRange || 
           iDataStore->CurrentRange() == EFingerHwrEnglishRange ) && 
           iHwrWnd->CurrentCharRange() != EFingerHwrSymbolRange )
        {
        iHwrWnd->UpdateLayout( CurStateManager()->IsStandbyState() );
        }
    }

// ---------------------------------------------------------------------------
// handler of virtual key down event.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::OnVirtualKeyDownL( CFepUiBaseCtrl* /*aCtrl*/,
    const TDesC& /*aData*/ )
    {

    }

// ---------------------------------------------------------------------------
// handler of virtual key up event.
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrLayout::OnVirtualKeyUpL( CFepUiBaseCtrl* /*aCtrl*/,
    const TDesC& aData )
    {
    const TVirtualKeyEventData* vkdata = NULL;
    const TKeyEvent *keydata = NULL;
    vkdata = reinterpret_cast<const TVirtualKeyEventData*>( aData.Ptr() );
    keydata = reinterpret_cast<const TKeyEvent*>( aData.Ptr() );

    if ( vkdata->iKeyEvent.iScanCode == KEmotionKeyMark )
        {
        TBuf<KTempBufSize> buf;
        buf.Append( vkdata->iKeyData );
        SubmitStringToFep( buf );
        }
    else
        {
        TBuf<1> bufBck;
        bufBck.Append( keydata->iScanCode );
        SignalOwner( ESignalKeyEvent, bufBck );
        }
    //restore range before sct popup
    RestoreRange();
    }

// ---------------------------------------------------------------------------
// handler of candidate list event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnCandidateSelectedL( CFepUiBaseCtrl* aCtrl,
    const TDesC& aData )
    {
    if ( !aCtrl )
        {
        return;
        }
    
    CurStateManager()->HandleEventL( EHwrEventCandidateSelected, aData );
    
    iLastOverlapIdx = iDataStore->RemoveDuplicateCand(iDataStore->Candidate(),
                                          const_cast<RPointerArray<HBufC>&>(iDataStore->Predictive()),
                                          0,
                                          0);
    

    iHwrWnd->UpdateLayout( EFalse );
    }

// ---------------------------------------------------------------------------
// handler of predictive list event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnPredictSelectedL( CFepUiBaseCtrl* aCtrl,
    const TDesC& aData )
    {
    if ( !aCtrl )
        {
        return;
        }

    CurStateManager()->HandleEventL( EHwrEventPredictiveSelected, aData );
    iLastOverlapIdx = iDataStore->RemoveDuplicateCand(iDataStore->Candidate(),
                      const_cast<RPointerArray<HBufC>&>(iDataStore->Predictive()),
                      0,
                      0);

    iHwrWnd->UpdateLayout( EFalse );
    }

// ---------------------------------------------------------------------------
// clicked event handler of icf.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnIcfClicked()
    {
    if ( iHwrWnd->CurrentCharRange() == EFingerHwrSymbolRange )
        {
        // cancel sct and restore last range.
        RestoreRange();
        }
    else
        {
        // Go to standby
        ChangeCurStateManagerToStandby();
        iHwrWnd->UpdateLayout( ETrue );
        }
    }

// ---------------------------------------------------------------------------
// handler of ime selector event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnImeSelected( TInt /*aEventType*/,
    CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    if ( !aCtrl )
        {
        return;
        }

    CFepLayoutChoiceList::SEvent* event= NULL;
    event = (CFepLayoutChoiceList::SEvent*)aEventData.Ptr();
    if ( !event || event->iIndex == -1 )
        {
        return;
        }

    TBool switchByMode = ETrue;
    TInt mode = event->iCommand;

    TBuf<KTempBufSize> buf;
    TInt length = sizeof(TBool)/sizeof(TText);
    buf.Append( reinterpret_cast<TText*>( &switchByMode ), length );
    
    length = sizeof(TInt)/sizeof(TText);
    buf.Append( reinterpret_cast<TText*>( &mode ), length );
    SignalOwner( ESignalLayoutUIChanged, buf );
    }

// ---------------------------------------------------------------------------
// handler of StrokeStarted event.
// ---------------------------------------------------------------------------
//                                              
void CPeninputFingerHwrLayout::OnHwrStrokeStartedL()
    {
    if(!iDataStore->StartCharacter())
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        // set first stroke flag for Auto Complete
        iDataStore->SetFirstStroke(ETrue);
        
        // The default candidate cell is not highlight
        iDataStore->SetHighlight(EFalse);
#else
        if(iDataStore->PrimaryRange() == ERangeEnglish)
            {
            iDataStore->SetFixEnglishSymbolL();
            }
        // Hide the fix English Symbol list         
        iHwrWnd->HideTwoRowEngSymbolList(ETrue);
        
        // The default candidate cell is not highlight
        iDataStore->SetHighlight(EFalse);      
#endif
        }
        
    // remember the start writing position.
    iDataStore->SetStartCharacter(ETrue);
    iHwrWnd->CancelAllPopList();
    CurStateManager()->HandleEventL( EEventHwrStrokeStarted, KNullDesC );
    }

// ---------------------------------------------------------------------------
// handler of StrokeFinished event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnHwrStrokeFinishedL()
    {
    RArray<TPoint> points = iHwrWnd->StrokeList();
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&points), sizeof(&points));
    CurStateManager()->HandleEventL( EEventHwrStrokeFinished, ptr );
    
#ifdef RD_INTELLIGENT_TEXT_INPUT     
    iDataStore->SetFirstStroke(EFalse);
#endif    

    iHwrWnd->UpdateLayout(EFalse);
    }

// ---------------------------------------------------------------------------
// handler of CharacterFinished event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::OnHwrCharacterFinishedL()
    {
    iDataStore->SetStartCharacter( EFalse );

    RArray<TPoint> points = iHwrWnd->StrokeList();
    TPtrC ptr;
    ptr.Set( reinterpret_cast<TText*>( &points ), sizeof(&points) );
    CurStateManager()->HandleEventL( EEventHwrCharacterTimerOut, ptr );

    // When recognize Enter or Backspace, it will go to standby
    iHwrWnd->UpdateLayout( CurStateManager()->IsStandbyState(), EFalse, EFalse );
    }

// ---------------------------------------------------------------------------
// handler of NumMapping changed event.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrLayout::OnNumMappingChangedL()
    {
    HBufC* mapping = iDataStore->KeyMappingStringL();

    iHwrWnd->SetNumericMapping( *mapping );

    delete mapping;
    }

// ---------------------------------------------------------------------------
// Handler of the candidate list event.
// ---------------------------------------------------------------------------
//   
void CPeninputFingerHwrLayout::OnCandidateWndEventL()
    {
    TInt candscnt = iDataStore->PredictiveCount(); 
 
    iDataStore->GetNextPredictivePageL();

    iLastOverlapIdx = iDataStore->RemoveDuplicateCand(iDataStore->Candidate(),
                                          const_cast<RPointerArray<HBufC>&>(iDataStore->Predictive()),
                                          iLastOverlapIdx + 1,
                                          candscnt);

    if ( iDataStore->PredictiveCount() < KMaxPredictiveCount )
        {
        RPointerArray<HBufC> predicts = iDataStore->Predictive();
        iHwrWnd->PredictiveList()->AddCandidatesL( predicts,
                CFepCtrlDropdownList::EListExpandable );
        }
    }


// ---------------------------------------------------------------------------
// Notify the layout owner the primary has changed.
// ---------------------------------------------------------------------------
//   
void CPeninputFingerHwrLayout::NotifyPrimaryRangeChanged( TInt aPrimaryRange )
    {
    TBuf<KIntLen> buf;
    buf.Append( ( TUint16* )( &aPrimaryRange ), KIntLen );
    SignalOwner( ESignalRange, buf );
    }

// ---------------------------------------------------------------------------
// get value from repository.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrLayout::GetNewValue( 
        CAknFepRepositoryWatcher* aWatcher, const TInt aId )
    {
    TInt newValue = -1;

    if ( aWatcher == iGSRepositoryWatcher )
        {
        iRepositorySetting->Get( aId, newValue );
        }

    return newValue;
    }

// ---------------------------------------------------------------------------
// get hwr trail color from repository.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrLayout::GetPenTrailColor()
    {
    TInt newValue = 0;
    iRepositorySetting->Get( KSettingsPenColor, newValue );

    if ( !AknPenInputTrailColorUtils::CheckColorInColorTable( newValue ) )
        {
        return AknPenInputTrailColorUtils::GetTrailColorByTheme();
        }
    return newValue;
    }

// ---------------------------------------------------------------------------
// set hwr writing speed.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::SetWritingSpeed( const TInt aWritingSpeed )
    {
    TTimeIntervalMicroSeconds32 speed;
    switch ( aWritingSpeed )
        {
        case EWritingSpeedVeryFast:
            {
            speed = EHwrWritingSpeedVeryFast;
            }
            break;
        case EWritingSpeedFast:
            {
            speed = EHwrWritingSpeedFast;
            }
            break;
        case EWritingSpeedNormal:
            {
            speed = EHwrWritingSpeedNormal;
            }
            break;
        case EWritingSpeedSlow:
            {
            speed = EHwrWritingSpeedSlow;
            }
            break;
        case EWritingSpeedVerySlow:
            {
            speed = EHwrWritingSpeedVerySlow;
            }
            break;
        default:
            {
            speed = EHwrWritingSpeedNormal;
            }
            break;
        }

    iHwrWnd->SetBoxWritingSpeed( speed );
    }

// ---------------------------------------------------------------------------
// set hwr writing pen width.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::SetBoxPenSize( const TSize aPenSize )
    {
    iHwrWnd->SetBoxPenSize( aPenSize );
    }

// ---------------------------------------------------------------------------
// set hwr writing pen color.
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrLayout::SetBoxPenColor( const TInt aPenColor )
    {
    iHwrWnd->SetBoxPenColor( aPenColor );
    }
// ---------------------------------------------------------------------------
// restore the range before sct popup.
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrLayout::RestoreRange()
    {
    TInt rangeFlag = iHwrWnd->RestoreNonSctLayout();
    iDataStore->SetPrimaryRange( rangeFlag );
    iDataStore->SaveRange( rangeFlag );
    ChangeCurStateManagerToStandby();
    NotifyPrimaryRangeChanged( rangeFlag );
    }

// ---------------------------------------------------------------------------
// get current state manager.
// ---------------------------------------------------------------------------
// 
CPeninputFingerHwrStateManagerBase* CPeninputFingerHwrLayout::CurStateManager()
    {
    TInt index = iDataStore->CurrentRange();
    if ( index < 0 || index >= iStateMgrArray.Count()  ) 
        {
        return NULL;
        }
    else
        {
        return iStateMgrArray[index];
        }
    }


// ---------------------------------------------------------------------------
// change current state manager to standby state.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrLayout::ChangeCurStateManagerToStandby()
    {
    CPeninputFingerHwrStateManagerBase* stateMgr = CurStateManager();
    if ( stateMgr )
        {
        stateMgr->SetState( CPeninputFingerHwrStateManagerBase::EStateStandBy );
        }
    }

//End of file
