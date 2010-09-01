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
*       Avkon implementation of the FEP's Menu component
*
*/











#ifndef AKNFEPUIAVKONMENUS_H
#define AKNFEPUIAVKONMENUS_H

#include "aknfepuimenus.h"
#include <eikmobs.h>

class CEikMenuPane;
class CEikMenuBar;
 

class CAknFepUIAvkonMenuPaneImpl : public CAknFepUiInterfaceMenuPane
	{
public:
	CAknFepUIAvkonMenuPaneImpl();
    void ConstructMenuSctRowFromDialogL( TInt aCharCase, TDes& aSpecialChars, TInt aResourceId );
    void ConstructMenuSctRowL( TDes& aSpecialChars, TInt aResourceId );
    void DeleteMenuItem(TInt aCommandId);	
    void InsertMenuItemL(const SItemData& aMenuItem, TInt aPosition);
    TBool MenuItemExists(TInt aCommandId, TInt& aPosition);
    TInt NumberOfItemsInPane() const;
    void SetItemDimmed(TInt aCommandId, TBool aDimmed);
    void SetMenuPane(CEikMenuPane* aMenuPane);
    CEikMenuPane* GetMenuPane();
public:
	CEikMenuPane* iMenuPane;	// not owned
	};


class CAknFepUIAvkonMenuBarImpl : public CAknFepUiInterfaceMenuBar
	{
private:
	NONSHARABLE_CLASS(TEditMenuObserver) : public MEikMenuObserver
		{
	public: // from MEikMenuObserver
		void ProcessCommandL(TInt aCommandId);
		void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
		void SetEmphasis(CCoeControl* aMenuControl,TBool aEmphasis);
	public:
		MAknFepUiInterfaceMenuObserver* iObserver;
		CAknFepUIAvkonMenuPaneImpl iTempPane;
		};
public:
	CAknFepUIAvkonMenuBarImpl();
	CAknFepUiInterfaceMenuPane* MenuPane();
	void SetMenuType(TMenuType aMenuType);
	void StopDisplayingMenuBar();
	void ReplaceAllMenuPanes(TInt aResourceId);
	void TryDisplayMenuBarL();
	void SetFocus(TBool aFocus);
	void RemoveFromStack();
	TInt FirstMenuPaneResourceId();
	TBool IsDisplayed();
	void AddToStackL(TInt aPriority,TInt aStackingFlags);
    void TryDisplayMenuBarWithoutFepMenusL();
    void SetMenuTitleResourceId(TInt aResourceId);
    void SetEditMenuObserver(MAknFepUiInterfaceMenuObserver* aEditMenuObserver);
    void RemoveEditMenuObserver(MAknFepUiInterfaceMenuObserver* aEditMenuObserver);

public:
	CEikMenuBar* iMenuBar;	// not owned
	CAknFepUIAvkonMenuPaneImpl iTempMenuPane;
	TEditMenuObserver iEditMenuObserver;
	};


class CAknFepUIAvkonMenuBarOwningImpl : public CAknFepUIAvkonMenuBarImpl, public MEikMenuObserver
	{
public:
	~CAknFepUIAvkonMenuBarOwningImpl();
	void ConstructL(MAknFepUiInterfaceMenuObserver* aObserver, TInt aHotKeyResourceId, TInt aMenuTitleResourceId);

private:	// from MEikMenuObserver
	void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
	void DynInitMenuBarL(TInt aResourceId,CEikMenuBar* aMenuBar);
	void SetEmphasis(CCoeControl* aMenuControl,TBool aEmphasis);

private:	// from MEikCommandObserver
	void ProcessCommandL(TInt aCommandId);

private:
	// owns CAknFepUIAvkonMenuBarImpl::iMenuBar
	MAknFepUiInterfaceMenuObserver* iObserver;
	};


#endif
