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












#ifndef __AKN_FEP_UI_AVKON_CONTAINER_JAPANESE_H__
#define __AKN_FEP_UI_AVKON_CONTAINER_JAPANESE_H__

// INCLUDES
#include <AknFepUICtrlContainerJapanese.h>
#include <badesca.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATION

// CLASS DECLARATION
class CAknFepUIManagerJapanese;

/**
 * Container for the Japanese Candidate Pane and the Predictive Candidate Pane.
 *
 * @lib AknFep.lib
 * @since 2.6
 */
NONSHARABLE_CLASS(CAknFepUICtrlContainerJapanese)
    : 
    public CBase,
    public MAknFepUICtrlContainerJapanese
    {
public: // Constructer and Destructor
    /**
     * first phase construction
     * @since 2.6
     * @param aUiMng
     * @return 
     */
    static CAknFepUICtrlContainerJapanese* NewL(CAknFepUIManagerJapanese* aUiMng);

    /**
     * Destructor.
     */
    ~CAknFepUICtrlContainerJapanese();

public: // New faunctions
    /**
     * @since 2.6
     * @param aId
     * @return
     */
    MAknFepUICtrlCandidatePaneInterface* CandidatePane(TAknFepUICtrlCandidatePaneId aId);

private:
    /**
     * private c++ constructor.
     * @since 2.6
     */
    CAknFepUICtrlContainerJapanese();

    /**
     * Second phase construction
     * @since 2.6
     * @param aUiMng
     */
    void ConstructL(CAknFepUIManagerJapanese* aUiMng);

private: // Data
    CArrayPtrFlat<MAknFepUICtrlCandidatePaneInterface>*     iCandidatePaneArray;
    };

#endif // __AKN_FEP_UI_AVKON_CONTAINER_JAPANESE_H__

// End of file
