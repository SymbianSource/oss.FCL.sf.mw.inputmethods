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
*       CAknFepInputStateQwertyKorean implementation
*
*/

#include "AknFepUiInputStateQwertyKorean.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
//#include "AknFepDbgKorean.h"

#include <PtiEngine.h>
#include <PtiDefs.h>
#include <PtiKeyMappings.h>
#include <PtiDefs.h>   

const TInt KMaxPtiTextBufSize = 255;

TAknFepInputStateQwertyKorean::TAknFepInputStateQwertyKorean(
        MAknFepUIManagerStateInterface* aOwner)
    : TAknFepInputStateQwerty(aOwner)
    {
    CPtiEngine& ptiEngine(*(aOwner->PtiEngine()));
    ptiEngine.SetInputMode(EPtiEngineQwertyKorean);
    ptiEngine.ClearCurrentWord();
    }

TAknFepInputStateQwertyKorean::~TAknFepInputStateQwertyKorean()
    {
    }

TBool TAknFepInputStateQwertyKorean::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {	
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;
       
	switch( aKey )
		{
		case EKeyDelete:
		case EStdKeyDelete:
		case EKeyBackspace:
		case EStdKeyBackspace:
			{
			if (fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
				{
				TPtrC newText = ptiengine->DeleteKeyPress();
				if( newText.Length() > 0 )
					{
					  fepMan->UpdateInlineEditL(newText, newText.Length());
					}
				else
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
			break;
		case EStdKeyEnter:
		case EStdKeySpace:
		case EStdKeyTab:
		case EStdKeyLeftArrow:
		case EStdKeyRightArrow:
		case EStdKeyDownArrow:
		case EStdKeyUpArrow:
			{
			fepMan->CommitInlineEditL();	
			ptiengine->CommitCurrentWord();
			ret = EFalse; 
			}
			break;
		default:
			{
			//normal character handled here
			if ( EShortKeyPress == aLength )
				{
				if (!(fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction)))
					{ 
					// start inline edit
					ptiengine->ClearCurrentWord();
					fepMan->StartInlineEditL();
					fepMan->SetInlineEditingCursorVisibilityL(ETrue);
					}
										 
				TPtrC aText = ptiengine->AppendKeyPress((TPtiKey)aKey);
				if( aText.Length() > 0 )
					{
					 fepMan->UpdateInlineEditL(aText, aText.Length());	
					 if( aText.Length()  >= KMaxPtiTextBufSize )
						{
						// force commit 
						fepMan->CommitInlineEditL();	
						ptiengine->CommitCurrentWord();
						}	
					}	
				}
			else // long press
				{
				if (!fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
					{
					return ETrue; 			
					}
				
				//delete last input text
				TPtrC text = ptiengine->DeleteKeyPress();
				if( text.Length() >0 )
					{
				    fepMan->UpdateInlineEditL(text, text.Length());
					}
	            
	            // add new text
				TPtiTextCase previousCase = ptiengine->Case();
				switch ( previousCase )
					{
					case EPtiCaseLower:
					case EPtiCaseUpper:
						ptiengine->SetCase( EPtiCaseFnLower );
						break;
					default:
						break;
					}
					   
				TPtrC newText = ptiengine->AppendKeyPress((TPtiKey)aKey);
				if( newText.Length() >0 )
					{
					 fepMan->UpdateInlineEditL(newText, newText.Length());
					}
				ptiengine->SetCase( previousCase );
				fepMan->CommitInlineEditL();
				ptiengine->ClearCurrentWord();
				}
			}  
			break;
		}                                                             	  	                    				   
    return ret;
    }
