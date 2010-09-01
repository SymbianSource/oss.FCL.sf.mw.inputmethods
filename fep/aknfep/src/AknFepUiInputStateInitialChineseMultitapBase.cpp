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
*       Provides the TAknFepInputStateInitialChineseMultitapBase methods.
*
*/












#include "AknFepUiInputStateInitialChineseMultitapBase.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepCaseManager.h"

#include <PtiEngine.h>
#include <PtiDefs.h>

TAknFepInputStateInitialChineseMultitapBase::TAknFepInputStateInitialChineseMultitapBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    iState = EInitial;
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    TPtiTextCase caseMode = ptiengine->Case();

    ptiengine->ClearCurrentWord();
    ptiengine->SetInputMode(EPtiEngineMultitapping);

    ptiengine->SetCase(caseMode);
    }

TBool TAknFepInputStateInitialChineseMultitapBase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;

    if(iData && (iData != aKey))
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
        CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
        caseMan->UpdateCase(ENullNaviEvent); 
        }

    iData = aKey;
    if(aLength == EShortKeyPress)
        {
        if (aKey == EPtiKeyStar)
            {
            if (fepMan->EditSubmenuInUse())
        		{
     		    fepMan->ClearCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);                    
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
            if (text.Length())
                {
                fepMan->NewCharacterL(text);
                }
            }
        }
    else // long keypress
        {
        if (aKey == EPtiKeyStar)
            {
            if (fepMan->EditSubmenuInUse())
            	{
       			fepMan->LaunchSpecialCharacterTableL();                
        	    } 
            }        
        }

    return ret;
    }

// Functions from MPtiObserver interface
void TAknFepInputStateInitialChineseMultitapBase::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        //Changing the below line from "TRAPD(err," to TRAP_IGNORE 
        //to remove compiler warning
        TRAP_IGNORE(fepMan->CommitInlineEditL())
        //do something if error??

        CPtiEngine* ptiengine = iOwner->PtiEngine();
        ptiengine->ClearCurrentWord();

        CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
        caseMan->UpdateCase(ENullNaviEvent); 

        iData = 0;
        }
    }

// End of file
