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
* Description:            Declaration of core part of CAknFepLanguageManager class.
*
*/












#ifndef __AKN_FEP_LANGUAGE_MANAGER__H__
#define __AKN_FEP_LANGUAGE_MANAGER__H__

// INCLUDES
#include <e32base.h>
#include <coemain.h>
#include <badesca.h>

#include "AknFepGlobalEnums.h" // TWidthChar

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATION
class CAknFepPenInputImePlugin;
class MInputMethodInterface;

// CLASS DECLARATION
class MAknFepManagerInterface;
class MAknFepManagerUIInterface;
class CAknFepCaseManager;
class RPeninputServer;
class TImePlguinImplDetail;
class CAknFepPluginManager;
class CAknFepSharedDataInterface;

/**
 * CAknFepLanguageManager is a part of Japanese FEP.
 *
 * @lib AknFep.lib
 * @since 2.6
 * @see CAknFepManager, other application
 */
class CAknFepLanguageManager : public CBase
    {
private: // enum
    enum
        {
        EUiManWestern,
        EUiManChinese,
        EUiManJapanese,
        EUiManKorean,
        EUiManMax
        };

public: // Constructer and Destructor
    /**
     * first phase construction
     *
     * @since 2.6
     * @param aFepMan
     * @param aCaseMan
     * @param aLanguage
     */
    static CAknFepLanguageManager* NewL( MAknFepManagerUIInterface* aFepMan, 
                                         CAknFepCaseManager* aCaseMan,
                                         CAknFepSharedDataInterface* aSharedData = NULL );

    /**
     * Destructor.
     *
     * @since 2.6
     */
    virtual ~CAknFepLanguageManager();

public: // New Functions
    /**
     * get ui manager object.
     *
     * @since 2.6
     * @param aMode
     * @param aCharacterWidth
     * @param aPredictive
     * @return 
     */
    MAknFepManagerInterface* GetFepUI(TInt aMode, TWidthChar aCharacterWidth, TBool aPredictive);

    /**
     * set language id to ui manager.
     *
     * @since 2.6
     * @param aMode
     * @param aCharacterWidth
     * @param aPredictive
     * @return 
     */
    void SetInputLanguageL(TInt aInputLanguage);

    TLanguage InputLanguage() 
        {
        return (TLanguage)iInputLanguage;
        }

    /**
     * Get plugin UI manager object.
     *
     * @since S60 v3.2
     *
     * @param aPreferedMode The prefered plugin input mode. If prefered mode is not available
     *        a substitute choice (according UI sepc) is returned.
     * @param aPenInputLan Current pen input language.
     * @param aDisplayLang Current phone display language
     * @param aPenServer The pen input server object.
     * @return Plugin UI manager.
     */
    MAknFepManagerInterface* GetPluginInputFepUiL(TPluginInputMode aPreferedMode, 
                                                  TInt aPenInputLang,
                                                  TInt aDisplayLang,
                                                  RPeninputServer* aPenServer);

    /**
     * Test specified plugin UI mode is support or not
     *
     * @since S60 v3.2
     *
     * @param aMode The plugin input mode to test.
     * @param aPenInputLan Current pen input language.
     * @param aPenServer The pen input server object.
     * @return ETrue if support, EFalse otherwise.
     */
    TBool  IsPluginInputAvaiable(TPluginInputMode aMode, 
                                 TInt aPenInputLang,
                                 RPeninputServer* aPenServer);

    /**
     * Set split view flag
     *
     * @since S60 v3.2
     *
     * @param aIsSplitView 
     * @return None
     */	
    void SetSplitView(TBool aIsSplitView);

    /**
     * Get current IME plugin object.
     *
     * @since S60 v3.2
     *
     * @return IME Plugin object.
     */
    inline CAknFepPenInputImePlugin* CurrentImePlugin();
    
    /**
     * Get split view flag
     * 
     * 
     * @since S60 v3.2
     * @return ETrue if split view flag is set
     */
    inline TBool IsSplitView();
    
private: // New Functions
    /**
     * private c++ constructor.
     *
     * @since 2.6
     */
    CAknFepLanguageManager( MAknFepManagerUIInterface* aFepManager, 
                            CAknFepCaseManager* aCaseManager,
                            CAknFepSharedDataInterface* aSharedData = NULL );

    /**
     * Second phase construction
     *
     * @since 2.6
     */
    void ConstructL();
    
    void ConstructUiManagerL();

    TInt UiManagerId();

    

    /**
     * Activate IME plugin by implemenation.
     * If current IME plugin's impl id is equal to aImeImplId, then current
     * IME plugin object is returned.
     *
     * @since S60 v3.2
     *
     * @param aImeImplId The IME implementation ID.
     * @param aPenServer The pen input server object.
     * @return IME Plugin imeplementation ID.
     */
    CAknFepPenInputImePlugin* ActivateImePlugin(TInt aImeImplId, RPeninputServer* aPenServer);

    /**
     * Activate IME plugin's UI by mode.
     *
     * @since S60 v3.2
     *
     * @param aImePlugin The IME plugin object.
     * @param aMode The mode of layout UI.
     * @return The Plugin UI interface.
     */
    MAknFepManagerInterface*  ActivateLayoutUiPluginL(CAknFepPenInputImePlugin* aImePlugin,
                                                      TInt aPenInputLang,
                                                      TPluginInputMode aMode);

    TBool  IsPluginInputAvaiableL(TPluginInputMode aMode, 
                                 TInt aPenInputLang,
                                 RPeninputServer* aPenServer);
    

private: // Data
    CArrayPtr<MAknFepManagerInterface>*     iUiManArray;    
    TInt iInputLanguage;
    MAknFepManagerUIInterface* iFepManager;
    CAknFepCaseManager* iCaseManager;
    CAknFepPluginManager* iPluginManager;
    
    /**
     * Current IME plugin object. Own.
     * @since S60 v3.2
     */
    CAknFepPenInputImePlugin* iCurImePlugin;

    /**
     * Current layout UI object. Not own.
     * @since S60 v3.2
     */
    MAknFepManagerInterface* iCurUiPlugIn;

    /**
     * Is split view flag
     * @since S60 v3.2
     */	
    TBool iIsSplitView;

    /**
     * Shared data manager
     */
    CAknFepSharedDataInterface* iSharedData;

    };

inline CAknFepPenInputImePlugin* CAknFepLanguageManager::CurrentImePlugin()
    {
    return iCurImePlugin;
    }

inline TBool CAknFepLanguageManager::IsSplitView()
    {
    return iIsSplitView;
    }

#endif // __AKN_FEP_LANGUAGE_MANAGER__H__
// End of file
