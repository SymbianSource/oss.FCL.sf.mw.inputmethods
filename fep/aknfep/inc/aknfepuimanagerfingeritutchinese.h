/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the CAknFepUIManagerFingerItutChinese definition.
*
*/












#ifndef __AKN_FEP_UI_MANAGER_FINGERITUT_CHINESE_H__
#define __AKN_FEP_UI_MANAGER_FINGERITUT_CHINESE_H__

// INCLUDES
#include <PtiDefs.h>

#include "AknFepUIManagerStateInterface.h"
#include "AknFepUiInputStateChineseBase.h"

// FORWARD DECLARATION
class CAknFepUIManagerBase;
class CAknFepPluginManager;

/**
 *  CAknFepUIManagerFingerItutChinese ui class
 *
 *  This class is Chinese finger ITUT ui manager. 
 *  It manage the Chinese finger ITUT state class.
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class CAknFepUIManagerFingerItutChinese : public CAknFepUIManagerBase
    {
    
public: 

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aFepMan Pointer to fep manager
     * @param aCaseMan Pointer to case manager
     * @param aLanguage Current language
     * @param aPluginMager Pointer to plugin manager
     * @return Pointer to created CAknFepUIManagerFingerItutChinese object
     */  
     static CAknFepUIManagerFingerItutChinese* NewL(MAknFepManagerUIInterface* aFepMan, 
         CAknFepCaseManager* aCaseMan, TLanguage aLanguage, CAknFepPluginManager* aPluginMager);
         
    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepUIManagerFingerItutChinese();

public:

// From CAknFepUIManagerBase

    /**
     * From CAknFepUIManagerBase
     * Handle key event  
     *
     * @since S60 v3.2
     * @param aKey Event key
     * @param aLength The length of key press
     * @return ETrue for handling by state machine; otherwise EFalse
     */ 
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength,TEventCode aEventCode = EEventKey);       

    /**
     * From CAknFepUIManagerBase
     * Close current Finger ITUT ui  
     *
     * @since S60 v3.2
     * @return none
     */ 
    void CloseUI();

    /**
     * From CAknFepUIManagerBase
     * Input mode is supported by this ui  
     *
     * @since S60 v3.2
     * @param aMode Input mode
     * @return error code
     */ 
    TInt SupportLanguage(TInt aMode) const;

    /**
     * From CAknFepUIManagerBase
     * Set current input language  
     *
     * @since S60 v3.2
     * @param aLanguage Current language
     * @return none
     */ 
    void SetInputLanguageL(TLanguage aLanguage);

    /**
     * From CAknFepUIManagerBase
     * Change state machine  
     *
     * @since S60 v3.2
     * @param aState ui state
     * @return current ui state
     */ 
    TUIState ChangeState(TUIState aState);

    /**
     * From CAknFepUIManagerBase
     * Change ITUT state machine  
     *
     * @since S60 v3.2
     * @param aState ui state
     * @return none
     */ 
    void ChangeStatetItut(TUIState aState);
    
private: 

    /**
     * From CAknFepUIManagerBase
     * Get current fep ui state machine
     *
     * @since S60 v3.2
     * @return pointer to TAknFepInputStateBase
     */ 
    TAknFepInputStateBase* FepUIState();

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aFepMan Pointer to fep manager
     * @param aCaseMan Pointer to case manager
     * @param aPluginMager Pointer to plugin manager
     * @return None
     */  
    CAknFepUIManagerFingerItutChinese(MAknFepManagerUIInterface* aFepMan, 
        CAknFepCaseManager* aCaseMan, CAknFepPluginManager* aPluginMager);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aLanguage Current language
     * @return None
     */ 
    void ConstructL(TLanguage aLanguage);

private: // Data

    /**
     * Current state machine
     */
    TAknFepInputStateBase iFepUiState;//TAknFepInputStateChineseBase iFepUiState;

    /**
     * Current flag
     */
	TUint iFlags;

    /**
     * Previous mode
     */
    TInt iPrevMode;

    /**
     * Fep plugin manager (Not own)
     */
    CAknFepPluginManager* iPluginMager;

    };
    
#endif //__AKN_FEP_UI_MANAGER_FINGERITUT_CHINESE_H__

// End of file
