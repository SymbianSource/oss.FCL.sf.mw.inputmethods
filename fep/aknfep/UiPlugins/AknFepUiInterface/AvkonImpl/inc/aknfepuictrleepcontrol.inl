/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
 *      Provides the CAknFepUICtrlPinyinEEP definition 
 *
*/












// ---------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------
//
inline CAknFepUICtrlEEPControl::TSegment::TSegment( TInt aBeg, TInt aEnd,
    TDspStyle aStyle )
    {
    iBegin = aBeg;
    iEnd = aEnd;
    iStyle = aStyle;
    }

// ---------------------------------------------------------
// Compare the Index.
// ---------------------------------------------------------
//
inline TInt CAknFepUICtrlEEPControl::TSegment::CompareByBegIndex(
    const TSegment& s1, const TSegment& s2 )
    {
    if ( s1.iBegin == s2.iBegin )
        return 0;
    else
        return s1.iBegin > s2.iBegin ? 1 : -1;
    }

