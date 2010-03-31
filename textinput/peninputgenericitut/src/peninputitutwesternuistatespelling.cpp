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

#include <peninputrawkeybutton.h>

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
#include <peninputlayoutbubblectrl.h>

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
    CFepUiBaseCtrl* baseCtrl = iOwner->LayoutContext()->Control(ECtrlIdSwitch);
    if ( baseCtrl )
        {
        baseCtrl->Hide(ETrue);
        }
    iOwner->LayoutContext()->Control(ECtrlIdOptions)->Hide(ETrue);
    iOwner->LayoutContext()->Control(ECtrlIdClose)->Hide(ETrue);
    baseCtrl = iOwner->LayoutContext()->Control(ECtrlIdIndicator);
    if ( baseCtrl )
        {
        baseCtrl->Hide(ETrue);
        }
    // Hide Chinese Arrow Up button and Arrow Down button 
    CAknFepCtrlRawKeyButton* upbtn = 
        static_cast<CAknFepCtrlRawKeyButton *>(iOwner->LayoutContext()->Control(ECtrlIdArrowUp));
    CAknFepCtrlRawKeyButton* downbtn =  
        static_cast<CAknFepCtrlRawKeyButton *>(iOwner->LayoutContext()->Control(ECtrlIdArrowDown));

    if( upbtn )
        {
    	upbtn->Hide(ETrue);
        }
    
    if( downbtn )
    	{
    	downbtn->Hide(ETrue);
    	}
    //


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
    // Show ICF, Backspace after position changed to avoid flicker
    iOwner->LayoutContext()->Control(ECtrlIdBackspace)->Hide( EFalse );
    iOwner->LayoutContext()->Control(ECtrlIdICF)->Hide( EFalse );
    CAknFepCtrlLabel* indilabel = static_cast<CAknFepCtrlLabel*>
                                  (iOwner->LayoutContext()->Control(ECtrlIdSpellIndicator));
    // Modify begin
    if ( indilabel && iOwner->DataMgr()->IsChineseSpellMode())
    // Modify end
        {
        indilabel->Hide( EFalse );
        }
    iOwner->DataMgr()->SetUpdate(ETrue);
    }

void CWesternItutUiStateSpelling::OnExit()
    {
    CAknFepCtrlLabel* indilabel = static_cast<CAknFepCtrlLabel*>
                                  (iOwner->LayoutContext()->Control(ECtrlIdSpellIndicator));
    if ( indilabel )
        {
        indilabel->Hide( ETrue );
        }
    // Hide ICF, Backspace, Arrow contrls when exit to avoid flicker
    iOwner->LayoutContext()->ShowArrowBtn(0);
    iOwner->LayoutContext()->Control(ECtrlIdBackspace)->Hide( ETrue );
    iOwner->LayoutContext()->Control(ECtrlIdICF)->Hide( ETrue );
    iICF->SetPromptTextL( KNullDesC );
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
    iOwner->DataMgr()->SetChineseSpellFlag( EFalse );
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
    CFont* icffont;
    if ( iOwner->DataMgr()->IsPortraitWest() && 
            (!iOwner->DataMgr()->IsChineseSpellMode()))
        {
        icffont = TItutDataConverter::AnyToFont(iOwner->DataMgr()->RequestData(ESpellFont));
        }
    else
        {
        icffont = TItutDataConverter::AnyToFont(iOwner->DataMgr()->RequestData(EIcfFont));
        }
    TRect rect = TItutDataConverter::AnyToRect(iOwner->DataMgr()->RequestData(ESpellICFRect) );
    
    //rect.iTl.iY -= 50;
    iICF->SetTextMargin ( 2,
    					  2,
    					  2,
    					  2);
 
    iICF->SetLineSpace( 1 );
    if ( iOwner->DataMgr()->IsPortraitWest() && 
            (!iOwner->DataMgr()->IsChineseSpellMode()))
        {
        iICF->SizeChangedL(rect, 
                      iOwner->DataMgr()->iSpellIcfTextHeightForPrtWest,
                      icffont->FontMaxHeight(),
                      icffont);
        }
    else
        {
        iICF->SizeChangedL(rect, 
                      iOwner->DataMgr()->iIcfTextHeight,
                      icffont->FontMaxHeight(),
                      icffont);
        }

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
	if ( iOwner->DataMgr()->IsPortraitWest() && 
	        (!iOwner->DataMgr()->IsChineseSpellMode()))
	    {
		iOwner->LayoutContext()->Control(ECtrlIdArrowLeft)->Hide( EFalse );
		iOwner->LayoutContext()->Control(ECtrlIdArrowRight)->Hide( EFalse );
		
	    // left button
		CAknFepCtrlCommonButton* middleBtn = static_cast<CAknFepCtrlCommonButton*>(
									  iOwner->LayoutContext()->Control(ECtrlIdArrowLeft));
		TRect middleRect = TItutDataConverter::AnyToRect( 
				iOwner->DataMgr()->RequestData(ESpellArrowLeft));
		TRect middleInnerRect = TItutDataConverter::AnyToRect( 
							iOwner->DataMgr()->RequestData(ESpellArrowLeftInner));
		middleBtn->SizeChanged( middleRect, middleInnerRect, ETrue );
	
		// right button
		middleBtn = static_cast<CAknFepCtrlCommonButton*>(
									  iOwner->LayoutContext()->Control(ECtrlIdArrowRight));
		middleRect = TItutDataConverter::AnyToRect( 
				iOwner->DataMgr()->RequestData(ESpellArrowRight));
		middleInnerRect = TItutDataConverter::AnyToRect( 
							iOwner->DataMgr()->RequestData(ESpellArrowRightInner));
		middleBtn->SizeChanged( middleRect, middleInnerRect, ETrue );
		
		// Indicator bubble
        TRect bubbleRect = TItutDataConverter::AnyToRect( 
                iOwner->DataMgr()->RequestData( ESpellIndiPaneWithoutTextRect ));
        TRect iconRect = TItutDataConverter::AnyToRect( 
                iOwner->DataMgr()->RequestData( ESpellIndiIconWithoutTextRect ));
        TSize offset( 0, 6 );
        TSize size( iconRect.Width(), iconRect.Height());
        
        iICF->MsgBubbleCtrl()->SetRect( bubbleRect );
        iICF->MsgBubbleCtrl()->SetIconOffsetAndSize( offset, size );
        
        TBuf<100> text;
        iICF->MsgBubbleCtrl()->GetText( text );
        iICF->ShowBubble( text, iICF->MsgBubbleCtrl()->Rect());
	    }
	}
	
// End Of File
