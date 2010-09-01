/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#include "peninputpluginkrcombinelistquerydialog.h"

// -----------------------------------------------------------------------------
// CPeninputPluginKrCombineListQueryDialog::CPeninputPluginKrCombineListQueryDialog
// Construction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputPluginKrCombineListQueryDialog::CPeninputPluginKrCombineListQueryDialog(TInt* aIndex)
    :CAknListQueryDialog(aIndex)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputPluginKrCombineListQueryDialog::CPeninputPluginKrCombineListQueryDialog
// Construction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputPluginKrCombineListQueryDialog::CPeninputPluginKrCombineListQueryDialog(
          CListBoxView::CSelectionIndexArray* aSelectionIndexArray)
    :CAknListQueryDialog(aSelectionIndexArray)
    {
    }


// -----------------------------------------------------------------------------
// CPeninputPluginKrCombineListQueryDialog::HandleResourceChange
// Cancel itself when rotating screen  
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputPluginKrCombineListQueryDialog::HandleResourceChange( TInt aType )
    {
    CAknListQueryDialog::HandleResourceChange(aType);
    if( aType == KEikDynamicLayoutVariantSwitch ) 
        {
        TRAP_IGNORE(CAknListQueryDialog::TryExitL(EAknSoftkeyCancel));
        }
    }


