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


#ifndef C_PENINPUTVKBCNLAFDATAMGR_H
#define C_PENINPUTVKBCNLAFDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <AknFepGlobalEnums.h>
// User includes

// Forward decalaration

// laf type definition
enum TPeninputCnQwtLayoutType
	{
	ELayoutNone = -1,
	ELayout10x3,
	ELayout11x3,
	ELayout11x4,
	ELayout10x3_withCandi,
	ELayout10x4
	};

enum TPeninputCnCellType
	{
	EVkbCangjieAndPinying,			// 10
	EVkbStroke,						// 10
	EVkbZhuyin,		 				// 11 
	EVkbLatin_Zhuyin,				// 11
	EVkbLatin,						// 10
	EVkbAccent,						
	EVkbNumber,						// 10
	EVkbNumber_Zhuyin,				// 11
	EVkbCangjieAndPinying_FSQ,		// 10
	EVkbStroke_FSQ,					// 10
	EVkbZhuyin_FSQ,					// 11
	EVkbLatin_FSQ,					// 10
	EVkbNumber_FSQ,					// 10
	EVkbLatin_Zhuyin_FSQ,			// 11
	EVkbNumber_Zhuyin_FSQ			// 11
	};
	
enum TPeninputCnVkbLafOption
	{
	EOptionNone,					// Default option setting	
	EOption3CellRange = 0x0001,
	EOptionWithSmily  = 0x0002
	};

struct TPeninputCnButtonLayoutData
	{
	TRect iRect;
	TRect iInnerRect;
	};
	
struct TPeninputCnKeyLayoutData
	{
	TRect iKaypadRect;
	RArray<TRect> iRects;
	RArray<TRect> iCharRects;
	TAknTextLineLayout iTextLayout;
	const CFont* iFont;
	};

struct TPeninputCnEntryLayoutData
	{
	TRect iRect;
	TRect iPromptTextRect;
	TRect iTextRect;
	const CFont* iPromptTextFont;
	const CFont* iTextFont;
	};
	
struct TPeninputCnICFConfig
	{
	TInt iLeftMargin;
	TInt iRightMargin;
	TInt iTopMargin;
	TInt iTextLineHeight;
	TInt iLineSpaceMargin;
	};


struct TPeninputCnRangeBarLayoutData
	{
	TRect iRect;
	RArray<TPeninputCnButtonLayoutData> iCellDataArray;
	};
	
struct TPeninputCnSpaceIconLayoutDataEx
	{
	TSize iIconOffset;		// The offset to the left and top border of frame
	TInt  iSideIconWidth; 	// The width of two sizes'subicon
	};

/*
* Store LAF data for candidate list
*/
struct TPeninputCnCandidateListLayoutData
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
struct TPeninputCnTooltipBoxLayoutData
    {
    TRect iRect;
    TAknTextLineLayout iTooltipTextLayout;
    const CFont* iFont;
    };

class CPeninputCnQwtLayoutDataInfo : public CBase
	{
public:
	virtual ~CPeninputCnQwtLayoutDataInfo();
	TPeninputCnButtonLayoutData 	iCloseButton;
	TPeninputCnButtonLayoutData 	iOptionButton;
	TPeninputCnEntryLayoutData  	iICF;
	TPeninputCnButtonLayoutData	iMoveButton;	
	TPeninputCnButtonLayoutData	iLeftButton;
	TPeninputCnButtonLayoutData	iRightButton;
	TPeninputCnButtonLayoutData	iClearButton;
	TPeninputCnKeyLayoutData		iKeypad;
	TPeninputCnButtonLayoutData 	iShiftButton;	
	TPeninputCnRangeBarLayoutData	iRangbar;
	TPeninputCnButtonLayoutData 	iSmillyButton;
	TPeninputCnButtonLayoutData	iSpaceButton;
	TPeninputCnButtonLayoutData	iEnterButton;
	TPeninputCnButtonLayoutData	iSwitchButton;
	TRect iTitlebarRect;
	TRect iClientRect;
	TRect iQwtRect;
	TRect iPreviewWndRect;
	TRect iPreviewWndInnerRect;
	TAknTextLineLayout iPreviewWndText;
	CFont* iPreviewFont;
	TPeninputCnSpaceIconLayoutDataEx iSpaceIconLayout;
	
	/*
	* LAF data for candidate list
	*/
	TPeninputCnCandidateListLayoutData iCandidateList;
	
	/*
	* LAF data for tooltip box
	*/
	TPeninputCnTooltipBoxLayoutData iTooltipBox;
    
    /*
	* LAF data for composition area
	*/
	TRect iCompositionRect;
	};	

