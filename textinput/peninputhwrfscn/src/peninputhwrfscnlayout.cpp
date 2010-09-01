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

//SYSTEM INCLUDES
#include <bautils.h>
#include <coemain.h>

//FEP Includes
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <peninputlayoutchoicelist.h>

//USER INCLUDES
#include "peninputhwrfscncandidatepanel.h"
#include "peninputhwrfscncontrolbar.h"
#include "peninputhwrfscndatastore.h"
#include "peninputhwrfscnstatemanager.h"
#include "peninputhwrfscnwritingbox.h"
#include "peninputhwrfscnevent.h"
#include "peninputhwrfscnlayout.h"

//CONST DEFINATION
_LIT( KFSHWRRssFile, "\\resource\\peninputhwrfscreencn.rsc" );
const TUint8 KTempBufSize = 8;
const TUint8 KIntLen = sizeof( TInt )/2; 
const TUint8 KIntSize = sizeof( TInt );

// ============================ MEMBER FUNCTIONS ===============================
  
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnLayout* CPeninputHwrfscnLayout::NewL( 
    MLayoutOwner* aLayoutOwner, const TAny* aInitData )
    {
    CPeninputHwrfscnLayout* self = new( ELeave ) 
        CPeninputHwrfscnLayout( aLayoutOwner );
    CleanupStack::PushL( self );
    self->ConstructL( aInitData );
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::CPeninputHwrfscnLayout()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnLayout::CPeninputHwrfscnLayout( MLayoutOwner* aLayoutOwner )
    :CPenInputFullScreenLayoutBase( aLayoutOwner )
    {  
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::~CPeninputHwrfscnLayout()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnLayout::~CPeninputHwrfscnLayout()
    {
    CCoeEnv::Static()->DeleteResourceFile( iResId );
    
    delete iDataStore;
    delete iStateMgr;
    delete iFsHwrBox;
    delete iCandidatePanel;
    delete iControlBar;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::ConstructL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnLayout::ConstructL( const TAny* aInitData )
    {
    BaseConstructL();    
    //load resource
    TFileName resFileName( KFSHWRRssFile );
    CCoeEnv* coeEnv = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile( coeEnv->FsSession(), resFileName );
    iResId = coeEnv->AddResourceFileL( resFileName );
    
    //initialze the data store
    TPeninputLayoutData* layoutData = (TPeninputLayoutData*)aInitData;    

    iDataStore = CPeninputHwrfscnDataStore::NewL( (*layoutData).iPtiEngine);
    
    //create state manager   
    CreateStateManagerL(); 
    
    //create the transparent window
    CreateHwrWindowL();
    
    //create the candidate panel
    CreateCandidatePanelL();
    
    //create the control panel
    CreateControlBarL();
    
    //set screen layout extent
    SetRect( TRect( TPoint( 0, 0 ), ScreenSize() ) );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::OnDeActivate()
// .
// -----------------------------------------------------------------------------
//        
void CPeninputHwrfscnLayout::OnDeActivate()
	{
	//CFepUiLayout::OnDeActivate();	
	CPenInputFullScreenLayoutBase::OnDeActivate();
	iFsHwrBox->CancelCharWriting();
	TRAP_IGNORE( NotifyOutsideEventL( ECandidatePanel | ECtrlBar ) );	
	}
	
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::CreateControlBarL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnLayout::CreateControlBarL()
	{
	iControlBar = CPeninputHwrfscnControlBar::NewL( this );
	iControlBar->AddEventObserver( this );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::CreateCandidatePanelL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnLayout::CreateCandidatePanelL()
	{
	iCandidatePanel = CPeninputHwrfscnCandidatePanel::NewL( this );
	iCandidatePanel->AddEventObserver( this );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::CreateHwrWindowL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnLayout::CreateHwrWindowL()
	{	
	iFsHwrBox = CPeninputHwrfscnWritingBox::NewL( this );
	iFsHwrBox->AddEventObserver( this );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::CreateStateManagerL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnLayout::CreateStateManagerL()
    {
    iStateMgr = CPeninputHwrfscnStateManager::NewL( this );
    }
    	
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::HandleCommand()
//  handle the command from Fep
// -----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnLayout::HandleCommand( TInt aCmd, TUint8* aData )
    {
	if( CFepUiLayout::HandleCommand( aCmd, aData ) == KErrNone )
	    {
	    return KErrNone;
	    }
	
	TInt ret = KErrUnknown;   
	switch( aCmd )
        {
        case ECmdPenInputEditorNumericKeyMap:
            {
            iDataStore->SetNumberMode( *( TAknEditorNumericKeymap* )aData );
            
            ret = KErrNone;
            }
            break;
            
        case ECmdPenInputPermittedRange:
            {
            TInt ranges = *( TInt* )aData;
            iDataStore->SetPermittedRanges( ranges );
            
            ret = KErrNone;
            }
            break;
        
        case ECmdPenInputLanguage:
            {
            TInt language = *( TInt* )aData;
            TRAP_IGNORE( iDataStore->SetLanguageL( language ) );
            
            ret = KErrNone;
            }
            break;
        
        case ECmdPenInputRange:
            {
            TInt range = *( TInt* )aData;
            iDataStore->SetPrimaryRange( range );
            
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
   
        case ECmdPenInputEditorCustomNumericKeyMap:
            {
            TInt len = *( TInt* )( aData - KIntSize );
            TPtrC16 keymapRes( ( TUint16* )aData, len / 2 );
            TRAP_IGNORE( iDataStore->SetNumberModeL( keymapRes ) );
            
            ret = KErrNone;
            }
            break;
            
        case ECmdPenInputWindowOpen:
            {
            LayoutOwner()->Hide( EFalse );
            TRAP_IGNORE( NotifyOutsideEventL( ECandidatePanel | ECtrlBar ) );
            RootControl()->UpdateValidRegion(NULL, EFalse);
            TPixelsTwipsAndRotation size; 
            CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
            TSize wndSize(size.iPixelSize.iWidth,size.iPixelSize.iHeight);
            iDataStore->SetInputAreaSize(wndSize);
            iDataStore->SetScreenSize(wndSize);
            ret = KErrNone;
            }
            break;
            
        case ECmdPenInputWindowClose:
            {
            // clear stroke on the screen when close FS-HWR before writing time out 
            //iFsHwrBox->CancelCharWriting();
            TRAP_IGNORE( NotifyOutsideEventL( ECandidatePanel | ECtrlBar ) ); 
            LayoutOwner()->Hide( ETrue );           
            
            ret = KErrNone;
            }
            break;
        
        case ECmdPenInputEnableSettingBtn:    
            {
            iControlBar->SetEnableSettingBtn(*aData);
            ret = KErrNone;
            }
            break;
    
        default:
            break;
        }
          
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::SizeChanged()
// 
// -----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnLayout::SizeChanged( const TAny* /*pData*/ )
    {
    //Handle size change event

	// To clear the stroke on the screen when rotate device
    //iFsHwrBox->CancelCharWriting();
    SetRect( TRect( TPoint( 0, 0 ), ScreenSize() ) );
    iFsHwrBox->SizeChanged();
    iCandidatePanel->SizeChanged();
    iControlBar->SizeChanged();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::HandleControlEvent()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnLayout::HandleControlEvent( TInt aEventType, 
    CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventKeyBack:
        case EHwrEventBtnMoreFunction:
        case EHwrEventSetRangeChinese:
        case EHwrEventSetRangeEnglish:
        case EHwrEventSetRangeNumber:
        case EHwrEventRangeClick:
        case EEventButtonDown:
        case EHwrEventClose:
        case EHwrEventTouchInputOption:
        case EEventChoiceSelected:            
            {           
            TRAP_IGNORE( HandleControlBarEventL( aEventType, aCtrl, aEventData ) );            
            }
            break;
            
        case EHwrEventKeySpace:
        case EHwrEventKeyEnter:
//        case EHwrEventInputLangSwitch:
        case EHwrEventVkbSwitch:
        case EEventVirtualKeyUp:
            {
            TRAP_IGNORE( HandleFunctionListEventL( aEventType, aCtrl, aEventData ) );
            }
            break;
            
        case EHwrEventBeginWriting:
        case EHwrEventEndStroke:
        case EHwrEventEndWriting:
        case EHwrEventCandidateSelected:
        case EHwrEventPredictiveSelected:
            {
            TRAP_IGNORE( HandleHwrWndEventL( aEventType, aEventData ) );
            }
            break;
            
        case EHwrEventGetNextCandidatePage:
            {
            TRAP_IGNORE( HandleCandidateWndEventL( aEventType, aEventData ) );
            }
            break;  
                      
        default:
        	break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::HandleRawKeyEventL()
// .
// -----------------------------------------------------------------------------
//    
TBool CPeninputHwrfscnLayout::HandleRawKeyEventL( const TRawEvent& /*aKeyEvent*/ )
    {
    NotifyOutsideEventL( ECandidatePanel | ECtrlBar );
    return EFalse;            
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::HandleControlBarEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnLayout::HandleControlBarEventL( TInt aEventType,
                                                     CFepUiBaseCtrl* /*aCtrl*/,
                                                     const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventKeyBack:
            {
            if ( !StrokeFilter() )
                {
                iFsHwrBox->CancelCharWriting();
                }
            else
                {
                TBuf<1> bufBck;
                bufBck.Append( EKeyBackspace );
                SubmitStringToFep( bufBck );    
                }
            NotifyOutsideEventL( ECandidatePanel );
            }   
            break;
            	
        case EHwrEventSetRangeChinese:
            {
            iDataStore->SetPrimaryRange( ERangeNative );
            iDataStore->SaveRange( ERangeNative );
            NotifyPrimaryRangeChanged( ERangeNative );
            iControlBar->Draw();
            }
            break;
            
        case EHwrEventSetRangeEnglish:
            {
            iDataStore->SetPrimaryRange( ERangeEnglish );
            iDataStore->SaveRange( ERangeEnglish );
            NotifyPrimaryRangeChanged( ERangeEnglish );
            iControlBar->Draw();
            }
            break;
            
        case EHwrEventSetRangeNumber:
            {
            iDataStore->SetPrimaryRange( ERangeNumber );
            iDataStore->SaveRange( ERangeNumber );
            NotifyPrimaryRangeChanged( ERangeNumber );
            iControlBar->Draw();
            }
            break;
            
        case EHwrEventRangeClick:
        case EHwrEventBtnMoreFunction:
            {
            iFsHwrBox->CancelCharWriting();
            NotifyOutsideEventL( ECandidatePanel );
            }
            break;
            
        case EHwrEventTouchInputOption:
            {
            SignalOwner( ESignalLaunchOptionMenu );
            }
            break;
                    
        case EEventChoiceSelected:
            {
            CFepLayoutChoiceList::SEvent* event = 
                ( CFepLayoutChoiceList::SEvent* )aEventData.Ptr();
            if ( event->iIndex == -1 )
                {
                return;
                }                
            
            TBool switchByMode = ETrue;
            TInt  mode = event->iCommand;

            TBuf<KTempBufSize> buf;
            buf.Append( reinterpret_cast<TText*>( &switchByMode ), 
                sizeof( TBool )/sizeof( TText ) );
            buf.Append( reinterpret_cast<TText*>( &mode ),
                sizeof( TInt )/sizeof( TText ) );
            SignalOwner( ESignalLayoutUIChanged, buf );
            }
            break;
            
        case EHwrEventClose:
            {
            SignalOwner(ESignalLayoutClosed);
            }
            break;
            
        default:
        	break;            
        }                
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::HandleFunctionListEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnLayout::HandleFunctionListEventL( TInt aEventType,
                                                       CFepUiBaseCtrl* /*aCtrl*/,
                                                       const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventKeyEnter:
            {
            TBuf<1> bufEnter;
            bufEnter.Append( EKeyEnter );
            SubmitStringToFep( bufEnter );
            }
            break;
            	
        case EHwrEventKeySpace:
            {
            if ( !iDataStore->IsNumberOnly() )
                {
                TBuf<1> bufSpace;
                bufSpace.Append( EKeySpace );
                SubmitStringToFep( bufSpace );
                }
            }
            break;
            
        case EEventVirtualKeyUp:
            {
            TBuf<KTempBufSize> buf;
            TVirtualKeyEventData* event = ( TVirtualKeyEventData* ) aEventData.Ptr();
            buf.Append( event->iKeyData );
            //the special char: smilefont char and cryfont char
            if( iDataStore->IsSpecialDisplayChars( buf ) )
                {
                HBufC* convertedBuf = iDataStore->ConvertDisplayChars( buf );
                buf = *convertedBuf;
                delete convertedBuf;
                }
            SubmitStringToFep( buf );
            }
            break;
        }
        
    }                                                     

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::HandleHwrWndEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnLayout::HandleHwrWndEventL( TInt aEventType,
    const TDesC& aEventData )
    {   
    if( aEventType == EHwrEventBeginWriting )
        {
        NotifyOutsideEventL( ECandidatePanel | ECtrlBar );
        }

    iStateMgr->HandleEventL( aEventType, aEventData );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::HandleCandidateWndEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnLayout::HandleCandidateWndEventL( TInt /*aEventType*/,
    const TDesC& /*aEventData*/ )
    {
    iDataStore->GetNextPredictivePageL();
    }
                                                           
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::DataStore()
// .
// -----------------------------------------------------------------------------
//	
CPeninputHwrfscnDataStore& CPeninputHwrfscnLayout::DataStore()
    {
    return *iDataStore;
    }
 
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::OnAppEditorTextComing()
// .
// -----------------------------------------------------------------------------
//	    
TInt CPeninputHwrfscnLayout::OnAppEditorTextComing( 
    const TFepInputContextFieldData& aData )
    {
    iCandidatePanel->HandleCommand( EHwrEventCursorPosChanged,
        ( TUint8* )&aData.iRect );
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::PreHandleEvent()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnLayout::PreHandleEvent( TEventType /*aType*/, 
    const TRawEvent& /*aEvent*/ )
    {
 
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::BeforeSendingEventsToBackground()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnLayout::BeforeSendingEventsToBackground()
    {
    TRAP_IGNORE( NotifyOutsideEventL( ECandidatePanel | ECtrlBar ) );
    }
        
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::PenInputUiType()
// .
// -----------------------------------------------------------------------------
//	
TInt CPeninputHwrfscnLayout::PenInputType()
	{
	return EPluginInputModeFSc;
	}
	
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::NotifyOutsideEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnLayout::NotifyOutsideEventL( TInt aPanel )
    {
    if( aPanel & ECandidatePanel && aPanel & ECtrlBar )
        {
        iCandidatePanel->HandleCommand( EHwrEventPointerOutsideWnd, NULL );		
    	iControlBar->HandleCommand( EHwrEventPointerOutsideWnd, NULL );
    	iStateMgr->HandleEventL( EHwrEventPointerOutsideWnd, KNullDesC );
        }
    else if( aPanel & ECandidatePanel )
        {
        iCandidatePanel->HandleCommand( EHwrEventPointerOutsideWnd, NULL );	
        iStateMgr->HandleEventL( EHwrEventPointerOutsideWnd, KNullDesC );
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::NotifyPrimaryRangeChanged()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnLayout::NotifyPrimaryRangeChanged( TInt aPrimaryRange )    
    {
    TBuf<KIntLen> buf;
    buf.Append( ( TUint16* )( &aPrimaryRange ), KIntLen );
    SignalOwner( ESignalRange, buf );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::Replace()
// Delete the previous charcode and submit the new one
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnLayout::Replace( const TDesC& aOldCharCode, 
    const TDesC& aNewCharCode )
    {
    // user tries to correct the previous selection
    // Delete the previous character and append the new one
    HBufC* newCharBuf = HBufC::New( aNewCharCode.Length() + KIntLen );
    if( newCharBuf )
        {
        TInt len = aOldCharCode.Length();
        newCharBuf->Des().Append( ( TUint16* )&len, KIntLen );
        newCharBuf->Des().Append( aNewCharCode );
        SignalOwner( ESignalReplaceText, *newCharBuf );
        
        delete newCharBuf;
        }
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::SubmitStringToFep()
// Submit the char code.
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnLayout::SubmitStringToFep( const TDesC& aCharCode )
    {
    SignalOwner( ESignalKeyEvent, aCharCode );	        	
    }
     
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::OnResourceChange()
// Submit the char code.
// -----------------------------------------------------------------------------
//   
TInt CPeninputHwrfscnLayout::OnResourceChange(TInt aType)
	{
	CFepUiLayout::OnResourceChange(aType);
	if( aType == KAknsMessageSkinChange )
		{
		// Do other things when skin change
		iControlBar->HandleSkinChanged();			
		}
	return KErrNone;
	}
// -----------------------------------------------------------------------------
// CPeninputHwrfscnLayout::CandidatePanel()
// .
// -----------------------------------------------------------------------------
//    
CPeninputHwrfscnCandidatePanel& CPeninputHwrfscnLayout::CandidatePanel()
	{
	return *iCandidatePanel;
	}

void CPeninputHwrfscnLayout::ClearScreen()
    {
    if (iFsHwrBox)
        {
        iFsHwrBox->CancelCharWriting();
        TRAP_IGNORE(NotifyOutsideEventL( ECandidatePanel ));
        }
    }

CPeninputHwrfscnWritingBox& CPeninputHwrfscnLayout::HwrfscnWritingBox()
    {
    return *iFsHwrBox;
    }
//End of file
