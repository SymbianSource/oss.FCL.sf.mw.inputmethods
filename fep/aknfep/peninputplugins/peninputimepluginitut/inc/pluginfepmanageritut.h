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
* Description:            ITUT layout UI class header
*
*/











#ifndef C_PLUGINFEPMANAGERITUT_H
#define C_PLUGINFEPMANAGERITUT_H

// System includes
#include <e32base.h>

// User includes
#include "pluginfepmanagerbase.h"

class CPenInputImePluginItut;

/**
 *  CPluginFepManagerItut is the ITUT layout UI interface 
 *  "proxy" class.
 *
 *  @lib peninputimepluginitut.lib
 *  @since S60 v3.2
 */
class CPluginFepManagerItut : public CPluginFepManagerBase
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aOwner IME plugin reference 
     * @param aPenInputServer The pointer of RPeninputServer.
     * @return Pointer to created CPluginFepManagerItut object
     */
    static CPluginFepManagerItut* NewL( CPenInputImePluginItut& aOwner,
                                      RPeninputServer* aPenInputServer );
                                      
    /**
     * Handle key event.
     * Since most plugin layout UIs are based on animation, they receive 
     * key and process key events before this function. 
     * 
     * @since S60 v3.2
     *
     * @param aKey Key event code.
     * @param aLength Key press length.
     * @return ETrue if processed, EFalse otherwise.
     */
    TBool HandleKeyL( TInt aKey, TKeyPressLength aLength, TEventCode aEventCode = EEventKey );                                      

private:
    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aOwner IME plugin reference 
     * @param aPenInputServer The pointer of RPeninputServer.
     */
     
    CPluginFepManagerItut( CPenInputImePluginItut& aOwner,
                         RPeninputServer* aPenInputServer );
    };
#endif // C_PLUGINFEPMANAGERITUT_H

// End Of File
