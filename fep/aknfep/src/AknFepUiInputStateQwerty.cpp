/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateQwerty methods.
*
*/












#include "AknFepUiInputStateQwerty.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepCaseManager.h"
#include "AknFepUiIndicInputManager.h"


#include <PtiEngine.h>
#include <PtiDefs.h>
#include <PtiKeyMappings.h>
#include <PtiDefs.h>        
#include <centralrepository.h> 
#include <AknFepInternalCRKeys.h>       
#ifdef RD_HINDI_PHONETIC_INPUT	
#include <PtiIndicDefs.h>       
#endif
const TInt KEventIndexForW = 3;
const TInt KEventIndexForP = 2;
const TInt KPtiCapitalEIndex = 19;
const TInt KPtiStarIndex = 0;
const TInt KPtiPlusIndex = 1;
const TInt KPtiMinusIndex = 18;
const TInt KPtiCommaIndex = 17;
const TInt KPtiHashIndex = 4;
const TInt KMaxOutputSizeForAKeyPress = 10;

_LIT(KAknFepPlainNumbers, "1234567890");
const TInt KNumAllowedIdleNumericChars = 22;    
const TUint16 KAllAllowedNumericChars[] =
 	{'*','+','p','w','#','1','2','3','4','5','6','7','8','9','0', ';','.',',','-','E','?','/'};
const TKeyEvent KEventsForSecondPass[]  = 
	{
		{42, EStdKeyNkpAsterisk, 3, 0}, 
    	{43, EStdKeyNkpPlus, 3, 0},
        {112, 80, EModifierAutorepeatable, 0},
        {119, 87, EModifierAutorepeatable, 0},                                           
        {35, EStdKeyHash, EModifierAutorepeatable, 0},
        {EPtiKey1, EPtiKey1, EModifierAutorepeatable, 0},
        {EPtiKey2, EPtiKey2, EModifierAutorepeatable, 0},
        {EPtiKey3, EPtiKey3, EModifierAutorepeatable, 0},
        {EPtiKey4, EPtiKey4, EModifierAutorepeatable, 0},
        {EPtiKey5, EPtiKey5, EModifierAutorepeatable, 0},
        {EPtiKey6, EPtiKey6, EModifierAutorepeatable, 0},
        {EPtiKey7, EPtiKey7, EModifierAutorepeatable, 0},
        {EPtiKey8, EPtiKey8, EModifierAutorepeatable, 0},
        {EPtiKey9, EPtiKey9, EModifierAutorepeatable, 0},
        {EPtiKey0, EPtiKey0, EModifierAutorepeatable, 0},
        {59,       EStdKeySemiColon, EModifierAutorepeatable, 0},        
        {46,       122, EModifierAutorepeatable, 0},
        {44,       121, EModifierAutorepeatable, 0},
   		{45,       130, EModifierAutorepeatable, 0},
   		{101,      69,  EModifierAutorepeatable|EModifierLeftShift, 0}, 
   		{63,       EStdKeyForwardSlash, EModifierAutorepeatable|EModifierLeftShift, 0},
        {47,       123, EModifierAutorepeatable, 0} 
    };

const TInt KFull_SimpleSymbolMapNumber = 8;
const TUint16 KFull_SimpleSymbolMaps[KFull_SimpleSymbolMapNumber][2] = 
      { {0xFF0A,'*'},{0xFF0B,'+'},{0xFF0D,'-'},{0xFF03,'#'},
        {0xFF1B,';'},{0xFF0E,'.'},{0xFF0C,','},{0x3002, '.'}};

const TInt KNumCharKeyExceptions = 14;
const TUint16 CharKeyExceptions[] = {0x00f2, 0x00e0, 0x00e8, 0x00e9, 0x00e7, 0x00f9,
                                     0x0161, 0x010d, 0x017e, 0x00fd, 0x00e1, 0x00ed,
                                     0x00e9, 0x00ec};

