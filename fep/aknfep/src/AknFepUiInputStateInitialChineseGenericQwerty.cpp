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
*       Provides the TAknFepInputStateInitialChineseGenericQwerty methods.
*
*/












#include "AknFepUiInputStateInitialChineseGenericQwerty.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepManagerUIInterface.h"

#include "AknFepManager.h"         //FepMan flag

#include <PtiEngine.h>

#ifdef RD_INTELLIGENT_TEXT_INPUT
#include <AknFepFnKeyManager.h>
#endif
const TText KZhuyinTone2 = 0x02CA; // qwerty key6
const TText KZhuyinTone3 = 0x02C7; // qwerty key3
const TText KZhuyinTone4 = 0x02CB; // qwerty key4
const TText KZhuyinTone5 = 0x02D9; // qwerty key7

TAknFepInputStateInitialChineseGenericQwerty::
TAknFepInputStateInitialChineseGenericQwerty(MAknFepUIManagerStateInterface* aOwner, TInt aMode)
    :TAknFepInputStateQwerty(aOwner)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    MPtiLanguage* ptilang = ptiengine->CurrentLanguage(); 
#endif
    
    switch( aMode )
        {
        case EPinyin:
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if (ptilang->HasInputMode(EPtiEnginePinyinPhraseQwerty))
                {         
                ptiengine->SetInputMode(EPtiEnginePinyinPhraseQwerty);
                }
            else
#endif
            ptiengine->SetInputMode(EPtiEnginePinyinQwerty);
            break;
        case EStroke:
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if (ptilang->HasInputMode(EPtiEngineStrokePhraseQwerty))         
                ptiengine->SetInputMode(EPtiEngineStrokePhraseQwerty);
            else
#endif
            ptiengine->SetInputMode(EPtiEngineStrokeQwerty);
            break;
        case EZhuyin:
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if (ptilang->HasInputMode(EPtiEngineZhuyinPhraseQwerty))         
                ptiengine->SetInputMode(EPtiEngineZhuyinPhraseQwerty);
            else
#endif
            ptiengine->SetInputMode(EPtiEngineZhuyinQwerty);
            break;
        case ECangJie:
            ptiengine->SetInputMode(EPtiEngineNormalCangjieQwerty);
            break;
        default:
            break;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    ptiengine->SetKeyboardType(iOwner->FepMan()->KeyboardLayout());        
#endif
    }

TInt TAknFepInputStateInitialChineseGenericQwerty::ToneMark(TInt aKey, TDes& aResult)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TInt number = 0;
    aResult.Zero();
    if(ptiengine)
        {
        TBuf<KMaxName> data;
        ptiengine->MappingDataForKey((TPtiKey)aKey, data, EPtiCaseLower);
        if(data.Length() > 0)
            {
            for(TInt i = 0; i < data.Length(); i++)
                {
                if(data[i] == KZhuyinTone2 || data[i] == KZhuyinTone3 || data[i] == KZhuyinTone4 || data[i] == KZhuyinTone5)
                    {                   
                    if(aResult.MaxLength() > i)
                        {
                    aResult.Append(data[i]);
                    number++;                        
                        }                                            
                    }
                }
            }
        }
    return number;
    }
	
TBool TAknFepInputStateInitialChineseGenericQwerty::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
#ifdef RD_INTELLIGENT_TEXT_INPUT
	CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
	const TPtiKeyboardType keyboardType = ptiengine->KeyboardType();
#endif

    if(iData)
        {
        iData = 0;
        }
    
    if(aKey == EStdKeyLeftFunc && aLength == EShortKeyPress) 
        {
        if (fepMan->IsAbleToLaunchSCT())
            {
            fepMan->LaunchSpecialCharacterTableL();
            }
        }
    else if(iOwner->IsQwertyZhuyinToneMarkKey(aKey) &&
        aLength == EShortKeyPress
#ifdef RD_INTELLIGENT_TEXT_INPUT
        && !fnKeyState   
#endif
        && !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)
        && !fepMan->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed)
        && !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        TBuf<1> ToneMarkBuf;
        if(ToneMark(aKey, ToneMarkBuf) > 0)



                    {



        
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        fepMan->NewCharacterL(ToneMarkBuf);
        fepMan->CommitInlineEditL();
        if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
            {
            fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
            iOwner->FepMan()->TryCloseUiL();
            }
        ret = ETrue;
            }
        }
    else if(iOwner->IsValidChineseInputKeyQwerty(aKey) && 
            !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) && 
#ifdef RD_INTELLIGENT_TEXT_INPUT
            !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) &&
            !fepMan->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed) &&
            !fnKeyState)
#else
            !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode))
