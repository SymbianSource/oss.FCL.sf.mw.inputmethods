/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStatePredictiveCandidatePinyin methods.
*
*/












#include "AknFepUiInputStatePredictiveCandidatePinyin.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface

TAknFepInputStatePredictiveCandidatePinyin::TAknFepInputStatePredictiveCandidatePinyin(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStatePredictiveCandidate(aOwner, aUIContainer)
    {
    }

TBool TAknFepInputStatePredictiveCandidatePinyin::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    if ((aKey == EPtiKey0) || (aKey == EPtiKey1))
        {
        iOwner->FepMan()->TryCloseUiL();
        return EFalse;
        }
    else
        {
        return TAknFepInputStatePredictiveCandidate::HandleKeyL(aKey, aLength);
        }
    }

// End of file
