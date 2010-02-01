/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the CAknFepUiSpellContainer methods.
*
*/












// INCLUDE FILES
#include <eikgted.h>                  // for CEikGlobalTextEditor
#include <eiktxlbm.h>                 // for CTextListBoxModel
#include <eikclbd.h>                  // for CColumnListBoxData
#include <eikbtgpc.h>                 // for CEikButtonGroupContainer
#include <barsread.h>                 // for TResourceReader
#include <eikclb.h>                   // for CEikColumnListBox
#include <aknnotewrappers.h>          // for CAknInformationNote
#include <appapaclayout.cdl.h>        // for Layout stuff
#include <StringLoader.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>

#include "aknfepuispellcontainer.h"            
#include "aknfepuispellinputpane.h"

// CONSTANTS
const TInt KNumberOfContainerControl = 1;

// Enumarations
enum TControls
    {
    EInputPane
    };

// ================= MEMBER FUNCTIONS ======================

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::CAknFepUiSpellContainer
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CAknFepUiSpellContainer::CAknFepUiSpellContainer()
:iInputPane( NULL )
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::InputPane
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CAknFepUiSpellInputPane* CAknFepUiSpellContainer::InputPane()
    {
    return iInputPane;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::SetInputWinObserver
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellContainer::SetInputWinObserver( MEikEdwinObserver* aObserver )
    {
    iInputPane->SetInputWinObserver(aObserver);
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellContainer::ConstructL(const TInt aEditorFlag, 
    const TInt aEditorCase, const TInt aEditorSCTResID)
    {  
    CreateWindowL();

    iInputPane = CAknFepUiSpellInputPane::NewL( this, aEditorFlag, aEditorCase,aEditorSCTResID);
    
    iInputPane->SetMopParent(this);
    
    iInputPane->SetFocus( ETrue );
    SetContainerRect();
    
    iEikonEnv->EikAppUi()->AddToStackL(this,ECoeStackPriorityDialog);    
    ActivateL();
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CAknFepUiSpellContainer* CAknFepUiSpellContainer::NewL(const TInt aEditorFlag, 
    const TInt aEditorCase, const TInt aEditorSCTResID)
    {
    CAknFepUiSpellContainer* self = new ( ELeave ) CAknFepUiSpellContainer();
    CleanupStack::PushL( self );
    
    self->SetMopParent(CEikonEnv::Static()->EikAppUi());
    self->ConstructL(aEditorFlag, aEditorCase, aEditorSCTResID);

    CleanupStack::Pop();
    
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::~CAknFepUiSpellContainer
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CAknFepUiSpellContainer::~CAknFepUiSpellContainer()
    {
    iEikonEnv->EikAppUi()->RemoveFromStack(this);
    
    delete iInputPane;
    iInputPane = NULL;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::CountComponentControls
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TInt CAknFepUiSpellContainer::CountComponentControls() const
    {
    return KNumberOfContainerControl; 
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::ComponentControl
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CCoeControl* CAknFepUiSpellContainer::ComponentControl( TInt aIndex ) const
    {
    switch ( aIndex )
        {
        case EInputPane:
            {
            return iInputPane;
            }
        default:
            {
            return NULL;
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::Draw
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellContainer::Draw( const TRect& /*aRect*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::InputCapabilities
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TCoeInputCapabilities CAknFepUiSpellContainer::InputCapabilities() const
    {
    if ( iInputPane && iInputPane->IsFocused())
        {
        return iInputPane->InputCapabilities();
        }

    return ENone;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::OfferKeyEventL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TKeyResponse CAknFepUiSpellContainer::OfferKeyEventL( 
            const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if ( iInputPane && iInputPane->IsFocused())
        {
        return iInputPane->OfferKeyEventL( aKeyEvent, aType );
        }
    
    return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::SizeChanged
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellContainer::SizeChanged()
    {  
    iInputPane->SetRect(Rect());
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::SetContainerRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellContainer::FocusChanged(TDrawNow aDrawNow)
    {
    TBool bFocused = IsFocused() ? ETrue : EFalse;
    
    if ( ComponentControl( 0 ) )
        {
        ComponentControl( 0 )->SetFocus( bFocused, aDrawNow );
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellContainer::SetContainerRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void  CAknFepUiSpellContainer::SetContainerRect()
    {
    TAknWindowLineLayout appWnd = AknLayoutScalable_Apps::application_window().LayoutLine();
    TAknWindowLineLayout mainPane;
    if ( Layout_Meta_Data::IsLandscapeOrientation())
        {
        mainPane = AknLayoutScalable_Apps::main_pane(4).LayoutLine();	
        }
    else
        {
        mainPane = AknLayoutScalable_Apps::main_pane(0).LayoutLine();		
        }
     
    TAknLayoutRect mainPaneRect;
    mainPaneRect.LayoutRect(appWnd.Rect(), mainPane);

    mainPaneRect.Rect();
    
    TInt ay = mainPaneRect.Rect().iTl.iY + 
              ((mainPaneRect.Rect().iBr.iY - mainPaneRect.Rect().iTl.iY) / 5)*4;
    
    TRect containerRect(0, ay, mainPaneRect.Rect().iBr.iX, mainPaneRect.Rect().iBr.iY);
    TRect tempRect = TRect(0,0,1,1);
    SetRect(tempRect);
    }
    
// End of File  
