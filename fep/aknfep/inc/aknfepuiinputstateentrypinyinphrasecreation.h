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
*       Provides the TAknFepInputStateEntryPinyinPhraseCreation definition.
*
*/












#ifndef T_AKNFEPUIINPUTSTATEENTRYPINYINPHRASECREATION_H
#define T_AKNFEPUIINPUTSTATEENTRYPINYINPHRASECREATION_H


#include "aknfepinputstatepinyinphrasecreationbase.h"


class TAknFepInputStateEntryPinyinPhraseCreation : public TAknFepInputStatePinyinPhraseCreationBase
    {
public:
    
    /**
     * C++ construct.
     *
     * @since S60 v5.0.
     * @return self object.
     */ 
    TAknFepInputStateEntryPinyinPhraseCreation(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    
    /**
     * HandleKeyL.
     *
     * @param aKey input key.
     * @param aLength TKeyPressLength.
     * @since S60 v5.0.
     * @return ETrue.
     */ 
    TBool HandleKeyL( TInt aKey, TKeyPressLength aLength );
    
    /**
     * Initialize State.
     *
     * @since S60 v5.0.
     * @return none.
     */ 
    void InitializeState();
    
private:
    
    /**
     * Implicity show the show info.
     *
     * @since S60 v5.0.
     * @return none.
     */        
    void ImplicityShowInfo( TBool aGetScreenCoordinates = EFalse );

    /**
     * Implicity show the show info.
     *
     * @since S60 v5.0.
     * @return none.
     */        
    void ImplicityShowInfoL( TBool aGetScreenCoordinates );

    /**
     * EKeyBackspace.
     *
     * @param aLength TKeyPressLength.
     * @since S60 v5.0
     * @return None.
     */        
    void HandleKeyBackspace( TKeyPressLength aLength );
    
    /**
     * EKeyOK And EKeyCBA1L.
     *
     * @since S60 v5.0
     * @return None.
     */        
    void HandleKeyOKAndKeyCBA1L();
    
    /**
     * others key input.
     *
     * @param aKey input key.
     * @since S60 v5.0
     * @return None.
     */        
    void HandleOthersKey( TInt aKey );
    
    /**
     * Judge that cursor can move.
     * 
     * @since S60 v5.0
     * @return ETrue the cursor can move, EFalse the cursor can't move.
     */
    TBool IsMoveCursor();
    
    /**
     * move cursor to left.
     * 
     * @since S60 v5.0
     * @return none.
     */
    void MoveCursorToLeft();
    
    /**
     * move cursor to left.
     * 
     * @since S60 v5.0
     * @return none.
     */
    void MoveCursorToRight();
    
    /**
     * Add phrase to DB.
     *
     * @since S60 v5.0
     * @param aPhraseAdd the phrase to be added to DB  
     * @return ETrue if the phrase is added to DB successfully, otherwise EFalse.
     */    
    TBool AddPhraseToDB( const TDesC& aPhraseAdd );
    
    /**
     * Try to close UI.
     * 
     * @since S60 v5.0
     * @return ETrue close the UI.
     */
    TBool TryCloseUI( TKeyPressLength aLength );
    
    };

#endif //T_AKNFEPUIINPUTSTATEENTRYPINYINPHRASECREATION_H

// End of file
