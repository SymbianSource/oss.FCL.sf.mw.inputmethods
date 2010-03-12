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
* Description:  drop-down list control
*
*/


#include <aknlayoutscalable_apps.cdl.h>
#include <peninputgeneralitutnew.rsg>
#include <peninputitutconfig_chinese.rsg>
#include <skinlayout.cdl.h>
#include <AknsDrawUtils.h>
#include <AknsUtils.h>
#include <AknUtils.h> 
#include <peninputrawkeybutton.h>
#include <peninputrepeatbutton.h>
#include <peninputcommonbutton.h>
#include "peninputrepeatbuttonex.h"
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <peninputdropdownlist.h>
#include <peninputlayoutchoicelist.h>
#include <peninputscrollablelist.h>
#include <s32mem.h>
#include <peninputlabel.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputlayoutchoicelist.h>

#include "peninputgenericitutwindowmanager.h"
#include "peninputgenericitutwindow.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutuistatebase.h"
#include "peninputgenericitutconverter.h"

_LIT(KEmptyString, "");
const TInt KPeriodicTimerIntervalSec(2500000);
/* Teleca change end, 18.05.2009 ssal */
CGenericItutWindowManager* CGenericItutWindowManager::NewL(CGenericItutUiLayout* aLayoutOwner,
                                                           CGenericItutDataMgr* aDataMgr)
    {
    CGenericItutWindowManager* layout = new (ELeave) 
                                                CGenericItutWindowManager(aLayoutOwner,aDataMgr);
    CleanupStack::PushL(layout);
    layout->ConstructL();
    CleanupStack::Pop(layout);
    return layout;
    }

CGenericItutWindowManager::CGenericItutWindowManager(CGenericItutUiLayout* aLayoutOwner,
                                                     CGenericItutDataMgr* aDataMgr)
                                          :iLayoutOwner(aLayoutOwner),
                                           iDataMgr(aDataMgr),
                                           iLastRawKeyDown(EStdKeyNull)
                                           
    {
    iInEditWordQueryDlg = EFalse;
    }

CGenericItutWindowManager::~CGenericItutWindowManager()
    {
    delete iWindow;
    /* Teleca change begin, 18.05.2009 ssal */
    if (iInfoTimer)
    	{
    	iInfoTimer->Cancel();
    	}
   	delete iInfoTimer;
    /* Teleca change end, 18.05.2009 ssal */
    }

void CGenericItutWindowManager::SetPropertyL(MItutPropertySubscriber::TItutProperty aPropertyName, 
                                        const TDesC& aPropertyValue)
    {
    iWindow->SetPropertyL(aPropertyName, aPropertyValue);
    
    }

void CGenericItutWindowManager::ConstructL()
    {
    iWindow = CGenericItutWindow::NewL(this, iLayoutOwner,iDataMgr);
    /* Teleca change begin, 18.05.2009 ssal */
    iInfoTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    /* Teleca change end, 18.05.2009 ssal */
    }

void CGenericItutWindowManager::SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType)
    {
    iLayoutOwner->SimulateRawEvent(aScanCode, aType);
    }

// ---------------------------------------------------------------------------
// Simulate raw event to window server immediately.
// simulating raw event command won't be scheduled by AO 
// because it won't be added into CPeninputAnimObj::iAnimCmd.
// ---------------------------------------------------------------------------
//    
void CGenericItutWindowManager::SimulateImmeRawEvent( TInt aScanCode, TRawEvent::TType aType )
	{
	iLayoutOwner->SimulateImmeRawEvent(aScanCode, aType );
	}

void CGenericItutWindowManager::ShowInputModeSwitchL()
    {
    iWindow->DisplayInputmodeSwitchL();   
    }
                
