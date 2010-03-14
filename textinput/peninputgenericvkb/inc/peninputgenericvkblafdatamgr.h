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
* Description:  peninput vkb laf manager
*
*/


#ifndef C_CPENINPUTGENERICLAFDATAMGR_H
#define C_CPENINPUTGENERICLAFDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <AknFepGlobalEnums.h>
// User includes

// Forward decalaration

// laf type definition
enum TPeninputQwtLayoutType
	{
	ELayoutNone = -1,
	ELayout10x3,
	ELayout11x3,
	ELayout11x4	
	};

enum TPeninputVkbLafOption
	{
	EOptionNone,					// Default option setting	
	EOption3CellRange = 0x0001,
	EOptionWithSmily  = 0x0002
	};

struct TPeninputButtonLayoutData
	{
	TRect iRect;
	TRect iInnerRect;
	};

// with 3 pieces icon as it's graphics
struct TPeninputButtonLayoutDataEx
	{
	TRect iIconsFrameRect;			// The frame rect contains all the icons
	TRect iMiddleIconRect;			// Middle icon rect
	};	
	
struct TPeninputKeyLayoutData
	{
	TRect iKaypadRect;
	RArray<TRect> iRects;
	RArray<TRect> iCharRects;
	TAknTextLineLayout iTextLayout;
	const CFont* iFont;
	};

struct TPeninputEntryLayoutData
	{
	TRect iRect;
	TRect iPromptTextRect;
	TRect iTextRect;
	const CFont* iPromptTextFont;
	const CFont* iTextFont;
	};

struct TPeninputRangeBarLayoutData
	{
	TRect iRect;
	RArray<TPeninputButtonLayoutData> iCellDataArray;
	};

struct TPeninputICFConfig
	{
	TInt iLeftMargin;
	TInt iRightMargin;
	TInt iTopMargin;
	TInt iTextLineHeight;
	TInt iLineSpaceMargin;
	};
	
/*
* Store LAF data for candidate list
*/
struct TPeninputCandidateListLayoutData
    {
    TSize iSelectionItemSize;    
    TInt iSelectionHorMargin;
    TInt iSelectionVerMargin;
    TSize iSelectionNaviSize;
    TSize iSelectionNaviInnerSize;
    TAknTextLineLayout iSelectionTextLine;
    TAknTextLineLayout iSelectionPageTextLine;
    const CFont* iFont;
    };

/*
* Stroe LAF data for tooltip box
*/
struct TPeninputTooltipBoxLayoutData
    {
    TRect iRect;
    TAknTextLineLayout iTooltipTextLayout;
    const CFont* iFont;       
    };
    
class CPeninputQwtLayoutDataInfo : public CBase
	{
public:
	virtual ~CPeninputQwtLayoutDataInfo();
	TPeninputButtonLayoutData 	iCloseButton;
	TPeninputButtonLayoutData 	iOptionButton;
	TPeninputEntryLayoutData  	iICF;
	TPeninputButtonLayoutData	iMoveButton;	
	TPeninputButtonLayoutData	iLeftButton;
	TPeninputButtonLayoutData	iRightButton;
	TPeninputButtonLayoutData	iClearButton;
	TPeninputKeyLayoutData		iKeypad;
	TPeninputButtonLayoutData 	iShiftButton;	
	TPeninputRangeBarLayoutData	iRangbar;
	TPeninputButtonLayoutData 	iSmillyButton;
	TPeninputButtonLayoutData	iSpaceButton;
	TPeninputButtonLayoutDataEx iSpaceButtonEx;	
	TPeninputButtonLayoutData	iEnterButton;
	TPeninputButtonLayoutData	iSwitchButton;
	TRect iTitlebarRect;
	TRect iClientRect;
	TRect iQwtRect;
	TRect iPreviewWndRect;
	TRect iPreviewWndInnerRect;
	TAknTextLineLayout iPreviewWndText;
	
	/*
	* LAF data for candidate list
	*/
	TPeninputCandidateListLayoutData iCandidateList;
	
	/*
	* LAF data for tooltip box
	*/
	TPeninputTooltipBoxLayoutData iTooltipBox;
	};

