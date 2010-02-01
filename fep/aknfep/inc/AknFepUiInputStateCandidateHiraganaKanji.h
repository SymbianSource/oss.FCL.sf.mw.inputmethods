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
* Description:           
*       Provides the TAknFepUiInputStateCandidateHiraganaKanji definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_CANDIDATE_HIRAGANA_KANJI_H__
#define __AKN_FEP_UI_INPUT_STATE_CANDIDATE_HIRAGANA_KANJI_H__

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
 *  TAknFepUiInputStateCandidateHiraganaKanji class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class TAknFepUiInputStateCandidateHiraganaKanji
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
     TAknFepUiInputStateCandidateHiraganaKanji(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerJapanese* aUIContainer);

public: // Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Initialize state
     * 
     * @since 2.6
     */
    virtual void InitializeStateL();

    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Processing of Closeing UI
     *
     * @since 2.6
     */
    virtual void CloseUI();

protected:// Functions from base classes
    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling short keypress events of the numeric and the star.
     *
     * @since 2.6
     * @param aKey The event key code
     * @return ETrue
     */
    virtual TBool HandleNumericShortKeyL(TInt aKey);

    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling navigate keypress events.
     *
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETure
     */
    virtual TBool HandleNaviKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * From TAknFepUiInputStateEntryJapaneseMultitap
     * Handling auxiliary keypress events including OK and Backspace.
     * 
     * @since 2.6
     * @param aKey The event key code
     * @param aLength The Length of keypress
     * @return ETure
     */
    virtual TBool HandleAuxKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * From TAknFepUiInputStateJapanesePredictiveBase
     * Handling Command events. This Method is called by CAknFepUIManagerJapanese.
     *
     * @since 2.6
     * @param aCommandId Command ID value
     */
     void HandleCommandL(TInt aCommandId);

protected: // New Fanctions
    virtual void UpdateCandidateListL(TInt aIndex = 0);

    };
#endif //__AKN_FEP_UI_INPUT_STATE_CANDIDATE_HIRAGANA_KANJI_H__
// End of file
