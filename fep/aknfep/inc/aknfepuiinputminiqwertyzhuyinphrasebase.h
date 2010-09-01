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
*      Provides the TAknFepInputMiniQwertyZhuyinPhraseBase definition.
*
*/












#ifndef T_AKNFEPUIINPUTMINIQWERTYZHUYINPHRASEBASE_H
#define T_AKNFEPUIINPUTMINIQWERTYZHUYINPHRASEBASE_H

//#include "AknFepUiInputStateChineseBase.h"
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"

class TAknFepInputMiniQwertyZhuyinPhraseBase : public TAknFepInputStateCandidateQwertyBaseChinesePhrase
    {
public:
    
    /**
     * C++ default constructor
     *
     * @since S60 v3.2.3
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputMiniQwertyZhuyinPhraseBase(
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
     * Get the valid keystroke.
     *
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue,if the valid keystroke is not null, EFalse otehrs.
     */
    TBool GetValidKeystroke();
    
    /**
     * Refresh editing pane
     *
     * @since S60 v3.2.3
     * @param None.
     * @return None
     */
    void RefreshEditPane();
    
    /**
     * Refresh spelling pane
     *
     * @since S60 v3.2.3
     * @param aClearSelection ETrue if need to clear current selection, otherwise EFalse.
     * @return None
     */
    void RefreshSpellingPane( TBool aClearSelection = EFalse );
    
    /**
     * Refresh candidate
     *
     * @since S60 v3.2.3
     * @param aClearSelection, if ETrue then clear selection of popup, others EFalse.
     * @return none
     */
    void RefreshCandidatePane( TBool aClearSelection = EFalse );

    /**
     * Get candidate info.
     *
     * @since S60 v3.2.3
     * @param aClearSelection, if ETrue then clear selection of popup, others EFalse.
     * @return none
     */
    void GetCandidate( TBool aClearSelection = EFalse );
    
    /**
     * Update indicator
     *
     * @since S60 v3.2.3
     * @param None
     * @return None.
     */
    void UpdateIndicator();
    
    /**
     * Change CBA.
     *
     * @since S60 v3.2.3
     * @param None.
     * @return None.
     */        
    void ChangeCbaL();
    
    /**
     * clear Selection
     *
     * @since S60 v5.0
     * @return None.
     */
    void ClearDeliberateSelection();
    
private:
    
    /**
     * clear the ptiengine keystroke.
     *
     * @since S60 v3.2.3
     * @param None
     * @return None
     */
    void ClearPtiEngineKeystroke();
    
    /**
     * Check the spelling whether is phrase spelling.
     *
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue,if one of spelling of PtiEngine is not phrase spelling, 
     *  EFalse others.
     */
    TBool CheckPhraseSpellingL();
    
    /**
     * Get the valid keystroke.
     *
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue,if the valid keystroke is not null, EFalse otehrs.
     */
    TBool GetValidKeystrokeL();
    
    /**
     * Check whether tone mark border upon.
     *
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue,if the tone mark border upon, EFalse otehrs.
     */
    TBool CheckToneMarkBorderUpon();
    
    /**
     * Get candidate info.
     *
     * @since S60 v3.2.3
     * @param aClearSelection, if ETrue then clear selection of popup, others EFalse.
     * @return none
     */
    void GetCandidateL( TBool aClearSelection = EFalse );
    };		

#endif //T_AKNFEPUIINPUTMINIQWERTYZHUYINPHRASEBASE_H

// End of file
