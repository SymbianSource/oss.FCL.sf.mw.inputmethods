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
*       Provides the AknFepUiInputStateInitialChineseGenericHalfQwerty methods.
*
*/











#include "AknFepUiInputStateInitialChineseGenericHalfQwerty.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"         //FepMan flag

#include <PtiEngine.h>
#include <AknFepFnKeyManager.h>

const TInt KFnMappedCharacters = 5;
_LIT(KEmotionSmile, "\x003A\x0029");//:)
_LIT(KEmotionLaugh, "\x003A\x0050");//:P
_LIT(KEmotionSad, "\x003A\x0028");//:(
_LIT(KSpace," "); 

// ---------------------------------------------------------------------------
// TAknFepInputStateInitialChineseGenericHalfQwerty::TAknFepInputStateInitialChineseGenericHalfQwerty
// the construt funtion 
// ---------------------------------------------------------------------------
//
TAknFepInputStateInitialChineseGenericHalfQwerty
::TAknFepInputStateInitialChineseGenericHalfQwerty(MAknFepUIManagerStateInterface* aOwner, TInt /*aMode*/)
    :TAknFepInputStateHalfQwerty(aOwner)
    {
    iState = EInitial;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TPtiTextCase caseMode = ptiengine->Case();
    ptiengine->ClearCurrentWord();
    ptiengine->SetInputMode(EPtiEngineHalfQwerty);
    ptiengine->SetCase(caseMode);
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateInitialChineseGenericHalfQwerty::HandleKeyL
// handle key event
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateInitialChineseGenericHalfQwerty::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CAknFepFnKeyManager::TFnKeyState fnKeyState = fepMan->FnKeyState();
    if ( !fepMan->EditorHasFreeSpace() && fnKeyState )
        {
        return EFalse;
        }
    
    if ( aKey == EPtiKeyQwerty0 )
    	{
    	aKey = EPtiKeyQwertySpace;
    	}
    
    if (iData && (iData != aKey) && aKey != EStdKeyBackspace )
        {
        ptiengine->CancelTimerActivity();           
        ptiengine->ClearCurrentWord();        
        fepMan->ClearFlag( CAknFepManager::EFlagInsideMultitapInlineEditingTransaction );
        iData = 0;
        if ( fnKeyState == CAknFepFnKeyManager::EFnKeyNext )
            {
            fepMan->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNone );
            fnKeyState = CAknFepFnKeyManager::EFnKeyNone;
            }
        }
    
    if( aKey == EStdKeyLeftFunc && !fnKeyState) //chr key
        {
        fepMan->LaunchSelectModeMenuL();
        }
    else if ( HandleEmotionKeyL( aKey, aLength ) )
        {
        return ETrue;
        }
    else if ( fnKeyState == CAknFepFnKeyManager::EFnKeyNone && (aKey
        == EPtiKeyQwertyLeftShift || aKey == EStdKeyF21 ) )
        {
        return EFalse;
        }
    else if ( aKey == EStdKeyLeftArrow || aKey == EStdKeyRightArrow || aKey
        == EStdKeyUpArrow || aKey == EStdKeyDownArrow || aKey
        == EStdKeyBackspace )
        {
        if ( fepMan->IsFlagSet( CAknFepManager::EFlagShiftKeyDepressed )
            && fnKeyState )
            {
            return ETrue;
            }
        else
            {
            if ( fnKeyState == CAknFepFnKeyManager::EFnKeyNext )
                {
                fepMan->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNone );
                }
            return EFalse;
            }
        }
    else if ( aKey == EStdKeyDevice0 || aKey == EStdKeyDevice1 || 
        aKey == EStdKeyDevice2 )
        {
        return EFalse;
        }
    else if ( aLength == EShortKeyPress
        && fepMan->IsFlagSet( CAknFepManager::EFlagShiftKeyDepressed )
        && aKey != EStdKeyLeftShift )
        {
        return ETrue;
        }
    else if ( ( IsCharacter( aKey ) && aLength == EShortKeyPress )
        && !fepMan->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed )
        && !fepMan->IsFlagSet( CAknFepManager::EFlagQwertyShiftMode )
        && !fnKeyState )
        {
        ptiengine->ClearCurrentWord();
        ptiengine->ResetToneMark();
        if ( iOwner->IsValidChineseInputKeyQwerty( aKey ) )
            {
            if(iOwner->ChangeState(EEntry) == EEntry)
                {
                iOwner->FepMan()->SendUIActivatedNotification();
                }
            }
        else if( aLength == EShortKeyPress )
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            return ETrue;
            }
        }
    else if ( fepMan->IsFlagSet( CAknFepManager::EFlagQwertyShiftMode )
        && fepMan->IsFlagSet( CAknFepManager::EFlagQwertyChrKeyDepressed ) )
        {
        ret = ETrue;
        }
    else if ( aKey == EPtiKeyQwertyApostrophe && aLength == ELongKeyPress )
        {
        ret = ETrue; 
        }
    else if (IsSCTKey(aKey))
        {
        HandleKeyByShiftOrCharPressedL(aKey,aLength);
        if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
            {
            fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);        
            }
        ret = ETrue;
        }
    else if( aKey == EStdKeySpace )
        {
        if ( aLength == EShortKeyPress)
            {
            fepMan->StartInlineEditL();
            fepMan->UpdateInlineEditL( KSpace, 0 );
            fepMan->CommitInlineEditL();
            if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                {
                fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                }
            }
        else
            {
            TPtrC text;
            TBuf<KFnMappedCharacters> allFnMappedCharacters;
            ptiengine->MappingDataForKey( (TPtiKey)aKey,
                allFnMappedCharacters, EPtiCaseFnLower );
            text.Set( allFnMappedCharacters );
            if ( text.Length() > 0 )
                {
                TBuf<1> chr;
                chr.Append( text[0] );
                fepMan->RemovePreviousCharacterL();
                fepMan->NewCharacterL( chr );
                fepMan->CommitInlineEditL();
                if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                    {
                    fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                    }
                ptiengine->ClearCurrentWord();
                iData = 0;
                }
            }
        ret = ETrue;
        }
    else
        {
        ret = TAknFepInputStateHalfQwerty::HandleKeyL( aKey, aLength );
        if ( fnKeyState == CAknFepFnKeyManager::EFnKeyNext && !IsMultitapkey( aKey ) )
            {
            fepMan->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNone );
            }
        if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
            {
            fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateInitialChineseGenericHalfQwerty::IsSCTKey
// to check whether the key is sct key 
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateInitialChineseGenericHalfQwerty::IsSCTKey(TInt aKey)
    {
    TBool response = EFalse;
    switch (iOwner->FepMan()->InputMode())
        {
        case EPinyin:
            break;
        case EStroke:
        case EZhuyin:
            if (aKey == EPtiKeyQwertyQ ||aKey == EPtiKeyQwertyA ||aKey
                    == EPtiKeyQwertyZ ||aKey == EPtiKeyQwertyO ||aKey
                    == EPtiKeyQwertyL)
                {
                response = ETrue;
                }
            break;
        case EStrokeFind:
        case EZhuyinFind:
            if (aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ)
                {
                response = ETrue;
                }
            break;
        }
    if ( (/*aKey == EStdKeyFullStop || */aKey == EStdKeySpace )
        && iOwner->FepMan()->FnKeyState() )
        {
        response = ETrue;
        }
    return response;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateInitialChineseGenericHalfQwerty::IsCharacter
// to check whether the key stroke is a character key or not 
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateInitialChineseGenericHalfQwerty::IsCharacter(TInt aKey)
    {
    TBool response = EFalse;
    switch (iOwner->FepMan()->InputMode())
        {
        case EPinyin:
            if (( aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ ) ||
            	( aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9 ) )
                {
                response = ETrue;
                }
            break;
        case EStroke:
        case EStrokeFind:
            if ((aKey == EPtiKeyQwertyE ||aKey == EPtiKeyQwertyT ||aKey
                    == EPtiKeyQwertyU ||aKey == EPtiKeyQwertyD ||aKey
                    == EPtiKeyQwertyG ||aKey == EPtiKeyQwertyJ ) || 
               (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty6))
                {
                response = ETrue;
                }
            break;
        case EZhuyin:
            if ( (aKey == EPtiKeyQwertyE ||aKey == EPtiKeyQwertyT ||aKey
                    == EPtiKeyQwertyU ||aKey == EPtiKeyQwertyD ||aKey
                    == EPtiKeyQwertyG ||aKey == EPtiKeyQwertyJ ||aKey
                    == EPtiKeyQwertyC ||aKey == EPtiKeyQwertyB ||aKey
                    == EPtiKeyQwertyM ||aKey == EPtiKeyQwertySpace ) ||
                (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9))
                {
                response = ETrue;
                }
            break;
        }
    return response;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateInitialChineseGenericHalfQwerty::HandleKeyByShiftOrCharPressedL
// Handle the shift key envent 
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateInitialChineseGenericHalfQwerty::HandleKeyByShiftOrCharPressedL(
    TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = EFalse;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringShiftKeyPress);
    TPtiTextCase caseMode = EPtiCaseLower;
    TBool setfnstate = EFalse;
    if (fepMan->IsOnlyNumericPermitted())
        {
        return ret;
        }
    TPtiTextCase shiftedCase = caseMode;
    if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)) 
        {
        shiftedCase = EPtiCaseUpper;
        } 
    if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) )
        {
        shiftedCase = EPtiCaseChrLower;
        }
    if ( fepMan->FnKeyState() )
        {
        shiftedCase = EPtiCaseFnLower;
        }
    if ( aKey == EPtiKeyQwertyQ ||aKey == EPtiKeyQwertyA ||aKey
        == EPtiKeyQwertyZ ||aKey == EPtiKeyQwertyO ||aKey == EPtiKeyQwertyL )
        {
        shiftedCase = EPtiCaseFnLower;
        setfnstate = ETrue;
        }
    ptiengine->SetCase(shiftedCase);
    TPtrC text;
    text.Set(ptiengine->AppendKeyPress((TPtiKey)aKey));
    if (text.Length())
        {
        if ( aLength == ELongKeyPress && !iOwner->FepMan()->FnKeyState() )
            {
            fepMan->RemovePreviousCharacterL();
            }
        TBuf<1> chr;            
        for (TInt jj = 0; jj < text.Length(); jj++)
            {
            chr.Zero();
            chr.Append( text[jj] );
            fepMan->NewCharacterL( chr );
            fepMan->CommitInlineEditL();
            if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                {
                fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                }
            ptiengine->ClearCurrentWord();
            }
        ret = ETrue;
        }
    if(setfnstate && fepMan->FnKeyState() == CAknFepFnKeyManager::EFnKeyNext )
        {
        ptiengine->SetCase(EPtiCaseLower); 
        }    
    
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();    
    if(fnKeyState == CAknFepFnKeyManager::EFnKeyNext)
        fepMan->SetFnKeyState(CAknFepFnKeyManager::EFnKeyNone);
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateInitialChineseGenericHalfQwerty::HandleEmotionKeyL
// Handle the emotion key envent 
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateInitialChineseGenericHalfQwerty::HandleEmotionKeyL( TInt aKey, TKeyPressLength aLength )
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool ret = EFalse;
    TPtrC emotion;
    if ( ( aKey != EPtiKeyQwerty7 &&
         aKey != EPtiKeyQwerty8 &&
         aKey != EPtiKeyQwerty9 ) ||
         fepMan->FnKeyState())
        {
        return EFalse;
        }
    if ( EStroke == fepMan->InputMode() || EStrokeFind == fepMan->InputMode() )
        {
        if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
            {
            fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
            return ETrue;
            }
        else
            {
            switch ( aKey )
                {
                case EPtiKeyQwerty7:
                    emotion.Set( KEmotionSmile );
                    ret = ETrue;
                    break;
                case EPtiKeyQwerty8:
                    emotion.Set( KEmotionLaugh );
                    ret = ETrue;
                    break;
                case EPtiKeyQwerty9:
                    emotion.Set( KEmotionSad );
                    ret = ETrue;
                    break;
                default:
                    break;
                }
            }
        }
    if ( ret )
        {
        if ( aLength == EShortKeyPress)
            {
            fepMan->NewTextL(emotion);
            fepMan->CommitInlineEditL();
            if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                {
                fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                }
            }
        else
            {
            for( TInt i=0; i<emotion.Length(); i++ )
                {
                fepMan->RemovePreviousCharacterL();
                }
            fepMan->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNext );
            fepMan->PtiEngine()->SetCase( EPtiCaseFnLower );
            return EFalse;
            }
        }
    return ret;
    }

TBool TAknFepInputStateInitialChineseGenericHalfQwerty::IsMultitapkey( TInt aKey )
    {
    TBool ret = EFalse;
    if ( aKey == EStdKeyLeftFunc || aKey == EPtiKeyQwertyLeftShift ||
        aKey == EStdKeyFullStop )
        {
        ret = ETrue;
        }
    return ret;
    }
//end of the file
