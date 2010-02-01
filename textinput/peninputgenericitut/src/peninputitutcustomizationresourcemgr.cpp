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


#include <peninputlayout.h>
#include <peninputlayoutbutton.h>
#include <peninputlayoutvkb.h>
#include <peninputrawkeybutton.h>
#include <AknIconUtils.h>
#include <fbs.h>

#include "peninputitutcustomizationresourcemgr.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutdata.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutconverter.h"

CItutCustomizedRscMgr* CItutCustomizedRscMgr::NewL(CFepUiLayout* aUiLayout,
                                                   MItutLayoutContext* aLayoutContext)
    {
    CItutCustomizedRscMgr* self = new (ELeave) CItutCustomizedRscMgr(aUiLayout, aLayoutContext);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CItutCustomizedRscMgr::~CItutCustomizedRscMgr()
    {
    iAddedCtrls.Reset();
    iAddedCtrls.Close();

    iDimmedCtrls.Reset();
    iDimmedCtrls.Close();

    iHidedCtrls.Reset();
    iHidedCtrls.Close();
    
    iDimmedVKeys.Reset();
    iDimmedVKeys.Close();
    }

void CItutCustomizedRscMgr::Restore()
    {
    // in customized case, only keypad's key and button can be dimmed
    for (TInt i = 0; i < iDimmedCtrls.Count(); i++)
        {
        static_cast<CButtonBase*>(iDimmedCtrls[i])->SetDimmed(EFalse);
        }
    iDimmedCtrls.Reset();
    
    for (TInt i = 0; i < iHidedCtrls.Count(); i++)
        {
        iHidedCtrls[i]->Hide(EFalse);
        }
    iHidedCtrls.Reset();
    
    for (TInt i = 0; i < iDimmedVKeys.Count(); i++)
        {
        iDimmedVKeys[i]->SetDimmed(EFalse);
        }
    iDimmedVKeys.Reset();
        
    // hide all new created ctrl
    for (TInt i = 0; i < iAddedCtrls.Count(); i++)
        {
        iAddedCtrls[i].iCtrl->Hide(ETrue);
        }
    }

void CItutCustomizedRscMgr::RemoveAddedCtrls()
    {
    for (TInt i = 0; i < iAddedCtrls.Count(); i++)
        {
        iUiLayout->RemoveControl(iAddedCtrls[i].iCtrl);
        }
    iAddedCtrls.Reset();

    iNewCtrlId = ECtrlIdLastStdCtrl;
    }

void CItutCustomizedRscMgr::CreateCustomizedRawKeyButtonL(TItutKey aKeyInfo)
    {
    CAknFepCtrlRawKeyButton* rawkeybtn = NULL;

    rawkeybtn = CAknFepCtrlRawKeyButton::NewL(iUiLayout,
                                              iNewCtrlId,
											  KAknsIIDQsnFrFunctionButtonNormal,
										   	  KAknsIIDQsnFrFunctionButtonPressed,
										      KAknsIIDQsnFrFunctionButtonInactive,                                             
                                              EEventRawKeyDownEvent,
                                              EEventRawKeyUpEvent);
    rawkeybtn->SetRect(TItutDataConverter::AnyToRect(
                               iLayoutContext->DataMgr()->RequestData(aKeyInfo.iPos)));
    rawkeybtn->SetForgroundBmpRect(TItutDataConverter::AnyToRect(
                               iLayoutContext->DataMgr()->RequestData(aKeyInfo.iInnerPos)));
    rawkeybtn->SetScanCode(aKeyInfo.iScanCode);

    TItutBmp itutbmp = aKeyInfo.iForgroundBmp;

    if (IsBmpValid(itutbmp))
        {
        CFbsBitmap* bmp = new (ELeave) CFbsBitmap;
        CleanupStack::PushL(bmp);
        bmp->Duplicate(itutbmp.iBmpHandle);
        CleanupStack::Pop(bmp);
        
        if (IsMaskBmpValid(itutbmp))
            {
            CFbsBitmap* maskbmp = new (ELeave) CFbsBitmap;
            CleanupStack::PushL(maskbmp);
            maskbmp->Duplicate(itutbmp.iMaskBmpHandle);
            CleanupStack::Pop(maskbmp);
            
            rawkeybtn->SetForegroundBmpL(bmp, maskbmp);
            }
        else
            {
            rawkeybtn->SetForegroundBmpL(bmp, NULL);
            }    
        }

    rawkeybtn->SetTextL(aKeyInfo.iText);
    rawkeybtn->SetTextFormat(aKeyInfo.iTextFormat);

    TAddedRawKeyCtrl newctrl = {aKeyInfo.iPos, aKeyInfo.iInnerPos, 
                                aKeyInfo.iTextFormat, rawkeybtn};

    iUiLayout->AddControlL(rawkeybtn);
    iAddedCtrls.AppendL(newctrl);
    rawkeybtn->Draw();
    iUiLayout->UpdateArea(rawkeybtn, rawkeybtn->Rect(), EFalse);
    iNewCtrlId++;
    }

void CItutCustomizedRscMgr::ResizeAddedCtrls()
    {
    for (TInt i = 0; i < iAddedCtrls.Count(); i++)
        {
        iAddedCtrls[i].iCtrl->SetTextFormat(iAddedCtrls[i].iTextFormat);
        iAddedCtrls[i].iCtrl->SizeChanged(
                     TItutDataConverter::AnyToRect(
                        iLayoutContext->DataMgr()->RequestData(iAddedCtrls[i].iPos)), 
                     TItutDataConverter::AnyToRect(
                        iLayoutContext->DataMgr()->RequestData(iAddedCtrls[i].iInnerPos)),
                     ETrue);
        }
    }

TInt CItutCustomizedRscMgr::AddDimmedCtrl(CFepUiBaseCtrl* aCtrl)
    {
    if (!aCtrl)
        {
        return KErrNone;
        }
    
    if (iDimmedCtrls.Find(aCtrl) == KErrNotFound)
        {
        return iDimmedCtrls.Append(aCtrl);
        }

    return KErrNone;
    }
    
TInt CItutCustomizedRscMgr::AddDimmedVKey(CVirtualKey* aKey)
    {
    if (!aKey)
        {
        return KErrNone;
        }
    
    if (iDimmedVKeys.Find(aKey) == KErrNotFound)
        {
        return iDimmedVKeys.Append(aKey);
        }
    
    return KErrNone;    
    }

TInt CItutCustomizedRscMgr::AddHidedCtrl(CFepUiBaseCtrl* aCtrl)
    {
    if (!aCtrl)
        {
        return KErrNone;
        }

    if (iHidedCtrls.Find(aCtrl) == KErrNotFound)
        {
        aCtrl->Hide(ETrue);
        return iHidedCtrls.Append(aCtrl);
        }

    return KErrNone;    
    }

CItutCustomizedRscMgr::CItutCustomizedRscMgr(
    CFepUiLayout* aUiLayout, MItutLayoutContext* aLayoutContext)
    :iUiLayout(aUiLayout), 
     iLayoutContext(aLayoutContext),
     iNewCtrlId(ECtrlIdLastStdCtrl)
    {
    }

void CItutCustomizedRscMgr::ConstructL()
    {
    }

TBool CItutCustomizedRscMgr::IsBmpValid(const TItutBmp& aBmp)
    {
    return aBmp.iBmpHandle != KInvalidBmp;
    }

TBool CItutCustomizedRscMgr::IsMaskBmpValid(const TItutBmp& aMaskBmp)
    {
    return aMaskBmp.iMaskBmpHandle != KInvalidBmp;
    }

TInt CItutCustomizedRscMgr::FindRawKeyBtn(TInt aScanCode)
    {
    for (TInt i = 0; i < iAddedCtrls.Count(); i++)
        {
        CAknFepCtrlRawKeyButton* rawkeybtn = 
                                      static_cast<CAknFepCtrlRawKeyButton*>(iAddedCtrls[i].iCtrl);
        if (rawkeybtn->ScanCode() == aScanCode)
            {
            return i;
            }
        }

    return KErrNotFound;
    }

void CItutCustomizedRscMgr::ShowRawKeyBtn(TInt aScanCode, TBool aShow)
    {
    for (TInt i = 0; i < iAddedCtrls.Count(); i++)
        {
        CAknFepCtrlRawKeyButton* rawkeybtn = 
                                      static_cast<CAknFepCtrlRawKeyButton*>(iAddedCtrls[i].iCtrl);
        if (rawkeybtn->ScanCode() == aScanCode)
            {
            rawkeybtn->Hide(!aShow);
            return;
            }
        }
    }

// End Of File