TAknFepInputStateQwerty::TAknFepInputStateQwerty(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateBase(aOwner)
    {
    iState = EInitial;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Predictive QWERTY (XT9) changes ---->
    // The order of the following two commands has been swapped. This way the Xt9 core will keep its
    // internal state while a dialog with no prediction is opened on top (like the "Edit word" query).
    ptiengine->SetInputMode(EPtiEngineQwerty);
    ptiengine->ClearCurrentWord();
    // Predictive QWERTY (XT9) changes <----

    MAknFepManagerUIInterface *fepMan = iOwner->FepMan();
    TPtiKeyboardType keyLayout  = fepMan->KeyboardLayout();
    ptiengine->SetKeyboardType(keyLayout);
    if (keyLayout == EPtiKeyboardHalfQwerty)
        {
        ptiengine->SetInputMode(EPtiEngineHalfQwerty);        
        }    
#else 
    ptiengine->ClearCurrentWord();
    ptiengine->SetInputMode(EPtiEngineQwerty);
#endif //RD_INTELLIGENT_TEXT_INPUT
    }

TBool TAknFepInputStateQwerty::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = EFalse;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
#endif
    const TInt KDeleteKeyScanCode = 1;

	if (aKey == KDeleteKeyScanCode 
#ifdef RD_INTELLIGENT_TEXT_INPUT
	        || CAknFepFnKeyManager::EFnKeyNone != fnKeyState
#endif
	        ) 
		{
		// Pending dead key is handled as a special case.
		if (ptiengine->HandleCommandL(EPtiCommandDeadKeyWaiting, NULL) == 1)
			{
			ptiengine->HandleCommandL(EPtiCommandGetAndClearDeadKeyRootChar, NULL);
			return ETrue;
			}			
		if (iFakeDelete)
			{
			ptiengine->HandleCommandL(EPtiCommandClearVowelSeq, NULL);
			ptiengine->HandleCommandL(EPtiCommandGetAndClearLastVietnameseChar, NULL);						
			}
		else
			{
			ptiengine->HandleCommandL(EPtiCommandResetVietnameseVowelSeqAndToneMark, NULL);										
			}
		}
	else if (aKey == EPtiKeyQwertySpace
#ifdef RD_INTELLIGENT_TEXT_INPUT
	         && (fnKeyState == CAknFepFnKeyManager::EFnKeyNone)
#endif
	        )
		{
		// Space clears vietnamese vowels and tones.
		ptiengine->HandleCommandL(EPtiCommandResetVietnameseVowelSeqAndToneMark, NULL);								
		}
	if( aLength == ELongKeyPress )
		{
		// the vowel seq tone mark of the short key press should be reset
		ptiengine->HandleCommandL(EPtiCommandResetVietnameseVowelSeqAndToneMark, NULL);
		}
	iFakeDelete = EFalse;	
	iCharKeyException = EFalse;		
	
#ifdef RD_INTELLIGENT_TEXT_INPUT
	CPtiCoreLanguage* langCur = static_cast<CPtiCoreLanguage*>(ptiengine->CurrentLanguage());	
	TBool chinese = EFalse;
	if(langCur->LanguageCode()==ELangPrcChinese ||
	        langCur->LanguageCode()==ELangHongKongChinese ||
	        langCur->LanguageCode()==ELangTaiwanChinese )
	    {
	    chinese = ETrue;
	    }
    if (chinese && fepMan->IsOnlyNumericPermitted())
        {
        ptiengine->ActivateLanguageL(ELangEnglish);
        ptiengine->SetInputMode(EPtiEngineQwerty);
        ptiengine->SetKeyboardType( iOwner->FepMan()->KeyboardLayout());
        }
    const TBool isQwertyKey = ptiengine->IsValidKey((TPtiKey)aKey);
#else	
	const TBool isQwertyKey = IsQwertyKey(aKey);
#endif	

#ifdef RD_INTELLIGENT_TEXT_INPUT
    if (aKey == EStdKeyEnter)
		{
		fepMan->SimulateKeyEventL(EKeyF19); // Asynchronous case update.
		}
#endif	
    
    if ( !isQwertyKey )
        {
        return EFalse;
        }
#ifndef RD_INTELLIGENT_TEXT_INPUT
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
#endif
    CAknFepCaseManager* caseMan = iOwner->CaseMan(); 

    fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringShiftKeyPress);
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __REVERSE_FN_KEY_SUPPORTED
	//Reverse Fn key mapping needs to be checked.
	if ( fepMan->IsOnlyNumericPermitted() && !fepMan->IsReverseFnkeyInput())    
#else
	if ( fepMan->IsOnlyNumericPermitted() )    
#endif //__REVERSE_FN_KEY_SUPPORTED
		{
		if ((fnKeyState == CAknFepFnKeyManager::EFnKeyNone) && 
		    (aKey == EStdKeySpace || aKey == EStdKeyEnter))
#else
	if ( fepMan->IsOnlyNumericPermitted() )    
		{ 
		if (aKey == EStdKeyEnter)
			{
			return EFalse;
			}
		if (aKey == EStdKeySpace)
#endif
			{
			return ETrue;
			}
		
		return CheckForValidNumberEntryForqwerty(aKey);
		}    
    
    if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) &&
        iData && (iData != aKey) )
        {
        // Chr key is still pressed down but character key was changed from previous one
        // to new one. The previous character is committed to the editor and we continue 
        // prosessing new key.
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
        iData = 0;
        caseMan->UpdateCase(ENullNaviEvent); 
        }

    TBool isCharacter = ETrue;
    if (!iData)
        {
        isCharacter = IsCharacterKey(aKey);
        }

	HandleCaseChange(aKey, isCharacter);
	    
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __REVERSE_FN_KEY_SUPPORTED
	
	TInt origLanguage = ptiengine->CurrentLanguage()->LanguageCode();
	TPtiEngineInputMode origInputMode = ptiengine->InputMode();
	TBool languageChanged = EFalse;
    // In reverse fn key mapping filter the char applicable for
    // Numeric editor.
    if( fepMan->IsOnlyNumericPermitted() &&  fepMan->IsReverseFnkeyInput())
        {
        if( !fepMan->IsValidCharForNumericInFnReverseL(aKey, iOwner->PtiEngine()->Case()) )
            {
             TChar chr;

            if( ! TryGetValidCharForKey(aKey, chr) )
                {
                // If the key in not valid char in the current input language in Fn reverse mode,
                // then we will check if against English language. And if its valid then we will
                // insert the char based out of key for English input language and revert back 
                // the change in input language at the end.

                if(ptiengine->CurrentLanguage()->LanguageCode() == ELangEnglish)
                    {
                    return ETrue;
                    }

                languageChanged = ETrue;
                ptiengine->ActivateLanguageL(ELangEnglish);
       		    ptiengine->SetInputMode(EPtiEngineQwerty);            		

                if( !fepMan->IsValidCharForNumericInFnReverseL(aKey, iOwner->PtiEngine()->Case()) )
                    {
                    if( ! TryGetValidCharForKey(aKey, chr) )
	                    {
	                    ptiengine->ActivateLanguageL(origLanguage);
    	                ptiengine->SetInputMode(origInputMode );  
	
    	                return ETrue;
    	                }
    	            else
    	            	{
    	            	if(languageChanged)
    	            		{
        	        		ptiengine->ActivateLanguageL(origLanguage);
            	    		ptiengine->SetInputMode(origInputMode );            		
                			}

	            		TBuf<1> buf;    
	    	        	buf.Append(chr);
    	    	    	fepMan->NewCharacterL(buf);
        	    		fepMan->CommitInlineEditL();      
	        	    	ptiengine->ClearCurrentWord();                
    	        		iData = 0;
        	    		ptiengine->CancelTimerActivity();
            			return ETrue;    	                
    	            	}
                    }
                }
			else
            	{
	            if(languageChanged)
    	            {
        	        ptiengine->ActivateLanguageL(origLanguage);
            	    ptiengine->SetInputMode(origInputMode );            		
                	}

	            TBuf<1> buf;    
    	        buf.Append(chr);
        	    fepMan->NewCharacterL(buf);
            	fepMan->CommitInlineEditL();      
	            ptiengine->ClearCurrentWord();                
    	        iData = 0;
        	    ptiengine->CancelTimerActivity();
            	return ETrue;
            	}
            }
        }
    
