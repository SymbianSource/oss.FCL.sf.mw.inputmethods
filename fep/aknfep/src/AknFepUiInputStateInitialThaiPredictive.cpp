/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateInitialThaiPredictive methods.
*
*/












#include "AknFepUiInputStateInitialThaiPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepManager.h"
#include "AknFepCaseManager.h"                  //CAknFepCaseManager

#include <PtiDefs.h>


TAknFepInputStateInitialThaiPredictive::
TAknFepInputStateInitialThaiPredictive(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateInitialMultitapBase(aOwner)
    {
    }

TBool TAknFepInputStateInitialThaiPredictive::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    TBool ret = ETrue;

    if (aKey == EPtiKeyStar )
        {
        if (aLength == EShortKeyPress)
            {
            if (!fepMan->EditSubmenuInUse())
                {
                if (fepMan->IsAbleToLaunchSCT())
                    {
                    fepMan->LaunchSpecialCharacterTableL(iOwner->ThaiSCTResourceId(0x0020,aKey));
                    return (ETrue);
                    }
                }
            else
                {
                fepMan->ClearCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);
                }
            }
        else                
            {
            if (fepMan->EditSubmenuInUse())
                {                                       
                fepMan->LaunchSpecialCharacterTableL(iOwner->ThaiSCTResourceId(0x0020,aKey));
                return (ETrue); 
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

void TAknFepInputStateInitialThaiPredictive::HandleCommandL(TInt aCommandId)
    {
    if (aCommandId == EAknCmdEditInsertSymbol)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        fepMan->LaunchSpecialCharacterTableL(
                           iOwner->ThaiSCTResourceId(fepMan->PreviousChar(),EPtiKeyStar));
        }
    }

// End of file
