/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           
*
*/












// System includes
#include <aknedsts.h> 
#include <AknUtils.h> 
#include <aknappui.h> 
#include <eikccpu.h> 
#include <eikdialg.h> 
#include <eikcapc.h> // CEikCaptionedControl
#include <aknQueryControl.h> 
#include <centralrepository.h> 
#include <StringLoader.h> 
#include <AknFepPluginManager.h>
#include <aknfeppeninputimeplugin.h>
#include <UikonInternalPSKeys.h> // KUikGlobalNotesAllowed
#include <AvkonInternalCRKeys.h>    // KAknQwertyInputModeActive
#include <hlplch.h>
#include <peninputgsinterface.h>
#include <aknfep.rsg>
#include <avkon.rsg>
#include <AknIndicatorContainer.h>  //CAknIndicatorContainer
#include <StringLoader.h>
#include <peninputgenericitutcustomization.h>
#include <eikgted.h> 
#include <txtglobl.h>
#include <eikedwin.h> 
#include <featmgr.h>                 //FeatureManager
#include <aknextendedinputcapabilities.h>
#include <avkon.hrh>
#include <PtiEngine.h>
#include <peninputcommonlayoutglobalenum.h>
#include <PtiCompositionDataIF.h> // MPtiEngineCompositionDataInterface
#include <eikapp.h>
#include <AknFepGlobalEnums.h>
#include <AknFepInternalCRKeys.h>
#include <AknFepInternalPSKeys.h>
#include <aknFepVietnameseInputTables.h>

// User includes
#include "AknFepManagerInterface.h" // MAknFepManagerInterface
#include "AknFepUiManagerBase.h"
#include "aknfepuimanagerfingeritutchinese.h"

// For Spell
#include "aknfepuispellcontainer.h"            
#include "aknfepuispellinputpane.h"

#include <eikgted.h> 

#include <apgwgnam.h>

#include <PtiKeyMappings.h>
#include <AknPriv.hrh>

#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS
#include <gfxtranseffect/gfxtranseffect.h>
#include <akntransitionutils.h>
#include <akntranseffect.h>

#include <e32property.h>
#include <avkondomainpskeys.h>
#endif

// Constants
const TInt KCursorBlinkPerioid = 10000;//300000; // five tenth of a second * 2
const TInt KMaxServerDataLength = 50;
const TUint KVkbParagraphSeparator = 0x2029;
const TUint KVkbTabulator = 0x0009;
const TUid KAknfepUID= { 0x100056de }; 
const TUid KUidtruiApp = { 0x2000B104 }; 
const TInt KAknScreenServerUid = 0x100056cf;
const TInt KAknCapServerUid = 0x10207218;
const TInt KAknNotifySrvUid = 0x10281EF2;
const TInt KAknHelpAppUid = 0x10005234;
const TInt KInvalidValue = -1;

// Paste event for AknCcpuSupport
const TKeyEvent KAknCcpuPasteEvent = {EKeyF18, EEikCmdEditPaste, EModifierCtrl, 1}; 

const TUint KSupportedDeadKeys[] = { 0x005E, 0x0060, 0x00A8, 0x007E, 0x00AF, 
		                             0x00B4, 0x00B8, 0x02C7, 0x02DB, 0x02DD, 
		                             0x00A8, 0x0384, 0x0385 }; 

_LIT(KVkbTabChar, " ");

_LIT(KHWR_HLP_MAIN_VIEW, "HWR_HLP_MAIN_VIEW");
_LIT(KVQWERTY_HLP_MAIN_VIEW, "VQWERTY_HLP_MAIN_VIEW"); 
_LIT(KVITU_HLP_MAIN_VIEW, "VITU_HLP_MAIN_VIEW");

const TInt KDefaultCandidateArraySize = 16;
const TInt KNumberOfCases = 6;

const TInt KChineseSpellMode = 100;
const TInt KWesternSpellMode = 101;
 _LIT_SECURE_ID(KPhoneSecureId, 0x100058b3);

#define iAvkonAppUi ((CAknAppUi*)CEikonEnv::Static()->EikAppUi())
TUid AppUidFromWndGroupIdL(TInt aWndGrpId)
    {
    RWsSession &ws = CCoeEnv::Static()->WsSession();
    //TInt wgId =ws.GetFocusWindowGroup();
    CApaWindowGroupName* wg = CApaWindowGroupName::NewLC(ws,aWndGrpId);

    TUid id = wg->AppUid();
    CleanupStack::PopAndDestroy(wg);    	
    return id;
    
    }

TUid GetFocusAppUid()
    { 
    TUid id = {0x0};
    TRAP_IGNORE(id = AppUidFromWndGroupIdL(CCoeEnv::Static()->WsSession().GetFocusWindowGroup()));
    return id;
    }
TUid GetCurAppUid()
    {
    TUid id = {0x0};	
    TRAP_IGNORE(id = AppUidFromWndGroupIdL(CCoeEnv::Static()->RootWin().Identifier())); 
    return id;           
    }

TBool IsGlobalNotesApp(TUid& aUid)		  
    {
    const TInt KAknCapServerUid = 0x10207218;
    const TInt KAknNotifySrvUid = 0x10281EF2;      
    if(aUid.iUid == KAknCapServerUid || aUid.iUid == KAknNotifySrvUid)
        return ETrue;
    return EFalse;
    }

// -----------------------------------------------------------------------------
// Check if the app specified by aUid is avkon notify server
// -----------------------------------------------------------------------------
//
TBool IsAknNotifyServerApp( const TUid& aUid )
    {
    const TInt KAknNotifySrvUid = 0x10281EF2; 
    return aUid.iUid == KAknNotifySrvUid;
    }

#ifdef RD_SCALABLE_UI_V2
inline TBool IsAbleTouseCallBubble()
    {
    const TInt KAknCapServerUid = 0x10207218;    
    return GetCurAppUid().iUid != KAknCapServerUid;
    }
#endif //RD_SCALABLE_UI_V2
    
RDrawableWindow* GetFocusWindow()
    {
    CCoeControl* focusControl = CCoeEnv::Static()->AppUi()->TopFocusedControl();
    if( focusControl )
        {
        return focusControl->DrawableWindow();
        }
    return NULL;
    }
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CAknFepPluginManager::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//    
CAknFepPluginManager* CAknFepPluginManager::NewL( CAknFepManager& aFepMan,
                                                CAknFepSharedDataInterface& aSharedData,
                                                CAknFepLanguageManager& aLangMan,
                                                CAknFepCaseManager& aCaseMan )
    { 
    CAknFepPluginManager* self = new (ELeave) CAknFepPluginManager
        ( aFepMan, aSharedData, aLangMan, aCaseMan ); 
    CleanupStack::PushL( self );
    self->ConstructL();       
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::~CAknFepPluginManager
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
CAknFepPluginManager::~CAknFepPluginManager()
    {  
    if ( iConnectAo )
    	{
    	iConnectAo->Cancel();
    	delete iConnectAo;
    	}
    
    delete iAdjustDataQueryTimer;

    iCurrentPluginInputFepUI = NULL; // why not delete????
    delete iPenInputMenu;
    
    if( iPenInputSvrConnected )
        {
        iPenInputServer.Close();
        iPenInputSvrConnected = EFalse;
        }
    delete iFingerItutChineseUI;
    
    delete iIcfDataBuf;
    iIcfDataBuf = NULL;
    
    delete iSpellText;
    iSpellText = NULL;

    delete iSpell;
    iSpell = NULL;
    
    delete iLastEditorContentBuf;
    
    delete iAvkonRepository;
    
    /*    
    for(TInt i = 0; i < iCandidateList.Count(); i++)
        {
        HBufC* buf = iCandidateList[i];
        delete buf;
        }
    iCandidateList.Close();
    */
    iCandidateList.ResetAndDestroy();
    iSendAllList.iCandidates.Reset();
    iOpenWindowList.Close();
    iEventData.Close();    
    iPreCaption.Close();
    
    ClosePeninputSetting();
    }
    
// ---------------------------------------------------------------------------
// CAknFepPluginManager::CAknFepPluginManager
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CAknFepPluginManager::CAknFepPluginManager( CAknFepManager& aFepMan, 
                                          CAknFepSharedDataInterface& aSharedData,
                                          CAknFepLanguageManager& aLangMan,
                                          CAknFepCaseManager& aCaseMan )
    : iFepMan( aFepMan ), iLangMan( aLangMan ), iCaseMan( aCaseMan ),
      iSharedData( aSharedData ), iPluginPrimaryRange( ERangeInvalid ), iCandidateIndex(1),
      iCharStartPostion( KInvalidValue )	  
    {
    iIndicatorImgID = 0;
    iIndicatorTextID = 0;
    iLastDataQueryWin = NULL;
    iDefaultOrientation = -1;
    iIsInEditWordQueryDlg = EFalse;    
    iClosePluginInputMode = EFalse;
    iDimGainForeground = ETrue;
    iAlignment = 0;
	iNeedFetchDimState = ETrue;
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::ConstructL()
    {
    iPenInputMenu = CAknFepPluginMenuManager::NewL(&iFepMan);
    
    //here ERangeInvalid means there is no change of plugin input range.
    iFepMan.UiInterface()->TouchPaneSetFepPenSupportInterface(this);
    iEventData.CreateMaxL(KMaxServerDataLength);
    
    RThread client;
    iHasSWEventCap = client.HasCapability(ECapabilitySwEvent);
    client.Close();
    iAvkonRepository = CRepository::NewL( KCRUidAvkon ); 
    
    iConnectAo = new (ELeave)CConnectAo(this);
    
    // iPortraitFSQEnabled indicates whether portrait FSQ feature is enabled.
    iPortraitFSQEnabled = FeatureManager::FeatureSupported( 
        KFeatureIdFfVirtualFullscrPortraitQwertyInput );
    }

// -----------------------------------------------------------------------------
// CAknFepPluginManager::ActivatePenInputL
// Change to default plugin input mode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::ActivatePenInputL()
    {
    if( !iPenInputSvrConnected || !iPenInputServer.IsVisible() || iPenInputServer.IsDimmed() )
        {
        if( iFepMan.FepAwareTextEditor() )
            {            
  			iPreferredUiMode = ETrue;	
            TryChangePluginInputModeByModeL((TPluginInputMode)(iSharedData.PluginInputMode()),
                                            EPenInputOpenManually,
                                            ERangeInvalid);
            }
        }    
    iFepMan.PtiEngine()->CancelTimerActivity();    
    }

	
// ---------------------------------------------------------------------------
// CAknFepPluginManager::DeactivatePenInputL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::DeactivatePenInputL()
    {
    ClosePluginInputModeL(ETrue);
    }

// -----------------------------------------------------------------------------
// CAknFepPluginManager::HandleServerEventL
// Handle pen input server events.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepPluginManager::HandleServerEventL(TInt aEventId)
    {
    if(ESignalServerExit == aEventId)
        {
        iPenInputSvrConnected = EFalse; 
        return ETrue;
        }
    if(ESignalServerReady == aEventId)
        {
        //reconnect to server
        iFepMan.SendEventsToPluginManL(EPluginEditorActivate);
        return ETrue;
        }
    TBool bHandled = EFalse;
    if( !iLangMan.CurrentImePlugin())
        {
        return bHandled;
        }
    
    iEventData.Zero();
    TPtrC pBuf; 
    TInt dataLen = iPenInputServer.GetServerEventData(iEventData);
    
    if( KErrNone == dataLen )
        {
        pBuf.Set(iEventData);
        }
    else if ( dataLen > 0 )
        {
        iEventData.ReAllocL(dataLen);
        dataLen = iPenInputServer.GetServerEventData(iEventData);
        pBuf.Set(iEventData);
        }
    if( dataLen != KErrNone )
	    {
	    return ETrue;
	    }

    if( !iLangMan.CurrentImePlugin()->HandleServerEventL(aEventId, iEventData) )
        {
        bHandled = ETrue;
        
        //only handle events which IME plug-in don't handled
        switch( aEventId )
            {
            case ESignalReplacePartText:
            case ESignalReplaceText://Get key event
            case ESignalReplaceTextJp:
                {
                TInt repeatCount = 0;
                TInt len = *(TInt*)pBuf.Ptr();
                TInt stringLength = pBuf.Length();
                TInt midLength = sizeof(TInt)/sizeof(TText);
                
                if (midLength > stringLength)
                    {
                    break;   
                    }

                iFepMan.SetFlag( CAknFepManager::EFlagSupressAutoUpdate );

                TPtrC s = pBuf.Mid(midLength);//sizeof(TInt)/sizeof(TText)
                                
                //set selection
                MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
                if( iFepMan.IsFepAwareTextEditor() && edit )
                    {
                    TCursorSelection curSor;
                    edit->GetCursorSelectionForFep(curSor);
                    if ( curSor.iAnchorPos == curSor.iCursorPos )
                        {
                        if (ESignalReplaceTextJp == aEventId)
                            {
                            if (curSor.iCursorPos  >= len)
                                {
                                curSor.iAnchorPos = curSor.iCursorPos - len;
                                if (curSor.iAnchorPos<0 || 
                                    curSor.iAnchorPos>edit->DocumentLengthForFep())
                                    {
                                    break;
                                    }
                                }
                            }
                        else
                            {
                            if(ESignalReplacePartText == aEventId)
                                {
                                curSor.iAnchorPos = curSor.iCursorPos - len;
                                }
                            else
                                {
                                if ( iCharStartPostion == KInvalidValue )    
                                    {
                                    curSor.iAnchorPos = curSor.iCursorPos -  
                                        ( iLastSubmitCount<len?iLastSubmitCount:len );
                                    }
                                else
                                    {
                                    curSor.iAnchorPos = iCharStartPostion;  
                                    iLastSubmitCount = curSor.iCursorPos - iCharStartPostion;
                                    }
                                }
                            
                            if (curSor.iAnchorPos<0 || 
                                curSor.iAnchorPos>edit->DocumentLengthForFep())
                                {
                                break;
                                }
                            
                            // check repeated string 
                            if (len >= iLastSubmitCount)
                                {
                                TBuf<CAknFepManager::EMaximumFepWordLength> textToUncommit;
                                textToUncommit.Zero();    
                                
                                 edit->GetEditorContentForFep(textToUncommit, curSor.iAnchorPos, 
                                                           Min(curSor.Length(), 
                                                           CAknFepManager::EMaximumFepWordLength));
                                
                                repeatCount = RepeatStringFromLeft(s,textToUncommit);    
                                curSor.iAnchorPos += repeatCount;
                                if (curSor.iAnchorPos<0 || 
                                    curSor.iAnchorPos>edit->DocumentLengthForFep())
                                    {
                                    break;
                                    }
                                }
                                
                            if (curSor.iCursorPos > curSor.iAnchorPos)
                                {
                                edit->SetCursorSelectionForFepL(curSor);    
                                }
                            }
                        }
                    }
                else
                    {
                    TBuf<1> backSpace;
                    backSpace.Append(TChar(EKeyBackspace));
                    for(TInt i = 0; i < len; ++i)
                        {
                        SubmitUiPluginTextL(backSpace, EKeyEvent);
                        }
                    }
                iFepMan.ClearFlag( CAknFepManager::EFlagSupressAutoUpdate );
                
                if (repeatCount > 0)
                    {
                    OnPenInputServerKeyEventL(s.Right(s.Length() - repeatCount));
                    
                    iLastSubmitCount += repeatCount;   
                    }
                else
                    {
                    OnPenInputServerKeyEventL(s);    
                    }
                }
                break;
            case ESignalKeyEvent:
                {
                TPtr ptr( const_cast<TUint16*>(pBuf.Ptr()), pBuf.Length(), pBuf.Length() );
                
                // If input mode is latin and Vkb or FSQ (landscape or portrait), 
                // data case is adjusted.
                if( iFepMan.InputMode() == ELatin && 
                    ( iPluginInputMode == EPluginInputModeVkb || 
                      iPluginInputMode == EPluginInputModeFSQ || 
                      iPluginInputMode == EPluginInputModePortraitFSQ ) )
                    {
                    AdjustDataCase( ptr );
                    }
                OnPenInputServerKeyEventL(pBuf);
              	}
              	break;
            case ESignalNotifyUpdateCba://Update CBA
                {
                TInt* cbaData = (TInt* )(pBuf.Ptr());
                iFepMan.UpdateCbaL(*cbaData);
                }
                break;                    
            case ESignalHwNotifySctPopup:
                iFepMan.LaunchSpecialCharacterTableL( 0,ETrue );//possible?
                break;  
            case ESignalChangeAmPm:
                {
                iFepMan.ChangeMfneAmPm();
                }
                break;
            case ESignalSetAppEditorCursor:
                {
                TInt8 *ptrData = (TInt8* )(pBuf.Ptr());
                TCursorSelection curSel = *(TCursorSelection* )(ptrData);
                TBool sync = *(TBool *)(ptrData + sizeof(TCursorSelection));
                
                if ( iFepMan.UncommittedText().Length() > 0 )
                    {
                    iFepMan.PtiEngine()->CancelTimerActivity(); 
                    //using TryCloseUiL() to sync Fep state
                    //iFepMan.CommitInlineEditL();
                    iFepMan.TryCloseUiL();
                    }
                //now change the cursor position
                SetCursorSelectionL(curSel, sync);
                }
                break;
            case ESignalLaunchLanguageMenu:
                LaunchPenInputLanguageSelectionL(ETrue);
                break;                                       
            case ESignalLaunchOptionMenu:
            case ESignalLaunchOptionsMenu:
                {
                if (iInMenu || (!iPenInputMenu) || iPenInputMenu->IsShowing())
                    {
                    break;    
                    }
                iLaunchMenu = ETrue;
                if ( PluginInputMode() == EPluginInputModeItut ||
                     PluginInputMode() == EPluginInputModeFSQ ||
                     PluginInputMode() == EPluginInputModeFingerHwr ||
                     PluginInputMode() == EPluginInputModePortraitFSQ )
                    {
                    SetMenuState();
                    }                
                else
                    {
                    ClosePluginInputUiL(ETrue);
                    }           
                LaunchPenInputMenuL(R_AVKON_PENINPUT_OPTION_MENU_BAR);
                }
                break;
            case ESignalLayoutClosed:
                if(iPluginInputMode == EPluginInputModeItut)
                	{
					iFepMan.PtiEngine()->CancelTimerActivity();
                	}
                ClosePluginInputModeL(ETrue);
                if(iPenInputSvrConnected ) //lost foreground
                    {
                    iPenInputServer.LoseForeground();
                    }                
                break;                    
            case ESignalCaseMode:
                {
                TInt* pluginCase = (TInt* )(pBuf.Ptr());    
                iFepMan.ClearFlag(CAknFepManager::EFlagSupressAutoUpdate);
                
                if (ECaseInverseText == *pluginCase)
                    {
                    iFepMan.SetFlag(CAknFepManager::EFlagQwertyShiftMode);    
                    }
                iCaseMan.SetCurrentCase(*pluginCase);
                
                if (ECaseInverseText == *pluginCase)
                    {
                    iFepMan.ClearFlag(CAknFepManager::EFlagQwertyShiftMode);    
                    }
                }
                break;
            case ESignalRange:
                {
                HideTooltipOnFSQL();

                TInt newPrimaryRange = *(TInt* )(pBuf.Ptr());
                // Addition for ITI features on FSQ, 
                // when swith to other ranges from latin/native range, 
                // need to commit text to fep and ptiengint if necessary
                if ( EnableITIOnFSQ() && newPrimaryRange != ERangeEnglish 
                     && newPrimaryRange != ERangeNative )
                    {
                    iFepMan.CommitInlineEditL();
                    iFepMan.PtiEngine()->CommitCurrentWord();
                    }                
                iPluginPrimaryRange = newPrimaryRange;
                if (ELangJapanese == iSharedData.InputTextLanguage()
                    && ERangeNative == iPluginPrimaryRange )
                    {
                    TInt subrange = *(TInt* )( (TInt8* )(pBuf.Ptr()) + sizeof(TInt) );
                    iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputJapaneseSetSubRange, 
                                                             subrange);
                    }
                SyncIndicatorWithPluginRangeL();
                // If input mode is FSQ, update indicator accordingly.
                if ( PluginInputMode() == EPluginInputModeFSQ || 
                     PluginInputMode() == EPluginInputModePortraitFSQ )
                    {
					UpdateFSQIndicator();
                    }
                }
                break;
            case ESignalLayoutUIChanged:
                {
                TInt8 *ptrData = (TInt8* )(pBuf.Ptr());
                TBool switchByMode = *(TInt*)(ptrData);
                if( switchByMode )
                    {
                    TInt mode = *(TInt* )( ptrData + sizeof(switchByMode) );
					ClosePluginInputModeL(ETrue);               
                    iFepMan.TryCloseUiL();  
					iPreferredUiMode = EFalse;
                    TryChangePluginInputModeByModeL((TPluginInputMode)mode, 
                                                    EPenInputOpenManually,
                                                    ERangeInvalid);
                    }
                }
                break;
            case ESignalCommitInlineText:
                {
                iFepMan.CommitInlineEditL();
                }
                break;
            case ESignalEnterMatchSelection:
                {
                TBool on = *((TInt*)(pBuf.Ptr()));

                if (on)
                    {
                    EnterMatchSelectionState();
                    }
                else
                    {
                    iFepMan.PluginUIManager()->SetFingerMatchSelectionState(EFalse);
                    }    
                }
                break;
            case ESignalShowCandidate:
                {
                // Get candidate list
                TInt activeIdx = 0;
                CDesCArray* candidates = new (ELeave) CDesCArrayFlat
                                                 ( KDefaultCandidateArraySize );
                CleanupStack::PushL( candidates );
                iFepMan.GetCandidatesL(*candidates, activeIdx);

                TFepITICandidateList candidateListData;
                candidateListData.iItemArray = candidates;
                candidateListData.iActiveIndex 
                            = ( candidates->Count() >= 2 ) ? 1 : activeIdx;  
                candidateListData.iLangCode = iFepMan.CurrentInputLangCode();
                if ( iLangMan.IsSplitView() )
                    {
                    candidateListData.iRect = iFepMan.InitRectForSplitCandL();
                    }
                
                ShowCandidateListL( candidateListData );                
                CleanupStack::PopAndDestroy( candidates );                
                }
                break;
                
            case ESignalSelectMatchSelectionText:
                {
                TInt* ptrData = (TInt*)(pBuf.Ptr());
                TInt itemidx = *(ptrData);
                TBool commit = *(ptrData + 1);
                
                iFepMan.UpdateInlineEditL(iSendAllList.iCandidates[itemidx], 
                                          iSendAllList.iCandidates[itemidx].Length());
                
                if (commit)
                    {
                    iFepMan.TryCloseUiL();
                    iFepMan.FepUI()->HandleCommandL(ESignalSelectMatchSelectionText);
                    }
                else
                    {
                    iFepMan.PluginUIManager()->SetFingerMatchSelectionState(EFalse);
                    }    
                
                SendIcfDataL(EPluginSyncUpdatedText);
                }
                break;
            case ESignalEnterSpellMode:
                {
                iCurrentPluginInputFepUI->HandleCommandL(ECmdPeninputDisableLayoutDrawing,ETrue);
                // this will allow the layout to be redraw even if the layout was activeated
                iCurrentPluginInputFepUI->HandleCommandL(ECmdPeninputEnalbeLayoutReDrawWhenActive,ETrue);
                iFepMan.LaunchEditWordQueryL();
                }
                break;
            case ESignalExitSpellMode:
                {
                TInt* ptrData = (TInt*)(pBuf.Ptr());
                TBool exitbyok = *(ptrData);

				//remove spell mode editor will cause focus change, so as pen UI is closed and opened again.
				iFocuschangedForSpellEditor = ETrue;
				iCurrentPluginInputFepUI->HandleCommandL(ECmdPeninputDisableLayoutDrawing,ETrue);
				
                // this will allow the layout to be redraw even if the layout was activeated
                iCurrentPluginInputFepUI->HandleCommandL(ECmdPeninputEnalbeLayoutReDrawWhenActive,ETrue);
                exitbyok ? iFepMan.ExitPluginSpellModeByOk() : 
                           iFepMan.ExitPluginSpellModeByCancel();
                }
                break;
            case ESignalLaunchSCT:
            	{
            	iFepMan.LaunchSpecialCharacterTableL(0,ETrue);	
            	}
            	break;
            case ESignalGetEditMenuData:
                {
                //Calcuate which menu needed to add
                iFepMan.GetPermittedEditorMenu(EFalse);
                }
                break;
            case ESignalEditorMenuCommand:
                {
                //Execute editor menu command
                TInt* command = (TInt* )(pBuf.Ptr());
                iFepMan.ProcessEditorMenuCommand(*command);
                iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerInputMode,
                                                         iFepMan.InputMode());
                    
                iIndicatorImgID = 0;
                iIndicatorTextID = 0;  
                UpdateITUTIndicator();                
                }
                break;                
            case ESignalCharacterStart:
                {
                TBool charStart = *(TBool*)(pBuf.Ptr());
                if( charStart )
                    {
                    MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor(); 
                    if( !edit )
                        {
                        break;
                        }
                    TCursorSelection curSor;
                    edit->GetCursorSelectionForFep(curSor);
                    iCharStartPostion = Min( curSor.iCursorPos, curSor.iAnchorPos );          

                    }
                else
                    {
                    iCharStartPostion = KInvalidValue;
                    }
                }
                break;
            case ESignalSelectCandidate:
                {
                // Select candidate word on fsq
                TInt index = *(TInt*)( pBuf.Ptr() );
                CommitCandidateWordOnFSQL( index );                
                break;
                }
            case ESignalAcceptTooltip:
                {
                // Simulate Up key and then join in the key flow for HW
                // to accept tooltip
                TKeyEvent keyEvent = { EKeyUpArrow, EStdKeyUpArrow, 0, 0 };                  
                CCoeEnv::Static()->SimulateKeyEventL( keyEvent, EEventKey );
                
                // commit tooltip text then commit inline                
                iFepMan.CommitInlineEditL();
                // Need to commit into engine, it will clear textbuffer in it.
                iFepMan.PtiEngine()->CommitCurrentWord();
                }
                break;
            case ESignalCommitITIInlineText:
                {
                // Commit autocompletion
                iFepMan.CommitInlineEditL();
                // Need to commit into engine, it will clear textbuffer in it.
                iFepMan.PtiEngine()->CommitCurrentWord();
                }
                break;
            case ESignalDeleteLastInput:
                {

                iFepMan.SetFlag( CAknFepManager::EFlagSupressAutoUpdate );

                //set selection
                MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
                if( iFepMan.IsFepAwareTextEditor() && edit )
                    {
                    TCursorSelection curSor;
                    edit->GetCursorSelectionForFep(curSor);
                    if ( curSor.iAnchorPos == curSor.iCursorPos )
                        {
                        curSor.iAnchorPos = curSor.iCursorPos - iLastSubmitCount;
                        
                        if (curSor.iAnchorPos<0 || 
                            curSor.iAnchorPos>edit->DocumentLengthForFep())
                            {
                            break;
                            }
                        
                        if (curSor.iCursorPos > curSor.iAnchorPos)
                            {
                            edit->SetCursorSelectionForFepL(curSor);    
                            }
                        }
                    TBuf<1> backSpace;
                    backSpace.Append(TChar(EKeyBackspace));
                    SubmitUiPluginTextL(backSpace, EKeyEvent);
                    }
                iFepMan.ClearFlag( CAknFepManager::EFlagSupressAutoUpdate );
                
                }
                break;
            case ESignalArabicSCTChar:
                {
                TPtrC ptr( const_cast<TUint16*>(pBuf.Ptr()), pBuf.Length());
//                if(iFepMan.InputMode() == ELatin && (iPluginInputMode == EPluginInputModeVkb || 
//                   iPluginInputMode == EPluginInputModeFSQ))
//                    {
//                    AdjustDataCase( ptr );
//                    }
                iFepMan.StartInlineEditL(ptr);
                iFepMan.CommitInlineEditL();
                SendIcfDataL( EPluginSyncAll );                
//                OnPenInputServerKeyEventL(pBuf);                
                }
                break;
            case ESignalDeactivateSprite:
            	{
                ClosePluginInputUiL( ETrue );
                if(iPenInputSvrConnected ) //lost foreground
                    {
                    iPenInputServer.LoseForeground();
                    }
            	}
            	break;
            default:
                break;
            }
        }  
    return bHandled;
    }
    
