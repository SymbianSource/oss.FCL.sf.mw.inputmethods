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










#include "AknFepVietnameseToneManager.h"
#include "AknFepManager.h"


CAknFepVietnameseToneManager::CAknFepVietnameseToneManager()
    {
    }

CAknFepVietnameseToneManager::~CAknFepVietnameseToneManager()
    {
    }

CAknFepVietnameseToneManager* CAknFepVietnameseToneManager::NewL()
    {
    CAknFepVietnameseToneManager* self = new (ELeave) CAknFepVietnameseToneManager;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

void CAknFepVietnameseToneManager::ConstructL()
    {
    }

void CAknFepVietnameseToneManager::StopToneMarkLooping()
    {
    iToneIndex = 0;
    iVowelIndex = 0;
    iIsLooping = EFalse;
    }

TBool CAknFepVietnameseToneManager::StartToneMarkLooping()
    {
    __ASSERT_DEBUG(iFepMan != NULL, 
        RDebug::Print(
        _L("In CAknFepVietnameseToneManager::StartLooping,\nINVALID PARAM: Empty Fep Manager!")));

    StopToneMarkLooping();

    const TText prevChr = iFepMan->PreviousChar();
    
    // Judge if the prevChr is a vowel or not
    for (TUint i = 0; i < sizeof(VietVowelList) / sizeof(TText); ++i)
        {
        if (prevChr == VietVowelList[i])
            {
            iIsLooping = ETrue;
            iVowelIndex = i;
            break;
            }
        }

    return iIsLooping;
    }

TText CAknFepVietnameseToneManager::GetVowelWithToneMark() const
    {
    // First vowel row lists those tone marks themselves
    return VietToneMatrix[iVowelIndex+1][iToneIndex];
    }