class MPeninputLafDataBase
	{
public:
	virtual const TPeninputButtonLayoutData& ButtonLayoutData( TInt aButtonID ) const = 0;
	virtual const TPeninputEntryLayoutData& ICFLayoutData() const = 0;
	virtual const TPeninputKeyLayoutData& KeypadLayoutData() const = 0;
	virtual const TPeninputRangeBarLayoutData& RangbarLayoutData() const = 0;
	
	virtual const CPeninputQwtLayoutDataInfo& QwtLayoutData() const = 0;
	virtual void SetLayoutType( TInt aLayoutType ) = 0;
	virtual TInt LayoutType() const = 0;
	};
	
class MPeninputLafDataCommon : public MPeninputLafDataBase
	{
public:
    virtual ~MPeninputLafDataCommon();
	void  DestoryDataCommon();
	const TPeninputButtonLayoutData& ButtonLayoutData( TInt aButtonID ) const;
	const TPeninputEntryLayoutData& ICFLayoutData() const ;
	const TPeninputKeyLayoutData& KeypadLayoutData() const ;
	const TPeninputRangeBarLayoutData& RangbarLayoutData() const;
	const CPeninputQwtLayoutDataInfo& QwtLayoutData() const;
	
	/*
	* Return layout data for candidate list;
	*
	* @return The layout data for candidate list
	*/	
	const TPeninputCandidateListLayoutData& CandidateListLayoutData() const;
	
	/*
	* Return layout data for tooltip box;
	*
	* @return The layout data for tooltip box
	*/	
	const TPeninputTooltipBoxLayoutData& TooltipBoxLayoutData() const;
	
	inline void SetLayoutType( TInt aLayoutType );
	inline TInt LayoutType() const;
	inline void SetLafOption( TPeninputVkbLafOption aLafOption );
	inline TPeninputVkbLafOption LafOption() const;
	void ConstructL();
	TBool IsValid() const;
protected:
	void GetTopAndBottomPaneInfo( const TRect& aParentWndRect, 
								  TInt aLayoutType,
								  TInt aLayoutParam,
								  CPeninputQwtLayoutDataInfo& aDataInfo );
	virtual void ReadLafInfoL() = 0;
	
	TInt iLayoutType;
	TPeninputVkbLafOption iLafOption;
	RPointerArray<CPeninputQwtLayoutDataInfo> iLayoutDataInfo;
	}; 
	
class CPeninputLafDataVKB : public CBase, 
							public MPeninputLafDataCommon
					
	{
public:	
    static CPeninputLafDataVKB* NewL( TInt aLayoutType );
	static CPeninputLafDataVKB* NewLC( TInt aLayoutType );	
	virtual ~CPeninputLafDataVKB();
	
protected:
	CPeninputLafDataVKB( TInt aLayoutType );

	void ReadLafInfoL();
	};
	
class CPeninputLafDataFSQ : public CBase, 
							public MPeninputLafDataCommon

	{
public:	
    static CPeninputLafDataFSQ* NewL( TInt aLayoutType );
	static CPeninputLafDataFSQ* NewLC( TInt aLayoutType );
	virtual ~CPeninputLafDataFSQ();

protected:
	CPeninputLafDataFSQ( TInt aLayoutType );

	void ReadLafInfoL();
	
private:

    /**
     * Read LAF data for candidate list 
     *  
     * @param aRect the parent window's rect
     * @return candidate list layout data
     */	
	TPeninputCandidateListLayoutData ReadLafForCandidateList( const TRect& aRect );

    /**
     * Read LAF data for tooltip box
     *  
     * @param aRect the parent window's rect
     * @return tooltip box layout data
     */	
	TPeninputTooltipBoxLayoutData ReadLafForTooltipBox( const TRect& aRect );
	};	
	
/**
 *  Peninput VKB data management class
 *  This class manager the data of VKB
 *  
 *  @lib peninputgenericvkb.lib
 *  @since S60 v3.2
 */
