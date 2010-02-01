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
* Description:  vkb layout inline function 
*
*/


#ifndef C_CAKNFEPCTRLVKBLAYOUT_H
#define C_CAKNFEPCTRLVKBLAYOUT_H

// system includes
#include <barsread.h>
#include <peninputvkbctrlext.h>

// forward declarations
class CPeninputVkbLayoutInfo;
class CPeninputVkbCtrlExt;

// class declarations
/**
 *  Basic functionality for graphic buttons
 *  It provides creation from resource and can react according to SizeChanged Msg
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlVkbLayout : public CBase
    {
public: 
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlButton type object
     */
    IMPORT_C static CAknFepCtrlVkbLayout* NewL();
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlButton type object
     */
    IMPORT_C static CAknFepCtrlVkbLayout* NewLC();

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    IMPORT_C virtual ~CAknFepCtrlVkbLayout();

    /**
     * This function creates the input range button from resource,
     * before calling this function, caller should set resource id of 
     * range button first
     * 
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void ConstructFromResourceL();
    
    /**
     * This function get the vkb layout info list
     *
     * @since S60 v3.2
     * @return The vkb layout info list
     */
    IMPORT_C RPointerArray<CPeninputVkbLayoutInfo> VkbLayoutInfoList();
      
    /**
     * Get virtual keyboard
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputVkbCtrlExt object
     */     
    IMPORT_C void SetVkbKeyboard( CPeninputVkbCtrlExt* aVkbCtrl );

    /**
     * This function get the vkb layout ID
     *
     * @since S60 v3.2
     * @return The vkb layout ID
     */
    IMPORT_C void ChangeVkbLayout( TInt aVkbLayoutId );
     
    /**
     * Set resource Id
     *
     * @since S60 V4.0
     * @param aResId The resource id;
     */                
    IMPORT_C void SetResourceId(TInt32 aResId);    
    
    /**
     * Set resource Id
     *
     * @since S60 V4.0
     * @param aResId The resource id;
     */                
    IMPORT_C void SetIrregularResourceId(TInt32 aResId);
    
    /**
     * Set resource Id
     *
     * @since S60 V4.0
     * @param aResId The resource id;
     */                
    IMPORT_C void SetNonIrregularResourceId(TInt32 aResId);

    /**
     * This function creates the input range button from resource,
     * before calling this function, caller should set resource id of 
     * range button first
     * 
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void ConstructFromIrregularResourceL();            
    
    /**
     * This function creates the input range button from resource,
     * before calling this function, caller should set resource id of 
     * range button first
     * 
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void ConstructFromNonIrregularResourceL();   
          
    /**
     * Reset
     *
     * @since S60 V4.0
     */                
    IMPORT_C void Reset();  
    
protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     */
    CAknFepCtrlVkbLayout();
    
    void ConstructL();
    
private:

    void CreateIrregularKeyBitmapsL(TResourceReader& reader, 
                                   TPtrC& bmpFileName, 
                                   TVirtualKeyIrregularKeyType aKeyType);
                                   
    void CreateIrregularKeyBitmapL(TResourceReader& reader, TPtrC& bmpFileName, 
                                  TVirtualKeyIrregularKeyType aKeyType ,
                                  TVirtualKeyIrregularKeyState aKeyState, 
                                  TVirtualKeyIrregularKeyState aKeyStateMask);        

protected:

    RPointerArray<CPeninputVkbLayoutInfo> iVkbLayoutInfoList;
    
    /**
     * The first time construct flag of button
     */
    TBool iFirstTimeConstruct;
    
    /**
     * The resource Id associatied with this control
     */
    TInt32 iResourceId;
    
    /**
     * The resource Id associatied with this control
     */
    TInt32 iIrregularResId;    
	
	/**
     * The resource Id associatied with this control
     */
    TInt32 iNonIrregularResId;
private:
        
    /**
     * The vkb extenting control for vkb area
     * Not own
     */
    CPeninputVkbCtrlExt* iVkbCtrl;
    
    /**
     * The vkb layout ID
     */
    TInt iVkbLayoutId;
    };    



class CVirtualKey;

/**
 *  Vkb key info definition
 *
 *  This class is to define key info. Application can create it from  
 *  resource file. Key info includes Unicode, begin row, end row,  
 *  begin column and end column. 
 *
 *  @lib peninputvkb.lib
 *  @since S60 v3.2
 */
