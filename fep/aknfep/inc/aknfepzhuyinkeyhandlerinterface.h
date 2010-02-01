/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/










/**
 * aknfepzhuyinkeyhandlerinterface.h
 * 
 */
#ifndef M_AKNFEPZHUYINKEYHANDLERINTERFACE_H
#define M_AKNFEPZHUYINKEYHANDLERINTERFACE_H

#include <e32std.h>

class MZhuyinKeyHandler
    {
public:
    enum TStatusChgFlag
    {
    EInit,
    EFromSpellingEditingToEntry,
    EFromCandidateToEntry
    };
    virtual TInt GetZhuyinSymbol ( TPtiKey aKey, TDes& aOutBuf ) = 0;
    virtual void Reset() = 0;
    virtual TInt GetToneMark ( const TDes& aBaseSymbol, TDes& aOutBuf ) = 0;
    virtual void SetState( TInt aState ) = 0;
    virtual TInt GetState() = 0;
    virtual void SetStatusChgFlag ( TStatusChgFlag aChgFlag ) = 0;
    virtual TStatusChgFlag GetStatusChgFlag () = 0;
    };

#endif /*M_AKNFEPZHUYINKEYHANDLERINTERFACE_H*/
