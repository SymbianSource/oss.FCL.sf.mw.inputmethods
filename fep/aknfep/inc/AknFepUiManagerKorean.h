/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIManagerKorean definition.
*
*/
#ifndef __AKN_FEP_UI_MANAGER_KOREAN_H__
#define __AKN_FEP_UI_MANAGER_KOREAN_H__

// INCLUDES
#include <uikon.hrh>                        //TAknEditorNumericKeymap

#include "AknFepUiManagerBase.h"
#include "AknFepUiInputStateBase.h"

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepManagerInterface;
class MAknFepManagerUIInterface;
class MAknFepUIManagerStateInterface;
class MAknFepUIManagerStateInterface;


/**
 * CAknFepUIManagerWestern class.
 * This class is mounted based on CAknFepUiManager.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 */
class CAknFepUIManagerKorean : public CAknFepUIManagerBase
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
    static CAknFepUIManagerKorean* NewL(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan, TLanguage aLanguage);

    /**
     * 
     * 
     * @since 2.6
     */
    virtual ~CAknFepUIManagerKorean();

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
    CAknFepUIManagerKorean(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan);
    
   	/**
     * 
     * 
     * @since 2.6
     */

    TAknFepInputStateBase* FepUIState();



private: // Data
                              
    TAknFepInputStateBase iFepUiState;
    
    };

#endif //__AKN_FEP_UI_MANAGER_KOREAN_H__

// End of file
