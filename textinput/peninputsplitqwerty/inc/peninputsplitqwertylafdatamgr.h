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
* Description:  L&F data manager
*
*/

#ifndef C_PENINPUTSPLITQWERTYLAFDATAMGR_H
#define C_PENINPUTSPLITQWERTYLAFDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <AknFepGlobalEnums.h>

// User includes

// Forward decalaration

/**
 * laf type definition
 */ 
enum TPeninputQwtLayoutType
    {
    ELayoutNone = -1,
    ELayout10x3,
    ELayout11x3,
    ELayout11x4	
    };

/**
 * laf option definition
 */
enum TPeninputVkbLafOption
    {
    EOptionNone,
    EOption3CellRange = 0x0001,
    EOptionWithSmily  = 0x0002
    };

/**
 * button data
 */
struct TPeninputButtonLayoutData
    {
    TRect iRect;
    TRect iInnerRect;
    };

/**
 * 3-pieces icon data
 */
struct TPeninputButtonLayoutDataEx
    {
    TRect iIconsFrameRect; // The frame rect contains all the icons
    TRect iMiddleIconRect; // Middle icon rect
    };

/**
 * virtual keypad data
 */
struct TPeninputKeyLayoutData
	{
	TRect iKaypadRect;
	RArray<TRect> iRects;
	RArray<TRect> iCharRects;
	TAknTextLineLayout iTextLayout;
	const CFont* iFont;
	};


/**
 *  class CPeninputQwtLayoutDataInfo
 *
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */   

class CPeninputQwtLayoutDataInfo : public CBase
    {
public:
    virtual ~CPeninputQwtLayoutDataInfo();

public:
    TPeninputButtonLayoutData   iCloseButton;
    TPeninputButtonLayoutData   iOptionButton;
    TPeninputButtonLayoutData   iMoveButton;
    TPeninputButtonLayoutData   iLeftButton;
    TPeninputButtonLayoutData   iRightButton;
    TPeninputButtonLayoutData   iClearButton;
    TPeninputKeyLayoutData      iKeypad;
    TPeninputButtonLayoutData   iShiftButton;
    TPeninputButtonLayoutData   iSmillyButton;
    TPeninputButtonLayoutData   iSpaceButton;
    TPeninputButtonLayoutDataEx iSpaceButtonEx;
    TPeninputButtonLayoutData   iEnterButton;
    TPeninputButtonLayoutData   iRangeButton;
    TRect iTitlebarRect;
    TRect iClientRect;
    TRect iQwtRect;
    TRect iPreviewWndRect;
    TRect iPreviewWndInnerRect;
    TAknTextLineLayout iPreviewWndText;
    };

/**
 *  interface MPeninputLafDataBase
 *
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */ 

class MPeninputLafDataBase
    {
public:
    /**
     * retrieve laf data of a button specified by aButtonID
     *
     * @since S60 v5.0
     * @param aButtonID id of button control
     * @return ref to laf data object
     */
    virtual const TPeninputButtonLayoutData& ButtonLayoutData( 
                                                  TInt aButtonID ) const = 0;

    /**
     * Retrieve laf data of keypad
     *
     * @since S60 v5.0
     * @return ref to laf data object
     */
    virtual const TPeninputKeyLayoutData& KeypadLayoutData() const = 0;

    /**
     * Retrieve laf data of qwerty layout
     *
     * @since S60 v5.0
     * @return ref to laf data object
     */
    virtual const CPeninputQwtLayoutDataInfo& QwtLayoutData() const = 0;

    /**
     * Set layout type
     *
     * @since S60 v5.0
     * @param aLayoutType layout type
     * @return none
     */
    virtual void SetLayoutType( TInt aLayoutType ) = 0;

    /**
     * Get current layout type
     *
     * @since S60 v5.0
     * @return layout type
     */
    virtual TInt LayoutType() const = 0;
    
    };