#endif //__REVERSE_FN_KEY_SUPPORTED
#endif //RD_INTELLIGENT_TEXT_INPUT

    iData = aKey;

    TBool keyEventHandledByFep = ETrue;
    TInt languageCode = 0;
    MPtiLanguage* curlang = ptiengine->CurrentLanguage();
    if (curlang)
        {
        languageCode = curlang->LanguageCode();
        }

   TBuf<KMaxOutputSizeForAKeyPress> filtered;   
    if ( keyEventHandledByFep &&
        (isCharacter || isQwertyKey) )
        {        
        TPtrC text;
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
        TBool fnCaseActive = (ptiengine->Case()==EPtiCaseFnLower || ptiengine->Case()==EPtiCaseFnUpper);
        if (aKey == EStdKeySpace && !fnCaseActive /*(fnKeyState == CAknFepFnKeyManager::EFnKeyNone)*/)

#else
        if (aKey == EStdKeySpace && (fnKeyState == CAknFepFnKeyManager::EFnKeyNone))
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__       
#else 
        if (aKey == EStdKeySpace)
#endif
            {
            // PtiEngine does not have mapping for the space key.
            TUint16 deadRoot = ptiengine->HandleCommandL(EPtiCommandGetAndClearDeadKeyRootChar);
            if (deadRoot)
            	{
            	// There was a dead key press waiting. Space will cancel it and
            	// insert the root char.
            	TBuf<1> dbuf;
            	dbuf.Append(deadRoot);
            	text.Set(dbuf);
            	}
            else
            	{
                _LIT(KSpaceCharacter, " ");
            	text.Set(KSpaceCharacter);
            	}
            }
        else if (aKey == EStdKeyEnter)
            {
            // PtiEngine does not have mapping for the enter key.
            const TText KAknFEPLineFeedSymbol = 0x21B2;
            TBuf<1> lineFeed;
            lineFeed.Zero();
            lineFeed.Append(KAknFEPLineFeedSymbol);
            text.Set(lineFeed);
            }
        else
            {
            TPtiTextCase CurCase = ptiengine->Case();
            if(fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric())
                {
                if(CheckForValidNumberEntryForqwerty(aKey))
                    ptiengine->SetCase((TPtiTextCase)EPtiCaseFnLower);
                else
                    ptiengine->SetCase((TPtiTextCase)EPtiCaseLower);
                } 
            // Get keymapping from PtiEngine.                
            text.Set(ptiengine->AppendKeyPress((TPtiKey)aKey));
            // Handle all exception cases.
            CustomFilter(text, filtered, languageCode, caseMan->CurrentCase(), 
                    ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)|| 
                      fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress)));                        
            text.Set(filtered);
#ifdef RD_INTELLIGENT_TEXT_INPUT
			SetShortKeyPressText(text);
            if( text.Length() == 0 ) // no mapping available
                {
                if( ( EStdKeySpace == aKey || EKeySpace == aKey ))
                    {
					// Launching the Edit menu on press of Fn key and space
					if(fnKeyState == CAknFepFnKeyManager::EFnKeyDown)
						fepMan->LaunchSelectModeMenuL();
					else
						{
						// Editor should insert space when function key is in 
						// locked state and space key is pressed.
						_LIT(KSpaceCharacter, " ");
            			text.Set(KSpaceCharacter);
						}
					}
                }
