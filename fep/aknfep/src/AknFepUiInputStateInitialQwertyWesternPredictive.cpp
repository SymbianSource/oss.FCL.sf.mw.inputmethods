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
*       Provides the TAknFepInputStateInitialQwertyWesternPredictive methods.
*
*/












#include "AknFepUiInputStateInitialQwertyWesternPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepManager.h"
#include "AknFepCaseManager.h"                  //CAknFepCaseManager

#include <PtiDefs.h>

TAknFepInputStateInitialQwertyWesternPredictive::
TAknFepInputStateInitialQwertyWesternPredictive(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateBase(aOwner)
    {
    }

TBool TAknFepInputStateInitialQwertyWesternPredictive::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
    
    switch (aKey)
        {
        case EStdKeyNull:
            // Take no action if simulated key ends up here
            break;
            
        case EStdKeySpace: //fall through
            {     
            if (fnKeyState == CAknFepFnKeyManager::EFnKeyDown)
                {
                fepMan->LaunchSelectModeMenuL();
                }
			else
				{
				_LIT(KSpace," ");                
				fepMan->StartInlineEditL();
				fepMan->UpdateInlineEditL(KSpace,0);
				fepMan->CommitInlineEditL();
        		return ETrue;
            	}   
            }
        case EStdKeyEnter:
        case EStdKeyTab:
        case EStdKeyBackspace:
        case EStdKeyLeftArrow:
        case EStdKeyRightArrow:
        case EStdKeyUpArrow:
        case EStdKeyDownArrow:
            {
            // Navigation keys do not activate the entry state. Instead they are handled by the editor directly.
            
            // Asyncronous case update is needed after the editor has handled the key
            fepMan->SimulateKeyEventL(EKeyF19); 
            }
            break;
        // In case of Forward Deletion, we should not be doing anything        
        case EStdKeyF21:
        case EKeyLeftShift:
            break;
        default:        
            {
            // Most keys activate the entry state and are handled there
            fepMan->TryCloseUiL(); //expires the multitap timer
            iOwner->ChangeState(EEntry);
            fepMan->SendUIActivatedNotification();
            }
        }

    return EFalse;
    }

// End of file
