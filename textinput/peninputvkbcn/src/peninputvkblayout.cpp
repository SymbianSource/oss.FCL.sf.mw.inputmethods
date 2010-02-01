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
* Description:  Implementation of the vkb layout
*
*/
 

// system includes
#include <aknfeppeninputenums.h>    //command from fep or IME to plugin
#include <AknFepGlobalEnums.h> //define EPluginInputModeHwr
#include <peninputcmd.h>            // use global signal
#include <peninputlayoutchoicelist.h>
#include <peninputlayoutbutton.h>
#include <peninputdragbutton.h>
#include <coemain.h>
#include <PtiEngine.h>
#include <PtiUserDicEntry.h>
#include <peninputpluginutils.h>
#include <bautils.h>
#include <peninputmultimodebutton.h>    // add by jiawenjuan
#include <peninputlayoutmultilineicf.h>
#include <peninputlayoutbubblectrl.h>
#include <s32mem.h>

// user includes
#include "peninputvkblayout.h"  
#include "peninputvkbevent.h"         // use event
#include "peninputvkb.hrh"            // control ID and range defination
#include "peninputvkbwindow.h"        // use vkb window, which derived from base window
#include "peninputvkbuistate.h"       // use ui state
#include "peninputvkbuistatemgr.h"    // use ui state mgr
#include "peninputvkbdatamgr.h"       // use data mgr
#include "peninputvkbclientarea.h"

// constant definition
_LIT(KResourceFile, "z:\\resource\\peninputvkbcn.rsc");

const TInt KIntSizeToInt16 = 2;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CAknFepVkbLayoutImp::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbLayoutImp* CAknFepVkbLayoutImp::NewL( TAny* /*aInitParams*/ )
    {
    CAknFepVkbLayoutImp* self = new(ELeave) CAknFepVkbLayoutImp();
    CleanupStack::PushL(self);
    self->ConstructL( EFalse ); 
    CleanupStack::Pop();

    return self;
    }

