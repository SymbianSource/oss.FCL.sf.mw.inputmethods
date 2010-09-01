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
* Description:  vkb control pool inline function 
*
*/


inline TInt CPinyinAnalyserDbFeed::GetSpellCount() const
    {
    return iSpellLenBuf.Length();
    }
    
inline const TPtrC CPinyinAnalyserDbFeed::GetSpellByIndex( TInt aIndex ) const
    {
    int offset = 0;
    for ( TInt i = 0;  i < aIndex;  ++i  )
        {
        offset += iSpellLenBuf[i];
        }
    return iSpellBuf.Mid( offset, iSpellLenBuf[aIndex] );
    }

inline TInt CPinyinAnalyserDbFeed::GetSpellElementCount() const
    {
    return iSpellElemLen;
    }
    
inline TUint16 CPinyinAnalyserDbFeed::GetSpellElementByIndex( TInt aIndex ) const
    {
    return iSpellElemBase + aIndex;
    }

inline TBool CPinyinAnalyserDbFeed::IsNormalElement( TUint16 aCode ) const
    {
    return aCode >= iSpellElemBase && aCode < iSpellElemBase + iSpellElemLen;
    }
    
inline TSpellType CPinyinAnalyserDbFeed::GetSpellElementType( TUint16 aCode ) const
    {
    return ( TSpellType )iSpellTypeBuf[ aCode - iSpellElemBase ];
    }
 
 inline TBool CPinyinAnalyserDbFeed::IsFirstElement( TUint16 aCode ) const
    {
    for ( TInt i = 0; i < iNoFirstElements.Length(); i++ )
        {
        if ( iNoFirstElements[i] == aCode )
            {
            return EFalse;
            }
        }
    return ETrue;
    }
