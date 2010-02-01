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
*       Provides the TAknFepInputStateEntryZhuyinPhraseCreation methods.
*
*/












#ifndef T_AKNFEPINPUTSTATEENTRYZHUYINPHRASECREATION_H
#define T_AKNFEPINPUTSTATEENTRYZHUYINPHRASECREATION_H


//#include "AknFepUIInputStateChineseBase.h"
#include "aknfepuiinputstatezhuyinphrasecreationbase.h"

class TAknFepInputStateEntryZhuyinPhraseCreation : public TAknFepInputStateZhuyinPhraseCreationBase
    {
public:
    /**
     * C++ constructor.
     * 
     * @since S60 v5.0.
     * @param fepuimanagerstate interface
     * @param uicontainer
     * @return self object.
     */  
    TAknFepInputStateEntryZhuyinPhraseCreation(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);
    
    /**
     * Handle Key function.
     * 
     * @since S60 v5.0.
     * 
     * @param the user input key.
     * @param TKeyPressLength the key length.
     * @return ETrue if handle key is dealed with, EFalse others.
     */  
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    /**
     * Initialize State.
     * 
     * @since S60 v5.0.
     * @return None.
     */ 
    void InitializeState();

private:

    /**
     * Set the show info high light.
     * 
     * @since S60 v5.0.
     * @return none.
     */         
    void SetHighLight();
    
    /**
     * Set the show info high light.
     * 
     * @since S60 v5.0.
     * @return none.
     */         
    void SetHighLightMoreThanMax();
    
    /**
     * move the index of the keystroke left.
     * 
     * @since S60 v5.0.
     * @return ETrue if move ok.
     */
    TBool MoveIndexOfKeystrokeLeft();

    /**
     * move the index of the keystroke right.
     * 
     * @since S60 v5.0.
     * @return ETrue if move ok.
     */
    TBool MoveIndexOfKeystrokeRight();
    
    /**
     * Add phrase to DB.
     *
     * @since S60 v5.0.
     * @param aPhraseAdd the phrase to be added to DB  
     * @return ETrue if the phrase is added to DB successfully, otherwise EFalse.
     */
     TBool AddPhraseToDB( const TDesC& aPhraseAdd );
    
    /**
     * Check auto change state.
     * 
     * @since S60 v5.0.
     * @return ETrue if need to auto change state,EFalse others.
     */
    TBool AutoChangeState();
    
    /**
     * Check high light.
     * 
     * @since S60 v5.0.
     * @return ETrue if the hight light,EFalse others.
     */
    TBool CheckHighLightL(); 
    
    };

#endif //T_AKNFEPINPUTSTATEENTRYZHUYINPHRASECREATION_H

// End of file