class CPeninputVkbKeyInfo : public CBase
    {

public:

    /**
     * Two-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return The pointer to CPeninputVkbKeyInfo object
     */
    IMPORT_C static CPeninputVkbKeyInfo* NewL( TResourceReader& aReader );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */     
    IMPORT_C virtual ~CPeninputVkbKeyInfo();

    /**
     * Get unicode logical string
     *
     * @since S60 v3.2
     * @return unicode logical string
     */ 
    inline HBufC* Unicode() const;  

    /**
     * Get key's scan code
     *
     * @since S60 v3.2
     * @return Key's scan code
     */ 
    inline TInt ScanCode() const;  
    
    /**
     * Get key's begin row
     *
     * @since S60 v3.2
     * @return Key's begin row
     */       
    inline TInt BeginRow() const;
    
    /**
     * Get key's begin column
     *
     * @since S60 v3.2
     * @return Key's begin column
     */       
    inline TInt BeginColumn() const;
 
    /**
     * Get key's end row
     *
     * @since S60 v3.2
     * @return Key's end row
     */       
    inline TInt EndRow() const;

    /**
     * Get key's end column
     *
     * @since S60 v3.2
     * @return Key's end column
     */       
    inline TInt EndColumn() const;

    /**
     * Get high light bitmap index
     *
     * @since S60 v3.2
     * @return High light bitmap index
     */     
    inline TInt HighlightBmpIndex()const;

    /**
     * Get high light mask bitmap index
     *
     * @since S60 v3.2
     * @return High light mask bitmap index
     */     
    inline TInt HighlightMaskBmpIndex() const;    
    
    /**
     * Get virtual key
     *
     * @since S60 v3.2
     * @return The pointer to CVirtualKey object
     */     
    inline CVirtualKey* Key() const;
    
    void SetIrregularKeyType(TInt aColumns);

protected:

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return None
     */
    void ConstructL( TResourceReader& aReader );

    /**
     * Contruct from resource
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return None
     */    
    void ConstructFromResourceL( TResourceReader& aReader );

private: 

    /**
     * Create key
     *
     * @since S60 v3.2
     * @return None
     */     
    void CreateKeyL();
    
    TVirtualKeyIrregularKeyType KeyTypeFromColumn(TInt aColumns);
    
private: // data

    /**
     * Key unicode string
     */ 
    HBufC* iUnicode;
    
    /**
     * Scan code 
     */     
    TInt iScanCode;
    
    /**
     * Begin row
     */     
    TInt iBeginRow;

    /**
     * Begin column
     */     
    TInt iBeginColumn;
    
    /**
     * End row
     */     
    TInt iEndRow;
    
    /**
     * End column
     */     
    TInt iEndColumn;
    
    /**
     * High light bitmap index
     */     
    TInt iHighlightBmpIndex;
    
    /**
     * High light mask bitmap index
     */     
    TInt iHighlightMaskBmpIndex;

    /**
     * Virtual key 
     * Own
     */    
    CVirtualKey* iKey;
    
    };

/**
 *  vkb layout definition
 *
 *  This class is to implement vkb layout. Application can create it from resource 
 *  file. This class is to control vkb interface layout. 
 *
 *  @lib peninputvkb.lib
 *  @since S60 v3.2
 */   
