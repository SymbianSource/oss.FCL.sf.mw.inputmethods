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
* Description:            HWR layout UI class header
*
*/












#ifndef C_PLUGINFEPMANAGERHWR_H
#define C_PLUGINFEPMANAGERHWR_H

// System includes
#include <e32base.h>

// User includes
#include "pluginfepmanagerbase.h"

//Forward declaration
class CPenInputImePluginCn;
class RPeninputServer;

/**
 *  CPluginFepManagerHwr is the chinese HWR layout UI interface 
 *  "proxy" class.
 *
 *  @lib peninputimeplugincn.lib
 *  @since S60 v3.2
 */
class CPluginFepManagerHwr : public CPluginFepManagerBase
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aOwner IME plugin reference 
     * @param aPenInputServer The pointer of RPeninputServer.
     * @return Pointer to created CPluginFepManagerHwr object
     */
    static CPluginFepManagerHwr* NewL(CPenInputImePluginCn& aOwner,
                                      RPeninputServer* aPenInputServer);

     /**
     * Set layout UI current number mode key mapping.
     * For example, 0-9 ; 0-9,*,# and etc.
     * 
     * @since S60 v3.2
     *
     * @param aAknEditorNumericKeymap Key mapping enum value.
     * @return None.
     */
   void SetNumberModeKeyMappingL( TAknEditorNumericKeymap aAknEditorNumericKeymap );

    /**
     * Handle command come from FEP.
     * 
     * @since S60 v3.2
     *
     * @param aCommandId Command id.
     * @param aParam  Command parameter.
     * @return None.
     */
    void HandleCommandL( TInt aCommandId, TInt aParam );

private:
    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aOwner IME plugin reference 
     * @param aPenInputServer The pointer of RPeninputServer.
     */
    CPluginFepManagerHwr(CPenInputImePluginCn& aOwner,
                         RPeninputServer* aPenInputServer);
 
    /**
     * Set layout primary range.
     *
     * @since S60 v3.2
     * @param aEditorState Current editor state.
     * @return None
     */
    void SetLayoutRange(const CAknEdwinState* aEditorState);
    
    /**
     * Flag to skin set number key mapping.
     */
    TInt iNumberKeyMapping;
    };

#endif // C_PLUGINFEPMANAGERHWR_H

// End Of File
