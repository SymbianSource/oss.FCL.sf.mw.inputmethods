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
*       Provides the TAknFepInputStateZhuyinFindQwerty methods.
*
*/












#include "AknFepUiInputStateZhuyinFindQwerty.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepCaseManager.h"

#include <e32keys.h>
#include <PtiEngine.h>
#include <PtiDefs.h>                //keys

TAknFepInputStateZhuyinFindQwerty::TAknFepInputStateZhuyinFindQwerty(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer) :
    TAknFepInputStateChineseFindQwerty(aOwner, aUIContainer)
    {
    iOwner->PtiEngine()->SetInputMode(EPtiEngineZhuyinQwerty);
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
TAknFepInputStateZhuyinFindMiniQwerty::TAknFepInputStateZhuyinFindMiniQwerty(
        MAknFepUIManagerStateInterface* aOwner, TInt aMode) :
    TAknFepInputStateInitialChineseGenericQwerty(aOwner, aMode)
    {
    iState = EInitial;
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    ptiengine->SetInputMode(EPtiEngineInputModeZhuyinMiniQwertyFind);
    ptiengine->SetKeyboardType(iOwner->FepMan()->KeyboardLayout());

    }

TBool TAknFepInputStateZhuyinFindMiniQwerty::HandleKeyL(TInt aKey,
        TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = EFalse;

    if (iData != aKey)
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
        CAknFepCaseManager* caseMan = iOwner->CaseMan();
        caseMan->UpdateCase(ENullNaviEvent);
        }

    iData = aKey;
    if ( aLength == EShortKeyPress && CAknFepFnKeyManager::EFnKeyNone
        == iOwner->FepMan()->FnKeyState() && (!fepMan->IsFlagSet(
        CAknFepManager::EFlagQwertyShiftMode
            | CAknFepManager::EFlagShiftKeyDepressed )) )
        {
        if (ptiengine->InputMode()!= EPtiEngineInputModeZhuyinMiniQwertyFind)
            {
            ptiengine->SetInputMode(EPtiEngineInputModeZhuyinMiniQwertyFind);
            }
        switch (aKey)
            {
            case EPtiKeyQwerty1:
                aKey = EPtiKeyQwertyQ;
                break;
            case EPtiKeyQwerty2:
                aKey = EPtiKeyQwertyW;
                break;
            case EPtiKeyQwerty3:
                aKey = EPtiKeyQwertyE;
                break;
            case EPtiKeyQwerty4:
                aKey = EPtiKeyQwertyR;
                break;
            case EPtiKeyQwerty5:
                aKey = EPtiKeyQwertyT;
                break;
            case EPtiKeyQwerty6:
                aKey = EPtiKeyQwertyY;
                break;
            case EPtiKeyQwerty7:
                aKey = EPtiKeyQwertyU;
                break;
            case EPtiKeyQwerty8:
                aKey = EPtiKeyQwertyI;
                break;
            case EPtiKeyQwerty9:
                aKey = EPtiKeyQwertyO;
                break;
            case EPtiKeyQwerty0:
                aKey = EPtiKeyQwertyP;
                break;
            default:
                break;
            }

        if (aKey == EPtiKeyQwertyE || aKey == EPtiKeyQwertyR || aKey
                == EPtiKeyQwertyY || aKey == EPtiKeyQwertyU)
            {
            ret= ETrue;
            }
        else if (iOwner->IsValidChineseInputKeyQwerty(aKey) )
            {
        	HandleCaseChange(aKey, ETrue);
            TPtrC text = ptiengine->AppendKeyPress((TPtiKey)aKey);
            if (text.Length())
                {
                fepMan->NewCharacterL(text);
                ret = ETrue;
                }
            }
        else if (aKey == EStdKeyBackspace)
            {
            iOwner->PtiEngine()->DeleteKeyPress();
            }
        else if ( EPtiKeyQwertyComma == aKey )
        	{
        		//show the width comma 
            TPtrC text = ptiengine->AppendKeyPress((TPtiKey)aKey);
            if (text.Length())
                {
                fepMan->NewCharacterL(text);
                ret = ETrue;
                }
        	}
        }
    else
        {
        ptiengine->SetInputMode( EPtiEngineZhuyinPhraseQwerty );
        ptiengine->ClearCurrentWord();        
        if ( !fepMan->IsFlagSet( CAknFepManager::EFlagQwertyShiftMode
            | CAknFepManager::EFlagShiftKeyDepressed ) )
            {
            // the case for Fn mode
            fepMan->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNext );
            }
        ret = TAknFepInputStateInitialChineseGenericQwerty::HandleKeyL( aKey,
            aLength );
        }
    return ret;
    }

void TAknFepInputStateZhuyinFindMiniQwerty::KeyTimerExpired()
    {

    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        TRAP_IGNORE(fepMan->CommitInlineEditL())

        CPtiEngine* ptiengine = iOwner->PtiEngine();
        ptiengine->ClearCurrentWord();

        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
        }

    }