#endif
            if(fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric())
                ptiengine->SetCase(CurCase);
            }
        
        TBuf<5> replacedText;
        if (text.Length())
            {
            MPtiLanguage* lang = ptiengine->CurrentLanguage();
            if (lang->LanguageCode() == ELangVietnamese)
            	{
            	 if (ptiengine->HandleCommandL(EPtiCommandGetAndClearVowelSequence, NULL) > 0)
            	 	{
            	 	// This key press produced Vietnamese vowel sequence.
            	 	// Delete previous char, it will be replaced with accented char.
            	 	// This is done by sending delete event before accented character is
            	 	// added to buffer.
            	 	TInt lastChar = ptiengine->HandleCommandL(EPtiCommandGetAndClearLastVietnameseChar,
            	 	                                          NULL);
            	 	iFakeDelete = ETrue;            	 	
            	 	TKeyEvent delKey = {8, EStdKeyBackspace, 1, 0};        
        	 		CCoeEnv::Static()->SimulateKeyEventL(delKey, EEventKey);				 		
        	 		// Restore last char.
					ptiengine->HandleCommandL(EPtiCommandSetVietnameseLastChar, (TAny*)lastChar);        	 		
            	 	}  
            	 else 
            	 	{            	 
            	 	TUint16 replaced = ptiengine->HandleCommandL(EPtiCommandQueryReplacePrevious, NULL);
            	 	if (replaced)            	 	
            	 		{
            	 		// Remember last key press, since delete will destroy it.
						TInt last = ptiengine->HandleCommandL(EPtiCommandGetVietnameseLastKey, NULL);            	 		
    					if(!fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
    					    iFakeDelete = ETrue;
 						TKeyEvent delKey = {8, EStdKeyBackspace, 1, 0};        
        	 			CCoeEnv::Static()->SimulateKeyEventL(delKey, EEventKey);  
        	 			replacedText.Append(replaced);
        	 			replacedText.Append(text);
        	 			text.Set(replacedText);   	       	 			
        	 			// Restore last key.
        	 			ptiengine->HandleCommandL(EPtiCommandSetLastKeyForVietnamese, (TAny*)last);
            	 		}            	 	
            	 	}            	
            	}
            
            // Because of dead key functionality there may be several characters waiting.
            TBuf<1> chr;            
            for (TInt jj = 0; jj < text.Length(); jj++)
              	{
              	chr.Zero();
               	chr.Append(text[jj]);
	            fepMan->NewCharacterL(chr);               	         	                        
            	if ( !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) ||
            	     (ELongKeyPress == aLength) )
            	    {
               		// Commit the character if chr key is not pressed.
               	    fepMan->CommitInlineEditL();
               	    ptiengine->ClearCurrentWord();
               	    iData = 0;
                	}
            	else
                   {
                   fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);
                   }
              	}
            ret = ETrue;
            }
        else
            {
            if (ptiengine->HandleCommandL(EPtiCommandDeadKeyWaiting, NULL) == 1)
            {
            if (caseMan->CurrentCase() == EAknEditorTextCase
             || caseMan->CurrentCase() == EAknEditorUpperCase)
            	{
            	ptiengine->SetCase(EPtiCaseUpper);	            	
            	}            
            else if (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) || 
                    fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress))
            	{
            	ptiengine->SetCase(EPtiCaseLower);	
            	}
           	}
#ifdef RD_INTELLIGENT_TEXT_INPUT           	
           	else
           	    {
                fepMan->SimulateKeyEventL(EKeyF19); 
           	    }
#endif            	
            ret = ETrue;
            }
        }
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __REVERSE_FN_KEY_SUPPORTED
	
    if(languageChanged)
        {
       	ptiengine->ActivateLanguageL(origLanguage);
     	ptiengine->SetInputMode(origInputMode );            		
        }
    
#endif //__REVERSE_FN_KEY_SUPPORTED
#endif //RD_INTELLIGENT_TEXT_INPUT    

    return ret;
    }

// Functions from MPtiObserver interface
void TAknFepInputStateQwerty::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        TRAP_IGNORE(fepMan->CommitInlineEditL())
        //do something if error??

        CPtiEngine* ptiengine = iOwner->PtiEngine();
        ptiengine->ClearCurrentWord();

        CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
        caseMan->UpdateCase(ENullNaviEvent); 

        iData = 0;
        }
    }



TBool TAknFepInputStateQwerty::IsExceptionChar(TUint16 aChar)
	{
    for (TInt i = 0; i < KNumCharKeyExceptions; i++)
      	{
       	if (CharKeyExceptions[i] == aChar)	
       		{ 		
       		return ETrue;
       		}
   		} 	
	
	return EFalse;		
	}


TBool TAknFepInputStateQwerty::IsCharacterKey(const TInt aKey)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    // Max length doesn't matter because we'll need only the first character.    
    TBuf<KMaxName> lowerdata;   
    
    ptiengine->MappingDataForKey((TPtiKey)aKey, lowerdata, EPtiCaseLower);        
    if (lowerdata.Length() > 0)
    	{
        TChar lower = lowerdata[0];
        
        if (IsExceptionChar(lower))
       	    {
       	    TBuf<KMaxName> upperdata; 
			ptiengine->MappingDataForKey((TPtiKey)aKey, upperdata, EPtiCaseUpper);                   	    
			if (upperdata.Length() > 0)
				{
				TChar upperchr = upperdata[0];
				upperchr.LowerCase();
				if (upperchr != lower)
					{
		        	iCharKeyException = ETrue;       		
	    	    	return EFalse;        													
					}								
				}       	    
       		} 
        
    	TUint upper = lower.GetUpperCase();
    
	    if ( TUint(lower) != upper )
    	    {
   			return ETrue; 	
       		}
#ifdef RD_INTELLIGENT_TEXT_INPUT
	    //Characters from languages without case changes like Arabic/Urdu 
	    //cannot be identified with above logic. Hence, a special check below:
	    else if ( IsLanguageWithoutCaseChanges() && fepMan->IsOnlyNumericPermitted() && lower.IsAlpha() )
	        {
	        return ETrue;
	        }
#endif
     	}
    
    return EFalse;
    }

