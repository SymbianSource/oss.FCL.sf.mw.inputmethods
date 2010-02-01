/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Predective text input engine API header
*
*/



#ifndef _CSYMBOLLIST_H_HEADER_INCLUDED
#define _CSYMBOLLIST_H_HEADER_INCLUDED

// system includes
#include <badesca.h>
#include <f32file.h>
#include <s32strm.h>

// user includes
#include "PtiHwrRecognizer.h"

class CSymbol:public CBase
    {
    
    friend class CSymbolList;
    
  public:
   /*
    *Two phase constructor, create instance of implementation by
    * supplied resolution parameters.
    *
    * @param  aStream The stream which contain the data to construct a CSymbol
    * @return   CSymbol* pointer if successful, otherwise NULL.
    */ 
    static  CSymbol* NewL(RReadStream& aStream);

   /*
    *Two phase constructor, create instance of implementation by
    * supplied resolution parameters.
    *
    * @param  aText The text of a symbol
    * @param  aModel The pointer vector of a symbol
    * @param  aHelpline 
    * @param  aBaseLine
    * @param  aRange The range the symbol belongs to
    * @return   CSymbol* pointer if successful, otherwise NULL.
    */ 
    static  CSymbol* NewL(const TDesC& aText, const RArray<TPoint>& aModel, int aHelpline, int aBaseLine, const THwrUdmRange& aRange);
    
    /**
     * Destructor
     */
    ~CSymbol();
   
   /*
    * Check whether the two symbols are the same
    *
    * @param  aFirst The first symbol
    * @param  aSecond The second symbol
    * @return   Positive. if fist symbol is greater than the second symbol. 
    *                  Negative. if fist symbol is less than the second symbol. 
    *                  Zero, if both symbols have the same text and the their ranges are the same.     
    */
    static TInt SymbolOrderDescending(const CSymbol& aFirst, const CSymbol& aSecond);
    
   /*
    * Check whether the symbol match the aRange
    *
    * @param  aRange The range to be matched.
    * @return   ETrue if matched, otherwise EFalse.
    */
    TBool Match ( const THwrUdmRange& aRange );

private:
   
   /*
    * Externalize to a stream
    *
    * @param  aStream The stream to Externalize
    * @return   None.
    */
   void ExternalizeL(RWriteStream& aStream);
       
   /*
    * Internalize to a stream
    *
    * @param  aStream The stream to internalize
    * @return   None.
    */
   void InternalizeL(RReadStream& aStream);
   
    /**
     * C++ default constructor
     */
    CSymbol();
    
    /**
     * Symbian 2nd phase constructor.
     *
     * @return None
     */
    void ConstructL(const TDesC& aText, const RArray<TPoint>& aModel, int aHelpline, int aBaseLine,const THwrUdmRange& aRange);

    /**
     * Symbian 2nd phase constructor.
     *
     * @return None
     */
    void ConstructL(RReadStream& aStream);
    
private:

    int iHelpLine;

    int iBaseLine;

    HBufC* iSymbolName;
    
    // used only for preset models.
    TUint iPresetCode;

    TPoint* iPointVector;
    
    TInt iPointVectorLen;
    
    THwrUdmRange iRange;
    };

