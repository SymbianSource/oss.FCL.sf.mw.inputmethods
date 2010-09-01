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
#ifndef T_TGENERICITUTCONVERTER_H
#define T_TGENERICITUTCONVERTER_H

// System includes
#include <e32cmn.h>
#include <e32des16.h>
#include <e32def.h>
#include <gdi.h>
#include <AknLayout2Def.h>

class TItutDataConverter
    {
public:
     static TInt AnyToInt(TAny* aOriginal);

     static TRect AnyToRect(TAny* aOriginal);

     static TPoint AnyToPoint(TAny* aOriginal);

     static TRgb AnyToRgb(TAny* aOriginal);

     static CFont* AnyToFont(TAny* aOriginal);
     
     static TSize AnyToSize(TAny* aOriginal);
     
     static TAknTextLineLayout AnyToTextLine(TAny* aOriginal);

     static RPointerArray<HBufC>& AnyToRptrArray(TAny* aOriginal);
     
     static RArray<TRect>& AnyToRectArray(TAny* aOriginal);

     static void ConvertChnPhraseCandidateL(const TDesC& aSource, RPointerArray<HBufC>& aList);

     static void ConvertNonPhraseCandidateL(const TDesC& aSource, RPointerArray<HBufC>& aList);
    };
    
#endif // T_TGENERICITUTCONVERTER_H
