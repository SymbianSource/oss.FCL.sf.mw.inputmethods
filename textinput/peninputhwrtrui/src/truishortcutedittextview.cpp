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
* Description:  Implement of class CTruiShortcutEditTextView
*
*/


#include <akntextsettingpage.h>
#include <trui.rsg>
#include <akntoolbar.h>
#include <eikcolib.h>

#include "truiappui.h"
#include "truishortcutedittextview.h"
#include "truitextsettingpagecontainer.h"
#include "truiengine.h"
#include "truiinfomessageview.h"
#include "truiapplication.h"
#include "truishortcuteditview.h"
#include "truishortcutsview.h"
#include "truicontainerbase.h"

// ======== MEMBER FUNCTIONS ========

CTruiShortcutEditTextView::CTruiShortcutEditTextView()
    {
    }

void CTruiShortcutEditTextView::ConstructL()
    {
    BaseConstructL(R_TRUI_SHORTCUTEDITTEXTVIEW);
    }

CTruiShortcutEditTextView* CTruiShortcutEditTextView::NewL()
    {
    CTruiShortcutEditTextView* self = CTruiShortcutEditTextView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

CTruiShortcutEditTextView* CTruiShortcutEditTextView::NewLC()
    {
    CTruiShortcutEditTextView* self = new( ELeave ) CTruiShortcutEditTextView;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CTruiShortcutEditTextView::~CTruiShortcutEditTextView()
    {
    }
    
// ---------------------------------------------------------------------------
// From class CAknView.
// Returns views id.
// ---------------------------------------------------------------------------
//
TUid CTruiShortcutEditTextView::Id() const
    {
    return KTruiShortcutEditTextViewId;    
    }    

// ---------------------------------------------------------------------------
// From class CAknView.
// Carried out when View is activated
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditTextView::DoActivateL( const TVwsViewId& aPrevViewId,
                  TUid /*aCustomMessageId*/,
                  const TDesC8& /*aCustomMessage*/ )
	{    
    CTruiAppUi* appui = static_cast<CTruiAppUi*>( AppUi() );
    if ( aPrevViewId == TVwsViewId( KUidtruiApp, KTruiInfoMessageViewId ) 
         || aPrevViewId == TVwsViewId( KUidtruiApp, KTruiShortcutsViewId ) )
        {
        if ( appui->HwrEngine()->NewShortcut() )
            {
            iStartView = TVwsViewId( KUidtruiApp, KTruiInfoMessageViewId );
            }
        else
            {
            iStartView = aPrevViewId;
            }        
        }
    
    // Set title pane
    if ( iStartView == TVwsViewId( KUidtruiApp, KTruiInfoMessageViewId ) )
        {
        HBufC* titleText = iEikonEnv->AllocReadResourceLC
                           ( R_TRUI_SHORTCUTEDITTEXTVIEW_TITLEPANE_TEXT );
        HBufC* titleNumber = iEikonEnv->AllocReadResourceLC
                           ( R_TRUI_SHORTCUTEDITTEXTVIEW_TITLEPANE_NUMBER );
         
        // Offer new locfile by localization team
        HBufC* titlePaneText = iEikonEnv->AllocReadResourceLC
                       ( R_TRUI_SHORTCUTEDITVIEW_TITLEPANE_TEXT );
        appui->ShowTitlePaneL( *titlePaneText );
        CleanupStack::PopAndDestroy( titlePaneText );
        CleanupStack::PopAndDestroy( titleNumber );
        CleanupStack::PopAndDestroy( titleText );
        }
	else if ( iStartView == TVwsViewId( KUidtruiApp, KTruiShortcutsViewId ) )
	    {
        HBufC* titleText = iEikonEnv->AllocReadResourceLC
                           ( R_TRUI_SHORTCUTEDITTEXTVIEW_TITLEPANE_TEXT );
        appui->ShowTitlePaneL( *titleText );
        CleanupStack::PopAndDestroy( titleText );
	    }
	    
	// Close navi pane
    appui->CloseNaviPaneL();

    // Open container
    if ( iStartView == TVwsViewId( KUidtruiApp, KTruiInfoMessageViewId ) )
        {
        iContainer = CTruiTextSettingPageContainer::NewL
                                              ( R_TRUI_SHORTCUT_WIZARD_EDITOR,
                                                appui->HwrEngine()->Shortcut(),
                                                ClientRect(),
                                                EFalse,
                        CAknTextSettingPage::EPutCursorAtEnd | 
                        CAknTextSettingPage::EPredictiveTextEntryPermitted );
        }
    else
        {
        iContainer = CTruiTextSettingPageContainer::NewL
                                              ( R_TRUI_SHORTCUT_EDIT_TEXT_EDITOR,
                                                appui->HwrEngine()->Shortcut(),
                                                ClientRect(),
                                                ETrue,
                        CAknTextSettingPage::EPutCursorAtEnd | 
                        CAknTextSettingPage::EPredictiveTextEntryPermitted );
        }    
	AppUi()->AddToStackL( *this, iContainer );	
	}

// ---------------------------------------------------------------------------
// From class CAknView.
// Carried out when View is deactivated
// ---------------------------------------------------------------------------
//
void CTruiShortcutEditTextView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }

    }