#endif
        {
        if ( aLength == EShortKeyPress )
        	{
            ptiengine->ClearCurrentWord();
            ptiengine->ResetToneMark();
            
            if(iOwner->ChangeState(EEntry) == EEntry)
                {
                iOwner->FepMan()->SendUIActivatedNotification();
                }
        	}
        else
        	{
        	return ETrue;
        	}
        }
    else if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) && 
             fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        ret = ETrue;
        }
    else if (IsCharacter(aKey) || IsSCTKey(aKey))
        {
        HandleKeyByShiftOrCharPressed(aKey,aLength);
        if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
            {
            fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);        
            }
        ret = ETrue;
        }
    else if ((IsCharacter(aKey) || aKey == EPtiKeyQwertySpace) 
           && fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
        {
        fepMan->ClearFlag(CAknFepManager::EFlagQwertyChrKeyDepressed);
        fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if((aKey == EPtiKeyQwertySpace) &&EPtiKeyboardQwerty4x12 != ptiengine->KeyboardType())
            {
            TPtrC text;
            _LIT(KSpaceCharacter, " ");
            text.Set(KSpaceCharacter);        
                
            if (text.Length())
                {
                TBuf<1> chr;            
                for (TInt jj = 0; jj < text.Length(); jj++)
                   	{
                   	chr.Zero();
                   	chr.Append(text[jj]);
                    fepMan->NewCharacterL(chr);               	         	                        
               	    fepMan->CommitInlineEditL();
                   	}
                ret = ETrue;
                }                
            }
#endif
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    else if(aKey == EPtiKeyQwertySpace)
        {
        if (fnKeyState == CAknFepFnKeyManager::EFnKeyDown)
            {
            fepMan->LaunchSelectModeMenuL();
            ret = ETrue;
            }
        else
            {
	        TPtrC text;
	        _LIT(KSpaceCharacter, " ");
	        text.Set(KSpaceCharacter);        
	            
	        if (text.Length())
	            {
	            TBuf<1> chr;            
	            for (TInt jj = 0; jj < text.Length(); jj++)
	               	{
	               	chr.Zero();
	               	chr.Append(text[jj]);
	                fepMan->NewCharacterL(chr);               	         	                        
	           	    fepMan->CommitInlineEditL();
	           	    //ptiengine->ClearCurrentWord();
	               	}
	            ret = ETrue;
	            } 
            }
        }
#endif
    return ret;
    }

TBool TAknFepInputStateInitialChineseGenericQwerty::IsSCTKey(TInt aKey)
    {
    TBool response = EFalse;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    CPtiEngine* ptiengine = iOwner->PtiEngine();   
    if(EPtiKeyboardQwerty4x12 == ptiengine->KeyboardType()) 
        {
#endif
    if (IsNumberKey( aKey ) ||
        aKey == EPtiKeyQwertyPlus ||
        aKey == EPtiKeyQwertyMinus ||
        aKey == EPtiKeyQwertyComma ||
        aKey == EPtiKeyQwertySemicolon ||
        aKey == EPtiKeyQwertyFullstop ||
        aKey == EPtiKeyQwertyHash ||
        aKey == EPtiKeyQwertySlash ||
        aKey == EPtiKeyQwertyApostrophe)
        {
        response = ETrue;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
        }
    else if(EPtiKeyboardQwerty4x10 == ptiengine->KeyboardType())
        {
        if(aKey == EPtiKeyQwertyComma ||
           aKey == EPtiKeyQwertyFullstop ||
           aKey == EPtiKeyQwertySlash ||
           aKey == EStdKeySingleQuote ||
           aKey == EStdKeySquareBracketRight ||
           aKey == EStdKeySquareBracketLeft)
            {
            response = ETrue;
            }
        }
    else if(EPtiKeyboardQwerty3x11 == ptiengine->KeyboardType())
    	{
    	 if(aKey == EPtiKeyQwertyComma ||
    		aKey == EPtiKeyQwertyFullstop ||
    	    aKey == EPtiKeyQwertySlash ||
    	    aKey == EStdKeySingleQuote ||
    	    aKey == EStdKeySquareBracketRight ||
    	    aKey == EPtiKeyQwertyAtCharacter )
    		 {
    		 response = ETrue;
    		 }
    	}
#endif
    return response;
    }

//--------------------------------------------------------------------------
// The gate to decide whether it is a character is whether it has valid mapping
// If return ETrue, the key will pass to ptiengine.
//--------------------------------------------------------------------------
//
TBool TAknFepInputStateInitialChineseGenericQwerty::IsCharacter(TInt aKey)
    {
    TBool response = EFalse;
    CPtiEngine* ptiengine = iOwner->PtiEngine(); 
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TPtiTextCase textCase = EPtiCaseLower;
    if (ptiengine && fepMan)
        {
        TBuf<KMaxName> data;
        ptiengine->MappingDataForKey((TPtiKey)aKey, data, textCase);
        if (data.Length() > 0)
            {
        response = ETrue;
            }
        }
		
	// exception Z might has no mapping but need to put into ptiengine.
    if(!response && aKey == EPtiKeyQwertyZ)
        {
        response = ETrue;
        }
    return response;
    }
    
TBool TAknFepInputStateInitialChineseGenericQwerty::HandleKeyByShiftOrCharPressed(TInt aKey, 
                                                                                  TKeyPressLength /*aLength*/)
    {
    TBool ret = EFalse;
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CAknFepCaseManager* caseMan = iOwner->CaseMan(); 

    fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringShiftKeyPress);
    
    TPtiTextCase caseMode = EPtiCaseLower;
    if (fepMan->IsOnlyNumericPermitted())
        {
        return ret;
        }
    TPtiTextCase shiftedCase = caseMode;
    if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode | CAknFepManager::EFlagShiftKeyDepressed)) 
        {
        shiftedCase = EPtiCaseUpper;
        } 
    
    if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) )
        {
        shiftedCase = EPtiCaseChrLower;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if (fepMan->FnKeyState())
        {
        shiftedCase = EPtiCaseFnLower;
        }
#endif
    
    ptiengine->SetCase(shiftedCase);
    

    TPtrC text;
    text.Set(ptiengine->AppendKeyPress((TPtiKey)aKey));
        
    if (text.Length())
        {
        TBuf<1> chr;            
        for (TInt jj = 0; jj < text.Length(); jj++)
           	{
           	chr.Zero();
           	chr.Append(text[jj]);
           	TRAP_IGNORE(fepMan->NewCharacterL(chr));
           	TRAP_IGNORE(fepMan->CommitInlineEditL());
       	    ptiengine->ClearCurrentWord();
           	}
        ret = ETrue;
        }

    return ret;
    }

void TAknFepInputStateInitialChineseGenericQwerty::KeyTimerExpired()
    {
    
    }    
    
// End of file
