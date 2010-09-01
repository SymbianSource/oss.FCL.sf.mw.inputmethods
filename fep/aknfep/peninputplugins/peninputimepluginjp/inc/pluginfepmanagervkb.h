/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            VKB layout UI class header
*
*/











#ifndef C_PLUGINFEPMANAGERVKB_H
#define C_PLUGINFEPMANAGERVKB_H

#include <e32base.h>
#include "pluginfepmanagerbase.h"

class CPenInputImePluginJp;

/**
 *  CPluginFepManagerHwr is the Japanese VKB layout UI interface 
 *  "proxy" class.
 *
 *  @lib peninputimepluginjp.lib
 *  @since S60 v3.2
 */
class CPluginFepManagerVkb : public CPluginFepManagerBase
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
    static CPluginFepManagerVkb* NewL(CPenInputImePluginJp& aOwner,
                                      RPeninputServer* aPenInputServer);

private:
    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aOwner IME plugin reference 
     * @param aPenInputServer The pointer of RPeninputServer.
     */
     
    CPluginFepManagerVkb(CPenInputImePluginJp& aOwner,
                         RPeninputServer* aPenInputServer);
    };
#endif // C_PLUGINFEPMANAGERVKB_H

// End Of File