void CGenericItutWindowManager::HandleCtrlEventL(TInt aEventType, 
                                                 CFepUiBaseCtrl* aCtrl, 
                                                 const TDesC& aEventData)
    {
    switch(aEventType)
        {
        case EEventRawKeyDownEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            SimulateRawEvent(key->iScanCode,TRawEvent::EKeyDown);
            SetLastRawKeyDown(key->iScanCode, ETrue, aCtrl);
            }
            break;
        case EEventRawKeyUpEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            SimulateRawEvent(key->iScanCode,TRawEvent::EKeyUp);
            SetLastRawKeyDown(key->iScanCode, EFalse, aCtrl);
            }
            break;
        case EItutCmdEnterSpellMode:
            {
            iLayoutOwner->SignalOwner(ESignalEnterSpellMode);
            }
            break;        
        case EEventButtonUp:
            {
            //from pen input switch button
            if (aCtrl->ControlId() == ECtrlIdSwitch)
                {
                TRAP_IGNORE( iWindow->Icf()->UpdateSecretTextL() );
                ShowInputModeSwitchL();
                }
            else if (aCtrl->ControlId() == ECtrlIdArrowUp)
                {
                TRAP_IGNORE(iWindow->Icf()->HandleArrowBtnEventL(
                                                          CFepLayoutMultiLineIcf::EArrowUp));
                }
            else if (aCtrl->ControlId() == ECtrlIdArrowDown)
                {
                TRAP_IGNORE(iWindow->Icf()->HandleArrowBtnEventL(
                                                          CFepLayoutMultiLineIcf::EArrowDown));
                }
            else if (aCtrl->ControlId() == ECtrlIdClose)
                {
                iLayoutOwner->SignalOwner(ESignalLayoutClosed);
                }
            else if (aCtrl->ControlId() == ECtrlIdOptions)
                {
                iLayoutOwner->SignalOwner(ESignalLaunchOptionsMenu);
//                static_cast<CAknFepCtrlCommonButton*>(aCtrl)->SetDimmed(ETrue);
                }
            else if (aCtrl->ControlId() == ECtrlIdIndicator)
                {
                iLayoutOwner->SignalOwner(ESignalGetEditMenuData);
                }                
            }
            break;
        case EItutCmdArrowUp:
            {
            if (aCtrl->ControlId() == ECtrlIdArrowUp)
                {
                TRAP_IGNORE(iWindow->Icf()->HandleArrowBtnEventL(
                                                          CFepLayoutMultiLineIcf::EArrowUp));
                }
            }
            break;
        case EItutCmdArrowDown:
            {
            if (aCtrl->ControlId() == ECtrlIdArrowDown)
                {
                TRAP_IGNORE(iWindow->Icf()->HandleArrowBtnEventL(
                                                          CFepLayoutMultiLineIcf::EArrowDown));
                }
            }
            break;
        case EEventChoiceSelected:
            {
            CFepLayoutChoiceList::SEvent* event = 
                (CFepLayoutChoiceList::SEvent*)aEventData.Ptr();
                
            if (aCtrl->ControlId() == ECtrlIdInputSwitch)
                {
                if (event->iIndex == -1)
                    {
                    iWindow->InputModeSwitch()->CloseWindow();
                    break;
                    }

                if (EPluginInputModeItut != event->iCommand)
                    {
                    TBool switchByMode = ETrue;
                    TInt  mode = event->iCommand;

                    TBuf<8> buf;
                    buf.Append(reinterpret_cast<TText*>(&switchByMode), 
                               sizeof(TBool)/sizeof(TText));
                    buf.Append(reinterpret_cast<TText*>(&mode), 
                               sizeof(TInt)/sizeof(TText));
                    iLayoutOwner->SignalOwner(ESignalLayoutUIChanged,buf);
                    }
                }
            else if (aCtrl->ControlId() == ECtrlIdMatchSelection)
                {
                if (event->iCommand == EItutCmdPredictItemSelected)
                    ReportItemSelected(ESignalSelectMatchSelectionText, event->iIndex, ETrue);
                else if (event->iCommand == EItutCmdEnterSpellMode)
                    iLayoutOwner->SignalOwner(ESignalEnterSpellMode);
                }
            else if (aCtrl->ControlId() == ECtrlIdEditorMenu)
                {
                if (event->iIndex == -1)
                    {
                    iWindow->EditorMenu()->CloseWindow();
                    break;
                    }
                else
                    {                        
                    TInt command = event->iCommand;
                    TBuf<8> buf;
                    buf.Append(reinterpret_cast<TText*>(&command), sizeof(TInt)/sizeof(TText));
                    iLayoutOwner->SignalOwner(ESignalEditorMenuCommand,buf);
                    }
                }                
            }
            break;
        case EEventCapturePointer:
            {
            if (iRawKeyDown)
                {
                SimulateRawEvent(iLastRawKeyDown, TRawEvent::EKeyUp);
                iRawKeyCtrl->CancelPointerDownL();
                SetLastRawKeyDown(iLastRawKeyDown, EFalse, NULL);
                }
            }
            break;
        default:
            break;
        };
    }

