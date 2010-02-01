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

#include "peninputgenericitutlayout.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutuistatebase.h"
#include "peninputitutchnuimgr.h"
#include "peninputitutwesternuimgr.h"
#include "peninputgenericitutconverter.h"
#include "peninputgenericitutwindowmanager.h"



CGenericItutUiLayout* CGenericItutUiLayout::NewL(MLayoutOwner* aLayoutOwner,
                                                 const TAny* aInitData)
    {
    CGenericItutUiLayout* layout = new (ELeave) CGenericItutUiLayout(aLayoutOwner);

    CleanupStack::PushL(layout);
    layout->ConstructL(aInitData);
    CleanupStack::Pop(layout);

    return layout;
    }

CGenericItutUiLayout::CGenericItutUiLayout(MLayoutOwner* aLayoutOwner)
                                          :CFepUiLayout(aLayoutOwner)
    {
    }

CGenericItutUiLayout::~CGenericItutUiLayout()
    {
    delete iDataMgr;
    delete iChnUiMgr;
    delete iWesternUiMgr;
    delete iWindowMgr;
    }

void CGenericItutUiLayout::ConstructL(const TAny* aInitData)
    {
    BaseConstructL();

    TPeninputLayoutData* layoutData = (TPeninputLayoutData*)aInitData;
    
    iDataMgr = CGenericItutDataMgr::NewL(this, layoutData->iPtiEngine );
    

    SetScreenSize(TItutDataConverter::AnyToSize(iDataMgr->RequestData(EScreenSize)));

    SetRect(TItutDataConverter::AnyToRect(iDataMgr->RequestData(ELayoutRect)));
   
    // western ui mgr is default ui mgr
    iWindowMgr = CGenericItutWindowManager::NewL(this, iDataMgr);
    iDataMgr->AddSubscriber(iWindowMgr);    
    iWesternUiMgr = CWesternItutUiMgr::NewL(this, iDataMgr, iWindowMgr);
    }


void CGenericItutUiLayout::SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType)
    {
    TRawEvent event; 
    event.Set(aType,aScanCode);
    
    TPtrC eventPtr(reinterpret_cast<const TUint16*>(&event),sizeof(event)/sizeof(TUint16));

    SignalOwner(ESignalSimulateEvent,eventPtr);
    }

// ---------------------------------------------------------------------------
// Simulate raw event to window server immediately.
// simulating raw event command won't be scheduled by AO 
// because it won't be added into CPeninputAnimObj::iAnimCmd.
// ---------------------------------------------------------------------------
//    
void CGenericItutUiLayout::SimulateImmeRawEvent( TInt aScanCode, TRawEvent::TType aType )
    {
    TRawEvent event; 
    event.Set(aType,aScanCode);    
    TPtrC eventPtr(reinterpret_cast<const TUint16*>(&event),sizeof(event)/sizeof(TUint16));
    SignalOwner( ESignalImmeSimulateEvent, eventPtr );   	
    }

void CGenericItutUiLayout::HandleControlEvent(TInt aEventType, 
                                              CFepUiBaseCtrl* aCtrl, 
                                              const TDesC& aEventData)
    {
    TBool handled = EFalse;

    if (UiMgr())
        {
        TRAP_IGNORE(handled = UiMgr()->HandleCtrlEventL(aEventType,
                                                        aCtrl,
                                                        aEventData));
        }

    if (handled)
        {
        return;   
        }

    TRAP_IGNORE(iWindowMgr->HandleCtrlEventL(aEventType,
                                             aCtrl,
                                             aEventData));
   }

TInt CGenericItutUiLayout::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
    {
    TInt err = iWindowMgr->OnAppEditorTextComing(aData);

    if (UiMgr()->CurrentState() &&
        UiMgr()->CurrentState()->StateType() == CGenericItutUiMgrBase::EStateSpelling)
        {
        TRAP_IGNORE(UiMgr()->HandleCommandL(EItutCmdCheckIcfEmpty, NULL));
        }
 
     return err;
    }


