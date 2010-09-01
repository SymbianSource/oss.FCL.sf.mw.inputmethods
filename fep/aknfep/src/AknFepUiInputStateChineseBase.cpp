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
* Description:            Provides the TAknFepUiInputStateChineseBase methods.
*
*/












// INCLUDE FILES
#include <avkon.rsg>

#include "AknFepUiInputStateChineseBase.h"
#include "AknFepUIManagerStateInterface.h"   //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"        //MAknFepManagerUIInterface
#include <PtiDefs.h>                //keys
#include "AknFepGlobalEnums.h"
#include "AknFepPanic.h"

// CONSTANTS

/**
*  TAknFepUiInputStateChineseBase class.
* 
*/

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepInputStateChineseBase::TAknFepInputStateChineseBase
// Constractor
// ---------------------------------------------------------------------------
//
TAknFepInputStateChineseBase::
TAknFepInputStateChineseBase(MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateBase(aOwner)
    {
    iUIContainer = aUIContainer;

    /* UI is not activated at this point yet.
    if (aOwner)
        {
        aOwner->FepMan()->SendUIActivatedNotification();
        }
        */
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateChineseBase::operator=
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateChineseBase::operator=(const TAknFepInputStateBase& aState)
    {
    Mem::Copy(this, &aState, sizeof(TAknFepInputStateBase));
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateChineseBase::operator=
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateChineseBase::operator=(const TAknFepInputStateChineseBase& aState)
    {
    Mem::Copy(this, &aState, sizeof(TAknFepInputStateChineseBase));
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateChineseBase::UIContainer()
// Getting UI container object
// @return Japanese UI container object
// ---------------------------------------------------------------------------
//

MAknFepUICtrlContainerChinese* TAknFepInputStateChineseBase::UIContainer()
    {
    return static_cast<MAknFepUICtrlContainerChinese*>(iUIContainer);
    }


// ---------------------------------------------------------------------------
// TAknFepInputStateChineseBase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateChineseBase::HandleCommandL(TInt aCommandId)
	{	
	switch (aCommandId)
	    {
		// Handle the event frome command.
		case EAknSoftkeyCancel:
	//	case (TUint16)EAknSoftkeyCancel: //the Cancle in soft CBA
			iOwner->FepMan()->TryCloseUiL();
			break;  
		case EAknSoftkeySelect:
		//case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
			break;
		default:
			break;
		}    	
 	}
// End of file
