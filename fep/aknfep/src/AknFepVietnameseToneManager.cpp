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

void CAknFepVietnameseToneManager::StartKeyLooping(TInt aKey)
    {
    iLoopingKey = aKey;
    iIsKeyLooping = ETrue;
    iIsLoopingCombined = EFalse;
    }

void CAknFepVietnameseToneManager::StopKeyLooping()
    {
    iLoopingKey = 0;
    iIsKeyLooping = EFalse;
    iIsLoopingCombined = EFalse;
    }

TInt CAknFepVietnameseToneManager::ToneMarkIndex() const
    {
    const TText prevChr = iFepMan->PreviousChar();

    // All tone mark is after number in product key mapping.
    // Notice: The order in product key mapping should be the same with in tone mark array. 
    for (TUint i = 0; i < KNumberOfToneMarks; ++i)
        {
        TBuf<1> num;
        num.Num( i + 2 );
        if (prevChr == num [0])
            {
            return i;
            }
        }
    
    return KErrNotFound;
    }

TBool CAknFepVietnameseToneManager::GetLoopingToneMarkVowel( TText& aText ) const
    {
    const TText prev2prevChr = iFepMan->PreviousToPreviousChar( ETrue );

    // Judge if the previous to previous Character is a vowel or not
    TInt vowelIndex = KErrNotFound;
    for (TUint i = 0; i < sizeof(VietVowelList) / sizeof(TText); ++i)
        {
        if ( prev2prevChr == VietVowelList[i] )
            {
            vowelIndex = i;
            break;
            }
        }
    
    if (vowelIndex >= 0)
        {
        aText =  VietToneMatrix[vowelIndex+1][ ToneMarkIndex() ];
        return ETrue;
        }
    
    return EFalse;
    }