TInt CGenericItutWindowManager::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
    {
    TRAPD(err, iWindow->Icf()->SetTextL(aData));

    if (!iWindow->MatchSelection()->Hiden())
        {
        iWindow->MatchSelection()->Draw();
        }
    
    return err;
    }

void CGenericItutWindowManager::SetPromptTextL(TUint8* aData)
    {
    iWindow->SetPromptTextL(aData);
    }

TBool CGenericItutWindowManager::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    TBool handle = EFalse;
    TInt* data = (TInt*) aData;
    switch (aCmd)
        {
        case ECmdPenInputIsSecretText:
            {
            iWindow->Icf()->SetTextIsSecret(*aData);
            CPeninputLayoutInputmodelChoice* choice = 
                iWindow->InputModeSwitch();
            if ( choice )
                {
                choice->SetSecretEditor(*aData);
                }
            }
            break;
        case ECmdPenInputIsNumberGrouping:
            {
            iWindow->Icf()->SetNumberGrouping(*aData);
            }
            break;
        case ECmdPenInputDimArrowKeys:
            {
            TBool IsDimArrowKeys = *aData;
            DimArrowKeys( IsDimArrowKeys );
            }
            break;
        case ECmdPenInputFingerMatchIndicator:
            {
            if ( IsPortraitWest() && ( !iDataMgr->IsChineseGlobalLanguage()))
                {
                iWindow->UpdateIndiBubbleL( aData );
                }
            else
                {
                UpdateIndicatorL( aData );
                }
            handle = ETrue;
            }
            break;
        case ECmdPenInputSetPromptText:
            {
            SetPromptTextL(aData);
            handle = ETrue;
            }
            break;
        case ECmdPenInputSendEditMenuData:
            {
            // Show menu command list
            iWindow->ShowEditorMenuL(data);
            }
            break;
        case ECmdPenInputEnableSettingBtn:
            {
            CAknFepCtrlCommonButton * optionsbtn;
            TBool enable = *data;            
            optionsbtn = static_cast<CAknFepCtrlCommonButton *>(iWindow->Control(ECtrlIdOptions));
            if(optionsbtn && !enable)
                {
                optionsbtn->SetDimmed(ETrue);
                handle = ETrue;
                iDimOperationBtn = ETrue;
                }
            else if(optionsbtn && enable)
                {
                optionsbtn->SetDimmed(EFalse);
                handle = ETrue;
                iDimOperationBtn = EFalse;
                }
            break;
            }

        case ECmdPenInputEnableIndicatorButton:
            {
            if ( NULL != static_cast<CAknFepCtrlCommonButton*>(Control(ECtrlIdIndicator)))
                {
                if (*data)
                    {
                    static_cast<CAknFepCtrlCommonButton*>(
                                                  Control(ECtrlIdIndicator))->SetDimmed(EFalse);
                    }
                else
                    {
                    static_cast<CAknFepCtrlCommonButton*>(
                                                  Control(ECtrlIdIndicator))->SetDimmed(ETrue);
                    }
                }
            }
        break;
        case ECmdPenInputSymbolOfHardKeyOne:
            {
            iWindow->SetHardKeyOneSymbol(aData);
            handle = ETrue;
            }
            break;
        case ECmdPenInputRotation:
            {
            if (iWindow->InputModeSwitch())
                {
                iWindow->InputModeSwitch()->CloseWindow();    
                }
            
            if (iWindow->EditorMenu()) 
                {
                iWindow->EditorMenu()->CloseWindow();    
                }
            }
        break;

        case ECmdPenInputInEditWordQueryDlg:
            {
            iInEditWordQueryDlg = *data;
            }
            break;
        default:
        break;
            
        };
    return handle;
    }


