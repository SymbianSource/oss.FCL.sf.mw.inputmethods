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
#include <peninputlayoutvkb.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputcmdparam.h>
#include <peninputlayoutbutton.h>

#include "peninputitutwesternuistatecusotmization.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputitutcustomizationresourcemgr.h"
#include "peninputgenericitutcustomization.h"
#include "peninputgenericitutconverter.h"

CWesternItutUiStateCustomization* CWesternItutUiStateCustomization::NewL(
    CGenericItutUiMgrBase* aOwner)
    {
    CWesternItutUiStateCustomization* self = 
        new (ELeave) CWesternItutUiStateCustomization(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CWesternItutUiStateCustomization::~CWesternItutUiStateCustomization()
    {
    if (iRscMgr)
        {
        iRscMgr->RemoveAddedCtrls();
        delete iRscMgr;
        }
    }

CWesternItutUiStateCustomization::CWesternItutUiStateCustomization(
    CGenericItutUiMgrBase* aOwner):CGenericItutUiStateBase(aOwner)
    {
    }
    
void CWesternItutUiStateCustomization::ConstructL()
    {
    CGenericItutUiStateBase::ConstructL();
    iRscMgr = CItutCustomizedRscMgr::NewL(iOwner->LayoutContext()->UiLayout(), 
                                          iOwner->LayoutContext());
    }

void CWesternItutUiStateCustomization::OnEntryL()
    {
    }

void CWesternItutUiStateCustomization::OnExit()
    {
    iRscMgr->Restore();
    }

TInt CWesternItutUiStateCustomization::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    switch (aCmd)
        {
        case EItutExtCmdSetLayout:
            {
            TInt layouttype = *(reinterpret_cast<TInt*>(aData));
            switch (layouttype)
                {
                case EItutLayoutNumeric:
                    {
                    iOwner->DataMgr()->NotifyChangeControlLayout(
                        MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
                        R_FINGER_INPUT_KEYPAD_NUMBER);
                    break;
                    }
                case EItutLayoutAlphabetic:
                    {
                    // should consider: if pin code need to support change keypad when case changed
                    // should also consider: if pin code need to consider different keypad res id used for different
                    // language script
                    iOwner->DataMgr()->NotifyChangeControlLayout(
                        MItutPropertySubscriber::EItutPropertyKeypadResourceId,
                        iOwner->DataMgr()->KeypadResourceId());
                    break;
                    }
                default:
                    return KErrNotSupported;
                }

            return KErrNone;
            }
        case EItutExtCmdDimKey:
            {
            TInt keyidx = *(reinterpret_cast<TInt*>(aData));

            if (IsKeyIdxValid(keyidx))
                {
                DimKey(keyidx);
                }

            return KErrNone;
            }
        case EItutExtCmdHideKey:
            {
            TInt keyidx = *(reinterpret_cast<TInt*>(aData));
            
            if (IsKeyIdxValid(keyidx))
                {
                HideKey(keyidx);
                }

            return KErrNone;
            }
        case EItutExtCmdAddKey:
            {
            TItutKey itutkey = *(reinterpret_cast<TItutKey*>(aData));

            if (iRscMgr->FindRawKeyBtn(itutkey.iScanCode) == KErrNotFound)
                {
                iRscMgr->CreateCustomizedRawKeyButtonL(itutkey);
                }
            else
                {
                iRscMgr->ShowRawKeyBtn(itutkey.iScanCode);
                }    

            return KErrNone;
            }
        case EItutExtCmdSetIcfData:
            {
            TFepInputContextFieldData icfdata = 
                *(reinterpret_cast<TFepInputContextFieldData*>(aData));
            CFepLayoutMultiLineIcf* icffield = 
                static_cast<CFepLayoutMultiLineIcf*>(iOwner->LayoutContext()->Control(ECtrlIdICF));
            icffield->SetTextL(icfdata);
            }
            break;
        case EItutExtCmdSizeChanged:
            {
            iRscMgr->ResizeAddedCtrls();
            }
        default:
            break;
        }

    return KErrNotSupported;    
    }

CGenericItutUiMgrBase::TUiState CWesternItutUiStateCustomization::StateType()
    {
    return CGenericItutUiMgrBase::EStateCustomization;
    }

void CWesternItutUiStateCustomization::DimKey(TInt aKeyIndex)
    {
    TInt keyscancode = ItutKeymap[aKeyIndex].iScanCode;
    TInt ctrlid = ItutKeymap[aKeyIndex].iCtrlId;
    
    if (keyscancode != KInvalidScanCode)
        {
        CVirtualKeyboard* keypad = 
            static_cast<CVirtualKeyboard*>(iOwner->LayoutContext()->Control(ECtrlIdStdItut));
        CVirtualKey* dimkey = FindKeyByScanCode(keyscancode);
        keypad->DimKey(dimkey, ETrue);
        iRscMgr->AddDimmedVKey(dimkey);
        }
    else if (ctrlid != KInvalidCtrlId)
        {
        CButtonBase* btn = static_cast<CButtonBase*>(iOwner->LayoutContext()->Control(ctrlid));
        btn->SetDimmed(ETrue);
        iRscMgr->AddDimmedCtrl(btn);
        }
    }

void CWesternItutUiStateCustomization::HideKey(TInt aKeyIndex)
    {
    // cannot hide virtual key, only can hide button
    TInt ctrlid = ItutKeymap[aKeyIndex].iCtrlId;
    
    if (ctrlid != KInvalidCtrlId)
        {
        iRscMgr->AddHidedCtrl(iOwner->LayoutContext()->Control(ctrlid));
        }
    }

CVirtualKey* CWesternItutUiStateCustomization::FindKeyByScanCode(TInt aScanCode)
    {
    CVirtualKeyboard* keypad = 
        static_cast<CVirtualKeyboard*>(iOwner->LayoutContext()->Control(ECtrlIdStdItut));
    const RPointerArray<CVirtualKey>& keys = keypad->KeyArray();
    
    for (TInt i = 0; i < keys.Count(); i++)
        {
        if (keys[i]->ScanCode() == aScanCode)
            {
            return keys[i];
            }
        }

    return NULL;
    }

TBool CWesternItutUiStateCustomization::IsKeyIdxValid(TInt aKeyIndex)
    {
    return (aKeyIndex >= 0 && aKeyIndex < sizeof(ItutKeymap)/sizeof(ItutKeymap[0]));
    }

// End Of File