#ifdef __HALF_QWERTY_KEYPAD

TAknFepInputStateZhuyinFindHalfQwerty::TAknFepInputStateZhuyinFindHalfQwerty(
        MAknFepUIManagerStateInterface* aOwner, TInt aMode) :
    TAknFepInputStateInitialChineseGenericHalfQwerty(aOwner, aMode)
    {
    iState = EInitial;
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    ptiengine->SetInputMode(EPtiEngineZhuyinPhraseHalfQwerty);
    ptiengine->SetKeyboardType(iOwner->FepMan()->KeyboardLayout());

    }

TBool TAknFepInputStateZhuyinFindHalfQwerty::HandleKeyL(TInt aKey,
        TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = EFalse;

    if (iData && (iData != aKey))
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
        fepMan->ClearFlag(
                CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
        iOwner->CaseMan()->UpdateCase(ENullNaviEvent);
        }

    iData = aKey;
    
    if (IsValidZhuyinKey(aKey) && CAknFepFnKeyManager::EFnKeyNone
			== iOwner->FepMan()->FnKeyState() && aLength == EShortKeyPress )
		{
		TPtrC text;
		switch (aKey)
			{
			case EPtiKeyQwertyE:
				aKey = EPtiKey1;
				break;
			case EPtiKeyQwertyT:
                aKey = EPtiKey2;
                break;
            case EPtiKeyQwertyU:
                aKey = EPtiKey3;
                break;
            case EPtiKeyQwertyD:
                aKey = EPtiKey4;
                break;
            case EPtiKeyQwertyG:
                aKey = EPtiKey5;
                break;
            case EPtiKeyQwertyJ:
                aKey = EPtiKey6;
                break;
            case EPtiKeyQwertyC:
                aKey = EPtiKey7;
                break;
            case EPtiKeyQwertyB:
                aKey = EPtiKey8;
                break;
            case EPtiKeyQwertyM:
                aKey = EPtiKey9;
                break;
            case EPtiKeyQwertySpace:
                aKey = EPtiKey0;
                break;
			default:
				break;
			}
			ptiengine->SetCase(EPtiCaseLower);
			ptiengine->SetInputMode(EPtiEngineZhuyinPhraseHalfQwerty);
			text.Set(ptiengine->AppendKeyPress((TPtiKey)aKey));
			if (text.Length())
				{
				fepMan->StartInlineEditL();
				fepMan->UpdateInlineEditL(text,0);
				fepMan->CommitInlineEditL();
				ret = ETrue;
				}
			}
    else if ( IsValidZhuyinKey(aKey) && aLength == ELongKeyPress)
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
    else if (IsValidZhuyinKey(aKey) && aLength == ELongKeyPress
            && CAknFepFnKeyManager::EFnKeyLock
                    != iOwner->FepMan()->FnKeyState())
        {
        ret = EFalse;
        }
    else if (fepMan->EditorHasFreeSpace())
        {
        TPtiTextCase caseMode = ptiengine->Case();
        if (ptiengine->InputMode() != EPtiEngineHalfQwerty)
            {
            ptiengine->ClearCurrentWord();
            ptiengine->SetInputMode(EPtiEngineHalfQwerty);
            }
        ptiengine->SetCase(caseMode);

        ret = TAknFepInputStateInitialChineseGenericHalfQwerty::HandleKeyL(
                aKey, aLength);          
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateZhuyinFindHalfQwerty::KeyTimerExpired
// 
// Functions from MPtiObserver interface
// ---------------------------------------------------------------------------
//
void TAknFepInputStateZhuyinFindHalfQwerty::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        if (fepMan->EditorHasFreeSpace())
            {
                TRAP_IGNORE(fepMan->CommitInlineEditL())
            }
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        ptiengine->ClearCurrentWord();

        fepMan->ClearFlag(
                CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
        CAknFepFnKeyManager::TFnKeyState fnKeyState = fepMan->FnKeyState();
        if ( !fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) )
            {
            if (fnKeyState == CAknFepFnKeyManager::EFnKeyNext)
                fepMan->SetFnKeyState(CAknFepFnKeyManager::EFnKeyNone);
            }
        }
    }

TBool TAknFepInputStateZhuyinFindHalfQwerty::IsValidZhuyinKey(TInt aKey)
    {
    if (aKey == EPtiKeyQwertyE|| aKey == EPtiKeyQwertyT|| aKey
            == EPtiKeyQwertyU|| aKey == EPtiKeyQwertyD|| aKey
            == EPtiKeyQwertyG|| aKey == EPtiKeyQwertyJ|| aKey
            == EPtiKeyQwertyC|| aKey == EPtiKeyQwertyB|| aKey
            == EPtiKeyQwertyM|| aKey == EPtiKeyQwertySpace)
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }

    }
#endif //__HALF_QWERTY_KEYPAD

#endif //RD_INTELLIGENT_TEXT_INPUT
// End of file
