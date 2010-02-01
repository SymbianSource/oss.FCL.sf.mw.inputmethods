/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Default number mode mapping data.
*
*/


#include "PtiDefaultNumberModeMappingdata.h"

const TUint16 MultitapData_table[] =
    {
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
	'*','+','p','w',    
    '0'    
    };

const TInt KMultiTapDataSize = 14;

const TPtiKeyBinding MultiTapKeyBindings_table[] =
    {
    {EPtiKey1, EPtiCaseLower, 0, 1},
    {EPtiKey2, EPtiCaseLower, 1, 1},
    {EPtiKey3, EPtiCaseLower, 2, 1},
    {EPtiKey4, EPtiCaseLower, 3, 1},
    {EPtiKey5, EPtiCaseLower, 4, 1},
    {EPtiKey6, EPtiCaseLower, 5, 1},
    {EPtiKey7, EPtiCaseLower, 6, 1},
    {EPtiKey8, EPtiCaseLower, 7, 1},
    {EPtiKey9, EPtiCaseLower, 8, 1},
    {EPtiKeyStar, EPtiCaseLower, 9, 4},
    {EPtiKey0, EPtiCaseLower, 13, 1},
    
    {EPtiKey1, EPtiCaseUpper, 0, 1},
    {EPtiKey2, EPtiCaseUpper, 1, 1},
    {EPtiKey3, EPtiCaseUpper, 2, 1},
    {EPtiKey4, EPtiCaseUpper, 3, 1},
    {EPtiKey5, EPtiCaseUpper, 4, 1},
    {EPtiKey6, EPtiCaseUpper, 5, 1},
    {EPtiKey7, EPtiCaseUpper, 6, 1},
    {EPtiKey8, EPtiCaseUpper, 7, 1},
    {EPtiKey9, EPtiCaseUpper, 8, 1},
    {EPtiKeyStar, EPtiCaseUpper, 9, 4},
    {EPtiKey0, EPtiCaseUpper, 13, 1}    
    };

const TInt KMaxMultitapIndex = 22;

//
// CPtiNumberModeDataImpl
//

// ---------------------------------------------------------------------------
// CPtiNumberModeDataImpl::NewL
// 
// ---------------------------------------------------------------------------
// 
CPtiNumberModeDataImpl* CPtiNumberModeDataImpl::NewL()
	{
    return new (ELeave) CPtiNumberModeDataImpl();	
	}
	
	
// ---------------------------------------------------------------------------
// CPtiNumberModeDataImpl::LanguageCode
// 
// ---------------------------------------------------------------------------
// 		
TInt CPtiNumberModeDataImpl::LanguageCode() const
    {
    return ELangNumeric;
    }


// ---------------------------------------------------------------------------
// CPtiNumberModeDataImpl::KeyData
// 
// ---------------------------------------------------------------------------
// 	
const TUint16* CPtiNumberModeDataImpl::KeyData(TPtiKeyboardType /*aKeyboardType*/, TInt& aDataSize) const
    {    
    aDataSize = KMultiTapDataSize;
    return MultitapData_table;       
    }


// ---------------------------------------------------------------------------
// CPtiNumberModeDataImpl::KeyBindingTable
// 
// ---------------------------------------------------------------------------
// 	
const TPtiKeyBinding* CPtiNumberModeDataImpl::KeyBindingTable(TPtiKeyboardType /*aKeyboardType*/, TInt& aNumItems) const
    {
    aNumItems = KMaxMultitapIndex;
    return MultiTapKeyBindings_table;
    }	
	
// End of file.