class CSymbolList :public CBase
    {
    friend class CSymbolList;
public:
   /*
    *Two phase constructor, create instance of implementation by
    * supplied resolution parameters.
    *
    * @param  aFilePath The file which contain the data to construct a CSymbol
    * @return   CSymbolList* pointer if successful, otherwise NULL.
    */ 
    static CSymbolList* NewL(const TDesC& aFilePath, CSymbolList* aPresetList );
    
    /**
     * Destructor
     */
    virtual ~CSymbolList();

    /**
     * Set character model
     *
     * @param aText Set  text for this symbol
     * @param aModel Array of points where the character's model is stored.
     * @return None
     */
    void SetSymbolModelL(const TDesC& aText, const RArray<TPoint>& aModel, TInt aHelpLine, TInt aBaseLine,const THwrUdmRange& aRange  );

    /**
     * Check if symbol exist.
     *
     * @param aText The symbol which is checked
     * @param aRange The range this symbol belongs to
     * @return TBool True means this symbol has model
     *               False means this symbol doesn't has model yet
     */
    TBool CheckSymbolModel(const TDesC& aChar,const THwrUdmRange& aRange );
  
    /**
     * Get symbol model
     *
     * @param aText Get this symbol's model
     * @param aModel Array of points where the symbol's model is stored.
     * @param aRange The range this symbol belongs to
     * @return None
     */
    void GetSymbolModelL(const TDesC& aChar, RArray<TPoint>& aModel, TUint& aUnicode, const THwrUdmRange& aRange );

    /**
     * Delete symbol model
     *
     * @param aText Delete this symbol's model
     * @param aRange The range this symbol belongs to
     * @return None
     */
    void DeleteSymbolModelL(const TDesC& aChar,const THwrUdmRange& aRange );

    /**
     * Get text list of all the symbol models
     *
     * @param aList The text list.
     * @param aRange The range this symbol belongs to
     * @return None
     */
    void GetModelTextListL(RPointerArray<HBufC>& aList,const THwrUdmRange& aRange );

    /**
     * Change the text of an existing symbol model
     *
     * @param aOldText The text of an existing symbol model
     * @param aNewText The new text of the symbol model
     * @param aRange The range this symbol belongs to
     * @return None
     */
    void ChangeSymbolTextL(const TDesC& aOldText, const TDesC& aNewText,const THwrUdmRange& aRange );
    
    /**
     * Get the index array that matched the aRange
     *
     * @param aList The index array of the matched symbols.
     * @param aRange The range this symbol belongs to
     * @return None
     */
    void GetModelIndexListL( RArray<TInt>& aList, const THwrUdmRange& aRange);
    
    /**
     * Get the symbol contents for the specified index
     *
     * @param aIndex The index array of the matched symbols.
     * @param aSymbolName The name of the symbol
     * @param aModel The stroke for the symbol
     * @return KErrNone if success;
     */    
    TInt GetSymbolDataRef(TInt aIndex, TPtrC& aSymbolName,RArray<TPoint>& aModel, TInt& aHelpLine, TInt& aBaseLine );
       
        /**
     * Save preset shortcuts.
     * 
     * @param aUnicode The assigned preset text.
     * @param aShortcut The shortcut to be save.
     */
    void SavePresetShortcutL( TUint aUnicode, const TDesC& aShortcut ) ;
    
    /**
     * Return all unicodes of preset text stored in PTI engine
     * 
     * @param aPresets All preset text's unicodes are stored in this array.     
     */
    void GetAllPresetSymbolsL( RArray<TUint>& aPresets ) ;  
    
    /**
     * Get trails assigned to a preset text.
     * 
     * @param aUnicode The unicode of the preset text.
     * @param aModel Store trails of the preset text.
     * @param aShortcut Indicate whether the preset text has been assigned to a shortcut text.
     *        If KNullDesC, it is not in use.
     */
    void GetPresetSymbolByUnicodeL( TUint aUnicode, RArray<TPoint>& aModel, TDes& aShortcut ) ;     
    
     /*
    * ExternalizeL to a file
    *
    * @return   None.
    */
    void ExternalizeL();
    
private:

    /**
     * Symbian 2nd phase constructor.
     * @param aFilePath The file which contain the symbol model
     * @return None
     */
    void ConstructL(const TDesC& aFilePath);
    
    /**
     * C++ default constructor
     */
    CSymbolList(  CSymbolList* aPresetList  );

    /**
     * Get the index for the specified aText & aRange.
     * @param aText The text of  a symbol model
     * @param aIndex To store the index of the symbol model
     * @param aRange The range this symbol belongs to 
     * @return KErrNone if finding the symbol model
     */
    TInt GetSymbolIndex(const TDesC& aText, TInt& aIndex,const THwrUdmRange& aRange );
    
   /*
    * Internalize from a file
    *
    * @param  aFile The file to internalize
    * @return   None.
    */
    void InternalizeL(const TDesC& aFile);

    
    /**
     * Call RMutex.Signal() in cleanupstack.
     *
     * @return None
     */
    static void SignalMutex( TAny* aMutex );
private:
    
    RPointerArray<CSymbol> iSymbolList;
    
    HBufC* iFilePath;
    
    RFs iRfs;
    
    RMutex iMutex;
    
    CSymbolList* iPresetModels;
    
};


#endif /* _CSYMBOLLIST_H_HEADER_INCLUDED */
