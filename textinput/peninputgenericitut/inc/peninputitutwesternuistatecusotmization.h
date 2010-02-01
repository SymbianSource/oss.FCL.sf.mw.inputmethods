/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#ifndef C_WESTERNITUTUISTATECUSTOMIZATION_H
#define C_WESTERNITUTUISTATECUSTOMIZATION_H

#include "peninputgenericitutuistatebase.h"

class CVirtualKey;
class CItutCustomizedRscMgr;

class CWesternItutUiStateCustomization : public CGenericItutUiStateBase
    {
public:
    static CWesternItutUiStateCustomization* NewL(CGenericItutUiMgrBase* aOwner);

    ~CWesternItutUiStateCustomization();

    void OnExit();

    void OnEntryL();

    TInt HandleCommandL(TInt aCmd, TUint8* aData);

    CGenericItutUiMgrBase::TUiState StateType();

protected:
    CWesternItutUiStateCustomization(CGenericItutUiMgrBase* aOwner);
    void ConstructL();

private:
    void DimKey(TInt aKeyIndex);

    void HideKey(TInt aKeyIndex);

    CVirtualKey* FindKeyByScanCode(TInt aScanCode);

    TBool IsKeyIdxValid(TInt aKeyIndex);

private:
    CItutCustomizedRscMgr* iRscMgr;
    };

#endif //C_WESTERNITUTUISTATECUSTOMIZATION_H