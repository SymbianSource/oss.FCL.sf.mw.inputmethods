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












#ifndef CAKNFEPUICTRLJAPANESECANDIDATELISTBOXVIEW_H
#define CAKNFEPUICTRLJAPANESECANDIDATELISTBOXVIEW_H

// INCLUDES

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CAknFepUICtrlJapaneseCandidateListboxView draws all the list items and 
*  manages the area of the screen allocated for list itself. In particular,
*  this class does not handle margins of the whole list.
*
*  @lib AknFep.lib
*  @since 2.6
*/
NONSHARABLE_CLASS(CAknFepUICtrlJapaneseCandidateListboxView) : public CListBoxView
    {
    public: // Functions from CListBoxView
        virtual void Draw(const TRect* aClipRect = NULL) const;
        virtual void DrawItem(TInt aItemIndex) const;
    };

#endif  // CAKNFEPUICTRLJAPANESECANDIDATELISTBOXVIEW_H

// End of File
