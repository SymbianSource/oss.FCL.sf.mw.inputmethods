/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the AknFepPluginMenuManager methods.
*
*/











#include <coemain.h>
#include <coeaui.h>

#include "AknFepPluginMenuManager.h" // MAknFepManagerInterface
#include "AknFepManager.h" 


// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepPluginMenuManager* CAknFepPluginMenuManager::NewL(CAknFepManager* aManager)
    {
    return new(ELeave) CAknFepPluginMenuManager(aManager);
    }

// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::~CAknFepPluginMenuManager
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepPluginMenuManager::~CAknFepPluginMenuManager()
    {
    if( iOptionMenuBar )
        {
        iOptionMenuBar->RemoveFromStack();
        delete iOptionMenuBar;
        }
    }

// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::aResourceId
// Show speicfied menu pane
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginMenuManager::ShowL(TInt aResourceId)
    {
    if( !iOptionMenuBar )
        {
		iOptionMenuBar = iFepManager->UiInterface()->NewMenuBarL(this, 0, aResourceId);
        }
    else
        {
        iOptionMenuBar->SetMenuTitleResourceId(aResourceId);
        }

    if( !iOptionMenuBar->IsDisplayed() )
        {
        iOptionMenuBar->AddToStackL(ECoeStackPriorityDialog + 1,
                                    ECoeStackFlagStandard);
        iOptionMenuBar->SetMenuType(CAknFepUiInterfaceMenuBar::EMenuOptionsNoTaskSwapper);
        iOptionMenuBar->TryDisplayMenuBarWithoutFepMenusL();
        }
    }

// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::IsShowing
// Get menu show status
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepPluginMenuManager::IsShowing()
    {
    if( iOptionMenuBar )
        {
        return iOptionMenuBar->IsDisplayed();
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::Hide
// Hide current showed menu
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginMenuManager::Hide()
    {
    if( IsShowing() )
        {
        iOptionMenuBar->StopDisplayingMenuBar();
        }
    }

// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::MenuPaneId
// Get current menu resource id
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAknFepPluginMenuManager::MenuPaneId()
    {
    if( iOptionMenuBar )
        {
        return iOptionMenuBar->FirstMenuPaneResourceId();
        }
    return 0;
    }

// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::SetEmphasis
// called when menu get or lose focus
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginMenuManager::SetEmphasis(CBase* aMenuControl, TBool aEmphasis)
    {
    if( !aEmphasis && aMenuControl == iOptionMenuBar)
        {
        iOptionMenuBar->SetFocus(EFalse);
		iOptionMenuBar->RemoveFromStack();
        }
    }

// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::ProcessCommandL
// Process command user selects
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepPluginMenuManager::ProcessCommandL(TInt aCommandId)
    {
    if( aCommandId == EEikCmdCanceled)
        {
        //The command is not sent to FEP
        Hide();
        }
    }

// -----------------------------------------------------------------------------
// CAknFepPluginMenuManager::MenuPaneId
// C++ default constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepPluginMenuManager::CAknFepPluginMenuManager(CAknFepManager* aFepManager)
:iFepManager(aFepManager)
    {
    }

//End Of File
