/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline function for latch button
*
*/


//class CFepLayoutLatchButton
// -----------------------------------------------------------------------------
// Is button latched?
// -----------------------------------------------------------------------------
//    
inline TBool CFepLayoutLatchButton::Latched()
	{
	return iLatched;
	}
	
// -----------------------------------------------------------------------------
// set button latch state
// -----------------------------------------------------------------------------
//    
inline void CFepLayoutLatchButton::SetLatched(TBool aFlag)
	{
	iLatched = aFlag;
	}	

//end of file
