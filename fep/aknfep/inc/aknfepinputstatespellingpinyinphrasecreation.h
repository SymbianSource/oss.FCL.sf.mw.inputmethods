/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the aknfepinputstatespellingpinyinphrasecreation definition.
*
*/












#ifndef T_AKNFEPINPUTSTATESPELLINGPINYINPHRASECREATION_H
#define T_AKNFEPINPUTSTATESPELLINGPINYINPHRASECREATION_H

#include "aknfepinputstatepinyinphrasecreationbase.h"

class TAknFepInputStateSpellingPinyinPhraseCreation : public TAknFepInputStatePinyinPhraseCreationBase
    {
public:
    
    /**
     * C++ construct.
     *
     * @since S60 v5.0.
     * @return self object.
     */ 
    TAknFepInputStateSpellingPinyinPhraseCreation(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    
    /**
     * HandleKeyL.
     *
     * @since S60 v5.0.
     * @param aKey the input key.
     * @param TKeyPressLength the key press length.
     * @return ETrue.
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    /**
     * Initialize State.
     *
     * @since S60 v5.0.
     * @return none.
     */
    void InitializeStateL();

private:
    
    /**
     * Implicity Show Info.
     *
     * @since S60 v5.0
     * @return None.
     */    
    void ImplicityShowInfo( TBool aGetScreenCoordinates = EFalse );
    
    /**
     * Refresh Show Info.
     *
     * @since S60 v5.0
     * @return None.
     */    
    void RefreshShowInfo();
    
    /**
     * KeyBackspace Or KeyUpArrow
     *
     * @since S60 v5.0
     * @param TKeyPressLength the key press length.
     * @return None.
     */
    void HandleKeyBackspaceOrKeyUpArrow( TKeyPressLength aLength );
    
    
    /**
     * KeyBackspace And KeyUpArrow
     *
     * @since S60 v5.0
     * @param TKeyPressLength the key press length.
     * @return None.
     */
    void HandleKeyOKOrKeyDownArrowOrKeyCBA1( TKeyPressLength aLength );
  
    /**
     * KeyRightArrow.
     *
     * @since S60 v5.0
     * @return None.
     */  
    void HandleKeyRightArrow();
    
    /**
     * KeyLeftArrow.
     *
     * @since S60 v5.0
     * @return None.
     */ 
    void HandleKeyLeftArrow();
    /**
     * Refresh Valid Keystroke.
     *
     * @since S60 v5.0
     * @return ETrue refresh ok, others EFalse.
     */  
    TBool RefreshValidKeystroke();
    
    /**
     * Refresh Valid Keystroke.
     *
     * @since S60 v5.0
     * @return None.
     */  
    void RefreshValidKeystrokeL();
    
    };

#endif //T_AKNFEPINPUTSTATESPELLINGPINYINPHRASECREATION_H

// End of file
