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
* Description:            HWR layout UI class implementation file.
*
*/












#include "pluginfepmanagerhwr.h"
#include "peninputimepluginjp.h"
#include <aknfeppeninputenums.h>
#include <aknedsts.h>               //CAknEdwinState

// -----------------------------------------------------------------------------
// CPluginFepManagerHwr::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerHwr* CPluginFepManagerHwr::NewL(CPenInputImePluginJp& aOwner,
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
    TAknEditorNumericKeymap aAknEditorNumericKeymap )
    {
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
CPluginFepManagerHwr::CPluginFepManagerHwr(CPenInputImePluginJp& aOwner,
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
    if ( iPermittedRange == ERangeNumber)
        {
        SendCommandToServer( ECmdPenInputEditorNumericKeyMap,
                             iNumberKeyMapping );
        }
    else
        {
        //for alphanumeric editor, always use EAknEditorAlphanumericNumberModeKeymap
        SendCommandToServer( ECmdPenInputEditorNumericKeyMap,
                             EAknEditorAlphanumericNumberModeKeymap );
        }
    }

 // End Of File