class MPeninputCnLafDataBase
	{
public:
	virtual const TPeninputCnButtonLayoutData& ButtonLayoutData( TInt aButtonID ) const = 0;
	virtual const TPeninputCnEntryLayoutData& ICFLayoutData() const = 0;
	virtual const TPeninputCnKeyLayoutData& KeypadLayoutData() const = 0;
	virtual const TPeninputCnRangeBarLayoutData& RangbarLayoutData() const = 0;
	
	virtual const CPeninputCnQwtLayoutDataInfo& QwtLayoutData() const = 0;
	virtual void SetLayoutType( TInt aLayoutType ) = 0;
	virtual TInt LayoutType() const = 0;
	};
	
class MPeninputCnLafDataCommon : public MPeninputCnLafDataBase
	{
public:
    virtual ~MPeninputCnLafDataCommon();
	void  DestoryDataCommon();
	const TPeninputCnButtonLayoutData& ButtonLayoutData( TInt aButtonID ) const;
	const TPeninputCnEntryLayoutData& ICFLayoutData() const ;
	const TPeninputCnKeyLayoutData& KeypadLayoutData() const ;
	const TPeninputCnRangeBarLayoutData& RangbarLayoutData() const;
	const CPeninputCnQwtLayoutDataInfo& QwtLayoutData() const;
    
    /*
	* Get layout rect for composition area
	*
	* @param none 
	* @return rect of composition area
	*/
	const TRect& compositionLayoutData() const;
	
	/*
	* Return layout data for candidate list;
	*
	* @return The layout data for candidate list
	*/	
	const TPeninputCnCandidateListLayoutData& CandidateListLayoutData() const;
	
	/*
	* Return layout data for tooltip box;
	*
	* @return The layout data for tooltip box
	*/	
	const TPeninputCnTooltipBoxLayoutData& TooltipBoxLayoutData() const;
	
	inline void SetLayoutType( TInt aLayoutType );
	inline TInt LayoutType() const;
	inline void SetLafOption( TPeninputCnVkbLafOption aLafOption );
	inline TPeninputCnVkbLafOption LafOption() const;
	void ConstructL();
	TBool IsValid() const;
protected:
	void GetTopAndBottomPaneInfo( const TRect& aParentWndRect, 
								  TInt aLayoutType,
								  TInt aLayoutParam,
								  CPeninputCnQwtLayoutDataInfo& aDataInfo );
	virtual void ReadLafInfoL() = 0;
	
	TInt iLayoutType;
	TPeninputCnVkbLafOption iLafOption;
	RPointerArray<CPeninputCnQwtLayoutDataInfo> iLayoutDataInfo;
	}; 
	
class CPeninputCnLafDataVKB : public CBase, 
							public MPeninputCnLafDataCommon
					
	{
public:	
    static CPeninputCnLafDataVKB* NewL( TInt aLayoutType );
	static CPeninputCnLafDataVKB* NewLC( TInt aLayoutType );	
	virtual ~CPeninputCnLafDataVKB();
	
protected:
	CPeninputCnLafDataVKB( TInt aLayoutType );

	void ReadLafInfoL();
	};
	
class CPeninputCnLafDataFSQ : public CBase, 
							public MPeninputCnLafDataCommon

	{
public:	
    static CPeninputCnLafDataFSQ* NewL( TInt aLayoutType );
	static CPeninputCnLafDataFSQ* NewLC( TInt aLayoutType );
	virtual ~CPeninputCnLafDataFSQ();

protected:
	CPeninputCnLafDataFSQ( TInt aLayoutType );

	void ReadLafInfoL();
	
private:

    /**
     * Read LAF data for candidate list 
     *  
     * @param aRect the parent window's rect
     * @return candidate list layout data
     */	
	TPeninputCnCandidateListLayoutData ReadLafForCandidateList( const TRect& aRect );

    /**
     * Read LAF data for tooltip box
     *  
     * @param aRect the parent window's rect
     * @return tooltip box layout data
     */	
	TPeninputCnTooltipBoxLayoutData ReadLafForTooltipBox( const TRect& aRect );
	};	
	
