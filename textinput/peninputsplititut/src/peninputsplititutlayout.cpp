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
* Description:  split itu-t layout
*
*/


#include <peninputsplititutnew.rsg>
#include <peninputsplititutconfig_chinese.rsg>
#include <skinlayout.cdl.h>
#include <AknsDrawUtils.h>
#include <AknsUtils.h>
#include <AknUtils.h> 
#include <peninputrawkeybutton.h>
#include <peninputrepeatbutton.h>
#include <peninputcommonbutton.h>
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
//#include <peninputdropdownlist.h>

#include <s32mem.h>
//#include <peninputlayoutbubblectrl.h>

#include <peninputdataprovider.h>      
#include <peninputcommonlayoutglobalenum.h>

#include "peninputsplititutlayout.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutuimgrbase.h"
#include "peninputsplititutuistatebase.h"
#include "peninputsplititutchnuimgr.h"
#include "peninputsplititutwesternuimgr.h"
#include "peninputsplititutconverter.h"
#include "peninputsplititutwindowmanager.h"


// ---------------------------------------------------------------------------
// CSplitItutUiLayout::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutUiLayout* CSplitItutUiLayout::NewL(MLayoutOwner* aLayoutOwner,
                                                 const TAny* aInitData)
    {
    CSplitItutUiLayout* layout = new (ELeave) CSplitItutUiLayout(aLayoutOwner);

    CleanupStack::PushL(layout);
    layout->ConstructL(aInitData);
    CleanupStack::Pop(layout);

    return layout;
    }
    
// ---------------------------------------------------------------------------
// CSplitItutUiLayout::CSplitItutUiLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutUiLayout::CSplitItutUiLayout(MLayoutOwner* aLayoutOwner)
                                          :CFepUiLayout(aLayoutOwner)
    {
    }

