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
* Description:  trigger string implementation
*
*/

//  INCLUDES
#include "peninputfingerhwrtriggerstr.h"

// ---------------------------------------------------------
// destructor
// ---------------------------------------------------------
//
CAknFepHwrTriggerStr::~CAknFepHwrTriggerStr()
    {
    }

// ---------------------------------------------------------
// Append the char code to trigger string 
// ---------------------------------------------------------
//
void CAknFepHwrTriggerStr::Append( const TDesC& aChar )
    {
    const TInt len = aChar.Length()+ iString.Length();
    if ( len <= iString.MaxLength())
        {
        iString.Append( aChar );
        }
    }

// ---------------------------------------------------------
// Set the char code to trigger string 
// ---------------------------------------------------------
//
void CAknFepHwrTriggerStr::Set( const TDesC& aChar )
    {
    if ( aChar.Length() <= iString.MaxLength() )
        {
        iString.Zero();
        iString.Append( aChar );
        }
    }
    

// ---------------------------------------------------------
// Get trigger string 
// ---------------------------------------------------------
//
TDesC& CAknFepHwrTriggerStr::GetString()
    {
    return iString;
    }
    
// ---------------------------------------------------------
// Delete the last char
// ---------------------------------------------------------
//
void CAknFepHwrTriggerStr::DeleteLast()
    {
    if (iString.Length() > 0)
        {
        iString.Delete(iString.Length() - 1, 1);
        }
    }

// ---------------------------------------------------------
// Reset the trigger string
// ---------------------------------------------------------
//   
void CAknFepHwrTriggerStr::DeleteAll()
    {
    iString.Zero();
    }

// ---------------------------------------------------------
// Is empty for the tringger string
// ---------------------------------------------------------
//      
TBool CAknFepHwrTriggerStr::IsEmpty()
    {
    return (iString.Length() == 0);
    }       
    
// End Of File
