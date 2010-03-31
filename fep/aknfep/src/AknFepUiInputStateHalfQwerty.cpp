/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateHalfQwerty methods.
*
*/












#include "AknFepUiInputStateHalfQwerty.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepCaseManager.h"

#include <PtiEngine.h>
#include <PtiDefs.h>
#include <PtiKeyMappings.h>
#include <centralrepository.h> 
#include <AknFepInternalCRKeys.h>       
  

TAknFepInputStateHalfQwerty::TAknFepInputStateHalfQwerty(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateBase(aOwner)
    {
    iState = EInitial;
    iData = 0;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    ptiengine->ClearCurrentWord();
    ptiengine->SetInputMode(EPtiEngineHalfQwerty);
    }

TBool TAknFepInputStateHalfQwerty::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = EFalse;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    if( ptiengine == NULL || aKey == EStdKeyNull || 
    		aKey == EStdKeyF21 || aKey == EKeyF21 || aKey == EKeyF20 || aKey == EKeyF19 ||
		aKey == EStdKeyLeftArrow || aKey == EStdKeyRightArrow || aKey == EStdKeyDownArrow || aKey == EStdKeyUpArrow )
        return ret;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();    
    TPtiTextCase CurCase = ptiengine->Case();

    if (iData && (iData != aKey) )
        {
        ptiengine->CancelTimerActivity();        
        fepMan->CommitInlineEditL();      
        ptiengine->ClearCurrentWord();        
        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
        iData = 0;
        }
    // if it is a long key press of the punctuation key,
		 
	  if( aLength == ELongKeyPress && aKey == EStdKeyFullStop)
	  	{
	  	// Commit the current word and then do the usual stuff
	  	ptiengine->CancelTimerActivity();	  		  	
	  	}
	if( !fepMan->EditorHasFreeSpace() && !iData )
		{
		// Do not proceed as we have reached max limit and timer has expired
		ptiengine->CancelTimerActivity();
		ptiengine->ClearCurrentWord();        
        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
        return 0;
        }
    
    if( EStdKeyEnter == aKey )
		{
		// Asynchronous case update.
		fepMan->SimulateKeyEventL(EKeyF19);
		return 0;
		}
		
    if (!iOwner->PtiEngine()->IsValidKey((TPtiKey)aKey))
        {     
        iData = 0;
        ChrKeyTimerExpired();
        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);        
        ptiengine->CancelTimerActivity();		                
        return ret;
        }
  TPtrC text;
  /*
        if ( fepMan->IsOnlyNumericPermitted() )    
            {
            ptiengine->CancelTimerActivity();       
            text.Set(ptiengine->AppendKeyPress((TPtiKey)aKey)); 
            TPtrC numericChars(KAllAllowedNumericChars, KNumAllowedIdleNumericChars);   
            if (text.Length() && numericChars.Locate(text[0]) != KErrNotFound)
                {
                TBuf<1> chr;    
                chr.Append(text[0]);
                fepMan->NewCharacterL(chr);             
                ptiengine->CancelTimerActivity();        
                fepMan->CommitInlineEditL();      
                ptiengine->ClearCurrentWord();                                      
                }
            
            return ETrue;
            }
 */
#ifdef __REVERSE_FN_KEY_SUPPORTED
	TInt origLanguage = ptiengine->CurrentLanguage()->LanguageCode();
    TPtiEngineInputMode origInputMode = ptiengine->InputMode();
    TBool languageChanged = EFalse;
		
    // In reverse fn key mapping filter the char applicable for
    // Numeric editor.
    if( fepMan->IsOnlyNumericPermitted() )
        {
        
        if( EStdKeyLeftFunc == aKey )
            {
            TCallBack timerExpire(TimerExpire, this);
            fepMan->MultitapThroughSCTCharL(timerExpire); 
            return ETrue;
            }
        else 
            {
            ChrKeyTimerExpired();
            }
        if( !fepMan->IsValidCharForNumericInFnReverseL( aKey, iOwner->PtiEngine()->Case() ) )
            {
            if(CheckHasValidNumerKeyMappedL(aKey,languageChanged))
                return ETrue;
            }
        }
    
#endif //__REVERSE_FN_KEY_SUPPORTED


