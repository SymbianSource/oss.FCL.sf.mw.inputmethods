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
* Description:            Provides CAknFepManager definition
*
*/


#ifndef C_HBUFCARRAY_H
#define C_HBUFCARRAY_H

// INCLUDES
#include <e32std.h>
#include <e32cmn.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  class CHBufCArray
 *
 *  Wrapper of HBufC Array.
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
class CHBufCArray : public CBase
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created CHBufCArray object
     */    
    static CHBufCArray* NewL();
    
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created CHBufCArray object
     */    
    static CHBufCArray* NewLC();

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */    
    ~CHBufCArray();
    
    /**
     * get array.
     * 
     * @since S60 v5.0
     * @return ref of array.
     */
    RPointerArray<HBufC>& Array();
    
private:

    /**
     * C++ default constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    CHBufCArray();
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    void ConstructL();

private:
    /**
     * HBufC Array.
     */
    RPointerArray<HBufC> iHBufCArray;
    };

#endif // C_HBUFCARRAY_H

// End Of File
