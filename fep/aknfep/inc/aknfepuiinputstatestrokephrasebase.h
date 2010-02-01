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
*      Provides the TAknFepInputStateStrokePhraseBase definition.
*
*/












#ifndef T_AKNFEPUIINPUTSTATESTROKEPHRASEBASE_H
#define T_AKNFEPUIINPUTSTATESTROKEPHRASEBASE_H

#include "AknFepUiInputStateChineseBase.h"

/**
 * key struct.
 * @since S60 v5.0
 */

struct TKeyMap
    {
    TUint16 iValue;
    TInt iKeyCode;
    };

class TAknFepInputStateStrokePhraseBase : public TAknFepInputStateChineseBase
    {
public:
    
    /**
     * C++ default constructor
     *
     * @since S60 v5.0
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateStrokePhraseBase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer
        );
    
    /**
     * Handle system key press event
     *
     * @since S60 v5.0
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL( TInt aKey, TKeyPressLength aLength );
    
    /**
     * Update input window
     *
     * @since S60 v5.0
     * @param aRefresh, if ETRue need to get the screen position, other need not.      
     * @return None
     */
    void RefreshUI( TBool aRefresh = EFalse );
    
    /**
     * Update popup window
     *
     * @since S60 v5.0
     * @return ETrue if the first group stroke is valid, others return EFalse.
     */
    TBool CheckFirstGroupStroke();

    /**
     * Add phrase to the UDB.
     * 
     * @param aPhraseAdd that need to be added to the UDB.
     * @since S60 v5.0.
     * @return None.
     */ 
    void AddPhraseToDB( const TDesC& aPhraseAdd );    

    
    /**
     * Refresh candidate
     *
     * @since S60 V5.0
     * @return none
     */
    void RefreshCandidate();
    
    /**
     * Check key whether need repeat.
     * 
     * @since S60 v5.0
     * @return ETrue if the key need repeat, EFalse others.
     */
    TBool CheckKeyNeedRepeat( TKeyPressLength aLength );
    
    /**
     * Get new candidate.
     * 
     * @since S60 v5.0
     * @return None.
     */
    void GetNewCandidatesL(const TDesC& aSource, RPointerArray<HBufC>& aList);
private:    
    
    /**
     * Handle KeyBackspace event.
     * 
     * @param aLength Key press length
     * @since S60 v5.0
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void HandleKeyBackspaceL( TKeyPressLength aLength );
    
    /**
     * Entry state handle CBA1 or OK key press event.
     *
     * @since S60 v5.0
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void HandleKeyCBA1OrKeyOKEntry( TKeyPressLength aLength );
    
    /**
     * Edit state handle CBA1 or OK key press event.
     *
     * @since S60 v5.0
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void HandleKeyCBA1OrKeyOKEditL();
    
    /**
     * Handle CBA1 or OK key press event.
     *
     * @since S60 v5.0
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void HandleKeyCBA1OrKeyOKL( TKeyPressLength aLength );
    
    /**
     * Handle default key press event.
     *
     * @since S60 v5.0
     * @param aKey System key
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    void HandleDefaultKeyL( TInt aKey );
    
    /**
     * Show the showinfo on the EEP ctrl.
     * 
     * @since S60 v5.0.
     * @return None.
     */
    void ShowInfoOnEEPPane();
    
    /**
     * Show the showinfo on the candidate ctrl.
     * 
     * @since S60 v5.0.
     * @return None.
     */
    void ShowInfoOnCandidatePane();
    
    /**
     * get the show keystroke.
     * 
     * @param aKey, the input keystroke.
     * @param aKeystroke the show keystroke.
     * @since S60 v5.0.
     * @return None.
     */
    void GetShowKeystroke( TInt aKey, TDes& aKeystroke );
    
    /**
     * Handle horizontal navigation.
     * 
     * @param aKey the input keystroke.
     * @since S60 v5.0.
     * @return ETrue if high light can move, EFase others.
     */ 
    TBool HandleHorizontalNavigation( TInt aKey );
    
    /**
     * Add keystroke.
     * 
     * @param aKey the input keystroke.
     * @since S60 v5.0.
     * @return ETrue if that the key can add to the current keystroke, EFase others.
     */            
    TBool AddKeystrokeL( TInt aKey );
    
    /**
     * Get the candidate info.
     * 
     * @since S60 v5.0.
     * @return none.
     */            
    void GetCandidateL();
    
    /**
     * Change CBA.
     *
     * @since S60 v5.0
     * @return None.
     */        
    void ChangeCbaL();
    
    /**
     * Update indicator
     *
     * @since S60 v5.0
     * @return None.
     */
    void UpdateIndicator();
    
    /**
     * Delete current keystroke.
     * 
     * @since S60 v5.0.
     * @return ETrue if that can be deleted, EFase others.
     */            
    TBool DeleteKeystroke();
    
    /**
     * Clear Keystroke of PtiEngine.
     *
     * @since S60 v5.0
     * @return None.
     */
    void ClearPtiEngineKeystroke();

    /**
     * get the key code.
     * 
     * @param aKey, the keycode.
     * @param aKeystroke, the show keystroke.
     * @since S60 v5.0
     * @return none.
     */
    void GetKeystrokeCode( TInt& aKey, const TDesC& aKeystroke );
    
    /**
     * Check all the keystroke.
     * 
     * @since S60 v5.0.
     * @return True if All the keystroke is valid.
     */            
    TBool CheckAllKeystroke();
    
    /**
     * Revert the phrase to keystroke.
     * @since S60 v5.0
     * @return none.
     */
    void RevertPhraseToKeystrokeL();
    
    /**
     * set warning color.
     * 
     * @since S60 v5.0.
     * @return none.
     */         
    void SetWarningColor();
    
    /**
     * Move the cursor to Left.
     * 
     * @since S60 v5.0.
     * @return none.
     */         
    void MoveCursorLeft();
    
    /**
     * Move the cursor to Right.
     * 
     * @since S60 v5.0.
     * @return none.
     */         
    void MoveCursorRight();

    };		

#endif //T_AKNFEPUIINPUTSTATESTROKEPHRASEBASE_H

// End of file