TInt CGenericItutUiLayout::HandleCommand(TInt aCmd, TUint8* aData)
    {
    if ( KErrNone == CFepUiLayout::HandleCommand( aCmd, aData ) )
        {
        return KErrNone;
        }

    TUint* data = (TUint*) aData;

    switch(aCmd)
        {
        case ECmdPenInputLanguage:
            {
            TRAP_IGNORE(SetInputLanguageL(*data));
            
            }
            break;
        case ECmdPenInputRange:
        	{
        	iDataMgr->SetLatinOnly( *data );
        	}
        	break;
        case ECmdPenInputFingerInputMode:
            {
            TRAP_IGNORE(SetInputModeL(*data));
            }
            break;
        case ECmdPenInputFingerKeyPress:
        case ECmdPenInputFingerLongKeyPress:
        case EItutExtCmdCustomerState:
        case EItutExtCmdSetLayout:
        case EItutExtCmdDimKey:
        case EItutExtCmdHideKey:
        case EItutExtCmdAddKey:
        case EItutExtCmdSizeChanged:
        case EItutExtCmdSetIcfData:
        case ECmdPenInputCase:
        case ECmdPenInputFingerMatchSelection:
        case ECmdPenInputFingerSpelling:
            {
            if (UiMgr())
                {
                TInt handled = KErrNone;
                TRAP_IGNORE(handled = UiMgr()->HandleCommandL(aCmd, aData));
                return handled;
                }
            }
            break;
        case ECmdPenInputFingerMatchList:
            {
            if (UiMgr())
                {
                TInt handled = KErrNone;
                TRAP_IGNORE(handled = UiMgr()->HandleCommandL(aCmd, aData));
                return handled;
                }
            }
            break;
        case ECmdPenInputSetTextAlignment:
            {
            iDataMgr->SetIcfTextAlignment(*data);
            iDataMgr->SetTextAlignment();
            }
            break;
        case ECmdPenInputNoFreeSpace:
        	{
            TRAP_IGNORE(ChineseUiManager()->HandleCommandL(ECmdPenInputNoFreeSpace, NULL));
        	}
        case ECmdPenInputSetPromptText:        	
        case ECmdPenInputFingerMatchIndicator:
        case ECmdPenInputSendEditMenuData:
        case ECmdPenInputEnableSettingBtn:
        case ECmdPenInputDimArrowKeys:
        case ECmdPenInputRotation:
            {
            TRAP_IGNORE(iWindowMgr->HandleCommandL(aCmd, aData));
            }
            break;
        case ECmdPenInputIsSecretText:
            {
            TRAP_IGNORE(iWindowMgr->HandleCommandL(aCmd, aData));
            *data ? iIsSecret = ETrue: iIsSecret = EFalse;             
             if (UiMgr())
                {
                TInt handled = KErrNone;
                TRAP_IGNORE(handled = UiMgr()->HandleCommandL(aCmd, aData));
                return handled;
                }
            }
        break;
        case ECmdPenInputEnableIndicatorButton:
            {
            TRAP_IGNORE(iWindowMgr->HandleCommandL(aCmd, aData));
            } 
        break;
        case ECmdPenInputSymbolOfHardKeyOne:
        case ECmdPenInputInEditWordQueryDlg:
            {
            TRAP_IGNORE(iWindowMgr->HandleCommandL(aCmd, aData));
            }
            break;
        case ECmdPenInputIsNumberGrouping:
            {
            TRAP_IGNORE(iWindowMgr->HandleCommandL(aCmd, aData));
            }
            break;
        case ECmdPenInputEditorNumericKeyMap:
        	{
        	iDataMgr->SetNumericKeymap( *data );
        	}
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
        	{
        	iDataMgr->SetNumericKeymapData( (TInt*) data );
        	}
        	break;
        case ECmdPenInputPermittedRange:
        	{
        	iDataMgr->SetNumericOnly( *data );
        	}
        	break;
        case ECmdPenInputCharacterPreview:
            {
            iWindowMgr->ShowBubble(*aData);    
            }
            break;
        default:
            break;
        }

    return KErrNone;
    }

TInt CGenericItutUiLayout::PenInputType()
    {
    return EPluginInputModeItut;
    }

TInt CGenericItutUiLayout::SizeChanged(const TAny* /*pData*/)
    {
    TBool overLap = UiMgr()->CurrentState()->StateType() 
        == CGenericItutUiMgrBase::EStateMatchSelection ? ETrue: EFalse;
        
    iDataMgr->ReadLafInfo();

    // set layout rect
    SetRect(TItutDataConverter::AnyToRect(iDataMgr->RequestData(ELayoutRect)));
        
    iWindowMgr->SizeChanged(overLap);

    return KErrNone;    
    }

void CGenericItutUiLayout::SubmitText(const TDesC& aEventData)
    {
    SignalOwner(ESignalKeyEvent, aEventData);
    }

TInt CGenericItutUiLayout::OnSkinChange()
    {
    CFepUiLayout::OnSkinChange();
    
    TInt err = iWindowMgr->OnSkinChange();

    return err;
    }


