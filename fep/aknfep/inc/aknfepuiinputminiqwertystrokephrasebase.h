/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*      Provides the TAknFepInputMiniQwertyStrokePhraseBase definition.
*
*/












#ifndef T_AKNFEPUIINPUTMINIQWERTYSTROKEPHRASEBASE_H
#define T_AKNFEPUIINPUTMINIQWERTYSTROKEPHRASEBASE_H

//#include "AknFepUiInputStateChineseBase.h"
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"

/**
 * key struct.
 * @since S60 v3.2.3
 */

class TAknFepInputMiniQwertyStrokePhraseBase : public TAknFepInputStateCandidateQwertyBaseChinesePhrase
    {
public:
    /**
     * change state flag.
     */
    enum TAknFepStrokePhraseChange
        {
        EManualChangeState,
        EAutoChangeStateFromInput,
        EAutoChangeStateFromCandidate
        };
    
    /**
     * C++ default constructor
     *
     * @since S60 v3.2.3
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputMiniQwertyStrokePhraseBase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer
        );
    
    /**
     * Handle system key press event
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL( TInt aKey, TKeyPressLength aLength );
    
    /**
     * Update input window
     *
     * @since S60 v3.2.3
     * @param aRefresh, if ETRue need to get the screen position, other need not.      
     * @return None
     */
    void RefreshUI();
    
    /**
     * check the first group stroke of keystroke.
     *
     * @since S60 v3.2.3
     * @return ETrue if the first group stroke is valid, others return EFalse.
     */
    TBool CheckFirstGroupStroke();

    /**
     * Add phrase to the UDB.
     * 
     * @param aPhraseAdd that need to be added to the UDB.
     * @since S60 v3.2.3.
     * @return None.
     */ 
    void AddPhraseToDB( const TDesC& aPhraseAdd );    

    
    /**
     * Refresh candidate
     *
     * @since S60 v3.2.3
     * @return none
     */
    void RefreshCandidate();
    
    /**
     * Check key whether need repeat.
     * 
     * @since S60 v3.2.3
     * @return ETrue if the key need repeat, EFalse others.
     */
    TBool CheckKeyNeedRepeat( TKeyPressLength aLength );
    
    /**
     * Commint text to EEP or editor
     *
     * @since S60 V3.2.3
     * @param aDes Text had been select from candidate pane
     * @return ETrue if keep state, otherwise need to change to pridictive or editor
     */
    TBool CommitInlineEEPL( const TDesC& aDes );
    
    /**
     * Update indicator
     *
     * @since S60 v3.2.3
     * @return None.
     */
    void UpdateIndicator();
    
    /**
      * Do action afer commit, change state to predictive or not.
      *
      * @since S60 v3.2.3
      * @return None.
      */
    void DoActionAfterCommit();
    
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
private:
    
    /**
     * Handle key from candidate.
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @param aLength Key press length
     * return None
     */
    void HandleKeyFromCandidateL(TInt aKey, TKeyPressLength aLength );
    
    /**
     * Handle CBA1 or select event.
     * 
     * @since S60 v3.2.3
     * @return None.
     */
    void HandleCommitL( TKeyPressLength aLength );
    
    /**
     * Auto change state.
     * 
     * @since S60 v3.2.3
     * @return Etru auto change state,ohters don't change state.
     */
    TBool HandleAutoChangeState();
    
    /**
     * Handle KeyBackspace event.
     * 
     * @param aLength Key press length
     * @since S60 v3.2.3
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void HandleKeyBackspaceL( TKeyPressLength aLength );
    
    /**
     * Handle default event.
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleDefaultL( TInt aKey, TKeyPressLength aLength );
    
    /**
     * Handle default key press event.
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @return none.
     */
    void HandleDefaultKeyL( TInt aKey );
    
    /**
     * Show the showinfo on the EEP ctrl.
     * 
     * @since S60 v3.2.3.
     * @return None.
     */
    void ShowInfoOnEEPPane();
    
    /**
     * Show the showinfo on the candidate ctrl.
     * 
     * @since S60 v3.2.3.
     * @return None.
     */
    void ShowInfoOnCandidatePane();
    
    /**
     * Handle horizontal navigation.
     * 
     * @param aKey the input keystroke.
     * @since S60 v3.2.3.
     * @return ETrue if high light can move, EFase others.
     */ 
    TBool HandleHorizontalNavigation( TInt aKey );
    
    /**
     * Add keystroke.
     * 
     * @param aKey the input keystroke.
     * @since S60 v3.2.3.
     * @return ETrue if that the key can add to the current keystroke, EFase others.
     */            
    TBool AddKeystrokeL( TInt aKey );
    
    /**
     * Get the candidate info.
     * 
     * @since S60 v3.2.3.
     * @return none.
     */            
    void GetCandidateL();
    
    /**
     * Change CBA.
     *
     * @since S60 v3.2.3
     * @return None.
     */        
    void ChangeCbaL();
    
    /**
     * Delete current keystroke.
     * 
     * @since S60 v3.2.3.
     * @return ETrue if that can be deleted, EFase others.
     */            
    TBool DeleteKeystroke();
    
    /**
     * Clear Keystroke of PtiEngine.
     *
     * @since S60 v3.2.3
     * @return None.
     */
    void ClearPtiEngineKeystroke();
    
    /**
     * get the show keystroke.
     * 
     * @param aKey, the input keystroke.
     * @param aKeystroke the show keystroke.
     * @since S60 v3.2.3.
     * @return None.
     */
    void GetShowKeystroke( TInt aKey, TDes& aKeystroke );    
    
    /**
     * Revert the phrase to keystroke.
     * @since S60 v3.2.3
     * @return none.
     */
    void RevertPhraseToKeystrokeL();
    
    /**
     * set warning color.
     * 
     * @since S60 v3.2.3.
     * @return none.
     */         
    void SetWarningColor();
    
    /**
     * Move the cursor to Left.
     * 
     * @since S60 v3.2.3.
     * @return ETrue if cursor can be moved, other EFalse.
     */         
    TBool MoveCursorLeft();
    
    /**
     * Move the cursor to Right.
     * 
     * @since S60 v3.2.3.
     * @return ETrue if cursor can be moved, other EFalse.
     */         
    TBool MoveCursorRight();

    /**
     * check the all group stroke of keystroke.
     *
     * @since S60 v3.2.3
     * @return ETrue if the first group stroke is valid, others return EFalse.
     */
    TBool CheckAllGroupStroke();
    
    /**
      * Do action afer commit, change state to predictive or not.
      *
      * @since S60 v3.2.3
      * @return None.
      */
    void DoActionAfterCommitL();
    
    /**
     * check the delimeter 
     * @ aKeystroke   the key stroke
     * @@since S60 v3.2.3
     * @return ETrue if the key stoke is the delimeter ,otherwise return EFalse .
     */
   TBool CheckSpellingDLT( const TDesC& aKeystroke );

    };		

#endif //T_AKNFEPUIINPUTMINIQWERTYSTROKEPHRASEBASE_H

// End of file
