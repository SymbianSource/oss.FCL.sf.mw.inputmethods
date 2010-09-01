/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateCandidateQwertyZhuyin methods.
*
*/












#include "AknFepUiInputStateCandidateQwertyZhuyin.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUICtrlCandidatePane.h"      //MAknFepUICtrlCandidatePane
#include <PtiEngine.h>                      //CPtiEngine

#include "AknFepManager.h"                  //FepMan flag

TAknFepInputStateCandidateQwertyZhuyin::TAknFepInputStateCandidateQwertyZhuyin(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateQwertyChinese(aOwner, aUIContainer)
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->CandidatePane()->SelectFirst();

    TBool multiplePages = iOwner->PtiEngine()->MoreCandidatePages();
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);
    UpdateIndicator();
    }

TBool TAknFepInputStateCandidateQwertyZhuyin::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    if(aKey == EStdKeyBackspace)
        {
        iOwner->ChangeState(EEntry);
        return ETrue;
        }
    else
        {
        return TAknFepInputStateCandidateQwertyChinese::HandleKeyL(aKey,aLength);
        }
    }

// End of file