TBool TAknFepInputStateQwerty::IsNumberKey(const TInt aKey)
    {
    TBool response = EFalse;
    if (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9)
        {
        response = ETrue;
        }
    return response;
    }

TBool TAknFepInputStateQwerty::IsQwertyKey(const TInt aKey)
    {
    TBool response = EFalse;
    if ( (aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) ||
        IsNumberKey( aKey ) ||
        aKey == EStdKeySpace ||
        aKey == EStdKeyEnter ||
        aKey == EPtiKeyQwertyPlus ||
        aKey == EPtiKeyQwertyMinus ||
        aKey == EPtiKeyQwertyComma ||
        aKey == EPtiKeyQwertySemicolon ||
        aKey == EPtiKeyQwertyFullstop ||
        aKey == EPtiKeyQwertyHash ||
        aKey == EPtiKeyQwertySlash ||
        aKey == EPtiKeyQwertyApostrophe
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        || aKey == EPtiKeyQwertyAtCharacter
#endif 
        )
        {
        response = ETrue;
        }
    return response;
    }



#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool TAknFepInputStateQwerty::IsCharacterNumber(TChar aChar)
    {
    TBool isNumber;
    isNumber =     (iOwner->NumberModeKeyMapping().Locate(aChar) != KErrNotFound) || 
        (KAknFepPlainNumbers().Locate(aChar) != KErrNotFound) || 
        iOwner->FepMan()->IsAllowedKeymappingForNumberMode(aChar);
    return isNumber;
    }
#endif
        