TBool CGenericItutUiLayout::PressOnCtrls(TPoint aPt)
    {
    const RPointerArray<CFepUiBaseCtrl>& ctrllist = RootControl()->ControlList();

    for (TInt i = 0; i < ctrllist.Count(); i++)
        {
        if (!ctrllist[i]->Hiden() && ctrllist[i]->Rect().Contains(aPt))
            {
            return ETrue;
            }
        }

    return EFalse;    
    }

CGenericItutUiMgrBase* CGenericItutUiLayout::UiMgr()
    {
    if (iDataMgr->IsChinese())
       {
       iCurrentUiMgr = ChineseUiManager();
       }
    else
       {
       iCurrentUiMgr = iWesternUiMgr;
       }

    return iCurrentUiMgr;        
    }
    
void CGenericItutUiLayout::SetInputModeL(TInt aMode)
    {
    // fix EJML-7HM6GM, e.g. both EStrokeFind and EStroke will be stored as EStroke
    if (aMode != iDataMgr->InputMode() || 
        aMode == EStroke || aMode == EStrokeFind ||
        aMode == EZhuyin || aMode == EZhuyinFind ||
        ( iDataMgr->IsNumericOnly() && iDataMgr->IsCharFlagChanged() ) )
        {
        // deactivate original uimgr first
        if (iCurrentUiMgr 
            && iCurrentUiMgr->CurrentState()
            && iCurrentUiMgr->CurrentState()->StateType() != CGenericItutUiMgrBase::EStateSpelling)
            {
            iCurrentUiMgr->DeactivateUI();
            }

        iDataMgr->SetInputModeL(aMode);

        UiMgr()->ActivateUiL();
        }
    }

void CGenericItutUiLayout::SetInputLanguageL(TInt aLanguage)
    {
    TInt previousLang = iDataMgr->InputLanguage();
    iDataMgr->SetLanguageL(aLanguage);
    iWindowMgr->SetLanguageL(aLanguage);
    
    if (previousLang != aLanguage)
        {
        ApplyVariantLafDataL();   
        }
    }

void CGenericItutUiLayout::OnDeActivate()
    {
    TRAP_IGNORE(UiMgr()->HandleCommandL(ECmdPenInputDeActive, NULL));

    CFepUiLayout::OnDeActivate();
    }
void CGenericItutUiLayout::OnActivate()
    {
    CFepUiLayout::OnActivate();
     
    iWindowMgr->OnActivate();
    }

CFepUiLayout* CGenericItutUiLayout::UiLayout()
    {
    return this;
    }

   
void CGenericItutUiLayout::EnterMatchSelectionState(TBool aOn)
    {
    TBuf<4> buf;
    buf.Append(reinterpret_cast<TText*>(&aOn), sizeof(TBool)/sizeof(TText));
    SignalOwner(ESignalEnterMatchSelection, buf);
    }

void CGenericItutUiLayout::HandleAppInfoChange(const TDesC& aInfo, 
                                               TPeninputAppInfo aType)
    {
    if ( aType == EAppIndicatorMsg )
        {
        TRAP_IGNORE(iWindowMgr->HandleAppInfoChangeL(aInfo));
        }
    /* Teleca change begin, 19.05.2009 ssal */    
    if ( aType == EAppByteWarningMsg || aType == EAppMessageTypeChangeMsg)
        {
        TRAP_IGNORE(iWindowMgr->DisplayWarningNote(aInfo));
        } 
    /* Teleca change end, 19.05.2009 ssal */       
    }
    
void CGenericItutUiLayout::ShowArrowBtn(TInt aShowFlag)
    {
    iWindowMgr->ShowArrowBtn(aShowFlag);    
    }
    
void CGenericItutUiLayout::ApplyVariantLafDataL(TBool aResolutionChange)
    {
    iWindowMgr->ApplyVariantLafDataL(aResolutionChange);
    }
    
CGenericItutWindowManager* CGenericItutUiLayout::UiManager()
    {
    return iWindowMgr;   
    }

CFepUiBaseCtrl* CGenericItutUiLayout::Control(TInt aCtrlId)
    {
    return iWindowMgr->Control(aCtrlId); 
    }

CGenericItutUiMgrBase* CGenericItutUiLayout::ChineseUiManager()
    {
    if (!iChnUiMgr)
        {
        TRAP_IGNORE( iChnUiMgr = CChnItutUiMgr::NewL(this, iDataMgr, iWindowMgr) );
        }
        
    return iChnUiMgr;
        
    }
    
// End Of File