// -----------------------------------------------------------------------------
// CAknFepPluginManager::EnterMatchSelectionState
// Enter match selection state
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepPluginManager::EnterMatchSelectionState()
    {
    GetAllPredictiveCandidate();
    
    if (iCandidateList.Count() > 0)
        {
        SetFingerMatchSelectionState(ETrue);
        iFepMan.PluginUIManager()->ShowAllCandidates();
        return ETrue;
        }
    
    return EFalse;    
    }

// -----------------------------------------------------------------------------
// CAknFepPluginManager::ClearDestroyedEditorPointer
// Set the editor pointer to NULL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::ClearDestroyedEditorPointer()
	{
	iEdwin = NULL;
	}

// -----------------------------------------------------------------------------
// CAknFepPluginManager::EdwinState
// Get the editor state
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknEdwinState* CAknFepPluginManager::EdwinState()
	{
	CAknEdwinState* edwinState( NULL );
	if ( iEdwin && iEdwin->Extension1())
		{
	    // Get the editor state
		edwinState = static_cast<CAknEdwinState*>( 
				iEdwin->Extension1()->State( KNullUid ));
		}
	
	return edwinState;
	}
	
// -----------------------------------------------------------------------------
// CAknFepPluginManager::HandleEventsFromFepL
// Handle events from FEP
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::HandleEventsFromFepL( TInt aEventType, TInt aEventData )
    {

    if( iPluginInputMode == EPluginInputModeNone && 
        aEventType != EPluginEditorActivate && 
        aEventType != EPluginActivate &&
        aEventType != EPluginResourceChanged &&
        aEventType != EPluginSwitchMode &&
        aEventType != EPluginSwitchToPortrait &&
        aEventType != EPluginSwitchToLandscape)
        {      
        return;
        }
        
    switch ( aEventType )
        {
        case EPluginEditorActivate:
            {
            ActivatePenInputL();
            RWsSession &ws = CCoeEnv::Static()->WsSession();
            TInt wgId =ws.GetFocusWindowGroup();
            iEditorPriority = ws.GetWindowGroupOrdinalPriority(wgId);
            }
            break;
        case EPluginActivate:
            {//The command is obsolete, use other commands
            //current only some Japanese menu commands call this
            ActivatePenInputL();    
            }            
            break;
        case EPluginCloseMode:
            {
            if (iPluginInputMode == EPluginInputModeItut && 
                !iSharedData.PredictiveTextAutoCompleteOn())
            	{
            	return;
            	}
            
            ClosePluginInputModeL( aEventData );
            }
            break;
        case EPluginFocusItemDestroy:
            if( iFepMan.FepAwareTextEditor() )
                {
                iMfne = NULL;
                iEdwin = NULL;
                if ( !aEventData )
                    {
                    RemoveLastFocusedWinFromOpenList();
                    }
                else
                    {
                    ClosePluginInputUiL(ETrue);
                    if(iPenInputSvrConnected ) //lost foreground
                        {
                        iPenInputServer.LoseForeground();
                        }
                    }
                }
            break;
        case EPluginSyncFepAwareText:
            {
            SyncFepAwareText( TPluginSync(aEventData) );
            }
            break;
        /*case EPluginActivateInGlobalNote:
            {
            OnFocusChangedL(ETrue);
            iPenInputServer.ActivatePeninputInNotesL();           
            }
            break;*/
        case EPluginMenuCmd:
            {
            ProcessMenuCommandL( aEventData );
            }
            break;
        case EPluginForegroundChanged:
            {
            iForegroundChange = ETrue;   
            iLaunchHelp = EFalse;    
            
            if (!aEventData && IsInGlobleNoteEditor())
                {
                ClosePluginInputUiL(ETrue);
                if(iPenInputSvrConnected ) //lost foreground
                    {
                    iPenInputServer.LoseForeground();
                    }
                }
            else
                {
                OnFocusChangedL( aEventData );   
                }
            iForegroundChange = EFalse;
            }
            break;
        case EPluginFocusChanged:
            {
            OnFocusChangedL( aEventData );
            }
            break;
        case EPluginResourceChanged:
            {
            iOrientationChangedfromUI = ETrue;
            
            TUid uid = GetCurAppUid();
            if ( IsAknNotifyServerApp( uid ) )
                {
                //turn off AutoForeground feature of global query temporarily during layout switching
                //if not, global query may be shown on top of virtual input by unexpected tap events.
                CCoeEnv::Static()->RootWin().AutoForeground( EFalse );
                TRAPD( err, OnResourceChangedL( aEventData ) );
                CCoeEnv::Static()->RootWin().AutoForeground( ETrue );
                User::LeaveIfError( err );
                }
            else
                {
                OnResourceChangedL( aEventData );
                }
            
            iOrientationChangedfromUI = EFalse;
            }
            break;
        case EPluginFaseSwap:
            {
            iFastSwapByMenu = aEventData;
            }
            break;
        case EPluginKeyEvent:
            {
            HandleKeyEventL( aEventData );
            }
            break;
        case EPluginUpdateIndicator:
            {
            UpdateITUTIndicator();
            // Update indicator for FSQ (landscape or portrait).
            if ( PluginInputMode() == EPluginInputModeFSQ ||
                 PluginInputMode() == EPluginInputModePortraitFSQ )
                {
				UpdateFSQIndicator();
                }
            }
            break;
        case EPluginPreview:
            {
            iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCharacterPreview, 
               aEventData);                 
            }
            break;
        case EPluginPromptChanged:
            {
            SetPromptText( EFalse );
            }
            break;
        case ELostForeground:
            {
            if(iPenInputSvrConnected ) //lost foreground
                {
                iPenInputServer.LoseForeground();
                }
            }
            break;
        case EPluginShowTooltip:
        	{
        	ShowTooltipOnFSQL( aEventData );
        	}
        	break;
        case EPluginHideTooltip:
        	{
        	HideTooltipOnFSQL();
        	}
        	break;
        case EPluginSwitchToPortrait:
            {
            iSharedData.SetDefaultArabicFingerHwrOrientation(0);
            TryChangePluginInputModeByModeL(EPluginInputModeFingerHwr,
                                            EPenInputOpenManually,
                                            ERangeInvalid);
            }
            break;
        case EPluginSwitchToLandscape:
            {
            iSharedData.SetDefaultArabicFingerHwrOrientation(1);            
            TryChangePluginInputModeByModeL(EPluginInputModeFingerHwr,
                                            EPenInputOpenManually,
                                            ERangeInvalid);
            }
            break;
        case EPluginSwitchMode:
            {
            iPreferredUiMode = EFalse;
            TPluginInputMode defaultMode = EPluginInputModeNone;
            TPixelsTwipsAndRotation size; 
            CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
        
            TBool landscape = size.iPixelSize.iWidth > size.iPixelSize.iHeight;
            if(iSharedData.AutoRotateEnabled())
                {
                if(landscape)
                    {
                    defaultMode = EPluginInputModeFSQ;
                    }
                else
                    {
                    // Use the last used portrait input mode.
                    defaultMode = iSharedData.PluginPortraitInputMode();
                    }
                }
            else
                {
                if(landscape)
                    {
                    defaultMode = EPluginInputModeFSQ;
                    }
                else
                    {
                    // Use the last used portrait input mode.
                    defaultMode = iSharedData.PluginPortraitInputMode();
                    }
                }
            TryChangePluginInputModeByModeL(defaultMode,
                                            EPenInputOpenManually,
                                            ERangeInvalid);
            }
            break;
        case EPluginUpdatePenInputITIStatus:
        	{
        	if ( iCurrentPluginInputFepUI )
        		{
                iCurrentPluginInputFepUI->HandleCommandL( ECmdPeninputITIStatus,
                                              iFepMan.WesternPredictive() );        	
        		}
        	}
        	break;
        case EPluginEnableFetchDimState:
        	{
        	iNeedFetchDimState = ETrue;
        	}
        	break;
        case EPluginArabicNumModeChanged:
            if(iCurrentPluginInputFepUI)
                {
                iCurrentPluginInputFepUI->HandleCommandL(
                        ECmdPeninputArabicNumModeChanged,aEventData);                            
                }
            break;
		case EPluginEnablePriorityChangeOnOriChange:
            if( iCurrentPluginInputFepUI && PluginInputMode() != EPluginInputModeNone )
                {
				// Don't handle this command if pen ui is not opened.
                if( iPenInputSvrConnected )
                    {
                    iPenInputServer.EnablePriorityChangeOnOriChange(TBool(aEventData));  
                    }
                }

        default:
            break;
        }
    }
            
// -----------------------------------------------------------------------------
// CAknFepPluginManager::TryChangePluginInputModeByModeL
// Change current input method to plugin input method with specified layout UI id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepPluginManager::TryChangePluginInputModeByModeL
    (TPluginInputMode aSuggestMode, TInt aOpenMode,TInt aSuggestRange)
    {
    iNeedFetchDimState = ETrue;
    if ( iSharedData.QwertyInputMode() )
        {
        return EFalse;
        }

    TBool rtn = EFalse;
    if ( iFepMan.ExtendedInputCapabilities() & 
        CAknExtendedInputCapabilities::EInputEditorDisableVKB ) 
        {
        return EFalse;
        }

    if (! ConnectServer() )
        {
        return EFalse;
        }
    
    
    if ((iPenInputServer.PreferredUiMode() != EPluginInputModeNone) && iPreferredUiMode )
        {
        aSuggestMode = iPenInputServer.PreferredUiMode();
        }
        
    if (!GetCurSuggestMode( aSuggestMode ))
        {
        return EFalse;    
        }
    
    // remember the old fep aware editor.
    TBool cleanContent = EFalse;   
    if(iOldFepAwareEditor != iFepMan.FepAwareTextEditor())
        {
        cleanContent = ETrue;
        iOldFepAwareEditor = iFepMan.FepAwareTextEditor();
        }

    if( !iAdjustDataQueryTimer )
        {
        iAdjustDataQueryTimer = CPeriodic::NewL(CActive::EPriorityStandard);
        }
            
    // Addition for ITI features on FSQ, if iITISettingDialogOpen == ETrue,
    // it means that predictive setting dialog is closed, then re-open FSQ again.
    // Need to set iITISettingDialogOpen = EFalse at this moment
    if ( iITISettingDialogOpen )
    	{
    	iITISettingDialogOpen = EFalse;
    	}

    // Set the KAknFepTouchInputActive PS to 1, 
    // it means that touch input is active now.
    RProperty::Set( KPSUidAknFep, KAknFepTouchInputActive, 1 );
    
    // getting a new ui manager object corresponded in aMode.
    TInt uiLanguage = iSharedData.DisplayLanguage();
    if( uiLanguage == ELangTest )
        {
        uiLanguage = User::Language();
        }
    
    if ( iDefaultOrientation == -1 )
        {
        iDefaultOrientation = iAvkonAppUi->Orientation();
        }
    iPenInputServer.BackgroudDefaultOri( iDefaultOrientation );
    
    TPixelsTwipsAndRotation size; 
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
    TBool landscape = size.iPixelSize.iWidth > size.iPixelSize.iHeight;     
    // For portrait only mode, need to disable FSQ.
    TBool disableFSQ = iDefaultOrientation == CAknAppUiBase::EAppUiOrientationPortrait
                       || ( !iSharedData.AutoRotateEnabled() 
                            && !landscape 
                            && !iAvkonAppUi->OrientationCanBeChanged() );                 
        
    if ( disableFSQ )
        {
        iPenInputServer.SetDisabledLayout( EPluginInputModeFSQ );
        }
    else
        {
        // if fsq had been disabled before and now application is not portrait only,
        // need to enable fsq again;
        TInt disableMode = iPenInputServer.DisabledLayout();
        if( disableMode & EPluginInputModeFSQ )
            {
            iPenInputServer.SetDisabledLayout( -1 );//reset disable type
            iPenInputServer.SetDisabledLayout( disableMode & ~EPluginInputModeFSQ );
            }
        }
    
    // Both ITU-T and portrait FSQ are handled here.  Because getting and setting
    // disabled layout separately caused issues.
    TBool disablePortraitInputMode = 
               iDefaultOrientation == CAknAppUiBase::EAppUiOrientationLandscape 
               || ( !iSharedData.AutoRotateEnabled() 
                    && landscape 
                    && !iAvkonAppUi->OrientationCanBeChanged() );

 
    if ( disablePortraitInputMode )
        {
        // Disable V-ITUT and portrait FSQ, and if current aSuggestMode is 
        // EPluginInputModeItut or EPluginInputModePortraitFSQ, replace it with 
        // EPluginInputModeFSQ.
        iPenInputServer.SetDisabledLayout( 
            EPluginInputModeItut | EPluginInputModePortraitFSQ );
        if( aSuggestMode == EPluginInputModeItut || 
            aSuggestMode == EPluginInputModePortraitFSQ )
            {
            aSuggestMode = EPluginInputModeFSQ;
            }
        }
    else
        {
        // if V-ITUT had been disabled before, enable it now;
        TInt disableMode = iPenInputServer.DisabledLayout();
        if ( ( disableMode & EPluginInputModeItut ) || 
             ( disableMode & EPluginInputModePortraitFSQ ) )
            {
            iPenInputServer.SetDisabledLayout( -1 ); // Reset
            TInt enableMode( 0 );
            if ( disableMode & EPluginInputModeItut )
                {
                enableMode |= EPluginInputModeItut;
                }
            if ( disableMode & EPluginInputModePortraitFSQ )
                {
                enableMode |= EPluginInputModePortraitFSQ;
                }
            iPenInputServer.SetDisabledLayout( disableMode & ~enableMode );
            }
        }


    if ( aSuggestMode == EPluginInputModeFSQ)
        {
        TPluginInputMode tempInputMode = iPluginInputMode;           
        iPluginInputMode = aSuggestMode;
        
        if (disableFSQ && (aSuggestMode != EPluginInputModeFingerHwr))
            {
            iPluginInputMode = tempInputMode;
            aSuggestMode = iSharedData.PluginPortraitInputMode();
            }
        else
            {
            if ( (!iSharedData.AutoRotateEnabled() 
                    && aSuggestMode == EPluginInputModeFSQ))
                {
                iFepMan.SetNotifyPlugin( EFalse );
                iAvkonAppUi->SetOrientationL( CAknAppUiBase::EAppUiOrientationLandscape );
                iFepMan.SetNotifyPlugin( ETrue );
                iOrientationChanged = ETrue;
                }
        
		    /*CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
              landscape = size.iPixelSize.iWidth > size.iPixelSize.iHeight;  
              if( !landscape )
                  {
                  iPluginInputMode = tempInputMode;
                  aSuggestMode = EPluginInputModeItut;
                  }*/
            //
            
            }
        }
    else if(aSuggestMode == EPluginInputModeFingerHwr 
            && iSharedData.InputTextLanguage() == ELangArabic
            && !iSharedData.AutoRotateEnabled())
        {
        TInt hOrient = iSharedData.DefaultArabicFingerHwrOrientation();
        if(landscape)
            {
            if(!hOrient)
                {
                iAvkonAppUi->SetOrientationL( CAknAppUiBase::EAppUiOrientationPortrait );
                iOrientationChanged = ETrue;                
                }
            }
        else
            {
            if(hOrient)
                {
                iAvkonAppUi->SetOrientationL( CAknAppUiBase::EAppUiOrientationLandscape );
                iOrientationChanged = ETrue;                
                }
            }
        }
    else if ( iOrientationChanged )
        {
        iFepMan.SetNotifyPlugin( EFalse );
        iAvkonAppUi->SetOrientationL( (CAknAppUiBase::TAppUiOrientation)iDefaultOrientation );
        iFepMan.SetNotifyPlugin( ETrue );
        iOrientationChanged = EFalse;
        }
    
    TBool isSplit = IsEditorSupportSplitIme();
    TInt inputLang = iFepMan.InputLanguageCapabilities().iInputLanguageCode;
    // temp code for Chinese split view
    if ( inputLang == ELangPrcChinese || 
    	 inputLang == ELangTaiwanChinese ||
    	 inputLang == ELangHongKongChinese )
        {
        isSplit = EFalse;
        }    

    iLangMan.SetSplitView(isSplit);

 
    MAknFepManagerInterface* fepUI = iLangMan.GetPluginInputFepUiL(
                                                    aSuggestMode, 
                                                    inputLang,
                                                    uiLanguage,
                                                    &iPenInputServer); 
    iCurrentPluginInputFepUI = fepUI;
    
    if ( iCurrentPluginInputFepUI )     
        {
        // iPenInputServer.DimUiLayout(EFalse);
        
        AddCurrentWinToOpenListL();
        if (iPluginInputMode == EPluginInputModeItut)
            {
            iIndicatorImgID = 0;
            iIndicatorTextID = 0;           
            }

        iPluginInputMode = (TPluginInputMode)iLangMan.CurrentImePlugin()->CurrentMode();


        // Addition for ITI features on FSQ
        // Before open touch window, need to set iPluginPrimaryRange = 0
        // which means use the default range.
        iPluginPrimaryRange = 0;
        
        InitializePluginInputL(aOpenMode, aSuggestRange, cleanContent);

        //following codes is used to tell MFNE editor the Touch Input 
        //has been opened. Because MFNE editor has no editorstate, so 
        //use this way to implement this.
        if ( iFepMan.IsMfneEditor() )
            {
            MCoeFepAwareTextEditor* mfne( iFepMan.FepAwareTextEditor() );
            iMfneChanged = ( iMfne != mfne );
            iMfne = mfne;
            InformMfneUiStatusL( ETrue );  
            }
        
        //save plugin input mode to repository
        if( iPenInputServer.PreferredUiMode() == EPluginInputModeNone )
            {
            //do not remember application set input mode
            iSharedData.SetPluginInputMode(iPluginInputMode);
            
            // Refresh landscape value
            CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation( size );
            landscape = size.iPixelSize.iWidth > size.iPixelSize.iHeight;     
            // Remember portrait input mode.  
            if ( !landscape )
                {
                iSharedData.SetPluginPortraitInputMode( iPluginInputMode ); 
                }
            }
            
        if (CurrentFepInputUI())
           	{
           	CurrentFepInputUI()->SetMode(iFepMan.InputMode(), ETrue, EFalse);		
           	}
			
        if(!iOrientationChangedfromUI)
		    {
		    iFepMan.UpdateCbaL( NULL );
		    }
        
        // inform edwin that Touch Input is opened.
        if ( iFepMan.EditorState() )
            {
            SetEdwinFlagsByUiStatus( ETrue );
            }
		
        // Notify application touch window state
        NotifyAppUiImeTouchWndStateL( ETrue );
        if (iPluginInputMode == EPluginInputModeItut)
            {
            RProperty::Set( KPSUidAknFep, KAknFepVirtualKeyboardType, EPtiKeyboard12Key );
            if(iFepMan.PtiEngine())
                {
                iFepMan.PtiEngine()->SetKeyboardType(EPtiKeyboard12Key);
                }
            iFepMan.SetQwertyMode( EFalse );    
            }

        rtn = ETrue;
        }
    else
        {
        iPluginInputMode = EPluginInputModeNone;
        }   
        
    if( iCurrentPluginInputFepUI && iPluginInputMode != EPluginInputModeNone )
        {
        iFepMan.UiInterface()->TouchPaneSetInputMethodIconActivated(ETrue);
        }
    else
        {
        iFepMan.UiInterface()->TouchPaneSetInputMethodIconActivated(EFalse);
        }
        
        
    if ( aOpenMode == EPenInputOpenManually )   
    	{
        iCurLanguage = iSharedData.InputTextLanguage();	
    	}
    return rtn;
    }
    
// -----------------------------------------------------------------------------
// CAknFepPluginManager::ClosePluginInputModeL
// Close plugin input mode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::ClosePluginInputModeL( TBool aRestore )
    {   
    if (iPluginInputMode == EPluginInputModeNone)
        {
        return;
        }
      
    iOldFepAwareEditor = NULL;	
    if ( iSharedData.QwertyInputMode() && iPenInputMenu )
        {
        iPenInputMenu->Hide();
        }    
        
    TPluginInputMode prePluginInputMode = iPluginInputMode;
    RemoveCurrentWinFromOpenList();
    ClosePluginInputUiL(ETrue);
    iClosePluginInputMode = ETrue;
       
    // TInt prevIputMode = iPluginInputMode;
    
    if( iOpenWindowList.Count() == 0 || iSharedData.QwertyInputMode() )
        {
        iPluginInputMode = EPluginInputModeNone;
        }
    
    if( aRestore && !iSharedData.QwertyInputMode() && iFepMan.InputMode() == ELatin)
        {
	    RestorePredictStateL();
        
        //  comment out the following code
        if ( iFepMan.EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor
             && !iOrientationChangedfromUI )
            {
            if( AknTextUtils::NumericEditorDigitType() == EDigitTypeEasternArabicIndic || 
                AknTextUtils::NumericEditorDigitType() == EDigitTypeDevanagari ||
                AknTextUtils::NumericEditorDigitType() == EDigitTypeArabicIndic )
                {
                iFepMan.TryChangeModeL( ENativeNumber );
                }
            else
                {
                iFepMan.TryChangeModeL( ENumber );
                }
            }
        }
    //iFepMan.UpdateCbaL( NULL ); 
                
    iFepMan.UiInterface()->TouchPaneSetInputMethodIconActivated(EFalse);
    
    // inform edwin that Touch Input is closed.
    SetEdwinFlagsByUiStatus( EFalse );
	
    if ( prePluginInputMode == EPluginInputModeVkb )
        {
        iFepMan.UpdateIndicators();
        }        
    }   
   
