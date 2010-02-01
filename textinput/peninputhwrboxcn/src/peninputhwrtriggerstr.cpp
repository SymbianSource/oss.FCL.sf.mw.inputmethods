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
* Description:  trigger string implementation
*
*/


//  INCLUDES
#include "peninputhwrtriggerstr.h"

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
void CAknFepHwrTriggerStr::Append(const TDesC& aChar)
    {
    const TInt len = aChar.Length()+ iString.Length();
    if ( len <= iString.MaxLength())
        {
        iString.Append(aChar);
        }
    }

// ---------------------------------------------------------
// Set the char code to trigger string 
// ---------------------------------------------------------
//
void CAknFepHwrTriggerStr::Set(const TDesC& aChar)
    {
    if ( aChar.Length() <= iString.MaxLength())
        {
        iString.Zero();
        iString.Append(aChar);
        }
    }

// ---------------------------------------------------------
// Using the last char code as trigger string 
// ---------------------------------------------------------
//
void CAknFepHwrTriggerStr::SetLast()
    {
    iString.Delete(0, iString.Length() - 1);
    }

// ---------------------------------------------------------
// Delete the last char code 
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
// Is the trigger string empty? 
// ---------------------------------------------------------
//
TBool CAknFepHwrTriggerStr::IsEmpty()
    {
    return (iString.Length() == 0);
    }

// ---------------------------------------------------------
// Get trigger string 
// ---------------------------------------------------------
//
TDesC& CAknFepHwrTriggerStr::GetString()
    {
    return iString;
    }
    
// End Of File
