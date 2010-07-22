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
* Description:           
*       Provides the CAknFepUICtrlContainerJapanese definition.
*
*/












#ifndef __AKN_FEP_UI_CONTAINER_JAPANESE_H__
#define __AKN_FEP_UI_CONTAINER_JAPANESE_H__

// INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATION

// CLASS DECLARATION
class MAknFepUICtrlCandidatePaneInterface;

/**
 * Container for the Japanese Candidate Pane and the Predictive Candidate Pane.
 *
 * @lib AknFep.lib
 * @since 2.6
 */
class MAknFepUICtrlContainerJapanese
    {
public:
    /**
     *
     * @since 2.6
     */
    enum TAknFepUICtrlCandidatePaneId
        {
        EJapaneseCandidatePane,
        EJapanesePredictiveCandidatePane,
        EJapaneseCandidatePaneMax
        };

public: // New faunctions
    /**
     * destructor
     */
    virtual ~MAknFepUICtrlContainerJapanese() = 0;

    /**
     * @since 2.6
     * @param aId
     * @return
     */
    virtual MAknFepUICtrlCandidatePaneInterface* CandidatePane(TAknFepUICtrlCandidatePaneId aId) = 0;
    };

#endif // __AKN_FEP_UI_CONTAINER_JAPANESE_H__

// End of file
