/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the FEP's Avkon UI access methods.
*
*/












#include "AknFepUiAvkonStylusPopUpMenu.h"
#include "AknFepUiInterfacePanic.h"
#include "aknfepuimenus.h"
#include <aknstyluspopupmenu.h>


CAknFepUIAvkonStylusPopUpMenu::~CAknFepUIAvkonStylusPopUpMenu()
	{
	delete iPopupMenu;
	}

void CAknFepUIAvkonStylusPopUpMenu::ConstructL(MAknFepUiInterfaceMenuObserver* aObserver, const TPoint& aPoint)
	{
	iObserver = aObserver;
	iPopupMenu = CAknStylusPopUpMenu::NewL(this, aPoint);
	}

void CAknFepUIAvkonStylusPopUpMenu::AddMenuItemL(const TDesC& aItem, const TInt aCommandId)
	{
	iPopupMenu->AddMenuItemL(aItem, aCommandId);
	}

void CAknFepUIAvkonStylusPopUpMenu::SetPosition(const TPoint& aPoint)
	{
	iPopupMenu->SetPosition(aPoint);
	}

void CAknFepUIAvkonStylusPopUpMenu::ShowMenu()
	{
	iPopupMenu->ShowMenu();
	}

void CAknFepUIAvkonStylusPopUpMenu::DynInitMenuPaneL(TInt /*aResourceId*/,CEikMenuPane* /*aMenuPane*/)
	{
	AknFepUiInterfacePanic(EAvkonImplNotSupported);
	}

void CAknFepUIAvkonStylusPopUpMenu::DynInitMenuBarL(TInt /*aResourceId*/,CEikMenuBar* /*aMenuBar*/)
	{
	AknFepUiInterfacePanic(EAvkonImplNotSupported);
	}

void CAknFepUIAvkonStylusPopUpMenu::SetEmphasis(CCoeControl* /*aMenuControl*/,TBool /*aEmphasis*/)
	{
	AknFepUiInterfacePanic(EAvkonImplNotSupported);
	}

void CAknFepUIAvkonStylusPopUpMenu::ProcessCommandL(TInt aCommandId)
	{
	iObserver->ProcessCommandL(aCommandId);
	}

