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
* Description:           
*       Provides the TAknFepUiInputStateCompletionHiraganaKanji methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateCompletionHiraganaKanji.h"
#include "AknFepUIManagerStateInterface.h"   // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        // MAknFepManagerUIInterface
#include "AknFepUICtrlCandidatePaneInterface.h"
                                        // MAknFepUICtrlCandidatePaneInterface
#include "AknFepUICtrlContainerJapanese.h"
#include "AknFepGlobalEnums.h"
#include "AknFepManager.h"

#include <PtiDefs.h>                     // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>        // MPtiEngineCompositionDataInterface

/**
 *  TAknFepUiInputStateCompletionHiraganaKanji class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionHiraganaKanji::
//                                  TAknFepUiInputStateCompletionHiraganaKanji
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCompletionHiraganaKanji::
    TAknFepUiInputStateCompletionHiraganaKanji(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateCompletionJapaneseMultitap(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionHiraganaKanji::HandleNumericShortKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionHiraganaKanji::HandleNumericShortKeyL(TInt aKey)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = CommitInlineEditingCharacterL();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->ConvertingString();
    if (text.Length() > 0)
        { // for last character is space or line feed
        iOwner->ChangeState(EInitial);
        fepMan->SimulateKeyEventL(aKey, ETrue);
        ret = ETrue;
        }
    else
        {
        iOwner->ChangeState(EInitial);
        if (iOwner->IsQwerty())
            {
            if (IsQwertyKey(aKey) && aKey != EStdKeySpace)
                {
                // Run on aKey if pressing qwerty key.
                ret = EFalse;
                }
            }
        else
            {
            if (IsNumberKey(aKey))
                {
                // Run on aKey if pressing number key.
                ret = EFalse;
                }
            }
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionHiraganaKanji::HandleNumericLongKeyL
// Handling Numeric long keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionHiraganaKanji::HandleNumericLongKeyL(TInt aKey)
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    ret = ProcessNumericLongKeyPressL(aKey);
    if (iOwner->IsPredictive() && fepMan->EditorHasFreeSpace())
        {
        iOwner->ChangeState(ENextWord);
        }
    else
        {
        iOwner->ChangeState(EInitial);
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionHiraganaKanji::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionHiraganaKanji::HandleAuxKeyL(TInt aKey,
                                                                TKeyPressLength aLength)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TPtiEngineCommand ptiCommand = EPtiCommandUserActionAllComplete;

    if (aKey == EKeyOK 
     || aKey == EStdKeyDevice3          /* 0xA7 */
     || aKey == EStdKeyEnter
     || aKey == EStdKeyNkpEnter)
        {
        if (aLength == EShortKeyPress)
            {
            ptiCommand = EPtiCommandUserActionComplete;
            }
        ptiEngine->HandleCommandL(ptiCommand);

        UpdateInlineEditingCharacterL();
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionHiraganaKanji::UpdateInlineEditingCharacterL
// Update inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionHiraganaKanji::UpdateInlineEditingCharacterL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;

    TBool isnextwordchar = !fepMan->IsFlagSet(CAknFepManager::EFlagLineFeedCharacter
                                    | CAknFepManager::EFlagSpaceCharacter
                                    | CAknFepManager::EFlagFullWidthSpaceCharacter);
    
    if(CommitInlineEditingCharacterL())
        {
        const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
        TPtrC text = data->ConvertingString();
        TInt cursorpos = data->CursorPosition();
        if (text.Length() > 0)
            {
            if (fepMan->EditorHasFreeSpace())
                {
                if (data->NumberOfPhrase() > 0)
                    {
                    TPtrC8 clauseChar = data->LengthOfPhrase();
                    cursorpos = clauseChar[0];
                    }
                iOwner->ChangeState(EConversion);
                fepMan->StartInlineEditL();
                fepMan->UpdateInlineEditL(text, cursorpos);
                }
            else
                {
                iOwner->ChangeState(EInitial);
                }
            }
        else
            {
            __ASSERT_DEBUG(text.Length() == 0,
                RDebug::Print(
                   _L("In TAknFepUiInputStateCompletionJapaneseMultitap::UpdateInlineEditingCharacterL,\nINVALID DATA: ConvertingString. Roll back to initial State!")));
            ptiEngine->ClearCurrentWord();
            if(iOwner->IsPredictive()
            && (fepMan->EditorHasFreeSpace() && isnextwordchar) )
                {
                iOwner->ChangeState(ENextWord);
                }
            else
                {
                MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
                MAknFepUICtrlCandidatePaneInterface* predictivePane =
                uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::EJapanesePredictiveCandidatePane);
                // Closeing candidate pop-up list
                predictivePane->HideWindow();
                iOwner->ChangeState(EInitial);
                }
            }

        ret = ETrue;
        }
    
    return ret;
    }
// End of file