CAknFepVkbLayoutImp* CAknFepVkbLayoutImp::NewFsqL( TAny* /*aInitParams*/ )
    {
    CAknFepVkbLayoutImp* self = new ( ELeave ) CAknFepVkbLayoutImp();
    CleanupStack::PushL( self );
    self->ConstructL( ETrue ); 
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayoutImp::~CAknFepVkbLayoutImp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbLayoutImp::~CAknFepVkbLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayoutImp::CAknFepVkbLayoutImp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbLayoutImp::CAknFepVkbLayoutImp()
    {
    // See ConstructL() for initialisation completion.
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayoutImp::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayoutImp::ConstructL(TBool aIsFsq)
    {
    iIsFsq = aIsFsq;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayoutImp::CreateFepUiLayoutL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiLayout* CAknFepVkbLayoutImp::CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,const TAny* aData)
    {
    CAknFepVkbLayout* uiLayout = CAknFepVkbLayout::NewL( aLayoutOwner, aData, iIsFsq );

    return uiLayout;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbLayout* CAknFepVkbLayout::NewL(MLayoutOwner* aLayoutOwner, 
                                         const TAny* aInitData, TBool aIsFsq )
    {
    CAknFepVkbLayout* self = new (ELeave) CAknFepVkbLayout(aLayoutOwner);

    CleanupStack::PushL(self);
    self->ConstructL(aInitData, aIsFsq);
    CleanupStack::Pop();

    return self;  
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::CAknFepVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbLayout::CAknFepVkbLayout(MLayoutOwner* aLayoutOwner)
    : CFepUiLayout(aLayoutOwner), iNeedDealFocusGained(ETrue)    
    {
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::ConstructL(const TAny* aInitData, TBool aIsFsq)
    {
    iLayoutType = aIsFsq ? EPluginInputModeFSQ : EPluginInputModeVkb;
    BaseConstructL();

    CCoeEnv* coeEnv = CCoeEnv::Static();
    TPixelsTwipsAndRotation ptSize;     

    TFileName resFileName( KResourceFile );
    BaflUtils::NearestLanguageFile(coeEnv->FsSession(), resFileName );

    coeEnv->ScreenDevice()->GetDefaultScreenSizeAndRotation(ptSize);
    iResId = coeEnv->AddResourceFileL( resFileName );
    TPeninputLayoutData* layoutData = (TPeninputLayoutData*)aInitData;
    iPtiEngine = (*layoutData).iPtiEngine;

    //create date manager
    iDataMgr = CAknFepVkbDataMgr::NewL(this, iPtiEngine);

    // Store screen mode, and get new window position
    iDataMgr->SetScreenMode(ptSize);

    //Create ui state manager
    iUiStateMgr = CAknFepVkbUiStateMgr::NewL(this);

    //create vkb window
    iVkbWindow = CAknFepVkbWindow::NewL(this, 
                                        EAknFepVkbCtrlIdVkbWindow, 
                                        iDataMgr,
                                        iUiStateMgr);
    iVkbWindow->Hide(ETrue);
    AddControlL(iVkbWindow);
    
    //config layout self using the whole screen
    //set proper size
    SetScreenSize(ptSize.iPixelSize);

    // When layout is initianized, we should make the entire window visible
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::~CAknFepVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbLayout::~CAknFepVkbLayout()
    {
    delete iDataMgr;
    CCoeEnv::Static()->DeleteResourceFile(iResId);
    delete iUiStateMgr;
    }        

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::HandleCommand
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbLayout::HandleCommand(TInt aCmd, TUint8* aData)
    {
    // Handle command from fep or IME to this layout
    TUint* data = (TUint*) aData;
    TBuf<KIntSizeToInt16> bufStyle;
    TInt style;
    TInt ret = KErrNone;
                
    switch ( aCmd )
        {
        case ECmdPenInputPermittedRange:
            iDataMgr->SetPermittedRange(*data);
            break;
        case ECmdPenInputEditorNumericKeyMap:
            iDataMgr->SetEditorNumericKeymap(*data);
            break;
        case ECmdPenInputCase:
            DoCaseChange(*data);
            break;
        case ECmdPenInputPermittedCase:
            iDataMgr->SetPermittedCase(*data);
            break;
        case ECmdPenInputLanguage:
            {
            ChangeInputLanguage(*data);
            iVkbWindow->SetSwitchlistLanguage(*data);
            }
            break;
        // When data query need to change position    
        case ECmdPenInputSetWindowPos:
            iDataMgr->SetDataQueryRect(*((TRect*)aData));
            
            style = EVkbPositionChangeDataQuery;
            bufStyle = ( TUint16* )&style;
            SendEventToVkbControl(EVkbEventPositionChanged, NULL, bufStyle);
            break;
        case ECmdPenInputWindowOpen:  
            if ( *data == EPenInputOpenManually )
                {
                style = EVkbPositionChangeBrJustify;
                bufStyle = ( TUint16* )&style;
                SendEventToVkbControl(EVkbEventPositionChanged, NULL, bufStyle );  
                }
			iVkbWindow->Hide(EFalse);
            LayoutOwner()->Hide(EFalse);                     
            break;
        case ECmdPenInputWindowClose:
			iVkbWindow->Hide(ETrue);
            LayoutOwner()->Hide(ETrue);            
            break;
        case ECmdPenInputRange:
            SetCurrentRange(*data);
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
            {
            iDataMgr->SetData(EAknFepDataTypeUseDefinedResource, (TInt) aData);
            }
            break;
        case ECmdPenInputEnableSettingBtn:
            {
            iVkbWindow->SetEnableSettingBtn(*data);
            }
            break;
        case ECmdPenInputIsSecretText:
            {
            iVkbWindow->SetSwitchlistSecretFlag(*data);

            iVkbWindow->SetTextIsSecret( *aData );
            }
            break;
        case ECmdPenInputDimArrowKeys:
            {
            TBool IsDimArrowKeys = *aData;
            iVkbWindow->DimArrowKeys( IsDimArrowKeys );
            }
            break;
        case ECmdPenInputSetPromptText:
            {
            if( iLayoutType == EPluginInputModeFSQ )
            TRAP_IGNORE(iVkbWindow->SetPromptTextL(aData));
            }
            break;
        case ECmdPenInputCharacterPreview:
            {
            iVkbWindow->ShowBubble(*aData);    
            }
            break;
        case ECmdPenInputSetTextAlignment:
        	{
		    TRAP_IGNORE(iVkbWindow->SetTextAlignmentL( *aData ));
        	}
        	break;        
        case ECmdPenInputPopupTooltip:
            {    
            TRAP_IGNORE( HandleShowTooltipOnFSQCmdL( aData ) );
            }
            break;
        case ECmdPenInputHideTooltip:
            {
            iVkbWindow->HideTooltipOnFSQ();
            }
            break;
        case ECmdPenInputPopupCandidateList:
            {
            TRAP_IGNORE( HandleShowCandidateListOnFSQCmdL( aData ) );
            }
            break;
        case ECmdPenInputHideCandidateList:
            {
            iVkbWindow->HideCandidateListOnFSQ();
            }
            break;
        case ECmdPeninputITIStatus:
            {
            // Set the flag to indicate if FSQ with ITI feature is opened
            iITIEnabled = *aData ;
            }            
            break;
        default:
            {
            ret = CFepUiLayout::HandleCommand( aCmd, aData );
            }
            break;
        }
            
    return ret;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::SizeChanged
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbLayout::SizeChanged(const TAny* /*pData*/)
    {
    // Handle size changed event
    TPixelsTwipsAndRotation size; 
    
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
        
    // Store screen mode, and get new window position
    iDataMgr->SetScreenMode(size);
    
    iDataMgr->SetSizeChanging(ETrue);
     // The following need to implement
    iVkbWindow->SizeChanged(ETrue);
    iDataMgr->SetSizeChanging(EFalse);

    // When layout size is changed, we should make the entire window visible
    TInt style = EVkbPositionChangeBrJustify;
    TBuf<KIntSizeToInt16> bufStyle;
    bufStyle = ( TUint16* )&style;
    SendEventToVkbControl(EVkbEventPositionChanged, NULL, bufStyle);
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::OnAppEditorTextComing
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbLayout::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
    {
    // Handle edit text coming
    if (iVkbWindow)
        {
        TRAPD(err, iVkbWindow->SetEditorTextL(aData));
        return err;
        }
        
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbLayout::HandleControlEvent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                          const TDesC& aEventData)
    {        
    // Handle event from control
    TInt* data = (TInt*)aEventData.Ptr();  
    
    //static TInt COUNT = 1; 
    
    //deal with the event from all controls in layout
    switch (aEventType)
        {
        case EVkbEventWindowClose:
            iVkbWindow->ClientArea()->HandleControlEvent( aEventType, aCtrl, aEventData );
            SignalOwner(ESignalLayoutClosed);
            break;
        case EVkbEventTouchInputOption:
        case EPeninputLayoutEventOption:
            SignalOwner(ESignalLaunchOptionMenu);
            break;
        case EVkbEventInputLangSwitch:
            SignalOwner(ESignalLaunchLanguageMenu);
            break;
        
        //When drag end, store the data
        case EEventDraggingEnd:
            {
            TInt x = 0;
            TInt y = 0;
            x = 0x0000FFFF & ( Position().iX + Rect().Width());
            y = 0xFFFF0000 & ( (Position().iY + Rect().Height() ) << 16 );
            if ( x|y )
                {
                iDataMgr->SetWindowPosition(x | y);
                }
            }
            break;
            
        case EVkbEventPositionChanged:
            SendEventToVkbControl(EVkbEventPositionChanged,NULL);
            break;
            
        case EVkbEventHwr:
            {
            TRAP_IGNORE( iVkbWindow->UpdateICFTextL() );
            iVkbWindow->PopupSwitchWindow();                       
            }
            break;
        
        case EVkbEventSetRange:
            if ( *data == ERangeAccent )
                {
                iVkbWindow->PopupAccentWindow();
                }
            else
                {
                // When change range mannually, we should make the entire window visible
                //iVkbWindow->Hide(ETrue);
                SetCurrentRange(*data,ETrue,ETrue);
                //iVkbWindow->Hide(EFalse);
           
                //singal server
                TInt range = *data;
                TBuf<4> buf;
                buf.Append(reinterpret_cast<TText*>(&range), 4);
                SignalOwner(ESignalRange, buf);                      
                }
            break;
            
        case EEventChoiceSelected:
            {
            CFepLayoutChoiceList::SEvent* event = (CFepLayoutChoiceList::SEvent*)aEventData.Ptr();
            if( event->iIndex != -1 )
                {
                if (aCtrl->ControlId() == EAknFepVkbPopupWindow)
                    {
                TInt currentRange = iDataMgr->CurrentRange();
                TInt currentAccent = iDataMgr->CurrentAccent();
                if ( (currentRange == ERangeAccent) && ( currentAccent == event->iCommand ) )
                    {
                    return;
                    }
                else
                    {
                    iDataMgr->SetCurrentAccent(event->iCommand);
                    // When change range mannually, we should make the entire window visible
                    SetCurrentRange(ERangeAccent,EFalse, ETrue);

                    TInt style = EVkbPositionChangeBrJustify;
                    TBuf<KIntSizeToInt16> bufStyle;
                    bufStyle = ( TUint16* )&style;
                    SendEventToVkbControl(EVkbEventPositionChanged, NULL, bufStyle);
                    
                    //singal server
                    TInt range = ERangeAccent;
                    TBuf<4> buf;
                    buf.Append(reinterpret_cast<TText*>(&range), 4);
                    SignalOwner(ESignalRange, buf);                      
                        }
                    
                    }
                else if (aCtrl->ControlId() == EPeninutWindowCtrlIdSwitcherPopupWindow)
                    {
                    if (iLayoutType != event->iCommand)
                        {
                        TBool switchByMode = ETrue;
                        TInt  mode = event->iCommand;

                        TBuf<8> buf;
                        buf.Append(reinterpret_cast<TText*>(&switchByMode), 
                                   sizeof(TBool)/sizeof(TText));
                        buf.Append(reinterpret_cast<TText*>(&mode), 
                                   sizeof(TInt)/sizeof(TText));
                        SignalOwner(ESignalLayoutUIChanged,buf);
                        }
                    }
                else if ( aCtrl->ControlId() == EPentinputHwrCandidateList )
                    {
                    TRAP_IGNORE( iVkbWindow->HandleCandidateListSelectedOnFSQL( aCtrl, aEventData ) );
                    }
                }
            else
                {
                iVkbWindow->ResetAccentButton();                
                }
            }

            break; 
        case EVkbEventCompFieldAnalysisReq:
            //Send the data to engine to recognize
            TRAP_IGNORE(iDataMgr->GetCandidatesL(aEventData,EFalse));
            break;

        case EVkbEventCompFieldSubmit:
            {
            SignalOwner(ESignalKeyEvent,aEventData);
            iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn()); 
            ForceEditGainFocus(EInputContextFieldGetFocus);
            
            // predictive char is selected
            TRAP_IGNORE(iDataMgr->GetCandidatesL(aEventData,ETrue));
            
            // create new word if needed
            if ((iVkbWindow->NeedCreateWord()) && (aEventData.Length() > 0))
            	{
            	
            	// only bind spell sequence in zhuyin or pinyin mode
            	if ( iDataMgr->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnZhuyin
                    || iDataMgr->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnPinyin )
            	    {
            	    TPtrC buf = iVkbWindow->GetCreatedWordSpell();
            	    TPtiUserDictionaryEntry newUdbEntry(buf);
                    iPtiEngine->AddUserDictionaryEntry(newUdbEntry);
            	    }
            	else 
            	    {
            	    TPtiUserDictionaryEntry newUdbEntry(aEventData);
                    iPtiEngine->AddUserDictionaryEntry(newUdbEntry);
            	    }
                }
            }
            break;

        case EVkbEventCandidateSelected:
            if ( !iVkbWindow->CompositionFieldStringLength() )
                {
                //First send it to fep
                TBuf<32> buf = aEventData;
                buf.SetLength(buf.Length()-1);
                SignalOwner(ESignalKeyEvent,buf);
                
                // predictive char is selected
                TRAP_IGNORE(iDataMgr->GetCandidatesL(buf,ETrue));
                ForceEditGainFocus(EInputContextFieldGetFocus);
                }
            else
                {
                // candidate char is selected
                SendEventToVkbControl(EVkbEventCandidateSelected,aCtrl,aEventData);
                }
            break;
       	case EVkbEventQueryCandidateExisted:
       	    {
            if (iDataMgr->Candidates().Count() >= KMaxPredictCandCnt)
                {
                iVkbWindow->ClientArea()->SetNextCandidateExistFlag(EFalse);
                }
            else
                {
                iVkbWindow->ClientArea()->SetNextCandidateExistFlag
                    (iDataMgr->NextCandidateExist());
                }          
       	    }
       	    break;
       	case EVkbEventGetNextPageCandidate:
       	    {
            TRAP_IGNORE(iDataMgr->GetCandidatesL(KNullDesC, EFalse, ETrue));
       	    }
       	    break;
       	case EVkbEventGetPreviousPageCandidate:
       	    // actually, 'get previous page candidate' do nothing here
       	    // the branch here just to avoid it is handled by 'default' branch
            break;
        case EVkbEventStandby2Composition:
        case EVkbEventComposition2Standby:
            SendEventToVkbControl(aEventType, aCtrl, aEventData);
            break;
            
        case EVkbEventCompFieldDataFlush:
            SendEventToVkbControl(aEventType,aCtrl,aEventData);
            iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn()); 
            ForceEditGainFocus(EInputContextFieldGetFocus);         
            break;
            
        case EVkbEventCompFieldNoChars:       
            SendEventToVkbControl(EVkbEventComposition2Standby,NULL);            
        case EVkbEventClearContent:
            SendEventToVkbControl(aEventType,aCtrl,aEventData);
            break;
        
        case EEventControlFocusGained:
            HandleFocusGained(aCtrl,aEventData);
            break;
        
        case EEventButtonUp:
        	{
        	// check current range
		    TInt curRange = iDataMgr->CurrentRange();		    
        	if( aCtrl->ControlId() == EAknFepVkbCtrlIdShiftBtn)
	        	{
	        	if( (curRange == ERangeNative) || (curRange == ERangeEnglish) ||
	        	    (curRange == ERangeAccent))
	        		{
	        		CAknFepCtrlMultiModeButton* button = 
					static_cast<CAknFepCtrlMultiModeButton*>( aCtrl );
		        	TInt realCase = button->GetRealCaseByMode( button->CurrentMode() );	     	       		
					HandleMergedBtnClicked( realCase );
	        		}
		       	else
		       		{
		       		iVkbWindow->LoopVkbLayout(EVkbBoth, ETrue);
		       		TInt shiftStatus = iDataMgr->RequestData(EAknFepDataTypeShfitDown);
				    iVkbWindow->ChangeMergedButtonStatus( shiftStatus, 0 );
		       		}				
	        	}

        	}
        	break;
        case EEventButtonDown:            
            //Set length when pen down on backspace button
            iDataMgr->SetData(EAknFepDataTypeLengthWhenButtondownOnBack,
                              iVkbWindow->CompositionFieldStringLength());
            break;
            
        case EPeninputLayoutEventBack:
        case EVkbEventKeyEnter:
        case EVkbEventKeySpace:
        case EEventVirtualKeyUp:
        case EVkbEventLeftArrow:
        case EVkbEventRightArrow:        
        case EVkbEventUpArrow:
        case EVkbEventDownArrow:
             if ( ! iUiStateMgr->CurrentUiState()->HandleControlEvent(aEventType,aEventData) )
                 {
                 SendEventToVkbControl(aEventType,aCtrl,aEventData);
                 }
             break;
             
        //case EVkbEventCandidatesChanged:            // the event will be sent to control directly        
        //case EEventControlFocusLost:                // the event will be sent to control directly 
        //case EVkbEventCompFieldAnalysisResponse:    // the event will be sent to control directly 
        //case EVkbEventCompFieldCursorChange:        // no action      
        //    break;
        case EEventIcfPointerUpEvent:
            if(iDataMgr->RequestData( EAknFepDataTypeIMLayout ) != EAknFepVkbImNumber &&
               iDataMgr->RequestData( EAknFepDataTypeIMLayout ) != EAknFepVkbImLatin)
                {
                // Clear trigger string when changing cursor position
                iDataMgr->ClearTriggerString();
                }
            break;
        case EEventSetAppCursorSelection:
        	iUiStateMgr->CurrentUiState()->HandleControlEvent(aEventType,aEventData);
        	break;
        case EEventTooltipPointerUp:
            {
            // Signal to FEP to accept tooltip data 
            SignalOwner( ESignalAcceptTooltip );            
            }
            break;
        case EEventPointerDownOnInline:
            {
            // show candidate list when tap on active words
            SignalOwner( ESignalShowCandidate );            
            }
            break;        
        default:
            break;    
        }           
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::HandleShiftBtnClicked
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::HandleMergedBtnClicked( TInt aCase )
	{
	switch( aCase )
		{
		case ECaseLower:
		    {
		    iVkbWindow->LoopVkbLayout(EVkbCapslock, ETrue);
		    }
		    break;
		case ECaseUpper:
			{
			iVkbWindow->LoopVkbLayout(EVkbShift, ETrue);
			}
			break;
		case ECaseText:
//		case ECaseInverseText:
			{
			// Current case is Text, equals shift button
			iVkbWindow->LoopVkbLayout(EVkbBoth, ETrue);
			}
			break;	
		default:
			break;
		}
		
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::HandleRawKeyEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CAknFepVkbLayout::HandleRawKeyEventL(const TRawEvent& aKeyEvent)
    { 
    iVkbWindow->CloseAccentWindow();
    return iUiStateMgr->CurrentUiState()->HandleKeyEventL(aKeyEvent);
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::SendEventToVkbControl
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::SendEventToVkbControl(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                             const TDesC& aEventData)
    {
    iVkbWindow->HandleControlEvent(aEventType,aCtrl,aEventData);
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::Sendkey
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::Sendkey(TInt aEventType,const TDesC& aEventData)
    {
    SignalOwner(aEventType,aEventData); 
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::RequestData
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbLayout::RequestData(TAknFepDataType aDataType)
    {
    return iDataMgr->RequestData(aDataType);
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::Reset
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::Reset()
    {
    iDataMgr->Reset();
    
    ForceEditGainFocus(EInputContextFieldGetFocus);
    
    SendEventToVkbControl(EVkbEventClearContent,iVkbWindow,KNullDesC16);
    
    SendEventToVkbControl(EVkbEventResetShiftCapslock,iVkbWindow,KNullDesC16);
    
    iNeedDealFocusGained = ETrue;
    
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::SetData
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::SetData(TAknFepDataType aDataType,TInt aData)
    {
    iDataMgr->SetData(aDataType,aData);
    }
                
// ---------------------------------------------------------------------------
// CAknFepVkbLayout::SetData
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CAknFepVkbLayout::IsValidDestination(const TRect& aRect,
                                           CDragBar* aDragBar,
                                           TBool& aVInfo,TBool& aHInfo,
                                           TBool aFlag)
    {
    // Validate move button destination 
    if (!aDragBar)
        {
        return EFalse;
        }
    TRect rect = aRect;
    
    if( aFlag )
        {//layout is moving
        aHInfo = aVInfo = EFalse;
        TRect dragbarRect(aRect.iTl, aDragBar->Rect().Size());
        
        if( dragbarRect.iTl.iX >=0 && dragbarRect.iBr.iX < ScreenSize().iWidth )
            {
            aHInfo = ETrue;
            }
            
        if( dragbarRect.iTl.iY >=0 && dragbarRect.iBr.iY < ScreenSize().iHeight)
            {
            aVInfo = ETrue;
            }
        
        return aHInfo && aVInfo;
        }
    else
        {
        if (aDragBar)
            {
            rect.SetSize(aDragBar->Rect().Size());
            }
        TBool tlInvalid = !Rect().Contains(rect.iTl);
        TBool brInvalid = !Rect().Contains(rect.iBr);
        
        if (tlInvalid && !brInvalid)
            {
            if (rect.iTl.iX >= Rect().iTl.iX && rect.iTl.iX <= Rect().iBr.iX)
                {
                aHInfo = ETrue;
                }
            else if (rect.iTl.iY >= Rect().iTl.iY && rect.iTl.iY <= Rect().iBr.iY)
                {
                aVInfo = ETrue;
                }
            }
        else if (!tlInvalid && brInvalid)
            {
            if (rect.iBr.iX >= Rect().iTl.iX && rect.iBr.iX <= Rect().iBr.iX)
                {
                aHInfo = ETrue;
                }
            else if (rect.iBr.iY >= Rect().iTl.iY && rect.iBr.iY <= Rect().iBr.iY)

                {
                aVInfo = ETrue;
                }
            }

        return !(tlInvalid || brInvalid);
        }
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::ChangeInputLanguage
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::ChangeInputLanguage(TInt aLangID)
    {
    //1.Notify engine the language need to change
    //const TDesC& showText = iDataMgr->SetInputLanguage(aLangID);
    TInt switchDisplayLang = iDataMgr->SetInputLanguage(aLangID);
    TBuf<10> showText;    
    AknPenInputUtils::GetISOLanguageCode(TLanguage(switchDisplayLang), showText);
    
    SendEventToVkbControl(EVkbEventLanguageChanged,NULL,showText);     
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbLayout::SetCurrentRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::SetCurrentRange(TInt aRange, TBool aNeedSave, TBool aNotify)
    {             
    if ( aNeedSave  && ( iDataMgr->PermittedRange() & ERangeEnglish ) 
         && ( (aRange & ERangeNative) || (aRange & ERangeEnglish) ) )
        {
        //save to CenRep
        iDataMgr->WriteLastUsedRange(aRange);
        }
        
    Reset();
    
	iDataMgr->SetCurrentRange(aRange);
	iVkbWindow->ChangeImLayout(iDataMgr->IMLayout());
    
    //iVkbWindow->SizeChanged(ETrue);
    if ( aRange == ERangeNative )
        {
        iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());    
        }
    else
        {
        iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandby());    
        }
    
    if ( aNotify )
        {
        TInt style = EVkbPositionChangeBrJustify;
        TBuf<KIntSizeToInt16> bufStyle;
        bufStyle = ( TUint16* )&style;
        
        SendEventToVkbControl(EVkbEventPositionChanged, NULL, bufStyle);       
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbLayout::HandleFocusGained
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::HandleFocusGained(CFepUiBaseCtrl* aCtrl, const TDesC& aEventData)
    {       
    if ( !iNeedDealFocusGained || !iVkbWindow )
        {
        return;
        }
    
    SendEventToVkbControl(EEventControlFocusGained,aCtrl,aEventData);
   
    // Change current state machine
    if ( iDataMgr->CurrentRange() == ERangeNative )
        {
        if ( iVkbWindow->FocusedControlID() == EPeninputWindowCtrlIdMultiLineICF )
            {
            if ( iVkbWindow->CompositionFieldStringLength() )
                {
                iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycnWithChars()); 
                }
            else
                {
                iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateStandbycn());    
                }
            }
        else if ( iVkbWindow->FocusedControlID() == EAknFepVkbCtrlIdCompositionField )
           {
           if ( iVkbWindow->CompositionFieldStringLength() )
                {
                iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateCompositionWithChars()); 
                }
             else
                {
                iUiStateMgr->SetCurrentUiState(iUiStateMgr->UiStateCompositionNoChars());    
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::ForceEditGainFocus
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::ForceEditGainFocus(TInt aWhichEdit)
    {
    TBuf<KIntSizeToInt16> data;
    iNeedDealFocusGained = EFalse;//to avoid focus gain loop
    data = (TUint16*) &aWhichEdit;
    SendEventToVkbControl(EEventControlFocusGained,NULL,data);      
    iNeedDealFocusGained = ETrue; //reset the flag
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::CompositionFieldStringLength
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbLayout::CompositionFieldStringLength()
    {
    return iVkbWindow->CompositionFieldStringLength();
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbLayout::GetCandidatesL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbLayout::GetCandidatesL(const TDesC& aInputString, 
                                      const TBool aIsPredictive,
                                      TBool aForNextPage)
    {
    return iDataMgr->GetCandidatesL(aInputString, aIsPredictive, aForNextPage);
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::ClearTriggerString
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::ClearTriggerString()
    {
    iDataMgr->ClearTriggerString();
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::DoCaseChange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::DoCaseChange(TInt aCase)
    {
    TInt curCase = iDataMgr->Case();
    
    if ( curCase == aCase )
        {
        return;
        }
        
    iDataMgr->SetCase(aCase);
    
    if ( ( iDataMgr->CurrentRange() == ERangeEnglish ) ||
        ( iDataMgr->CurrentRange() == ERangeAccent ) )
        {
        switch (aCase)
            {
            case ECaseUpper: 
                iDataMgr->SetShiftDown(EFalse);
                iDataMgr->SetCapslockDown(ETrue);
                break;
            case ECaseLower: 
                iDataMgr->SetShiftDown(EFalse);
                iDataMgr->SetCapslockDown(EFalse);
                break;
            case ECaseText: 
                iDataMgr->SetShiftDown(ETrue);
                iDataMgr->SetCapslockDown(EFalse);
                break;
            case ECaseInverseText: 
                iDataMgr->SetShiftDown(ETrue);
                iDataMgr->SetCapslockDown(ETrue);
                break;
            default:  
                return;             
            }  
         
        iVkbWindow->LoopVkbLayout(EVkbNone, EFalse);
        } 
    else
        {
        iDataMgr->SetCase(ECaseInvalide);
        }   
    }

// ---------------------------------------------------------------------------
// CAknFepVkbLayout::PenInputUiType
// return current input UI type
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbLayout::PenInputType()
    {
    return iLayoutType;
    }

void CAknFepVkbLayout::SetPositionFromOutside(const TPoint& aNewPos)
    {
    SetLayoutPos(aNewPos);
    TInt x = 0x0000FFFF & aNewPos.iX;
    TInt y = 0xFFFF0000 & ( aNewPos.iY << 16 );  
    
    return SetData( EAknFepDataTypeWindowPosition, x | y );
    }
        
// ---------------------------------------------------------------------------
// CAknFepVkbLayout::Window
// return window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbWindow* CAknFepVkbLayout::Window() const
    {
    return iVkbWindow;
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::SemiTransparencyRequired
// .
// ---------------------------------------------------------------------------
//
TBool CAknFepVkbLayout::SemiTransparencyRequired()
    {
    return ETrue;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbLayout::HandleAppInfoChange
// .
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::HandleAppInfoChange(const TDesC& aInfo, 
                                           TPeninputAppInfo aType)
    {
    if ( iVkbWindow )
        {
        CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*> 
                                      (iVkbWindow->Control(EPeninputWindowCtrlIdMultiLineICF)) ;
        
        if ( icf && ( aType == EAppIndicatorMsg ) && (iLayoutType == EPluginInputModeFSQ) )
            { 
            if ( aInfo.Length() > 0 )
                {
                icf->ShowBubble(aInfo,icf->MsgBubbleCtrl()->Rect());
                }
            else
                {
                icf->HideBubble();
                }
            }            
        }
    }

// ---------------------------------------------------------------------------
// Handle show tooltip command.
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::HandleShowTooltipOnFSQCmdL( TUint8* aData )
    {
    if ( !iITIEnabled || iLayoutType != EPluginInputModeFSQ )
		{
		return;
		}
    
    TPtr8 buf8( aData, sizeof(TInt32), sizeof(TInt32) );
    RDesReadStream readStream;
    readStream.Open( buf8 );
    CleanupClosePushL( readStream );
    TInt dataSize = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy( &readStream );
    if ( dataSize > 0 )
        {
        TUint16* dataAddress = (TUint16*)( aData + sizeof(TInt32) );
        HBufC* tooltipText = ReadTextInfoHBufCL( dataAddress, 
                                                 ( dataSize + 1 )/ 2 );
        if ( tooltipText )
            {
            CleanupStack::PushL( tooltipText );
            iVkbWindow->ShowTooltipOnFSQL( *tooltipText );
            CleanupStack::PopAndDestroy( tooltipText );
            }        
        }     
    }

// ---------------------------------------------------------------------------
// Handle show candidate list command.
// ---------------------------------------------------------------------------
//
void CAknFepVkbLayout::HandleShowCandidateListOnFSQCmdL( TUint8* aData )
    {
    if ( !iITIEnabled || iLayoutType != EPluginInputModeFSQ )
        {
        return;
        }
    
    // Read candidate data from a block of memory staring from aData
    // The format is activeIndex | count of candiates | 
    // length 1 | text 1 | length 2 | text 2 |...
    TPtr8 buf8( aData, sizeof( TInt32 )*2, sizeof( TInt32 )*2 );
    RDesReadStream readStream;
    readStream.Open( buf8 );
    CleanupClosePushL( readStream );
    // Get activeIndex
    TInt activeIndex = readStream.ReadInt32L();
    // Get count of candidates
    TInt count = readStream.ReadInt32L();    
    CleanupStack::PopAndDestroy( &readStream );
    
    CDesCArray* itemArray = NULL;
    if ( count > 0 )
        {        
        TUint8* curPointer = aData + sizeof(TInt) * 2;
        itemArray = new (ELeave) CDesCArrayFlat( count );
        CleanupStack::PushL( itemArray );
        for ( TInt i = 0; i < count; i++ )
            {
            // Get length
            buf8.Set( curPointer, sizeof( TInt32 ), sizeof( TInt32 ) );
            readStream.Open( buf8 );
            CleanupClosePushL( readStream );
            TInt32 textSize = 0;
            textSize = readStream.ReadInt32L();            
            CleanupStack::PopAndDestroy( &readStream );
            if ( textSize > 0 )
                {
                // Get text
                curPointer += sizeof( TInt32 );
                HBufC* itemText = ReadTextInfoHBufCL
                                      ( (TUint16*)curPointer, 
                                        ( textSize + 1 )/ 2 );
                if ( itemText )
                    {
                    CleanupStack::PushL( itemText );
                    itemArray->AppendL( *itemText );                    
                    CleanupStack::PopAndDestroy( itemText ); 
                    }     
                curPointer += textSize;
                }
            }
        
        iVkbWindow->ShowCandidateListOnFSQL( itemArray, activeIndex );
        CleanupStack::PopAndDestroy( itemArray );
        }  
    }

// ---------------------------------------------------------------------------
// Handle show candidate list command.
// ---------------------------------------------------------------------------
//
HBufC* CAknFepVkbLayout::ReadTextInfoHBufCL( const TUint16* aStartPtr, 
                                                      TInt aLength )
    {
    HBufC* itemText = NULL;
    if ( aLength > 0 )
        {
        itemText = HBufC::NewLC( aLength );
        TPtr itemTextPtr = itemText->Des();
        itemTextPtr.Copy( aStartPtr, aLength ); 
        CleanupStack::Pop( itemText ); 
        }    
    return itemText;
    }
// End Of File