TInt CGenericItutWindowManager::SizeChanged(TBool aIcfOverlap)
    {
    return iWindow->SizeChanged(aIcfOverlap);
    }

void CGenericItutWindowManager::SubmitText(const TDesC& aEventData)
    {
    iLayoutOwner->SignalOwner(ESignalKeyEvent, aEventData);
    }

TInt CGenericItutWindowManager::OnSkinChange()
    {
    TRAPD(err,
          iWindow->ConstructSpellCtrlFromResourceL(); 
          iWindow->ConstructItutKeypadFromResourceL(iWindow->ItutKeypad()->ResourceId());
          iWindow->ConstructIcfFromResourceL());
   
    iWindow->OnSkinChange();
    return err;
    }

void CGenericItutWindowManager::ShowArrowBtn(TInt aShowFlag)
    {
    iWindow->Control(ECtrlIdArrowLeft)->Hide(!(EBtnArrowLeft & aShowFlag));
    iWindow->Control(ECtrlIdArrowRight)->Hide(!(EBtnArrowRight & aShowFlag));
    iWindow->Control(ECtrlIdArrowUp)->Hide(!(EBtnArrowUp & aShowFlag));
    iWindow->Control(ECtrlIdArrowDown)->Hide(!(EBtnArrowDown & aShowFlag));
    }
    
CFepUiLayout* CGenericItutWindowManager::UiLayout()
    {
    return iLayoutOwner;
    }
    
TBool CGenericItutWindowManager::IsPortraitWest()
    {
    return !iDataMgr->IsChinese() && !iDataMgr->IsLandScape();
    }
void CGenericItutWindowManager::HandleAppInfoChangeL(const TDesC& aInfo)
    {
    CGenericItutUiLayout * itutLayoutOwner;
    itutLayoutOwner = static_cast<CGenericItutUiLayout *>(UiLayout());
    if ( aInfo.Length() > 0 && 
         itutLayoutOwner->UiMgr()->CurrentState() &&
         itutLayoutOwner->UiMgr()->CurrentState()->StateType() !=
            CGenericItutUiMgrBase::EStateSpelling && 
         !iInEditWordQueryDlg)
        {
        if ( IsPortraitWest() && (!iDataMgr->IsChineseGlobalLanguage()))
            {
			iWindow->Icf()->HideBubble();
            iWindow->SetIndiWithTextFlag( ETrue );
            iWindow->IndiBubbleWithText();
            
            if ( iDataMgr->IndicatorData().iIndicatorImgID != 0 &&
                 iDataMgr->IndicatorData().iIndicatorMaskID != 0 && 
                 iDataMgr->IndicatorData().iIndicatorTextImgID != 0 &&
                 iDataMgr->IndicatorData().iIndicatorTextMaskID != 0 )
                {
                iWindow->SetIndiBubbleImageL( 
                        iDataMgr->IndicatorData().iIndicatorImgID,
                        iDataMgr->IndicatorData().iIndicatorMaskID,
                        iDataMgr->IndicatorData().iIndicatorTextImgID,
                        iDataMgr->IndicatorData().iIndicatorTextMaskID);
                }
            }
        iWindow->Icf()->ShowBubble(aInfo,iWindow->Icf()->MsgBubbleCtrl()->Rect());
        }
    else
        {
        if ((!IsPortraitWest()) || iDataMgr->IsChineseGlobalLanguage())
            {
            iWindow->Icf()->HideBubble();
            }
        else
            {
			if ( itutLayoutOwner->UiMgr()->CurrentState() &&
			     itutLayoutOwner->UiMgr()->CurrentState()->StateType() !=
			            CGenericItutUiMgrBase::EStateSpelling )
				{
				iWindow->Icf()->HideBubble();
				iWindow->SetIndiWithTextFlag( EFalse );
				iWindow->IndiBubbleWithoutText();
				}
				
				if ( iDataMgr->IndicatorData().iIndicatorImgID != 0 &&
					 iDataMgr->IndicatorData().iIndicatorMaskID != 0 && 
					 iDataMgr->IndicatorData().iIndicatorTextImgID != 0 &&
					 iDataMgr->IndicatorData().iIndicatorTextMaskID != 0 )
					{
					iWindow->SetIndiBubbleImageL( 
							iDataMgr->IndicatorData().iIndicatorImgID,
							iDataMgr->IndicatorData().iIndicatorMaskID,
							iDataMgr->IndicatorData().iIndicatorTextImgID,
							iDataMgr->IndicatorData().iIndicatorTextMaskID);
					}
            
            iWindow->Icf()->ShowBubble( KEmptyString, 
                    iWindow->Icf()->MsgBubbleCtrl()->Rect());
            }
        }
    }

