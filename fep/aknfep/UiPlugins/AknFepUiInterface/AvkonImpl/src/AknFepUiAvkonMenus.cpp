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












#include "AknFepUiAvkonMenus.h"
#include <eikmenub.h>
#include <eikmenup.h>
#include <eikappui.h>

CAknFepUIAvkonMenuPaneImpl::CAknFepUIAvkonMenuPaneImpl()
	{
	}

void CAknFepUIAvkonMenuPaneImpl::ConstructMenuSctRowFromDialogL( TInt aCharCase, TDes& aSpecialChars, TInt aResourceId )
	{
	if (iMenuPane)
		iMenuPane->ConstructMenuSctRowFromDialogL(aCharCase, aSpecialChars, aResourceId);
	}

void CAknFepUIAvkonMenuPaneImpl::ConstructMenuSctRowL( TDes& aSpecialChars, TInt aResourceId )
	{
	if (iMenuPane)
		iMenuPane->ConstructMenuSctRowL(aSpecialChars, aResourceId);
	}

void CAknFepUIAvkonMenuPaneImpl::DeleteMenuItem(TInt aCommandId)
	{
	if (iMenuPane)
		iMenuPane->DeleteMenuItem(aCommandId);
	}

void CAknFepUIAvkonMenuPaneImpl::InsertMenuItemL(const SItemData& aMenuItem, TInt aPosition)
	{
	if (iMenuPane)
		iMenuPane->InsertMenuItemL(*reinterpret_cast<const CEikMenuPaneItem::SData*>(&aMenuItem), aPosition);
	}

TBool CAknFepUIAvkonMenuPaneImpl::MenuItemExists(TInt aCommandId, TInt& aPosition)
	{
	if (iMenuPane)
		return iMenuPane->MenuItemExists(aCommandId, aPosition);
	return EFalse;
	}

TInt CAknFepUIAvkonMenuPaneImpl::NumberOfItemsInPane() const
	{
	if (iMenuPane)
		return iMenuPane->NumberOfItemsInPane();
	return 0;
	}

void CAknFepUIAvkonMenuPaneImpl::SetItemDimmed(TInt aCommandId, TBool aDimmed)
	{
	TInt index(0);
    if (iMenuPane && MenuItemExists( aCommandId, index ) )
		iMenuPane->SetItemDimmed(aCommandId, aDimmed);
	}

void CAknFepUIAvkonMenuPaneImpl::SetMenuPane(CEikMenuPane* aMenuPane)
	{
	iMenuPane = aMenuPane;
	}
	
CEikMenuPane* CAknFepUIAvkonMenuPaneImpl::GetMenuPane()
  {
  return iMenuPane;
  }
    
CAknFepUIAvkonMenuBarImpl::CAknFepUIAvkonMenuBarImpl()
	{
	}

CAknFepUiInterfaceMenuPane* CAknFepUIAvkonMenuBarImpl::MenuPane()
	{
	if (iMenuBar)
	    {
    	iTempMenuPane.iMenuPane = iMenuBar->MenuPane();
	    }
	return &iTempMenuPane;
	}

void CAknFepUIAvkonMenuBarImpl::SetMenuType(TMenuType aMenuType)
	{
	if (iMenuBar)
		iMenuBar->SetMenuType(static_cast<CEikMenuBar::TMenuType>(aMenuType));
	}

void CAknFepUIAvkonMenuBarImpl::StopDisplayingMenuBar()
	{
	if (iMenuBar)
		iMenuBar->StopDisplayingMenuBar();
	}

void CAknFepUIAvkonMenuBarImpl::ReplaceAllMenuPanes(TInt aResourceId)
	{
    // remove app menu panes
    TInt count = iMenuBar->TitleArray()->Count() -  1;
    for (TInt ii = 0; ii<count; ii++)
        {
        delete iMenuBar->TitleArray()->At(0);
        iMenuBar->TitleArray()->Delete(0);
        }
    // add the mode menu
    iMenuBar->TitleArray()->At(0)->iData.iMenuPaneResourceId = aResourceId;
	}

void CAknFepUIAvkonMenuBarImpl::TryDisplayMenuBarL()
	{
	if (iMenuBar)
		iMenuBar->TryDisplayMenuBarL();
	}

void CAknFepUIAvkonMenuBarImpl::SetFocus(TBool aFocus)
	{
	if (iMenuBar)
		iMenuBar->SetFocus(aFocus);
	}

void CAknFepUIAvkonMenuBarImpl::RemoveFromStack()
	{
	if (iMenuBar)
		CCoeEnv::Static()->AppUi()->RemoveFromStack(iMenuBar);
	}

