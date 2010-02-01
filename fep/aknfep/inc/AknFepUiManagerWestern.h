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
*       Provides the CAknFepUIManagerWestern definition.
*
*/












#ifndef __AKN_FEP_UI_MANAGER_WESTERN_H__
#define __AKN_FEP_UI_MANAGER_WESTERN_H__

// INCLUDES
#include <uikon.hrh>                        //TAknEditorNumericKeymap

#include "AknFepUiManagerBase.h"
#include "AknFepUiInputStateBase.h"

// CONSTANTS
#ifndef RD_INTELLIGENT_TEXT_INPUT
#define KSLATEGRAY TRgb(112,138,144); // slate gray color used in AutoCompletion mode
#else	// RD_PRED_AW_COMPLETION
#define KSLATEGRAY TRgb(112,138,144); // slate gray color used in AutoCompletion mode
#endif // RD_PRED_AW_COMPLETION

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepManagerInterface;
class MAknFepManagerUIInterface;
class MAknFepUIManagerStateInterface;
class CAknFepThaiSCTSelector;
class CAknFepVietnameseToneManager;

/**
 * CAknFepUIManagerWestern class.
 * This class is mounted based on CAknFepUiManager.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class CAknFepUIManagerWestern : public CAknFepUIManagerBase
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
    static CAknFepUIManagerWestern* NewL(MAknFepManagerUIInterface* aFepMan, 
                                         CAknFepCaseManager* aCaseMan, TLanguage aLanguage);

    /**
     * 
     * 
     * @since 2.6
     */
    virtual ~CAknFepUIManagerWestern();

public: // Functions from base classes
    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    TBool HandleKeyL( TInt aKey, TKeyPressLength aLength, TEventCode aEventCode = EEventKey );

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
    void AddTextToUserDictionaryL(const TDesC& aText);

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    void GetFormatOfFepInlineText(TCharFormat& aFormat, TInt& aNumberOfCharactersWithSameFormat, 
                                  TInt aPositionOfCharacter) const;

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    TInt SupportLanguage(TInt aMode) const;

    /**
     * Activates western predictive entry UI to inline editing state 
     * by changing input state to entry state.
     * 
     * @since 2.6
     */
    void ActivateUI();

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
    TInt ThaiSCTResourceId(TUint aPrewChar, TInt aKey);

     /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    CAknFepVietnameseToneManager* VietnameseToneManager() const; 

private: // New Functions
    /**
     * 
     * 
     * @since 2.6
     */
    void ConstructL(TLanguage aLanguage);

    /**
     * 
     * 
     * @since 2.6
     */
    CAknFepUIManagerWestern(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan);

    /**
     * 
     * 
     * @since 2.6
     */
    TAknFepInputStateBase* FepUIState();

private: // Data

    TAknFepInputStateBase iFepUiState;
    CAknFepThaiSCTSelector* iAknFepThaiSCTSelector;
    CAknFepVietnameseToneManager* iVietToneMarkMgr; // own, tone mark mgr for vietnamese
    };

#endif //__AKN_FEP_UI_MANAGER_WESTERN_H__

// End of file
