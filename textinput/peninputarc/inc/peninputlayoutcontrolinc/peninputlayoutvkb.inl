/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline function for virtual keyboard controls
*
*/


//class CVirtualKeyboard

// ---------------------------------------------------------------------------
// Set fading parameter
// ---------------------------------------------------------------------------
//
inline void CVirtualKeyboard::SetFadingParameters(TUint8 aBlackMap, 
                                                            TUint8 aWhiteMap)
    {
    iFadingParamBlack = aBlackMap;
    iFadingParamWhite = aWhiteMap;
    };

// ---------------------------------------------------------------------------
// Get key array
// ---------------------------------------------------------------------------
//
inline const RPointerArray<CVirtualKey>& CVirtualKeyboard::KeyArray()
    {
    return iKeyArray;
    }

// ---------------------------------------------------------------------------
// Get key highlight bitmap array
// ---------------------------------------------------------------------------
//    
inline const RPointerArray<CFbsBitmap>& CVirtualKeyboard::KeyHighlightBmpList()
    {
    return iKeyHighLightBmpList;
    }
    
// ---------------------------------------------------------------------------
// Get key highlight mask bitmap array
// ---------------------------------------------------------------------------
//    
inline const RPointerArray<CFbsBitmap>& CVirtualKeyboard::KeyHighlightMaskBmpList()
    {
    return iKeyHighLightMaskBmpList;
    }    
    
