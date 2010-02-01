/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateInitialThaiMultitap methods.
*
*/












#include "AknFepUIInputStateInitialThaiMultitap.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface

#include "AknFepCaseManager.h"
#include <uikon.hrh>

#include <PtiEngine.h>
#include <PtiDefs.h>
#include <aknSctDialog.h>
#include <avkon.rsg>
#include <AknFep.rsg>


TAknFepInputStateInitialThaiMultitap::
TAknFepInputStateInitialThaiMultitap(MAknFepUIManagerStateInterface* aOwner)
:TAknFepInputStateInitialMultitapBase(aOwner)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TPtiTextCase caseMode = ptiengine->Case();
    TRAP_IGNORE(ptiengine->ActivateLanguageL(ELangThai))
    ptiengine->SetCase(caseMode);
    }

TBool TAknFepInputStateInitialThaiMultitap::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    if(aLength == EShortKeyPress)
        {
        if (aKey == EPtiKey0 && fepMan->IsAbleToLaunchSCT())
        	{			
            fepMan->LaunchSpecialCharacterTableL(
                     iOwner->ThaiSCTResourceId(fepMan->PreviousChar(),aKey));
            return (ETrue);                    	
        	}
		else if (aKey == EPtiKeyStar)        	
			{
			if (!fepMan->EditSubmenuInUse())		
				{
				if (fepMan->IsAbleToLaunchSCT())
        			{
              		fepMan->LaunchSpecialCharacterTableL(
              		   iOwner->ThaiSCTResourceId(fepMan->PreviousChar(),aKey));
               		return (ETrue); 
               		}        	
				}
			else
				{
				fepMan->ClearCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);				
				}							
			}                        
        }
	else
		{
		if (aKey == EPtiKeyStar && fepMan->EditSubmenuInUse() && fepMan->IsAbleToLaunchSCT())
			{
            fepMan->LaunchSpecialCharacterTableL(
                     iOwner->ThaiSCTResourceId(fepMan->PreviousChar(),aKey));
            return (ETrue);			
			}		
		}        

    return (TAknFepInputStateInitialMultitapBase::HandleKeyL(aKey, aLength));
    }


void TAknFepInputStateInitialThaiMultitap::HandleCommandL(TInt aCommandId)
    {
    if (aCommandId == EAknCmdEditInsertSymbol)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        fepMan->LaunchSpecialCharacterTableL(
                     iOwner->ThaiSCTResourceId(fepMan->PreviousChar(),EPtiKeyStar));
        }
    }

// End of file
