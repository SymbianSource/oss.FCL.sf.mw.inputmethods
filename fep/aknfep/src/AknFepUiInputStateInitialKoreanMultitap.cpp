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
*       Provides the TAknFepInputStateInitialKoreanMultitap methods.
*
*/

#include "AknFepUiInputStateInitialKoreanMultitap.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepCaseManager.h"
#include "AknFepUiIndicInputManager.h"
#include "AknFepManager.h"

#include <PtiEngine.h>
#include <PtiDefs.h>

TAknFepInputStateInitialKoreanMultitap::TAknFepInputStateInitialKoreanMultitap(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateInitialMultitapBase(aOwner)
    {
    iState = EInitial; 
    
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    TPtiTextCase caseMode = ptiengine->Case();

#ifdef RD_INTELLIGENT_TEXT_INPUT
     TRAP_IGNORE(ptiengine->ActivateLanguageL(ELangKorean, EPtiEngineMultitappingKorean))
    ptiengine->SetInputMode(EPtiEngineMultitappingKorean);
#else
     TRAP_IGNORE(ptiengine->ActivateLanguageL(ELangKorean, EPtiEngineMultitapping))
    ptiengine->SetInputMode(EPtiEngineMultitapping);
#endif //RD_INTELLIGENT_TEXT_INPUT

    ptiengine->SetCase(caseMode);
    }

TBool TAknFepInputStateInitialKoreanMultitap::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;
       
    if(aLength == EShortKeyPress)
        {
                     
       	if(aKey == EKeyBackspace)
        	{
        	// Narat Geul engine tries first to disassemble hangul to jamos. if disassembly is not possible,
        	// then clear character. 
        	if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
        		{
        		TPtrC newText = ptiengine->DeleteKeyPress();
       			fepMan->UpdateInlineEditL(newText, newText.Length());
       			
        		if (newText.Length() == 0)
        			{
	        		fepMan->CancelInlineEdit();
   		     		ptiengine->ClearCurrentWord();
        			}	      	
        		}
        	else
        		{
        		ret = EFalse;	
        		}
        	}
        else
            {
	
            if(aKey == EKeyLeftArrow ||
            	aKey == EKeyRightArrow ||
            	aKey == EKeyDownArrow ||
            	aKey == EKeyUpArrow)
            	{
               	ret = EFalse;            
            	}
            else
            	{
            	if (!(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)))
                	{ // 'new' inline edit
             	  	ptiengine->ClearCurrentWord();
                	fepMan->StartInlineEditL();
                	fepMan->SetInlineEditingCursorVisibilityL(ETrue);
                	}
                      	   	             
            	TPtrC aText = ptiengine->AppendKeyPress((TPtiKey)aKey);
            	fepMan->UpdateInlineEditL(aText, aText.Length());	
   						
            	// Force commit	when buffer length is 511. Needs to be commited in order to 
            	// prevent ptiengines iTextBuffer overflow. Even though it is very unlikely
            	// case
            	if(aText.Length() >= 254)
            		{
            		if (CommitOrNot(aText.Right(CAknFepManager::ESingleCharacter), aKey))
            			{
            			TPtrC text = ptiengine->DeleteKeyPress();
            			fepMan->UpdateInlineEditL(text, text.Length());
            			fepMan->CommitInlineEditL();
            			
            			ptiengine->ClearCurrentWord();
            			
            			TPtrC newText = ptiengine->AppendKeyPress((TPtiKey)aKey);			
            			fepMan->StartInlineEditL();
            			fepMan->UpdateInlineEditL(newText, newText.Length());	
            			}
            		else if(aText.Length() >= 255)
            			{// force commit 
            			fepMan->CommitInlineEditL();	
            			}
            		}
				else
					{
					fepMan->UpdateInlineEditL(aText, aText.Length());	
					}           		
            	}                                                              	  	                    				
          	}    
        }
    else // long keypress
        {  
          
 	    if (aKey == EPtiKeyStar)
            {
            /* Fix bug for EHQN-7ZWE9F 
           	ptiengine->DeleteKeyPress();
            TPtrC text = ptiengine->PreviousCandidate();
            fepMan->UpdateInlineEditL(text, text.Length());
            fepMan->CommitInlineEditL();
            ptiengine->ClearCurrentWord();*/

            fepMan->LaunchSelectModeMenuL();               
            }
		else
			{
			// Insert number through long key press.			                  
        	if (aKey != EKeyBackspace)
        		{
        		if (!fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
        			{
        			// No edit transaction going on, we have ended here because of long
        			// key press created editor on UI or some other similar non-standard situation.
        			// We can't handle key press, so just consume it.
        			return ETrue; 			
        			}
				
			 	TPtrC text = ptiengine->PreviousCandidate();
           	 	fepMan->UpdateInlineEditL(text, text.Length());
               	fepMan->CommitInlineEditL();
               	ptiengine->ClearCurrentWord();
       			ret = EFalse;
        		}
        	else if (aKey == EKeyBackspace)
        		{
        		if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
        			{
					fepMan->CommitInlineEditL();
					ptiengine->ClearCurrentWord();
					fepMan->TryCloseUiL();
        			}
        		ret = EFalse;	
        		}
        	else
        		{
        		ret = EFalse;
        		}       		
			}
		
        } 
    return ret;
    } 
     
// Decides wheather text should be commited or not. Makes comparison between latest key input
// and "seed" jamos. if latest input match with one of "seed" jamos and star or hash key cannot make
// any changes to input, then commit.
// NOTE; the "seed" jamos are characters that are printed on keymat. Narat geul engine compiles all
// the hanguls from 12 jamos.  
TBool TAknFepInputStateInitialKoreanMultitap::CommitOrNot(TPtrC aData, TInt aKey)
	{
	TBool commit = EFalse;
	TBuf<CAknFepManager::EMaximumFepWordLength> mappings;
	TInt len;
	
	CPtiEngine* ptiengine = iOwner->PtiEngine();	
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
	
	ptiengine->MappingDataForKey((TPtiKey)aKey, mappings, ptiengine->Case());
	len = mappings.Length();
	
	for (TInt i = 0; i < len; i++ )
		{
		if(aData.Compare(mappings.Left(i + 1)) == 0)
			{
			commit = ETrue;
			break;
			}
		}
		
	return commit;
	}

// End of file
