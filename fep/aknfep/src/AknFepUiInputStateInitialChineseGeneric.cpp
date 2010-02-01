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
*       Provides the TAknFepInputStateInitialChineseGeneric methods.
*
*/












#include "AknFepUiInputStateInitialChineseGeneric.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepManagerUIInterface.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "AknFepManager.h"

#include <PtiEngine.h>
#include <e32keys.h>
#include <PtiDefs.h>                //keys
#include <uikon.hrh>

TAknFepInputStateInitialChineseGeneric::TAknFepInputStateInitialChineseGeneric(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateInitialChineseMultitapBase(aOwner, aUIContainer)
    {
    }

TBool TAknFepInputStateInitialChineseGeneric::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    TBool ret = ETrue;

    if(iData && (iData != aKey))
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
        }
    
    //7,8,9 are not multitap in EStroke, so commit
    if ((fepMan->InputMode() == EStroke) &&
         (iData == EPtiKey7 || iData == EPtiKey8 || iData == EPtiKey9 ))
    	{
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
    	}
    
    iData = aKey;

    if(iOwner->IsValidChineseInputKey(aKey))
        {
        if ( aLength == EShortKeyPress )
        	{
            ptiengine->ClearCurrentWord();
            ptiengine->ResetToneMark();

            if(iOwner->ChangeState(EEntry) == EEntry)
                {
                iOwner->FepMan()->SendUIActivatedNotification();
                }
            ret = EFalse; //indicate we can't handle the key...
        	}
        else
        	{
        	return ETrue;
        	}

        }
    else if(aKey == EPtiKey0 || aKey == EPtiKey1) // do multitap on zero key...
        {
        if(aLength == EShortKeyPress)
            {
            const TPtiTextCase oldCase = ptiengine->Case();
            if (fepMan->InputMode() == EStroke)
            	{
            	ptiengine->SetCase(EPtiCaseUpper);
            	}
            TPtrC aText = ptiengine->AppendKeyPress((TPtiKey)aKey);
            fepMan->NewCharacterL(aText);
            ptiengine->SetCase(oldCase);
            }
        else
            {
            //don't want long keypresses to result in numbers as this would be inconsistent in
            //Chinese modes, so commit the current character
            fepMan->CommitInlineEditL();
            ptiengine->ClearCurrentWord();
            }
        }
    //do multitap on 7-9 key
    else if ((fepMan->InputMode() == EStroke) &&
             (aKey == EPtiKey7 || aKey == EPtiKey8 ||
              aKey == EPtiKey9 ))    		   
    	{
    	if(aLength == EShortKeyPress)
            {
            const TPtiTextCase oldCase = ptiengine->Case();
            ptiengine->SetCase(EPtiCaseUpper);            
            
            TPtrC text = ptiengine->AppendKeyPress((TPtiKey)aKey);
            fepMan->NewCharacterL(text);
            ptiengine->SetCase(oldCase);
            }
        else
            {
            //don't want long keypresses to result in numbers as this would be inconsistent in
            //Chinese modes, so commit the current character
            fepMan->CommitInlineEditL();
            ptiengine->ClearCurrentWord();
            }
    	}
    else if (aKey == EPtiKeyStar)
        {
        if (aLength == EShortKeyPress)
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
       		if (fepMan->EditSubmenuInUse())
            	{
       			fepMan->LaunchSpecialCharacterTableL();                
        	    }        	
            }
        }
    else if (aKey == EKeyOK)
        {
        ret = EFalse;   //don't consume OK key in initial state 
        }

    return ret;
    }

// End of file
