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

#include "peninputsplititutuistatebase.h"

class CFepLayoutMultiLineIcf;
class CAknFepCtrlCommonButton;

class CWesternSplitItutUiStateSpelling : public CSplitItutUiStateBase
    {
public:
    static CWesternSplitItutUiStateSpelling* NewL(CSplitItutUiMgrBase* aOwner);

    ~CWesternSplitItutUiStateSpelling();

    void OnExit();

    void OnEntryL();

    CSplitItutUiMgrBase::TUiState StateType();

    TInt HandleCommandL(TInt aCmd, TUint8* aData);

    TBool HandleCtrlEventL(TInt aEventType, 
                           CFepUiBaseCtrl* aCtrl, 
                           const TDesC& aEventData);

protected:
    CWesternSplitItutUiStateSpelling(CSplitItutUiMgrBase* aOwner);
    void ConstructL();
    void CreateTextBtnIfNeededL(CAknFepCtrlCommonButton*& aBtn, 
                                TInt aCtrlId,
                                TInt aCtrlPos,
                                TInt aTextResId,
                                TAknTextLineLayout aTextFormat);
	void ReCalcLayoutL();
private:
    CFepLayoutMultiLineIcf* iICF;
    CAknFepCtrlCommonButton* iOk;
    CAknFepCtrlCommonButton* iCancel;
    };

#endif //C_WESTERNITUTUISTATESPELLING_H