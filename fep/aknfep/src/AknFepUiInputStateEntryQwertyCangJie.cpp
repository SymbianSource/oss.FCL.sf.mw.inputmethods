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
*       Provides the TAknFepUIInputStateEntryQwertyCangJie methods.
*
*/












#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepUiInputStateEntryQwertyCangJie.h"
#include "AknFepUICtrlCandidatePane.h"

#include <PtiEngine.h>      //CPtiEngine
#include <e32keys.h>        //keys  
#include <avkon.hrh>

const TInt KEasyCangJieLengthLimit(2);      // Spelling limit for Easy CangJie is 2 characters
const TInt KNormalAdvCangJieLengthLimit(5); // Spelling limit for Normal and Advanced CangJie is 5 characters

TAknFepInputStateEntryQwertyCangJie::TAknFepInputStateEntryQwertyCangJie(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryQwertyBase(aOwner,aUIContainer)                           
    {
    UIContainer()->CandidatePane()->ShowCandidateOrdinals(ETrue);
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    ptiengine->SetInputMode( ( TPtiEngineInputMode )iOwner->FepMan()->CangJieMode() );

    // This is a must otherwise Engine cannot find keys in KeyMapping
    ptiengine->SetCase(EPtiCaseLower);
    ptiengine->SetCandidatePageLength(MAknFepUICtrlContainerChinese::ELayoutInput);
    }

TBool TAknFepInputStateEntryQwertyCangJie::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TPtiEngineInputMode currentMode = ptiengine->InputMode();

    TBool processKey = ETrue;

    if (!IsValidCangJieCode(aKey))
        return TAknFepInputStateEntryQwertyBase::HandleKeyL(aKey, aLength);
    
    TInt bufferLength = 0;
    TInt lengthLimit = 0;
    // Limit the length of the key append for different CangJie level
    switch(currentMode)
        {
        case EPtiEngineEasyCangjieQwerty:
            lengthLimit = KEasyCangJieLengthLimit;
            break;
        case EPtiEngineNormalCangjieQwerty:
        case EPtiEngineAdvCangjieQwerty:
            lengthLimit = KNormalAdvCangJieLengthLimit;
            break;
        default:
            break;
        }
        
    if (ptiengine->PhoneticSpellingCount() > 0)
        {
        TPtrC spellingBuf = ptiengine->GetPhoneticSpelling(1);
        bufferLength = spellingBuf.Length();
        
        if (bufferLength >= lengthLimit)
            {
            processKey = EFalse;
            fepMan->PlaySound(EAvkonSIDConfirmationTone);
            }
        }    

    return processKey ? TAknFepInputStateEntryQwertyBase::HandleKeyL(aKey, aLength) : ETrue;
    }


// End of file
