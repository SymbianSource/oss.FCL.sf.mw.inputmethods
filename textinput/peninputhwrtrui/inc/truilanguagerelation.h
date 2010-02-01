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
* Description:  TLanguageRelation class of TrainingUI
*
*/


#ifndef T_TRUILANGUAGERELATION_H
#define T_TRUILANGUAGERELATION_H

#include <e32std.h>

class TLanguageRelation
    {
public:
    TLanguageRelation( TInt aCode, TInt aLanguageScriptId );
    
    inline TInt LanguageCode() const
        {
        return iCode;
        };
    
    inline TInt LanguageScriptId() const
        {
        return iLanguageScriptId;
        };
  
private:
    TInt iCode;
    TInt iLanguageScriptId;    
    };

#endif // T_TRUILANGUAGERELATION_H