//    if( aLength == EShortKeyPress )
        {
        /*
        if (aKey == EStdKeySpace && (fnKeyState == CAknFepFnKeyManager::EFnKeyNone))
            {
            _LIT(KSpaceCharacter, " ");
            text.Set(KSpaceCharacter);        
            }        
        else*/ 
   if( aKey == EStdKeySpace && fepMan->FnKeyState() == CAknFepFnKeyManager::EFnKeyNone)
    		{
    		_LIT(KSpace," "); 
			fepMan->StartInlineEditL();
            fepMan->UpdateInlineEditL(KSpace,0);
            fepMan->CommitInlineEditL();
        	return ETrue;        		
    		}    
        else
            {
            TBool isCharacter = ETrue;
            if (!iData)
                {
                isCharacter = IsCharacterKey(aKey);
                }
            HandleCaseChange(aKey, isCharacter);        
            CurCase = ptiengine->Case();

            if(( (fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric())
                    || fepMan->IsPhoneNumberEditor() )
                    && fepMan->FnKeyState() != CAknFepFnKeyManager::EFnKeyNone)
            {            
            if( EStdKeyLeftFunc == aKey )
                {
                TCallBack timerExpire(TimerExpire, this);
                fepMan->MultitapThroughSCTCharL(timerExpire); 
                return ETrue;
                }
            else
                { 
                ChrKeyTimerExpired();
                }	

            if(!fepMan->IsPhoneNumberEditor())
			{
				TChar validchr;
				if(!CheckHasValidNumerKeyMappedL(aKey,languageChanged))
					ptiengine->SetCase((TPtiTextCase)EPtiCaseLower);
				else
					return ETrue;
			}
            }
            if(!iData)
            	{
            	// this should be called after case change 
            	// so that the shift state is taken care of
            	#ifdef __HALF_QWERTY_PREVIEW_PANE
                	fepMan->PreparePreviewPaneL((TPtiKey)aKey);
				#endif //__HALF_QWERTY_PREVIEW_PANE
            	}

            // Over ride the case changes, force to fnLower for 
            // long key press.
            if( aLength == ELongKeyPress && aKey == EStdKeyFullStop)
            	{
            	iOwner->PtiEngine()->SetCase( EPtiCaseFnLower );
            	}
            	            
            // Get keymapping from PtiEngine.                
            text.Set(ptiengine->AppendKeyPress((TPtiKey)aKey));

            // send the text to be highlighted here
            #ifdef __HALF_QWERTY_PREVIEW_PANE
            	fepMan->ShowPaneL();
			#endif //__HALF_QWERTY_PREVIEW_PANE            	
            }
            
        if (text.Length() > 0)        
             {               
            TBuf<1> chr;    
            chr.Append(text[0]);
            if( fepMan->IsOnlyNumericPermitted() 
            	&& !iOwner->FepMan()->IsValidCharInNumericEditorL(text[0]) )
            	{
            	ptiengine->CancelTimerActivity(); 
            	
#ifdef __REVERSE_FN_KEY_SUPPORTED            	
	            if(languageChanged)
    	            {
        	        ptiengine->ActivateLanguageL(origLanguage);
            	    ptiengine->SetInputMode(origInputMode );            		
                	}            
#endif                		
	            if(fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric())
					ptiengine->SetCase(CurCase);      		
            	return ETrue;
            	}            
            if(fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric()
                                            && fepMan->FnKeyState() != CAknFepFnKeyManager::EFnKeyNone)
                {
                if(!iOwner->FepMan()->IsValidCharInNumericEditorL(text[0]) )
                    {
                    ptiengine->CancelTimerActivity(); 
                                    
#ifdef __REVERSE_FN_KEY_SUPPORTED               
                    if(languageChanged)
                        {
                        ptiengine->ActivateLanguageL(origLanguage);
                        ptiengine->SetInputMode(origInputMode );                    
                        }            
#endif  
                    }
            	}
            fepMan->NewCharacterL(chr);
            
           // if ( aKey == EStdKeyEnter)
           //     {
           //     fepMan->CommitInlineEditL();      
           //     ptiengine->ClearCurrentWord();                
           //     iData = 0;
           //     }
           // else            
           //     {    
                iData = aKey;            
                fepMan->SetFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);                   
            //    }
                                  
            }
        if((text.Length() > 0) || (IsKeymapped(aKey)))
            {
            ret = ETrue;
            }
        TBuf<32> mappedCharacters;
        iOwner->PtiEngine()->MappingDataForKey((TPtiKey)aKey, mappedCharacters, iOwner->PtiEngine()->Case());
        if( mappedCharacters.Length() < 2 )
            {
            ptiengine->CancelTimerActivity(); 
            if( aLength == EShortKeyPress )
            	{            	
            	SetShortKeyPressText(text);
            	}
            }
		if(fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric())
			ptiengine->SetCase(CurCase);
    }
    
// else
// //Long key press
//    {
//        if (aKey == EStdKeyEnter)
//            {
            // PtiEngine does not have mapping for the enter key.
