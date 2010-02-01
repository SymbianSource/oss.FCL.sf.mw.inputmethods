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
*       Provides the TAknFepInputStateStrokeFindQwerty methods.
*
*/












#include "AknFepUiInputStatelStrokeFindQwerty.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepManagerUIInterface.h"

#include <e32keys.h>
#include <PtiEngine.h>
#include <PtiDefs.h>                //keys

TAknFepInputStateStrokeFindQwerty::TAknFepInputStateStrokeFindQwerty(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseFindQwerty(aOwner, aUIContainer)
    {
    iOwner->PtiEngine()->SetInputMode(EPtiEngineStrokeQwerty);
    }
                                

#ifdef RD_INTELLIGENT_TEXT_INPUT
TAknFepInputStateStrokeFindMiniQwerty::TAknFepInputStateStrokeFindMiniQwerty(
        MAknFepUIManagerStateInterface* aOwner, TInt aMode) :
    TAknFepInputStateInitialChineseGenericQwerty(aOwner, aMode)
    {
    iOwner->PtiEngine()->SetInputMode(EPtiEngineStrokePhraseQwerty);

    iOwner->PtiEngine()->SetKeyboardType(iOwner->FepMan()->KeyboardLayout());

    }

TBool TAknFepInputStateStrokeFindMiniQwerty::HandleKeyL(TInt aKey,
        TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
    TBool ret = EFalse;

    if (aLength == EShortKeyPress && CAknFepFnKeyManager::EFnKeyNone
            == iOwner->FepMan()->FnKeyState()
            && iOwner->IsValidChineseInputKeyQwerty(aKey))
        {
        HandleCaseChange(aKey, ETrue);
        ptiengine->AppendKeyPress((TPtiKey)aKey);
        TPtrC text = ptiengine->GetPhoneticSpelling(1);

        if (text.Length())
            {
            fepMan->NewCharacterL(text);
            fepMan->CommitInlineEditL();
            ptiengine->ClearCurrentWord();
            ret = ETrue;
            }
        }
    else if ( CAknFepFnKeyManager::EFnKeyNone == fnKeyState 
    		&& (aLength == EShortKeyPress && aKey >=EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ ))
    	{
    	ret = ETrue;
    	}
    else if ( fnKeyState != CAknFepFnKeyManager::EFnKeyNone 
    		&& aKey >=EPtiKeyQwertyA 
    		&& aKey <= EPtiKeyQwertyZ )
    	{
    	ret = TAknFepInputStateQwerty::HandleKeyL( aKey, aLength );
    	}
    else
        {
        return TAknFepInputStateInitialChineseGenericQwerty::HandleKeyL(aKey,
                aLength);
        }
    return ret;
    } 



#ifdef __HALF_QWERTY_KEYPAD
TAknFepInputStateStrokeFindHalfQwerty::TAknFepInputStateStrokeFindHalfQwerty(
        MAknFepUIManagerStateInterface* aOwner, TInt aMode) :
    TAknFepInputStateInitialChineseGenericHalfQwerty(aOwner, aMode)
    {
    iOwner->PtiEngine()->SetInputMode(EPtiEngineStrokePhraseHalfQwerty);

    iOwner->PtiEngine()->SetKeyboardType(iOwner->FepMan()->KeyboardLayout());

    }

TBool TAknFepInputStateStrokeFindHalfQwerty::HandleKeyL(TInt aKey,
        TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = EFalse;

    if (iOwner->IsValidChineseInputKeyQwerty(aKey)
            && CAknFepFnKeyManager::EFnKeyNone == iOwner->FepMan()->FnKeyState() && aLength == EShortKeyPress)
        {

        ptiengine->SetCase(EPtiCaseLower);
        ptiengine->SetInputMode(EPtiEngineStrokePhraseHalfQwerty);

        ptiengine->AppendKeyPress((TPtiKey)aKey);
        TPtrC text = ptiengine->GetPhoneticSpelling(1);

        if (text.Length())
            {
			fepMan->StartInlineEditL();
			fepMan->UpdateInlineEditL(text,0);
			fepMan->CommitInlineEditL();
//            fepMan->NewCharacterL(aText);
//            fepMan->CommitInlineEditL();
//            ptiengine->ClearCurrentWord();
            ret = ETrue;
            }
        }
    else if( iOwner->IsValidChineseInputKeyQwerty(aKey) && aLength == ELongKeyPress )
    	{   	
    	TPtrC text;
        TBuf<5> allFnMappedCharacters;
        ptiengine->MappingDataForKey((TPtiKey)aKey, allFnMappedCharacters, EPtiCaseFnLower);
        text.Set(allFnMappedCharacters);
        if (text.Length())
        	{
        	TBuf<1> chr;   
            chr.Append( text[0] );
            fepMan->RemovePreviousCharacterL();
            fepMan->NewCharacterL(chr);
            fepMan->CommitInlineEditL();      
            ptiengine->ClearCurrentWord();                
            iData = 0;
        	}                                 
        ret = ETrue;
    	}
    else
        {
        TPtiTextCase caseMode = ptiengine->Case();
        if (ptiengine->InputMode()!=EPtiEngineHalfQwerty)
            {
            ptiengine->ClearCurrentWord();
            ptiengine->SetInputMode(EPtiEngineHalfQwerty);
            }
        ptiengine->SetCase(caseMode);
        
        return TAknFepInputStateInitialChineseGenericHalfQwerty::HandleKeyL(
                aKey, aLength);
                
                
        /*if (fepMan->FnKeyState() == CAknFepFnKeyManager::EFnKeyNext)
                    fepMan->SetFnKeyState(CAknFepFnKeyManager::EFnKeyNone);*/
        }

    return ret;
    }
#endif

#endif 
// End of file
