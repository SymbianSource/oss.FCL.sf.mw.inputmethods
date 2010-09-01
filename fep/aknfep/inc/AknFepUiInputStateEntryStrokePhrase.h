/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            header file of Stroke phrase entry state
*
*/












#ifndef T_AKNFEPUIINPUTSTATEENTRYSTROKEPHRASE_H
#define T_AKNFEPUIINPUTSTATEENTRYSTROKEPHRASE_H

// User includes 
#include "aknfepuiinputstatestrokephrasebase.h"

/**
 *  TAknFepInputStateEntryStrokePhrase class.
 *
 *  Implementation of Stroke phrase entry state
 *
 *  @lib avkonfep.dll
 *  @since S60 v5.0
 */
class TAknFepInputStateEntryStrokePhrase : 
    public TAknFepInputStateStrokePhraseBase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V5.0
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEntryStrokePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer );
	 /**
    * Handling Command events.
    * This Method is called by CAknFepUIManagerJapanese.
    *
    * @param aCommandId Command ID value
    */
    void HandleCommandL(TInt aCommandId);                                   

    /**
     * From TAknFepInputStateEntryZhuyinStrokeBasePhrase
     * Handle system key press event
     *
     * @since S60 V5.0
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
private:   
    /**
     * Commit phrase to the EEP ctrl.
     *
     * @since S60 v5.0
     * @param aText that need to commint to the EEP phrase.     
     * @return ETrue commit to the EEP phrase successful, EFalse other.
     */
    TBool CommitInlineEEPL( const TDesC& aText );
    /**
     * Handle CBA1,ok and select candidate phrase key press.
     *
     * @since S60 v5.0
     * @return None.
     */
    void HandleCommitL();
    };

/**
 *  TAknFepInputStateEditStrokePhrase class.
 *
 *  Implementation of Stroke phrase creation edit state
 *
 *  @lib avkonfep.dll
 *  @since S60 v5.0
 */
class TAknFepInputStateEditStrokePhrase : public TAknFepInputStateStrokePhraseBase
    {
public:

    /**
     * C++ construct.
     *
     * @since S60 v5.0.
     * @return self object.
     */ 
    TAknFepInputStateEditStrokePhrase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    
    /**
     * HandleKeyL.
     *
     * @since S60 v5.0.
     * @return ETrue.
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    };    


/**
 *  TAknFepInputStateEntryStrokePhraseCreation class.
 *
 *  Implementation of Stroke phrase creation candidate state
 *
 *  @lib avkonfep.dll
 *  @since S60 v5.0
 */
class TAknFepInputStateCandidateStrokePhrase : 
    public TAknFepInputStateStrokePhraseBase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 v5.0
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateCandidateStrokePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);

    /**
     * From TAknFepInputStateEntryZhuyinStrokeBasePhrase
     * Handle system key press event
     *
     * @since S60 v5.0
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );   
    
private:
    
    /**
     * Handle Horizontal Navigation.
     *
     * @since S60 v5.0
     * @param aKey,key press.     
     * @return None
     */
    void HandleHorizontalNavigation(TInt aKey);
    
    /**
     * Commit phrase to the EEP ctrl.
     *
     * @since S60 v5.0
     * @param aText that need to commint to the EEP phrase.     
     * @return ETrue commit to the EEP phrase successful, EFalse other.
     */
    TBool CommitInlineEEPL( const TDesC& aText );
    
    /**
     * Handle CBA1,ok and select candidate phrase key press.
     *
     * @since S60 v5.0
     * @return None.
     */
    void HandleCommitL();
    
    inline TInt MapKeyToIndex(TInt aKey);
    
    };

inline TInt TAknFepInputStateCandidateStrokePhrase::MapKeyToIndex( TInt aKey )
    {
    TInt index = aKey - EPtiKey0; // get zero-based value first
    return !index ? 9 : --index; // map it for one-based keys...
    }    
    
#endif //T_AKNFEPUIINPUTSTATEENTRYSTROKEPHRASE_H

// End of file