// -----------------------------------------------------------------------------
// CAknFepPluginManager::ClosePluginInputUiL
// Close plugin input UI, only close UI
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::ClosePluginInputUiL(TBool aResetState)
    {
    // For addition of ITI features on FSQ, 
    // need to restore some values stored before opening FSQ    
    
    iConnectAo->Cancel();
    ResetItiStateL();

    if( iPenInputSvrConnected )
        {
        if( iCurrentPluginInputFepUI )
            {
            //Change for finger support of MFNE editor, it is a requirement for Tube
            InformMfneUiStatusL( EFalse );            
            SetAknEdwinFlags( iEdwin, EAknEditorFlagHideTextView, EFalse );
            
            iPenInputServer.ClearServerEvent();
            if(iFocuschangedForSpellEditor)
                {
                iFocuschangedForSpellEditor = EFalse;
                //disable the redrawing, it will be redrawn in 
                //iCurrentPluginInputFepUI->HandleCommandL(ECmdPeninputDisableLayoutDrawing,ETrue);
                return; //don't close the UI if this is caused by removing spell editor
                }
            else
                {
                iCurrentPluginInputFepUI->CloseUI();
                }
            if( aResetState )
                {
                iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputWindowClose);
                }
            
            //iFepMan.UpdateCbaL(NULL); 
            
            if (iPluginInputMode == EPluginInputModeItut)
                {
                iIndicatorImgID = 0;
                iIndicatorTextID = 0;           
                }            
            }
        }
    
    // iITISettingDialogOpen = ETrue: ITI setting dialog is opened.
    // iOrientationChanged = ETrue: Phone is forcibly rotated.
    // iSharedData.AutoRotateEnabled() = ETrue: Sensor is on.
    // ITI setting dialog should be close by restoring orientation. 
    // Known case: 
    // When sensor is off, ITI setting dialog is opened on FSQ,
    // and then sensor is set to on, back to ITI setting dialog,
    // ITI setting dialog should be closed and Virtual ITU-T should be launched.
    if ( iITISettingDialogOpen && iSharedData.AutoRotateEnabled() && iOrientationChanged)
        {
        iITISettingDialogOpen = EFalse;
        }

    // The orientation should not be restored if ITI setting dialog is opened.
    // Otherwise, the orientation should be restored.
    if ( (iPluginInputMode == EPluginInputModeFSQ || (iPluginInputMode == EPluginInputModeFingerHwr 
         && iSharedData.InputTextLanguage() == ELangArabic)) && iOrientationChanged 
                                                    && !iITISettingDialogOpen )
        {
        // This TRAP_IGNORE is essential , never delete it
        TRAP_IGNORE( iAvkonAppUi->SetOrientationL( (CAknAppUiBase::TAppUiOrientation)iDefaultOrientation ) );
        iOrientationChanged = EFalse;
        } 
        
    // Notify editor the touch window has been closed
    NotifyAppUiImeTouchWndStateL( EFalse );
    
    iCharStartPostion = KInvalidValue;
    
    // Set the KAknFepTouchInputActive PS to 0, 
    // it means that touch input is inactive now.
    RProperty::Set( KPSUidAknFep, KAknFepTouchInputActive, 0 );
    
    // when UI close,iEdwin possible is deleted already,we should not access it.
    iEdwin = NULL;
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::OnResourceChangedL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::OnResourceChangedL( TInt aType )
    { 
    if ( !iCurrentPluginInputFepUI 
    	 || iPluginInputMode == EPluginInputModeNone )
        {
        return;
        }
        
    if ( (iPluginInputMode == EPluginInputModeFSQ || (iPluginInputMode == EPluginInputModeFingerHwr 
	     && iSharedData.InputTextLanguage() == ELangArabic))
	     && !iSharedData.AutoRotateEnabled() )
        {
        return;
        }    

    TInt preInputMode = iPluginInputMode;
    if ( iSharedData.AutoRotateEnabled() && 
        ( preInputMode == EPluginInputModeFSQ || preInputMode == EPluginInputModeItut ||
          preInputMode == EPluginInputModePortraitFSQ ) )
        {
        // Calculate current input mode by orientation.
        // even though peninputserver is brought to backgroud, 
        // the value for current input mode also needs to be ready. 
        // For example, when open task switcher, pen ui is invisible and peniput server is background, 
        // then switch orientation...
        // After that when focus go back to editor, config FEP must be done before pen ui open,
        // And FEP must know for which kind of input method it configur.        
        TPixelsTwipsAndRotation size; 
        CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);        
        iPluginInputMode = ( size.iPixelSize.iWidth < size.iPixelSize.iHeight ) ? 
            iSharedData.PluginPortraitInputMode() : EPluginInputModeFSQ;                       
        }
    
    if ( !iPenInputServer.IsForeground() )
	    {
	    return;
	    }

    TBool setResChange = EFalse;
    
    if ( preInputMode == EPluginInputModeItut || 
         preInputMode == EPluginInputModeFSQ ||
         preInputMode == EPluginInputModePortraitFSQ ||
         preInputMode == EPluginInputModeFingerHwr )
        {
        setResChange = ETrue;
        iPenInputServer.SetResourceChange(ETrue);           
        }
    
    iResourceChange = ETrue;   

    TBool needToChangeInputMode = ETrue;
    if ( iSharedData.AutoRotateEnabled() && 
         ( preInputMode == EPluginInputModeFSQ || 
           preInputMode == EPluginInputModePortraitFSQ || 
           preInputMode == EPluginInputModeItut ) )
        {
        if ( IsSpellVisible() )
            {
            iFepMan.ExitPluginSpellModeByCancel();
            iFepMan.SetNotifyPlugin( EFalse );
            iFepMan.HandleChangeInFocusForSettingFep();
            iFepMan.SetNotifyPlugin( ETrue );
            }
        
         if ( iPenInputMenu && iPenInputMenu->IsShowing() )
             {
             iPenInputMenu->Hide();
             ResetMenuState(EFalse);
             }
            
        if( preInputMode != iPluginInputMode )
            {            
            ClosePluginInputModeL(ETrue);
            iFepMan.TryCloseUiL();
            }
        else
            {
            needToChangeInputMode = EFalse;
            }
        }
    
    if(needToChangeInputMode)
	    {
		TryChangePluginInputModeByModeL( iPluginInputMode,
                                     EPenInputOpenManually,
                                     ERangeInvalid ); 
		}
    else 
        {
        if ( !NotifyInGlobalNoteEditorL() )
            {
            iFocuschangedForSpellEditor = EFalse; 
            iCurrentPluginInputFepUI->ActivateUI(); 
            iNeedFetchDimState = ETrue;
            }
        }

    
    /*if(size.iPixelSize.iWidth > size.iPixelSize.iHeight ) //landscape
        {
        if ( iPluginInputMode == EPluginInputModeVkb )
            {        	
            // Get store value from CenRep
            TryChangePluginInputModeByModeL((TPluginInputMode)(iSharedData.FepLastUseVkbModeForLandscape()),
                                            EPenInputOpenManually,
                                            ERangeInvalid); 
            hasChanged = ETrue;                                
            }                 
        } 
    else
        {
        if ( iPluginInputMode == EPluginInputModeFSQ )
            {
            TryChangePluginInputModeByModeL(EPluginInputModeVkb,
                                            EPenInputOpenManually,
                                            ERangeInvalid);     
            hasChanged = ETrue;                                       
            }
        
        }*/
    if ( /*!hasChanged &&*/ iCurrentPluginInputFepUI )
        {
        iCurrentPluginInputFepUI->ResourceChanged(aType);
        }
    
	if(needToChangeInputMode)
	    {
		if (iInMenu)
			{
			SetMenuState(EFalse);
			}
		}

	if (setResChange)    
        {
        iPenInputServer.SetResourceChange(EFalse);         
        }

    iResourceChange = EFalse;
    if(IsDisplayDataQuery() && //data query
       PluginInputMode() == EPluginInputModeVkb &&
       !iAdjustDataQueryTimer->IsActive()) //only vkb mode 
        {
        iAdjustDataQueryTimer->Start(KCursorBlinkPerioid, 
                                     KCursorBlinkPerioid, 
                                     TCallBack(AdjustDataQueryCallBackL , this)); 
        }
    }     

// ---------------------------------------------------------------------------
// CAknFepPluginManager::SyncFepAwareText
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::SyncFepAwareText( TPluginSync aSyncType, TBool /*aSendNow*/ )
    {
    TRAP_IGNORE(SendIcfDataL(aSyncType));
    }   
       
// ---------------------------------------------------------------------------
// CAknFepPluginManager::ProcessMenuCommand
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::ProcessMenuCommandL(TInt aCommandId)
    {
    if( iPenInputMenu && iPenInputMenu->IsShowing() )
        {
        iPenInputMenu->Hide();
        if( !iLangMan.CurrentImePlugin()->HandleMenuCommandL(aCommandId) )
            {
            switch(aCommandId)
                {
                case EPenInputCmdHwrTraining:
                    {
                    LaunchHwrTrainUiL();
                    iFepMan.SetGainForeground( EFalse );
                    }
                    break;
                case EPenInputCmdSetting:
                    {
                    if (iInMenu)
                        {
                        ResetMenuState(EFalse);    
                        ClosePluginInputUiL(ETrue);
                        if(iPenInputSvrConnected ) //lost foreground
                            {
                            iPenInputServer.LoseForeground();
                            }                        
                        }

                    LaunchPenInputSettingL();
                    }
                    break;                
                case EPenInputCmdVITUT:
                    {
                    ProcessChangingInputModeCmdL(EPluginInputModeItut);
                    }
                    break;
                case EPeninputCmdFSQ:
                // "Edit portrait" menu - switch to portrait FSQ
                case EPeninputCmdPortraitEditor:
                    {
                    if ( iPortraitFSQEnabled )
                        {
                        ProcessChangingInputModeCmdL(EPluginInputModePortraitFSQ);          
                        }
                    else 
                        {
                        ProcessChangingInputModeCmdL(EPluginInputModeFSQ);
                        }
                    }
                    break;
                // "Edit landscape" menu - switch to landscape FSQ
                case EPeninputCmdLandscapeEditor:

                    {
                    ProcessChangingInputModeCmdL(EPluginInputModeFSQ);
                    }
                    break;
                case EPeninputCmdHwr:
                	{
                    ProcessChangingInputModeCmdL(EPluginInputModeFingerHwr);  
                	}
                	break;
                case EFepInputCmdHelp:
                    {
                    if (iInMenu)
                        {
                        ResetMenuState(EFalse);    
                        ClosePluginInputUiL(ETrue);
                        if(iPenInputSvrConnected ) //lost foreground
                            {
                            iPenInputServer.LoseForeground();
                            }                        
                       }
                    if (GetCurAppUid().iUid != KAknHelpAppUid)
                        {
                        iLaunchHelp = ETrue;
                        }
                    LaunchHelpL();
                    }
                    break;
                case EPenInputCmdCut:
                    {//ctrl + x
                    iFepMan.HandleCopyCutEventL(EEikCmdEditCut);
                    }   // Send cut-event to edwin.
                    break;
                case EPenInputCmdCopy:
                    {//ctrl + c
                    iFepMan.HandleCopyCutEventL(EEikCmdEditCopy);
                    }   // Send copy-event to edwin.
                    break;
                case EEikCmdEditPaste:
                    {//ctrl + v
                    ResetMenuState();    
                    TKeyEvent ccpuKey = KAknCcpuPasteEvent;
                    CCoeEnv::Static()->SimulateKeyEventL( ccpuKey, EEventKey );
                    }   // Send copy-event to edwin.
                    break;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::HandleKeyEventL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::HandleKeyEventL(TInt aKeyId)
    {
    switch ( aKeyId )
        {
        case EKeyF19:
            {
            SyncFepAwareText( EPluginSyncUpdatedText );
            }
            break;
        case EPtiKeyHash:
            {
            if (PluginInputMode() == EPluginInputModeItut) 
                {
                if ( iFepMan.IsChineseInputLanguage() || 
                    !( iFepMan.InputMode() == ENumber && iFepMan.WasLastKeyPressAHashKey() ) )
                    {
                    if (iCurrentPluginInputFepUI)
                        {
                        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerInputMode,
                            iFepMan.InputMode());
                            
                        iIndicatorImgID = 0;
                        iIndicatorTextID = 0;  
                        UpdateITUTIndicator();

                        if (iFepMan.InputMode() == ELatin)
                            {
                            iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCaseMode, 
                                                                     iCaseMan.CurrentCase());	
                            }

                        if (CurrentFepInputUI())
                        	{
                        	CurrentFepInputUI()->SetMode(iFepMan.InputMode(), ETrue, EFalse);	
                        	}
                        }
                    }    
                if ( !iFepMan.IsSupportsSecretText() )
                    {
                    SyncFepAwareText( EPluginSyncUpdatedText );  
                    }
                else
                	{
                	if (iCurrentPluginInputFepUI)
                		{
                		iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCaseMode, 
                                                             iCaseMan.CurrentCase());	
                		}
                	}
                }
            }
            break;
        case EKeyCBA1:
            {
            }
            break;
        case EKeyCBA2:
            {
            }
            break;
        case EPtiKeyStar:
            {
			if (PluginInputMode() != EPluginInputModeNone )
                {
                SyncFepAwareText( EPluginSyncUpdatedText );
                }
            break;
            }
        case EPtiKey0: //fall through
        case EPtiKey1:
        case EPtiKey2:
        case EPtiKey3:
        case EPtiKey4:
        case EPtiKey5:
        case EPtiKey6:
        case EPtiKey7:
        case EPtiKey8:
        case EPtiKey9:
            {
            if (PluginInputMode() != EPluginInputModeNone )
                {
                SyncFepAwareText( EPluginSyncUpdatedText );
                }
            }
            break;
        case EKeyDownArrow:
        case EKeyUpArrow:
        case EKeyLeftArrow:
        case EKeyRightArrow:
            {
            SyncFepAwareText();
            }
            break;
        /*    
        case EKeyLeftArrow:
            {
           
            if ( !iFepMan.IsMfneEditor() &&
                 ( PluginInputMode() == EPluginInputModeItut || 
                   PluginInputMode() == EPluginInputModeFSQ ) &&
                 iCurSelPre.Length() > 0)
                {
                MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor(); 
                if(iCurSelPre.iCursorPos > 0)
                    {
                    TInt pos = iCurSelPre.iCursorPos - 1;
                    edit->SetCursorSelectionForFepL
                        ( TCursorSelection( pos, iCurSelPre.iAnchorPos ) );
					iCaseMan.UpdateCase(ENullNaviEvent);
                    }
                else  if(iCurSelPre.iCursorPos == 0)
                    {
                    edit->SetCursorSelectionForFepL
                        ( TCursorSelection( iCurSelPre.iCursorPos, iCurSelPre.iAnchorPos ) );
                    }
                }
            SyncFepAwareText();
            }
            break;
        case EKeyRightArrow:
            {
            MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
            if ( !iFepMan.IsMfneEditor() &&
                 ( PluginInputMode() == EPluginInputModeItut || 
                   PluginInputMode() == EPluginInputModeFSQ ) &&
                 iCurSelPre.Length() > 0)
                {          
                if(iCurSelPre.iCursorPos < edit->DocumentLengthForFep())
                    {
                    TInt pos = iCurSelPre.iCursorPos + 1;
                    edit->SetCursorSelectionForFepL
                        ( TCursorSelection( pos, iCurSelPre.iAnchorPos ) );
					iCaseMan.UpdateCase(ENullNaviEvent);
                    }
                else if(iCurSelPre.iCursorPos == edit->DocumentLengthForFep())
                    {
                    edit->SetCursorSelectionForFepL
                        ( TCursorSelection( iCurSelPre.iCursorPos, iCurSelPre.iAnchorPos ) );
                    }  
                }
            SyncFepAwareText();
            }
            break;
            */ 
        case EKeyBackspace:
            {
            }
            break;
        case EKeyOK:
            {
            ClosePluginInputUiL( ETrue );
            }
            break;
        case EKeyEscape:
            {
			ClosePluginInputUiL( ETrue );			
			iFepMan.SetNotifyPlugin( EFalse );
            HideSpellEditor();
            }
            break;
       }
    }
    
// ---------------------------------------------------------------------------
// CAknFepPluginManager::InitMenuPaneL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::InitMenuPaneL( CAknEdwinState* aEditorState, 
    CAknFepUiInterfaceMenuPane* aMenuPane, TInt aResourceId )
    {
    //CAknEdwinState* remeberedEditorState = iFepMan.RememberedEditorState();

    if(aResourceId == R_AVKON_TOUCH_TOUCHINPUT_MENU )
        {
        if (aEditorState && aEditorState->CcpuState())  
            {   
            if (!aEditorState->CcpuState()->CcpuCanCopy())
                {
                aMenuPane->SetItemDimmed(EEikCmdEditCopy, ETrue);           
                }   
                            
            if (!aEditorState->CcpuState()->CcpuCanCut())
                {
                aMenuPane->SetItemDimmed(EEikCmdEditCut, ETrue);          
                }               
                            
            if (!aEditorState->CcpuState()->CcpuCanPaste())         
                {
                aMenuPane->SetItemDimmed(EEikCmdEditPaste, ETrue);             
                }
            }
        else
            {
            aMenuPane->SetItemDimmed(EEikCmdEditCopy, ETrue);           
            aMenuPane->SetItemDimmed(EEikCmdEditCut, ETrue);          
            aMenuPane->SetItemDimmed(EEikCmdEditPaste, ETrue);             
            }

       // For training UI
       if (!(iPluginInputMode == EPluginInputModeHwr ||
             iPluginInputMode == EPluginInputModeFSc ||
             iPluginInputMode == EPluginInputModeFingerHwr))
            {
            aMenuPane->SetItemDimmed(EPenInputCmdSetting, ETrue);    
            aMenuPane->SetItemDimmed(EPenInputCmdHwrTraining, ETrue);    
            }
       else
            {
            // Mode is HWR or Full scree HWR
            // If fep is running in HWR Training, dim "HWR Training" item.
            TUid appId = GetCurAppUid();
            if ( appId == KUidtruiApp )
                {
                aMenuPane->SetItemDimmed( EPenInputCmdHwrTraining, ETrue );
                }
            }
       
       // For Predictive
       if (iPluginInputMode != EPluginInputModeItut)
           {
           aMenuPane->SetItemDimmed(EAknCmdEditInsertSymbol, ETrue);
           aMenuPane->SetItemDimmed(EAknCmdPredictiveTextCascade, ETrue); 
           aMenuPane->SetItemDimmed(EAknCmdMultitapPredictiveT9On, ETrue); 
           }
                   
       //For Preview bubble
       if ( ! (iPluginInputMode == EPluginInputModeVkb ||
               iPluginInputMode == EPluginInputModeFSQ ||
               iPluginInputMode == EPluginInputModeFingerHwr ||
               iPluginInputMode == EPluginInputModePortraitFSQ ) )
           {
           if(!(iPluginInputMode == EPluginInputModeItut &&
             (iSharedData.InputTextLanguage() == ELangPrcChinese ||
             iSharedData.InputTextLanguage() == ELangTaiwanChinese ||
             iSharedData.InputTextLanguage() == ELangHongKongChinese )))
               {
               aMenuPane->SetItemDimmed(EPenInputPreview, ETrue);
               }
           }
            
       // for inputmode
       if (iPluginInputMode != EPluginInputModeItut)
           {
            // dim menu items for Chinese input modes that aren't valid in the current editor
            // or the device subvariant
            for (TInt mode = KFirstMode; mode <= KLastMode; mode = mode << 1)
                {
                iFepMan.DimMenuItem(aMenuPane, mode);
                }
           aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);     
           aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue); 
           aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue); 
           
           }
       else 
           {
           iFepMan.DimInputmodeMenuItems(aMenuPane);
           if (iFepMan.IsChineseInputLanguage())
                {
                DimAllModeAndCase(aMenuPane);   
                }
           }
       if (iFepMan.IsAbleToLaunchSmiley()) 
           {
           TInt indexForSmiley;
           if (aMenuPane->MenuItemExists(EAknCmdEditInsertSmiley, 
                   indexForSmiley))
               {
               aMenuPane->SetItemDimmed(EAknCmdEditInsertSmiley, EFalse);
               }
           }
           
        // Add Insert Symbol for valid input modes.   
        if ( iPluginInputMode == EPluginInputModeItut ||
             iPluginInputMode == EPluginInputModeVkb ||
             iPluginInputMode == EPluginInputModeFSQ ||
             iPluginInputMode == EPluginInputModePortraitFSQ )
            {
            TInt index = 0;     
            if (isLanuchSCT && aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index))
                 {
                 aMenuPane->SetItemDimmed(EAknCmdEditInsertSymbol, EFalse);
                 }
            }
        
        TBool disableDic = ETrue;
        
        if ((iPluginInputMode == EPluginInputModeHwr)
                && (iFepMan.IsArabicInputLanguage()))
            {
            disableDic = EFalse;
            }

        TInt dicIndex = 0;
        if (aMenuPane->MenuItemExists(
                EPenInputCmdRecognitionWithDictionary, dicIndex))
            {
            aMenuPane->SetItemDimmed(
                    EPenInputCmdRecognitionWithDictionary, disableDic);
            }
        
        // add some input mode in option menu
        
    	TInt disabledInputMode = iPenInputServer.DisabledLayout();
    	TInt curInputMode = iLangMan.CurrentImePlugin()->CurrentMode();
    	TBool isChinese = iFepMan.IsChineseInputLanguage();
    	
    /* US2/US4/US5
    1. Orientation sensor off

        1-1. Portrait orientation
            If currently ITU-T mode
            “QWERTY keyboard?-> Port fsq
            “Edit landscape?-> Land fsq

            If currently Portrait FSQ mode
            “Alphanumeric keypad?
            “Edit landscape?-> Land fsq

        1-2. Landscape orientation
            “Alphanumeric keypad?
            “Edit portrait?- Port fsq

    2. Orientation sensor on

        2-1.Portrait orientation
            If currently ITU-T mode
            “QWERTY keyboard?-> Port fsq

            If currently Portrait FSQ mode
            “Alphanumeric keypad?

        2-2. Landscape orientation
            --- No input switch menu
*/

    // Get the current phone's orientation
    TPixelsTwipsAndRotation size; 
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
    TBool isPortrait = size.iPixelSize.iWidth < size.iPixelSize.iHeight;

    // "QWERTY keyboard" - EPeninputCmdFSQ menu
    if ( iPortraitFSQEnabled )
        {
        // "QWERTY keyboard" - EPeninputCmdFSQ menu is shown in these conditions
        // if portrait FSQ feature flag is enabled
        // 1) Non-Chinese language
        // 2) Current input mode is not portrait FSQ and also the mode is not disabled
        // 3) Current orientation is portrait
        if ( !isChinese
            && ( curInputMode != EPluginInputModePortraitFSQ ) 
            && !( disabledInputMode & EPluginInputModePortraitFSQ ) 
            && isPortrait )
        {
        aMenuPane->SetItemDimmed( EPeninputCmdFSQ, EFalse );
        }
        
        }
    else
        {
        if ( !isChinese && ( curInputMode != EPluginInputModeFSQ ) 
            && !( disabledInputMode & EPluginInputModeFSQ ) 
            && FeatureManager::FeatureSupported( KFeatureIdVirtualFullscrQwertyInput ) )
            {
            aMenuPane->SetItemDimmed( EPeninputCmdFSQ, EFalse );
            }
        }

    // "Alphanumeric keypad" - EPenInputCmdVITUT menu is shown in these conditions.
    // 1) Non-Chinese language
    // 2) Current input mode is not ITU-T and also the mode is not disabled
    // 3) ITU-T feature flag is enabled
    if ( !isChinese 
        && ( curInputMode != EPluginInputModeItut ) 
        && !( disabledInputMode & EPluginInputModeItut ) 
        && FeatureManager::FeatureSupported( KFeatureIdVirtualItutInput ) )
        {
        aMenuPane->SetItemDimmed(EPenInputCmdVITUT, EFalse);
        }
    // "Edit portrait" - EPeninputCmdPortraitEditor menu is shown in these conditions.
    // 1) Portrait FSQ feature flag is enabled
    // 2) Non-Chinese language
    // 3) Current input mode is not portrait FSQ and also the mode is not disabled
    // 4) Current orientation is landscape
    if ( iPortraitFSQEnabled
        && !isChinese 
        && ( curInputMode != EPluginInputModePortraitFSQ ) 
        && !( disabledInputMode & EPluginInputModePortraitFSQ ) 
        && !isPortrait )
        {
        aMenuPane->SetItemDimmed(EPeninputCmdPortraitEditor, EFalse);
        }

    // "Edit landscape" - EPeninputCmdLandscapeEditor menu is shown in these conditions.
    // 1) Portrait FSQ feature flag is enabled 
    //    This menu needed only when the feature is turned on.
    // 2) Landscape FSQ feature flag is enabled
    // 3) Non-Chinese language
    // 4) Current input mode is not landscape FSQ and also the mode is not disabled
    // 5) Current orientation is portrait
    if ( iPortraitFSQEnabled
        && FeatureManager::FeatureSupported( KFeatureIdVirtualFullscrQwertyInput ) 
        && !isChinese 
        && ( curInputMode != EPluginInputModeFSQ ) 
        && !( disabledInputMode & EPluginInputModeFSQ ) 
        && isPortrait )
        {
        aMenuPane->SetItemDimmed(EPeninputCmdLandscapeEditor, EFalse);
        }
   	
   	if ( FeatureManager::FeatureSupported( KFeatureIdArabicHandwritingRecognitionInput )
           &&iFepMan.IsArabicInputLanguage() 
           && ( curInputMode != EPluginInputModeFingerHwr )
   	    && !( disabledInputMode & EPluginInputModeFingerHwr ))
   	    {
   	    aMenuPane->SetItemDimmed( EPeninputCmdHwr, EFalse );
   	    }
		
		// check if this is the arabic finger hwr
		TBool isArabicFingerHwr = (iPluginInputMode == EPluginInputModeFingerHwr
		                            && ELangArabic  == iSharedData.InputTextLanguage());
		
        // Initialize the menu item for Arabic Finger HWR
        if(isArabicFingerHwr)
            {
            InitMenuItemForArabicFingerHwrL(aMenuPane);
            }	     
        
        iLangMan.CurrentImePlugin()->DynInitMenuPaneL(aMenuPane);
        }
    }
    
void CAknFepPluginManager::InitMenuItemForArabicFingerHwrL(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
	TInt index = 0;
	TBool isAutoRotateEnabled = iSharedData.AutoRotateEnabled();
	// add menu item to switch to landscape or protrait
	if(!isAutoRotateEnabled)
		{
		TPixelsTwipsAndRotation size; 
		CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
		TBool landscape = size.iPixelSize.iWidth > size.iPixelSize.iHeight;
		if(landscape)
			{
			index = 0;
			// add switch to protrait
			if(aMenuPane->MenuItemExists(EPenInputCmdHwrInputToPortrait, index))
				{
				aMenuPane->SetItemDimmed(EPenInputCmdHwrInputToPortrait,EFalse);
				}
			}
		else
			{
			index = 0;
			// add switch to landscape
			if(aMenuPane->MenuItemExists(EPenInputCmdHwrInputToLandscape, index))
				{
				aMenuPane->SetItemDimmed(EPenInputCmdHwrInputToLandscape,EFalse);
				}
			}
		}
	
	// hide the setting menu			
//	index = 0;
//	if(aMenuPane->MenuItemExists(EPenInputCmdSetting, index))
//		{
//		aMenuPane->SetItemDimmed(EPenInputCmdSetting, ETrue);
//		}    			
	
	//Show number mode menu item.
	if(iFepMan.InputMode() == ENumber || iFepMan.InputMode() == ENativeNumber)
	    {
        if(iSharedData.DefaultArabicNumberMode())
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeNumber, EFalse);
            }
        else
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeArabicIndicNumber, EFalse);
            }
	    }
	// add the writing speed menu
	index = 0;
	if(aMenuPane->MenuItemExists(EPenInputCmdWritingSpeed, index))
		{
		aMenuPane->SetItemDimmed(EPenInputCmdWritingSpeed,EFalse);
		}
	
	// add the guidingline menu
	index = 0;
	if(aMenuPane->MenuItemExists(EPenInputCmdGuidingLine, index))
		{ 
		aMenuPane->SetItemDimmed(EPenInputCmdGuidingLine,EFalse);			
		}
	}
 
