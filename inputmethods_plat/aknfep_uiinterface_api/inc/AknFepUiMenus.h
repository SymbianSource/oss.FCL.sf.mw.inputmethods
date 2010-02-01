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
*       Interface through which the FEP can use menu components
*
*/















#ifndef AKNFEPUIINTERFACEMENUS_H
#define AKNFEPUIINTERFACEMENUS_H

#include <e32base.h>
#include <eikcmobs.h>
#include <avkon.hrh>

class CEikMenuPane;
class MAknFepUiInterfaceMenuObserver;


class CAknFepUiInterfaceMenuPane : public CBase
	{
public:
    // Struct copied from CEikMenuPaneItem
    struct SItemData
        {
        enum { ENominalTextLength=40 };
        TInt iCommandId;
        TInt iCascadeId;
        TInt iFlags;
        TBuf<ENominalTextLength> iText; // less than this actually stored
        TBuf<1> iExtraText;
        };
 
public:
    virtual void ConstructMenuSctRowFromDialogL( TInt aCharCase, TDes& aSpecialChars, TInt aResourceId ) = 0;
    virtual void ConstructMenuSctRowL( TDes& aSpecialChars, TInt aResourceId ) = 0;
    virtual void DeleteMenuItem(TInt aCommandId) = 0;	
    virtual void InsertMenuItemL(const SItemData& aMenuItem, TInt aPosition) = 0;
    virtual TBool MenuItemExists(TInt aCommandId, TInt& aPosition) = 0;
    virtual TInt NumberOfItemsInPane() const = 0;
    virtual void SetItemDimmed(TInt aCommandId, TBool aDimmed) = 0;
    virtual void SetMenuPane(CEikMenuPane* aMenuPane) = 0;
    virtual CEikMenuPane* GetMenuPane() = 0;
	};

class CAknFepUiInterfaceMenuBar : public CBase
	{
public:
	// Enum copied from CEikMenuBar
    enum TMenuType {
        EMenuOptions = 0,
        EMenuEdit = 2,
        EMenuOptionsNoTaskSwapper = 3
        };
public:
	virtual CAknFepUiInterfaceMenuPane* MenuPane() = 0;
	virtual void SetMenuType(TMenuType aMenuType) = 0;
	virtual void StopDisplayingMenuBar() = 0;
	virtual void ReplaceAllMenuPanes(TInt aResourceId) = 0; //TitleArray replaced with use-case tailored functions
	virtual void TryDisplayMenuBarL() = 0;
	virtual void SetFocus(TBool aFocus) = 0;
	virtual void RemoveFromStack() = 0;
	virtual TInt FirstMenuPaneResourceId() = 0;
	virtual TBool IsDisplayed() = 0;
	virtual void AddToStackL(TInt aPriority,TInt aStackingFlags) = 0;
    virtual void TryDisplayMenuBarWithoutFepMenusL() = 0;
    virtual void SetMenuTitleResourceId(TInt aResourceId) = 0;
    virtual void SetEditMenuObserver(MAknFepUiInterfaceMenuObserver* aEditMenuObserver) = 0;
    virtual void RemoveEditMenuObserver(MAknFepUiInterfaceMenuObserver* aEditMenuObserver) = 0;
	};

class MAknFepUiInterfaceMenuObserver : public MEikCommandObserver
	{
public:
	IMPORT_C virtual void DynInitMenuPaneL(TInt aResourceId,CAknFepUiInterfaceMenuPane* aMenuPane);
	IMPORT_C virtual void DynInitMenuBarL(TInt aResourceId,CAknFepUiInterfaceMenuBar* aMenuBar);
	virtual void SetEmphasis(CBase* aMenuControl,TBool aEmphasis)=0;
	};

#endif
