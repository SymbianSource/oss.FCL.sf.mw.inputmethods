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
 *      Provides the TAknFepInputStateEntryZhuyinPhraseCreation methods.
 *
*/












#ifndef T_AKNFEPINPUTSTATEZHUYINPHRASECREATIONBASE_H
#define T_AKNFEPINPUTSTATEZHUYINPHRASECREATIONBASE_H

#include "AknFepUiInputStateCandidateBase.h"

class TAknFepInputStateZhuyinPhraseCreationBase : public TAknFepInputStateCandidateBase
    {
public:
    
    /**
     * C++ constructor.
     * 
     * @since S60 v5.0.
     * @return self object.
     */  
    TAknFepInputStateZhuyinPhraseCreationBase(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerChinese* aUIContainer);
    
    /**
     * Add keystroke.
     * 
     * @param aKey the input keystroke.
     * @since S60 v5.0.
     * @return ETrue if that the key can add to the current keystroke, EFalse others.
     */            
    TBool AddKeystrokeL( TInt aKey );
    
    /**
     * Add tone mark.
     * 
     * @since S60 v5.0.
     * @return ETrue if that the key can add to the current keystroke, EFalse others.
     */            
    TBool AddToneMarkL(); 
    
    /**
     * Get the candidate info and refresh ui.
     * 
     * @since S60 v5.0.
     * @return ETrue if get candidate sucessful, EFalse others.
     */            
    TBool UpdateEEPContent();
    
    /**
     * Check the key stroke group more than max.
     * 
     * @since S60 v5.0
     * 
     * @return value that more than max begine position.
     */
    TInt BegPosOfMoreThanSeven();
    
    /**
     * Check the key stroke group more than max.
     * 
     * @since S60 v5.0
     * @param aBegPos that more than max begine position.
     * @return none.
     */
    void HighlightMoreThanSeven( TInt aBegPos );
    
    /**
     * Check the keystroke count.
     * 
     * @since S60 v5.0.
     * @param aIndex the index of keystroke.
     * @param aChineseCount the chinese character count.
     * @param aSymbol the zhuyinsymbol.
     * @param aZhuyinSymbol the zhuyin symbol array.
     * @return ETure if keystroke count is not more than max,EFalse others.
     */
    TBool ReplaceKeystrokeL( 
                          TInt aIndex, 
                          TInt aChineseCount,
                          const TDesC& aSymbol, 
                          CDesCArrayFlat* aZhuyinSymbol );
    
    /**
     * Update Selection
     *
     * @since S60 v5.0
     * @param aSelection the index of selecion.
     * @return None.
     */ 
    void RefreshUI();
    
    /**
     * Update indicator
     *
     * @since S60 v5.0
     * @return None.
     */
    void UpdateIndicator();
    
    void HandleCommandL(TInt /*aCommandId*/);
    
    TBool DeleteZhuyinSymbolOrChinese();
    
    
    /**
     * Change CBA.
     *
     * @since S60 v5.0.
     * @return None.
     */
    void ChangeCbaL();
    
private:
    
    /**
     * Check the keystroke count.
     * 
     * @since S60 v5.0.
     * @param aChineseCount the chinese character count.
     * @param aZhuyinSymbol the zhuyin symbol array.
     * @return ETure if keystroke count is not more than max,EFalse others.
     */
    TBool CheckKeystrokeCountL( TInt aChineseCount, CDesCArrayFlat* aZhuyinSymbol );
    
    void CheckShowInfo( TDes& aOutDes, CDesCArrayFlat* aZhuyinSymbol );
    
    void DeleteSeparatorWhenInvaild( TDes& aDes );

    };

#endif /*T_AKNFEPINPUTSTATEZHUYINPHRASECREATIONBASE_H*/

// End of file
