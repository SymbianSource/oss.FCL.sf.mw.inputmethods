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
* Description:            Pen input related enums
*
*/











#ifndef C_AKNFEPZHUYINANALYSERDB_H
#define C_AKNFEPZHUYINANALYSERDB_H


#include <e32base.h>

enum TPinyinAnalyserDbType
    {
    EPinyinAnalyserTypePinyin,
    EPinyinAnalyserTypeZhuyin
    };

enum TSpellType
{
    ESpellVowel,
    ESpellConsonant,
    ESpellSpecialConsonnant
};
const TUint16 KProgramSeparator = 0x002e;
/**
 *  Composition no chars ui state
 *  It provides key event handling and internal event handling in composition no chars state
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CZhuyinAnalyserDbFeed : public CBase
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiStateMgr The vkb ui state manager
     * @param aContext The vkb layout context
     * @return The pointer point to CAknFepVkbPinyinAnalyser type object
     */
    static CZhuyinAnalyserDbFeed* NewL( TPinyinAnalyserDbType aType );
        
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CZhuyinAnalyserDbFeed();
    
    /**
     * Get the Number of whole spell set.        
     *
     * @since S60 V4.0        
     */ 
    TInt GetSpellCount() const;
    
    /**
     * Get the Number of whole spell set.        
     *
     * @since S60 V4.0        
     * @param The index for the spell.
     */ 
    const TPtrC GetSpellByIndex( TInt aIndex ) const;
    
    /**
     * Get the Number of spell elements.        
     *
     * @since S60 V4.0        
     */ 
    TInt GetSpellElementCount() const;
    
    /**
     * Get the unicode number for spell element index.       
     *
     * @since S60 V4.0        
     * @param The index for the spell element.
     */ 
    TUint16 GetSpellElementByIndex( TInt aIndex ) const;
    
    TBool IsNormalElement( TUint16 aCode ) const;
    
    TSpellType GetSpellElementType( TUint16 aCode ) const;
    
    TBool IsFirstElement( TUint16 aCode ) const;
    
    TBool IsToneMark( TUint16 aCodeFirst, TUint16 aCodeSecond );
    
private:
    CZhuyinAnalyserDbFeed();
    
    void ConstructL( TPinyinAnalyserDbType aType );
    
private:
    TPtrC iSpellBuf;
    TPtrC iSpellLenBuf;
    TUint16 iSpellElemBase;
    TUint16 iSpellElemLen;
    TPtrC iSpellTypeBuf;
    TPtrC iNoFirstElements;
    };

#include "AknFepZhuyinAnalyserDb.inl"

#endif //C_AKNFEPZHUYINANALYSERDB_H
