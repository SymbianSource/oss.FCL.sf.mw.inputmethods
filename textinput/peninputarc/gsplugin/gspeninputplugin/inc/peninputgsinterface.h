/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  peninputgsinterface header file.
*
*/

 
#ifndef C_PENINPUTGSINTERFACE_H
#define C_PENINPUTGSINTERFACE_H

// INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>
 
/**
 *  CPenInputGSInterface interface class
 *
 *  This set of interfaces offers pen input settings. User can use this interface 
 *  to launch pen input settings directly not only its main view but also its four 
 *  setting pages which are "Pen input language", "Writing speed", "Pen trail width"
 *  and "Pen trail colour".
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CPenInputGSInterface : public CBase
    {

public: 

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CGSPenInputInterface object
     */
    static CPenInputGSInterface* NewL();

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPenInputGSInterface();
        
// New

    /**
     * Dispaly input pen main view
     *
     * @since S60 v3.2
     * @return None.
     */
    virtual void ShowMainViewL() = 0;

    /**
     * Display "Pen input language" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    virtual TBool ShowInputLanguagePageL() = 0;

    /**
     * Display "Input Mode" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    virtual TBool ShowInputModePageL() = 0;
    
    /**
     * Display "Writing speed" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    virtual TBool ShowWritingSpeedPageL() = 0;
    
    /**
     * Display "Guide Line" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    virtual TBool ShowGuideLinePageL() = 0;    
    
    /**
     * Display "Pen trail width" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    virtual TBool ShowTrailWidthPageL() = 0;
    virtual TBool ShowRecognitionWithDictionaryL() = 0;
    
    /**
     * Display "Pen trail width" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    virtual TBool ShowChineseFindMethodPageL() = 0;

    /**
     * Display "Pen trail colour" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    virtual TBool ShowTrailColourPageL() = 0;
    
private:
     
    /**
     * Instance identifier key 
     */
     TUid iDtor_ID_Key;
    
    };

#include "peninputgsinterface.inl" 

#endif // C_PENINPUTGSINTERFACE_H

// End Of File