void TAknFepInputStateQwerty::NumericCharacterForQwertyKey(TInt aInScanCode,
                                                           TKeyEvent& aEvent,
                                                           TBool aShifted, TChar& aResult)
	{  
 	CPtiEngine* ptiengine = iOwner->PtiEngine();	
#ifdef RD_INTELLIGENT_TEXT_INPUT
 	MAknFepManagerUIInterface *fepMan = iOwner->FepMan();			
#endif
	aResult = 0;
	TUint16 firstChar;
	
	const TInt KMaxNumCharsPerCase = 50;
	TBuf<KMaxNumCharsPerCase> keys;	
	TInt langCode = 0;
	
	CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>(ptiengine->CurrentLanguage());		
	if (lang)
		{
		langCode = lang->LanguageCode();
		if (langCode == ELangFarsi || langCode == ELangUrdu || langCode == ELangHindi 
		 || langCode == ELangPrcChinese || langCode == ELangHongKongChinese 
		 || langCode == ELangTaiwanChinese || langCode == ELangArabic
		 || TAknFepUiIndicInputManager :: IsIndicLangauge((TLanguage) langCode )
#ifdef RD_HINDI_PHONETIC_INPUT	
		 || TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage( (TLanguage)langCode )
#endif
		)
			{
			lang = static_cast<CPtiCoreLanguage*>(ptiengine->GetLanguage(ELangEnglish));			
			}				
		}
	
	if (lang)
		{
		
		// If the key have scancode EPtiKeyQwertyW, EPtiKeyQwertyP map to any valid
		// numeric character that valid numeric character never be inserted.
		// Earlier this was not happening, because key map with numeric key always

		
		CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());
        TBool keyNotMapToNumeric(ETrue);
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if(maps)
            {
            maps->GetDataForKey((TPtiKey)aInScanCode, keys, EPtiCaseFnLower);
            keyNotMapToNumeric = !(keys.Length() && IsCharacterNumber(keys[0]));
            }
#endif
		
		if (keyNotMapToNumeric && langCode == ELangVietnamese && aInScanCode == EPtiKeyQwertyW &&
			!aShifted)
			{
			// This is handled as a special case because 'w' is normally produced by double press
			// of w-key in Vitenamese but in number mode that can't be done.
			aResult = 'w';
			aEvent = KEventsForSecondPass[KEventIndexForW];
			return;
			}
			
		if (langCode == ELangHindi 
#ifdef RD_MARATHI
		|| langCode == ELangMarathi
#endif // RD_MARATHI
		    )
			{
			if (aInScanCode == EPtiKeyQwertyPlus && aShifted)
				{
				aResult = '-';
				aEvent = KEventsForSecondPass[KPtiMinusIndex];
				return;													
				}
			else if (aInScanCode == EPtiKeyQwertyMinus && !aShifted)
				{
				return;
				}				
			}
			
		if (langCode == ELangArabic)
			{				
			if (aInScanCode == EPtiKeyQwertyComma && !aShifted)			
				{
				aResult = ',';
				aEvent = KEventsForSecondPass[KPtiCommaIndex];
				return;					
				}				
			else if (aInScanCode == EPtiKeyQwertyE && aShifted)	
				{				
				aResult = 'E';
				aEvent = KEventsForSecondPass[KPtiCapitalEIndex];
				return;									
				}				
			}
			
		if (langCode == ELangFarsi)	
			{
			// Farsi uses english numeric keymappings with following exceptions.
			if ((aInScanCode == EPtiKeyQwertyPlus && !aShifted) ||
			    (aInScanCode == EPtiKeyQwertyComma && !aShifted))
				{
				return;   // Block
				}
			
			if (aInScanCode == EPtiKeyQwerty7 && aShifted)
				{
				aResult = '*';
				aEvent = KEventsForSecondPass[KPtiStarIndex];
				return;					
				}
			if (aInScanCode == EPtiKeyQwerty8 && aShifted)
				{
				aResult = '-';
				aEvent = KEventsForSecondPass[KPtiMinusIndex];
				return;					
				}				
			if (aInScanCode == EPtiKeyQwerty9 && aShifted)
				{
				aResult = '+';
				aEvent = KEventsForSecondPass[KPtiPlusIndex];
				return;									
				}				
			if (aInScanCode == EPtiKeyQwerty2 && aShifted)
				{
				aResult = ',';
				aEvent = KEventsForSecondPass[KPtiCommaIndex];
				return;									
				}								
			}
		else if (langCode == ELangUrdu)	
			{
			// urdu uses english numeric keymappings with following exceptions.	
			if (aInScanCode == EPtiKeyQwerty0 && aShifted)
				{
				aResult = '+';
				aEvent = KEventsForSecondPass[KPtiPlusIndex];
				return;					
				}			
			if (aInScanCode == EPtiKeyQwerty3 && aShifted)
				{
				aResult = '#';
				aEvent = KEventsForSecondPass[KPtiHashIndex];
				return;					
				}			
			if (aInScanCode == EPtiKeyQwertyP)
				{
				if (iOwner->NumberModeKeyMapping().Locate('p') == KErrNotFound)
					{
					if (!aShifted)
						{
						aResult = '-';
						aEvent = KEventsForSecondPass[KPtiMinusIndex];
						return;											
						}
					else
						{											
						return;																	
						}
					}
				else
					{
					if (aShifted)
						{
						aResult = '-';
						aEvent = KEventsForSecondPass[KPtiMinusIndex];
						return;															
						}
					else
						{
						aResult = 'p';
						aEvent = KEventsForSecondPass[KEventIndexForP];
						return;					
						
						}					
					}									
				}							
			}
		
		// Handle other languages, which don't have 'p' and 'w' in normal keymapping data.		
		
        if( keyNotMapToNumeric &&
		    (langCode == ELangGreek || langCode == ELangRussian ||
		    langCode == ELangArabic || langCode == ELangHebrew || 
		    langCode == ELangBulgarian || langCode == ELangThai ||
		    langCode == ELangUkrainian) && !aShifted)	
		   {
		   if (aInScanCode == EPtiKeyQwertyW)
		   	   {
		   	   aResult = 'w';
			   aEvent = KEventsForSecondPass[KEventIndexForW];
			   return;
		   	   }
		   else if (aInScanCode == EPtiKeyQwertyP)
		       {
	   	   	   aResult = 'p';
			   aEvent = KEventsForSecondPass[KEventIndexForP];
			   return;		   		
		   	   }		   		   
		   }
							

		if (maps)
			{
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if(aShifted)
                {
                maps->GetDataForKey((TPtiKey)aInScanCode, keys, EPtiCaseUpper);
                }
            else
                {            
                maps->GetDataForKey((TPtiKey)aInScanCode, keys, EPtiCaseFnLower);
                if( !(keys.Length() && IsCharacterNumber(keys[0])))
                    maps->GetDataForKey((TPtiKey)aInScanCode, keys, EPtiCaseFnUpper);
                if( !(keys.Length() && IsCharacterNumber(keys[0])))
                    maps->GetDataForKey((TPtiKey)aInScanCode, keys, EPtiCaseLower);
                if( !(keys.Length() && IsCharacterNumber(keys[0])))
                    maps->GetDataForKey((TPtiKey)aInScanCode, keys, EPtiCaseUpper);                
                if( !(keys.Length() && IsCharacterNumber(keys[0])))
                    keys.SetLength(0);
                }
                          
#else 
			if (aShifted)
				{
				maps->GetDataForKey((TPtiKey)aInScanCode, keys, EPtiCaseUpper);				
				}
			else
				{
				maps->GetDataForKey((TPtiKey)aInScanCode, keys, EPtiCaseLower);				
				}
#endif
			if ( keys.Length() > 0 )
			    {
        		switch (keys[0])		
        			{
        			case KPtiPinyinMarker:
        			case KPtiStrokeMarker:
        			case KPtiZhuyinMarker:
        			case KPtiCangjieMarker:
					case KPtiGetAllMarker:        			
        			     if (keys.Length() > 1)
        			        firstChar = keys[1];
        			     else
        			        firstChar = 0;
        			     break;
        			default:			     
        				 firstChar =  keys[0];			     
        			}
        						
                for (TInt j = 0; j < KFull_SimpleSymbolMapNumber; j++)
                    {
                    if (firstChar == KFull_SimpleSymbolMaps[j][0])
                        {
                        firstChar = KFull_SimpleSymbolMaps[j][1];
                        break;
                        }             
                    }  
                    										
			for (TInt i = 0; i < KNumAllowedIdleNumericChars; i++)
				{			
				if (firstChar == KAllAllowedNumericChars[i])								
					{
					aEvent = KEventsForSecondPass[i];
					aResult = KAllAllowedNumericChars[i];																					
					break;						
						}									
					}						
			    }			    
			}
		}			
	}    
        
           
    
