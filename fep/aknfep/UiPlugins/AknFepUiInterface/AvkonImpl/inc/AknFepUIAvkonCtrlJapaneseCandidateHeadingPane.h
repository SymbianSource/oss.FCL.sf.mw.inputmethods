/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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












#ifndef CAKNFEPUICTRLJAPANESECANDIDATEHEADINGPANE_H
#define CAKNFEPUICTRLJAPANESECANDIDATEHEADINGPANE_H

// INCLUDES
#include <aknPopupHeadingPane.h>    // CAknPopupHeadingPane

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CAknFepUICtrlJapaneseCandidateHeadingPane class.
*
*  @lib AknFep.lib
*  @since 2.6
*/
NONSHARABLE_CLASS(CAknFepUICtrlJapaneseCandidateHeadingPane) : public CAknPopupHeadingPane
    {
    public:     // New functions
        /**
        * Set the parent window rect of heading pane
        * @since 2.6
        * @param aParentRect    parent window rect
        */
        void SetParentRect(const TRect& aParentRect);

    public:     // Functions from CAknPopupHeadingPane
        virtual void SizeChanged();

    public:     // Functions from CCoeControl
        virtual void ActivateL();

    private:    // Functions from CCoeControl
        TInt CountComponentControls() const;
        CCoeControl* ComponentControl(TInt aIndex) const;

    private:    // Functions from CAknPopupHeadingPane
        void LayoutListHeadingPane();

    private:    // Data
        TRect iParentRect;

    };

#endif  // CAKNFEPUICTRLJAPANESECANDIDATEHEADINGPANE_H

// End of File