// ---------------------------------------------------------------------------
// CAknFepPluginManager::OnFocusChangedL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::OnFocusChangedL( TBool aGainForeground )
    {
    if (iLaunchHelp)
        {
        return;    
        }
    
    if ( !iForegroundChange && iFepMan.StopProcessFocus()  )
        {
        if (iFepMan.CloseUiOnFocusChange())
            {
            ClosePluginInputUiL(ETrue);   
            }
                    
        return ;
        }

    if(aGainForeground && !iPenInputServer.AutoOpen())
        {
        ClosePluginInputUiL(ETrue);    
        return;	
        }

    
    TUid appId = GetCurAppUid();
    TBool bClose = ETrue;
    TBool bOpen = ETrue;
    TUid focusAppId = GetFocusAppUid();
    
    if ( !aGainForeground && appId == focusAppId )
        {
        return;
        }
        
    if( !iLaunchMenu && IsGlobalNotesApp(focusAppId)) 

        {
        if ( iPluginInputMode == EPluginInputModeItut ||
             iPluginInputMode == EPluginInputModeFSQ ||
             iPluginInputMode == EPluginInputModeFingerHwr ||
             iPluginInputMode == EPluginInputModePortraitFSQ )
            {
            bClose = EFalse;
            }

        }
    iLaunchMenu = EFalse;
    
    // In globsl notes editor, launching global notes not fep dlg or menu
    if(focusAppId.iUid == appId.iUid &&
       IsGlobalNotesApp(appId) &&
       !iFepMan.IsFepAwareTextEditor())
        {
        if ( iPluginInputMode == EPluginInputModeItut ||
             iPluginInputMode == EPluginInputModeFSQ ||
             iPluginInputMode == EPluginInputModeFingerHwr ||
             iPluginInputMode == EPluginInputModePortraitFSQ )
            {
             ClosePluginInputUiL(ETrue);
             if(iPenInputSvrConnected ) //lost foreground
                {
                iPenInputServer.LoseForeground();
                }
                
            return;
            }
        }
    
    //global notes editor won't open automatically
    /*if(IsGlobalNotesApp(appId) || IsGlobalNotesApp(focusAppId))
        {
        if(!iPenInputServer.IsDimmed())
            bOpen = EFalse;						
        }*/

    // The state means: when current app with dim state lose foreground by non-capserver or notify server
    // Need to cancel dim state
    if ( iForegroundChange && !aGainForeground && !IsGlobalNotesApp( focusAppId ) && iInMenu )
    	{
		ResetMenuState();
    	}  
    
    if ( aGainForeground && iFepMan.FepAwareTextEditor() && IsCurrentWindInOpenList() )
        {
        if(!iPenInputSvrConnected) 
            {
            HandleEventsFromFepL(EPluginEditorActivate,0);
            }
                
        if(!bOpen )
            {
            return;
            }

        if( iInMenu && 
        	( iCurEditor == iFepMan.FepAwareTextEditor() || iLaunchSCTInSpell ) )
            {            
            iLaunchSCTInSpell = EFalse;
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS
            if ( ( PluginInputMode() == EPluginInputModeFSQ ||
                   PluginInputMode() == EPluginInputModePortraitFSQ )
                    && iSharedData.ThemeEffectsEnabled() )
                {
                User::After(1);//waiting for menu cancel effect
                } 
#endif
            ResetMenuState();            
			// If need to open setting app automatically, 
			// do not open Touch window again. 
			TBool langChange = iCurLanguage != iSharedData.InputTextLanguage();
					
			TryChangePluginInputModeByModeL(
							(TPluginInputMode)iSharedData.PluginInputMode(),
				langChange?EPenInputOpenManually:EPenInputOpenAutomatically,
				ERangeInvalid);            
			return;
            }
            
//       iPenInputServer.UpdateAppInfo(KNullDesC, EAppIndicatorMsg);  
          
        //only automatically open when window in the list
        
        if ( !iFastSwapByMenu )
            {  
            TBool langChange = iCurLanguage != iSharedData.InputTextLanguage();
 //               iPenInputServer.DimUiLayout(EFalse);
            TBool enableAdaptiveSearch = EFalse;
            if ( iFepMan.IsFindPaneEditor() && iFepMan.IsSupportedAdaptiveSearch() )
                {
                iAvkonRepository->Get( KAknAvkonAdaptiveSearchEnabled, enableAdaptiveSearch );
                if ( enableAdaptiveSearch )
                    {
                    if ( !(iInMenu && 
                            ( iPluginInputMode == EPluginInputModeItut ||
                              iPluginInputMode == EPluginInputModeFSQ ||
                              iPluginInputMode ==EPluginInputModeFingerHwr ||
                              iPluginInputMode == EPluginInputModePortraitFSQ ) ) )
                        {
                        ClosePluginInputModeL(ETrue);
                        if(iPenInputSvrConnected ) //lost foreground
                            {
                            iPenInputServer.LoseForeground();
                            }                
                        }                
                    }
                }   
            if ( !enableAdaptiveSearch )
                {
			TInt openMode = EPenInputOpenAutomatically;
			TPluginInputMode inputMode = (TPluginInputMode)iSharedData.PluginInputMode();                        
				if ((inputMode == EPluginInputModeFSc || inputMode == EPluginInputModeHwr ||
					 inputMode == EPluginInputModeFingerHwr) &&
				     iFepMan.IsSupportsSecretText())
				{
                    openMode = EPenInputOpenManually;
                    }
                if(langChange)
                    {
                    openMode = EPenInputOpenManually;
                    }
             
                TryChangePluginInputModeByModeL(inputMode, openMode, ERangeInvalid);
                }
            }          
        iFastSwapByMenu = EFalse;                   
        }
    else 
        {   
        TBool enableAdaptiveSearch = EFalse;
        
        if ( iFepMan.IsFindPaneEditor() && iFepMan.IsSupportedAdaptiveSearch() )
            {
            iAvkonRepository->Get( KAknAvkonAdaptiveSearchEnabled, enableAdaptiveSearch );
            if ( enableAdaptiveSearch )
                {
                if ( iInMenu && ( iPluginInputMode == EPluginInputModeItut ||
                    iPluginInputMode == EPluginInputModeFSQ ||
                    iPluginInputMode == EPluginInputModeFingerHwr ||
                    iPluginInputMode == EPluginInputModePortraitFSQ ) )
                    {                
                    //iPenInputServer.DimUiLayout(ETrue); //dim the ui
                    return;
                    }                
                
                ClosePluginInputModeL(ETrue);
                if(iPenInputSvrConnected ) //lost foreground
                    {
                    iPenInputServer.LoseForeground(); 
                    }
                }
            }
        
        if ( !enableAdaptiveSearch )
            {
            if( iInMenu && ( iPluginInputMode == EPluginInputModeItut ||
                iPluginInputMode == EPluginInputModeFSQ ||
                iPluginInputMode == EPluginInputModeFingerHwr || 
                iPluginInputMode == EPluginInputModePortraitFSQ ) )
                {                
                //iPenInputServer.DimUiLayout(ETrue); //dim the ui
                return;
                }            
            
            //current control is not any kind of editor
            if(bClose)
                {
                ClosePluginInputUiL(ETrue);
                if(iPenInputSvrConnected ) //lost foreground
                    {
                    iPenInputServer.LoseForeground();
                    }
                }
            else
                {
                //send dim command
                //global notes app do nothing when lose focus
                if( iPenInputSvrConnected )
                    {
                    iPenInputServer.SetInternalPopUp(iInMenu);
                    iPenInputServer.SetEditorPriority(iEditorPriority);                
                    TUid curApp = GetCurAppUid();          
                    iPenInputServer.SetGlobalNotes( IsGlobalNotesApp(curApp) && 
                                                    !iInGlobleNoteEditor);
                    iPenInputServer.DimUiLayout(ETrue);
                    }
                }
            }
        }

    //set touch pane enable/disable
    if( iFepMan.FepAwareTextEditor() )
        {
        iFepMan.UiInterface()->TouchPaneAllowInputMethodActivation(ETrue);
        }
    else 
        {   
        iFepMan.UiInterface()->TouchPaneAllowInputMethodActivation(EFalse);
        }

    iFepMan.UiInterface()->TouchPaneRefreshL();        
    }    
void CAknFepPluginManager::SendIcfDataL(  TPluginSync aSyncType )
    {
    if( PluginInputMode() == EPluginInputModeNone || 
        !iCurrentPluginInputFepUI )
        {
        return;
        }
        
    MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
    if ( !edit )
        {
        if ( IsDimed() )
            {
            TFepInputContextFieldData icfData;
            icfData.iCmd = EPeninputICFInitial;
    	    icfData.iText.Set(*iLastEditorContentBuf);
    	    icfData.iCurSel = iCurSelPre;
    	    // special case for vietnamese, we need en/disable tone marker
    	    // when the text in editor changes, 
    	    // by checking whether the character before insertion point is vowel or not
    	    VietSyncToneMarkerL(*iLastEditorContentBuf, icfData.iCurSel);
    	    
    	    if ( iFepMan.IsFlagSet(CAknFepManager::EFlagNoMatches) )
                {
                icfData.iFlag = EFepICFDataInlineNoMatch;
                }
            if ( ExistInlineText() ) // inline
                {
                icfData.iStartPos = iFepMan.UncommittedText().LowerPos();
                icfData.iLength = iCurSelPre.iCursorPos - icfData.iStartPos;
                icfData.iMidPos = 0;
                }
            iCurrentPluginInputFepUI->HandleCommandL
                (ECmdPenInputSendEditorTextAndCurPos, reinterpret_cast<TInt>(&icfData));
            }
        return;
        }
            
    if( iDisableSyncText )            
        {
        iPendingSyncReq = ETrue;
        return;
        }

    iPendingSyncReq = EFalse;
            
    TBool enableCursor;
    TBool enableSelection;
    TFepInputContextFieldData icfData;
    
    if ( iFepMan.IsFeatureSupportedJapanese()
        && EPluginSyncCurSel == aSyncType
        && iFepMan.FepAwareTextEditor()->DocumentLengthForFep() > iPreDocumentLengthForFep
        && iCurSelCur.iAnchorPos > iPreDocumentLengthForFep ) // add linefeed by Down Key
        {
        aSyncType = EPluginSyncUpdatedText;
        iCurSelPre.iAnchorPos = iPreDocumentLengthForFep;
        }    
    
    edit->GetCursorSelectionForFep( iCurSelCur );
    TInt editContentLen = edit->DocumentLengthForFep();	
    	
    if( iCurSelCur.iCursorPos > editContentLen )
    	{
    	iCurSelCur.iCursorPos = editContentLen;
    	}
    if( iCurSelCur.iAnchorPos > editContentLen )
    	{
    	iCurSelCur.iAnchorPos = editContentLen;
    	}
    	
    // 1. set cursor visiblity
    CAknEdwinState* editorState = iFepMan.EditorState();
    TBool secretEditor = iFepMan.IsSupportsSecretText();
    
    if (editorState)
        {
		enableCursor = !IsEditorCursorDisabled(); 
        enableSelection = IsEditorCursorSelVisible();
        }
    else // MFNE
        {        
        enableCursor = (iCurSelCur.Length()==0);
        enableSelection = (iCurSelCur.Length()>0);
        }
    icfData.iCursorVisibility = enableCursor;
    icfData.iCursorSelVisible = enableSelection;    
 
    delete iIcfDataBuf;
    iIcfDataBuf = NULL;
    
    // 2. set cursor xy-position in window
    TPoint position(-1,-1);
    TInt height = 0;
    TInt ascent = 0;
    
    if( PluginInputMode() == EPluginInputModeFSc )
        {
        edit->GetScreenCoordinatesForFepL
            ( position, height, ascent, iCurSelCur.iCursorPos );
        icfData.iRect = TRect( TPoint( position.iX, position.iY-ascent ),
                               TPoint( position.iX, position.iY+height-ascent ) );
        }
    
    // 3. set cursor selection
    icfData.iCurSel = iCurSelCur;

    // 4. set leading edge flag
    icfData.iLeadingEdge = (iFepMan.DocPos().iType == TTmDocPosSpec::ELeading);
    
    // 5. set icf direction 
    if ( iFepMan.IsMfneEditor() )
        {
        icfData.iFlag = EFepICFDataDirectionMFNE;
        }
    else
        {
        if ( TBidiText::ScriptDirectionality
            ( TLanguage( iFepMan.InputLanguageCapabilities().iInputLanguageCode ) ) ==
            TBidiText::ELeftToRight )
            {
            icfData.iFlag = EFepICFDataDirectionLTR;
            }
        else
            {
            icfData.iFlag = EFepICFDataDirectionRTL;
            }
        }
    
    // 6. set other icf data info
    // get last editor content
	TPtrC lastEditorContent;
    if ( !iLastEditorContentBuf )
    	{
    	lastEditorContent.Set( KNullDesC );
    	}
    else
    	{
    	lastEditorContent.Set( iLastEditorContentBuf->Des() );
    	}
    
    // get current editor content	
    HBufC* currentEditorContentBuf = HBufC::NewLC( editContentLen );
    TPtr16 currentEditorContent = currentEditorContentBuf->Des();
	edit->GetEditorContentForFep( currentEditorContent, 0, editContentLen ); 
    // special case for vietnamese, we need en/disable tone marker
    // when the text in editor changes, 
    // by checking whether the character before insertion point is vowel or not
	VietSyncToneMarkerL(currentEditorContent, icfData.iCurSel);

	if (SetSyncIcfDataL( icfData, lastEditorContent, currentEditorContent))
    	{
		iCurrentPluginInputFepUI->HandleCommandL
			(ECmdPenInputSendEditorTextAndCurPos, reinterpret_cast<TInt>(&icfData));
		iFepMan.TryPopExactWordInICFL();
    	}

    if ( secretEditor ) 
        { 
        TBool cursorDisabled = IsEditorCursorDisabled(); 
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputDimArrowKeys, cursorDisabled );        
        } 


    // reset last editor content buffer
    delete iLastEditorContentBuf;
    iLastEditorContentBuf = currentEditorContentBuf;
	CleanupStack::Pop(); // currentEditorContentBuf
	
    iCurSelPre.iCursorPos = iCurSelCur.iCursorPos;
    iCurSelPre.iAnchorPos = iCurSelCur.iAnchorPos;
    iPreDocumentLengthForFep = editContentLen;
    }

void CAknFepPluginManager::VietSyncToneMarkerL(
		const TDesC& aEditorContent, const TCursorSelection& aCursorPos)
	{
	if(ELangVietnamese != iLangMan.InputLanguage())
		{
		return;
		}
	
	TBool bEnable = EFalse;
	TInt low = aCursorPos.LowerPos();
	
	if(low > 0)
		{
		ASSERT(aEditorContent.Length() >= low);
		TBuf<1> buf = aEditorContent.Mid(low - 1, 1);
		TBuf<1> vowel; 
		TInt cnt = sizeof(VietVowelList) / sizeof(VietVowelList[0]);
		
		for(TInt i = 0; !bEnable && i < cnt; ++i)
			{
			const TText* text = &VietVowelList[i];
			vowel.SetLength(0);
			vowel.Append(text, 1);
			bEnable = (buf.Compare(vowel) == 0);
			}
		}
	
	iCurrentPluginInputFepUI->HandleCommandL(ECmdPeninputVietSyncToneMarker, bEnable);  
	}

TBool CAknFepPluginManager::SetSyncIcfDataL( TFepInputContextFieldData& aIcfData,
                                            const TDesC& aLastEditorContent,
                                            const TDesC& aCurrentEditorContent )
	{
	MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
	
	    // if content is the same, sync cursor selection only
    if ( !iModeChanged && 
        aLastEditorContent.Compare( aCurrentEditorContent ) == 0 )
    	{
        if ( aIcfData.iCurSel.HigherPos() > edit->DocumentLengthForFep() || 
         aIcfData.iCurSel.LowerPos() < 0 || IsDimed() )
            {
            return EFalse;
            }
    	return SetSyncCurSelIcfDataL( aIcfData );    	
    	}
    		
	if ( TextInlined( aLastEditorContent, aCurrentEditorContent ) ) // inline
        {
        aIcfData.iCmd = EPeninputICFReplace;
        aIcfData.iStartPos = iFepMan.UncommittedText().LowerPos();
        aIcfData.iLength = iCurSelPre.HigherPos() - aIcfData.iStartPos;
        TInt readLen = iFepMan.UncommittedText().Length();
        iIcfDataBuf = HBufC::NewL( readLen );
        TPtr16 icfDes = iIcfDataBuf->Des();
        edit->GetEditorContentForFep( icfDes, aIcfData.iStartPos, readLen ); 
        if ( iFepMan.IsFlagSet(CAknFepManager::EFlagNoMatches) )
            {
            aIcfData.iFlag = EFepICFDataInlineNoMatch;
            }
        aIcfData.iText.Set(icfDes);
        
    	if ( EnableITIOnFSQ() )
    	    {
    	    TInt tailLength = 0;
    	    iFepMan.PtiEngine()->HandleCommandL
    	             ( EPtiCommandGetAutoCompletionTailLength, &tailLength );
    	    if ( tailLength > 0 )
    	        {
    	        aIcfData.iMidPos = tailLength;
    	        }
            else
                {
                aIcfData.iMidPos = 0;
                }
    	    }    	
        else
            {
            aIcfData.iMidPos = 0;
            }

        aIcfData.iCursorVisibility = EFalse;
        aIcfData.iCursorSelVisible = EFalse;
        }
    else if ( TextInserted( aLastEditorContent, aCurrentEditorContent ) ) // insert
        {
        aIcfData.iCmd = EPeninputICFReplace;
        aIcfData.iStartPos = iCurSelPre.LowerPos();
        aIcfData.iLength = iCurSelPre.Length();
        aIcfData.iMidPos = -1;
        TInt readLen = iCurSelCur.iCursorPos - iCurSelPre.LowerPos();
        iIcfDataBuf = HBufC::NewL( readLen );
        TPtr16 icfDes = iIcfDataBuf->Des();
        edit->GetEditorContentForFep( icfDes, aIcfData.iStartPos, readLen );
        aIcfData.iText.Set(icfDes);
        }
    else if ( TextDeleted( aLastEditorContent, aCurrentEditorContent ) ) // delete
        {
        aIcfData.iCmd = EPeninputICFDelete;
        aIcfData.iStartPos = iCurSelCur.iCursorPos;
        aIcfData.iLength = aLastEditorContent.Length() - aCurrentEditorContent.Length();
        aIcfData.iMidPos = -1;
        aIcfData.iText.Set(KNullDesC);
        }
    else if ( !iModeChanged && iFepMan.IsFeatureSupportedJapanese()
        && (iFepMan.FepAwareTextEditor()->DocumentLengthForFep() < iPreDocumentLengthForFep
            || iFepMan.FepAwareTextEditor()->DocumentLengthForFep() == iPreDocumentLengthForFep 
            && iCurSelCur.iCursorPos > 0)
        && iCurSelCur.iCursorPos == iCurSelPre.iCursorPos
        && iCurSelCur.iAnchorPos == iCurSelPre.iAnchorPos )
        {
        if ( iFepMan.FepAwareTextEditor()->DocumentLengthForFep() < iPreDocumentLengthForFep ) // Delete Right
            {
            aIcfData.iCmd = EPeninputICFDelete;
            aIcfData.iStartPos = iCurSelCur.iCursorPos;
            aIcfData.iLength = iPreDocumentLengthForFep - 
                               iFepMan.FepAwareTextEditor()->DocumentLengthForFep();
            aIcfData.iMidPos = -1;
            aIcfData.iText.Set(KNullDesC);
            }
        else  // Replace
            {
            TInt repLen = (iCurSelCur.iCursorPos > 1)? 2:1;
            iIcfDataBuf = HBufC::NewL( repLen );
            TPtr16 icfDes = iIcfDataBuf->Des();
            iFepMan.FepAwareTextEditor()->GetEditorContentForFep( icfDes, 
                                                                  iCurSelCur.iCursorPos - repLen, 
                                                                  repLen );
            if (2 == repLen && icfDes[0] == KVkbParagraphSeparator)
                {
                repLen = 1;
                icfDes.Delete(0,1);
                }
            aIcfData.iCmd = EPeninputICFReplace;
            aIcfData.iStartPos = iCurSelCur.iCursorPos - repLen;
            aIcfData.iLength = repLen;
            aIcfData.iMidPos = -1;
            aIcfData.iText.Set(icfDes);
            }
        }
    else if ( TextMultiTapped( aLastEditorContent, aCurrentEditorContent ) )// multitap
        {
        aIcfData.iCmd = EPeninputICFReplace;
        aIcfData.iStartPos = iCurSelCur.iCursorPos - 1;
        aIcfData.iLength = 1;
        aIcfData.iMidPos = -1;
        iIcfDataBuf = HBufC::NewL( aIcfData.iLength );
        TPtr16 icfDes = iIcfDataBuf->Des();
        edit->GetEditorContentForFep( icfDes, aIcfData.iStartPos, aIcfData.iLength );
        aIcfData.iText.Set(icfDes);
        }
    else
        {
        aIcfData.iCmd = EPeninputICFInitial;
	    aIcfData.iStartPos = 0;
	    aIcfData.iLength = edit->DocumentLengthForFep();
	    aIcfData.iMidPos = -1;
	    iIcfDataBuf = HBufC::NewL( aIcfData.iLength );
	    TPtr16 icfDes = iIcfDataBuf->Des();
	    edit->GetEditorContentForFep( icfDes, 0, aIcfData.iLength );
	    aIcfData.iText.Set(icfDes);
        }
    if ( aIcfData.iStartPos > edit->DocumentLengthForFep() || aIcfData.iStartPos < 0 )
        {
        return EFalse;
        }
    return ETrue;
	}
	
void CAknFepPluginManager::SetICFTextForSpellL()
	{
	MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
	
	if ( edit )
		{
		TInt length = edit->DocumentLengthForFep();
		HBufC* icfTextBuf = HBufC::NewLC( length );
    	TPtr icfText = icfTextBuf->Des();

		edit->GetEditorContentForFep( icfText, 0, length );
		
		TCursorSelection curSel;
		edit->GetCursorSelectionForFep( curSel );
		TInt curPos = curSel.iCursorPos;
		
		HBufC* titleStr = StringLoader::LoadL(R_AKNFEP_PROMPT_TEXT);                          
		CleanupStack::PushL( titleStr );
		TPtr16 promptText = titleStr->Des();
		
		TFepSpellICFDisplayContent displayContent;
		displayContent.iCurPos = curPos;
		displayContent.iICFText.Set( icfText );
		displayContent.iPromptText.Set( promptText );
		
		iCurrentPluginInputFepUI->HandleCommandL( 
        	ECmdPeninputSpellICFDisplayContent,
            reinterpret_cast<TInt>(&displayContent));
        
        CleanupStack::PopAndDestroy( titleStr );
		CleanupStack::PopAndDestroy( icfTextBuf );
		}
	}
TBool CAknFepPluginManager::SetSyncCurSelIcfDataL( TFepInputContextFieldData& aIcfData )
	{
	MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
	if ( ExistInlineText() ) // inline
        {
        aIcfData.iCmd = EPeninputICFSetCurSel;
        aIcfData.iStartPos = iFepMan.UncommittedText().LowerPos();
        aIcfData.iLength = iCurSelPre.iCursorPos - aIcfData.iStartPos;
        TInt readLen = iFepMan.UncommittedText().Length();
        iIcfDataBuf = HBufC::NewL( readLen );
        TPtr16 icfDes = iIcfDataBuf->Des();
        edit->GetEditorContentForFep( icfDes, aIcfData.iStartPos, readLen ); 
        if ( iFepMan.IsFlagSet(CAknFepManager::EFlagNoMatches) )
            {
            aIcfData.iFlag = EFepICFDataInlineNoMatch;
            }
        aIcfData.iText.Set(icfDes);
        
    	if ( EnableITIOnFSQ() )
    	    {
    	    TInt tailLength = 0;
    	    iFepMan.PtiEngine()->HandleCommandL
    	             ( EPtiCommandGetAutoCompletionTailLength, &tailLength );
    	    if ( tailLength > 0 )
    	        {
    	        aIcfData.iMidPos = tailLength;
    	        }
    	    else
    	        {
    	        aIcfData.iMidPos = 0;
    	        }
    	    }
        else
            {
            aIcfData.iMidPos = 0;
            }

        aIcfData.iCursorVisibility = EFalse;
        aIcfData.iCursorSelVisible = EFalse;
        }
    else
        {
        // update curosr pos
        aIcfData.iCmd = EPeninputICFSetCurSel;
        aIcfData.iStartPos = iCurSelCur.iCursorPos;
        aIcfData.iMidPos = -1;
        aIcfData.iText.Set(KNullDesC);
        if( iFepMan.IsFlagSet(CAknFepManager::EFlagSupressAutoUpdate) && iFepMan.InputMode() == EHangul )
        	{
			aIcfData.iCursorSelVisible = EFalse;
        	}
        }
        
    iInlineStateOn = ( aIcfData.iMidPos == 0 );
	
    return ETrue;
	}   

TBool CAknFepPluginManager::TextInlined( const TDesC& aLastEditorContent, 
                                         const TDesC& aCurrentEditorContent)
	{
	if ( iModeChanged )
		{
		return EFalse;
		}
		
	return ( ExistInlineText()  && 
           aLastEditorContent.Left(iFepMan.UncommittedText().LowerPos()).Compare(
                   aCurrentEditorContent.Left(iFepMan.UncommittedText().LowerPos()) ) == 0 && 
           aLastEditorContent.Mid(iCurSelPre.HigherPos()).Compare(
                   aCurrentEditorContent.Mid(iCurSelCur.iCursorPos) ) == 0 );
	}

TBool CAknFepPluginManager::TextInserted( const TDesC& aLastEditorContent, 
                                          const TDesC& aCurrentEditorContent)
	{
	if ( iModeChanged )
		{
		return EFalse;
		}
	
	return ( iCurSelCur.LowerPos() > iCurSelPre.LowerPos() && 
           aLastEditorContent.Left(iCurSelPre.LowerPos()).Compare(
                 aCurrentEditorContent.Left(iCurSelPre.LowerPos()) ) == 0 && 
           aLastEditorContent.Mid(iCurSelPre.HigherPos()).Compare(
                 aCurrentEditorContent.Mid(iCurSelCur.iCursorPos) ) == 0);	
	}
	
TBool CAknFepPluginManager::TextDeleted( const TDesC& aLastEditorContent, 
                                         const TDesC& aCurrentEditorContent)
	{
	if ( iModeChanged )
		{
		return EFalse;
		}
	
    if ( aLastEditorContent.Length() < aCurrentEditorContent.Length() )
        {
        return EFalse;
        } 
	
	TPtrC curRightContent = aCurrentEditorContent.Mid(iCurSelCur.iCursorPos);

	TInt pos = aLastEditorContent.Length()-curRightContent.Length();

	return ( iCurSelCur.HigherPos() <= iCurSelPre.HigherPos() && 
           aLastEditorContent.Left(iCurSelCur.iCursorPos).Compare(
               aCurrentEditorContent.Left(iCurSelCur.iCursorPos) ) == 0 
           && ( pos >= 0) && aLastEditorContent.Mid(pos).Compare(curRightContent ) == 0	);
	}	
	
TBool CAknFepPluginManager::TextMultiTapped( const TDesC& aLastEditorContent, 
                                             const TDesC& aCurrentEditorContent)
	{
	if ( iModeChanged )
		{
		return EFalse;
		}

	MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
	
	if ( iCurSelCur.iCursorPos > aLastEditorContent.Length() )
	    {
	    return EFalse;
	    }
	return ( iCurSelCur.iCursorPos > 0 && 
        ( iCurSelPre.iCursorPos <= edit->DocumentMaximumLengthForFep() 
              || edit->DocumentMaximumLengthForFep() == 0 ) && 
        aLastEditorContent.Left(iCurSelCur.iCursorPos-1).Compare(
              aCurrentEditorContent.Left(iCurSelCur.iCursorPos-1) ) == 0 && 
        aLastEditorContent.Mid(iCurSelCur.iCursorPos).Compare(
              aCurrentEditorContent.Mid(iCurSelCur.iCursorPos) ) == 0);	
	}	
	
// -----------------------------------------------------------------------------
// CAknFepPluginManager::SetCursorSelectionL
// Set editor cursor selection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::SetCursorSelectionL(const TCursorSelection& aCurSel, TBool aSyncCursor)    
    {
    MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
    if(edit && 
       aCurSel.HigherPos() <= edit->DocumentLengthForFep() && 
       aCurSel.LowerPos() >= 0 )
        {
        if ( ( aCurSel.iCursorPos != iCurSelPre.iCursorPos ) || 
            ( aCurSel.iAnchorPos != iCurSelPre.iAnchorPos ) || aSyncCursor)
            {
            edit->SetCursorSelectionForFepL(aCurSel);
            
            iFepMan.SynCCPSoftKey();
            TCursorSelection curSel; 
            edit->GetCursorSelectionForFep( curSel );

            if(curSel.iCursorPos != aCurSel.iCursorPos || 
               curSel.iAnchorPos != aCurSel.iAnchorPos )
                {
                iCurSelPre.iCursorPos = aCurSel.iCursorPos;
                iCurSelPre.iAnchorPos = aCurSel.iAnchorPos;
                }

            if ( aSyncCursor || 
                curSel.iCursorPos != aCurSel.iCursorPos || 
                curSel.iAnchorPos != aCurSel.iAnchorPos || 
                curSel.iCursorPos != iCurSelPre.iCursorPos || 
                curSel.iAnchorPos != iCurSelPre.iAnchorPos )
                {
			    iCaseMan.UpdateCase(ENullNaviEvent);			    
                SendIcfDataL();
                }
            else
                {
                iCurSelPre.iCursorPos = aCurSel.iCursorPos;
                iCurSelPre.iAnchorPos = aCurSel.iAnchorPos;
                }
            }
        }
    }   
 
