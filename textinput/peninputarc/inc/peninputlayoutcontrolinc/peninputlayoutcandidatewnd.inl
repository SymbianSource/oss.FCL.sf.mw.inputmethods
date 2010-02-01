/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline function for candidate window
*
*/


//class CCandidateWnd

// -----------------------------------------------------------------------------
// Set baseline
// -----------------------------------------------------------------------------
//    
inline void CCandidateWnd::SetBaseline(TInt aBaseline)
    {
    iBaseline = aBaseline;
    }
    
// -----------------------------------------------------------------------------
// Get baseline
// -----------------------------------------------------------------------------
//    
inline TInt CCandidateWnd::Baseline()
    {
    return iBaseline;
    }
    
//end of file

