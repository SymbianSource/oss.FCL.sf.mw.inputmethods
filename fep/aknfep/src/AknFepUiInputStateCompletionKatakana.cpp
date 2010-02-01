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
*       Provides the TAknFepInputStateCompletionKatakana methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateCompletionKatakana.h"
#include "AknFepUIManagerStateInterface.h"     // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"          // MAknFepManagerUIInterface
#include "AknFepUICtrlContainerJapanese.h"

#include <PtiDefs.h>                    // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>       // MPtiEngineCompositionDataInterface


/**
*  TAknFepUiInputStateCompletionHiraganaKanji class.
* 
*/

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionKatakana::
//                                  TAknFepUiInputStateCompletionKatakana
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateCompletionKatakana::
    TAknFepUiInputStateCompletionKatakana(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateCompletionJapaneseMultitap(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionKatakana::
//                                  HandleNumericShortKeyL
// Handling short keypress events of the numeric and the star.
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionKatakana::HandleNumericShortKeyL(TInt aKey)
    {
    TBool ret = ETrue;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();

    CommitInlineEditingCharacterL();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->ConvertingString();
    if (text.Length() > 0 && fepMan->EditorHasFreeSpace())
        { // for last character is space or line feed
        iOwner->ChangeState(EInitial);
        iOwner->FepMan()->SimulateKeyEventL(aKey, ETrue);
        ret = ETrue;
        }
    else
        {
        ptiEngine->ClearCurrentWord();
        iOwner->ChangeState(EInitial);
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::UpdateInlineEditingCharacterL
// Update inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionKatakana::UpdateInlineEditingCharacterL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    TBool ret = EFalse;
    
    if(CommitInlineEditingCharacterL())
        {
        ptiEngine->ClearCurrentWord();
        iOwner->ChangeState(EInitial);
        ret = ETrue;
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionJapaneseMultitap::CommitInlineEditingCharacterL
// Commit inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionKatakana::CommitInlineEditingCharacterL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    TPtrC text = data->CompletedString();
    if (text.Length() > 0)
        {
        HBufC* buf = HBufC::NewLC(text.Length());
        TPtr ptr(buf->Des());
        ptr.Copy(text);
        if (fepMan->InputMode() == EHiragana)
        	{
        	for (TInt index=text.Length()-1; index >= 0; index-- )
        		{
        		if (!IsFullHiragana(TChar(text[index])))
        			{
        			ptr.Delete(index, 1);
        			}
        		}
        	}
        	
        fepMan->CommitInlineEditL(ptr, ptr.Length());
        CleanupStack::PopAndDestroy(buf);
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionKatakana::IsFullHiragana
// Check a character whether it is Full-width Katakana
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionKatakana::IsFullHiragana(const TChar aChr)
	{
	TBool rtn = EFalse;
    const TUint KFullHiraganaStart = 0x3041;
    const TUint KFullHiraganaEnd = 0x3094;
	
	if (KFullHiraganaStart <= aChr && aChr <= KFullHiraganaEnd)
		{
		rtn = ETrue;
		}
	
	return rtn;
	}

// ---------------------------------------------------------------------------
// TAknFepUiInputStateCompletionKatakana::HandleAuxKeyL
// Handling auxiliary keypress events including OK and Backspace
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateCompletionKatakana::HandleAuxKeyL(TInt aKey,
                                                           TKeyPressLength aLength)
    {
    TBool ret = ETrue;
    CPtiEngine* ptiEngine = iOwner->PtiEngine();

    if (aKey == EKeyOK
     || aKey == EStdKeyDevice3          /* 0xA7 */
     || aKey == EStdKeyEnter
     || aKey == EStdKeyNkpEnter)
        {
        TPtiEngineCommand ptiCommand = EPtiCommandUserActionComplete;
        if (aLength == ELongKeyPress)
            {
            ptiCommand = EPtiCommandUserActionAllComplete;
            }
        ptiEngine->HandleCommandL(ptiCommand);
        UpdateInlineEditingCharacterL();
        // OK-key or Enter-key shouldn't be consumed in Latin input.
        // Because OK-key or Enter-key might be used in application side.
        ret = EFalse;
        }

    return ret;
    }

// End of file