HBufC*  CAknFepPluginManager::GetNumberKeymapResource(TInt aCaseMode)
    {
    HBufC* rtn = NULL;
    TRAP_IGNORE( rtn = DoGetNumberKeymapResourceL( aCaseMode ));
    return rtn;
    }
    
HBufC*  CAknFepPluginManager::DoGetNumberKeymapResourceL(TInt aCaseMode)
    {//the code is borrowed from DoLaunchSctAndPctL
    
    HBufC* rtn = NULL;
    
    //first get resource id
    TInt currentEditorSCTResId = iFepMan.GetCurrentEditorSCTResId();
    
    //load from resource
    TResourceReader reader;
    CCoeEnv* env = CCoeEnv::Static();
    env->CreateResourceReaderLC(reader, currentEditorSCTResId);
    reader.ReadInt32();//LONG flag
    reader.ReadTPtrC();//LTEXT title="";
    reader.ReadInt32();//LLINK pages=0;
    reader.ReadInt32();//LLINK buttons=0;

    const TInt count = reader.ReadInt16(); //line count
    for( TInt i = 0; i < count; ++i )
        {
        TInt type = reader.ReadInt16();      
        if( type == EAknCtSpecialCharacterMap )
            {
            reader.ReadTPtrC(); //caption
            reader.ReadInt16(); //id
            reader.ReadInt32(); //flags          
            TInt component_count=reader.ReadInt16();
            for (TInt ii=0;ii<component_count;ii++)
                {
                TInt component_id=reader.ReadInt16();
                if(component_id == aCaseMode)
                    {
                    rtn = reader.ReadHBufCL();
                    break;
                    }
                else
                    {
                    reader.ReadTPtrC();
                    }
                }
  
            break;
            }
        else
            {
            //ignore line
            CEikCaptionedControl* line=new(ELeave) CEikCaptionedControl;
            CleanupStack::PushL(line);
            SEikControlInfo controlInfo=EikControlFactory::CreateByTypeL(type);
            if( controlInfo.iControl )
                {
                line->iControl = controlInfo.iControl;
                line->ConstructFromResourceL(reader);
                CleanupStack::PopAndDestroy(line); // line
                }
            else
                {
                User::Leave(KErrNotSupported);
                }
            }
        }

    CleanupStack::PopAndDestroy(); //reader
    return rtn;
    }
                             
// -----------------------------------------------------------------------------
// CAknFepPluginManager::InitializePluginInputL
// Initialize plugin UI
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::InitializePluginInputL(TInt aOpenMode, TInt aSuggestRange,
                                                  TBool aCleanContent)
    {
    if ( iFepMan.FepManState()!= EAknFepStateUIActive || iClosePluginInputMode )      
        {
        //Set hardware key board mode
        iFepMan.TryCloseUiL();
        iClosePluginInputMode = EFalse;
        }
    
    UpdatePredicState();
  
    NotifyLayoutL( aOpenMode, aSuggestRange, aCleanContent );
    
//    iPenInputServer.UpdateAppInfo(KNullDesC, EAppIndicatorMsg);  
    
    iPluginInputMode =  (TPluginInputMode)iLangMan.CurrentImePlugin()->CurrentMode(); 
    
    // Not only for data query, but also for other pop dialogs contain editor.
    //iPenInputServer.SetDataQueryPopped(IsDisplayDataQuery());
    iPenInputServer.SetDataQueryPopped(CCoeEnv::Static()->AppUi()->IsDisplayingDialog() &&
											CCoeEnv::Static()->AppUi()->TopFocusedControl());  

    //adjust VKB window if data query dialog is displaying
    if(IsDisplayDataQuery())
        {
        RDrawableWindow* focusWin = 
                       CCoeEnv::Static()->AppUi()->TopFocusedControl()->DrawableWindow();        
        if(!iLastDataQueryWin
           || iLastDataQueryWin != focusWin)
            {
            iDataQueryAlreadyShow = EFalse;
            iLastDataQueryWin = focusWin;
            }
        }
    if(IsDisplayDataQuery() && //data query
       !iDataQueryAlreadyShow &&  //only adjust once
       PluginInputMode() == EPluginInputModeVkb) //only vkb mode 
        {
        RepositionDataQuery(EFalse);
        }

    if ( !NotifyInGlobalNoteEditorL() )
    	{
        iFocuschangedForSpellEditor = EFalse; // restore the flag
    	iCurrentPluginInputFepUI->ActivateUI(); 
        iNeedFetchDimState = ETrue;
    	}
    
    iFepMan.UpdateIndicators();
    iCurrentPluginInputFepUI->SetNextFepUI(
        iLangMan.GetFepUI(iFepMan.InputMode(), ENoneWidthChar, 
        iFepMan.IsPredictive(iFepMan.InputMode())) );    
    //SyncFepAwareText( EPluginSyncUpdatedText ); //sync again
    iLastSubmitCount = 0;
    }

// -----------------------------------------------------------------------------
// CAknFepPluginManager::LaunchPenInputMenuL
// Launch plugin menu.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::LaunchPenInputMenuL(TInt aResourceId, TBool aRemeber)
    {
	// when launch option menu, dim state changes, 
	// so need fetch the state next time.
	iNeedFetchDimState = ETrue;
	
    TInt previousModes = iCurPermitModes;
    TBool previousSCT  = isLanuchSCT;
    TInt inputmode = PluginInputMode();
    
    if ( R_AVKON_PENINPUT_OPTION_MENU_BAR == aResourceId &&
           ( inputmode == EPluginInputModeItut ||
             inputmode == EPluginInputModeVkb ||
             inputmode == EPluginInputModeFSQ ||
             inputmode == EPluginInputModePortraitFSQ ) )
        {
        isLanuchSCT = EFalse;
        iCurPermitModes = iFepMan.PermittedModes(); 
        isLanuchSCT = iFepMan.IsAbleToLaunchSCT();  
        }
    else
        {
        iCurPermitModes = 0;    
        }
        
    if (aRemeber)
        {
        iFepMan.RemeberEditorState();   
        }
    else
        {
        iCurPermitModes = previousModes;
        isLanuchSCT = previousSCT;
        }
    
    iPenInputMenu->ShowL(aResourceId);
    }
 
// ---------------------------------------------------------------------------
// CAknFepPluginManager::LaunchPenInputLanguageSelectionL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::LaunchPenInputLanguageSelectionL( TBool aLaunchedByTouchWin )
    {
    //record langauge
    TInt oldLang = iSharedData.InputTextLanguage();
    CPenInputGSInterface*  setting = CPenInputGSInterface::NewL();
    CleanupStack::PushL(setting);
    iFepMan.SetCancelPopupInQwerty( aLaunchedByTouchWin );
    setting->ShowInputLanguagePageL();
    iFepMan.SetCancelPopupInQwerty( EFalse );
    CleanupStack::PopAndDestroy(setting); 
    TInt inputLanguage = iSharedData.InputTextLanguage();
    if( oldLang != inputLanguage)
        {
        iSharedData.SetInputTextLanguage(inputLanguage);
        iFepMan.ChangeInputLanguageL(inputLanguage);
        iFepMan.SetFlag( CAknFepManager::EFlagNewSharedDataInputLanguage 
            | CAknFepManager::EFlagNewSharedDataInputMode );

        }
    }        

// ---------------------------------------------------------------------------
// CAknFepPluginManager::LaunchPenInputRecognitionWithDictionarySelectionL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::LaunchPenInputRecognitionWithDictionarySelectionL()
    {
    //record langauge
    CPenInputGSInterface*  setting = CPenInputGSInterface::NewL();
    CleanupStack::PushL(setting);
    setting->ShowRecognitionWithDictionaryL();
    CleanupStack::PopAndDestroy(setting); 
    TInt inputLanguage = iSharedData.InputTextLanguage();
    }  

// ---------------------------------------------------------------------------
// CAknFepPluginManager::LaunchPenInputWritingSpeedSelectionL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::LaunchPenInputWritingSpeedSelectionL()
    {
    //record langauge
    CPenInputGSInterface*  setting = CPenInputGSInterface::NewL();
    CleanupStack::PushL(setting);
    setting->ShowWritingSpeedPageL();
    CleanupStack::PopAndDestroy(setting); 
    TInt inputLanguage = iSharedData.InputTextLanguage();
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::LaunchPenInputGuideLineSelectionL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::LaunchPenInputGuidingLineSelectionL()
    {
    //record langauge
    CPenInputGSInterface*  setting = CPenInputGSInterface::NewL();
    CleanupStack::PushL(setting);
    setting->ShowGuideLinePageL();
    CleanupStack::PopAndDestroy(setting); 
    TInt inputLanguage = iSharedData.InputTextLanguage();
    }
// ---------------------------------------------------------------------------
// CAknFepPluginManager::LaunchPenInputSettingL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::LaunchPenInputSettingL()
    {
    //record langauge
    TInt oldLang = iSharedData.InputTextLanguage();
    if ( !iGsInterface )
        {
        iGsInterface = CPenInputGSInterface::NewL();
        }    
    iGsInterface->ShowMainViewL();
    TInt inputLanguage = iSharedData.InputTextLanguage();
    if( oldLang != inputLanguage)
        {
        iFepMan.ChangeInputLanguageL(inputLanguage);
        }

    }     

// -----------------------------------------------------------------------------
// CAknFepManager::ParseInputContextContent
// Parse editor text passed to plugin ICF
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::ParseInputContextContent(TDes& aBuffer, TInt aCurrent,
                                              TInt& aFrom, TInt& aTo)
    {
    TInt maxLen = aBuffer.Length();
    TInt i;
    //find from
    for( i = aCurrent - 1; i >=0; i--)
        {
        if( aBuffer[i] == KVkbTabulator)
            {
            // tabulator found, replace it with a space
            aBuffer.Replace(i, 1, KVkbTabChar);
            }
        else if(aBuffer[i] == KVkbParagraphSeparator)
            {
            break;
            }
        }
        
    aFrom = i + 1;
    
    for(i = aCurrent; i < maxLen; i++)
        {
        if( aBuffer[i] == KVkbTabulator)
            {
            // tabulator found, replace it with a space
            aBuffer.Replace(i, 1, KVkbTabChar);
            }
        else if(aBuffer[i] == KVkbParagraphSeparator)
            {
            break;
            }
        }
        
    aTo = i;
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::SubmitUiPluginTextL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::SubmitUiPluginTextL(const TDesC& aData, 
											   TSimulateEventMode aForcedMode)
    {    
    iDisableSyncText = ETrue;
    // a flag to decide if to simulate space or enter key event in number only editor
    // if is asked, simulate these keys anyway.
    TBool disableSpaceEnterFlag = EFalse ;
    
    if ( iFepMan.PermittedModes() == EAknEditorNumericInputMode )
        {
        CAknEdwinState* editorState = iFepMan.EditorState();
        
        if ( !editorState )
            {
            disableSpaceEnterFlag = !iFepMan.IsMfneEditor();
            }
        else
            {
            disableSpaceEnterFlag = 
                     !(editorState->Flags() & EAknEditorFlagDeliverVirtualKeyEventsToApplication);
            }
        }

    CCoeEnv* env = CCoeEnv::Static();
    MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor(); 
        /*
        TBool noSimulate = !iFepMan.IsMfneEditor() && iCurSelPre.Length() > 0 &&
            ( PluginInputMode() == EPluginInputModeItut || PluginInputMode() == EPluginInputModeFSQ );
        */   
        for(int i = 0; i < aData.Length(); ++i)
            {
            TKeyEvent keyEvent = {aData[i], EStdKeyNull, 0, 0};
            
            /*
            if ( noSimulate )
                {
                if (keyEvent.iCode == EKeyLeftArrow )
                    {
                    if(iCurSelPre.iCursorPos > 0)
                        {
                        TInt pos = iCurSelPre.iCursorPos - 1;
                        edit->SetCursorSelectionForFepL
                            ( TCursorSelection( pos, iCurSelPre.iAnchorPos ) );
                        }
                    else  if(iCurSelPre.iCursorPos == 0)
                        {
                        edit->SetCursorSelectionForFepL
                            ( TCursorSelection( iCurSelPre.iCursorPos, iCurSelPre.iAnchorPos ) );
                        }
                    SendIcfDataL();
                    continue;
                    }
                else if (keyEvent.iCode == EKeyRightArrow )
                    {                   
                    if(iCurSelPre.iCursorPos < edit->DocumentLengthForFep())
                        {
                        TInt pos = iCurSelPre.iCursorPos + 1;
                        edit->SetCursorSelectionForFepL
                            ( TCursorSelection( pos, iCurSelPre.iAnchorPos ) );
                        }
                    else if(iCurSelPre.iCursorPos == edit->DocumentLengthForFep())
                        {
                        edit->SetCursorSelectionForFepL
                            ( TCursorSelection( iCurSelPre.iCursorPos, iCurSelPre.iAnchorPos ) );
                        }  
                    SendIcfDataL();
                    continue;
                    }    
                }
            */        
            
		    if (keyEvent.iCode == EKeyLeftArrow && iHasSWEventCap)
                {
                TRawEvent eventDown; 
                eventDown.Set( TRawEvent::EKeyDown, EStdKeyLeftArrow); 
                eventDown.SetTip( ETrue );
                static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->DisableNextKeySound( EStdKeyLeftArrow ); 
                CCoeEnv::Static()->WsSession().SimulateRawEvent( eventDown ); 

                TRawEvent eventUp; 
                eventUp.Set( TRawEvent::EKeyUp, 14); 
                eventUp.SetTip( ETrue );
                CCoeEnv::Static()->WsSession().SimulateRawEvent( eventUp ); 
                }
                
		    else if (keyEvent.iCode == EKeyRightArrow && iHasSWEventCap)
                {
                TRawEvent eventDown; 
                eventDown.Set( TRawEvent::EKeyDown, EStdKeyRightArrow); 
                eventDown.SetTip( ETrue );
                static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->DisableNextKeySound( EStdKeyRightArrow ); 
                CCoeEnv::Static()->WsSession().SimulateRawEvent( eventDown ); 

                TRawEvent eventUp; 
                eventUp.Set( TRawEvent::EKeyUp, EStdKeyRightArrow); 
                eventUp.SetTip( ETrue );
                CCoeEnv::Static()->WsSession().SimulateRawEvent( eventUp ); 
                }
            
            
            else if ( keyEvent.iCode >= EKeyApplication27 && !( (iLangMan.InputLanguage() == ELangFarsi) && EnableITIOnFSQ())
                    || (iFepMan.IsFeatureSupportedJapanese()
                         && iFepMan.InputMode() == EHiraganaKanji
                         && edit
                         && (EPtiKey0 <= keyEvent.iCode && keyEvent.iCode <= EPtiKey9)
                       )
                    )
                {
                TBuf<1> buf;
                buf.Append( TChar( aData[i] ) );
                iFepMan.SubmitInlineTextL( buf );
                }
            else
                {
                if ( ( disableSpaceEnterFlag && 
                    ( keyEvent.iCode == EKeySpace || keyEvent.iCode == EKeyEnter ) ) || 
                    ( edit && edit->DocumentLengthForFep() <= 0  && 
                    ( keyEvent.iCode == EKeyDelete || keyEvent.iCode == EKeyBackspace ) ) )
                    {
                    iLastSubmitCount--;
                    continue;
                    }
                if ( keyEvent.iCode == EKeyTab )
                    {
                    iLastSubmitCount--;
                    }
                else if (keyEvent.iCode == EKeySpace)
                    {
                    keyEvent.iScanCode = EStdKeySpace;
                    }
                else if (keyEvent.iCode == EKeyDelete)
                    {
                    keyEvent.iScanCode = EStdKeyDelete;
                    if (iSharedData.ClearDirection() == EClearDirectionLeft)
                        {
                        keyEvent.iCode = EKeyBackspace;
                        keyEvent.iScanCode = EStdKeyBackspace;
                        }
                    }
                else if (keyEvent.iCode == EKeyBackspace)
                    {
                    keyEvent.iScanCode = EStdKeyBackspace;
                    }
                else if (keyEvent.iCode == EKeyEnter)
                    {
                    keyEvent.iScanCode = EStdKeyEnter;
                    }
                else if ( EnableITIOnFSQ() )
                    {
                    // For addtion of ITI features on FSQ
                    // If FSQ is opened with ITI enabled,
                    // Must get scan code from keymapping, EStdKeyNull won't be handled by ITI engine. 
                    keyEvent.iScanCode = GetScanCodeFromHwKeymapping( aData[i] );
                    }
                // For converter
                // If inputting through VKB or HWR, the event can be done only when aType is equal to EEventKey.
                // Besides, only when inputting the dot in VKB or HWR, aKeyEvent.iScanCode is equal to decimalChar.
                CAknEdwinState* editorState = iFepMan.EditorState();
                TLocale locale;
                if(editorState && 
                  (editorState->NumericKeymap() == EAknEditorConverterNumberModeKeymap ||
                    editorState->NumericKeymap() == EAknEditorCalculatorNumberModeKeymap) &&
                    keyEvent.iCode == locale.DecimalSeparator() )
                    {
                    keyEvent.iScanCode = locale.DecimalSeparator();
                    }

                // The keycode is a non-standard keyboard character
                keyEvent.iModifiers |= EModifierSpecial;
                
                
                if (keyEvent.iScanCode == EStdKeyBackspace && 
                	iHasSWEventCap &&
                	aForcedMode != EKeyEvent )
                    {
                    TRawEvent eventDown; 
                    eventDown.Set( TRawEvent::EKeyDown, EStdKeyBackspace); 
                    eventDown.SetTip( ETrue );
                    static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->DisableNextKeySound( 
                                                                                EStdKeyBackspace ); 
                    CCoeEnv::Static()->WsSession().SimulateRawEvent( eventDown ); 
                    
                    TRawEvent eventUp; 
                    eventUp.Set( TRawEvent::EKeyUp, EStdKeyBackspace); 
                    eventUp.SetTip( ETrue );
                    CCoeEnv::Static()->WsSession().SimulateRawEvent( eventUp );
                    
                    break;
                    }
                else if ( keyEvent.iScanCode == EStdKeyEnter && iHasSWEventCap )
                    {
                    TRawEvent eventDown; 
                    eventDown.Set( TRawEvent::EKeyDown, EStdKeyEnter ); 
                    iAvkonAppUi->DisableNextKeySound( EStdKeyEnter ); 
                    CCoeEnv::Static()->WsSession().SimulateRawEvent( eventDown ); 
                    
                    TRawEvent eventUp; 
                    eventUp.Set( TRawEvent::EKeyUp, EStdKeyEnter ); 
                    CCoeEnv::Static()->WsSession().SimulateRawEvent( eventUp );                 	
                    }
                else if ( EnableITIOnFSQ() )
                    {
                    // For addition of ITI features on FSQ.
                    // If FSQ is opened with ITI enabled, ensure keycatcher to handle key events
                    // else, remain the old logic.
                    // But, simulating raw key events is needed, as raw key down + raw key up is a complete key process, 
                    // then EEventDown, EEventKey, EEventUp will be generated, each event may be used differently.
                    if ( IsITISupportedKey( keyEvent ) )
                        {                            
                        // check if need to change current text case
                        TPtiTextCase textCase = 
                        CaseForMappedCharacter( TChar( keyEvent.iCode ) );                                                        
						if ( IsNeedToChangeTextCase( textCase ) )
                            {
                            iFepMan.PtiEngine()->SetCase( textCase );
                            }
                                                                       
						env->SimulateKeyEventL(keyEvent, EEventKeyDown);
						env->SimulateKeyEventL(keyEvent, EEventKey);
						env->SimulateKeyEventL(keyEvent, EEventKeyUp);
                        }
                    else if ( keyEvent.iScanCode == EStdKeySpace )
                        {
                        TRawEvent eventDown; 
                        eventDown.Set( TRawEvent::EKeyDown, keyEvent.iScanCode ); 
                        iAvkonAppUi->DisableNextKeySound( keyEvent.iScanCode ); 
                        CCoeEnv::Static()->WsSession().SimulateRawEvent( eventDown ); 
                        
                        TRawEvent eventUp; 
                        eventUp.Set( TRawEvent::EKeyUp, keyEvent.iScanCode ); 
                        CCoeEnv::Static()->WsSession().SimulateRawEvent( eventUp );                        
                        }
                    else
                        {
                        // When FSQ opened with ITI features enabled, 
                        // but key is not supported by ITI
                        // commit inline if necessary
                        if ( iFepMan.UncommittedText().Length() > 0 )
                            {                                
                            iFepMan.CommitInlineEditL();
                            iFepMan.PtiEngine()->CommitCurrentWord();
                            iFepMan.TryCloseUiL();
                            TKeyEvent keyEventUpdateCase = {EKeyF19, EStdKeyNull, 0, 0};
                            env->SimulateKeyEventL(keyEventUpdateCase, EEventKey);
                            }                            
                        // set flag to ensure not to be handled by key catcher.
                        iFepMan.SetFlag(CAknFepManager::EFlagPassNextKey);
                        env->SimulateKeyEventL(keyEvent, EEventKey);
                		iFepMan.ClearFlag(CAknFepManager::EFlagPassNextKey);
                        }
                    }
                else
                	{
                	// keep the old logic is not affected
                    iFepMan.SetFlag(CAknFepManager::EFlagPassNextKey);
                	env->SimulateKeyEventL(keyEvent, EEventKey);
                	iFepMan.ClearFlag(CAknFepManager::EFlagPassNextKey);
                	}                    
                }
            }  
    
    //fix defect             
    if( edit )
        {
        env->ForEachFepObserverCall(FepObserverHandleCompletionOfTransactionL);
        }
    iSyncWithPluginRange = ETrue;
    iCaseMan.UpdateCase(ENullNaviEvent);
    iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCaseMode, iCaseMan.CurrentCase());
    iSyncWithPluginRange = EFalse;
    
    iDisableSyncText = EFalse;
    if( iPendingSyncReq )
        {
        SendIcfDataL( EPluginSyncAll );
        }
    }    

// -----------------------------------------------------------------------------
// CAknFepPluginManager::OnPenInputServerKeyEventL
// Handle key event sent from pen input server
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::OnPenInputServerKeyEventL(const TDesC& aData)
    {	
    if ( !EnableITIOnFSQ() )
    	{
        iFepMan.PtiEngine()->CancelTimerActivity();
    	}    
    
    if( aData.Length() <= 0 )
        {
        return;
        }
 
    if ( iFepMan.IsFepAwareTextEditor() )
        {
        CAknEdwinState* editorState = iFepMan.EditorState();
        TBool sendFlag =  editorState && 
            (editorState->Flags() & EAknEditorFlagDeliverVirtualKeyEventsToApplication);
            
        TInt maxEdSize = iFepMan.FepAwareTextEditor()->
            DocumentMaximumLengthForFep();
        if ( maxEdSize == 0  )
            {
            iLastSubmitCount = aData.Length();
            SubmitUiPluginTextL(aData);
            }
        else
            {
            TCursorSelection currentSelection;
            iFepMan.FepAwareTextEditor()->GetCursorSelectionForFep(currentSelection);
            TInt freeSpaceOfEditor = maxEdSize - 
                iFepMan.FepAwareTextEditor()->DocumentLengthForFep() +
                currentSelection.Length(); 
                
            if ( freeSpaceOfEditor >= 0 )
                {
                if ( ( aData.Length() <= freeSpaceOfEditor ) ||
                    ( ( aData.Length()-1 == freeSpaceOfEditor ) && 
                      (( aData[aData.Length()-1] == EKeyBackspace ) ||
                      	 (aData[aData.Length()-1] == EKeyEnter )  ||
                         (aData[aData.Length()-1] == EKeyLeftArrow )  ||
                         (aData[aData.Length()-1] == EKeyRightArrow ) ) ))
                    {
                    iLastSubmitCount = aData.Length();
                    }
                else
                    {
                    iLastSubmitCount = freeSpaceOfEditor;
                    }
                    
                if ( sendFlag )
                    {
                    SubmitUiPluginTextL(aData);
                    }
                else
                    {
                    TPtrC charbuf = aData.Left(iLastSubmitCount);
                    SubmitUiPluginTextL(charbuf);
                    }
                    
                if (freeSpaceOfEditor == 0 || iLastSubmitCount == freeSpaceOfEditor)
                	{
                	HasNoFreeSpace();	
                	}
                    
                }
            else
                {
                iLastSubmitCount = 0;
                if ( sendFlag )
                    {
                    SubmitUiPluginTextL(aData);
                    }
                }
            }
        }
    else
        {
        SubmitUiPluginTextL(aData);
        }
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::SyncIndicatorWithPluginRangeL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::SyncIndicatorWithPluginRangeL()
    {
    iSyncWithPluginRange = ETrue;
    iPluginNativeRange = EFalse;
    TInt newMode = iFepMan.InputMode();
    if( iPluginPrimaryRange == ERangeNative )
        {
        if( !iFepMan.EditorState() )
            {
            return;
            }
        TInt defaultInputMode = iFepMan.EditorState()->DefaultInputMode();
        if ( iFepMan.IsChineseInputLanguage() )
           {
           defaultInputMode = iFepMan.InputMode();
           }    
        const TInt inputLanguage = iSharedData.InputTextLanguage();
        
        switch(inputLanguage)
            {
            case ELangPrcChinese:
                if( defaultInputMode != EPinyin &&
                    defaultInputMode != EStroke )
                    {
                    if ( iFepMan.LastChineseInputMode() )
                        {
                        newMode = iFepMan.LastChineseInputMode();
                        }
                    else
                        {
                    newMode = EPinyin;
                        }                    
                    }
                else
                    {
                    newMode = defaultInputMode;
                    }
                break;
            case ELangTaiwanChinese:
                if( defaultInputMode != EZhuyin &&
                    defaultInputMode != EStroke )
                    {
                    if ( iFepMan.LastChineseInputMode() )
                        {
                        newMode = iFepMan.LastChineseInputMode();
                        }
                    else
                        {
                    newMode = EZhuyin;
                        }                    
                    }
                else
                    {
                    newMode = defaultInputMode;
                    }
                break;
            case ELangHongKongChinese:
                if( defaultInputMode != ECangJie &&
                    defaultInputMode != EStroke )
                    {
                    if ( iFepMan.LastChineseInputMode() )
                        {
                        newMode = iFepMan.LastChineseInputMode();
                        }
                    else
                        {
                    newMode = EStroke;
                        }
                    }
                else
                    {
                    newMode = defaultInputMode;
                    }
                break;
            case ELangJapanese:
                {
                newMode = defaultInputMode;
                iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputJapaneseGetModeOfSubRange, (TInt)(&newMode));
                }
                break;
            case ELangHindi:
                {
                newMode = EHindi;
                }
                break;
            case ELangKorean:
                {
                newMode = EHangul;
                }
                break;
            default:
                newMode = ELatin;
                iPluginNativeRange = ETrue;
                break;
            }
        }
    else if( iPluginPrimaryRange == ERangeEnglish ||
             iPluginPrimaryRange == ERangeAccent )
        {
        newMode = ELatin;
        }
    else if( iPluginPrimaryRange == ERangeNumber )
        {
        newMode = ENumber;
        }
    else if ( iPluginPrimaryRange == ERangeNativeNumber )
        {
        newMode = ENativeNumber;
        iPluginNativeRange = ETrue;
        }
    //else do NOTHING but keep current for other ranges
    
    if( newMode != iFepMan.InputMode() || 
        ( newMode == ELatin || newMode == ENumber ) )
        {
        iFepMan.TryChangeModeL(newMode);
        UpdatePredicState();
        //iCaseMan.SetCurrentCase(iCaseMan.CurrentCase());
        iCaseMan.UpdateCase(ENullNaviEvent);
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCaseMode, 
                                                 iCaseMan.CurrentCase());
        }
    
    // Addition for ITI features on FSQ    
    if ( newMode == ELatin && IsSupportITIOnFSQ() )
        {
        // When change range back to Latin
        // 1. Config fep state again.        
        SetItiStateL();
        // 2. Restore predict state for FSQ.
        RestorePredictStateL();
        }    
    else
    	{
    	// 1. Restore predict state for FSQ.
    	ResetItiStateL();
    	// 2. Update predict state.
    	UpdatePredicState();
    	}
    //Set correct fep ui	
    if(IsSupportITIOnFSQ())
        {
        iCurrentPluginInputFepUI->SetNextFepUI(
            iLangMan.GetFepUI(iFepMan.InputMode(), ENoneWidthChar,
                iFepMan.IsPredictive(iFepMan.InputMode())) );  		
        }
    iSyncWithPluginRange = EFalse;
    }        

