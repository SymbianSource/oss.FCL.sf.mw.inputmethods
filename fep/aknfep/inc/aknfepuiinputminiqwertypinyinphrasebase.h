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
*      Provides the TAknFepInputMiniQwertyPinyinPhraseBase definition.
*
*/












#ifndef T_AKNFEPUIINPUTMINIQWERTYPINYINPHRASEBASE_H
#define T_AKNFEPUIINPUTMINIQWERTYPINYINPHRASEBASE_H

//#include "AknFepUiInputStateChineseBase.h"
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"

class TAknFepInputMiniQwertyPinyinPhraseBase : public TAknFepInputStateCandidateQwertyBaseChinesePhrase
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
    TAknFepInputMiniQwertyPinyinPhraseBase(
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
     * get the spell of least delimiter
     *
     * @since S60 v3.2.3
     * @return the spell of least delimiter
     */
    TPtrC getCurrentLeastDLTSpell();
    /**
     * Update input window
     *
     * @since S60 v3.2.3
     * @param aRefresh, if ETRue need to get the screen position, other need not.      
     * @return None
     */
    void RefreshUI( TBool aRefresh = EFalse );
    
    /**
     * check the first group stroke of keystroke.
     *
     * @since S60 v3.2.3
     * @param None
     * @return ETrue if the first group stroke is valid, others return EFalse.
     */
    TBool CheckFirstGroupStroke();

    /**
     * Add phrase to the UDB.
     *  
     * @since S60 v3.2.3.
     * @param aPhraseAdd that need to be added to the UDB.
     * @return None.
     */ 
    void AddPhraseToDB( const TDesC& aPhraseAdd );    

    
    /**
     * Refresh candidate
     *
     * @since S60 v3.2.3
     * @param None
     * @return none
     */
    void RefreshCandidate();
    
    /**
     * Check key whether need repeat.
     * 
     * @since S60 v3.2.3
     * @param aLength, the key press length.
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
     * @param None
     * @return None.
     */
    void UpdateIndicator();
    
    /**
      * Do action afer commit, change state to predictive or not.
      *
      * @since S60 v3.2.3
      * @param None
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
     * The pinyin tone mark. 
     */
    enum PinyinToneMark
        {
        EToneMark1 = 1,
        EToneMark2 = 2,
        EToneMark3 = 3,
        EToneMark4 = 4,
        EToneMark5 = 5,
        EOthers = -1
        };
    
    /**
     * Get the cursor's pos of show keystroke.
     *
     * @since S60 v3.2.3
     * @param None
     * @return cursor's pos of show keystroke.
     */
    TInt GetCursorPos();
    
    /**
     * Handle Enter key press.
     *
     * @since S60 v3.2.3
     * @param None
     * @return None
     */
    void HandlEnterKeyL();
    
    /**
     * Handle space key press.
     *
     * @since S60 v3.2.3
     * @param None
     * @return None
     */
    void HandlSpaceKeyL();
    
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
     * Handle tone mark key press.
     *
     * @since S60 v3.2.3
     * @param None
     * @return None
     */
    void HandlToneMarkL();
    
    /**
     * Check the delimiter that wether is manual delimiter.
     *
     * @since S60 v3.2.3
     * @param aCount the count of keystroke except delimiter.
     * @param aKeystroke that put to the Pti keystroke. 
     * @return ETrue if the deimiter is manual delimiter, EFalse others.
     */
    TBool CheckManualDelimiter( const TInt aCount,
                                const TDesC& aKeystroke );
    
    /**
     * Analyse Pinyin spelling.
     * 
     * @since S60 v3.2.3
     * @param aKeystroke that put to the Pti keystroke.
     * @param aSpelling get the spelling from Pti.
     * @param the show keystroke array. 
     * @return None.
     */
    void AnalyseSpellingL( const TDesC& aKeystroke, 
                           const TDesC& aSpelling,
                           CDesCArrayFlat* aShowKeystroke );
    
    /**
     * Analyse spelling after user press tone mark.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return None.
     */
    void AnalyseSpellingAddToneMarkL( 
                                    TInt aI,
                                    TInt aToneMarkCount,
                                    CDesCArrayFlat* aKeystroke,
                                    CDesCArrayFlat* showKeystroke );
    
    /**
     * Analyse spelling after user press key that is not tone mark.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return None.
     */
    void AnalyseSpellingAddKeyL( 
                               TInt& aI,
                               TInt aDelimterPos,
                               CDesCArrayFlat* aKeystroke,
                               CDesCArrayFlat* aShowKeystroke );
    
    /**
     * Analyse spelling after user press key.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return None.
     */
    void AnalyseL();
    
    /**
     * Handle CBA1 or select event.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return None.
     */
    void HandleCommitL();
    
    /**
     * Handle KeyBackspace event.
     * 
     * @since S60 v3.2.3
     * @param aLength Key press length 
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void HandleKeyBackspaceL( TKeyPressLength aLength );
    
    /**
     * Handle default key press event.
     *
     * @since S60 v3.2.3
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void HandleDefaultKeyL( TInt aKey );
    
    /**
     * Show the showinfo on the EEP ctrl.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return None.
     */
    void ShowInfoOnEEPPane();
    
    /**
     * Show the showinfo on the candidate ctrl.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return None.
     */
    void ShowInfoOnCandidatePane();
    
    /**
     * get the show keystroke.
     * 
     * @since S60 v3.2.3.
     * @param aKey, the input keystroke.
     * @param aKeystroke the show keystroke.
     * @return None.
     */
    void GetShowKeystroke( TInt aKey, TDes& aKeystroke );
    
    /**
     * Handle horizontal navigation.
     * 
     * @since S60 v3.2.3.
     * @param aKey the input keystroke.
     * @return ETrue if high light can move, EFase others.
     */ 
    TBool HandleHorizontalNavigation( TInt aKey );
    
    /**
     * Add keystroke.
     * 
     * @since S60 v3.2.3.
     * @param aKey the input keystroke.
     * @return ETrue if that the key can add to the current keystroke, EFase others.
     */            
    TBool AddKeystrokeL( TInt aKey );
    
    /**
     * Add tone mark.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return ETrue if that the key can add to the current keystroke, EFase others.
     */            
    TBool AddToneMarkL();
    
    /**
     * Get the candidate info.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return none.
     */            
    void GetCandidateL();
    
    /**
     * Change CBA.
     *
     * @since S60 v3.2.3
     * @param None.
     * @return None.
     */        
    void ChangeCbaL();
    
    /**
     * Delete more keystroke than max leng.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return None.
     */            
    void DeleteMoreKeystrokeL();
    
    /**
     * Delete current keystroke.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return ETrue if that can be deleted, EFase others.
     */            
    TBool DeleteKeystrokeL();
    
    /**
     * Clear Keystroke of PtiEngine.
     *
     * @since S60 v3.2.3.
     * @param None.
     * @return None.
     */
    void ClearPtiEngineKeystroke();

    /**
     * get the key code.
     * 
     * @since S60 v3.2.3
     * @param aKey, the keycode.
     * @param aKeystroke, the show keystroke.
     * @return none.
     */
    void GetKeystrokeCode( TInt& aKey, const TDesC& aKeystroke );
    
    /**
     * Revert the phrase to keystroke.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return none.
     */
    void RevertPhraseToKeystrokeL();
    
    /**
     * set warning color.
     * 
     * @since S60 v3.2.3.
     * @param aCount, the count of key.
     * @param aIndex, the index of key.
     * @param aPhraseCount, the count of phrase.
     * @param aToneMark, the tone mark.
     * @param aValid, the key whether is valid.
     * @param aShowKeystroke, the show key stroke array.
     * 
     * @return none.
     */         
    void SetWarningColor(
                        TInt aCount,
                        TInt aIndex,
                        TInt aI,
                        TInt aPhraseCount,
                        TInt aToneMark,
                        TBool aValid,
                        CDesCArrayFlat* aShowKeystroke );
    
    /**
     * set warning color.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return none.
     */         
    void SetWarningColor();
    
    /**
     * Move the cursor to Left.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return none.
     */         
    void MoveCursorLeft();
    
    /**
     * Move the cursor to Right.
     * 
     * @since S60 v3.2.3.
     * @param None.
     * @return none.
     */         
    void MoveCursorRight();

    /**
     * Check the special keystroke( i,u,v ).
     * 
     * @since S60 v3.2.3.
     * @param aKeystroke the key code.
     * @return ETrue if the param is special key, EFalse others.
     */         
    TBool CheckSpecialKey( const TInt aKeystroke );
    
    /**
     * Check the delimiter of spelling.
     * 
     * @since S60 v3.2.3.
     * @param aKeystroke the keystroke.
     * @return ETrue if the param is delimiter, EFalse others.
     */         
    TBool CheckSpellingDLT( const TDesC& aKeystroke );
    
    /**
     * Get pinyin tone mark.
     * 
     * @since S60 v3.2.3.
     * @param aKeystroke,The tone mark.
     * @return pinyin tone mark.
     */         
    TInt ToneMark( const TDesC& aKeystroke );
    
    /**
     * Check the spelling after add tone mark to the PTI.
     * 
     * @since S60 v3.2.3.
     * @param aToneMark,The tone mark.
     * @return ETue,if the splling is not phrase spelling,EFalse others.
     */         
    TBool CheckSpellingAddToneMarkToPTIL( const TInt aToneMark );

    /**
     * Check the spelling after add key to the PTI.
     * 
     * @since S60 v3.2.3.
     * @param aToneMark,The tone mark.
     * @return o,if the splling is not phrase spelling,other the position of delimiter.
     */         
    TInt CheckSpellingAddKeyToPTIL( const TDesC& aKeystroke );
    
    /**
     * check the all group stroke of keystroke.
     *
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue if the first group stroke is valid, others return EFalse.
     */
    TBool CheckAllGroupStroke();
    
    /**
      * Do action afer commit, change state to predictive or not.
      *
      * @since S60 v3.2.3
      * @param None.
      * @return None.
      */
    void DoActionAfterCommitL();
    
    };		

#endif //T_AKNFEPUIINPUTMINIQWERTYPINYINPHRASEBASE_H

// End of file
