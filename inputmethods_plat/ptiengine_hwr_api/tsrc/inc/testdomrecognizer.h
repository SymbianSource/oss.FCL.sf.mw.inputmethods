/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               Declares main application class.
*
*/















#ifndef C_CTESTDOMHWRRECOGNIZER_H
#define C_CTESTDOMHWRRECOGNIZER_H

#include <PtiHwrRecognizer.h>

#include "testdomptienginehwr.h"

class CTestDomHwrRecognizer : public CHwrRecognizer
    {
public:
    /**
     * NewL() 
     */
    static CTestDomHwrRecognizer* NewL();       
    
    /**
     * deconstructor
     */
    ~CTestDomHwrRecognizer(); 
protected:

    /**
     * C++ default constructor
     */
    CTestDomHwrRecognizer();

    /**
    * Second phase constructor
    *
    * @return None
    */
    void ConstructL();
    
public:
    /**
     * friend class to invoke protected fuctions of class CHwrRecognizer
     */
    friend class CTestDOMPtiengineHWR;
    };

#endif