/* Teleca change begin, 19.05.2009 ssal */
void CGenericItutWindowManager::DisplayWarningNote(const TDesC& aInfo)
    {
	if (!iInfoTimer->IsActive())
		{
		iInfoTimer->Start(KPeriodicTimerIntervalSec,
                          KPeriodicTimerIntervalSec, 
                          TCallBack(CGenericItutWindowManager::HideByteWarningBubble, this));
		}	                                       
    iWindow->Icf()->ShowByteWarningBubble(aInfo);
    }
/* Teleca change end, 19.05.2009 ssal */
void CGenericItutWindowManager::ApplyVariantLafDataL(TBool aResolutionChange)
    {
    iWindow->ApplyVariantLafDataL(aResolutionChange);
    }

void CGenericItutWindowManager::CreateChineseSpecificCtrlsIfNeededL()
    {
    iWindow->CreateChineseSpecificCtrlsIfNeededL();   
    }
    
void CGenericItutWindowManager::SetLanguageL(TInt aLanguage)
    {
    iWindow->Icf()->SetLanguageId(aLanguage);    
    }
    
void CGenericItutWindowManager::SetLastRawKeyDown(TInt aScanCode,  TBool aKeyDown, 
                                                  CFepUiBaseCtrl* aCtrl)
    {
    !aKeyDown ? iLastRawKeyDown = EStdKeyNull : iLastRawKeyDown = aScanCode;
    iRawKeyDown = aKeyDown;
    iRawKeyCtrl = aCtrl;
    } 
    
void CGenericItutWindowManager::ResetIndicatorL(CFepUiBaseCtrl* aIndicator)
	{
    iWindow->SetIndicatorImageL(aIndicator, 
                       iDataMgr->IndicatorData().iIndicatorImgID,
                       iDataMgr->IndicatorData().iIndicatorMaskID,
                       iDataMgr->IndicatorData().iIndicatorTextImgID,
                       iDataMgr->IndicatorData().iIndicatorTextMaskID);
	}

void CGenericItutWindowManager::UpdateIndicatorL(TUint8* aData)
    {
    iWindow->UpdateIndicatorL(aData);
    }

void CGenericItutWindowManager::ReportItemSelected(TInt aCmdId, TInt aIdx, TBool aCommit)
    {
    TBuf<8> buf;
    buf.Append(reinterpret_cast<TText*>(&aIdx), sizeof(TInt)/sizeof(TText));
    buf.Append(reinterpret_cast<TText*>(&aCommit), sizeof(TBool)/sizeof(TText));

    iLayoutOwner->SignalOwner(aCmdId, buf);
    }
    
CFepUiBaseCtrl* CGenericItutWindowManager::Control(TInt aCtrlId)  
    {
    return iWindow->Control(aCtrlId);   
    }

void CGenericItutWindowManager::OnActivate()
    {
    //static_cast<CAknFepCtrlCommonButton*>(Control(ECtrlIdOptions))->SetDimmed(EFalse);
    if (iDimOperationBtn)
        {
        static_cast<CAknFepCtrlCommonButton*>(Control(ECtrlIdOptions))->SetDimmed(ETrue);    
        }
    else
        {
        static_cast<CAknFepCtrlCommonButton*>(Control(ECtrlIdOptions))->SetDimmed(EFalse);      
        }
    }
