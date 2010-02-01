/*
* Copyright (c) 2003-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepThaiSCTSelector methods.
*
*/












#include "AknFepThaiSCTSelector.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface

#include "AknFepCaseManager.h"

#include <uikon.hrh>
#include <PtiEngine.h>
#include <PtiDefs.h>
#include <aknSctDialog.h>
#include <avkon.rsg>
#include <AknFep.rsg>
#include <e32std.h>

#include <AknFepManager.h>
CAknFepThaiSCTSelector::CAknFepThaiSCTSelector()
    {
    }

CAknFepThaiSCTSelector::~CAknFepThaiSCTSelector()
    {
    }

TInt CAknFepThaiSCTSelector::ThaiSCRResourceId(TUint aPrevGlyph, TInt aKey)//TInt aResID)
    {
    return(ThaiSctCharMap(ThaiGlyphType(aPrevGlyph), aKey));
    }


CAknFepThaiSCTSelector *CAknFepThaiSCTSelector::NewL()
    {
    
    CAknFepThaiSCTSelector *self = new(ELeave)CAknFepThaiSCTSelector;
    return (self);
    
    }

TInt CAknFepThaiSCTSelector::ThaiSctCharMap(enum AknFepSCTThaiGlyphTypes aGlyphType, TInt aKey)
    {
    switch (aGlyphType)
        {
        case EAknFepThaiCharNonThai:
        case EAknFepThaiCharNon:
        case EAknFepThaiCharLV:
        case EAknFepThaiCharFV1:
        case EAknFepThaiCharFV2:
        case EAknFepThaiCharFV3:
        case EAknFepThaiCharBD:
        case EAknFepThaiCharTone:
        case EAknFepThaiCharAD1:
        case EAknFepThaiCharAD2:
        case EAknFepThaiCharAD3:
            return((aKey==EPtiKeyStar) ? R_AKNFEP_SCT_THAI_STAR_COMMON : 
                                         R_AKNFEP_SCT_THAI_0_COMMON);
                
        case EAknFepThaiCharCons:
            return((aKey==EPtiKeyStar) ? R_AKNFEP_SCT_THAI_STAR_CONS : 
                                         R_AKNFEP_SCT_THAI_0_CONS );
            
        case EAknFepThaiCharAV1:
        case EAknFepThaiCharBV1:
            return((aKey==EPtiKeyStar) ? R_AKNFEP_SCT_THAI_STAR_AV1_BV1 : 
                                         R_AKNFEP_SCT_THAI_0_AV1_BV1);
            
        case EAknFepThaiCharAV2:
        case EAknFepThaiCharBV2:
            return((aKey==EPtiKeyStar) ? R_AKNFEP_SCT_THAI_STAR_AV2_BV2 : 
                                         R_AKNFEP_SCT_THAI_0_AV2_BV2);
            
        case EAknFepThaiCharAV3:
            return((aKey==EPtiKeyStar) ? R_AKNFEP_SCT_THAI_STAR_AV3 : 
                                         R_AKNFEP_SCT_THAI_0_AV3);
            
        default:
            return(0);
        }
    }

AknFepSCTThaiGlyphTypes CAknFepThaiSCTSelector::ThaiGlyphType(TUint aGlyph)
    {
    if (aGlyph >= 0x0E01 && aGlyph <= 0x0E2E && aGlyph != 0x0E24 && aGlyph != 0x0E26 )
        {
        return(EAknFepThaiCharCons);
        }
    else if (aGlyph >= 0x0E40 && aGlyph <= 0x0E44)
        {
        return(EAknFepThaiCharLV);
        }
    else if (aGlyph == 0x0E30 || aGlyph == 0x0E32 || aGlyph == 0x0E33)
        {
        return(EAknFepThaiCharFV1);
        }
    else if (aGlyph == 0x0E45)
        {
        return(EAknFepThaiCharFV2);
        }
    else if (aGlyph == 0x0E24 || aGlyph == 0x0E26)
        {
        return(EAknFepThaiCharFV3);
        }
    else if (aGlyph == 0x0E38)
        {
        return(EAknFepThaiCharBV1);
        }
    else if (aGlyph == 0x0E39)
        {
        return(EAknFepThaiCharBV2);
        }
    else if (aGlyph == 0x0E3A)
        {
        return(EAknFepThaiCharBD);
        }
    else if (aGlyph >= 0x0E48 && aGlyph <= 0x0E4B)
        {
        return(EAknFepThaiCharTone);
        }
    else if (aGlyph == 0x0E4C || aGlyph == 0x0E4D)
        {
        return(EAknFepThaiCharAD1);
        }
    else if (aGlyph == 0x0E47)
        {
        return(EAknFepThaiCharAD2);
        }
    else if (aGlyph == 0x0E4E)
        {
        return(EAknFepThaiCharAD3);
        }
    else if (aGlyph == 0x0E34)
        {
        return(EAknFepThaiCharAV1);
        }
    else if (aGlyph == 0x0E31 || aGlyph == 0x0E36)
        {
        return(EAknFepThaiCharAV2);
        }
    else if (aGlyph == 0x0E35 || aGlyph == 0x0E37)
        {
        return(EAknFepThaiCharAV3);
        }
    else
        {
        return(EAknFepThaiCharNonThai);
        }
    
    
    }


// End of file