/**
 *  Peninput VKB data management class
 *  This class manager the data of VKB
 *  
 *  @lib peninputgenericvkb.lib
 *  @since S60 v3.2
 */
class CPeninputCnGenericVkbLafMgr : public CBase 
    {   

public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputCnGenericVkbLafMgr object
     */
    static CPeninputCnGenericVkbLafMgr* NewL();

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputCnGenericVkbLafMgr object
     */
    static CPeninputCnGenericVkbLafMgr* NewLC();

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputCnGenericVkbLafMgr();

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
	void SetLayoutType( TPeninputCnQwtLayoutType aLayoutType );
	
	/**
     * Set option ( option change will cause reloading laf )
     *
     * @since S60 v3.2
     * @param aOption option type
     * @return None
     */
     void SetLafOption( TPeninputCnVkbLafOption aOption );
     TPeninputCnVkbLafOption OptionType() const;
     
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
	void GetICFConfig( TPeninputCnICFConfig& aICFConfig );
	const TRect& EntirePaneRect() const;
	const TRect& RangbarRect() const;
	TInt RangbarCellCount() const;
	void RangbarCellAt( TInt aIndex, TRect& aOutterRect, TRect& aInnerRect );
	
	TPeninputCnQwtLayoutType LayoutTypeFromRowsColumns(const TInt aRows, const TInt aColumns);
	const TRect& TitlebarRect() const;
	
	TBool IsValid();
	const TRect& ClientRect() const;
	
	/**
     * Get gap value for cell calc 
     *
     * @since S60 v3.2
     * @return gap value
     */		
	static TSize GetCellSize( TPeninputCnCellType aCellType );
	
	/**
     * Get gap value for cell calc 
     *
     * @since S60 v3.2
     * @return gap value
     */		
	TInt CellAidGap( TPeninputCnCellType aCellType ) const;
	
	inline void GetPreviewWndRect( TRect& aOutRect, TRect& aInnerRect ) const;
	inline TAknTextLineLayout PreviewWndText() const;
	inline CFont* PreviewFont() const;
	inline TSize GetSpaceIconOffset() const;
	inline TInt GetSpaceSideIconWidth() const;
    
	/**
     * Return layout data for candidate list
     *
     * @return The layout data for candidate list
     */
	const TPeninputCnCandidateListLayoutData& CandidateListLayoutData() const;
	
	/*
	* Return layout data for candidate list
	*
	* @return The layout data for tooltip box	
	*/	
	const TPeninputCnTooltipBoxLayoutData& TooltipBoxLayoutData() const;
    
    /*
	* Get layout rect for composition area
	*
	* @param none 
	* @return rect of composition area
	*/
	const TRect& compositionLayoutData() const;
    
protected:

	/**
     * Set laf data base type
     *
     * @since S60 v3.2
     * @param aLafData The exact laf data
     * @return None
     */
	void SetLafData( MPeninputCnLafDataBase* aLafData );
	
    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return None
     */
    CPeninputCnGenericVkbLafMgr();
    
private:
	MPeninputCnLafDataCommon* iLafData;
	TPluginInputMode iInputMode;
    };
   
inline void CPeninputCnGenericVkbLafMgr::GetPreviewWndRect( TRect& aOutRect, 
                                                            TRect& aInnerRect ) const
	{
	aOutRect = iLafData->QwtLayoutData().iPreviewWndRect;
	aInnerRect = iLafData->QwtLayoutData().iPreviewWndInnerRect;
	}
	
inline TAknTextLineLayout CPeninputCnGenericVkbLafMgr::PreviewWndText() const
	{
	return iLafData->QwtLayoutData().iPreviewWndText;
	}
	
inline CFont* CPeninputCnGenericVkbLafMgr::PreviewFont() const
	{
	return iLafData->QwtLayoutData().iPreviewFont;
	}	
	
inline TSize CPeninputCnGenericVkbLafMgr::GetSpaceIconOffset() const
	{
	return iLafData->QwtLayoutData().iSpaceIconLayout.iIconOffset;	
	}	
	
inline TInt CPeninputCnGenericVkbLafMgr::GetSpaceSideIconWidth() const
	{
	return iLafData->QwtLayoutData().iSpaceIconLayout.iSideIconWidth;	
	}	   
#endif // C_PENINPUTVKBCNLAFDATAMGR_H