/**
 *  interface MPeninputLafDataCommon
 *
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */ 
class MPeninputLafDataCommon : public MPeninputLafDataBase
    {
public:
    /**
     * C++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~MPeninputLafDataCommon();

    /**
     * Release laf data
     *
     * @since S60 v5.0
     * @return none
     */
    void DestoryDataCommon();

    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructL();
    
public: // From base MPeninputLafDataBase
   
    /**
     * From MPeninputLafDataBase
     * Retrieve laf data of a button specified by aButtonID
     *
     * @since S60 v5.0
     * @param aButtonID id of button control
     * @return ref to laf data object
     */
    const TPeninputButtonLayoutData& ButtonLayoutData( TInt aButtonID ) const;

    /**
     * From MPeninputLafDataBase
     * Retrieve laf data of keypad
     *
     * @since S60 v5.0
     * @return ref to laf data object
     */
    const TPeninputKeyLayoutData& KeypadLayoutData() const;

    /**
     * From MPeninputLafDataBase
     * Retrieve laf data of qwerty layout
     *
     * @since S60 v5.0
     * @return ref to laf data object
     */
    const CPeninputQwtLayoutDataInfo& QwtLayoutData() const;

    /**
     * From MPeninputLafDataBase
     * Set layout type
     *
     * @since S60 v5.0
     * @param aLayoutType layout type
     * @return none
     */
    void SetLayoutType( TInt aLayoutType );

    /**
     * From MPeninputLafDataBase
     * Get current layout type
     *
     * @since S60 v5.0
     * @return layout type
     */
    TInt LayoutType() const;

public:
    /**
     * Set laf option
     *
     * @since S60 v5.0
     * @param aLafOption laf option
     * @return none
     */
    void SetLafOption( TPeninputVkbLafOption aLafOption );

    /**
     * Get current laf option
     *
     * @since S60 v5.0
     * @return laf option
     */
    TPeninputVkbLafOption LafOption() const;

    /**
     * Check whether laf data is valid or not
     *
     * @since S60 v5.0
     * @return ETrue if laf data is valid
     */
    TBool IsValid() const;

protected:

    /**
     * Read laf data
     *
     * @since S60 v5.0
     * @return none
     */
    virtual void ReadLafInfoL() = 0;

protected:
    /**
     * Layout type
     */
    TInt iLayoutType;

    /**
     * Laf option
     */
    TPeninputVkbLafOption iLafOption;

    /**
     * Laf datas
     */
    RPointerArray<CPeninputQwtLayoutDataInfo> iLayoutDataInfo;
    }; 

/**
 *  class CPeninputLafDataFSQ
 *
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */ 
class CPeninputLafDataFSQ : public CBase, public MPeninputLafDataCommon
    {
public:
    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @param aLayoutType layout type
     * @return The pointer to created object
     */
    static CPeninputLafDataFSQ* NewL( TInt aLayoutType );

    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @param aLayoutType layout type
     * @return The pointer to created object
     */
    static CPeninputLafDataFSQ* NewLC( TInt aLayoutType );

    /**
     * C++ destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputLafDataFSQ();

protected: //from base MPeninputLafDataCommon
    
    /**
     * From MPeninputLafDataCommon
     * Read all laf data
     *
     * @since S60 v5.0
     * @return none
     */
    void ReadLafInfoL();

protected:    
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aContext The layout context
     * @return None
     */
    CPeninputLafDataFSQ( TInt aLayoutType );

private:
    /**
     * Read laf data
     * 
     * @since S60 v5.0
     * @param aLayoutType layout type
     * @param aRows row count of keypad
     * @param aCols column count leypad
     * @return pointer to created laf data object
     */
    CPeninputQwtLayoutDataInfo* ReadLC( const TInt aLayoutType, 
                                        const TInt aRows, 
                                        const TInt aCols );

    /**
     * Utils to calculate relative rect base on given point
     * 
     * @since S60 v5.0
     * @param aRect source rect
     * @param aPoint base point
     * @return result rect
     */
    TRect RelativeRect(const TRect& aRect, const TPoint& aPoint );
    
    /**
     * Utils to offset rect
     * 
     * @since S60 v5.0
     * @param aRect source rect
     * @param aDx x offset 
     * @param aDy y offset 
     * @return result rect
     */
    TRect OffsetRect(const TRect& aRect, TInt aDx, TInt aDy );
    
    };


