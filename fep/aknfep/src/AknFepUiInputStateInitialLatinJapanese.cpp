/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepUiInputStateInitialLatinJapanese methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateInitialLatinJapanese.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepCaseManager.h"               // CAknFepCaseManager
#include "AknFepManager.h"

#include <PtiDefs.h>                //keys
#include <PtiEngine.h>

// CONSTANTS

/**
 *  TAknFepUiInputStateInitialLatinJapanese class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialLatinJapanese::TAknFepUiInputStateInitialLatinJapanese
// Default constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateInitialLatinJapanese::TAknFepUiInputStateInitialLatinJapanese(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateInitialJapaneseBase(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialLatinJapanese::InitializeStateL
// Initialize State
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateInitialLatinJapanese::InitializeStateL()
    {
    TAknFepUiInputStateInitialJapaneseBase::InitializeStateL();
    
    // Setup PtiEngine
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TPtiEngineInputMode ptiInputMode = (iOwner->IsQwerty())? 
                                        EPtiEngineQwerty
                                        : EPtiEngineMultitapping;
    if (iOwner->FepMan()->CharacterWidth() == EFullWidthChar)
        {
        ptiInputMode = (iOwner->IsQwerty())? 
                            EPtiEngineFullWidthAlphabetQwerty
                            : EPtiEngineFullWidthAlphabet;
        }
    ptiEngine->SetInputMode(ptiInputMode);
    ptiEngine->ClearCurrentWord();

    // Setup Case
    CAknFepCaseManager* caseMan = iOwner->CaseMan();
    TInt currentCase = caseMan->CurrentCase();
    TPtiTextCase ptiCase = EPtiCaseUpper;
    if (currentCase == EAknEditorLowerCase)
        {
        ptiCase = EPtiCaseLower;
        }
    ptiEngine->SetCase(ptiCase);
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialLatinJapanese::HandleNumericShortKeyL
// Handling numeric short keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateInitialLatinJapanese::
HandleNumericShortKeyL(TInt aKey)
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    if(aKey == EPtiKeyStar)
        { // Handling of launch special character table
        if (!fepMan->HashKeySelectionInUse())
            {
            if (fepMan->EditSubmenuInUse())
                {
                fepMan->LaunchSelectModeMenuL();
                }
            else if (fepMan->IsAbleToLaunchSCT())
                {
                fepMan->LaunchSpecialCharacterTableL();
                }
            }
        ret = ETrue;
        }
    else
        {
        fepMan->StartInlineEditL();
        iOwner->ChangeState(EEntry);
        }
    // Setup Case without Qwerty
	if (!iOwner->IsQwerty())
		{
	    CPtiEngine* ptiEngine = iOwner->PtiEngine();
	    CAknFepCaseManager* caseMan = iOwner->CaseMan();
	    TInt currentCase = caseMan->CurrentCase();
	    TPtiTextCase ptiCase = EPtiCaseUpper;
	    if (currentCase == EAknEditorLowerCase)
	        {
	        ptiCase = EPtiCaseLower;
	        }
	    ptiEngine->SetCase(ptiCase);
		}

    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateInitialJapaneseBase::HandleAuxKeyL
// Handling OK and Backspace keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateInitialLatinJapanese::
    HandleAuxKeyL(TInt aKey,TKeyPressLength /*aLength*/)
    {
    TBool ret = EFalse;

    if (iOwner->IsQwerty())
        {
        switch (aKey)
            {
            case EStdKeyEnter:          /* 0x03 */
            case EStdKeyNkpEnter:       /* 0x88 */
                {
                // PtiEngine does not have mapping for the enter key.
                MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
                const TText KAknFEPLineFeedSymbol = 0x21B2;
                TBuf<1> lineFeed;
                lineFeed.Zero();
                lineFeed.Append(KAknFEPLineFeedSymbol);
                fepMan->NewCharacterL(lineFeed);
                // Commit the character if chr key is not pressed.
                fepMan->CommitInlineEditL();

                ret = ETrue;
                }
                break;
            default:
                break;
            }
        }
    return ret;
    }

// End of file
