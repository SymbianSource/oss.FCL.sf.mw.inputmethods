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
#ifndef M_SPLITITUTLAYOUTCONTEXT_H
#define M_SPLITITUTLAYOUTCONTEXT_H

#include <e32base.h>

class CFepUiBaseCtrl;
class CSplitItutDataMgr;
class CFepUiLayout;
class CSplitItutWindowManager;

enum TArrowBtns
    {
    EBtnArrowLeft = 0x01,
    EBtnArrowRight = 0x02,
    EBtnArrowUp = 0x04,
    EBtnArrowDown = 0x08
    };
    
enum TLandscapeBtns
    {
    EBtnOptions = 0x01,
    EBtnClose = 0x02
    };

class MSplitItutLayoutContext
    {
public:
    virtual CFepUiBaseCtrl* Control(TInt aCtrlId) = 0;
    virtual void SubmitText(const TDesC& aEventData) = 0;
    virtual void ShowArrowBtn(TInt aShowFlag) = 0;
    virtual CSplitItutDataMgr* DataMgr() = 0;
    // customized resource manager
    virtual CFepUiLayout* UiLayout() = 0;
    virtual TBool IsSecretEdtior() = 0;
    virtual void ApplyVariantLafDataL(TBool aResolutionChange = EFalse) = 0;
    virtual CSplitItutWindowManager* UiManager() = 0;
    };

#endif //M_SPLITITUTLAYOUTCONTEXT_H