//            const TText KAknFEPLineFeedSymbol = 0x21B2;
//            TBuf<1> lineFeed;
//            lineFeed.Zero();
//            lineFeed.Append(KAknFEPLineFeedSymbol);
//            text.Set(lineFeed);
//            }
//        else
//            {
//            TBool isCharacter = ETrue;
//            if (!iData)
//            {
//            isCharacter = IsCharacterKey(aKey);
//            }
//            //HandleCaseChange(aKey, isCharacter);
//            //Forcing Fn mode for long key press
//            
//            /*
//            //Start of Option-1
//            ptiengine->SetCase(EPtiCaseFnLower);
//            text.Set(ptiengine->AppendKeyPress((TPtiKey)aKey));
//            // End of Option-1
//            */
//
//            //Start of Option-2
//            //To do: Space key lower case being mapped and removing 
//            //hard coding of space in UI State.
//            TBuf<5> allFnMappedCharacters;
//            ptiengine->MappingDataForKey((TPtiKey)aKey, allFnMappedCharacters, EPtiCaseFnLower);
//            text.Set(allFnMappedCharacters);
//            // End of Option-2
//
//            }
//        #ifdef __HALF_QWERTY_PREVIEW_PANE
//        // in any case hide the preview pane
//        	fepMan->HidePane();
//		#endif //__HALF_QWERTY_PREVIEW_PANE
//            
//        if (text.Length() > 0)        
//            {               
//            TBuf<1> chr;    
//            chr.Append(text[0]);
//            fepMan->NewCharacterL(chr);
//            fepMan->CommitInlineEditL();      
//            ptiengine->ClearCurrentWord();                
//            iData = 0;
//            /*
//            if (aKey == EStdKeyEnter)
//                {
//                fepMan->CommitInlineEditL();      
//          ptiengine->ClearCurrentWord();                
//          iLastKey = 0;
//                }
//            else            
//                {    
//                iLastKey = aKey;            
//                fepMan->SetFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);                   
//                }
//            */
//                                  
//            ret = ETrue;
//            }
//        else
//            {
//            // text.Length is zero. Handle long key.
//            if( aKey == EStdKeyFullStop )
//                {
//                ptiengine->CancelTimerActivity();
//                }
//            }
//        }
    if( aKey!=EStdKeyBackspace && fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
    	return ETrue;
        
    if(ptiengine->IsValidKey((TPtiKey)aKey))
       	ret = ETrue;
        
#ifdef __REVERSE_FN_KEY_SUPPORTED
	if(languageChanged)
    	{
        ptiengine->ActivateLanguageL(origLanguage);
        ptiengine->SetInputMode(origInputMode );            		
        }    
#endif  
  
    return ret;
    }

// Functions from MPtiObserver interface
void TAknFepInputStateHalfQwerty::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        TRAP_IGNORE(fepMan->CommitInlineEditL())

        CPtiEngine* ptiengine = iOwner->PtiEngine();
        ptiengine->ClearCurrentWord();
        #ifdef __HALF_QWERTY_PREVIEW_PANE
        	fepMan->HidePane();
		#endif //__HALF_QWERTY_PREVIEW_PANE
        iData = 0;
        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
        CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();    
        if( !fepMan->IsFlagSet( CAknFepManager::EFlagLongShiftKeyPress ) )
            {
            if(fnKeyState == CAknFepFnKeyManager::EFnKeyNext)
                fepMan->SetFnKeyState(CAknFepFnKeyManager::EFnKeyNone);
            }
        }  
    }
    
void TAknFepInputStateHalfQwerty::HandleCaseChange(TInt /*aKey*/, TBool aIsCharacter)
    {
    // Punctuation key multitapping should not change case
    if( iData )
        return;
    
    TPtiTextCase caseMode = EPtiCaseLower;

    if (aIsCharacter)
        {
        if( iData == 0 || !( iOwner->FepMan()->IsOnlyNumericPermitted()  ) )
            {
            // use the Edwin case only when timer expires
            // To handle cases like insert fn character empty editor
            // followed by alpa character
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
            // user is still multitapping, use the existing PtiEngine case
            caseMode = iOwner->PtiEngine()->Case();
            }
        }
        
    TPtiTextCase shiftedCase = caseMode;
    
    TPtiTextCase curCase = iOwner->PtiEngine()->Case();
    if(curCase == EPtiCaseFnUpper || curCase == EPtiCaseFnLower)
        return;

    if ( iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) && 
        ( !aIsCharacter|| iOwner->CaseMan()->IsAbleChangeCase() ) )
        {
        // Case mode is changed to opposite because shift key is/was pressed and
        // in case of character key case changes are allowed. In case of
        // number or special key, shift mode is used always when 
        // shift key is pressed
        
                
        // clear this flag so that the shift key mode gets reset 
        // when shift key up event is received
        
        iOwner->FepMan()->ClearFlag(CAknFepManager::EFlagNoActionDuringShiftKeyPress);
        
		// Do not toggle case while multitapping
        if( !iData )
        	{
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
    iOwner->FepMan()->ClearFlag( CAknFepManager::EFlagNoActionDuringChrKeyPress );
    iOwner->PtiEngine()->SetCase(shiftedCase);
    
    }

TBool TAknFepInputStateHalfQwerty::IsCharacterKey(const TInt aKey)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();       
    // Max length doesn't matter because we'll need only the first character.    
    TBuf<KMaxName> lowerdata;   
    
    ptiengine->MappingDataForKey((TPtiKey)aKey, lowerdata, EPtiCaseLower);        
    if (lowerdata.Length() > 0)
        {
        TChar lower = lowerdata[0];        
        TUint upper = lower.GetUpperCase();
    
        if ( TUint(lower) != upper )
            {
            return ETrue;   
            }
        }
   
     return EFalse;
    }       