// ---------------------------------------------------------------------------
// CAknFepPluginManager::GetCurSuggestMode
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
TBool CAknFepPluginManager::GetCurSuggestMode( TPluginInputMode& aSuggestMode )
    {
    TInt fepDisableLayoouts = 0;
    TInt disableLayouts = iPenInputServer.DisabledLayout();
    // Get the current phone orientation.
    TPixelsTwipsAndRotation size; 
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
    TBool landscape = size.iPixelSize.iWidth > size.iPixelSize.iHeight;

    switch(aSuggestMode)
        {
		case EPluginInputModeFingerHwr:
        case EPluginInputModeFSc:
        case EPluginInputModeHwr:
			{
		    if( iFepMan.IsSupportsSecretText() || 
		    	(( ( aSuggestMode == EPluginInputModeFSc || 
		    		aSuggestMode == EPluginInputModeFingerHwr) &&
		    	  !(iSharedData.InputTextLanguage() == ELangPrcChinese ||
		    	          iSharedData.InputTextLanguage() == ELangTaiwanChinese ||
		    	          iSharedData.InputTextLanguage() == ELangHongKongChinese)) &&
		    	  ((aSuggestMode == EPluginInputModeFingerHwr && 
		    	          iSharedData.InputTextLanguage() != ELangArabic))))   
				{
                // If full screen portrait QWERTY feature is enabled, use it 
                // instead of ITU-T.
                aSuggestMode = iSharedData.PluginPortraitInputMode();                        

                // If portrait FSQ or ITU-T is disabled, use landscape FSQ.
                // If they are disabled, most likely the orientation is landscape. 
				if ((disableLayouts & aSuggestMode))
					{
					aSuggestMode = EPluginInputModeFSQ;
					}
				fepDisableLayoouts |= EPluginInputModeHwr | EPluginInputModeFSc |
				                      EPluginInputModeFingerHwr;
				}
			}
			break;
        case EPluginInputModeVkb:
            {
            // When in Capacitive hardware, there should be no MiniVKB.
            // So we use FSQ for Landscape mode and ITUT for Portrait instead.
            if( FeatureManager::FeatureSupported( KFeatureIdFfCapacitiveDisplay ))
                {
                if ( landscape ) // LandScape
                    {
                    aSuggestMode = EPluginInputModeFSQ;
                    }
                else // Portrait
                    {
                    // Use the last used portrait input mode.
                    aSuggestMode = iSharedData.PluginPortraitInputMode();
                    }
                }
            }
            break;
        case EPluginInputModeFSQ:
            {
            if ((disableLayouts & aSuggestMode))
                {
                // Use the last used portrait input mode.
                aSuggestMode = iSharedData.PluginPortraitInputMode();
                }
            }
            break;
        // If ITU-T or portrait FSQ are disabled, use landscape FSQ.    
        case EPluginInputModeItut:
        case EPluginInputModePortraitFSQ:	
            {
            if ((disableLayouts & aSuggestMode))
                {
                aSuggestMode = EPluginInputModeFSQ;
                }
            }
            break;
        default:
			{
            // Landscape FSQ is used in landscape orientation.
            if ( landscape )
                {
                aSuggestMode = EPluginInputModeFSQ; 
                }
            // If the current orientation is portrait, use the last used portrait input mode.
            else
                {
                aSuggestMode = iSharedData.PluginPortraitInputMode();
               
                // If the suggested mode is EPluginInputModeNone or disabled, use 
                // EPluginInputModeFSQ.
                // Note: If the suggested mode is landscape FSQ and it it also disabled,
                // still we use landscape FSQ because there is no choice.
                if ( ( aSuggestMode == EPluginInputModeNone ) ||
                     ( disableLayouts & aSuggestMode ) )
                    {
                    aSuggestMode = EPluginInputModeFSQ; 
                    }
                }
            }
            break;
        }

     if (disableLayouts == 0)
        {
        return ETrue;   
        }
        
     disableLayouts |= fepDisableLayoouts;
     
     // disable all
     if (disableLayouts ==  EPluginInputModeAll)
        {
        return EFalse;     
        }         
     
     if (!(disableLayouts & aSuggestMode))
        {
        return ETrue;   
        }
        
     TInt testMode = EPluginInputModeHwr;
     while ( testMode < EPluginInputModeAll )
         {
         if (!(testMode & disableLayouts))
             {
             break;
             }
         testMode<<=1;
         }
         
     if (((testMode == EPluginInputModeFSc ||
           testMode == EPluginInputModeFingerHwr) &&
         !(iSharedData.InputTextLanguage() == ELangPrcChinese ||
           iSharedData.InputTextLanguage() == ELangTaiwanChinese ||
           iSharedData.InputTextLanguage() == ELangHongKongChinese)) &&
         ((testMode == EPluginInputModeFingerHwr && 
           iSharedData.InputTextLanguage() != ELangArabic)))
         {
         return EFalse;   
         }
    
    aSuggestMode = TPluginInputMode(testMode);
        
    return ETrue;     
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::ConnectServer
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
TBool CAknFepPluginManager::ConnectServer()
    {
    if(!iPenInputSvrConnected)
        {
        if(!iConnectAo->IsActive())
            {
            iConnectAo->RequestConnect();
            iPenInputServer.ConnectAsyc(iConnectAo->RequestStatus());
            }
        // vv 
        return EFalse;
        }

    iPenInputServer.SetForeground(iOpenPenUiFlag);
    
    return ETrue;
    }
    
// ---------------------------------------------------------------------------
// CAknFepPluginManager::NotifyLayoutL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::NotifyLayoutL(TInt aOpenMode, TInt aSuggestRange, 
                                         TBool aCleanContent)
    {
    TBool isSecret = EFalse;
    if ( iFepMan.IsSupportsSecretText() )
        {
        isSecret = ETrue;
        } 
 
    CAknEdwinState* editorState = iFepMan.EditorState();// still stay here
    
    // for MFNE Editor
    if( !editorState && iFepMan.InputMode() == ENumber )
        {
        if( AknTextUtils::NumericEditorDigitType() == EDigitTypeEasternArabicIndic || 
            AknTextUtils::NumericEditorDigitType() == EDigitTypeDevanagari ||
            AknTextUtils::NumericEditorDigitType() == EDigitTypeArabicIndic )
            {
            iFepMan.TryChangeModeL( ENativeNumber );
            }
        else if ( AknTextUtils::NumericEditorDigitType() == EDigitTypeWestern )
            {
            iFepMan.TryChangeModeL( ENumber );
            }
        }

    if(editorState && (editorState->Flags() & EAknEditorFlagNumberGrouping))
        {
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputIsNumberGrouping, 
                                              ETrue );
        }
    else
        {
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputIsNumberGrouping, 
                                              EFalse );
        }    
                                                                                  
    if ( PluginInputMode() == EPluginInputModeItut )
        {
        // The preview bubble item will be shown in option menu 
        // when current language is Chinese
        if ( iFepMan.InputLanguageCapabilities().iInputLanguageCode == ELangPrcChinese ||
             iFepMan.InputLanguageCapabilities().iInputLanguageCode == ELangTaiwanChinese ||
             iFepMan.InputLanguageCapabilities().iInputLanguageCode == ELangHongKongChinese )
            {
            iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCharacterPreview, 
                   iFepMan.FepShowVkbPreviewStatus());                 
            }
        
        if ( iSharedData.InputTextLanguage() == ELangThai 
            && iFepMan.InputMode() == ENativeNumber  )
            {
            iFepMan.TryChangeModeL( ENumber );
            }

        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputPermittedRange,
                                         iFepMan.PermittedModes());
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputEditorState, 
                                             reinterpret_cast<TInt>( editorState ));
        NotifyLayoutKeymappingL();
        
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerInputMode,
                                                 iFepMan.InputMode());
        
        TFepSymbolOfHardwareOne symbolInfo = iFepMan.SymbolInfoOfHardKey1();
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputSymbolOfHardKeyOne, reinterpret_cast<TInt>(&symbolInfo));

        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCaseMode, 
                                       iCaseMan.CurrentCase());
        
        TBool enableIndicatorBtn = iFepMan.GetPermittedEditorMenu(ETrue) > 0? ETrue : EFalse;                                
        if (IsDimed() && iResourceChange)
            {
            enableIndicatorBtn = iEnableIndicatorBtn;   
            }
        else
            {
            iEnableIndicatorBtn = enableIndicatorBtn;    
            }
        

        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputEnableIndicatorButton,
                                                 enableIndicatorBtn); 
        
        if (!iSpellOn)
              {
            iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerSpelling, 0);
              SetPromptText( aCleanContent );     
              }
        else
            {
			SetITUTSpellingStateL(ETrue);
            iIndicatorImgID = 0;
            iIndicatorTextID = 0;
            }    
        UpdateITUTIndicator();       
        }
    else
        {
        HideSpellEditor();
        if ( PluginInputMode() == EPluginInputModeFSQ ||
             PluginInputMode() == EPluginInputModeVkb ||
             PluginInputMode() == EPluginInputModeFingerHwr ||
             PluginInputMode() == EPluginInputModePortraitFSQ )
            {
            iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCharacterPreview, 
                   iFepMan.FepShowVkbPreviewStatus());                 
            }
      
        // for Japanese peninput setting
        if (iFepMan.IsFeatureSupportedJapanese())
            {
            iFepMan.NotifyJapaneseSetting();
            }
 
 		NotifyLayoutKeymappingL();

        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputPermittedRange, 
                                                 iFepMan.PermittedModes());        
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputRange, iFepMan.InputMode());

        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputEditorState, 
                                                 reinterpret_cast<TInt>( editorState ));


		if(aSuggestRange != ERangeInvalid)
			{
			iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputSuggestRange, aSuggestRange);
			}
        
        //set allow change case
        TBool allowChangeCase = editorState && iCaseMan.IsAbleChangeCase();
        //disable/enable case change
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputAllowChangeCase, allowChangeCase);
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputPermittedCase, 
                                                 iCaseMan.PluginPermittedCases());
        }
    // moved    
/*    iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputEditorState, 
                                             reinterpret_cast<TInt>( editorState ));*/
                                             
    if ( PluginInputMode() != EPluginInputModeItut )
        {
         //set current case
        iCaseMan.SetCurrentCase(iCaseMan.CurrentCase());
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCaseMode, 
                                           iCaseMan.CurrentCase());
        if ( PluginInputMode() == EPluginInputModeFSQ ||
             PluginInputMode() == EPluginInputModeFingerHwr ||
             PluginInputMode() == EPluginInputModePortraitFSQ )
            {
            SetPromptText( aCleanContent );
            }
        }
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputEnableSettingBtn, 
                                                  IsEnableSettingBtn() );
    
    if(!IsDimed())
        {
        // remember previous password state
        iIsPassWord = isSecret;
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputIsSecretText, 
                                                  isSecret );
        }
    else
        {
        // Set previous password state when screen is dimed
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputIsSecretText, 
												  iIsPassWord );	
        }
			
    TBool enableArrowBtn = IsDimArrowKeys();

    if (IsDimed() && iResourceChange)
        {
        enableArrowBtn = iEnableArrowBtn;   
        }
    else
        {
        iEnableArrowBtn = enableArrowBtn;    
        }                                               
                                              
                                              
    iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputDimArrowKeys, 
                                              enableArrowBtn);    
    if ( PluginInputMode() == EPluginInputModeFSQ ||
         PluginInputMode() == EPluginInputModePortraitFSQ )
    	{
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputDimEnterKey, 
        		                                  iFepMan.IsFindPaneEditor() );
    	}

    SetIcfAlignment();
    
    iModeChanged = ETrue;
    SendIcfDataL( EPluginSyncAll );
    iModeChanged = EFalse;
    
    // For addition of ITI features on FSQ, do configuration before opening FSQ,
    // so that ITI features can be taken into effect.   
    SetItiStateL();	

    if ( ExistInlineText() )
        {
        SendIcfDataL();
        }
    
    if ( PluginInputMode() != EPluginInputModeItut )
        {
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputWindowOpen, aOpenMode);
        }
    if ( PluginInputMode() == EPluginInputModeItut 
          || PluginInputMode() == EPluginInputModeFSQ
          || PluginInputMode() == EPluginInputModeFingerHwr 
          || PluginInputMode() == EPluginInputModePortraitFSQ )
        {
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputInEditWordQueryDlg, iIsInEditWordQueryDlg);
        }    
    // For both landscape portrait FSQ mode, update FSQ indicator.
    if ( PluginInputMode() == EPluginInputModeFSQ ||
         PluginInputMode() == EPluginInputModePortraitFSQ )
        {
        UpdateFSQIndicator();
        }
    }

    
// ---------------------------------------------------------------------------
// CAknFepPluginManager::IsEnableModeSwitchBtn
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
TBool CAknFepPluginManager::IsEnableModeSwitchBtn()
    {
    TBool enableInSecret = EFalse;
        
    if( !iFepMan.IsSupportsSecretText() )
        {
        TPluginInputMode counterPart = iPluginInputMode == EPluginInputModeVkb ? 
                                EPluginInputModeHwr : EPluginInputModeVkb;
        if( iLangMan.IsPluginInputAvaiable(counterPart, 
                                      iCurrentPluginInputFepUI->SupportLanguage(iPluginInputMode), 
                                      &iPenInputServer)) 
            {
            enableInSecret = ETrue;
            }
        }
        
    return enableInSecret;
    }

// ---------------------------------------------------------------------------
// CAknFepPluginManager::IsEnableSettingBtn
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TBool CAknFepPluginManager::IsEnableSettingBtn()
    {
    TBool ebable = ETrue;
    TInt enalbeInStartup = 0;
    RProperty::Get(KPSUidUikon, KUikGlobalNotesAllowed, enalbeInStartup);
    ebable = enalbeInStartup;

    TInt priority = CCoeEnv::Static()->RootWin().OrdinalPriority();
    if (iInGlobleNoteEditor || ECoeWinPriorityNormal < priority ||
            (iPenInputMenu && iPenInputMenu->IsShowing()))
        {
        ebable = EFalse;    
        }
    
    return ebable;
    }
 
// ---------------------------------------------------------------------------
// CAknFepPluginManager::NotifyLayoutKeymappingL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//    
void CAknFepPluginManager::NotifyLayoutKeymappingL()
    {
    CAknEdwinState* editorState = iFepMan.EditorState();
    if ( editorState )
        {
        if( ( iFepMan.InputMode() == ENumber || iFepMan.InputMode() == ENativeNumber )&& 
            (editorState->Flags() & EAknEditorFlagUseSCTNumericCharmap) &&
            iFepMan.IsFepAwareTextEditor() &&
            iFepMan.IsAbleToLaunchSCT() )
            {
            TInt sctChars = EAknSCTNumeric;

            if ( !iFepMan.IsOnlyHalfWidthCharacterPermitted() )
                {
                sctChars = EAknSCTLowerCase;
                }
            //read resource from resource
            HBufC* keymapRes = GetNumberKeymapResource(sctChars);
            if( keymapRes )
                {
                CleanupStack::PushL(keymapRes);
                iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputEditorCustomNumericKeyMap,
                                                    reinterpret_cast<TInt>(keymapRes) );
                CleanupStack::PopAndDestroy(keymapRes);
                return;
                }
            }
        iCurrentPluginInputFepUI->SetNumberModeKeyMappingL(editorState->NumericKeymap() );
        }
    else
        {
        TCoeInputCapabilities inputCapabilities = iFepMan.InputCapabilities();

        if( inputCapabilities.SupportsWesternNumericReal() )
            {
            //read resource from resource
            HBufC* keymapRes = HBufC::NewL(2);
            TLocale locale;
            TChar decimalSep = locale.DecimalSeparator(); 
            TChar minusSign ='-'; 

            keymapRes->Des().Append(decimalSep);
            keymapRes->Des().Append(minusSign);

            CleanupStack::PushL(keymapRes);
            iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputEditorCustomNumericKeyMap,
                                                    reinterpret_cast<TInt>(keymapRes) );
            CleanupStack::PopAndDestroy(keymapRes);
            return;
            }

        iCurrentPluginInputFepUI->SetNumberModeKeyMappingL(EAknEditorPlainNumberModeKeymap);
        }
    }    

// ---------------------------------------------------------------------------
// CAknFepPluginManager::IsDisplayDataQuery
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TBool CAknFepPluginManager::IsDisplayDataQuery()
    {
    return iFepMan.UiInterface()->IsDisplayDataQuery(iFepMan.IsFepAwareTextEditor());
    }           

// ---------------------------------------------------------------------------
// CAknFepPluginManager::RepositionDataQuery
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepPluginManager::RepositionDataQuery(TBool aForce)
    {
    if( !aForce && iDataQueryAlreadyShow )
        {
        return;
        }
        
    TRect param;
    
    CEikAppUi* eikAppUi = (CEikAppUi *)CCoeEnv::Static()->AppUi();
    
    param = TRect(eikAppUi->TopFocusedControl()->PositionRelativeToScreen(),
                  eikAppUi->TopFocusedControl()->Size());
    
    TPixelsTwipsAndRotation size; 
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
    
    if(size.iPixelSize.iWidth > size.iPixelSize.iHeight )
        {
        // landscape, retrieve current position
        TRect parentRect;
        
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, parentRect); 
        CCoeControl* ctrl = eikAppUi->TopFocusedControl();
        TRect ctrlRect = ctrl->Rect();			
        ctrlRect.Move( 0, parentRect.iBr.iY - ctrlRect.iBr.iY );

        param = TRect( ctrlRect.iTl, ctrl->Size() );
        //ctrl->SetExtent( ctrlRect.iTl, ctrl->Size() );
        }

    TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputSetWindowPos, (TInt)&param));
    iDataQueryAlreadyShow = ETrue;
    }    

// ---------------------------------------------------------------------------
// CAknFepPluginManager::AdjustDataQueryCallBackL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TInt CAknFepPluginManager::AdjustDataQueryCallBackL(TAny *aPtr)
    {
    CAknFepPluginManager* self = static_cast<CAknFepPluginManager*>(aPtr);
    self->iAdjustDataQueryTimer->Cancel(); 
    if(self->IsDisplayDataQuery() && //data query
       self->iPluginInputMode == EPluginInputModeVkb) //only vkb mode 
        {
        self->RepositionDataQuery(ETrue);
        }
        
    return ETrue;
    }        
// -----------------------------------------------------------------------------
// CAknFepPluginManager::FepObserverHandleCompletionOfTransactionL
// Timer procedure for updating cursor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::FepObserverHandleCompletionOfTransactionL(MCoeFepObserver& aFepObserver)
    {
    aFepObserver.HandleCompletionOfTransactionL();
    }

MAknFepManagerInterface*  CAknFepPluginManager::CurrentFepInputUI()
	{
	if (PluginInputMode() ==  EPluginInputModeItut &&
	    (iFepMan.InputMode() == EPinyin || iFepMan.InputMode() == EZhuyin ||
	    iFepMan.InputMode() == EStroke || iFepMan.InputMode() == ECangJie))
	     
		{
		if (!iFingerItutChineseUI)
		    {
		    TRAP_IGNORE(CreateFingerItutChineseUiL());
		    }
		    
		return iFingerItutChineseUI;
		}
		
	return NULL;
	}

void CAknFepPluginManager::CreateFingerItutChineseUiL()
    {
    iFingerItutChineseUI = CAknFepUIManagerFingerItutChinese::NewL(&iFepMan, &iCaseMan, 
                                     (TLanguage)iSharedData.InputTextLanguage(), this);
    }

void CAknFepPluginManager::LaunchHwrTrainUiL()
    {
    _LIT(KPeninputHwrTrainingUI, "z:\\sys\\bin\\trui.exe" );
 
    RProcess process;
    TInt err = process.Create(KPeninputHwrTrainingUI, KNullDesC);
    User::LeaveIfError(err);

    process.Resume();
    process.Close();
    }    
    
TBool CAknFepPluginManager::IsVisible()
    {
    if( iPenInputSvrConnected )
        {
        return iPenInputServer.IsVisible();
        }
        
    return EFalse;
    }
    
void CAknFepPluginManager::SetFingerMatchSelectionState(TBool aMatchStateOn)
    {
    if(!iCurrentPluginInputFepUI)
        {
        return;
        }    
    if (aMatchStateOn)
        {
        TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerMatchSelection, 1));
        iMatchState = ETrue;
        }
    else
        {
        TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerMatchSelection, 0));
        iMatchState = EFalse;
        }
    }

void CAknFepPluginManager::ConvertDigitToDigittype(TDigitType& digittype,TDes& aConvertedNumeral)
	{
	TChar chchar(aConvertedNumeral[0]);
	switch(digittype)
    	{
    	case EDigitTypeDevanagari:
        	{
        	chchar = (aConvertedNumeral[0] + KLatinToDevanagariDigitsDelta);
	    	}
        	break;	
        case EDigitTypeArabicIndic:
            {
        	chchar = (aConvertedNumeral[0] + KLatinToArabicIndicDigitsDelta);
	        }
            break;
    	case EDigitTypeEasternArabicIndic:
        	{
        	chchar = (aConvertedNumeral[0] + KLatinToEasternArabicIndicDigitsDelta);
        	}
    	}
    aConvertedNumeral.Zero();
    aConvertedNumeral.Append(chchar);
	}

void CAknFepPluginManager::GetAllPredictiveCandidate()
    {
    TRAP_IGNORE(GetAllPredictiveCandidateL());    
    }

void CAknFepPluginManager::GetAllPredictiveCandidateL()
    {
    CPtiEngine* ptiengine = iFepMan.PtiEngine();
    TBuf<1> tempbuf;
    ResetCandidateList();

    for (TInt index = 0; index < ptiengine->NumberOfCandidates(); index++)
        {
        TPtrC newText = ptiengine->NextCandidate();

        if (newText.Length() > 0)
            {
      		if(newText.Length() == 1)
     			{
                TDigitType digittype = iFepMan.LocalDigit();
                TChar chchar(newText[0]);

                if(chchar.IsDigit() && digittype)
                    {
                    tempbuf.Zero();
                    tempbuf.Append(newText[0]);
                    ConvertDigitToDigittype(digittype,tempbuf);

                    HBufC* textBuf = tempbuf.Alloc();

                    CleanupStack::PushL(textBuf);
                    iCandidateList.Append(textBuf);
                    CleanupStack::Pop(textBuf);

                    continue;
                   	}
           		}

            HBufC* textBuf = newText.Alloc();
                
            CleanupStack::PushL(textBuf);
            iCandidateList.Append(textBuf);
            CleanupStack::Pop(textBuf);
            }
        }
    }

void CAknFepPluginManager::ResetCandidateList()
    {
    iCandidateIndex = 0;
    iCandidateList.ResetAndDestroy();
    }

void CAknFepPluginManager::ShowAllCandidates()
    {
    iSendAllList.iCandidates.Reset();

    GetAllPredictiveCandidate();

    if (!iCandidateList.Count())
        return;

    for (TInt i = 0; i < iCandidateList.Count(); i++)
        iSendAllList.iCandidates.Append(iCandidateList[i]->Des());
    
    SendMatchListCmd(iSendAllList.iCandidates);
    }

void CAknFepPluginManager::SendMatchListCmd(const RArray<TPtrC>& aList)
    {
    TFepITICandidateList list;
    list.iActiveIndex = 0;
    list.iItemArray2 = &aList;
    list.iLangCode = iFepMan.CurrentInputLangCode();
    if ( iLangMan.IsSplitView() )
        {
        TRAP_IGNORE( list.iRect = iFepMan.InitRectForSplitCandL() );
        }

    TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerMatchList, 
                                                         reinterpret_cast<TInt>(&list)));
    }

TBool CAknFepPluginManager::GetIndicatorImgID(const TInt IndicatorUID,TInt &aImage, TInt &aMask)
    {
    TBool ret = EFalse;
    TRAP_IGNORE(ret = GetIndicatorImgIDL(IndicatorUID, aImage, aMask));
    return ret;
    }
