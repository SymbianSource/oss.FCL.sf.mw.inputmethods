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
* Description:            Candidate list
*
*/












// INCLUDE FILES
#include <eikdef.h>

#include <aknViewAppUi.h>
#include <avkon.hrh>
#include <aknPopup.h>
#include <aknlists.h>
#include <AknsDrawUtils.h>
#include <AknLayout.lag>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlJapaneseCandidateHeadingPane.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateHeadingPane::SetParentRect
// Set rect of parent window
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateHeadingPane::SetParentRect(
        const TRect& aParentRect)
    {
    iParentRect = aParentRect;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateHeadingPane::SizeChanged
// This function is called when changing window size.
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateHeadingPane::SizeChanged()
    {
    LayoutListHeadingPane();
    AknsUtils::RegisterControlPosition(this);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateHeadingPane::ActivateL
// This function is called when doing active
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateHeadingPane::ActivateL()
    {
    CCoeControl::ActivateL();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateHeadingPane::CountComponentControls
// Return a number of control in Heading Pane
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapaneseCandidateHeadingPane::CountComponentControls() const
    {
    return 1;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateHeadingPane::ComponentControl
// Return a control object
// -----------------------------------------------------------------------------
//
CCoeControl* CAknFepUICtrlJapaneseCandidateHeadingPane::ComponentControl(
        TInt /*aIndex*/) const
    {
    return Prompt();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidateHeadingPane::LayoutListHeadingPane
// calculate layout from LAF information
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidateHeadingPane::LayoutListHeadingPane()
    {
    TAknTextLineLayout textLayout =
            AKN_LAYOUT_TEXT_Candidate_selection_list_texts_Line_1;
    textLayout.il = 0;

    TRect parentRect(0, 0, iParentRect.Width(), iParentRect.Height());
    AknLayoutUtils::LayoutLabel(Prompt(), parentRect, textLayout);

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb color;
    TInt error = AknsUtils::GetCachedColor( skin,
                                            color,
                                            KAknsIIDQsnTextColors,
                                            EAknsCIQsnTextColorsCG7 );
    if (!error)
        {
        // Ignore error
        TRAP_IGNORE( AknLayoutUtils::OverrideControlColorL(
                            *Prompt(),
                            EColorLabelText, color ) );
        }
    }

// End of File