TBool TAknFepInputStateHalfQwerty::TryGetValidCharForKey(const TInt& aKey, TChar& aChar)
    {    
    if( ! TryGetValidCharForKeyAndCase( aKey, aChar, EPtiCaseFnLower) )
        {
        if(! TryGetValidCharForKeyAndCase( aKey, aChar, EPtiCaseUpper))
            {
            if(! TryGetValidCharForKeyAndCase( aKey, aChar, EPtiCaseLower))
                {
                return EFalse;
                }
            }
        }
    return ETrue;
    }
TBool TAknFepInputStateHalfQwerty::TryGetValidCharForKeyAndCase(const TInt& aKey, TChar& aChar, TPtiTextCase aCase)
    {
    TBuf<KMaxName> mappedCharacters;

    iOwner->PtiEngine()->MappingDataForKey((TPtiKey)aKey, mappedCharacters, aCase);        
    TRAP_IGNORE(
    for (TInt i = 0; i < mappedCharacters.Length(); i++ )
        {
        if( iOwner->FepMan()->IsValidCharInNumericEditorL( mappedCharacters[i] ) )
            {
            aChar = mappedCharacters[i];
            return ETrue;
            } 
        }); 
    return EFalse;
    }
TInt TAknFepInputStateHalfQwerty::TimerExpire(TAny* aPtr)
    {
    reinterpret_cast<TAknFepInputStateHalfQwerty*>(aPtr)->ChrKeyTimerExpired();
    return 1;
    }
TInt TAknFepInputStateHalfQwerty::ChrKeyTimerExpired()
    {
    if(iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
        {
        // this function is only called for number only editors 

        TRAP_IGNORE(iOwner->FepMan()->CommitInlineEditL());
        }    
    return 1;
    }


TBool TAknFepInputStateHalfQwerty::IsKeymapped(TInt aKey)
    {
    TBuf<20> mapData;
    TBool ret = EFalse;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    for(TInt count=0;count<EPtiCaseMax;count++)
        {
        ptiengine->MappingDataForKey(static_cast<TPtiKey>(aKey), mapData, static_cast<TPtiTextCase> (count)); 
        if(mapData.Length())
            {
            ret = ETrue;
            break;
            }
        }
    return ret;
    }

TBool TAknFepInputStateHalfQwerty::CheckHasValidNumerKeyMappedL(TInt aKey, TBool &aLangChanged)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TInt origLanguage = ptiengine->CurrentLanguage()->LanguageCode();
    TPtiEngineInputMode origInputMode = ptiengine->InputMode();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    
    //TBool languageChanged = EFalse;
    
                TChar chr;

                if( ! TryGetValidCharForKey(aKey, chr) )
                    {
                    // If the key in not valid char in the current input language in Fn reverse mode,
                    // then we will check if against English language. And if its valid then we will
                    // insert the char based out of key for English input language and revert back 
                    // the change in input language at the end.

                    if(ptiengine->CurrentLanguage()->LanguageCode() == ELangEnglish)
                        {
                        if(fepMan->IsOnlyNumericPermitted())
                            return ETrue;
                        else
                            return EFalse;
                        }

                    aLangChanged = ETrue;
                    ptiengine->ActivateLanguageL(ELangEnglish);
                    ptiengine->SetInputMode(EPtiEngineHalfQwerty); 

                    if( !fepMan->IsValidCharForNumericInFnReverseL(aKey, iOwner->PtiEngine()->Case()) )
                        {
                        if( ! TryGetValidCharForKey(aKey, chr) )
                            {
                            ptiengine->ActivateLanguageL(origLanguage);
                            ptiengine->SetInputMode(origInputMode );  
                            if(fepMan->IsHybridAplhaEditor() && !fepMan->IsHybridAlphaModeChangedtoAplhanumeric())    
                                return EFalse;
                            else
                                return ETrue;
                            }
                        else
                            {
                            if(aLangChanged)
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
                    if(aLangChanged)
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
                
    
    return EFalse;
    }	
// End of file
