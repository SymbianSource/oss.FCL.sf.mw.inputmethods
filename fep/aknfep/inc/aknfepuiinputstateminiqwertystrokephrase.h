/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryMiniQwertyStrokePhrase,
*       TAknFepInputStateEditingMiniQwertyStrokePhrase,
*       TAknFepUiInputStateCandidateMiniQwertyStrokePhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_MINIQWERTY_STROKEPHRASECREATION_H__
#define __AKN_FEP_UI_INPUT_STATE_MINIQWERTY_STROKEPHRASECREATION_H__
//#include "aknfepuiinputminiqwertystrokephrasebase.h"
#include "aknfepuiinputminiqwertystrokephrasebase.h"
/**
 *  TAknFepInputStateEntryMiniQwertyStrokePhrase class.
 *
 *  Implementation of Stroke phrase entry state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepInputStateEntryMiniQwertyStrokePhrase: public TAknFepInputMiniQwertyStrokePhraseBase
    {
public: 
    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEntryMiniQwertyStrokePhrase(
            MAknFepUIManagerStateInterface* aOwner,
            MAknFepUICtrlContainerChinese* aUIContainer );
    /**
     * From TAknFepInputStateCandidateBasePhrase
     * Handle system key press
     *
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
     TBool HandleKeyL( TInt aKey, TKeyPressLength aLength );
     
     /**
      * Handling Command events.
      * This Method is called by CAknFepUIManagerJapanese.
      *
      * @param aCommandId Command ID value
      */
     void HandleCommandL( TInt aCommandId );
     
     void SubmitTextL( const TDesC& aText );

    };

/**
 *  TAknFepInputStateEditingMiniQwertyStrokePhrase class.
 *
 *  Implementation of Stroke phrase editing state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepInputStateEditingMiniQwertyStrokePhrase: public TAknFepInputMiniQwertyStrokePhraseBase
    {
public: 
    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEditingMiniQwertyStrokePhrase(
            MAknFepUIManagerStateInterface* aOwner,
            MAknFepUICtrlContainerChinese* aUIContainer );
    /**
     * From TAknFepInputStateCandidateBasePhrase
     * Handle system key press
     *
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL( TInt aKey ,TKeyPressLength alength );
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    
    void SubmitTextL( const TDesC& /*aText*/ );
    };

/**
 *  TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase class.
 *
 *  Implementation of Pinyin and Stroke phrase candidate state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepUiInputStateCandidateMiniQwertyStrokePhrase: public TAknFepInputMiniQwertyStrokePhraseBase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepUiInputStateCandidateMiniQwertyStrokePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    
    void SubmitTextL( const TDesC& aText );

protected:

    /**
     * From TAknFepInputStateCandidateBasePhrase
     * Handle system key press
     *
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    };

#endif /*__AKN_FEP_UI_INPUT_STATE_MINIQWERTY_STROKEPHRASECREATION_H__*/
