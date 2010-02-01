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
*       Provides the aknfepinputstatecandidatepinyinphrasecreation definition.
*
*/












#ifndef T_AKNFEPINPUTSTATECANDIDATEPINYINPHRASECREATION_H
#define T_AKNFEPINPUTSTATECANDIDATEPINYINPHRASECREATION_H

#include "aknfepinputstatepinyinphrasecreationbase.h"

class TAknFepInputStateCandidatePinyinPhraseCreation : 
    public TAknFepInputStatePinyinPhraseCreationBase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V5.0
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     * @return self object
     */
    TAknFepInputStateCandidatePinyinPhraseCreation(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);
    
protected:

    /**
     * From TAknFepInputStateCandidateBasePhrase
     * Handle system key press
     *
     * @since S60 V5.0
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    /**
     * Update indicator
     *
     * @since S60 V5.0
     * @return none.
     */
    void UpdateIndicator();
    
    /**
     * Handle horizontal navigation
     *
     * @since S60 V5.0
     * @return ETrue if can move left or right,EFalse others.
     */
    TBool HandleHorizontalNavigation(TInt aKey);
    
    /**
     * Handle vertical navigation
     *
     * @since S60 V5.0
     * @return ETrue if can move up or down,EFalse others.
     */
    TBool HandleVerticalNavigation(TInt aKey);
    
    /**
     * Commit Inline to the EEP ctrl
     *
     * @since S60 V5.0
     * @param aText the commit info.
     * @return ETrue if can move up or down,EFalse others.
     */
    TBool CommitInlineEEPL( const TDesC& aText  );

private:
    
    /**
     * KeyBackspace.
     *
     * @since S60 V5.0
     * @return none.
     */
    void KeyBackspace();
    
    /**
     * Key Up,Down,Left,RightArrow Or Key CBA1,OK.
     *
     * @since S60 V5.0
     * @return none.
     */
    void HandleKeyUpDownLeftRightArrowOrKeyCBA1OKL( 
                                 TInt aKey, 
                                 TKeyPressLength aLength );
    /**
       * the state change or not 
       *
       * @since S60 V5.0
       * @return none.
       */
    void ChangeStateOrNotL();
    };

#endif //T_AKNFEPINPUTSTATECANDIDATEPINYINPHRASECREATION_H

// End of file