TInt CAknFepUIAvkonMenuBarImpl::FirstMenuPaneResourceId()
	{
	if (iMenuBar)
		{
        CEikMenuBar::CTitleArray* titles = iMenuBar->TitleArray();
        if( titles->Count()	> 0 )
            {
            return (*titles)[0]->iData.iMenuPaneResourceId;
            }
		}
	return 0;
	}

TBool CAknFepUIAvkonMenuBarImpl::IsDisplayed()
	{
	if (iMenuBar)
		return iMenuBar->IsDisplayed();
	return EFalse;
	}

void CAknFepUIAvkonMenuBarImpl::AddToStackL(TInt aPriority, TInt aStackingFlags)
	{
	if (iMenuBar)
		CCoeEnv::Static()->AppUi()->AddToStackL(iMenuBar, aPriority, aStackingFlags);
	}

void CAknFepUIAvkonMenuBarImpl::TryDisplayMenuBarWithoutFepMenusL()
	{
	if (iMenuBar)
		iMenuBar->TryDisplayMenuBarWithoutFepMenusL();
	}

void CAknFepUIAvkonMenuBarImpl::SetMenuTitleResourceId(TInt aResourceId)
	{
	if (iMenuBar)
		iMenuBar->SetMenuTitleResourceId(aResourceId);
	}
	
void CAknFepUIAvkonMenuBarImpl::SetEditMenuObserver(MAknFepUiInterfaceMenuObserver* aEditMenuObserver)
	{
	iEditMenuObserver.iObserver = aEditMenuObserver;
	iMenuBar->SetEditMenuObserver(&iEditMenuObserver);
	}
	
void CAknFepUIAvkonMenuBarImpl::RemoveEditMenuObserver(MAknFepUiInterfaceMenuObserver* aEditMenuObserver)
	{
	if (iEditMenuObserver.iObserver == aEditMenuObserver)
		{
		iMenuBar->RemoveEditMenuObserver(&iEditMenuObserver);
		}
	}

void CAknFepUIAvkonMenuBarImpl::TEditMenuObserver::ProcessCommandL(TInt aCommandId)
	{
	if (iObserver)
		iObserver->ProcessCommandL(aCommandId);
	}

void CAknFepUIAvkonMenuBarImpl::TEditMenuObserver::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
	{
	if (iObserver)
		{
		iTempPane.iMenuPane = aMenuPane;
		iObserver->DynInitMenuPaneL(aResourceId, &iTempPane);
		}
	}

void CAknFepUIAvkonMenuBarImpl::TEditMenuObserver::SetEmphasis(CCoeControl* /*aMenuControl*/,TBool aEmphasis)
	{
	// client code is not currently using aMenuControl
	if (iObserver)
		iObserver->SetEmphasis(NULL, aEmphasis);
	}


CAknFepUIAvkonMenuBarOwningImpl::~CAknFepUIAvkonMenuBarOwningImpl()
	{
	delete iMenuBar;
	}

void CAknFepUIAvkonMenuBarOwningImpl::ConstructL(MAknFepUiInterfaceMenuObserver* aObserver, TInt aHotKeyResourceId, TInt aMenuTitleResourceId)
	{
	iObserver = aObserver;
    iMenuBar = new (ELeave) CEikMenuBar() ;
    iMenuBar->ConstructL(this, aHotKeyResourceId, aMenuTitleResourceId) ;
	}

void CAknFepUIAvkonMenuBarOwningImpl::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
	{
	iTempMenuPane.iMenuPane = aMenuPane;
	if (iObserver)
		iObserver->DynInitMenuPaneL(aResourceId, &iTempMenuPane);
	}

void CAknFepUIAvkonMenuBarOwningImpl::DynInitMenuBarL(TInt aResourceId,CEikMenuBar* /*aMenuBar*/)
	{
	if (iObserver)
		iObserver->DynInitMenuBarL(aResourceId, this);
	}

void CAknFepUIAvkonMenuBarOwningImpl::SetEmphasis(CCoeControl* aMenuControl,TBool aEmphasis)
	{
	CBase* obj = this;
	if (aMenuControl != iMenuBar)
		{
		iTempMenuPane.iMenuPane = static_cast<CEikMenuPane*>(aMenuControl);
		obj = &iTempMenuPane;
		}
	if (iObserver)
		iObserver->SetEmphasis(obj, aEmphasis);
	}

void CAknFepUIAvkonMenuBarOwningImpl::ProcessCommandL(TInt aCommandId)
	{
	if (iObserver)
		iObserver->ProcessCommandL(aCommandId);
	}