class CPeninputGenericVkbLafMgr : public CBase 
    {   

public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputGenericVkbLafMgr object
     */
    static CPeninputGenericVkbLafMgr* NewL();

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputGenericVkbLafMgr object
     */
    static CPeninputGenericVkbLafMgr* NewLC();

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputGenericVkbLafMgr();

  	/**
     * ConstructL
     *
     * @since S60 v3.2
     * @return None
     */
	void ConstructL();
	
	/**
     * Set layout type
     *
     * @since S60 v3.2
     * @param aLayoutType Layout type index
     * @return None
     */
    void SetInputModeL( TPluginInputMode aInputMode );
    
	/**
     * Get layout type
     *
     * @since S60 v3.2
     * @return Input mode
     */    
	TPluginInputMode InputMode() const;
	
	/**
     * Set layout type
     *
     * @since S60 v3.2
     * @param aLayoutType Layout type index
     * @return None
     */
	void SetLayoutType( TPeninputQwtLayoutType aLayoutType );
	
	/**
     * Set option ( option change will cause reloading laf )
     *
     * @since S60 v3.2
     * @param aOption option type
     * @return None
     */
     void SetLafOption( TPeninputVkbLafOption aOption );
     TPeninputVkbLafOption OptionType() const;
     
	/**
     * Get button laf data
     *
     * @since S60 v3.2
     * @param aButtonID 	button id
     * @param aOutterRect	button rect
     * @param aInnerRect	button forground rect
     * @return None
     */	
	void GetButtonRect( TPeninputCommonCtrlID aButtonID, 
						TRect& aOutterRect,
						TRect& aInnerRect );
	
	/**
     * reload laf data
     *
     * @since S60 v3.2
     * @return None
     */
	inline void UpdateLafDataL();
	
	/**
     * Get key laf data
     *
     * @since S60 v3.2
     * @param aColume 		the colume index of the key
     * @param aRow			the row index of the key
     * @return Key rect
     */								
	void GetKeyRect( TInt aColume, TInt aRow, TRect& aKeyRect, TRect& aCharRect ) const;
	const CFont* KeyTextFont() const;
	const TRect& KeypadRect() const;
	const TRect& ICFRect() const;
	const CFont* ICFPromptTextFont() const;
	const CFont* ICFTextFont() const;
	TAknTextLineLayout KeyTextLayout() const;
	void GetICFConfig( TPeninputICFConfig& aICFConfig );
	const TRect& EntirePaneRect() const;
	const TRect& RangbarRect() const;
	TInt RangbarCellCount() const;
	void RangbarCellAt( TInt aIndex, TRect& aOutterRect, TRect& aInnerRect );
	
	TPeninputQwtLayoutType LayoutTypeFromRowsColumns(const TInt aRows, const TInt aColumns);
	const TRect& TitlebarRect() const;
	
	TBool IsValid();
	const TRect& ClientRect() const;
	
	inline void GetPreviewWndRect( TRect& aOutRect, TRect& aInnerRect ) const;
	inline TAknTextLineLayout PreviewWndText() const;
	void GetSpaceButtonExLayoutInfo( TRect& aIconsFrameRect, TRect& aMiddleIconRect ) const;
	
	/**
     * Return layout data for candidate list
     *
     * 
     * @return The layout data for candidate list
     */
	const TPeninputCandidateListLayoutData& CandidateListLayoutData() const;
	
	/*
	* Return layout data for candidate list
	*
	* @return The layout data for tooltip box	
	*/	
	const TPeninputTooltipBoxLayoutData& TooltipBoxLayoutData() const;

protected:

	/**
     * Set laf data base type
     *
     * @since S60 v3.2
     * @param aLafData The exact laf data
     * @return None
     */
	void SetLafData( MPeninputLafDataBase* aLafData );
	
    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return None
     */
    CPeninputGenericVkbLafMgr();
    
private:
	MPeninputLafDataCommon* iLafData;
	TPluginInputMode iInputMode;
    };
   
inline void CPeninputGenericVkbLafMgr::UpdateLafDataL()
	{
	iLafData->ConstructL();	
	}
	
inline void CPeninputGenericVkbLafMgr::GetPreviewWndRect( TRect& aOutRect, 
                                                          TRect& aInnerRect ) const
	{
	aOutRect = iLafData->QwtLayoutData().iPreviewWndRect;
	aInnerRect = iLafData->QwtLayoutData().iPreviewWndInnerRect;
	}
	
inline TAknTextLineLayout CPeninputGenericVkbLafMgr::PreviewWndText() const
	{
	return iLafData->QwtLayoutData().iPreviewWndText;
	}
	
#endif // C_CPENINPUTGENERICLAFDATAMGR_H
