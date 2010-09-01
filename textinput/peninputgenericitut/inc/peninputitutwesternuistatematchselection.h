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

#ifndef C_WESTERNITUTUISTATEMATCHSELECTION_H
#define C_WESTERNITUTUISTATEMATCHSELECTION_H

#include "peninputgenericitutuistatebase.h"

class CFepLayoutScrollableList;
class CFepLayoutMultiLineIcf;

class CWesternItutUiStateMatchSelection : public CGenericItutUiStateBase
    {
public:
    static CWesternItutUiStateMatchSelection* NewL(CGenericItutUiMgrBase* aOwner);

    ~CWesternItutUiStateMatchSelection();

    void OnExit();

    void OnEntryL();

    CGenericItutUiMgrBase::TUiState StateType();

    TInt HandleCommandL(TInt aCmd, TUint8* aData);
    
    TBool HandleCtrlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

protected:
    CWesternItutUiStateMatchSelection(CGenericItutUiMgrBase* aOwner);
    void ConstructL();

private:
    void ReportItemSelected(TInt aCmdId, TInt aIdx, TBool aCommit);
    void ReportItemSelected(TInt aCmdId, TInt aIdx);
    void SetMatchSelListLTPosAndDisplay();

private:
    CFepLayoutScrollableList* iSelList;
    RPointerArray<HBufC> iCandlist;
    CFepLayoutMultiLineIcf* iIcf;
    HBufC* iSpellText;
    };

#endif //C_WESTERNITUTUISTATEMATCHSELECTION_H