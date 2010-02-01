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
* Description:
*
*/
#include "peninputlayoutmicfnomatchesinline.h"

_LIT(KTextForUnknownPredictiveTextWestern, "?");
_LIT(KTextForUnknownPredictiveTextArabic, "\x061f");   // Arabic Question Mark
#define ARABIC_CODE_PAGE_BASE   0x0600

TPtrC TFepLayoutMIcfNoMatchesInline::TextForUnknownPredictiveTextIndication(
                                                    const TDesC& aTextSample)
    {
    TUint codebase = CodeBaseOfText(aTextSample);

    if (codebase == ARABIC_CODE_PAGE_BASE)
        return KTextForUnknownPredictiveTextArabic(); //unknownTextBuffer;
    else
        return KTextForUnknownPredictiveTextWestern(); //unknownTextBuffer;
    }

TUint TFepLayoutMIcfNoMatchesInline::CodeBaseOfText(const TDesC& aTextSample)
    {
    TUint base = 0;

    // examine buffer; take first printable character
    // if none found, then 0 is returned
    TChar character;
    for (TInt index = 0;index < aTextSample.Length(); index++)
	    {
        character = aTextSample[index];
        if ( character.IsPrint())
            {
            TUint ch(character); 
            ch &= 0xffffff00; // only interested in upper bits
            base = (TUint)ch;
            break;
            }
	    }

    return base;
    }
    
// End Of File
