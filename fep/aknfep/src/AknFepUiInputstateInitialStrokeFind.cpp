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
*       Provides the TAknFepInputStateInitialStrokeFind methods.
*
*/












#include "AknFepUiInputStateInitialStrokeFind.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepManagerUIInterface.h"

#include <e32keys.h>
#include <PtiEngine.h>
#include <PtiDefs.h>                //keys

#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"

TAknFepInputStateInitialStrokeFind::TAknFepInputStateInitialStrokeFind(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateChineseBase(aOwner, aUIContainer)
    {
    iState = EInitial;
    iOwner->PtiEngine()->SetInputMode(EPtiEngineStroke);
    }

TBool TAknFepInputStateInitialStrokeFind::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;
    
    if (aKey == EKeyBackspace || aKey == EKeyOK)
        {
        ret = EFalse;
        }
    else if (aLength == EShortKeyPress) 
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
        else if (iOwner->IsValidChineseInputKey(aKey))
            {
            ptiengine->AppendKeyPress((TPtiKey)aKey);
            TPtrC text = ptiengine->GetPhoneticSpelling(1);

            if(text.Length()) 
                {
                fepMan->NewCharacterL(text);
                fepMan->CommitInlineEditL();
                ptiengine->ClearCurrentWord();
                }
            else
                {
                ret = EFalse;
                }
            }
        }
    else
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

// End of file
