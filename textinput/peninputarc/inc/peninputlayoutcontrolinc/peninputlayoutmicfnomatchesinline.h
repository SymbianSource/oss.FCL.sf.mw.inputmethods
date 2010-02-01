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
#ifndef TPENINPUTLAYOUTMICFNOMATCHESINLINE_H
#define TPENINPUTLAYOUTMICFNOMATCHESINLINE_H

#include <e32base.h>

class TFepLayoutMIcfNoMatchesInline
    {
public:

    /**
     * This returns a text sensitive no matches indication
     *
     * @returns a TPtr to the No Matches indication text to use for this buffer
     */
    static TPtrC TextForUnknownPredictiveTextIndication(const TDesC& aTextSample);

private:    
    /**
     * Returns a value that is the base value of a unicode code page. The significant content of
     * the buffer ( indices [1,aBufferLength]) and is examined from low to high index.  
     * Code page is the here defined as the unicode value excluding its lowest byte - Masked 
     * by 0xFF00 
     *
     * If a character with non-zero code page is found, that value is returned.  Otherwise, 0x0000
     * is returned
     * 
     * To totally dependable, this routine depends upon the chunk being of uniform codepage, 
     * or mixed codepage 0 + one other codepage.  For no-matches indicator text, this is the case
     *
     * @param aTextSample    buffer containing text, 
     * @returns              Codepage of first
     */
    static TUint CodeBaseOfText(const TDesC& aTextSample);
    };

#endif // TPENINPUTLAYOUTMICFNOMATCHESINLINE_H