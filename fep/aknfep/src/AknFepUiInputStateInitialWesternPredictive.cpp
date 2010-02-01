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
*       Provides the TAknFepInputStateInitialWesternPredictive methods.
*
*/












#include "AknFepUiInputStateInitialWesternPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepManager.h"
#include "AknFepCaseManager.h"                  //CAknFepCaseManager
#include "AknFepUiIndicInputManager.h"

#include <PtiEngine.h>
#include <PtiDefs.h>

TAknFepInputStateInitialWesternPredictive::
TAknFepInputStateInitialWesternPredictive(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateInitialMultitapBase(aOwner)
    {
    }

TBool TAknFepInputStateInitialWesternPredictive::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool ret = ETrue;

    if (aKey == EPtiKey0)
        {
        TAknFepInputStateInitialMultitapBase::HandleKeyL(aKey, aLength);
        }
    else if (aKey == EPtiKeyStar)
        {     
		if (aLength == EShortKeyPress)
			{			
			if (fepMan->IsFlagSet(CAknFepManager::EFlagLineFeedCharacter) && 
			    fepMan->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
			    {
			    // User is interrupting multitap timer with star key, but there
			    // is line feed event waiting. Make sure that it is handled first.
			    fepMan->CommitInlineEditL();
			    }
					        
        	if (!fepMan->EditSubmenuInUse())
        		{        		        	
        		if (fepMan->IsAbleToLaunchSCT())
        			{
        			fepMan->LaunchSpecialCharacterTableL();
        			}               	        		        		
        		}        	        	
			else
				{
				fepMan->ClearCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);
				}        		
			}
		else	        	
			{
			if (fepMan->EditSubmenuInUse() && fepMan->IsAbleToLaunchSCT())
				{										
				fepMan->LaunchSpecialCharacterTableL();           			
				}
			}
        }   
    else 
        {
        if (fepMan->IsFlagSet(CAknFepManager::EFlagSuppressInputStateChange))
            {
            // Long keypress of numeric keypress, we want to simply return EFalse
            // to enter a number into the editor, rather than move back to 
            // entry state, so just clear the flag
            fepMan->ClearFlag(CAknFepManager::EFlagSuppressInputStateChange);

            // Asyncronous case update after the number has been entered to the editor.
            fepMan->SimulateKeyEventL(EKeyF19); 
            }
        else
            {
            fepMan->TryCloseUiL(); //expires the multitap timer
            iOwner->ChangeState(EEntry);
            fepMan->SendUIActivatedNotification();
            }
        ret = EFalse;
        }
    return ret;
    }

// End of file
