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
* Description:  inline function implementation for gspeninputinterface
*
*/

 
inline CPenInputGSInterface* CPenInputGSInterface::NewL() 
    {
    const TUid KPenInputImpleUid = {0x1028185A};

    TAny* interface = REComSession::CreateImplementationL (
        KPenInputImpleUid, _FOFF (CPenInputGSInterface, iDtor_ID_Key));

    return reinterpret_cast <CPenInputGSInterface*> (interface);
    }
inline CPenInputGSInterface::~CPenInputGSInterface()
    {
    REComSession::DestroyedImplementation (iDtor_ID_Key);
    }

// End Of File
