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
* Description:            HWR layout UI class implementation file.
*
*/












// System includes
#include <aknfeppeninputenums.h>
#include <aknedsts.h>               //CAknEdwinState

// User includes
#include "pluginfepmanagerhwr.h"
#include "peninputimeplugincn.h"

// ======== MEMBER FUNCTIONS ========
// -----------------------------------------------------------------------------
// CPluginFepManagerHwr::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerHwr* CPluginFepManagerHwr::NewL(CPenInputImePluginCn& aOwner,
                                                 RPeninputServer* aPenInputServer)
    {
    CPluginFepManagerHwr* self = new(ELeave)CPluginFepManagerHwr(aOwner, aPenInputServer);
    CleanupStack::PushL(self);
    self->BaseConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerHwr::SetNumberModeKeyMappingL
// Set layout UI current number mode key mapping.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerHwr::SetNumberModeKeyMappingL(
                             TAknEditorNumericKeymap aAknEditorNumericKeymap)
    {
    //don't send number key map here, the key mapping is only meaningful
    //in ENumber range
    iNumberKeyMapping = aAknEditorNumericKeymap;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerHwr::HandleCommandL
// Handle command come from FEP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerHwr::HandleCommandL(TInt aCommandId,TInt aParam)
    {
    if( aCommandId == ECmdPenInputSetWindowPos)
        {
        return;
        }
    CPluginFepManagerBase::HandleCommandL(aCommandId, aParam);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerHwr::CPluginFepManagerHwr
// C++ default constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerHwr::CPluginFepManagerHwr(CPenInputImePluginCn& aOwner,
                                           RPeninputServer* aPenInputServer)
    :CPluginFepManagerBase(aOwner, aPenInputServer)
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerHwr::SetLayoutRange
// Set layout primary range.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerHwr::SetLayoutRange(const CAknEdwinState* aEditorState)
    {
    CPluginFepManagerBase::SetLayoutRange(aEditorState);
    
    //set number key mapping
    if( iPermittedRange == ERangeNumber )        
        {
        SendCommandToServer(ECmdPenInputEditorNumericKeyMap, iNumberKeyMapping);
        }
    else
        {
        //for not number only editor, we set plain number and 
        //second range handles other characters
        SendCommandToServer(ECmdPenInputEditorNumericKeyMap, 
                            EAknEditorPlainNumberModeKeymap);    
        }
    }

 // End Of File