TBool CAknFepPluginManager::GetIndicatorImgIDL(const TInt IndicatorUID,TInt &aImage, TInt &aMask)
    {
    TBool ret = EFalse;
    aImage = 0;
    aMask =0;
    TResourceReader reader;

    CCoeEnv::Static()->CreateResourceReaderLC(reader,
        R_AVKON_NAVI_PANE_EDITOR_INDICATORS );

    TInt indicatorCount = reader.ReadInt16(); // Number of indicators in the INDICATOR_PANE resource.
            
    for ( TInt ii = 0; ii < indicatorCount; ii++ )
        {
        TInt foundUid = reader.ReadInt16(); // First is the indicator UID.

        if ( foundUid == IndicatorUID )
            {
            reader.ReadInt16(); // narrow priority, can be ignored
            reader.ReadInt16(); // wide priority, can be ignored

            HBufC* bitmapFile = reader.ReadHBufCL(); // File containing the indicator graphics.
            CleanupStack::PushL( bitmapFile );
                    
            TInt stateCount = reader.ReadInt16(); // Number of indicator states.
            TInt state      = reader.ReadInt16(); // First state is used ( can be EAknIndicatorStateOn / EAknIndicatorStateAnimate ).
            TInt iconCount  = reader.ReadInt16(); // Number of icons or frames.
                    
            if ( state == IndicatorUID )
                {
    	      // Go through the animation frames.
                for ( TInt j = 0; j < iconCount; j++ )
                    {
                    // Use the last animation frame
                    // in case of animated indicator.
                    reader.ReadInt16();
                    reader.ReadInt16();
                    reader.ReadInt16();
                    reader.ReadInt16();
                    }
                }

            // These are the correct IDs to be used for
            // the indicator.
            aImage = reader.ReadInt16();
            aMask  = reader.ReadInt16();
                    
            
    	  // CREATE THE INDICATOR ICON WITH THE BITMAP FILE & BITMAP ID INFORMATION HERE

            
            CleanupStack::PopAndDestroy( bitmapFile );
            ret = ETrue;
            break; // icon created
            }
        else // Not the correct indicator, pass through the INDICATOR resource.
            {	 		
        	  reader.ReadInt16();
        	  reader.ReadInt16();    
        	  HBufC* bitmapFile = reader.ReadHBufCL(); // bmp filename
        	  delete bitmapFile;
        	  bitmapFile = NULL;
            TInt numberOfStates = reader.ReadInt16();  // Number of states
        	  for ( TInt j = 0; j < numberOfStates; j++ )
        	      {
        	      reader.ReadInt16(); // State id
                TInt numberOfIcons = reader.ReadInt16();	        	
        	      for ( TInt jj = 0; jj < numberOfIcons; jj++ )
                    {
        		    for ( TInt jjj = CAknIndicatorContainer::ELayoutModeUsual; 
                      jjj <= CAknIndicatorContainer::ELayoutModeWide; jjj++ )
        	              {
        	              reader.ReadInt16(); // bitmap
        	              reader.ReadInt16(); // mask
        			  }
        	          }        	
        	      }
        	  }
        }
    
    CleanupStack::PopAndDestroy(); // reader

    return ret;  
    }     

void CAknFepPluginManager::UpdateFSQIndicator()    
    {
    // If it is not either portrait or landscape FSQ, return.
    if ( !( EPluginInputModeFSQ == PluginInputMode() ||
      	    EPluginInputModePortraitFSQ == PluginInputMode() ) )
        {
        return;
        }
        
    TInt indicatorImgID;
    TInt indicatorTextID;
    
    if (iFepMan.GetIndicatorImgID(indicatorImgID, indicatorTextID))   
        {
        iIndicatorImgID = indicatorImgID;
        iIndicatorTextID = indicatorTextID;
        }
        TInt indImgid = 0;
        TInt indMaskid = 0; 

        TInt textImgid = 0;
        TInt textMaskid = 0; 

    if (iIndicatorImgID > 0)
        {
        GetIndicatorImgID(iIndicatorImgID ,indImgid, indMaskid);
        }
    
    if (iIndicatorTextID > 0)
        {
        GetIndicatorImgID(iIndicatorTextID ,textImgid, textMaskid);
        }
        
    if (iIndicatorImgID != 0 || iIndicatorTextID != 0)
        {
        TFepIndicatorInfo indicator;
        
        indicator.iIndicatorImgID = indImgid;
        indicator.iIndicatorMaskID = indMaskid;
        indicator.iIndicatorTextImgID = textImgid;
        indicator.iIndicatorTextMaskID = textMaskid;
        TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerMatchIndicator, 
            reinterpret_cast<TInt>(&indicator)));    
        }
   }
void CAknFepPluginManager::UpdateITUTIndicator()    
    {
    if ( EPluginInputModeItut != PluginInputMode() )
        {
        return;
        }
        
    TInt indicatorImgID;
    TInt indicatorTextID;
    
    if (iFepMan.GetIndicatorImgID(indicatorImgID, indicatorTextID))   
        {
        iIndicatorImgID = indicatorImgID;
        iIndicatorTextID = indicatorTextID;
        }
        TInt indImgid = 0;
        TInt indMaskid = 0; 

        TInt textImgid = 0;
        TInt textMaskid = 0; 

    if (iIndicatorImgID > 0)
        {
        GetIndicatorImgID(iIndicatorImgID ,indImgid, indMaskid);
        }
    
    if (iIndicatorTextID > 0)
        {
        GetIndicatorImgID(iIndicatorTextID ,textImgid, textMaskid);
        }
        
    if (iIndicatorImgID != 0 || iIndicatorTextID != 0)
        {
        TFepIndicatorInfo indicator;
        
        indicator.iIndicatorImgID = indImgid;
        indicator.iIndicatorMaskID = indMaskid;
        indicator.iIndicatorTextImgID = textImgid;
        indicator.iIndicatorTextMaskID = textMaskid;
        TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerMatchIndicator, 
            reinterpret_cast<TInt>(&indicator)));    
        }
   }

void CAknFepPluginManager::SetITUTSpellingStateL(TBool aState)
    {
    if (aState)
        {
		SetICFTextForSpellL();
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerSpelling, 1);

        HBufC* titleStr = StringLoader::LoadL(R_AKNFEP_PROMPT_TEXT);                          
        CleanupStack::PushL(titleStr);

        TFepPromptText promptText;
        promptText.iLines = 1;
        TPtr16 canDes = titleStr->Des();
        promptText.iText.Set(canDes);

        promptText.iCleanContent = EFalse;
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputSetPromptText,
            reinterpret_cast<TInt>(&promptText));

        CleanupStack::PopAndDestroy(titleStr);
        }
    else
        {
        TPtr offPromot(NULL, 0);
        TFepPromptText promptText;
        
        promptText.iLines = 1;
        promptText.iText.Set(offPromot);
        promptText.iCleanContent = EFalse;
        
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputSetPromptText,
            reinterpret_cast<TInt>(&promptText));
         
        iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputFingerSpelling, 0);
        }
    }

void CAknFepPluginManager::DisplaySpellEditor(const TInt aEditorFlag, 
                                              const TDesC& aInitText, 
                                              TCursorSelection aCurSel)
    {
    TRAP_IGNORE(DisplaySpellEditorL(aEditorFlag, aInitText, aCurSel));
    }
void CAknFepPluginManager::DisplaySpellEditorL(const TInt aEditorFlag, 
                                              const TDesC& aInitText, 
                                              TCursorSelection aCurSel)
    {
	TInt inputLang = iFepMan.InputLanguageCapabilities().iInputLanguageCode;
	if ( inputLang == ELangPrcChinese || inputLang == ELangHongKongChinese ||
		 inputLang == ELangTaiwanChinese )
		{
	    iCurrentPluginInputFepUI->HandleCommandL( 
	    		ECmdPeninputSpellLanguageMode, KChineseSpellMode );
		}
	else
		{
		iCurrentPluginInputFepUI->HandleCommandL( 
				ECmdPeninputSpellLanguageMode, KWesternSpellMode );
		}
    TInt editorFlag = 0;
    TInt editorCase = 0;
    TInt editorSCTResId = 0;
    
    if (aEditorFlag)
	   { 
	   editorFlag = aEditorFlag;	     
	   }
       
    editorCase = iCaseMan.CurrentCase();
        
    if( iFepMan.EditorState()->Flags() & EAknEditorFlagLatinInputModesOnly )
        {
        editorFlag = editorFlag | EAknEditorFlagLatinInputModesOnly;
        }
        
    if (FeatureManager::FeatureSupported(KFeatureIdJapanese))
        {
        editorFlag = editorFlag | EAknEditorFlagLatinInputModesOnly;
        }
    
    editorSCTResId = iFepMan.EditorState()->SpecialCharacterTableResourceId();
    
    if (FeatureManager::FeatureSupported(KFeatureIdChinese))
        {
        if (!editorSCTResId || editorSCTResId == R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG)
            {
            editorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_CHINESE;
            }
        }
    else if (!editorSCTResId)
        {
        editorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG;
        }
 
    iSpellCba = ESpellCBACancelEmpty;
    iSpellOn = ETrue;
   if( !iSpell )
    	{
        iSpell = CAknFepUiSpellContainer::NewL(editorFlag, editorCase, editorSCTResId, IsEditorSupportSplitIme());
    	}
    else
    	{
        iSpell->InputPane()->InputWin()->SetAknEditorCurrentCase(editorCase);
        iSpell->InputPane()->InputWin()->SetAknEditorCurrentInputMode(EAknEditorTextInputMode);
        if ( iLangMan.IsSplitView() )
         {
         iSpell->InputPane()->InputWin()->SetAknEditorFlags( 
              iSpell->InputPane()->InputWin()->AknEdwinFlags() | 
              aEditorFlag | EAknEditorFlagNoT9 | 
              EAknEditorFlagEnablePartialScreen );
         }
        else
         {
         iSpell->InputPane()->InputWin()->SetAknEditorFlags( 
              iSpell->InputPane()->InputWin()->AknEdwinFlags() | 
              aEditorFlag | EAknEditorFlagNoT9 );
         }

        iSpell->MakeVisible( ETrue );
        iSpell->SetInputWinFocus( ETrue );
    	}
   
    iSpell->SetInputWinObserver(this);
   
    iSpell->InputPane()->SetInputWinTextL(&aInitText);
    iSpell->InputPane()->InputWin()->SetSelectionL(aCurSel.iCursorPos, aCurSel.iAnchorPos);
    AddWinToOpenListL( iSpell->DrawableWindow() );
    }    

void CAknFepPluginManager::HideSpellEditor()
    {
    iSpellOn = EFalse;
    iSpellCba = ESpellCBANone;
    // Hide spell container instead to delete spell container to avoid flicker
    if( iSpell )
    	{
        iSpell->SetInputWinObserver( NULL );
        iSpell->SetInputWinFocus( EFalse );
        iSpell->MakeVisible( EFalse );
    	}
    }

HBufC*  CAknFepPluginManager::SpellTextInput()
    {
    HBufC* textInput = NULL;
    if (iSpell)
        {
        TRAP_IGNORE(textInput = iSpell->InputPane()->InputWin()->GetTextInHBufL());
        return textInput;    
        }
    
    return NULL;    
    }

void CAknFepPluginManager::HandleEdwinEventL( CEikEdwin* /*aEdwin*/, 
                                              TEdwinEvent aEventType )
    {
    if (aEventType == EEventTextUpdate)
        {
        TInt textLength = 0;
        HBufC* text = SpellTextInput(); 
        
        if (text)
            {
            CleanupStack::PushL(text);
            textLength = text->Length();
            CleanupStack::PopAndDestroy(text); 
            }
        
        if ( textLength == 0 && 
            iSpellCba == ESpellCBAOkCancel)
            {
            iSpellCba = ESpellCBACancelEmpty;
            iFepMan.UpdateCbaL(NULL);
            }
            
        if (textLength > 0 && 
            iSpellCba == ESpellCBACancelEmpty)
            {
            iSpellCba = ESpellCBAOkCancel;
            iFepMan.UpdateCbaL(NULL);
            }
        }
    }

TInt CAknFepPluginManager::GetSoftKeyResID()
    {
    TInt resId = 0;
    switch (iSpellCba)
        {
        case ESpellCBACancelEmpty:
            {
            resId = R_AKNFEP_SOFTKEYS_FINGER_CANCEL;
            }
         break;
        case ESpellCBAOkCancel:
            {
            resId = R_AKNFEP_SOFTKEYS_FINGER_OK_CANCEL;
            }
        case ESpellCBANone:
        default:
            {
            break;
            }
        }
        
    return resId;
    }

void CAknFepPluginManager::SetPromptText( TBool aCleanContent )
    {
    if (IsDimed() && !iResourceChange) 
        { 
        return;  
        } 
        
    MCoeCaptionRetrieverForFep* capRetrieve = NULL;
    if ( iFepMan.FepAwareTextEditor() )
        {
        capRetrieve = iFepMan.InputCapabilities().CaptionRetrieverForFep();
        }
    TFepPromptText promptText;
    promptText.iLines = 1;
    promptText.iCleanContent = aCleanContent;
    
    if (capRetrieve)
        {
        TBuf<100> caption;
        caption.Zero();
        capRetrieve->GetCaptionForFep(caption);
        promptText.iText.Set(caption);
        iPreCaption.Close();
        iPreCaption.Create(caption.Length());
        iPreCaption.Append(caption);
        }  
    else
        {
        promptText.iText.Set(KNullDesC);
        if ( IsDimed() )
            {
            promptText.iText.Set(iPreCaption);
            }
	    else
			{
			iPreCaption.Close();
			}
        }    

    TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputSetPromptText,
                                                         reinterpret_cast<TInt>(&promptText)));        
    }

void CAknFepPluginManager::SetIcfAlignment()
    {
    // spell editor is not really background editor.
    //if ( iSpellOn )
    //    {
    //    return;
    //    }
    
    TUint cap = iFepMan.ExtendedInputCapabilities();

    TInt alignment = EAknEditorAlignNone;

    if (cap & CAknExtendedInputCapabilities::EInputEditorAlignCenter)
        {
        alignment = EAknEditorAlignCenter;
        }
    else if (cap & CAknExtendedInputCapabilities::EInputEditorAlignRight)
        {
        alignment = EAknEditorAlignRight;
        }
    else if (cap & CAknExtendedInputCapabilities::EInputEditorAlignBidi)
        {
        alignment = EAknEditorAlignBidi;
        }
    else if (cap & CAknExtendedInputCapabilities::EInputEditorAlignLeft)
        {
        alignment = EAknEditorAlignLeft;
        }
    else
        {
        alignment = iAlignment;
        }
		
    TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputSetTextAlignment,
                                                         alignment));
														 
    iAlignment = alignment;
    }
    
void CAknFepPluginManager::HasNoFreeSpace()
	{
    if ( iCurrentPluginInputFepUI &&
         EPluginInputModeItut == PluginInputMode() )
        {
        TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputNoFreeSpace));
        }		
	}
    
// ---------------------------------------------------------------------------
// CAknFepPluginManager::PluginInputMode
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//           
TPluginInputMode CAknFepPluginManager::PluginInputMode()
    {
    if( iPluginInputMode != EPluginInputModeNone && IsCurrentWindInOpenList() )
        {
        return iPluginInputMode;            
        }

    return EPluginInputModeNone;
    }
    
void CAknFepPluginManager::AddCurrentWinToOpenListL()
    {
    AddWinToOpenListL( GetFocusWindow() );
    }

void CAknFepPluginManager::AddWinToOpenListL( RDrawableWindow* aWin)
    {
    /*if( aWin )
        {
        iLastFocusedWin = (TInt )aWin;
        if( iOpenWindowList.Find( (TInt)aWin ) == KErrNotFound )
            {
            iOpenWindowList.AppendL( (TInt)aWin );    
            }
        
        }    
      */  
     if( aWin )
        {
        iLastFocusedWin = aWin->WsHandle();
        if( iOpenWindowList.Find( aWin->WsHandle() ) == KErrNotFound )
            {
            iOpenWindowList.AppendL( aWin->WsHandle() );    
            }
        
        }    

    }

void CAknFepPluginManager::RemoveCurrentWinFromOpenList()
    {
    RDrawableWindow* win = GetFocusWindow();
    if( win )
        {
        TInt index = iOpenWindowList.Find( win->WsHandle() );
        if( index !=  KErrNotFound )
            {
            iOpenWindowList.Remove( index );           
            }
        }     
    }

void CAknFepPluginManager::RemoveLastFocusedWinFromOpenList()
    {
    TInt index = iOpenWindowList.Find( iLastFocusedWin );
    if( index !=  KErrNotFound )
        {
        iOpenWindowList.Remove( index );           
        }
    iLastFocusedWin = 0;

    }

TBool CAknFepPluginManager::IsCurrentWindInOpenList()
    {
    // When touch ui is activated, its associated focus window is inserted into iOpenWindowList.
    // When focus go back to the associated window, touch ui is activated automatically if the window is iOpenWindowList. 
    if ( IsDimed() )
        {
        return ETrue;
        }
    RDrawableWindow* win = GetFocusWindow();       
    if( win )
        {
        return iOpenWindowList.Find( win->WsHandle() ) != KErrNotFound;
        }
    return EFalse;
    }

TBool CAknFepPluginManager::ExistInlineText()
	{
	return ( iFepMan.WesternPredictive() && 
        iFepMan.IsFlagSet( CAknFepManager::EFlagInsideInlineEditingTransaction ) && 
        !iFepMan.IsFlagSet( CAknFepManager::EFlagInsideMultitapInlineEditingTransaction )&&
        iFepMan.UncommittedText().Length() > 0 );
	}
	
void CAknFepPluginManager::UpdateCaseMode()
	{
	if (iPreviousCoseMode != iCaseMan.CurrentCase())
		{
		iPreviousCoseMode =iCaseMan.CurrentCase();
		
		if (EPluginInputModeNone == PluginInputMode())
			{
			return;	
			}
	
		TRAP_IGNORE(iCurrentPluginInputFepUI->HandleCommandL(ECmdPenInputCaseMode, 
		                                                     iCaseMan.CurrentCase()));	
		UpdateITUTIndicator();
    if ( PluginInputMode() == EPluginInputModeFSQ ||
         PluginInputMode() == EPluginInputModePortraitFSQ )
	        {
			UpdateFSQIndicator();
	        }
		}	
	}

void CAknFepPluginManager::AdjustDataCase( TDes& aData )
		{
		if( iCaseMan.CurrentCase() == EAknEditorLowerCase  )
    		{
    		aData.LowerCase();
    		}
		else if( iCaseMan.CurrentCase() == EAknEditorUpperCase )
    		{
            if( iCaseMan.IsInverseCase() )
                {
                iCaseMan.SetInverseCase( EFalse );
                aData.LowerCase();
                return;
                }
            for( TInt i = 0; i<aData.Length(); i++ )
	   			{
	   			if ( aData[i] != 0x03C2 ) //final sigma exists in capslock mode
	   				{
	   				aData.UpperCase();
	   				}
    			}
    		}
		}
		
TBool CAknFepPluginManager::IsDimArrowKeys()
    {
    TBool cursorDisabled = IsEditorCursorDisabled(); 
    return cursorDisabled || iFepMan.IsFindPaneEditor(); 
    }
    
void CAknFepPluginManager::DimAllModeAndCase(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    aMenuPane->SetItemDimmed(EChinFepCmdModeStroke, ETrue);    
    aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyin, ETrue);    
    aMenuPane->SetItemDimmed(EChinFepCmdModePinyin, ETrue);    
    aMenuPane->SetItemDimmed(EChinFepCmdModeStrokePhrase, ETrue);    
    aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyinPhrase, ETrue);    
    aMenuPane->SetItemDimmed(EChinFepCmdModePinyinPhrase, ETrue);    
    aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);     
    aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue); 
    aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);         
    }
    
void CAknFepPluginManager::LaunchHelpL()
    {
    TCoeHelpContext context;
 
    switch (PluginInputMode())
        {
        case EPluginInputModeItut:
            {
            context.iContext = KVITU_HLP_MAIN_VIEW;    
            }
        break;
		case EPluginInputModeFingerHwr:
        case EPluginInputModeHwr:
        case EPluginInputModeFSc:
            {
            context.iContext = KHWR_HLP_MAIN_VIEW;    
            }
        break;        
        case EPluginInputModeVkb:
        case EPluginInputModeFSQ:
        case EPluginInputModePortraitFSQ:	
            {
            context.iContext = KVQWERTY_HLP_MAIN_VIEW;    
            }
        break;        
        default:
            {
            return;   
            }
        }

    context.iMajor =  KAknfepUID;
   
    CArrayFixFlat<TCoeHelpContext>* array = new(ELeave)CArrayFixFlat<TCoeHelpContext>(1);
    array->AppendL( context );
    HlpLauncher::LaunchHelpApplicationL(CCoeEnv::Static()->WsSession(),array);                    
    }

TInt CAknFepPluginManager::RepeatStringFromLeft(const TDesC& aSubmitData, TDes& aEditorData)
    {
    TInt index = 0;
    TInt ret = KErrNotFound;
    for (; index < aEditorData.Length(); index++ )
        {
        //aSubmitData.CompareC( aEditorData.Left(index + 1)) < 0
        ret = aSubmitData.Find( aEditorData.Left(index + 1)); 
        if ( ret != index )
            {            
            break;
            }
	}
    return ( aEditorData.Length() < aSubmitData.Length() ) ? index : 0;
    } 
      
TBool CAknFepPluginManager::IsGlobleNotes()
    {
    TUid appId = GetCurAppUid();
    TUid focusAppId = GetFocusAppUid();    
    
    if(IsGlobalNotesApp(appId) || IsGlobalNotesApp(focusAppId))
        {
        return ETrue;				
        }   
     
     return EFalse;
     }
    

    								  
void CAknFepPluginManager::SetMenuState(TBool aUpdateEditor)
    {
    if(!iPenInputSvrConnected ) 
        {
        return;
        }

    if ( ! ( PluginInputMode() == EPluginInputModeItut ||
             PluginInputMode() == EPluginInputModeFSQ ||
             PluginInputMode() == EPluginInputModeFingerHwr ||
             PluginInputMode() == EPluginInputModePortraitFSQ ) )
        {
        return;
        }

    //save current Fep aware editor
    if (aUpdateEditor && !iInMenu)
        {
        if(iFepMan.FepAwareTextEditor())
            {
            iCurMFNECap = iFepMan.IsMfneEditor() ? iFepMan.ExtendedInputCapabilities() : 0;
            iCurEditor = iFepMan.FepAwareTextEditor();
            }
        else
            {
            iCurMFNECap = 0;
            iCurEditor = NULL; 
            }
        }
        
    iInMenu = ETrue;

    TUid curApp = GetCurAppUid();
    iPenInputServer.SetGlobalNotes( IsGlobalNotesApp(curApp) && 
                                    !iInGlobleNoteEditor);
    iPenInputServer.SetInternalPopUp(ETrue);
    
    iPenInputServer.SetEditorPriority(iEditorPriority);
    iPenInputServer.DimUiLayout(ETrue); //dim the ui    
    }
    
void CAknFepPluginManager::ResetMenuState(TBool aUnDim)    
    {
    if(!iPenInputSvrConnected ) 
        {
        return;
        }
        
    iInMenu = EFalse;
    iCurMFNECap = 0;
    iCurEditor = NULL;
    if (aUnDim)
        {
        iPenInputServer.SetInternalPopUp(EFalse);
        iPenInputServer.DimUiLayout(EFalse);       
        }
    }
    
TBool CAknFepPluginManager::IsDimed()
    {
    if(!iPenInputSvrConnected ) 
        {
        return EFalse;
        }    
    if( iNeedFetchDimState )
        {
        iCurrentDimState = iPenInputServer.IsDimmed();  
        iNeedFetchDimState = EFalse;
        }
    return iCurrentDimState;
    }
void CAknFepPluginManager::HandleiDimGainForeground(TBool aGain)
    {
    if (aGain)
        {
        iDimGainForeground = ETrue;
        }
    else
        {
        if (iDimGainForeground)
            {
            TUid focusAppId = GetFocusAppUid();

            if(iInMenu &&
               !(focusAppId.iUid == KAknCapServerUid || 
                focusAppId.iUid == KAknNotifySrvUid  ||
                focusAppId.iUid == KAknScreenServerUid ) &&
               focusAppId != GetCurAppUid() )
                {
                iDimGainForeground = EFalse;
                if (iPenInputSvrConnected)
                    {
                    iPenInputServer.SetResourceChange(EFalse);    
                    }
                
                TRAP_IGNORE( ClosePluginInputUiL(ETrue) );
                if(iPenInputSvrConnected ) //lost foreground
                    {
                    iPenInputServer.LoseForeground();
                    }  
                iPenInputServer.ClearTouchUI();
                }
            }
        }
    }
    
/*
#ifdef RD_TACTILE_FEEDBACK
TBool CAknFepPluginManager::VKBIsOpened()
	{
	return (iPenInputSvrConnected && iPenInputServer.IsVisible() && !iPenInputServer.IsDimmed());
	}
#endif // RD_TACTILE_FEEDBACK	
*/
    
#ifdef RD_SCALABLE_UI_V2
void CAknFepPluginManager::SetCursorVisibility(TBool aVisibility, TBool aReportPlugin)
    {
    iCursorVisibility = aVisibility;

    if (aReportPlugin)
        {
        SyncFepAwareText();
        }
    }
#endif // RD_SCALABLE_UI_V2

// -----------------------------------------------------------------------------
// Update editor state according to current ui state
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::SetEdwinFlagsByUiStatus( TBool aOpened )
    {    
    MCoeFepAwareTextEditor* edwin( iFepMan.FepAwareTextEditor() );
    
    TInt uiStatusFlags( EAknEditorFlagTouchInputModeOpened 
                        | EAknEditorFlagHideTextView );
    
    if ( aOpened ) 
        {
        // if another editor gets focus, remove flags from previous editor.
        if ( iEdwin != edwin )
            {                
            SetAknEdwinFlags( iEdwin, uiStatusFlags, EFalse );
            }
        
        // set flags to current focused editor
        SetAknEdwinFlags( edwin, uiStatusFlags, ETrue );
        
        //save reference to focused editor
        iEdwin = edwin;
        }
    else 
        {            
        SetAknEdwinFlags( edwin, uiStatusFlags, EFalse );
        iEdwin = NULL;
        }
    }

// -----------------------------------------------------------------------------
// Update state flags of specified editor
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::SetAknEdwinFlags( MCoeFepAwareTextEditor* aEdwin, 
                                             TInt aFlags, TBool aSet )
    {
    if ( aEdwin && aEdwin->Extension1() )
        {
        CAknEdwinState* edwinState ( static_cast<CAknEdwinState*>( 
                                     aEdwin->Extension1()->State( KNullUid ) ) );
        if ( edwinState )
            {
            TInt edwinFlags( edwinState->Flags() );
            if ( aSet )
                {
                edwinFlags |= aFlags;
                }
            else
                {
                edwinFlags &= ~aFlags;
                }
            
            edwinState->SetFlags( edwinFlags );
            }
        }
    }

