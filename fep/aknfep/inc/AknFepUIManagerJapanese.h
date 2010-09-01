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
*       Provides the CAknFepManagerJapanese definition.
*
*/












#ifndef __AKN_FEP_UI_MANAGER_JAPANESE_H__
#define __AKN_FEP_UI_MANAGER_JAPANESE_H__

// INCLUDES
#include <bldvariant.hrh>
#include <AknUtils.h>
#include <uikon.hrh>                        //TAknEditorNumericKeymap

#include "AknFepManagerInterface.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUiManagerBase.h"
#include "AknFepUiInputStateJapaneseBase.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepManagerUIInterface;
class MAknFepUIManagerStateInterface;
class CAknFepCaseManager;
class CPtiEngine;
class MAknFepUICtrlContainerJapanese;

/**
 * CAknFepUIManagerJapanese class.
 * This class is mounted based on CAknFepUiManager.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class CAknFepUIManagerJapanese : public CAknFepUIManagerBase
{
private:
    /**
     * private constants
     */
    enum
        {
        EBaseLanguageNumericKeymapIndex = 0x00FF,
        EMaskShiftOffset                = 8
        };

public: // Constructors and destructor
    /**
     *
     *
     * @since 2.6
     */
    static CAknFepUIManagerJapanese* NewL(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan, TLanguage aLanguage);

    /**
     *
     *
     * @since 2.6
     */
    virtual ~CAknFepUIManagerJapanese();

public: // Functions from base classes
    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    void SetCase(TCase aCase);

    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength, TEventCode aEventCode = EEventKey);

    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    void CloseUI();

    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    void GetFormatOfFepInlineText(TCharFormat& aFormat, TInt& aNumberOfCharactersWithSameFormat, TInt aPositionOfCharacter) const;

    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    TBool IsValidShiftKeyPress() const;

    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    TUIState ChangeState(TUIState aState);

    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    CDesCArrayFlat* CandidateArray();

    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    TBool IsPredictive() const;

    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    void SetInlineEditUnderlineVisibility(TBool aFlag);

    /**
     * From MAknFepManagerInterface
     *
     * @since 2.8
     */
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT 
    void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode, TInt aKeyboardType);
#else
		void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode);
#endif 
#else    
    void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode);
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__	
    /**
     * From MAknFepUIManagerStateInterface
     *
     * @since 3.0
     */
    TBool IsQwerty() const;

    /**
     * From MAknFepManagerInterface
     * this is in base class only
     * @since 3.2
     */
    void HandleCommandL(TInt aCommandId);

    /**
     * From MAknFepUIManagerStateInterface
     *
     * @since 3.2
     */
    void ResourceChanged(TInt aType);

private: // Functions from base classes
    /**
     * From CAknFepUIManagerBase
     *
     * @since 2.6
     */
    TAknFepInputStateBase* FepUIState();

public: // New Functions
    /**
     *
     *
     * @since 2.6
     */
    MAknFepUICtrlContainerJapanese* UIContainer() const;

private: // New Functions
    /**
     *
     *
     * @since 2.6
     */
    CAknFepUIManagerJapanese(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan);

    /**
     *
     *
     * @since 2.6
     */
    void ConstructL(TLanguage aLanguage);

private: // Data

    TAknFepUiInputStateJapaneseBase iFepUiState;

    MAknFepUICtrlContainerJapanese* iContainerPane;

    CDesCArrayFlat* iCandidateArray; //owned

    TBool iFlagInlineEditUnderline; // underline flag in Inline edit

    TBool iQwerty;
    };

#endif //__AKN_FEP_UI_MANAGER_JAPANESE_H__

// End of file
