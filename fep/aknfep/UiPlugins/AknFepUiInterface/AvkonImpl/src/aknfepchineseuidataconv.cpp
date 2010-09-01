/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
*       Provides the CAknFepChineseUIDataConv methods.
*
*
*/


#include "aknfepchineseuidataconv.h"

TInt CAknFepChineseUIDataConv::AnyToInt(TAny* aOriginal)
    {
    return *(reinterpret_cast<TInt*>(aOriginal));
    }
    
TRect CAknFepChineseUIDataConv::AnyToRect(TAny* aOriginal)
    {
    return *(reinterpret_cast<TRect*>(aOriginal));
    }
    
CFont* CAknFepChineseUIDataConv::AnyToFont(TAny* aOriginal)
    {
    return reinterpret_cast<CFont*>(aOriginal);
    }
    
TPoint CAknFepChineseUIDataConv::AnyToPoint(TAny* aOriginal)
    {
    return *(reinterpret_cast<TPoint*>(aOriginal));
    }
    
TRgb CAknFepChineseUIDataConv::AnyToRgb(TAny* aOriginal)
    {
    return *(reinterpret_cast<TRgb*>(aOriginal));
    }
    
TSize CAknFepChineseUIDataConv::AnyToSize(TAny* aOriginal)
    {
    return *(reinterpret_cast<TSize*>(aOriginal));
    }
    
TAknTextLineLayout CAknFepChineseUIDataConv::AnyToTextLine(TAny* aOriginal)
    {
    return *(reinterpret_cast<TAknTextLineLayout*>(aOriginal));
    }

TAknLayoutText CAknFepChineseUIDataConv::AnyToLayoutText(TAny* aOriginal)
    {
    return *(reinterpret_cast<TAknLayoutText*>(aOriginal));
    }

TAknWindowLineLayout CAknFepChineseUIDataConv::AnyToWindowLine(TAny* aOriginal)
    {
    return *(reinterpret_cast<TAknWindowLineLayout*>(aOriginal));
    }

TAknLayoutRect CAknFepChineseUIDataConv::AnyToLayoutRect(TAny* aOriginal)
    {
    return *(reinterpret_cast<TAknLayoutRect*>(aOriginal));
    }

//End of File
