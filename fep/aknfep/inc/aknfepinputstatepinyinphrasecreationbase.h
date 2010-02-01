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
 *      Provides the TAknFepInputStateEntryPinyinPhraseCreation methods.
 *
*/












#ifndef T_AKNFEPINPUTSTATEPINYINPHRASECREATIONBASE_H
#define T_AKNFEPINPUTSTATEPINYINPHRASECREATIONBASE_H

#include <PtiDefs.h>                            //keys

#include "AknFepUiInputStateCandidateBase.h"

enum TMkeyReturnValue
    {
    EKeyStrokeNull = -2, //  neans the keystroke is null  
    EKeyDelimiter,   // means the input key is delimiter
    EkeyNumber       // means the input key is the number( from 2 to 9 )
    };

class TAknFepInputStatePinyinPhraseCreationBase : public TAknFepInputStateCandidateBase
    {
public:
    
    /**
     * C++ constructor.
     * 
     * @since S60 v5.0.
     * @return self object.
     */  
    TAknFepInputStatePinyinPhraseCreationBase(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerChinese* aUIContainer);
    
    /**
     * Add keystroke.
     * 
     * @param aKey the input keystroke.
     * @since S60 v5.0.
     * @return ETrue if that the key can add to the current keystroke, EFase others.
     */            
    TBool AddKeystroke( TInt aKey );
    
    /**
     * Add tone mark.
     * 
     * @since S60 v5.0.
     * @return ETrue if that the key can add to the current keystroke, EFase others.
     */            
    TBool AddToneMark(); 
    
    /**
     * get the valid keystroke.
     * 
     * @since S60 v5.0.
     * @return ETrue if valid keystroke happen change , EFase others.
     */            
    TBool GetValidKeystroke();
    
    /**
     * Delete current keystroke.
     * 
     * @since S60 v5.0.
     * @return ETrue if that can be deleted, EFase others.
     */            
    TBool DeleteCurrentKeystroke();
    
    /**
     * Revert the current chinse to keystroke.
     * 
     * @since S60 v5.0.
     * @return ETrue if that can be deleted, EFase others.
     */            
    TBool ChineseRevertKeystroke();
    
    /**
     * show info to the eep ctrl.
     * 
     * @since S60 v5.0.
     * @return none.
     */            
    void ShowInfo();
    
    /**
     * set the show info warning color.
     * 
     * @since S60 v5.0.
     * @return none.
     */            
    void SetWarningColor();
    
    /**
     * Get the candidate info.
     * 
     * @since S60 v5.0.
     * @return none.
     */            
    void GetCandidate( TInt aSelection = 0 );
    
    /**
     * Change CBA.
     *
     * @since S60 v5.0
     * @return None.
     */        
    void ChangeCba();
  
    /**
     * Refresh the candidate info.
     * 
     * @since S60 v5.0.
     * @return none.
     */            
    void RefreshCandidate();
    
    /**
     * Update Selection
     *
     * @since S60 v5.0
     * @param aSelection the index of selecion.
     * @return None.
     */ 
    void RefreshUI(TInt aSelection = 0);
    
    /**
     * clear Selection
     *
     * @since S60 v5.0
     * @return None.
     */
    void ClearDeliberateSelection();
    
    /**
     * Update indicator
     *
     * @since S60 v5.0
     * @return None.
     */
    void UpdateIndicator();
    
private:
    
    /**
     * clear the keystroke of PtiEngine.
     * 
     * @since S60 v5.0.
     * @return none.
     */
    void ClearPtiEngineKeystroke();
    
    /**
     * Add keystroke.
     * 
     * @param aKey the input keystroke.
     * @since S60 v5.0.
     * @return ETrue if that the key can add to the current keystroke, EFase others.
     */            
    TBool AddKeystrokeL( TInt aKey );  
    
    /**
     * Add tone mark.
     * 
     * @since S60 v5.0.
     * @return ETrue if that the key can add to the current keystroke, EFase others.
     */            
    TBool AddToneMarkL(); 
    
    /**
     * check the keystroke.
     * 
     * @param aInput the input keystroke.
     * @since S60 v5.0.
     * @return ETrue if the input param is tone mark, EFalse others.
     */            
    TBool IsToneMark( const TDesC& aInput );
    
    /**
     * check the keystroke.
     * 
     * @since S60 v5.0.
     * @return EFalse if that have invalid keystroke, ETrue others.
     */            
    TBool CheckKeystroke();
    
    /**
     * get the valid keystroke.
     * 
     * @since S60 v5.0.
     * @return ETrue if that have valid keystroke than happens change, EFase others.
     */            
    TBool GetValidKeystrokeL();
    
    /**
     * get the key code.
     * 
     * @param aKey the TPtiKey.
     * @param aKeystroke the input show key.
     * 
     * @since S60 v5.0.
     * @return -2 the aKeystroke is null,-1 if the input key is delimiter, 
     *  0 the input key is 2 - 9, others return tome mark.
     */
    TInt GetKeystrokeCode( TPtiKey& aKey, 
                           const TDesC& aKeystroke );
    
    /**
     * Append the key to the ptiengine.
     * 
     * @param aKey the TPtiKey.
     * @param aKey the add the spelling to the spelling array flag.
     * @since S60 v5.0.
     * @return ETrue if ptiengine can append keypress, others return EFalse.
     */
    TBool AppendKeyPressL( TPtiKey& aKey, TBool aAddSpellingFlag );
    
    
    /**
     * Append the tone mark to the ptiengine.
     * 
     * @param aToneMark the tone mark.
     * 
     * @since S60 v5.0.
     * @return ETrue if ptiengine can append tone mark, others return EFalse.
     */
    TBool AppendToneMarkL( TInt& aToneMark );
    
    /**
     * Revert the current chinse to keystroke.
     * 
     * @since S60 v5.0.
     * @return ETrue if that can be deleted, EFase others.
     */            
    TBool ChineseRevertKeystrokeL();
    
    /**
     * check the spelling delete the phrase spelling.
     * 
     * @since S60 v5.0.
     * @return none.
     */
    void CheckSpellingL( CDesCArrayFlat* aTempSpelling, CDesCArrayFlat* aAfterCheck );
    
    /**
     * check the tone mark and delete the phrase spelling.
     * 
     * @param aToneMark the tone mark.
     * @since S60 v5.0.
     * @return ETrue if the spelling is valid, EFalse others.
     */
    TBool CheckToneMark( TInt& aToneMark );
    
    /**
     * Get the candidate info.
     * 
     * @since S60 v5.0.
     * @return none.
     */            
    void GetCandidateL( TInt aSelection );
    
    /**
     * Change CBA.
     *
     * @since S60 v5.0
     * @return None.
     */        
    void ChangeCbaL();
    
    };

#endif /*T_AKNFEPINPUTSTATEPINYINPHRASECREATIONBASE_H*/

// End of file