// ---------------------------------------------------------------------------
// CVirtualKeyboard::SetDimKeyBmp
// Set key dim bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
inline void CVirtualKeyboard::SetDimKeyBmp(CFbsBitmap* aBitmap)
    {
    iDimKeyBmp = aBitmap;
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyboard::SetDimKeyMaskBmp
// Set key dim maks bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
inline void CVirtualKeyboard::SetDimKeyMaskBmp(CFbsBitmap* aBitmap)
    {
    iDimKeyMaskBmp = aBitmap;    
    }    
    
// ---------------------------------------------------------------------------
// CVirtualKeyboard::RemoveKey
// Remove a key by index. Index is not checked.
// ---------------------------------------------------------------------------
//      
inline void CVirtualKeyboard::RemoveKey(TInt aIndex)
    {
    RemoveKey(iKeyArray[aIndex]);
    }
    
inline void CVirtualKeyboard::SetKeySkinId(TVirtualKeyBmpType aBmpType, 
                                           TAknsItemID aSkinId)
    {
    switch (aBmpType)
        {
        case EKeyBmpNormal:
            {
            iKeyNormalSkinId = aSkinId;
            }
            break;
        case EKeyBmpHighlight:
            {
            iKeyHighlightSkinId = aSkinId;
            }
            break;
        case EKeyBmpDim:
            {
            iKeyDimSkinId = aSkinId;
            }
            break;
        default:
            break;    
        }
    }

inline TAknsItemID CVirtualKeyboard::KeySkinId(TVirtualKeyBmpType aBmpType)
    {
    switch (aBmpType)
        {
        case EKeyBmpNormal:
            {
            return iKeyNormalSkinId;
            }
            break;
        case EKeyBmpHighlight:
            {
            return iKeyHighlightSkinId;
            }
            break;
        case EKeyBmpDim:
            {
            return iKeyDimSkinId;
            }
            break;
        default:
            break;    
        }

    return KAknsIIDNone;
    }

inline void CVirtualKeyboard::SetTextLineLayout(TAknTextLineLayout aTextLine, TVirtualKeyTextPos aPos)
    {
    if (aPos >= 0 && aPos <= EPosLast)
        {
        iTextLineLayout[aPos] = aTextLine;
        iTextLineSet[aPos] = ETrue;
        }
    }

inline TAknTextLineLayout CVirtualKeyboard::TextLineLayout(TVirtualKeyTextPos aPos)
    {
    if (aPos >= 0 && aPos <= EPosLast)
        {
        return iTextLineLayout[aPos];
        }

    return iTextLineLayout[0];
    }

inline void CVirtualKeyboard::SetTextLineLayout(TAknTextLineLayout aTextLine)
    {
    iVKBTextLineLayout = aTextLine;
    }

inline TAknTextLineLayout CVirtualKeyboard::TextLineLayout()
    {
    return iVKBTextLineLayout;
    }
    
inline TBool CVirtualKeyboard::TextLineLayoutSet(TVirtualKeyTextPos aPos)
    {
    if (aPos >= 0 && aPos <= EPosLast)
        {
        return iTextLineSet[aPos];
        }

    return EFalse;
    }
    
inline void CVirtualKeyboard::SetDrawOpaqueBackground(TBool aNeedDraw)
    {
    iNeedDrawBackground = aNeedDraw;
    }

inline CPenInputColorIcon* CVirtualKeyboard::ShiftIcon()
	{
	return iShiftIcon;	
	}

//class CVirtualKey


// ---------------------------------------------------------------------------
//Return key position
// ---------------------------------------------------------------------------
//
inline const TRect& CVirtualKey::Rect() const
    {
    return iRect;
    }
   
// ---------------------------------------------------------------------------
//Return key code
// ---------------------------------------------------------------------------
//
inline TUint CVirtualKey::KeyCode() const
    {
    return iCode;
    }

// ---------------------------------------------------------------------------
// CVirtualKey::KeyUnicodes
//Return key unicodes -- support for multi language
// ---------------------------------------------------------------------------
inline const TDesC& CVirtualKey::KeyUnicodes() const
    {
    return *iCodeData;
    }
    

// ---------------------------------------------------------------------------
//Return key scancode
// ---------------------------------------------------------------------------
//
inline TInt CVirtualKey::ScanCode() const
    {
    return iScanCode;
    }

// ---------------------------------------------------------------------------
//get highlight bmp index
// ---------------------------------------------------------------------------
//        
inline TInt CVirtualKey::HighlightBmpIndex()
    {
    return iHighlightBmpIndex;
    }

// ---------------------------------------------------------------------------
//get highlight bmp index
// ---------------------------------------------------------------------------
//        
inline TInt CVirtualKey::HighlightMaskBmpIndex()
    {
    return iHighlightMaskBmpIndex;
    }
    
// ---------------------------------------------------------------------------
// set key dimmed
// ---------------------------------------------------------------------------
//   
inline void CVirtualKey::SetDimmed(TBool aFlag)
    {
    iDimmed = aFlag;
    }
     
// ---------------------------------------------------------------------------
// set key dimmed
// ---------------------------------------------------------------------------
//   
inline TBool CVirtualKey::Dimmed() const
    {
    return iDimmed;
    }
            
// ---------------------------------------------------------------------------
// set key to be a latch key
// ---------------------------------------------------------------------------
//       
inline void CVirtualKey::SetLatchKey(TBool aFlag)
    {
    iIsLatchKey = aFlag;
    }
    
// ---------------------------------------------------------------------------
// get latch latch style flag
// ---------------------------------------------------------------------------
//       
inline TBool CVirtualKey::IsLatchKey()
    {
    return iIsLatchKey;
    }

// ---------------------------------------------------------------------------
// get latch state
// ---------------------------------------------------------------------------
//       
inline TBool CVirtualKey::Latched()
    {
    return iLatched;
    }

inline void CVirtualKey::SetInnerRect(const TRect& aRect)
    {
    innerRect = aRect;
    }

inline TRect CVirtualKey::InnerRect()
    {
    return innerRect;
    }

inline void CVirtualKeyboard::SetKeyTextColorGroup(TInt aColorGroup)
	{
	iKeyTextColorGroup = aColorGroup;
	}
inline TInt CVirtualKeyboard::KeyTextColorGroup() const
	{
	return iKeyTextColorGroup;
	}
	
inline TDesC* CVirtualKey::DisplayUnicode() const
	{
	return iDisplayCodeData;	
	}

inline void CVirtualKey::SetKeyData( const TDesC& aKeyData )
    {
    if ( iCodeData )
        {
        delete iCodeData;
        }
    iCodeData = aKeyData.AllocL();
    }

inline void CVirtualKey::SetKeyScancode( TInt aScancode )
    {
    iScanCode = aScancode;
    }

//end of class CVritualKeyboard