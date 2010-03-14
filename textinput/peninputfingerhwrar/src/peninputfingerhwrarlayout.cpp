/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
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
#include "peninputfingerhwrardatastore.h"
#include "peninputfingerhwrarstatemanager.h"
#include "peninputfingerhwrarevent.h"
#include "peninputfingerhwrarlayout.h"
#include "peninputfingerhwrarcontrolid.h"

#include "peninputfingerhwrarwnd.h"
#include "peninputfingerhwrarwnd.rsg"

//CONST DEFINATION
_LIT( KFSHWRRssFile, "\\resource\\peninputfingerhwrarwnd.rsc" );

const TUint8 KTempBufSize = 8;
const TUint8 KIntLen = sizeof(TInt)/2;
const TUint8 KIntSize = sizeof(TInt);

const TInt16 KEmotionKeyMark = 0xFFFE;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArLayout* CPeninputFingerHwrArLayout::NewL( 
    MLayoutOwner* aLayoutOwner, const TAny* aInitData )
    {
    CPeninputFingerHwrArLayout* self = new( ELeave ) CPeninputFingerHwrArLayout( 
            aLayoutOwner );
    
    CleanupStack::PushL( self );
    self->ConstructL( aInitData );
    CleanupStack::Pop( self ); //self
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//   
void CPeninputFingerHwrArLayout::ConstructL( const TAny* aInitData )
    {
    BaseConstructL();
    //load resource
    TFileName resFileName( KFSHWRRssFile );
    CCoeEnv* coeEnv = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile( coeEnv->FsSession(), resFileName );
    iResId = coeEnv->AddResourceFileL( resFileName );

    //initialze the data store
    TPeninputLayoutData* layoutData = ( TPeninputLayoutData* )aInitData;

    iDataStore = CPeninputFingerHwrArDataStore::NewL( (*layoutData).iPtiEngine, this );

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
// handle the command from Fep.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLayout::HandleCommand( const TInt aCmd, TUint8* aData )
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
TInt CPeninputFingerHwrArLayout::HandleCommandL( const TInt aCmd, TUint8* aData )
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
            }
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
            {
            TInt len = *( TInt* )( aData - KIntSize );
            TPtrC16 keymapRes( (TUint16*)aData, len / 2 );
            iDataStore->SetCustomNumberModeL( keymapRes );
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
            LayoutOwner()->Hide( EFalse );
            RootControl()->UpdateValidRegion( NULL, EFalse );

            TRect hwrRect = iHwrWnd->WritingBoxRect();
            TSize hwrSize = hwrRect.Size();
            iDataStore->SetInputAreaSize(hwrSize);
            iDataStore->SetScreenSize(hwrSize);
            ChangeCurStateManagerToStandby();
            
            iDataStore->GetKeyboardType();
            iDataStore->SetKeyboardToQwerty();
            ret = KErrNone;
            }
            break;
        case ECmdPenInputWindowClose:
            {
            iDataStore->ResetKeyboardType();
            LayoutOwner()->Hide( ETrue );
            ret = KErrNone;
            }
            break;
        case ECmdPenInputSetTextAlignment:
            {
            iHwrWnd->SetTextAlignmentL( *aData, ELangArabic );
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
TInt CPeninputFingerHwrArLayout::SizeChanged( const TAny* /*pData*/ )
    {
    //Handle size change event
    SetRect( TRect( TPoint( 0, 0 ), ScreenSize() ) );
   
	TRAP_IGNORE(iHwrWnd->SizeChangedL());

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Handle editor text coming.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLayout::OnAppEditorTextComing( 
    const TFepInputContextFieldData& aData )
    {
    if ( iHwrWnd )
        {
        TRAPD( err, iHwrWnd->SetEditorTextL( aData ) );
		//
	    // implement the feature: the candiates will be filtered according to the char after the cursor
	    //
		
		//
		// As the icf will not insert the text coming from FEP Editor into iRichView 
		// when EFepICFDataDirectionMFNE was set,  
		// so if we are going to call iContextField->ExtractText(), it could cause a panic of 
		// out of range.
		// no idea why icf behaves like this.
		// this is just a temporary solution to avoid this panic happened.
		// The best solution is to get the cusor postion which should be provide by ICF control 
		// but ICF does not offer such API
		if(aData.iFlag & EFepICFDataDirectionMFNE )
			{
			return KErrNone;
			}
			
		iDataStore->SetFirstCandidateType(ECandDefaultFirst);	
		TUint16 charBeforeCursor = 0;
		
        if(iHwrWnd->GetCharBeforeCursor(aData.iCurSel.LowerPos()-1, charBeforeCursor))
		    {
			TFirstCandidateType candtype = ECandDefaultFirst;
			if(iDataStore->IsLatinNumber(charBeforeCursor)) // latin number
				{
				candtype = ECandLatinNumFirst;
				}
			else if(iDataStore->IsLatinChar(charBeforeCursor)) // uppercase & lowcase latin character
				{
				candtype = ECandLatinCharFirst;
				}
			else if(iDataStore->IsArabicNumber(charBeforeCursor)) // arabic-indic numbers
				{
				candtype = ECandArabicIndicNumFirst;
				}
			else if(iDataStore->IsArabicChar(charBeforeCursor))
			    {
			    candtype = ECandArabicCharFirst;
			    }
			iDataStore->SetFirstCandidateType(candtype);	
			}
        
        return err;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Handle application info change.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLayout::HandleAppInfoChange( const TDesC& aInfo,
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
TInt CPeninputFingerHwrArLayout::OnResourceChange( TInt aType )
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
TInt CPeninputFingerHwrArLayout::PenInputType()
    {
    return EPluginInputModeFingerHwr;
    }

// ---------------------------------------------------------------------------
// Called by owner when the layout is going to be hidden.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLayout::OnDeActivate()
    {
    //cancel writing
    iHwrWnd->CancelWriting();
    
    if(!iHwrWnd->IsSymbolTableShowingUp())
        {
        iHwrWnd->UpdateLayout( ETrue );
        }
    
    CFepUiLayout::OnDeActivate();
    }

// ---------------------------------------------------------------------------
// handle control event.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLayout::HandleControlEvent( TInt aEventType,
    CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    TRAP_IGNORE( HandleControlEventL( aEventType, aCtrl, aEventData ) );
    }

// ---------------------------------------------------------------------------
// handle control event.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLayout::HandleControlEventL( TInt aEventType,
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
        case EEventIcfPointerUpEvent:
            {
            OnIcfClicked();
            }
            break;
            
        case EHwrEventOutsideEvent:
            {
            ChangeCurStateManagerToStandby();
            iHwrWnd->CloseSymbolTable();
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
CPeninputFingerHwrArDataStore& CPeninputFingerHwrArLayout::DataStore()
    {
    return *iDataStore;
    }

// ---------------------------------------------------------------------------
// Delete the previous charcode and submit the new one
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLayout::Replace( const TDesC& aOldCharCode,
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
void CPeninputFingerHwrArLayout::SubmitStringToFep( const TDesC& aCharCode )
    {
    SignalOwner( ESignalKeyEvent, aCharCode );
    }

// ---------------------------------------------------------------------------
// Submit char to layout owner.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLayout::SubmitCharToFep( const TInt aCharCode )
    {
    TBuf<1> buf;
    buf.Append( aCharCode );
    SignalOwner( ESignalKeyEvent, buf );
    }

// ---------------------------------------------------------------------------
// repository callback for settings.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArLayout::HandleGSRepositoryCallBack( TAny* aPtr )
    {
    CPeninputFingerHwrArLayout *self = static_cast<CPeninputFingerHwrArLayout*>( aPtr );

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
            case KSettingsPenInputGuideLine:
                {
                
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
void CPeninputFingerHwrArLayout::LoadAndPublishDefaultL()
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
void CPeninputFingerHwrArLayout::SyncHwrStrokeEndMark( const TPoint&  aEndMark )
    {
    iHwrWnd->SetStrokeEndMark( aEndMark );
    }
   

// private

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArLayout::CPeninputFingerHwrArLayout( MLayoutOwner* aLayoutOwner ) 
    : CFepUiLayout( aLayoutOwner ), iLastOverlapIdx(KInvalidIndex)
    {
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArLayout::~CPeninputFingerHwrArLayout()
    {
    CCoeEnv::Static()->DeleteResourceFile( iResId );

    delete iDataStore;
    iStateMgrArray.ResetAndDestroy();
    iStateMgrArray.Close();

    delete iGSRepositoryWatcher;
    delete iRepositorySetting;
    }

// ---------------------------------------------------------------------------
// Create the hwr box window.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::CreateHwrWindowL()
    {
    iHwrWnd = CPeninputFingerHwrArWnd::NewL( this, EHwrCtrlIdHwrWindow, EFalse );

    iHwrWnd->SetResourceId( R_FINGERHWR_WINDOW );
    iHwrWnd->ConstructFromResourceL();
    
    iHwrWnd->SetStrokeEndMark( iDataStore->StrokeEndMark() );

    AddControlL( iHwrWnd, EFalse );
    }

// ---------------------------------------------------------------------------
// Create the state manager.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::CreateStateManagerL()
    {
    iStateMgrArray.AppendL( CPeninputFingerHwrArStateManager::NewL( this ) );
    }

// ---------------------------------------------------------------------------
// handler of button down event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::OnCtrlButtonDownL( TInt /*aEventType*/,
    CFepUiBaseCtrl* /*aCtrl*/, const TDesC& /*aEventData*/ )
    {

    }

// ---------------------------------------------------------------------------
// handler of button up event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::OnCtrlButtonUpL( TInt /*aEventType*/,
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
            
            if(iHwrWnd->IsSymbolTableShowingUp())
            	{
                iHwrWnd->CloseSymbolTable();
                }
            else
                {
                ChangeCurStateManagerToStandby();
                iHwrWnd->OpenSymbolTable();
                }
            }
            break;
        case EHwrCtrlId3Page1Btn:
            {
            iHwrWnd->ShowSctPage( 1, EPagePosPageNo );
            }
            break;
        case EHwrCtrlId3Page2Btn:
            {
            iHwrWnd->ShowSctPage( 2, EPagePosPageNo );
            }
            break;
        case EHwrCtrlId3Page3Btn:
            {
            iHwrWnd->ShowSctPage( 0, EPagePosPageNo );
            }
            break;
        case EHwrCtrlId2Page1Btn:
            {
            iHwrWnd->ShowSctPage( 1, EPagePosPageNo );
            }
            break;    
        case EHwrCtrlId2Page2Btn:
            {
            iHwrWnd->ShowSctPage( 0, EPagePosPageNo );
            }
            break;    
		case EHwrCtrlIdBtnEnter:
            {
			TBuf<1> bufEnter;
            bufEnter.Append( EKeyEnter );
            SubmitStringToFep( bufEnter );
            // close the symbol table
            iHwrWnd->CloseSymbolTable();
			}
            break;
        case EHwrCtrlIdBtnSpace:
            {
			TBuf<1> bufSpace;
			bufSpace.Append( EKeySpace );
			SubmitStringToFep( bufSpace );
			// close the symbol table
			iHwrWnd->CloseSymbolTable();
            break;    
			}		
        default:
            break;
        }

    }

// ---------------------------------------------------------------------------
// handler of rep-button clicked event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::OnRepButtonClickedL( CFepUiBaseCtrl* aCtrl,
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
void CPeninputFingerHwrArLayout::OnBackspaceClickedL()
    {
	iHwrWnd->CancelWriting();
    if (iHwrWnd->IsCandidateShowup())
        {
        // goto standby and clear screen
        iDataStore->SetStartCharacter(EFalse);
		ChangeCurStateManagerToStandby();
	    iHwrWnd->UpdateLayout( ETrue );
        }
    else
        {
		
        SubmitCharToFep( EKeyBackspace );
        CurStateManager()->HandleEventL( EHwrEventKeyBack, KNullDesC );
		if(iDataStore->StartCharacter())
		    {
			ChangeCurStateManagerToStandby();
		    iHwrWnd->UpdateLayout( ETrue );
			}
        }
    }

// ---------------------------------------------------------------------------
// handler of virtual key down event.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArLayout::OnVirtualKeyDownL( CFepUiBaseCtrl* /*aCtrl*/,
    const TDesC& /*aData*/ )
    {

    }

// ---------------------------------------------------------------------------
// handler of virtual key up event.
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrArLayout::OnVirtualKeyUpL( CFepUiBaseCtrl* /*aCtrl*/,
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
        
    // close the symbol table
    iHwrWnd->CloseSymbolTable();
    }

// ---------------------------------------------------------------------------
// handler of candidate list event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::OnCandidateSelectedL( CFepUiBaseCtrl* aCtrl,
    const TDesC& aData )
    {
    if ( !aCtrl )
        {
        return;
        }
    
    CurStateManager()->HandleEventL( EHwrEventCandidateSelected, aData );
    
    
    ChangeCurStateManagerToStandby();

    iHwrWnd->UpdateLayout( ETrue );
    }


// ---------------------------------------------------------------------------
// clicked event handler of icf.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::OnIcfClicked()
    {
    // Go to standby
	ChangeCurStateManagerToStandby();
	iHwrWnd->UpdateLayout( ETrue );
	}

// ---------------------------------------------------------------------------
// handler of StrokeStarted event.
// ---------------------------------------------------------------------------
//                                              
void CPeninputFingerHwrArLayout::OnHwrStrokeStartedL()
    {
	iHwrWnd->HideIndicator();
	// The default candidate cell is not highlight
	iDataStore->SetHighlight(EFalse);      
	iHwrWnd->UpdateLayout(ETrue);
    
	// remember the start writing position.
    iDataStore->SetStartCharacter(ETrue);
    CurStateManager()->HandleEventL( EEventHwrStrokeStarted, KNullDesC );
    }

// ---------------------------------------------------------------------------
// handler of StrokeFinished event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::OnHwrStrokeFinishedL()
    {
    RArray<TPoint> points = iHwrWnd->StrokeList();
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&points), sizeof(&points));
    CurStateManager()->HandleEventL( EEventHwrStrokeFinished, ptr );
    }

// ---------------------------------------------------------------------------
// handler of CharacterFinished event.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::OnHwrCharacterFinishedL()
    {
    iDataStore->SetStartCharacter( EFalse );

    RArray<TPoint> points = iHwrWnd->StrokeList();
    TPtrC ptr;
    ptr.Set( reinterpret_cast<TText*>( &points ), sizeof(&points) );
    CurStateManager()->HandleEventL( EEventHwrCharacterTimerOut, ptr );

    // When recognize Enter or Backspace, it will go to standby
    iHwrWnd->UpdateLayout( CurStateManager()->IsStandbyState(), ETrue);
    }

// ---------------------------------------------------------------------------
// get value from repository.
// ---------------------------------------------------------------------------
// 
TInt CPeninputFingerHwrArLayout::GetNewValue( 
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
TInt CPeninputFingerHwrArLayout::GetPenTrailColor()
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
void CPeninputFingerHwrArLayout::SetWritingSpeed( const TInt aWritingSpeed )
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
void CPeninputFingerHwrArLayout::SetBoxPenSize( const TSize aPenSize )
    {
    iHwrWnd->SetBoxPenSize( aPenSize );
    }

// ---------------------------------------------------------------------------
// set hwr writing pen color.
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrArLayout::SetBoxPenColor( const TInt aPenColor )
    {
    iHwrWnd->SetBoxPenColor( aPenColor );
    }

// ---------------------------------------------------------------------------
// set hwr writing pen color.
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrArLayout::SetGuideLineOn(const TBool aGuideLineOn)
    {
    iHwrWnd->SetGuideLineOn( aGuideLineOn );
    }

// ---------------------------------------------------------------------------
// get current state manager.
// ---------------------------------------------------------------------------
// 
CPeninputFingerHwrArStateManagerBase* CPeninputFingerHwrArLayout::CurStateManager()
    {
    return iStateMgrArray[0];
    }


// ---------------------------------------------------------------------------
// change current state manager to standby state.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrArLayout::ChangeCurStateManagerToStandby()
    {
    CPeninputFingerHwrArStateManagerBase* stateMgr = CurStateManager();
    if ( stateMgr )
        {
        stateMgr->SetState( CPeninputFingerHwrArStateManagerBase::EStateStandBy );
        }
    }

//End of file
