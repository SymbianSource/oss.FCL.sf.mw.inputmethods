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


#ifndef C_ITUTCUSTOMIZATIONRESOURCEMGR_H
#define C_ITUTCUSTOMIZATIONRESOURCEMGR_H

#include <e32base.h>
#include <e32cmn.h>

#include "peninputgenericitutcustomization.h"

class CFepUiBaseCtrl;
class CFepUiLayout;
class CFbsBitmap;
class MItutLayoutContext;
class CAknFepCtrlRawKeyButton;
class CVirtualKey;

class CItutCustomizedRscMgr : public CBase
    {
public:
    static CItutCustomizedRscMgr* NewL(CFepUiLayout* aUiLayout,
                                       MItutLayoutContext* aLayoutContext);
    virtual ~CItutCustomizedRscMgr();

    // release all bitmap's handles to app side created bmps
    // restore dimmed/hided keys by application, 
    void Restore();

    void RemoveAddedCtrls();

    void CreateCustomizedRawKeyButtonL(TItutKey aKeyInfo);

    TInt AddDimmedCtrl(CFepUiBaseCtrl* aCtrl);
    TInt AddHidedCtrl(CFepUiBaseCtrl* aCtrl);
    TInt AddDimmedVKey(CVirtualKey* aKey);
    
    void ResizeAddedCtrls();
    
    TInt FindRawKeyBtn(TInt aScanCode);

    void ShowRawKeyBtn(TInt aScanCode, TBool aShow = ETrue);

private:
    CItutCustomizedRscMgr(CFepUiLayout* aUiLayout,
                          MItutLayoutContext* aLayoutContext);
    void ConstructL();

private:
    struct TAddedRawKeyCtrl
        {
        TInt iPos;
        TInt iInnerPos;
        TAknTextLineLayout iTextFormat;
        CAknFepCtrlRawKeyButton* iCtrl;
        };

private:
    TBool IsBmpValid(const TItutBmp& aBmp);
    TBool IsMaskBmpValid(const TItutBmp& aMaskBmp);

private:
    RArray<TAddedRawKeyCtrl> iAddedCtrls;
    RPointerArray<CFepUiBaseCtrl> iDimmedCtrls;
    RPointerArray<CFepUiBaseCtrl> iHidedCtrls;
    RPointerArray<CVirtualKey> iDimmedVKeys;

    // not own
    CFepUiLayout* iUiLayout;
    MItutLayoutContext* iLayoutContext;
    
    // start control index of new added control
    TInt iNewCtrlId;
    };

#endif // C_ITUTCUSTOMIZATIONRESOURCEMGR_H