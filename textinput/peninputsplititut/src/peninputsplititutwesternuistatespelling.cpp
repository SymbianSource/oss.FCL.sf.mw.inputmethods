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


#include <AknFepGlobalEnums.h>
#include <peninputsplititutnew.rsg>
#include <peninputlayoutmultilineicf.h>
#include <StringLoader.h>
#include <peninputcommonbutton.h>
#include <peninputlayout.h>
#include <peninputcmd.h>
#include <peninputlabel.h>

#include "peninputsplititutwesternuistatespelling.h"
#include "peninputsplititutuimgrbase.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutlayoutcontext.h"
#include "peninputsplititutconverter.h"
#include "peninputsplititutlayout.h"
#include "peninputcommonbgctrl.h"
#include "peninputsplititutwindowmanager.h"
#include "peninputsplititutwindow.h"

CWesternSplitItutUiStateSpelling* CWesternSplitItutUiStateSpelling::NewL(CSplitItutUiMgrBase* aOwner)
    {
    CWesternSplitItutUiStateSpelling* self = new (ELeave) CWesternSplitItutUiStateSpelling(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CWesternSplitItutUiStateSpelling::~CWesternSplitItutUiStateSpelling()
    {
    }

CWesternSplitItutUiStateSpelling::CWesternSplitItutUiStateSpelling(CSplitItutUiMgrBase* aOwner)
                                                        :CSplitItutUiStateBase(aOwner)
    {
    iICF = static_cast<CFepLayoutMultiLineIcf*>(iOwner->LayoutContext()->Control(ECtrlIdICF));
    }

void CWesternSplitItutUiStateSpelling::ConstructL()
    {
    CSplitItutUiStateBase::ConstructL();
    }

void CWesternSplitItutUiStateSpelling::CreateTextBtnIfNeededL(CAknFepCtrlCommonButton*& aBtn, 
                                                         TInt aCtrlId,
                                                         TInt aCtrlPos,
                                                         TInt aTextResId,
                                                         TAknTextLineLayout aTextFormat)
    {
    if (!aBtn)
        {
        aBtn = CAknFepCtrlCommonButton::NewL(iOwner->LayoutContext()->UiLayout(), 
                                             aCtrlId,
                                             KAknsIIDQsnFrFunctionButtonNormal,
                                             KAknsIIDQsnFrFunctionButtonPressed,
                                             KAknsIIDQsnFrFunctionButtonInactive);
        aBtn->SetRect(TItutDataConverter::AnyToRect(iOwner->DataMgr()->RequestData(aCtrlPos)));
        HBufC* btntext = StringLoader::LoadLC(aTextResId);
        aBtn->SetTextFormat(aTextFormat);
        aBtn->SetTextL(*btntext);
        aBtn->SetTextColorIndex( EAknsCIQsnTextColorsCG65 );
        CleanupStack::PopAndDestroy(btntext);
        iOwner->LayoutContext()->UiLayout()->AddControlL(aBtn);
        }
    }

void CWesternSplitItutUiStateSpelling::OnEntryL()
    {
    TInt keypadResId = KInvalidResId;
    switch (iOwner->DataMgr()->InputMode())
        {
        case ENumber:
            keypadResId = R_FINGER_INPUT_KEYPAD_NUMBER;
            break;
        case ELatin:
            keypadResId = iOwner->DataMgr()->KeypadResourceId();
            break;
        default:
            break;                
        }
    //hide bubble in the first place.    
    iICF->HideBubble();

    // update resource id of keypad    
    iOwner->DataMgr()->NotifyChangeControlLayout(
        MItutPropertySubscriber::EItutPropertyKeypadResourceId, keypadResId);

    // hide not needed controls
    //iOwner->LayoutContext()->Control(ECtrlIdArrowLeft)->Hide(ETrue);
    //iOwner->LayoutContext()->Control(ECtrlIdArrowRight)->Hide(ETrue);
    iOwner->LayoutContext()->ShowArrowBtn( EBtnArrowLeft | EBtnArrowRight );
    iOwner->LayoutContext()->Control(ECtrlIdOptions)->Hide(ETrue);
    iOwner->LayoutContext()->Control(ECtrlIdClose)->Hide(ETrue);
    
    TAknTextLineLayout btntextformat = TItutDataConverter::AnyToTextLine(
                                               iOwner->DataMgr()->RequestData(EBtnTextLine));
    CreateTextBtnIfNeededL(iOk, ECtrlIdOkBtn, EItutPosOk, R_PENINPUT_FINGER_OK, btntextformat);
    iOk->SetReady(ETrue);
    iOk->Hide(EFalse);
    iICF->IsEmpty() ? iOk->SetDimmed(ETrue) : iOk->SetDimmed(EFalse);

    CreateTextBtnIfNeededL(iCancel, ECtrlIdCancelBtn, EItutPosCancel, 
                           R_PENINPUT_FINGER_CANCEL, btntextformat);
    iCancel->SetReady(ETrue);
    iCancel->Hide(EFalse);

    iOwner->DataMgr()->SetSpellMode(ETrue);
    iICF->SetLineSeparatorAfterPrompt(ETrue);
    
    ReCalcLayoutL();

    iICF->Hide( EFalse );
    iOwner->DataMgr()->SetUpdate(ETrue);
    }

void CWesternSplitItutUiStateSpelling::OnExit()
    {    
    iOk->Hide(ETrue);
    iCancel->Hide(ETrue);
    iOwner->DataMgr()->SetSpellMode(EFalse);
    iICF->Hide( ETrue );
    iOwner->LayoutContext()->ShowArrowBtn(0);
    }

CSplitItutUiMgrBase::TUiState CWesternSplitItutUiStateSpelling::StateType()
    {
    return CSplitItutUiMgrBase::EStateSpelling;
    }

TInt CWesternSplitItutUiStateSpelling::HandleCommandL(TInt aCmd, TUint8* /*aData*/)
    {
    switch (aCmd) 
        {
        case EItutExtCmdSizeChanged:
            {
			ReCalcLayoutL();
            return KErrNone;
            }
        
       	case EItutCmdCheckIcfEmpty:
       	    {
		    iICF->IsEmpty() ? iOk->SetDimmed(ETrue) : iOk->SetDimmed(EFalse);
            return KErrNone;
       	    }
        default:
            break;
        }

    return KErrNotSupported;    
    }

TBool CWesternSplitItutUiStateSpelling::HandleCtrlEventL(TInt aEventType, 
                                                    CFepUiBaseCtrl* aCtrl, 
                                                    const TDesC& /*aEventData*/)
    {
    switch (aEventType)
        {
        case EEventButtonUp:
            {
            TInt ctrlid = aCtrl->ControlId();

            if (ctrlid != ECtrlIdOkBtn && ctrlid != ECtrlIdCancelBtn)
                {
                return EFalse;
                }

            TBuf<4> buf;
            TBool exitbyok = (ctrlid == ECtrlIdOkBtn);
            buf.Append(reinterpret_cast<TText*>(&exitbyok), sizeof(TBool)/sizeof(TText));
            
            iOwner->LayoutContext()->UiLayout()->SignalOwner(ESignalExitSpellMode, buf);
            return ETrue;
            }
        default:
            break;
        }

    return EFalse;    
    }

void CWesternSplitItutUiStateSpelling::ReCalcLayoutL()
	{
	// Update "OK" and "Cancel" position
    TAknTextLineLayout btntextformat = TItutDataConverter::AnyToTextLine(
                                                 iOwner->DataMgr()->RequestData(EBtnTextLine));
    TRect btnrect = TItutDataConverter::AnyToRect(iOwner->DataMgr()->RequestData(EItutPosOk));
    iOk->SetTextFormat(btntextformat);
    iOk->SizeChanged(btnrect, btnrect, ETrue);

    btnrect = TItutDataConverter::AnyToRect(iOwner->DataMgr()->RequestData(EItutPosCancel));
    iCancel->SetTextFormat(btntextformat);
    iCancel->SizeChanged(btnrect, btnrect, ETrue);

    // Update key position
    iOwner->LayoutContext()->ApplyVariantLafDataForSpellL();	
	}
	
// End Of File
