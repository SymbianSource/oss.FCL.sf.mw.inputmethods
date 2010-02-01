/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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












#ifndef __AKN_FEP_THAI_SCT_SELECTOR__
#define __AKN_FEP_THAI_SCT_SELECTOR__

#include <e32def.h>
#include <e32base.h>

enum AknFepSCTThaiGlyphTypes
    {
    EAknFepThaiCharNon,
    EAknFepThaiCharCons,
    EAknFepThaiCharLV,
    EAknFepThaiCharFV1,
    EAknFepThaiCharFV2,
    EAknFepThaiCharFV3,
    EAknFepThaiCharBV1,
    EAknFepThaiCharBV2,
    EAknFepThaiCharBD,
    EAknFepThaiCharTone,
    EAknFepThaiCharAD1,
    EAknFepThaiCharAD2,
    EAknFepThaiCharAD3,
    EAknFepThaiCharAV1,
    EAknFepThaiCharAV2,
    EAknFepThaiCharAV3,
    EAknFepThaiCharNonThai,
    };

class CAknFepThaiSCTSelector : public CBase
    {
    public:
        
        TInt ThaiSCRResourceId(TUint aPrevGlyph, TInt aKey);

        static CAknFepThaiSCTSelector* NewL();
        
        ~CAknFepThaiSCTSelector();
        
    private:
        
        CAknFepThaiSCTSelector();

        AknFepSCTThaiGlyphTypes ThaiGlyphType(TUint aGlyph);

        TInt ThaiSctCharMap(enum AknFepSCTThaiGlyphTypes aGlyphType, TInt aKey);
        
    };

#endif // __AKN_FEP_UI_THAI_CHARACTER_MAP_SELECTOR__

// End of file
