/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepInputStateEntryWesternPredictiveJp methods.
*
*/












#include "AknFepUiInputStateEntryWesternPredictiveJp.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepManager.h"
#include "AknFepPanic.h"
#include "AknFepCaseManager.h"

#include <PtiEngine.h>                             //CPtiEngine
#include <PtiDefs.h>                            //keys
#include <featmgr.h>                //FeatureManager
#include <e32keys.h>
#include <aknfep.rsg>

TAknFepInputStateEntryWesternPredictiveJp::
TAknFepInputStateEntryWesternPredictiveJp(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateEntryWesternPredictive(aOwner)
    {
    }

// ---------------------------------------------------------------------------
// Handle Star-key event
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateEntryWesternPredictiveJp::HandleStarKeyL(TKeyPressLength aLength)
    {
    if (aLength == ELongKeyPress)
        {
        // Call the parent function
        return TAknFepInputStateEntryWesternPredictive::HandleStarKeyL(aLength);
        }

    // Check whether it is possible to change other cases.
    if (!iOwner->CaseMan()->IsAbleChangeCase())
        {
        return ETrue;
        }

    // Change next case mode (Lower->Upper->Text->Lower)
    TInt oldCase = iOwner->CaseMan()->CurrentCase();
    TInt newCase;
    switch (oldCase)
        {
        default:
        case EAknEditorLowerCase:
            newCase = EAknEditorUpperCase;
            break;
        case EAknEditorUpperCase:
            newCase = EAknEditorTextCase;
            break;
        case EAknEditorTextCase:
            newCase = EAknEditorLowerCase;
            break;
        }
    iOwner->CaseMan()->SetCurrentCase(newCase);
    newCase = iOwner->CaseMan()->CurrentCase();
    if (oldCase != newCase)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        TPtiTextCase caseMode = ptiengine->Case();
        TPtrC text = ptiengine->CurrentWord();
        HBufC* newText = HBufC::NewLC(text.Length());
        TPtr newPtr(newText->Des());
        newPtr.Copy(text);
        switch (newCase)
            {
            default:
            case EAknEditorLowerCase:    // For Japanese, T9 lower mode(abc)
                newPtr.LowerCase();
                caseMode = EPtiCaseLower;
                break;
            case EAknEditorUpperCase:   // For Japanese, T9 upper mode(ABC)
                newPtr.UpperCase();
                caseMode = EPtiCaseUpper;
                break;
            case EAknEditorTextCase:    // For Japanese, T9 capitalize mode(Abc)
                newPtr.Capitalize();
                caseMode = (newPtr.Length() > 1)?
                            EPtiCaseUpper : EPtiCaseLower;
                break;
            }
        fepMan->UpdateInlineEditL(newPtr, newPtr.Length());
        ptiengine->SetCurrentWord(newPtr);
        ptiengine->SetCase(caseMode);
        CleanupStack::PopAndDestroy(newText); // newText
        }
    return ETrue;
    }

// End of file