void CAknFepPluginManager::InformMfneUiStatusL( TBool aOpened )
    {
    if ( iMfne && ( iMfneChanged || !aOpened ))
        {
        //make the selection longer than maximum length of current 
        //text, so it won't change the behavior of MFNE editor which 
        //doesn't support finger touch.
        TInt cursor( iMfne->DocumentLengthForFep() + 1 );
        TInt anchor( 0 );
        if ( aOpened )
            {            
            anchor = cursor;
            }
        iMfne->SetCursorSelectionForFepL( 
            TCursorSelection( cursor, anchor ) );        
        }
    if ( !aOpened )
        {
        iMfne = NULL;
        }
    }

void CAknFepPluginManager::UpdatePredicState()
    {
    if(!iSharedData.QwertyInputMode() && iFepMan.InputMode() == ELatin)
        {
        if ( iPluginInputMode != EPluginInputModeItut
            // Add this condition for addition of ITI features on FSQ
            && !IsSupportITIOnFSQ() )
            {
            if( iSharedData.PredictiveTextOn())
                {
                iFepMan.SetWesternPredictive(EFalse);
                TRAP_IGNORE( iFepMan.TryCloseUiL() );
                iT9UsedBeforePluginInput = ETrue;
                }
            }
        }  

    if(iSharedData.PredictiveTextAutoCompleteOn()
        // Add this condition for addition of ITI features on FSQ
       && !IsSupportITIOnFSQ() )
        {
        iFepMan.SetWesternAutoComplete(EFalse);
        iAutoCompBeforePluginInput = ETrue;
        iSharedData.ResetPredictiveTextAutoCompleteOn();
        }
    }
    
TBool CAknFepPluginManager::NotifyInGlobalNoteEditorL()
	{
	TUid curAppId = GetCurAppUid();
	iInGlobleNoteEditor = IsGlobalNotesApp(curAppId) ? ETrue:EFalse;
	if (iCurrentPluginInputFepUI)
		{
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputEnableSettingBtn, 
	                                                  IsEnableSettingBtn() );
		}
		
    //if it's in global notes, show it.
    if ( EPluginInputModeItut == PluginInputMode() ||
         EPluginInputModeFSQ == PluginInputMode() ||
         EPluginInputModeFingerHwr == PluginInputMode() ||
         EPluginInputModePortraitFSQ == PluginInputMode() )    
		{
		if(iInGlobleNoteEditor && iPenInputSvrConnected)
			{
			iPenInputServer.ActivatePeninputInNotesL();
	        return ETrue;	
			}
		}   
    return EFalse;              
	}
	
// ---------------------------------------------------------------------------
// Get the correct indicator for VKB
// ---------------------------------------------------------------------------
//     
TAknEditingState CAknFepPluginManager::VKBIndicatorState
                                                ( TAknEditingState aOldState )
    {      
    if ( !IsChineseIndicator( aOldState ) ) 
        {
        return aOldState;
        }
    TAknEditingState editingState = EStateNone; 
    switch ( iSharedData.DefaultOnScreenVKB() )
        {
        case EPinyin:
            {
            editingState = EIndicatorStatePinyinPhrase;
            break;
            }
        case EStroke:
            {
            if ( iSharedData.PenInputLanguage() == ELangPrcChinese )
                {
                editingState = EIndicatorStateStrokePhrase;
                }
            else
                {
                editingState = EIndicatorStateStrokeTradPhrase;
                }
            break;
            }
        case EZhuyin:
            {            
            editingState = EIndicatorStateZhuyinPhrase;
            break;
            }
        case ECangJie:
            {            
            editingState = EIndicatorStateCangJie;
            break;
            }
        default:
            {
            editingState = aOldState;
            }            
        }        
    return editingState;
    }

void CAknFepPluginManager::ClosePeninputSetting()
    {
    if ( iGsInterface )
        {
        delete iGsInterface;
        iGsInterface = NULL;
        }
    }

// ---------------------------------------------------------------------------
// Show tooltip on FSQ to show the best guess word shown inline.
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::ShowTooltipOnFSQL( TInt aSecondaryIdx )
    {
    if ( aSecondaryIdx < 0  
    		|| !iCurrentPluginInputFepUI 
    		|| !EnableITIOnFSQ() 
    		|| !( iFepMan.EditorState() )
    		|| (iFepMan.EditorState()->Flags() & EAknEditorFlagEnablePartialScreen)
    		)
        {
        iTooltipOpenOnFSQ = EFalse;
        return;
        }
    
	CDesCArray* candidates = new (ELeave) CDesCArrayFlat(KDefaultCandidateArraySize);
	CleanupStack::PushL(candidates);
	TInt activeIdx = KErrNotFound;
	iFepMan.GetCandidatesL(*candidates, activeIdx);
	
	if (aSecondaryIdx < candidates->Count())
		{
		TPtrC aText = (*candidates)[aSecondaryIdx];
	    
   	    iCurrentPluginInputFepUI->HandleCommandL(
   	    		ECmdPenInputPopupTooltip,  reinterpret_cast<TInt>(&aText));
	    iTooltipOpenOnFSQ = ETrue;		
		}
	else
		{
		iTooltipOpenOnFSQ = EFalse;
		}
    
    CleanupStack::PopAndDestroy( candidates );
    }

// ---------------------------------------------------------------------------
// Hide tooltip on FSQ if necessary.
// ---------------------------------------------------------------------------
// 
void CAknFepPluginManager::HideTooltipOnFSQL()
    {
    if ( !iCurrentPluginInputFepUI || !EnableITIOnFSQ() || !iTooltipOpenOnFSQ )
        {
        return;
        }

    iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputHideTooltip );
    iTooltipOpenOnFSQ = EFalse;
    }

// --------------------------------------------------------------------------- 
// Show candidate list
// --------------------------------------------------------------------------- 
//  
void CAknFepPluginManager::ShowCandidateListL
                                      ( TFepITICandidateList aCandidateList )
    {
    if ( !iCurrentPluginInputFepUI )
        {
        return;       
        }
    HideTooltipOnFSQL();
    iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputPopupCandidateList,
                                reinterpret_cast<TInt>( &aCandidateList ) );    
    }

// --------------------------------------------------------------------------- 
// Hide candidate list.
// --------------------------------------------------------------------------- 
//  
void CAknFepPluginManager::HideCandidateListL()
    {    
    if ( !iCurrentPluginInputFepUI )
        {
        return;       
        }
    iCurrentPluginInputFepUI->HandleCommandL( ECmdPenInputHideCandidateList );    
    }


// --------------------------------------------------------------------------- 
// Commit candidate word on FSQ. 
// --------------------------------------------------------------------------- 
//  
void CAknFepPluginManager::CommitCandidateWordOnFSQL( TInt aIndex )
    {
    iFepMan.PtiEngine()->HandleCommandL( 
							EPtiCommandUserActionSetCurrentIndexOfCandidates, 
                            &aIndex ); 
    TPtrC selectedWord = iFepMan.PtiEngine()->CurrentWord();
    iFepMan.UpdateInlineEditL( selectedWord, selectedWord.Length() );
    iFepMan.PtiEngine()->CommitCurrentWord();
    iFepMan.CommitInlineEditL();
    }

TBool CAknFepPluginManager::IsChineseIndicator( TAknEditingState aOldState )
    {
    return aOldState == EIndicatorStatePinyinPhrase
           || aOldState == EIndicatorStateStrokePhrase
           || aOldState == EIndicatorStateStrokeTradPhrase
           || aOldState == EIndicatorStateZhuyinPhrase
           || aOldState == EIndicatorStateCangJie
           || aOldState == EIndicatorStateZhuyinFind
           || aOldState == EIndicatorStateStrokeFind;
    }
    
// --------------------------------------------------------------------------- 
 // Test if editor has EEikEdwinAvkonDisableCursor flag. 
 // --------------------------------------------------------------------------- 
 //  
 TBool CAknFepPluginManager::IsEditorCursorDisabled() 
     { 
     CAknEdwinState* editorState = iFepMan.EditorState(); 
     if ( !editorState ) 
         { 
         return EFalse; 
         } 
      
     TInt flags = editorState->Flags();         
     return ( flags & EEikEdwinAvkonDisableCursor ) == EEikEdwinAvkonDisableCursor; 
     } 

// --------------------------------------------------------------------------- 
// Check if the cusror in editor is visible. 
// --------------------------------------------------------------------------- 
//  
TBool CAknFepPluginManager::IsEditorCursorVisible()
    {
    CAknEdwinState* editorState = iFepMan.EditorState(); 
    if ( !editorState ) 
         { 
         return EFalse; 
         }
     return ( editorState->Flags() & EAknEditorFlagTextCursorVisible )      
            == EAknEditorFlagTextCursorVisible;
    }

// --------------------------------------------------------------------------- 
// Check if curor selection in editor is visible.
// --------------------------------------------------------------------------- 
//  
TBool CAknFepPluginManager::IsEditorCursorSelVisible()
    {
     CAknEdwinState* editorState = iFepMan.EditorState(); 
     if ( !editorState ) 
         { 
         return EFalse; 
         }
     return ( editorState->Flags() & EAknEditorFlagSelectionVisible )      
            == EAknEditorFlagSelectionVisible;
    }
        
// --------------------------------------------------------------------------- 
// Check if ITI features is enabled when FSQ is opened.
// --------------------------------------------------------------------------- 
//  
TBool CAknFepPluginManager::EnableITIOnFSQ()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if ( IsSupportITIOnFSQ()         
         && iFepMan.IsQwerty()
         && iFepMan.WesternPredictive() )
        {
        return ETrue;    
        }
    return EFalse;
#else
    return EFalse;
#endif // RD_INTELLIGENT_TEXT_INPUT
    }

// -----------------------------------------------------------------------------
// Check if ITI can be supported by FSQ..
// -----------------------------------------------------------------------------
//    
TBool CAknFepPluginManager::IsSupportITIOnFSQ()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if ( ( iPluginInputMode == EPluginInputModeFSQ ||
           iPluginInputMode == EPluginInputModePortraitFSQ )
         // Exclude non Latin mode 
         && iFepMan.InputMode() == ELatin
         // Exclude accent range
         && ( iPluginPrimaryRange == ERangeNative 
              || iPluginPrimaryRange == ERangeEnglish
              // iPluginPrimaryRange == 0 is the default range, 
              // default range couldn't be accent. 
              || !iPluginPrimaryRange )
         && iFepMan.InputLanguageCapabilities().iSupportsWesternQwertyPredictive
         && !iSharedData.QwertyInputMode()
         // No need to support in dialer application.
         && RProcess().SecureId().iId != KPhoneSecureId )
        {
        return ETrue;
        }        
#endif
    return EFalse;
    }

// --------------------------------------------------------------------------- 
// Get scan code from hw keymapping.
// --------------------------------------------------------------------------- 
//  
TInt CAknFepPluginManager::GetScanCodeFromHwKeymapping( TUint aKeyCode )
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    CPtiCoreLanguage *lang = static_cast<CPtiCoreLanguage*>
                                   ( iFepMan.PtiEngine()->CurrentLanguage() );
    MPtiKeyMappings* keymapping = lang->GetQwertyKeymappings();
    if ( keymapping )
        {
        // Save the old input mode
        TPtiEngineInputMode oldInputMode = iFepMan.PtiEngine()->InputMode();
        // Change to input mode to EPtiEngineQwertyPredictive, after that
        // qwerty keymapping can be got
        iFepMan.PtiEngine()->SetInputMode( EPtiEngineQwertyPredictive );
        iFepMan.PtiEngine()->SetKeyboardType(EPtiKeyboardQwerty4x12);
        // Find scancode by keycode from qwerty keymapping
        TInt retKey = keymapping->KeyForCharacter( aKeyCode );
        // Restore the old input mode
        iFepMan.PtiEngine()->SetInputMode( oldInputMode );
        
#if defined(__WINS__)
    // In Emulator, Window server will not handle EPtiKeyQwertyPlus key raw event
    // as normal, it will not produce EKeyEvent event for EPtiKeyQwertyPlus key.
    // And winodw server will handle EStdKeyNkpPlus key raw event as normal.
    if (retKey == EPtiKeyQwertyPlus)
        {
        retKey = EStdKeyNkpPlus;
        }
#endif
        return retKey;
        }
    return EPtiKeyNone;
#else
    return EPtiKeyNone;
#endif // RD_INTELLIGENT_TEXT_INPUT
    }

// --------------------------------------------------------------------------- 
// Check if those keys are IIT-supported keys.
// --------------------------------------------------------------------------- 
//  
TBool CAknFepPluginManager::IsITISupportedKey( const TKeyEvent& aKeyEvent )
    {
// ITI supported conditions
// 1. Not in keymapping: some function keys, space, enter, backspace
// 2. In keymappings: keys on AB/Native range.
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if ( aKeyEvent.iScanCode != EPtiKeyNone 
    	  && ( iPluginPrimaryRange == ERangeNative 
    	       || iPluginPrimaryRange == ERangeEnglish
    	       || ( iPluginPrimaryRange == 0 && iFepMan.InputMode() != ENumber 
    	               && iFepMan.InputMode() != ENativeNumber ) ) )
        {
        return ETrue;
        }
    return EFalse;
#else
    return EFalse;
#endif // RD_INTELLIGENT_TEXT_INPUT
    }
    
TPtiTextCase CAknFepPluginManager::CaseForMappedCharacter(TChar aCharacter)
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    const TInt KDefaulCoreMaximumWordLength = 100;
    TPtiTextCase casesTOCheckAgainst[KNumberOfCases] ={  EPtiCaseLower, EPtiCaseUpper, 
                                                      EPtiCaseChrLower, EPtiCaseChrUpper, 
                                                      EPtiCaseFnLower, EPtiCaseFnUpper };
    TPtiTextCase caseCalculated = EPtiCaseLower;
    TBuf<KDefaulCoreMaximumWordLength> mappedCharacters; 
    TBool isMappingFound = EFalse;
    TPtiEngineInputMode oldInputMode = iFepMan.PtiEngine()->InputMode();
    if ( oldInputMode != EPtiEngineQwertyPredictive ) 
		{
        // if current input mode isn't EPtiEngineQwertyPredictive, 
        // change it to EPtiEngineQwertyPredictive temporarily, 
        // so that during the process of searching in qwerty keymapping afterward, 
        // qwerty keymapping can be gained.
        iFepMan.PtiEngine()->SetInputMode( EPtiEngineQwertyPredictive );
		}

    TPtiKey key = iFepMan.PtiEngine()->CharacterToKey( aCharacter );

    //It loops through all the key mappings to find the character in the key mappings and
    //if the character available in the key mapping, fetches the corresponding case.
    for( TInt i=0; i< KNumberOfCases ; i++)
        {
        iFepMan.PtiEngine()->MappingDataForKey( key, mappedCharacters, 
                                                casesTOCheckAgainst[i] );
        for( TInt j=0; j < mappedCharacters.Length(); j++ )
            {
            if( mappedCharacters[j] == aCharacter )
                {
                caseCalculated = casesTOCheckAgainst[i];
                isMappingFound = ETrue;
                break;
                }
            }
        if ( isMappingFound )
        	{
        	break;
        	}
        }
    
    if ( oldInputMode != EPtiEngineQwertyPredictive ) 
    	{
        // if current input mode isn't EPtiEngineQwertyPredictive, 
        // current input mode is set to EPtiEngineQwertyPredictive temporarily before, 
        // now we need to restore it,
        // because state machine is responsible for changing it practically.
        iFepMan.PtiEngine()->SetInputMode( oldInputMode );
    	}
    //Now if there is no key mapping found for the character, then use the default TChar
    //APIs to find the case of the character.
    if( !isMappingFound )
        {
        caseCalculated = aCharacter.IsUpper()?EPtiCaseUpper:EPtiCaseLower;
        }
    return caseCalculated;
#else
    return aCharacter.IsUpper() ? EPtiCaseUpper : EPtiCaseLower;   
#endif // RD_INTELLIGENT_TEXT_INPUT
    }
    
TBool CAknFepPluginManager::IsNeedToChangeTextCase( const TPtiTextCase& aTextCase )
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
	// Not need change case if it is supported in language
	if ( iCaseMan.IsAbleChangeCase() )
		{
	    return EFalse;
		}
	
    TPtiTextCase textCase = aTextCase;
    if( EPtiKeyboardHalfQwerty != iFepMan.PtiEngine()->KeyboardType() )
		{
		if ( textCase  == EPtiCaseChrLower )
			{
			textCase = EPtiCaseLower;
			}
		else if (textCase == EPtiCaseChrUpper)
			{
			textCase = EPtiCaseUpper;
			}
		}
    return ( iFepMan.PtiEngine()->Case() == textCase )
            ? EFalse : ETrue;   
#else
    return EFalse;
#endif // RD_INTELLIGENT_TEXT_INPUT
    }

// -----------------------------------------------------------------------------
// Do some configuration to make FSQ support ITI features.
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::SetItiStateL()
    {
    if ( !IsSupportITIOnFSQ() )
    	{
    	return;
    	}
    if ( iIsITIConfigured )
    	{
        // if fep state has been configed to iti status before,
        // there is no need to config fep state again,
        // but sending command to fsq to update iti status flag is needed.               
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPeninputITIStatus,
                                              iFepMan.WesternPredictive() );    
		return;
    	}
    // 1. Config keyboard layout type
    SetItiKeyboardLayoutL();               
    
    // 2. Change FEP to qwerty mode.   
    iFepMan.SetQwertyMode( ETrue );

    // 3. Notify the activation of ITI on FSQ to peninput server        
    iCurrentPluginInputFepUI->HandleCommandL( ECmdPeninputITIStatus,
                                              iFepMan.WesternPredictive() );

    // 4. if there is un-commited text editor, 
    //    need to setcurrentword to ptiengine again.
    TInt unCommitedLen = iFepMan.UncommittedText().Length();
    if ( unCommitedLen > 0 )
        {
        HBufC* unCommitedText = HBufC::NewL( unCommitedLen );
        TPtr unCommitedTextPtr = unCommitedText->Des();        
        TInt startPos = iFepMan.UncommittedText().LowerPos();
        MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
        if ( edit )
        	{
            edit->GetEditorContentForFep( unCommitedTextPtr, 
                                          startPos, unCommitedLen );       
        	}
		iFepMan.PtiEngine()->SetCurrentWord( *unCommitedText );
		delete unCommitedText;
		unCommitedText = NULL;
        }
    
    // 5. Set ITI configured flag;
    iIsITIConfigured = ETrue;
    }

// -----------------------------------------------------------------------------
// Restore some configuration after closing FSQ.
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::ResetItiStateL()
    {    
    if ( !iIsITIConfigured )
    	{
    	return;
    	}
    
    // Close FSQ then current keyboard type return to the previously-stored one,
    // But, if iSharedData.QwertyInputMode() == EFalse, it means that hardware 
    // is slided, and capserver will be responsible to set the corrected keyboard type
    // and qwerty input mode. 
    // So there is no need to do step 1 and step 2 here, otherwise, 
    // values set by capserver will be overrided.
    if ( !iSharedData.QwertyInputMode() )
    	{    
		// 1. The first step is to restore keyboard type, because the value will
		// be used in iFepMan.SetQwertyMode( EFalse );
		ResetItiKeyboardLayoutL();
		
		// 2. Change FEP to non qwerty mode.
		iFepMan.SetQwertyMode( EFalse );    
    	}
    
    // 3. Notify the deactivation of ITI to peninputserver        
    iCurrentPluginInputFepUI->HandleCommandL( ECmdPeninputITIStatus, EFalse );    
 
    // 4. Clear ITI configured flag;
    iIsITIConfigured = EFalse; 
    }

// -----------------------------------------------------------------------------
// Config keyboard layout for ITI.
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::SetItiKeyboardLayoutL()
    { 
    if ( iLastKeyboardLayout || !IsSupportITIOnFSQ() )
        {
        return;
        }    
    // 1. Record the previous virtual keyboard type    
    RProperty::Get( KPSUidAknFep, KAknFepVirtualKeyboardType, iLastKeyboardLayout );
    
    // 2. Utilize right qwerty keyboard type
    RProperty::Set( KPSUidAknFep, KAknFepVirtualKeyboardType, EPtiKeyboardQwerty4x12 );
    
    }

// -----------------------------------------------------------------------------
// Restore keyboard layout after closing FSQ.
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::ResetItiKeyboardLayoutL()
    {    
    if ( !iLastKeyboardLayout )
    	{
    	return;
    	}
    
    // Close FSQ then current keyboard type return to the previously-stored one,
    //Restore keyboard layout type and qwerty mode of FEP        
    // 1. Store the previous keyboard type, 
    // except that opening predictive setting dialog cause fsq closed.    
    RProperty::Set( KPSUidAknFep, KAknFepVirtualKeyboardType, 
                                          iLastKeyboardLayout );
    
    
    iLastKeyboardLayout = 0;        
    }

// -----------------------------------------------------------------------------
// Restore predict state.
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::RestorePredictStateL()
    {
    if( iT9UsedBeforePluginInput )
        {
        iT9UsedBeforePluginInput = EFalse;
        iFepMan.SetWesternPredictive(ETrue);
        iFepMan.TryChangeModeL(ELatin);     
        }
    if(iAutoCompBeforePluginInput)
        {
        iAutoCompBeforePluginInput = EFalse;
        iFepMan.SetWesternAutoComplete(ETrue);        
        iFepMan.TryChangeModeL(ELatin);      
        }
    if ( IsSupportITIOnFSQ() )
    	{        
        iCurrentPluginInputFepUI->HandleCommandL( ECmdPeninputITIStatus,
                                                  iFepMan.WesternPredictive() );    	
    	}
    }

// -----------------------------------------------------------------------------
// Check if the keycode belongs to dead keys.
// -----------------------------------------------------------------------------
//
TBool CAknFepPluginManager::IsDeadKeyCode( TUint aKeyCode )
	{	
	for ( TInt i = 0; i < sizeof(KSupportedDeadKeys) / sizeof(TUint); i++ )
		{
		if ( aKeyCode == KSupportedDeadKeys[i] )
			{
			return ETrue;
			}
		}
	return EFalse;
	}
 
// -----------------------------------------------------------------------------
// Notify app touch input window closed or open.
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::NotifyAppUiImeTouchWndStateL( const TBool aTouchState )
    {
    if ( iLangMan.IsSplitView() )
        {
        CEikonEnv* eikEnv = CEikonEnv::Static();
        TUint param = aTouchState ? KAknSplitInputEnabled : KAknSplitInputDisabled;
        if ( eikEnv && eikEnv->EikAppUi() )
            {
            eikEnv->EikAppUi()->ReportResourceChangedToAppL( param );
            }
        }
    }

// -----------------------------------------------------------------------------
// Check if current editor support partial screen IMEs
// -----------------------------------------------------------------------------
//
TBool CAknFepPluginManager::IsEditorSupportSplitIme()
    {
	
	TBool disablePartialInput = FeatureManager::FeatureSupported( KFeatureIdChinese ) 
	                            || FeatureManager::FeatureSupported( KFeatureIdKorean );
	if ( disablePartialInput )
	    {
	    return EFalse;
	    }

    //Mfne editors
    if ( iFepMan.IsMfneEditor() )
        {
        TUint caps = iFepMan.ExtendedInputCapabilities();
        return CAknExtendedInputCapabilities::EInputEditorPartialScreen == 
                   ( caps & CAknExtendedInputCapabilities::EInputEditorPartialScreen );
        }
    
    //Normal editors
    CAknEdwinState* state = iFepMan.EditorState(); 
    if (iCurEditor && state == NULL)
        {
        if( iCurMFNECap )
            {
            return CAknExtendedInputCapabilities::EInputEditorPartialScreen == 
                 ( iCurMFNECap & CAknExtendedInputCapabilities::EInputEditorPartialScreen );
            }
        else
            {
            MCoeFepAwareTextEditor_Extension1* extension = iCurEditor->Extension1(); 
            if( extension )
                {
                state = static_cast<CAknEdwinState *>( extension->State(KNullUid) );
                }
            }
        }
    if ( state )
        {
        return EAknEditorFlagEnablePartialScreen == 
                   ( state->Flags() & EAknEditorFlagEnablePartialScreen );
        }
    
    return EFalse;
    }

void CAknFepPluginManager::OnServerReady(TInt aErr)
    {
    if( KErrNone != aErr )
        {
        iFepMan.UiInterface()->TouchPaneSetInputMethodIconActivated(EFalse);
            
        return;               
        }
    TRAP_IGNORE(iPenInputServer.AddPeninputServerObserverL(this)); //always add the handler            
    
    iPenInputSvrConnected = ETrue;                     
    TRAP_IGNORE(ActivatePenInputL());
    }

// -----------------------------------------------------------------------------
// Handling the menu cmd for changing the input mode
// -----------------------------------------------------------------------------
//
void CAknFepPluginManager::ProcessChangingInputModeCmdL(TInt aInputMode)
    {
    // Save iCurEditor and iCurMFNECap to temp variable the curEditor 
    // and CurMFNECap respectively before calling ResetMenuState 
    // since this function will reset iCurEditor
    // but later TryChangePluginInputModeByModeL() will use this as 
    // an flag to identify whether the mode to be opened is split one. 
    MCoeFepAwareTextEditor* savedCurEditor = iCurEditor;
    TUint savedCurMFNECap = iCurMFNECap;
    
    
    //Here calling this function is to reset menu state when the focus change 
    // caused by the screen orientation, in the normal circumstance the foucus
    // change should not occur while the screen orientation is handling. 
    // So directly resetting menu state to avoid the above case, 
    // preconditon is that the option menu must be closed.
    
    // Reset the menu state to clear iInMenu to process focus change message
    // following funciton will set iCurEditor to NULL
    ResetMenuState(EFalse);
    ClosePluginInputModeL(ETrue);  
    iFepMan.TryCloseUiL(); 

    // we have to resore iCurEditor's value which was saved before calling 
    // ResetMenuState since the iCurEditor will be used as a falg to identify 
    // whether the the mode to be opened is split one.
    // if it would be NULL, TryChangePluginInputModeByModeL will first open 
    // the Normal FSQ/VITUT, and then after a focus change
    // [the menu is really dismissed], 
    // the correct Split FSQ/VITT will be opened, so the flicker will be seen. 
    iCurEditor = savedCurEditor;
    iCurMFNECap = savedCurMFNECap;
     
    TryChangePluginInputModeByModeL((TPluginInputMode)aInputMode, 
                                    EPenInputOpenManually,
                                    ERangeInvalid);
    iCurMFNECap = 0;
    iCurEditor = NULL;
    }

CConnectAo::CConnectAo(CAknFepPluginManager* aClient) 
                : CActive(CActive::EPriorityStandard),
                  iClient(aClient)
    {
    CActiveScheduler::Add(this);
    //SetActive();
    }


void CConnectAo::RunL()
    {
    iClient->OnServerReady(iStatus.Int());
    }

void CConnectAo::DoCancel()
    {
    
    }
TInt CConnectAo::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

void CConnectAo::RequestConnect()
    {
    iStatus = KRequestPending;
    SetActive();
    }
TRequestStatus& CConnectAo::RequestStatus()
    {
    return iStatus;
    }

//End of File