void CGenericItutWindowManager::DimArrowKeys( TBool aDimArrow )
    {
    if (iDataMgr->IsChinese())
        {
        CAknFepCtrlRawKeyButton * leftbtn;
        CAknFepCtrlRawKeyButton * rightbtn;
        CAknFepCtrlRawKeyButton * upbtn;
        CAknFepCtrlRawKeyButton * downbtn;
        leftbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowLeft));
        rightbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowRight));
        upbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowUp));
        downbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowDown));

        leftbtn->SetDimmed(aDimArrow);   
        rightbtn->SetDimmed(aDimArrow);   
        upbtn->SetDimmed(aDimArrow);   
        downbtn->SetDimmed(aDimArrow);   

        }
    else
        {
        CAknFepCtrlRawKeyButton * leftbtn;
        CAknFepCtrlRawKeyButton * rightbtn;
        leftbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowLeft));
        rightbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowRight));

        leftbtn->SetDimmed(aDimArrow);   
        rightbtn->SetDimmed(aDimArrow);   
        }                
    }
    
void CGenericItutWindowManager::ShowBubble(TInt aShow)
    {
    CFepCtrlDropdownList* spell = 
        static_cast<CFepCtrlDropdownList*>(iWindow->Control(ECtrlIdSpellCandsList));
    CFepCtrlDropdownList* standard = 
        static_cast<CFepCtrlDropdownList*>(iWindow->Control(ECtrlIdStdCandsList));
    CFepCtrlDropdownList* punctuation = 
        static_cast<CFepCtrlDropdownList*>(iWindow->Control(ECtrlIdPuncCandsList));
    
    if (aShow > 0)
        {
    	TRect outrect,innerrect;
    	outrect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EPreviewBubbleRect));  
    	innerrect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EPreviewBubbleInnerRect));
        TAknTextLineLayout bubbleTextLayout = 
            TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EPreviewBubbleTextlayout));
        CFont* bubbleFont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EPreviewBubbleFont));
        
        if( spell )
            {
            spell->ShowBubble(ETrue);
            spell->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview); 	
         	
        	spell->SetTextFormat(bubbleTextLayout);
        	spell->SetBubbleTextFont(bubbleFont);
        	spell->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
        	spell->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
        	               innerrect.iTl.iY - outrect.iTl.iY,
        	               outrect.iBr.iX - innerrect.iBr.iX,
        	               outrect.iBr.iY - innerrect.iBr.iY);
            }
        
        if( standard )
            {
            standard->ShowBubble(ETrue);
            standard->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview); 	
         	
        	standard->SetTextFormat(bubbleTextLayout);
        	standard->SetBubbleTextFont(bubbleFont);
        	standard->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
        	standard->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
        	               innerrect.iTl.iY - outrect.iTl.iY,
        	               outrect.iBr.iX - innerrect.iBr.iX,
        	               outrect.iBr.iY - innerrect.iBr.iY);
            }
    
        if ( punctuation )
            {
            punctuation->ShowBubble(ETrue);
            punctuation->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview); 	
         	
        	punctuation->SetTextFormat(bubbleTextLayout);
        	punctuation->SetBubbleTextFont(bubbleFont);
        	punctuation->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
        	punctuation->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
        	               innerrect.iTl.iY - outrect.iTl.iY,
        	               outrect.iBr.iX - innerrect.iBr.iX,
        	               outrect.iBr.iY - innerrect.iBr.iY);
            }
        }
    else
        {
        if( spell )
            {
            spell->ShowBubble(EFalse);
            }
        if( standard )
            {
            standard->ShowBubble(EFalse);
            }
        if ( punctuation )
            {
            punctuation->ShowBubble(EFalse);
            }
        }
    }    
/* Teleca change begin, 18.05.2009 ssal */
TInt CGenericItutWindowManager::HideByteWarningBubble(TAny* aPointer)
    {
    CGenericItutWindowManager* manager = static_cast<CGenericItutWindowManager*>(aPointer);
    manager->HideByteWarningBubble();
    return KErrNone;
    }
          
void CGenericItutWindowManager::HideByteWarningBubble()
    {
    if (!iInfoTimer)
        {
        return;
        }
        
    if (iInfoTimer->IsActive())
        {
        iInfoTimer->Cancel();
        iWindow->Icf()->HideInfoBubble();
        }
    }    
// End Of File
