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
#include <peninputselectionlist.h>
#include <peninputlayoutmultilineicf.h>
#include <e32keys.h>
#include <peninputlayout.h>
#include <peninputcommonbutton.h>
#include <peninputrawkeybutton.h>

#include "peninputitutwesternuistatenonpredict.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutwindowmanager.h"
#include <peninputlayoutbubblectrl.h>

CWesternItutUiStateNonPredict* CWesternItutUiStateNonPredict::NewL(CGenericItutUiMgrBase* aOwner)
    {
    CWesternItutUiStateNonPredict* self = new (ELeave) CWesternItutUiStateNonPredict(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CWesternItutUiStateNonPredict::~CWesternItutUiStateNonPredict()
    {
    }

CWesternItutUiStateNonPredict::CWesternItutUiStateNonPredict(CGenericItutUiMgrBase* aOwner)
                                                            :CGenericItutUiStateBase(aOwner),
                                                             iConsumeLastKeyDown(EFalse)
    {
    iIcf = static_cast<CFepLayoutMultiLineIcf*>(iOwner->LayoutContext()->Control(ECtrlIdICF));
    }

void CWesternItutUiStateNonPredict::OnEntryL()
    {
    TInt keypadResId = KInvalidResId;
    switch (iOwner->DataMgr()->InputMode())
        {
        case ENumber:
        	if( iOwner->DataMgr()->IsExistPlusChar() )
        		{
        		keypadResId = R_FINGER_INPUT_KEYPAD_NUMBER_WITH_PLUS;
        		}
        	else
        		{
        		keypadResId = R_FINGER_INPUT_KEYPAD_NUMBER;
        		}
            break;
        case ELatin:
        case EHangul: // tp teleca change
            keypadResId = iOwner->DataMgr()->KeypadResourceId();
            break;
        default:
            break;                
        }

    // update resource id of keypad, candidate drop list
    iOwner->DataMgr()->NotifyChangeControlLayout(
        MItutPropertySubscriber::EItutPropertyKeypadResourceId, keypadResId);
    // Show ICF, Backspace after position changed to avoid flicker
    iOwner->LayoutContext()->Control(ECtrlIdBackspace)->Hide(EFalse);
    iOwner->LayoutContext()->Control(ECtrlIdICF)->Hide( EFalse );
    // for secret editor, there is no need to show navigation buttons
        {
        if (iOwner->DataMgr()->IsChinese())
            {
            iOwner->LayoutContext()->Control(ECtrlIdSwitch)->Hide(EFalse);
            iOwner->LayoutContext()->Control(ECtrlIdOptions)->Hide(EFalse);
            iOwner->LayoutContext()->Control(ECtrlIdClose)->Hide(EFalse);
            iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(EFalse);

            iOwner->LayoutContext()->ShowArrowBtn(EBtnArrowLeft | EBtnArrowRight
                 | EBtnArrowUp| EBtnArrowDown);   
            if(iOwner->LayoutContext()->IsSecretEdtior())
                {
                CAknFepCtrlRawKeyButton * leftbtn;
                CAknFepCtrlRawKeyButton * rightbtn;
                CAknFepCtrlRawKeyButton * upbtn;
                CAknFepCtrlRawKeyButton * downbtn;
                leftbtn = static_cast<CAknFepCtrlRawKeyButton *>
                                            (iOwner->LayoutContext()->Control(ECtrlIdArrowLeft));
                rightbtn = static_cast<CAknFepCtrlRawKeyButton *>
                                            (iOwner->LayoutContext()->Control(ECtrlIdArrowRight));
                upbtn = static_cast<CAknFepCtrlRawKeyButton *>
                                            (iOwner->LayoutContext()->Control(ECtrlIdArrowUp));
                downbtn = static_cast<CAknFepCtrlRawKeyButton *>
                                            (iOwner->LayoutContext()->Control(ECtrlIdArrowDown)); 
                leftbtn->SetDimmed(ETrue);   
                rightbtn->SetDimmed(ETrue);   
                upbtn->SetDimmed(ETrue);   
                downbtn->SetDimmed(ETrue);   
                }
            }
        else
            {
            iOwner->LayoutContext()->Control(ECtrlIdOptions)->Hide(EFalse);
            iOwner->LayoutContext()->Control(ECtrlIdClose)->Hide(EFalse);
            if ( iOwner->LayoutContext()->Control(ECtrlIdSwitch) != NULL )
				{
				iOwner->LayoutContext()->Control(ECtrlIdSwitch)->Hide(ETrue);
				}
            if ( iOwner->LayoutContext()->Control(ECtrlIdIndicator) != NULL )
				{
				iOwner->LayoutContext()->Control(ECtrlIdIndicator)->Hide(ETrue);
				}
            
            iIcf->MsgBubbleCtrl()->Hide( EFalse );
            
            iOwner->LayoutContext()->ShowArrowBtn(EBtnArrowLeft | EBtnArrowRight);    
            if(iOwner->LayoutContext()->IsSecretEdtior())
                {
                CAknFepCtrlRawKeyButton * leftbtn;
                CAknFepCtrlRawKeyButton * rightbtn;
                leftbtn = static_cast<CAknFepCtrlRawKeyButton *>
                                            (iOwner->LayoutContext()->Control(ECtrlIdArrowLeft));
                rightbtn = static_cast<CAknFepCtrlRawKeyButton *>
                                            (iOwner->LayoutContext()->Control(ECtrlIdArrowRight));
                leftbtn->SetDimmed(ETrue);   
                rightbtn->SetDimmed(ETrue);   
                }
            }
        }
    }

void CWesternItutUiStateNonPredict::OnExit()
    {
    // Hide ICF, Backspace, Arrow controls when exit to avoid flick
    iOwner->LayoutContext()->ShowArrowBtn(0);
    iOwner->LayoutContext()->Control(ECtrlIdICF)->Hide( ETrue );
    iOwner->LayoutContext()->Control(ECtrlIdBackspace)->Hide(ETrue);
    }

CGenericItutUiMgrBase::TUiState CWesternItutUiStateNonPredict::StateType()
    {
    return CGenericItutUiMgrBase::EStateWesternStandby;
    }

TInt CWesternItutUiStateNonPredict::HandleCommandL(TInt aCmd, TUint8* /*aData*/)
    {
    switch (aCmd) 
        {
        case ECmdPenInputIsSecretText:
            {
            //TUint* data = (TUint*) aData;
                {
                if (iOwner->DataMgr()->IsChinese())
                    {
                    iOwner->LayoutContext()->ShowArrowBtn(EBtnArrowLeft | EBtnArrowRight
                         | EBtnArrowUp| EBtnArrowDown);
                    /*CAknFepCtrlRawKeyButton * leftbtn;
                    CAknFepCtrlRawKeyButton * rightbtn;
                    CAknFepCtrlRawKeyButton * upbtn;
                    CAknFepCtrlRawKeyButton * downbtn;
                    leftbtn = static_cast<CAknFepCtrlRawKeyButton *>(iOwner->LayoutContext()->Control(ECtrlIdArrowLeft));
                    rightbtn = static_cast<CAknFepCtrlRawKeyButton *>(iOwner->LayoutContext()->Control(ECtrlIdArrowRight));
                    upbtn = static_cast<CAknFepCtrlRawKeyButton *>(iOwner->LayoutContext()->Control(ECtrlIdArrowUp));
                    downbtn = static_cast<CAknFepCtrlRawKeyButton *>(iOwner->LayoutContext()->Control(ECtrlIdArrowDown));                                    
                    if(*data > 0)
                        {                    
                        leftbtn->SetDimmed(ETrue);   
                        rightbtn->SetDimmed(ETrue);   
                        upbtn->SetDimmed(ETrue);   
                        downbtn->SetDimmed(ETrue);   
                        }
                    else
                        {
                        leftbtn->SetDimmed(EFalse);   
                        rightbtn->SetDimmed(EFalse);   
                        upbtn->SetDimmed(EFalse);   
                        downbtn->SetDimmed(EFalse);                           
                        }*/
                    }
                else
                    {
                    iOwner->LayoutContext()->ShowArrowBtn(EBtnArrowLeft | EBtnArrowRight);    
                    /*CAknFepCtrlRawKeyButton * leftbtn;
                    CAknFepCtrlRawKeyButton * rightbtn;
                    leftbtn = static_cast<CAknFepCtrlRawKeyButton *>(iOwner->LayoutContext()->Control(ECtrlIdArrowLeft));
                    rightbtn = static_cast<CAknFepCtrlRawKeyButton *>(iOwner->LayoutContext()->Control(ECtrlIdArrowRight));
                    if(*data > 0)
                        {
                        leftbtn->SetDimmed(ETrue);   
                        rightbtn->SetDimmed(ETrue);   
                        }
                    else
                        {
                        leftbtn->SetDimmed(EFalse);   
                        rightbtn->SetDimmed(EFalse);   
                        }*/
                    }
                }

            return KErrNone;
            }
        default:
            break;
        }

    return KErrNotSupported;    
    }

TBool CWesternItutUiStateNonPredict::HandleCtrlEventL(
    TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData)
    {
    switch (aEventType)
        {
        case EEventRawKeyDownEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());

            iConsumeLastKeyDown = EFalse;
            if (key->iScanCode == EStdKeyNkpAsterisk && 
                iIcf->InlineStateOn() && iIcf->NomatchState())
                {
                iConsumeLastKeyDown = ETrue;
                iOwner->UiManager()->SetLastRawKeyDown(key->iScanCode, ETrue, aCtrl);
                return ETrue;
                }

            return EFalse;    
            }
        case EEventRawKeyUpEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());

            if (key->iScanCode == EStdKeyNkpAsterisk)
                {
                if (iConsumeLastKeyDown)
                    {
                    iOwner->LayoutContext()->UiLayout()->SignalOwner(ESignalEnterSpellMode);
                    iOwner->UiManager()->SetLastRawKeyDown(key->iScanCode, EFalse, aCtrl);
                    return ETrue;
                    }
                }

            return EFalse;    
            }
        case EEventPointerDownOnNomatch:
            {
            iOwner->LayoutContext()->UiLayout()->SignalOwner(ESignalEnterSpellMode);
            return ETrue;
            }
        case EEventPointerDownOnInline:
            {
            iOwner->LayoutContext()->EnterMatchSelectionState(ETrue);
            return ETrue;
            }
        default:
            break;
        }

    return EFalse;
    }

// End Of File
