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
#include <peninputgeneralitutnew.rsg>
#include <peninputlayoutmultilineicf.h>
#include <StringLoader.h>
#include <peninputcommonbutton.h>
#include <peninputlayout.h>
#include <peninputcmd.h>
#include <peninputlabel.h>

#include "peninputitutwesternuistatespelling.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutconverter.h"
#include "peninputitutcustomizationresourcemgr.h"
#include "peninputgenericitutlayout.h"
#include "peninputcommonbgctrl.h"
#include "peninputgenericitutwindowmanager.h"
#include "peninputgenericitutwindow.h"

CWesternItutUiStateSpelling* CWesternItutUiStateSpelling::NewL(CGenericItutUiMgrBase* aOwner)
    {
    CWesternItutUiStateSpelling* self = new (ELeave) CWesternItutUiStateSpelling(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CWesternItutUiStateSpelling::~CWesternItutUiStateSpelling()
    {
    delete iRscMgr;
    }

CWesternItutUiStateSpelling::CWesternItutUiStateSpelling(CGenericItutUiMgrBase* aOwner)
                                                        :CGenericItutUiStateBase(aOwner)
    {
    iICF = static_cast<CFepLayoutMultiLineIcf*>(iOwner->LayoutContext()->Control(ECtrlIdICF));
    }

void CWesternItutUiStateSpelling::ConstructL()
    {
    CGenericItutUiStateBase::ConstructL();
    iRscMgr = CItutCustomizedRscMgr::NewL(iOwner->LayoutContext()->UiLayout(), 
                                          iOwner->LayoutContext());
    }

void CWesternItutUiStateSpelling::CreateTextBtnIfNeededL(CAknFepCtrlCommonButton*& aBtn, 
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

void CWesternItutUiStateSpelling::OnEntryL()
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
    // Show background of the spell popup window
    CAknFepCtrlCommonBgCtrl* bgCtrl = static_cast<CAknFepCtrlCommonBgCtrl*>
                                      (iOwner->LayoutContext()->Control(ECtrlIdBackground) );
    if(bgCtrl)
		{
		bgCtrl->ShowSubBgItem( 0, ETrue );	
		}

    // update resource id of keypad
    iOwner->DataMgr()->NotifyChangeControlLayout(
        MItutPropertySubscriber::EItutPropertyKeypadResourceId, keypadResId);

    // hide not needed controls
    iOwner->LayoutContext()->Control(ECtrlIdSwitch)->Hide(ETrue);
    iOwner->LayoutContext()->Control(ECtrlIdArrowLeft)->Hide(ETrue);
    iOwner->LayoutContext()->Control(ECtrlIdArrowRight)->Hide(ETrue);
    iOwner->LayoutContext()->Control(ECtrlIdOptions)->Hide(ETrue);
    iOwner->LayoutContext()->Control(ECtrlIdClose)->Hide(ETrue);
    iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(ETrue);

    TAknTextLineLayout btntextformat = TItutDataConverter::AnyToTextLine(
                                               iOwner->DataMgr()->RequestData(EBtnTextLine));
    TAknTextLineLayout btntextCancel = TItutDataConverter::AnyToTextLine(
                                               iOwner->DataMgr()->RequestData(ESpellTextCancle));
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
    CAknFepCtrlLabel* indilabel = static_cast<CAknFepCtrlLabel*>
                                      (iOwner->LayoutContext()->Control(ECtrlIdSpellIndicator));	
    indilabel->Hide( EFalse );
    iOwner->DataMgr()->SetUpdate(ETrue);
    }

void CWesternItutUiStateSpelling::OnExit()
    {
    CAknFepCtrlLabel* indilabel = static_cast<CAknFepCtrlLabel*>
    								                  (iOwner->LayoutContext()->Control(ECtrlIdSpellIndicator));
    indilabel->Hide( ETrue );
    iOk->Hide(ETrue);
    iCancel->Hide(ETrue);
    iOwner->DataMgr()->SetSpellMode(EFalse);
    iICF->SetLineSeparatorAfterPrompt(EFalse);
    // Hide background of the spell popup window
    CAknFepCtrlCommonBgCtrl* bgCtrl = static_cast<CAknFepCtrlCommonBgCtrl*>
                                      (iOwner->LayoutContext()->Control(ECtrlIdBackground) );
    if(bgCtrl)
		{
		bgCtrl->ShowSubBgItem( 0, EFalse );	
		}    
    }

CGenericItutUiMgrBase::TUiState CWesternItutUiStateSpelling::StateType()
    {
    return CGenericItutUiMgrBase::EStateSpelling;
    }

TInt CWesternItutUiStateSpelling::HandleCommandL(TInt aCmd, TUint8* /*aData*/)
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

TBool CWesternItutUiStateSpelling::HandleCtrlEventL(TInt aEventType, 
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

void CWesternItutUiStateSpelling::ReCalcLayoutL()
	{
    TAknTextLineLayout btntextformat = TItutDataConverter::AnyToTextLine(
                                                 iOwner->DataMgr()->RequestData(EBtnTextLine));
    TAknTextLineLayout btntextCancel = TItutDataConverter::AnyToTextLine(
                                                 iOwner->DataMgr()->RequestData(ESpellTextCancle));
    TRect btnrect = TItutDataConverter::AnyToRect(iOwner->DataMgr()->RequestData(EItutPosOk));
    iOk->SetTextFormat(btntextformat);
    iOk->SizeChanged(btnrect, btnrect, ETrue);

    btnrect = TItutDataConverter::AnyToRect(iOwner->DataMgr()->RequestData(EItutPosCancel));
    iCancel->SetTextFormat(btntextCancel);
    iCancel->SizeChanged(btnrect, btnrect, ETrue);
    //iICF->SetRect( TItutDataConverter::AnyToRect(iOwner->DataMgr()->RequestData(ESpellICFRect)) );
    CFont* icffont = TItutDataConverter::AnyToFont(iOwner->DataMgr()->RequestData(EIcfFont));
    TRect rect = TItutDataConverter::AnyToRect(iOwner->DataMgr()->RequestData(ESpellICFRect) );
    
    //rect.iTl.iY -= 50;
    iICF->SetTextMargin ( 2,
    					  2,
    					  2,
    					  2);
 
    iICF->SetLineSpace( 1 );
    iICF->SizeChangedL(rect, 
                  //icffont->HeightInPixels(),
                  iOwner->DataMgr()->iIcfTextHeight,
                  icffont->FontMaxHeight(),
                  icffont);

    CAknFepCtrlCommonButton* clrBtn = static_cast<CAknFepCtrlCommonButton*>(
									  iOwner->LayoutContext()->Control(ECtrlIdBackspace) );
	
	// When size changed in r-to-l language, it should be mirrored
	TRect rt = TItutDataConverter::AnyToRect( iOwner->DataMgr()->RequestData(ESpellClrRect) );
	TRect innerRect = TItutDataConverter::AnyToRect( 
						iOwner->DataMgr()->RequestData(ESpellClrInnerRect) );
    TBool bSizedChanged = (rt.Size() != clrBtn->Rect().Size()) && 
	    (innerRect.Size() != clrBtn->ForgroundBmpRect().Size());									  
    clrBtn->SizeChanged( rt, innerRect, ETrue );
	
	if( bSizedChanged && iOwner->DataMgr()->IsRtoLLanguage() )
		{
		iOwner->UiManager()->Window()->HandleButtonResOnLangDirChange( ECtrlIdBackspace );			
		}
	}
	
// End Of File
