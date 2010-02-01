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
* Description:  inline file of button
*
*/


inline TInt CAknFepCtrlMultiModeButton::NumberOfModes() const
    {
    return iNumberOfModes;
    }

inline TInt CAknFepCtrlMultiModeButton::CurrentMode() const
    {
    return iCurrentMode;
    }
    
inline void CAknFepCtrlMultiModeButton::SetFont(const CFont* aFont)
    {
    iFont = aFont;
    }
    
inline void CAknFepCtrlMultiModeButton::SetShadowFont(const CFont* aShadowFont)
	{
	iShadowFont = aShadowFont;
	}

// End Of File
