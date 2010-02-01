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
*       Avkon implementation of the FEP's UI component access
*
*/











#ifndef AKNFEPUIAVKONSTYLUSPOPUPMENU_H
#define AKNFEPUIAVKONSTYLUSPOPUPMENU_H

#include "AknFepUiStylusPopupMenu.h"
#include <eikmobs.h>

class CAknStylusPopUpMenu;
class MAknFepUiInterfaceMenuObserver;


class CAknFepUIAvkonStylusPopUpMenu : public CAknFepUiInterfaceStylusPopUpMenu, public MEikMenuObserver
	{
public:
	~CAknFepUIAvkonStylusPopUpMenu();
	void ConstructL(MAknFepUiInterfaceMenuObserver* aObserver, const TPoint& aPoint);

public:
    void AddMenuItemL(const TDesC& aItem, const TInt aCommandId);
    void SetPosition(const TPoint& aPoint);    
    void ShowMenu();

private:	// from MEikMenuObserver
	void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
	void DynInitMenuBarL(TInt aResourceId,CEikMenuBar* aMenuBar);
	void SetEmphasis(CCoeControl* aMenuControl,TBool aEmphasis);

private:	// from MEikCommandObserver
	void ProcessCommandL(TInt aCommandId);

private:
	MAknFepUiInterfaceMenuObserver* iObserver;
	CAknStylusPopUpMenu* iPopupMenu;
	};


#endif
