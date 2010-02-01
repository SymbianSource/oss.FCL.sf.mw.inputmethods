/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  vkn control pool management
*
*/


#include <peninputlayoutbasecontrol.h>

#include "peninputvkbctrlpool.h"

// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//
CAknFepVkbCtrlPool* CAknFepVkbCtrlPool::NewL()
    {
    CAknFepVkbCtrlPool* self = NewLC();
    CleanupStack::Pop(self);

    return self;        
    }
 
// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//    
CAknFepVkbCtrlPool* CAknFepVkbCtrlPool::NewLC()
    {
    CAknFepVkbCtrlPool* self = new (ELeave) CAknFepVkbCtrlPool();

    CleanupStack::PushL(self);
    self->ConstructL();

    return self;        
    }

// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//   
void CAknFepVkbCtrlPool::ConstructL() 
    {
    }

// ------------------------------------------------------------------------
// Destructor
// ------------------------------------------------------------------------
//         
CAknFepVkbCtrlPool::~CAknFepVkbCtrlPool()    
    {
    iControlList.ResetAndDestroy();
    iControlList.Close();
    }

// ------------------------------------------------------------------------
// Add one control to the pool, the control should have one control ID
// ------------------------------------------------------------------------
//    
void CAknFepVkbCtrlPool::AddControl(CFepUiBaseCtrl* aControl)
    {
    iControlList.Append(aControl);
    }

// ------------------------------------------------------------------------
// Remove one control from the pool based on control ID
// ------------------------------------------------------------------------
//     
void CAknFepVkbCtrlPool::RemoveControl(const TInt aControlID) 
    {
    CFepUiBaseCtrl* ctrl = Control(aControlID);
    const TInt index = iControlList.Find(ctrl);
    delete ctrl;
    iControlList.Remove(index);
    }
    
// ------------------------------------------------------------------------
// Get one control from the pool based on control ID
// ------------------------------------------------------------------------
//     
CFepUiBaseCtrl* CAknFepVkbCtrlPool::Control(const TInt aControlID) const
    {
    const TInt count = iControlList.Count();
    CFepUiBaseCtrl* ctrl = NULL;
    
    for (TInt i = 0; i < count; i++)
        {
        if (iControlList[i]->ControlId() == aControlID)
            {
            ctrl = iControlList[i];
            break;
            }
        }

    return ctrl;            
    }

// ------------------------------------------------------------------------
// Get one control from the pool based on the index of control array
// ------------------------------------------------------------------------
//   
CFepUiBaseCtrl* CAknFepVkbCtrlPool::ControlByIndex(const TInt aIndex) const
    {
    CFepUiBaseCtrl* ctrl = NULL;
    const TInt count = iControlList.Count();

    if ((aIndex < count) && (aIndex >= 0))
        {
        ctrl = iControlList[aIndex];
        }

    return ctrl;          
    }

// ------------------------------------------------------------------------
// Get current control count in the pool
// ------------------------------------------------------------------------
// 
TInt CAknFepVkbCtrlPool::ControlCount() const
    {
    return iControlList.Count();
    }

// ------------------------------------------------------------------------
// Remove all controls and delete them            
// ------------------------------------------------------------------------
// 
void CAknFepVkbCtrlPool::Reset()
    {
    iControlList.ResetAndDestroy();
    }

// End Of File
