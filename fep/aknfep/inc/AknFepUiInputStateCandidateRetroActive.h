/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepUiInputStateCandidateRetroActive definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_CANDIDATE_RETROACTIVE_H__
#define __AKN_FEP_UI_INPUT_STATE_CANDIDATE_RETROACTIVE_H__

// INCLUDES
#include <e32std.h>

#include "AknFepUiInputStateEntryJapaneseMultitap.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepUIManagerStateInterface;
class CAknFepUICtrlJapaneseCandidatePane;
enum TKeyPressLength;

/**
 *  TAknFepUiInputStateCandidateRetroActive class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateCandidateRetroActive
    :public TAknFepUiInputStateEntryJapaneseMultitap
    {
public: //  Constructors and destructor
    /**
     * C++ default constructor
     *
     * @since 2.6
     * @param aOwner Object derived MAknFepUIManagerStateInterfa
     * @param aUIContainer MAknFepUICtrlContainerJapanese object
     */
     TAknFepUiInputStateCandidateRetroActive(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerJapanese* aUIContainer);

public: // Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Initialize state
     *
     * @since 2.6
     */
    void InitializeStateL();

    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Processing of Closeing UI
     *
     * @since 2.6
     */
    void CloseUI();

protected:// Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling short keypress events of the numeric and the star.
     *
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue
     */
    TBool HandleNumericShortKeyL(TInt aKey);

    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling navigate keypress events.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETure
     */
    TBool HandleNaviKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling auxiliary keypress events including OK and Backspace.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETure
     */
    TBool HandleAuxKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * From TAknFepInputStateBase
     * Handling Command events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 2.6
     * @param aCommandId Command ID value
     */
     void HandleCommandL(TInt aCommandId);

private: // New Functions
    void ShowListL(TInt aCommandId, TInt aIndex = 0);
    void ShowPredictiveListL();
    void DoSelectItemL();
    void DoCommitItemL();

    };
#endif //__AKN_FEP_UI_INPUT_STATE_CANDIDATE_RETROACTIVE_H__
// End of file
