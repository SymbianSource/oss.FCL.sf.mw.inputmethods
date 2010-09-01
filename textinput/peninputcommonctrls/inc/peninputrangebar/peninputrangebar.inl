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
* Description:  inline file of range bar
*
*/


inline TInt CAknFepCtrlRangeBar::InputRange() const
    {
    return iInputRange;
    }

inline void CAknFepCtrlRangeBar::SetStyle(const TButtonGroupStyle aNewStyle)
    {
    iStyle = aNewStyle;
    }

inline TInt CAknFepCtrlRangeBar::Style() const
    {
    return iStyle;
    }
    
inline void CAknFepCtrlRangeBar::SetCellRect(const TRect aRect)
    {
    iIndicatorRect = aRect;
    }

// End Of File