void TAknFepInputStateQwerty::CustomFilter(TPtrC aInput, TDes& aOutput, TInt /*aLanguage*/, 
                                           TInt aCurrentCase, TBool /*aShiftDown*/)
    {   
    aOutput.Zero();
    
    if (aInput.Length() && aInput.Length() <= aOutput.MaxLength())
        {
        aOutput.Copy(aInput);       
        }

    if (aOutput.Length())       
        {      
        if (aCurrentCase == EAknEditorTextCase || aCurrentCase == EAknEditorUpperCase)
            {                                  
            if (iCharKeyException && IsExceptionChar(aOutput[0]))
            	{           
            	TChar chr = aOutput[0];
            	chr.UpperCase();
            	aOutput[0] = chr;            	
            	}
#ifndef RD_INTELLIGENT_TEXT_INPUT
            else if (aLanguage == ELangCzech)	
            	{
            	if (aOutput[0] == '/' && !aShiftDown)
            		{
            		aOutput[0] = 0x00c4;
            		}				   			            		            		            	            	
            	}                                                  
#endif // RD_INTELLIGENT_TEXT_INPUT
            }
        }       
    }
    
void TAknFepInputStateQwerty::HandleCaseChange(TInt aKey, TBool aIsCharacter)
	{
    TPtiTextCase caseMode = EPtiCaseLower;

    if (aIsCharacter)
        {
        if( !( iOwner->FepMan()->IsOnlyNumericPermitted() && !IsNumberKey(aKey) ) )
        	{
        	switch(iOwner->CaseMan()->CurrentCase())
           		{
            	case EAknEditorLowerCase:
                	caseMode = EPtiCaseLower;
	                break;            
    	        case EAknEditorTextCase:
        	    case EAknEditorUpperCase:
            	   caseMode = EPtiCaseUpper;
	            default:
    	           break;
        	    }
        	}
        else
        	{
        	caseMode = iOwner->PtiEngine()->Case();
        	}
       	}
 
    TPtiTextCase shiftedCase = caseMode;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    
    // If Fn key state is EFnKeyNone, Synchronise PtiEngine Textcase 
    if( CAknFepFnKeyManager::EFnKeyNone == iOwner->FepMan()->FnKeyState() )
        {
        iOwner->PtiEngine()->SetCase((TPtiTextCase)EPtiCaseLower);
        }
    else
        {
        if( (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)||
             iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) ))
            {
            iOwner->PtiEngine()->SetCase((TPtiTextCase)EPtiCaseFnUpper);
            }        
            
        TPtiTextCase curCase = iOwner->PtiEngine()->Case();
        if( EPtiCaseFnUpper  == curCase || EPtiCaseFnLower  == curCase )
    	    {    	    
    		if( EPtiCaseFnUpper == curCase )
    			{
    	    	TBuf<32> keyMappings;
    	    	iOwner->PtiEngine()->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseFnUpper);
    			
    			if( keyMappings.Length() == 0)	
    				{
    		    	iOwner->PtiEngine()->SetCase((TPtiTextCase)EPtiCaseFnLower);
    				}
    			}    	    
    	    return;	      	
    	    }
        }
#endif
    if ( (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)||
          iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) )&& 
       ( !aIsCharacter || iOwner->CaseMan()->IsAbleChangeCase() ) )
        {
        // Case mode is changed to opposite because shift key is/was pressed and
        // in case of character key case changes are allowed. In case of
        // number or special key, shift mode is used always when 
        // shift key is pressed
        switch ( caseMode )
            {
            case EPtiCaseUpper:
                shiftedCase = EPtiCaseLower;
                break;
            case EPtiCaseLower:
                shiftedCase = EPtiCaseUpper;
                break;
            default:
                break;
            }
        }
    
    if ( iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) )
        {
        // Chr key is pressed down. Chr mode is used.
        if (shiftedCase == EPtiCaseLower)
            {
            shiftedCase = EPtiCaseChrLower;
            }
        else if (shiftedCase == EPtiCaseUpper)
            {
            shiftedCase = EPtiCaseChrUpper;
            }
        }
    
    iOwner->PtiEngine()->SetCase(shiftedCase);
	
	}

#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool TAknFepInputStateQwerty::TryGetValidCharForKey(const TInt& aKey, TChar& aChar)
    {    
    if( ! TryGetValidCharForKeyAndCase( aKey, aChar, EPtiCaseLower) )
        {
        if(! TryGetValidCharForKeyAndCase( aKey, aChar, EPtiCaseUpper))
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
            //Do not check for FnUpper and FnLower case for Greek/Cyrillic/Hebrew script.
            //This is for handling special characters in numeric editors.
            if ( IsSpecialNonLatinScript() )
                {
                return EFalse;
                }
#endif
            if(! TryGetValidCharForKeyAndCase( aKey, aChar, EPtiCaseFnLower))
                {
                if(! TryGetValidCharForKeyAndCase( aKey, aChar, EPtiCaseFnUpper))
                	{
                	return EFalse;	
                	}
                }
            }
        }
    return ETrue;
    }
    
