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
* Description:            Provides the CAknFepUiSpellInputPane methods.
*
*/












#include <eikgted.h>            // for CEikGlobalTextEditor
#include <eikedwob.h>           // for MEikEdwinObserver
#include <eikenv.h>             // for CEikonEnv
#include <barsread.h>           // for TResourceReader
#include <eiklabel.h>           // for CEikLable
#include <AknUtils.h>
#include <eikedwin.h> 
#include <appapaclayout.cdl.h>        // for Layout stuff
#include <aknlayoutscalable_apps.cdl.h>
#include <StringLoader.h>

#include "aknfepuispellinputpane.h"

// CONSTANTS
const TInt KNumberOfControl = 1;
const TInt KSpellMaxLength = 63;    
const TInt KTouchSpellEditorMaxLine = 1;  
// Enumarations
enum TControls
    {
    EInputWin
    };

// ================= MEMBER FUNCTIONS ======================

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::CAknFepUiSpellInputPane
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CAknFepUiSpellInputPane::CAknFepUiSpellInputPane()
:iInputWin( NULL )
{
} 

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::CAknFepUiSpellInputPane
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellInputPane::ConstructL( CCoeControl* aParent, 
    const TInt aEditorFlag, const TInt aEditorCase, 
    const TInt aEditorSCTResID,
    const TBool aIsSplitEditor )
{
    // Create as a child window of the specified parent control's window.
    CreateWindowL( aParent );
    
    SetMopParent(aParent);

    // Create input editor
    iInputWin = new ( ELeave ) CEikEdwin;
    // Initialize the editor
    iInputWin->SetContainerWindowL( *this );
    iInputWin->ConstructL(0,0,0,KTouchSpellEditorMaxLine);
    
    iInputWin->SetAknEditorInputMode(EAknEditorTextInputMode);
    iInputWin->SetAknEditorSpecialCharacterTable(aEditorSCTResID);
    if ( aIsSplitEditor )
    	{
        iInputWin->SetAknEditorFlags( iInputWin->AknEdwinFlags() | 
                                      aEditorFlag | EAknEditorFlagNoT9 | 
                                      EAknEditorFlagEnablePartialScreen );
    	}
    else
    	{
         iInputWin->SetAknEditorFlags( iInputWin->AknEdwinFlags() | 
                                   aEditorFlag | EAknEditorFlagNoT9 );
    	}
    iInputWin->SetAknEditorCase(aEditorCase);
	iInputWin->SetMaxLength(KSpellMaxLength);
    iInputWin->SetMopParent(this);
    iInputWin->SetFocus( EFalse );

    iInputWin->ActivateL();

    ActivateL();
}

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CAknFepUiSpellInputPane* CAknFepUiSpellInputPane::NewL(
    CCoeControl* aParent, const TInt aEditorFlag, 
    const TInt aEditorCase, const TInt aEditorSCTResID, 
    const TBool aIsSplitEditor )
    {
    CAknFepUiSpellInputPane* self = new ( ELeave ) CAknFepUiSpellInputPane;
    
    CleanupStack::PushL( self );
    self->ConstructL( aParent, aEditorFlag, aEditorCase, aEditorSCTResID, aIsSplitEditor );
    CleanupStack::Pop();
    
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::~CAknFepUiSpellInputPane
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CAknFepUiSpellInputPane::~CAknFepUiSpellInputPane()
    {
    delete iInputWin;
    iInputWin = NULL;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::InputWin
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CEikEdwin* CAknFepUiSpellInputPane::InputWin() const
    {
    return iInputWin;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::SetInputWinObserver
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellInputPane::SetInputWinObserver( MEikEdwinObserver* aObserver )
    {
    iInputWin->SetEdwinObserver( aObserver );
    }


// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::SetInputWinTextL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellInputPane::SetInputWinTextL( const TDesC* aDes )
    {
    iInputWin->SetTextL( aDes );
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::SelectAllL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellInputPane::SelectAllL()
    {
    iInputWin->SelectAllL();
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::InputCapabilities
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TCoeInputCapabilities CAknFepUiSpellInputPane::InputCapabilities() const
    {
    if (iInputWin->IsFocused())
        {
        return iInputWin->InputCapabilities();
        }

    return ENone;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::OfferKeyEventL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TKeyResponse CAknFepUiSpellInputPane::OfferKeyEventL(
            const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if (iInputWin->IsFocused())
        {
		TKeyResponse  response = iInputWin->OfferKeyEventL( aKeyEvent, aType );
		if ( response == EKeyWasNotConsumed )
			{
			if ( aType == EEventKeyDown || aType == EEventKeyUp )
				{
                response = EKeyWasConsumed;
                }
			}
		return response;
        }

    return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::CountComponentControls
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
TInt CAknFepUiSpellInputPane::CountComponentControls() const
    {
    return KNumberOfControl;
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::ComponentControl
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
CCoeControl* CAknFepUiSpellInputPane::ComponentControl( TInt aIndex ) const
    {
    switch ( aIndex )
        {
        case EInputWin:
            {
            return iInputWin;
            }
        default:
            {
            return NULL;
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::Draw
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellInputPane::Draw( const TRect& /*aRect*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::FocusChanged
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellInputPane::FocusChanged( TDrawNow aDrawNow )
    {
    iInputWin->SetFocus( IsFocused(), aDrawNow );
    }

// ---------------------------------------------------------------------------
// CAknFepUiSpellInputPane::SizeChanged
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepUiSpellInputPane::SizeChanged()
	{
    }

// End of File
