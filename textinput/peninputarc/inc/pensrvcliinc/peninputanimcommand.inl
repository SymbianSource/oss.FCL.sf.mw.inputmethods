/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  inline header for animation command
*
*/


//class TAnimCmd

// ---------------------------------------------------------------------------
// Set command ready flag
// ---------------------------------------------------------------------------
//    	
inline void TAnimCmd::SetReady(TBool aFlag)
	{
	iReady = aFlag;
	}
	

// ---------------------------------------------------------------------------
// Test whether command ready
// ---------------------------------------------------------------------------
//    	
inline TBool TAnimCmd::Ready() const
	{
	return iReady;
	}

