/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/

// INCLUDES
#include <AknInputLanguageInfo.h>

// CLASS IMPLEMENTATION
EXPORT_C TAknInputLanguageCapabilities::TAknInputLanguageCapabilities()
	{
	iCapabilities = 0; // All flags cleared. No "default" capabilities
	}

EXPORT_C void TAknInputLanguageCapabilities::AssignCapability( TInt aCapabilityIndex, TBool aSet )
	{
	iCapabilities.Assign( aCapabilityIndex, aSet );
	}

EXPORT_C TBool TAknInputLanguageCapabilities::HasCapability( TInt aCapabilityIndex ) const
	{
	return iCapabilities.IsSet( aCapabilityIndex );
	}

EXPORT_C void TAknInputLanguageCapabilities::SetAllCapabilities()
	{
	// implementation has to anticipate future capabilities too.  So set all bits in 
	// the bitfield
	iCapabilities.SetAll();
	}

EXPORT_C TAknInputLanguageCapabilities TAknInputLanguageCapabilities::FilteredCapabilities( TAknInputLanguageCapabilities& aFilter) const
	{
	TAknInputLanguageCapabilities retVal;
	retVal.iCapabilities = TBitFlags32(iCapabilities.iFlags & aFilter.iCapabilities.iFlags);
	return retVal;
	}

EXPORT_C TBool TAknInputLanguageCapabilities::HasAnySupport() const
	{
	return (iCapabilities.iFlags != 0 );
	}

// End of File