class CPeninputVkbLayoutInfo : public CBase
    {

public:

    /**
     * Two-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return The pointer to CPeninputVkbLayoutInfo object
     */
    IMPORT_C static CPeninputVkbLayoutInfo* NewL( 
                                        TResourceReader& aReader );

    /**
     * Two-phase constructor
     * 
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return The pointer to CPeninputVkbLayoutInfo object
     */
    IMPORT_C static CPeninputVkbLayoutInfo* NewLC( 
                                      TResourceReader& aReader );

    /**
     * Get real size from vkb layout
     *
     * @since S60 v3.2
     * @param aUnitWidth Unit width    
     * @param aUnitHeight Unit height
     * @return The unit size
     */
    IMPORT_C const TSize GetRealSize( const TInt aUnitWidth, 
                                      const TInt aUnitHeight ) const;
 
    /**
     * Reset unit size 
     *
     * @since S60 v3.2
     * @param aUnitWidth Unit width
     * @param aUnitHeight Unit height
     * @param aUnitCharWidth Unit char width
     * @param aUnitCharHeight Unit char height
     * @param aCharLeftTopPoint Left top point of unit char
     * @return None
     */   
    IMPORT_C void ResetSize( const TInt aUnitWidth,
                             const TInt aUnitHeight,
                             const TInt aUnitCharWidth,
                             const TInt aUnitCharHeight,
                             const TPoint& aCharLeftTopPoint,
                             const CFont* aFont );
                             
                             
    /**
     * Reset unit size 
     *
     * @since S60 v3.2
     * @param aSize Keyboard size
     * @return None
     */   
    IMPORT_C void ResetLayoutInfo( const TSize& aSize);                             



    /**
     * Reset unit size 
     *
     * @since S60 v3.2
     * @param aUnitWidth Unit width
     * @param aUnitHeight Unit height
     * @param aUnitCharWidth Unit char width
     * @param aUnitCharHeight Unit char height
     * @param aCharLeftTopPoint Left top point of unit char
     * @return None
     */   
    IMPORT_C void SetKeyInfo(const TInt aRow,
                             const TInt aColumn,
                             const TRect& aKeyRect,
                             const TRect& aCharRect, 
                             const CFont* aFont );                             




    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputVkbLayoutInfo();
            
    /**
     * Find the key from the vkb accroding to the unicode
     *
     * @since S60 v3.2
     * @param aUnicode Unicode  
     * @return The pointer to CPeninputVkbKeyInfo object
     */    
    IMPORT_C CPeninputVkbKeyInfo* FindKey( const TInt aUnicode ) const;

    /**
     * Get layout id
     *
     * @since S60 v3.2
     * @return Layout id
     */
    inline TInt LayoutID() const;

    /**
     * Get column count 
     *
     * @since S60 v3.2
     * @return Column count
     */
    inline TInt Columns() const;

    /**
     * Get row count
     *
     * @since S60 v3.2
     * @return Row count 
     */
    inline TInt Rows() const;

    /**
     * Get the unit's width
     *
     * @since S60 v3.2
     * @return The unit's width 
     */
    inline TInt UnitWidth() const;

    /**
     * Get the unit's height
     *
     * @since S60 v3.2
     * @return The unit's height 
     */
    inline TInt UnitHeight() const;

    /**
     * Get the char's width
     *
     * @since S60 v3.2
     * @return The char's width 
     */
    inline TInt UnitCharWidth() const;

    /**
     * Get the char's height
     *
     * @since S60 v3.2
     * @return The char's height 
     */
    inline TInt UnitCharHeight() const;

    /**
     * Get the char's left top point
     *
     * @since S60 v3.2
     * @return The char's left top point 
     */
    inline TPoint CharLeftTopPoint() const;

    /**
     * Get the char's font
     *
     * @since S60 v3.2
     * @return The char's font 
     */
    inline const CFont* CharFont() const;
                    
    /**
     * Get background bitmap
     *
     * @since S60 v3.2
     * @return The pointer to background bitmap
     */
    inline CFbsBitmap* BgBmp() const;

    /**
     * Get mask bitmap
     *
     * @since S60 v3.2
     * @return The pointer to background mask bitmap
     */
    inline CFbsBitmap* BgMaskBmp() const;

    /**
     * Get dim bitmap
     *
     * @since S60 v3.2
     * @return The pointer to dim bitmap
     */
    inline CFbsBitmap* DimBmp() const;

    /**
     * Get dim mask bitmap
     *
     * @since S60 v3.2
     * @return The pointer to dim mask bitmap
     */
    inline CFbsBitmap* DimMaskBmp() const;

    /**
     * Get key info array 
     *
     * @since S60 v3.2
     * @return Key info array
     */
    inline RPointerArray<CPeninputVkbKeyInfo> KeyInfoList() const;
    
    /**
     * Get high light bitmap array
     *
     * @since S60 v3.2
     * @return High light bitmap array
     */
    inline RPointerArray<CFbsBitmap> HighlightBmps() const;

    /**
     * Get high light mask bitmap array
     *
     * @since S60 v3.2
     * @return High light mask bitmap array
     */
    inline RPointerArray<CFbsBitmap> HighlightMaskBmps() const;

    /**
     * Move key to certain point
     *
     * @since S60 v3.2
     * @param aTopLeft Point to move  
     * @return None
     */
    IMPORT_C void MoveKeys( const TPoint& aTopLeft );
    
    /**
     * Construct from resourcr file
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return None
     */ 
    void ConstructFromResourceL( TResourceReader& aReader, 
                                 CPeninputVkbCtrlExt* aVkbCtrl = NULL, 
                                 TInt aVkbLayoutId = 0 );
    
protected:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */  
    CPeninputVkbLayoutInfo();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return None
     */ 
    void ConstructL( TResourceReader& aReader );      
   
public:
    TBool iIrregular;     
                
private: // data

    /**
     * Layout id
     */  
    TInt iLayoutID;

    /**
     * Column count
     */  
    TInt iColumns;

    /**
     * Row count 
     */  
    TInt iRows;
    
    /**
     * Unit width 
     */  
    TInt iUnitWidth;
    
    /**
     * Unit height 
     */  
    TInt iUnitHeight;
 
    /**
     * Unit char width
     */  
    TInt iUnitCharWidth;
 
    /**
     * Unit char height  
     */  
    TInt iUnitCharHeight;
    
    /**
     * Char left top point  
     */  
    TPoint iCharLeftTopPoint;
    
    /**
     * Key font  
     */  
    const CFont* iFont;

    /**
     * Last point
     */  
    TPoint iLastTopLeft;

    /**
     * Background bitmap
     * Own
     */  
    CFbsBitmap* iBgBmp;

    /**
     * Background mask bitmap
     * Own
     */  
    CFbsBitmap* iBgMaskBmp;

    /**
     * Dim bitmap
     * Own
     */  
    CFbsBitmap* iDimBmp;

    /**
     * Dim mask bitmap
     * Own
     */  
    CFbsBitmap* iDimMaskBmp;

    /**
     * Key info array
     */  
    RPointerArray<CPeninputVkbKeyInfo> iKeyInfoList;
    
    /**
     * High light bitmap array
     */  
    RPointerArray<CFbsBitmap> iHighlightBmps;

    /**
     * High light mask bitmap array
     */  
    RPointerArray<CFbsBitmap> iHighlightMaskBmps;
    
    };
 
#include "peninputvkbctrllayout.inl"

#endif // C_CAKNFEPCTRLVKBLAYOUT_H
            
// End Of File