// ---------CSplitItutUiLayout----------------------------------------
// CSplitItutUiLayout::~CSplitItutUiLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutUiLayout::~CSplitItutUiLayout()
    {
    delete iDataMgr;
    delete iChnUiMgr;
    delete iWesternUiMgr;
    delete iWindowMgr;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::OnAppEditorTextComing
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutUiLayout::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
    {
	TInt err = 0;
	if ( iDataMgr->IsSpellMode())
		{
	    // Update the icf text
	    err = iWindowMgr->OnAppEditorTextComing( aData );
		}

	// notify icf has text or not
    if ( UiMgr()->CurrentState() &&
         UiMgr()->CurrentState()->StateType() == CSplitItutUiMgrBase::EStateSpelling )
        {
        TRAP_IGNORE(UiMgr()->HandleCommandL(EItutCmdCheckIcfEmpty, NULL));
        }
 
     return err;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::ConstructL(const TAny* aInitData)
    {
    BaseConstructL();

    TPeninputLayoutData* layoutData = (TPeninputLayoutData*)aInitData;
    
    iDataMgr = CSplitItutDataMgr::NewL(this, layoutData->iPtiEngine );
    SetScreenSize(TItutDataConverter::AnyToSize(iDataMgr->RequestData(EScreenSize)));

    // western ui mgr is default ui mgr
    iWindowMgr = CSplitItutWindowManager::NewL(this, iDataMgr);
    iDataMgr->AddSubscriber(iWindowMgr);    
    iWesternUiMgr = CWesternSplitItutUiMgr::NewL(this, iDataMgr, iWindowMgr);
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::SimulateRawEvent
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType)
    {
    TRawEvent event; 
    event.Set(aType,aScanCode);
    
    TPtrC eventPtr(reinterpret_cast<const TUint16*>(&event),sizeof(event)/sizeof(TUint16));

    SignalOwner(ESignalSimulateEvent,eventPtr);
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::HandleControlEvent
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::HandleControlEvent(TInt aEventType, 
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

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::HandleCommand
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutUiLayout::HandleCommand(TInt aCmd, TUint8* aData)
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
//            iWindowMgr->ShowBubble(*aData);    
            }
            break; 
            
        default:
            break;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::PenInputType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutUiLayout::PenInputType()
    {
    return EPluginInputModeItut;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::SizeChanged
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutUiLayout::SizeChanged(const TAny* /*pData*/)
    {
    iDataMgr->ReadLafInfo();

    // set layout rect
    SetRect(TItutDataConverter::AnyToRect(iDataMgr->RequestData(ELayoutRect)));
        
    iWindowMgr->SizeChanged();

    return KErrNone;    
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::SubmitText
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::SubmitText(const TDesC& aEventData)
    {
    SignalOwner(ESignalKeyEvent, aEventData);
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::OnSkinChange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutUiLayout::OnSkinChange()
    {
    CFepUiLayout::OnSkinChange();
    
    TInt err = iWindowMgr->OnSkinChange();

    return err;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::PressOnCtrls
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutUiLayout::PressOnCtrls(TPoint aPt)
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

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::UiMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutUiMgrBase* CSplitItutUiLayout::UiMgr()
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
    
// ---------------------------------------------------------------------------
// CSplitItutUiLayout::SetInputModeL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::SetInputModeL(TInt aMode)
    {
    // both EStrokeFind and EStroke will be stored as EStroke
    if (aMode != iDataMgr->InputMode() || 
        aMode == EStroke || aMode == EStrokeFind ||
        aMode == EZhuyin || aMode == EZhuyinFind ||
        ( iDataMgr->IsNumericOnly() && iDataMgr->IsCharFlagChanged() ) )
        {
        // deactivate original uimgr first
        iDataMgr->SetInputModeL(aMode);

        UiMgr()->ActivateUiL();
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::SetInputLanguageL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::SetInputLanguageL(TInt aLanguage)
    {
    TInt previousLang = iDataMgr->InputLanguage();
    iDataMgr->SetLanguageL(aLanguage);
    
    if (previousLang != aLanguage)
        {
        ApplyVariantLafDataL();   
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::OnDeActivate
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::OnDeActivate()
    {
    TRAP_IGNORE(UiMgr()->HandleCommandL(ECmdPenInputDeActive, NULL));

    CFepUiLayout::OnDeActivate();
    }
    
// ---------------------------------------------------------------------------
// CSplitItutUiLayout::OnActivate
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::OnActivate()
    {
    CFepUiLayout::OnActivate();

    SetRect(TItutDataConverter::AnyToRect(iDataMgr->RequestData(ELayoutRect)));
    TPoint offset = TItutDataConverter::AnyToPoint(iDataMgr->RequestData(ELayoutOffset));
    
    if(iDataMgr->IsSpellMode())
        {
        offset = TPoint(0,0);
        }
    
    LayoutOwner()->SetPosition( offset );   

    iWindowMgr->OnActivate();
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::UiLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CFepUiLayout* CSplitItutUiLayout::UiLayout()
    {
    return this;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::HandleAppInfoChange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::HandleAppInfoChange(const TDesC& aInfo, 
                                               TPeninputAppInfo aType)
    {
    if ( aType == EAppIndicatorMsg )
        {
        TRAP_IGNORE(iWindowMgr->HandleAppInfoChangeL(aInfo));
        }
    }
    
// ---------------------------------------------------------------------------
// CSplitItutUiLayout::ShowArrowBtn
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::ShowArrowBtn(TInt aShowFlag)
    {
    iWindowMgr->ShowArrowBtn(aShowFlag);    
    }
    
// ---------------------------------------------------------------------------
// CSplitItutUiLayout::ApplyVariantLafDataL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::ApplyVariantLafDataL(TBool aResolutionChange)
    {
    iWindowMgr->ApplyVariantLafDataL(aResolutionChange);
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::ApplyVariantLafDataForSpellL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiLayout::ApplyVariantLafDataForSpellL()
    {
    iWindowMgr->ApplyVariantLafDataForSpellL();
    }
    
// ---------------------------------------------------------------------------
// CSplitItutUiLayout::UiManager
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutWindowManager* CSplitItutUiLayout::UiManager()
    {
    return iWindowMgr;   
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::Control
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CSplitItutUiLayout::Control(TInt aCtrlId)
    {
    return iWindowMgr->Control(aCtrlId); 
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::ChineseUiManager
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutUiMgrBase* CSplitItutUiLayout::ChineseUiManager()
    {
    if (!iChnUiMgr)
        {
        TRAP_IGNORE( iChnUiMgr = CChnSplitItutUiMgr::NewL(this, iDataMgr, iWindowMgr) );
        }
        
    return iChnUiMgr;
        
    }
    
// End Of File