TBool TAknFepInputStateQwerty::TryGetValidCharForKeyAndCase(const TInt& aKey, TChar& aChar, TPtiTextCase aCase)
    {
    TBuf<KMaxName> mappedCharacters;

    iOwner->PtiEngine()->MappingDataForKey((TPtiKey)aKey, mappedCharacters, aCase);        
    for (TInt i = 0; i < mappedCharacters.Length(); i++ )
        {
        if( iOwner->FepMan()->IsValidCharInNumericEditorL( mappedCharacters[i] ) )
            {
            aChar = mappedCharacters[i];
            return ETrue;
            }
        }    
    return EFalse;
    }	

TBool TAknFepInputStateQwerty::IsLanguageWithoutCaseChanges()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TInt currLanguageCode = ptiengine->CurrentLanguage()->LanguageCode();
    //update below list as and when required.
    if (    ELangArabic == currLanguageCode ||
            ELangFarsi == currLanguageCode ||
            ELangHebrew == currLanguageCode ||
            ELangHindi == currLanguageCode ||
            ELangUrdu == currLanguageCode
        )
        {
        return ETrue;
        }
    return EFalse;        
    }

TBool TAknFepInputStateQwerty::IsSpecialNonLatinScript()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TInt currLanguageCode = ptiengine->CurrentLanguage()->LanguageCode();
    //update below list as and when required.
    if (    ELangGreek == currLanguageCode ||
            ELangRussian == currLanguageCode ||
            ELangUkrainian == currLanguageCode ||
            ELangBulgarian == currLanguageCode ||
            ELangHebrew == currLanguageCode 
        )
        {
        return ETrue;
        }
    return EFalse;    
    }
#endif

TBool TAknFepInputStateQwerty::CheckForValidNumberEntryForqwerty(TInt aKey)
    {
    TChar chr;							
    TKeyEvent numKey;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    NumericCharacterForQwertyKey(aKey, numKey,
                                 ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)|| 
                                       fepMan->IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress) ), chr);																							 	
    if ((chr != 0) &&
        ((iOwner->NumberModeKeyMapping().Locate(chr) != KErrNotFound) ||
         (KAknFepPlainNumbers().Locate(chr) != KErrNotFound)) ||
         fepMan->IsAllowedKeymappingForNumberMode(chr)) 
        {					
        if (chr == 'E')
            {
            TBuf<1> chrTmp;
            chrTmp.Append(chr);
            fepMan->NewCharacterL(chrTmp);               	         	                        
            fepMan->CommitInlineEditL();			
            }
        else
            {							
            // If editor is in number mode and incoming key press would produce
            // any of the allowed number mode characters, 
            // resend suitable key event let it pass through FEP. That way phone idle
            // receives the event too. This also takes care of remapping
            // different qwerty keymap layouts to edwin events.
            if(!(fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric()))
                fepMan->SetFlag(CAknFepManager::EFlagPassNextKey);   						 			
            CPtiEngine* ptiengine = iOwner->PtiEngine();
            CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>(ptiengine->CurrentLanguage());
            
            if (lang && (numKey.iScanCode >= EPtiKeyQwerty0 && numKey.iScanCode <= EPtiKeyQwerty9))
                {
                const TInt langCode = lang->LanguageCode();
                    
                if(langCode == ELangUrdu || langCode == ELangFarsi)
                    {
                    numKey.iCode += KLatinToEasternArabicIndicDigitsDelta;
                    numKey.iModifiers = numKey.iModifiers|EModifierSpecial;	
                    }
                else if (langCode == ELangArabic && fepMan->ArabicIndicDigitsInUse())
                    {
                    numKey.iCode += KLatinToArabicIndicDigitsDelta;
                    numKey.iModifiers = numKey.iModifiers|EModifierSpecial;
                    }
                else if(TAknFepUiIndicInputManager::IsIndicLangauge(TLanguage(langCode))
#ifdef RD_HINDI_PHONETIC_INPUT
                || TAknFepUiIndicInputManager::IsIndicPhoneticLanguage(TLanguage(langCode))
#endif
                )
                    {
                    numKey.iCode += KLatinToDevanagariDigitsDelta;							
                    numKey.iModifiers = numKey.iModifiers|EModifierSpecial;							
                    }
                }
            // Disable UI framewroks's qwerty iCode conversion for simulated event.	
            numKey.iModifiers |= EModifierSpecial;					
            if(!(fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric()))
                {
                CCoeEnv::Static()->SimulateKeyEventL(numKey, EEventKey);                        
#ifdef RD_INTELLIGENT_TEXT_INPUT
                CCoeEnv::Static()->SimulateKeyEventL(numKey, EEventKeyUp);
#endif
                }
            }
        }										  						 	                                  
						
    if((fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric())
                &&(numKey.iCode == 112 /*p*/ || numKey.iCode == 119/*w*/))
        return EFalse;    
    else if(!fepMan->IsHybridAplhaEditor() || fepMan->IsHybridAlphaModeChangedtoAplhanumeric())
        return ETrue;
    else if(!fepMan->IsHybridAlphaModeChangedtoAplhanumeric() && 
            (chr !=0 || (chr==0 && fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode))))
        return ETrue;
    else
        return EFalse; 
    }
// End of file
