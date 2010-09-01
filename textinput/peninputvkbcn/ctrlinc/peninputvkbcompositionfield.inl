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
* Description:  Inline Implementation of CCompositionConvertedElement
*
*/



// ---------------------------------------------------------
// Get the key
// ---------------------------------------------------------
//
inline const TDesC* CCompositionElement::Key() const
    {
    return iKey;
    }

// ---------------------------------------------------------
// Compare function
// ---------------------------------------------------------
//
inline TBool CCompositionElement::operator == (TUint aKeyValue) const
    {
    return (*iKey)[0] == aKeyValue;
    }
// ---------------------------------------------------------
// determine the type of element
// ---------------------------------------------------------
//
inline TBool CCompositionElement::IsTypeOf(TCompositionElementType aType) const
    {
    return (aType & iKeyType);
    }
    
// ---------------------------------------------------------
// Is element is a seperator?
// ---------------------------------------------------------
//
inline TBool CCompositionElement::IsSeparator() const
    {
    return IsTypeOf(EElementTypeSeparator);
    }
    
// ---------------------------------------------------------
// Is element an automatic seperator?
// ---------------------------------------------------------
//
inline TBool CCompositionElement::IsAutoSeparator() const
    {
    return IsTypeOf(EElementTypeAutoSeparator);
    //return iKey && (iKey->Compare(KAutoSeparator) == 0);
    }
    
// ---------------------------------------------------------
// Is element a manul seperator?
// ---------------------------------------------------------
//
inline TBool CCompositionElement::IsManualSeparator() const
    {
    return IsTypeOf(EElementTypeManualSeparator);
    }

// ---------------------------------------------------------
// Is element a tone mark?
// ---------------------------------------------------------
//
inline TBool CCompositionElement::IsTonemark() const
    {
    return IsTypeOf(EElementTypeTonemark);
    }

// ---------------------------------------------------------
// Is element a manul auxilian?
// ---------------------------------------------------------
//
inline TBool CCompositionElement::IsAuxilian() const
    {
    return IsTypeOf(EElementTypeAuxilian);
    }
    
// ---------------------------------------------------------
// Is element visiable?
// ---------------------------------------------------------
//
inline TBool CCompositionElement::IsVisible()
    {
    return !iHide;
    }

// ---------------------------------------------------------
// Set element show/hide
// ---------------------------------------------------------
//
inline void CCompositionElement::Hide(TBool aHide)
    {
    iHide = aHide;
    }

// End Of File
