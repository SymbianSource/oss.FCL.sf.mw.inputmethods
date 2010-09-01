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

#ifndef C_WESTERNITUTUISTATESPELLING_H
#define C_WESTERNITUTUISTATESPELLING_H

#include "peninputgenericitutuistatebase.h"

class CItutCustomizedRscMgr;
class CFepLayoutMultiLineIcf;
class CAknFepCtrlCommonButton;

class CWesternItutUiStateSpelling : public CGenericItutUiStateBase
    {
public:
    static CWesternItutUiStateSpelling* NewL(CGenericItutUiMgrBase* aOwner);

    ~CWesternItutUiStateSpelling();

    void OnExit();

    void OnEntryL();

    CGenericItutUiMgrBase::TUiState StateType();

    TInt HandleCommandL(TInt aCmd, TUint8* aData);

    TBool HandleCtrlEventL(TInt aEventType, 
                           CFepUiBaseCtrl* aCtrl, 
                           const TDesC& aEventData);

protected:
    CWesternItutUiStateSpelling(CGenericItutUiMgrBase* aOwner);
    void ConstructL();
    void CreateTextBtnIfNeededL(CAknFepCtrlCommonButton*& aBtn, 
                                TInt aCtrlId,
                                TInt aCtrlPos,
                                TInt aTextResId,
                                TAknTextLineLayout aTextFormat);
	void ReCalcLayoutL();
private:
    CItutCustomizedRscMgr* iRscMgr;
    CFepLayoutMultiLineIcf* iICF;
    CAknFepCtrlCommonButton* iOk;
    CAknFepCtrlCommonButton* iCancel;
    };

#endif //C_WESTERNITUTUISTATESPELLING_H