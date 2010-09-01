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
*       Provides the TAknFepInputStateInitialMultitapBase methods.
*
*/












#include "AknFepUiInputStateInitialMultitapBase.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepCaseManager.h"
#include "AknFepUiIndicInputManager.h"

#include <PtiEngine.h>
#include <PtiDefs.h>

TAknFepInputStateInitialMultitapBase::
TAknFepInputStateInitialMultitapBase(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateBase(aOwner)
    {
    iState = EInitial;
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    TPtiTextCase caseMode = ptiengine->Case();
#ifdef RD_INTELLIGENT_TEXT_INPUT  
    // This part of code TSW error fixing  
    // In case of ITU-T, whenever word is displaying along with autocompletion
    // part and if some how editor will lose focus [example: launch candidate list, 
    // launch option menu] word along with auto completion part get locked and come to inline editing.
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool clearCurrentword(ETrue);
    TRAP_IGNORE(    
	if( fepMan->WesternPredictive() )
		{
		TInt tailLength = 0;
		ptiengine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
		 if( tailLength > 0 )
			 {
			 clearCurrentword = EFalse;
			 }
		});

	if( clearCurrentword )
		{
#endif	
    ptiengine->ClearCurrentWord();

#ifdef RD_INTELLIGENT_TEXT_INPUT 
    	}
#endif	
    ptiengine->SetInputMode(EPtiEngineMultitapping);

    ptiengine->SetCase(caseMode);
    }

TBool TAknFepInputStateInitialMultitapBase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;

    if(iData && (iData != aKey))
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
        }

    iData = aKey;
    if(aLength == EShortKeyPress)
        {
        if (aKey == EPtiKeyStar)
            {
 			if (fepMan->EditSubmenuInUse())
        		{
        		return ret;
        		}                        
            else if (fepMan->IsAbleToLaunchSCT())
                {
                fepMan->LaunchSpecialCharacterTableL();
                }
            }
        else
            {
            TPtrC text = ptiengine->AppendKeyPress((TPtiKey)aKey);

            if( aKey == EPtiKey0 && text.Length())
            	{
            	MPtiLanguage* ptilang = ptiengine->CurrentLanguage();
        		TInt currentPtiLang = ( ptilang )? ptilang->LanguageCode() : 0;
            	if( TAknFepUiIndicInputManager::IsCharLigature( text[0], 
                                                            ( TLanguage )currentPtiLang ) )
            		{
            		ptiengine->ClearCurrentWord();
            		text.Set( ptiengine->AppendKeyPress( ( TPtiKey )aKey ) );
            		}
            	}

            if (text.Length())
                {
                fepMan->NewCharacterL(text);
                }
            }
        }
    else // long keypress
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if (aKey == EPtiKeyStar)
            {
            if(fepMan->EditSubmenuInUse() && fepMan->IsAbleToLaunchSCT())
                {
                fepMan->LaunchSpecialCharacterTableL();
                }
            }
#else
        if (aKey == EPtiKeyStar && fepMan->EditSubmenuInUse())
            {
            fepMan->LaunchSpecialCharacterTableL();                
            }    
#endif
		else
			{			                  
        	if (aKey != EKeyBackspace)
        		{
                TChar ch(aKey);

                TBuf<1> buf;
                buf.Append(ch);
        	    fepMan->NewCharacterL(buf);
        	    fepMan->CommitInlineEditL();
        	    ptiengine->ClearCurrentWord();
        		}
        	else
        		{
        		ret = EFalse;
        		}
			}
        }

    return ret;
    }

// Functions from MPtiObserver interface
void TAknFepInputStateInitialMultitapBase::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        TRAP_IGNORE(fepMan->CommitInlineEditL())

        CPtiEngine* ptiengine = iOwner->PtiEngine();
        ptiengine->ClearCurrentWord();

        iData = 0;
        }
    }

// Set the Star Key flag.    
void TAknFepInputStateInitialMultitapBase::SetStarKeyFlag( TBool aSet)
	{
	TAknFepInputStateBase::SetStarKeyFlag( aSet );
	}
// End of file