/**
 *  Peninput VKB data management class
 *  This class manager the data of VKB
 *  
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */
class CPeninputSplitQwertyLafMgr : public CBase 
    {   

public:
    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @return The pointer to created object
     */
    static CPeninputSplitQwertyLafMgr* NewL();

    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @return The pointer to created object
     */
    static CPeninputSplitQwertyLafMgr* NewLC();

    /**
     * C++ destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputSplitQwertyLafMgr();

public:
    /**
     * Set input mode and reload laf 
     *
     * @since S60 v5.0
     * @param aInputMode input mode
     * @return None
     */
    void SetInputModeL( TPluginInputMode aInputMode );
    
    /**
     * Get current input mode
     *
     * @since S60 v5.0
     * @return Input mode
     */    
    TPluginInputMode InputMode() const;

    /**
     * Set layout type
     *
     * @since S60 v5.0
     * @param aLayoutType Layout type index
     * @return None
     */
    void SetLayoutType( TPeninputQwtLayoutType aLayoutType );

    /**
     * Get layout type by row and column count
     * 
     * @since S60 v5.0
     * @return  layout type
     */
    TPeninputQwtLayoutType LayoutTypeFromRowsColumns( const TInt aRows, 
                                                      const TInt aColumns );
    
    /**
     * Set option ( and reload laf )
     *
     * @since S60 v5.0
     * @param aOption option type
     * @return None
     */
    void SetLafOption( TPeninputVkbLafOption aOption );

    /**
     * Get current option
     * 
     * @since S60 v5.0
     * @return option
     */
    TPeninputVkbLafOption OptionType() const;

    /**
     * Check whether laf data is valid or not
     * 
     * @since S60 v5.0
     * @return if laf data is valid
     */
    TBool IsValid();

    /**
     * Reload laf data
     *
     * @since S60 v5.0
     * @return None
     */
    void UpdateLafDataL();

    /**
     * Get rect of entire ui, screen coord-sys
     * 
     * @since S60 v5.0
     * @return rect of ui
     */
    const TRect& EntirePaneRect() const;

    /**
     * Get rect of entire ui, client coord-sys
     * 
     * @since S60 v5.0
     * @return rect of client
     */
    const TRect& ClientRect() const;

    /**
     * 
     * Get rect of button ctrl
     *
     * @since S60 v5.0
     * @param aButtonID button id
     * @param aOutterRect button rect
     * @param aInnerRect button forground rect
     * @return None
     */
    void GetButtonRect( TPeninputCommonCtrlID aButtonID, TRect& aOutterRect,
                                                         TRect& aInnerRect );

    /**
     * Get rect of virtual keypad ctrl
     * 
     * @since S60 v5.0
     * @return rect of keypad
     */
    const TRect& KeypadRect() const;
    
    /**
     * Get rect of virtual key ctrl
     *
     * @since S60 v5.0
     * @param aCol the colume index of the key
     * @param aRow	 the row index of the key
     * @return Key rect
     */
    void GetKeyRect( TInt aCol, TInt aRow, TRect& aKeyRect, TRect& aCharRect ) const;

    /**
     * Get text font of virtual keypad ctrl
     * 
     * @since S60 v5.0
     * @return none
     */
    const CFont* KeyTextFont() const;

    /**
     * Get text layout of virtual keypad ctrl
     * 
     * @since S60 v5.0
     * @return text layout
     */
    TAknTextLineLayout KeyTextLayout() const;

    /**
     * Get rect of preview bubble ctrl
     * 
     * @since S60 v5.0
     * @return none 
     */
    void GetPreviewWndRect( TRect& aOutRect, TRect& aInnerRect ) const;

    /**
     * Get text layout of preview bubble
     * 
     * @since S60 v5.0
     * @return text layout of preview bubble
     */
    TAknTextLineLayout PreviewWndText() const;

    /**
     * Get 3-pieces info of space button
     * 
     * @since S60 v5.0
     * @return none
     */
    void GetSpaceButtonExLayoutInfo( TRect& aIconsFrameRect,
                                     TRect& aMiddleIconRect ) const;

private:

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */
    CPeninputSplitQwertyLafMgr();
    
    /**
     * Symbian Constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructL();
    
private:
    /**
     * Laf data
     * Own
     */
    MPeninputLafDataCommon* iLafData;
    
    /**
     * Current input mode
     */
    TPluginInputMode iInputMode;
    };

#endif // C_PENINPUTSPLITQWERTYLAFDATAMGR_H
