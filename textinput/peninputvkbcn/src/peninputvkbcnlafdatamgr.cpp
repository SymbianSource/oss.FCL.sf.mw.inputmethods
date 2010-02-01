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
* Description:  peninput VKB data manager
*
*/


// System includes
#include <peninputdataconverter.h>
#include <peninputcommonlayoutglobalenum.h>
#include <AknUtils.h> 
#include <aknlayoutscalable_avkon.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>

// User includes
#include "peninputvkbcnlafdatamgr.h"

#define LAF_MODIFY 1

const TInt KKeypadLayoutCn[][2] = 
	{
		{ 10, 3 },		// 10x3
		{ 11, 3 },		// 11x3		
		{ 11, 4 },		// 11x4
		{ 10, 4	},		// 10x4
	};
	
const TInt KVkbTopAndBottomParam[] = { 5, 3, 4, 7};	
typedef TAknWindowComponentLayout (*Pfun)(TInt aVariety, TInt aCol = 0, TInt aRow = 0);

const Pfun cell_pane_func_cn[]=
	{
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp01,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp02,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp03,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp04,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp05,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp06,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp07,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp08,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp09,
	AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_cp10        	        	        	
	};

//	{ 0, 34, 68, 102, 136, 170, 204, 238, 272, 305 };
const TInt KKeypadRectXPoint10x[] = 
 	{ 0, 34, 68, 102, 136, 170, 204, 238, 272, 305 };
const TInt KKeypadCellWidth10x[] = 
	{ 41, 41, 41, 41, 41, 41, 41, 41, 41, 44 };	
//	{ 0, 32, 63, 94, 125, 155, 186, 217, 248, 279, 311 };		
const TInt KKeypadRectXPoint11x[] = 
	{ 0, 31, 62, 93, 124, 155, 186, 217, 248, 278, 311 };
const TInt KKeypadCellWidth11x[] = 
	{ 36, 36, 36, 36, 36, 36, 36, 36, 36, 39, 39 };
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputCnGenericVkbLafMgr* CPeninputCnGenericVkbLafMgr::NewL()
    {
    CPeninputCnGenericVkbLafMgr* self = NewLC();
    CleanupStack::Pop(self);
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::NewLC
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputCnGenericVkbLafMgr* CPeninputCnGenericVkbLafMgr::NewLC()
    {
    CPeninputCnGenericVkbLafMgr* self = 
        new ( ELeave ) CPeninputCnGenericVkbLafMgr();
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::CPeninputCnGenericVkbLafMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputCnGenericVkbLafMgr::CPeninputCnGenericVkbLafMgr():
    iInputMode(EPluginInputModeNone)
    {
    }
    
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::~CPeninputCnGenericVkbLafMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputCnGenericVkbLafMgr::~CPeninputCnGenericVkbLafMgr()
    {
	delete iLafData;
	iLafData = NULL;    
    }

TBool CPeninputCnGenericVkbLafMgr::IsValid()
    {
    if (!iLafData) 
    return EFalse;
    
    return iLafData->IsValid();
    }

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputCnGenericVkbLafMgr::ConstructL()
	{
		
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::SetInputMode
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputCnGenericVkbLafMgr::SetInputModeL( TPluginInputMode aInputMode)
	{
	if (iInputMode == aInputMode)
	    {
	    return;
	    }
	
	iInputMode = aInputMode;
	
	if (iLafData)
	    {
	    delete iLafData;
	    iLafData = NULL;
	    }
	    
     if (aInputMode == EPluginInputModeVkb)
        {
        iLafData = CPeninputCnLafDataVKB::NewL(ELayoutNone);
        }
     else if (aInputMode == EPluginInputModeFSQ)
        {
        iLafData = CPeninputCnLafDataFSQ::NewL(ELayoutNone);
        }
     else 
        {
        User::Leave(KErrNotSupported);
        }
	}	

TPluginInputMode CPeninputCnGenericVkbLafMgr::InputMode() const
    {
    return iInputMode;
    }
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::SetLayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputCnGenericVkbLafMgr::SetLayoutType( TPeninputCnQwtLayoutType aLayoutType )
	{
	ASSERT(iLafData);
	iLafData->SetLayoutType( aLayoutType );	
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::SetLafOption
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputCnGenericVkbLafMgr::SetLafOption( TPeninputCnVkbLafOption aOption )
	{
	if( iLafData->LafOption() == aOption )
		return;
	
	iLafData->SetLafOption( aOption );	
	TRAP_IGNORE(iLafData->ConstructL());
	}

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::SetLafOption
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TPeninputCnVkbLafOption CPeninputCnGenericVkbLafMgr::OptionType() const
	{
	return iLafData->LafOption();
	}

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::GetButtonRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputCnGenericVkbLafMgr::GetButtonRect( TPeninputCommonCtrlID aButtonID, 
								TRect& aOutterRect,
								TRect& aInnerRect )
	{
	ASSERT(iLafData);		
	const TPeninputCnButtonLayoutData& data = iLafData->ButtonLayoutData( aButtonID );
	aOutterRect = data.iRect;
	aInnerRect  = data.iInnerRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::KeyRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TRect& CPeninputCnGenericVkbLafMgr::KeypadRect() const
	{
	ASSERT(iLafData);
	return iLafData->KeypadLayoutData().iKaypadRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::ICFRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
const TRect& CPeninputCnGenericVkbLafMgr::ICFRect() const
	{
	ASSERT(iLafData);
	return iLafData->ICFLayoutData().iRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::ICFPromptTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
const CFont* CPeninputCnGenericVkbLafMgr::ICFPromptTextFont() const
	{
	ASSERT(iLafData);
	return iLafData->ICFLayoutData().iPromptTextFont;
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::ICFPromptTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
const CFont* CPeninputCnGenericVkbLafMgr::ICFTextFont() const
	{
	ASSERT(iLafData);
	return iLafData->ICFLayoutData().iTextFont;
	}
	
// ---------------------------------------------------------------------------
TAknTextLineLayout CPeninputCnGenericVkbLafMgr::KeyTextLayout() const
	{
	ASSERT(iLafData);
	return iLafData->KeypadLayoutData().iTextLayout;
	}
// CPeninputCnGenericVkbLafMgr::ICFPromptTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
const TRect& CPeninputCnGenericVkbLafMgr::EntirePaneRect() const
	{
	ASSERT(iLafData);
	return iLafData->QwtLayoutData().iQwtRect;
	}

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::KeyRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputCnGenericVkbLafMgr::GetKeyRect( TInt aColume, 
													TInt aRow, 
													TRect& aKeyRect,
													TRect& aCharRect ) const
	{
	ASSERT(iLafData);
	TInt colNum = 0;
	TInt index = 0;
	switch( iLafData->LayoutType() )
		{
		case ELayout10x3:
		case ELayout10x3_withCandi:
		case ELayout10x4:
			{
			colNum = 10;
			}
			break;
		case ELayout11x3:
			{
			colNum = 11;
			}
			break;	
		case ELayout11x4:
			{
			colNum = 11;
			}
			break;		
		}
	index = aRow * colNum + aColume;
	aKeyRect = iLafData->KeypadLayoutData().iRects[index];
	aCharRect = iLafData->KeypadLayoutData().iCharRects[index];
	}

TPeninputCnQwtLayoutType CPeninputCnGenericVkbLafMgr::
                                  LayoutTypeFromRowsColumns(const TInt aRows, const TInt aColumns)
    {
    TPeninputCnQwtLayoutType ret;
    
    if (aRows == 3 && aColumns == 10)
        {
        ret = ELayout10x3_withCandi;
        }
    else if (aRows == 3 && aColumns == 11)
        {
        ret = ELayout11x3;
        }
    else if (aRows == 4 && aColumns == 11)    
        {
        ret = ELayout11x4;
        }
    else if (aRows == 4 && aColumns == 10)    
        {
        ret = ELayout10x4;
        }        
    else
        {
        ret = ELayoutNone;
        }

     return ret;
    }
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::KeyTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const CFont* CPeninputCnGenericVkbLafMgr::KeyTextFont() const
	{
	ASSERT(iLafData);
	return iLafData->KeypadLayoutData().iFont;	
	}

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::KeyTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TRect& CPeninputCnGenericVkbLafMgr::RangbarRect() const
	{
	ASSERT(iLafData);
	return iLafData->RangbarLayoutData().iRect;
	}

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::GetICFConfig
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
void CPeninputCnGenericVkbLafMgr::GetICFConfig( TPeninputCnICFConfig& aICFConfig )
	{
	ASSERT(iLafData);	
	const TPeninputCnEntryLayoutData& icfLayout = iLafData->ICFLayoutData();
	aICFConfig.iLeftMargin = icfLayout.iPromptTextRect.iTl.iX - icfLayout.iRect.iTl.iX;
	aICFConfig.iRightMargin = icfLayout.iRect.iBr.iX - icfLayout.iPromptTextRect.iBr.iX;
	aICFConfig.iTopMargin = icfLayout.iPromptTextRect.iTl.iY - icfLayout.iRect.iTl.iY;
	aICFConfig.iTextLineHeight = icfLayout.iPromptTextRect.Height();
	
    TRect rect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
    TBool landscape = rect.Size().iWidth < rect.Size().iHeight ? EFalse : ETrue;
	
    // Just one row for ICF on PinYin layout
	if( iLafData->LayoutType() == ELayout11x4 && 
	    iInputMode == EPluginInputModeFSQ && landscape )
	    {
	    aICFConfig.iLineSpaceMargin = icfLayout.iRect.Height()
	                                  -aICFConfig.iTopMargin
	                                  -icfLayout.iPromptTextRect.Height();
	    }
  else
	    {
	    aICFConfig.iLineSpaceMargin = icfLayout.iTextRect.iTl.iY - icfLayout.iPromptTextRect.iBr.iY;
	    }
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::KeyTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TRect& CPeninputCnGenericVkbLafMgr::TitlebarRect() const
	{
	ASSERT(iLafData);
	return iLafData->QwtLayoutData().iTitlebarRect;
	}

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::ClientRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TRect& CPeninputCnGenericVkbLafMgr::ClientRect() const
	{
	ASSERT(iLafData);
	return iLafData->QwtLayoutData().iClientRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::RangbarCellCount
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputCnGenericVkbLafMgr::RangbarCellCount() const
	{
	ASSERT(iLafData);	
	return 	iLafData->RangbarLayoutData().iCellDataArray.Count();
	}

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::RangbarCellCount
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputCnGenericVkbLafMgr::RangbarCellAt( TInt aIndex, 
												 TRect& aOutterRect, 
												 TRect& aInnerRect )
	{
	ASSERT(iLafData);	
	aOutterRect = iLafData->RangbarLayoutData().iCellDataArray[aIndex].iRect;
	aInnerRect  = iLafData->RangbarLayoutData().iCellDataArray[aIndex].iInnerRect;  
	}

// ---------------------------------------------------------------------------
// Return layout data for candidate list
// ---------------------------------------------------------------------------
//
const TPeninputCnCandidateListLayoutData& 
                CPeninputCnGenericVkbLafMgr::CandidateListLayoutData() const
    {
    ASSERT(iLafData);
    return iLafData->QwtLayoutData().iCandidateList;
    }
	
// ---------------------------------------------------------------------------
// Return layout data for tooltip box
// ---------------------------------------------------------------------------
//
const TPeninputCnTooltipBoxLayoutData& 
                CPeninputCnGenericVkbLafMgr::TooltipBoxLayoutData() const
    {
    ASSERT(iLafData);
    return iLafData->QwtLayoutData().iTooltipBox;
    }

// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::GetCellSize
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TSize CPeninputCnGenericVkbLafMgr::GetCellSize( TPeninputCnCellType aCellType )
	{
	TAknWindowLineLayout appWnd; 
    TAknWindowLineLayout wndLayout, cellLayout;
    TAknLayoutRect wndRect, cellRect;
    
	appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();

	switch( aCellType )
		{
		case EVkbCangjieAndPinying: //10x4
		case EVkbStroke:
		case EVkbLatin:				
		case EVkbNumber:				
			{
			wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(4).LayoutLine();
			wndRect.LayoutRect(appWnd.Rect(), wndLayout);
			cellLayout = AknLayoutScalable_Avkon::aid_size_cell_vkb2(7).LayoutLine();
			cellRect.LayoutRect( wndRect.Rect(), cellLayout );		
			}
			break;
		case EVkbZhuyin:			//11x4	
		case EVkbLatin_Zhuyin:				
		case EVkbNumber_Zhuyin:
		//case EVkbAccent:
			wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(2).LayoutLine();
			wndRect.LayoutRect(appWnd.Rect(), wndLayout);
			cellLayout = AknLayoutScalable_Avkon::aid_size_cell_vkb2(4).LayoutLine();
			cellRect.LayoutRect( wndRect.Rect(), cellLayout );		
			break;
		case EVkbCangjieAndPinying_FSQ:	//10x4
		case EVkbStroke_FSQ:
		case EVkbLatin_FSQ:				
		case EVkbNumber_FSQ:		
			{
			wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
			wndRect.LayoutRect(appWnd.Rect(), wndLayout);
			cellLayout = AknLayoutScalable_Avkon::aid_size_cell_vkb2(6).LayoutLine();
			cellRect.LayoutRect( wndRect.Rect(), cellLayout );		
			}
			break;
		case EVkbNumber_Zhuyin_FSQ:		//11x4
		case EVkbZhuyin_FSQ:
		case EVkbLatin_Zhuyin_FSQ:
			{		
			wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
			wndRect.LayoutRect(appWnd.Rect(), wndLayout);
			cellLayout = AknLayoutScalable_Avkon::aid_size_cell_vkb2(2).LayoutLine();
			cellRect.LayoutRect( wndRect.Rect(), cellLayout );		
			}
			break;			
		}
	if( aCellType == EVkbCangjieAndPinying_FSQ || aCellType == EVkbStroke_FSQ)
		{
		TRect rect = cellRect.Rect();
		rect.Shrink( 5, 0 ); 
		return rect.Size();
		}
	return cellRect.Rect().Size();
	}
	
// ---------------------------------------------------------------------------
// CPeninputCnGenericVkbLafMgr::CellAidGap
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputCnGenericVkbLafMgr::CellAidGap( TPeninputCnCellType aCellType ) const
	{
	const TInt KColuNum[] = { 10, 10, 11, 11, 10, 11, 10, 11, 10, 10, 11, 10, 10, 11, 11 };
 	TSize unitGrid;
	unitGrid = CPeninputCnGenericVkbLafMgr::GetCellSize( aCellType );
//	TInt cellUnit = unitGrid.Rect().Width();
    TInt cellUnit = unitGrid.iWidth;
    TInt width = EntirePaneRect().Width();
    TInt cellAidGap = width - cellUnit*KColuNum[aCellType];
    if (aCellType == EVkbLatin || aCellType == EVkbNumber || aCellType == EVkbAccent)
    	{
    	cellAidGap = cellAidGap - 9;	
    	}
	return cellAidGap; 
	}

// ---------------------------------------------------------------------------
// CPeninputCnQwtLayoutDataInfo::~CPeninputCnQwtLayoutDataInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputCnQwtLayoutDataInfo::~CPeninputCnQwtLayoutDataInfo()
	{
	iKeypad.iRects.Close();
	iKeypad.iCharRects.Close();
	iRangbar.iCellDataArray.Close();
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::DestoryDataCommon
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void MPeninputCnLafDataCommon::DestoryDataCommon()
	{
	iLayoutDataInfo.ResetAndDestroy();
	iLayoutDataInfo.Close();
	}
	
MPeninputCnLafDataCommon::~MPeninputCnLafDataCommon()	
	{
		
	}
// CPeninputLafDataCommon::ButtonLayoutData(TInt aButtonID)
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TPeninputCnButtonLayoutData& MPeninputCnLafDataCommon::
                                                     ButtonLayoutData( TInt aButtonID ) const
	{
	ASSERT( IsValid() );
	const TPeninputCnButtonLayoutData* pData = NULL;

	switch( aButtonID )
		{
		case EPeninutWindowCtrlIdMoveBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iMoveButton);
			}
			break;
		case EPeninutWindowCtrlIdCloseBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iCloseButton);
			}
			break;
		case EPeninutWindowCtrlIdOptionBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iOptionButton);								
			}
			break;
		case EPeninutWindowCtrlIdBackspaceBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iClearButton);								
			}
			break;
		case EPeninutWindowCtrlIdEnterBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iEnterButton);								
			}
			break;
		case EPeninutWindowCtrlIdSwitchToHwrBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iSwitchButton);								
			}
			break;
		case EPeninutWindowCtrlIdSpaceBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iSpaceButton);								
			}
			break;	
		case EPeninutWindowCtrlIdShiftBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iShiftButton);								
			}
			break;	
		case EPeninupWindowCtrlIdArrowLeftBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iLeftButton);	
			}
			break;
		case EPeninupWindowCtrlIdArrowRightBtn:
			{
			pData = &(iLayoutDataInfo[iLayoutType]->iRightButton);	
			}
			break;
		default:
			ASSERT(EFalse);
			break;
		}
	ASSERT(pData);
	return *pData;
	}
// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::ICFLayoutData()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
const TPeninputCnEntryLayoutData& MPeninputCnLafDataCommon::ICFLayoutData() const
	{
	ASSERT( IsValid() );
	return iLayoutDataInfo[iLayoutType]->iICF;
	}
	
// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::KeypadLayoutData()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
const TPeninputCnKeyLayoutData& MPeninputCnLafDataCommon::KeypadLayoutData() const
	{
	ASSERT( IsValid() );
	return iLayoutDataInfo[iLayoutType]->iKeypad;
	}
	
// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::RangbarLayoutData(TInt aRangbarID)
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
const TPeninputCnRangeBarLayoutData& MPeninputCnLafDataCommon::RangbarLayoutData() const
	{
	ASSERT( IsValid() );
	return iLayoutDataInfo[iLayoutType]->iRangbar;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::RangbarLayoutData(TInt aRangbarID)
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
const CPeninputCnQwtLayoutDataInfo& MPeninputCnLafDataCommon::QwtLayoutData() const
	{
	ASSERT( IsValid() );
	return 	*iLayoutDataInfo[iLayoutType];
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::ConstructL()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
void MPeninputCnLafDataCommon::ConstructL()
	{
	ReadLafInfoL();
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::ConstructL()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
TBool MPeninputCnLafDataCommon::IsValid() const
	{
	return ( iLayoutDataInfo.Count() && iLayoutType != ELayoutNone ) ? ETrue : EFalse;
	}


// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::GetTopAndBottomPaneInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
void MPeninputCnLafDataCommon::GetTopAndBottomPaneInfo( const TRect& aParentWndRect, 
													  TInt aLayoutType,
													  TInt aLayoutParam,
													  CPeninputCnQwtLayoutDataInfo& aDataInfo )
	{
	// judge which case 
	const TInt endFSQType  = 2;
	
	
	TBool isVKB = aLayoutParam > endFSQType ? ETrue : EFalse;
	
	TAknWindowLineLayout toppane, bottompane, bottomgrid;
    TAknWindowLineLayout topleftpane, topentrypane, toprightpane; 
    TAknWindowLineLayout cellBtn, cellBtnInner;	
    
    TAknLayoutRect toppaneRect, bottompaneRect, bottomgridRect;
    TAknLayoutRect topleftpaneRect, topentrypaneRect, toprightpaneRect;
    TAknLayoutRect cellBtnRect, cellBtnInnerRect;
	TAknTextLineLayout cellText;
	TAknLayoutText txt;
	TRect keypadRect;
	// top pane
	if( isVKB )
		{
		toppane = AknLayoutScalable_Avkon::
				  vkb2_area_top_pane(KVkbTopAndBottomParam[aLayoutType]).LayoutLine();		
		}
	else
		{
		toppane = AknLayoutScalable_Avkon::vkb2_area_top_pane(aLayoutParam).LayoutLine();		
		}
	toppaneRect.LayoutRect(aParentWndRect, toppane);
	aDataInfo.iTitlebarRect = toppaneRect.Rect();
	topleftpane = AknLayoutScalable_Avkon::vkb2_top_grid_left_pane(aLayoutParam).LayoutLine();
	topleftpaneRect.LayoutRect(toppaneRect.Rect(), topleftpane);

	// close button
	cellBtn = AknLayoutScalable_Avkon::vkb2_top_cell_left_pane(aLayoutParam, 0 ,0).LayoutLine();
	cellBtnRect.LayoutRect(topleftpaneRect.Rect(), cellBtn);
	cellBtnInner = AknLayoutScalable_Avkon::vkb2_top_cell_left_pane_g1().LayoutLine();
	cellBtnInnerRect.LayoutRect(cellBtnRect.Rect(), cellBtnInner);
	aDataInfo.iCloseButton.iRect = cellBtnRect.Rect();
	aDataInfo.iCloseButton.iInnerRect = cellBtnInnerRect.Rect();
	// option button
	cellBtn = AknLayoutScalable_Avkon::vkb2_top_cell_left_pane(aLayoutParam, 0 ,1).LayoutLine();
	cellBtnRect.LayoutRect(topleftpaneRect.Rect(), cellBtn);
	cellBtnInner = AknLayoutScalable_Avkon::vkb2_top_cell_left_pane_g1().LayoutLine();
	cellBtnInnerRect.LayoutRect(cellBtnRect.Rect(), cellBtnInner);
	aDataInfo.iOptionButton.iRect = cellBtnRect.Rect();
	aDataInfo.iOptionButton.iInnerRect = cellBtnInnerRect.Rect();
	
    TRect rect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
    TBool landscape = rect.Size().iWidth < rect.Size().iHeight ? EFalse : ETrue;
    
	// entry pane
	// Just one row for ICF on PinYin layout
	if( aLayoutType == ELayout11x4 && !isVKB && landscape )
	    {
	    topentrypane = AknLayoutScalable_Avkon::vkb2_top_entry_pane(8).LayoutLine();
	    }
	else
	    {
	    topentrypane = AknLayoutScalable_Avkon::vkb2_top_entry_pane(aLayoutParam).LayoutLine();
	    }
	topentrypaneRect.LayoutRect(toppaneRect.Rect(), topentrypane);	
	aDataInfo.iICF.iRect = topentrypaneRect.Rect();
	if( aLayoutType == ELayout11x4 && !isVKB && landscape )
	    {
	    cellText = AknLayoutScalable_Avkon::vkb2_top_entry_pane_t1(5).LayoutLine();
	    }
	else
	    {
	    cellText = AknLayoutScalable_Avkon::vkb2_top_entry_pane_t1(aLayoutParam).LayoutLine();
	    }
	aDataInfo.iICF.iPromptTextFont = AknLayoutUtils::FontFromId(cellText.iFont, NULL);
	txt.LayoutText( topentrypaneRect.Rect(), cellText );
	aDataInfo.iICF.iPromptTextRect = txt.TextRect();

	if( aLayoutType == ELayout11x4 && !isVKB && landscape )
	    {
	    cellText = AknLayoutScalable_Avkon::vkb2_top_entry_pane_t1(5).LayoutLine();
	    }
	else
	    {
	    cellText = AknLayoutScalable_Avkon::vkb2_top_entry_pane_t2(aLayoutParam).LayoutLine();
	    }

	aDataInfo.iICF.iTextFont = AknLayoutUtils::FontFromId(cellText.iFont, NULL);
	txt.LayoutText( topentrypaneRect.Rect(), cellText );
	aDataInfo.iICF.iTextRect = txt.TextRect();
	
	// ?? need modified later. inner rect is same as outter rect.
	toprightpane = AknLayoutScalable_Avkon::vkb2_top_grid_right_pane(aLayoutParam).LayoutLine();
	toprightpaneRect.LayoutRect(toppaneRect.Rect(), toprightpane);
	if( isVKB )
		{
		// clear button	
		cellBtn = AknLayoutScalable_Avkon::
				  vkb2_top_grid_right_pane_g1(aLayoutParam).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		aDataInfo.iClearButton.iRect = cellBtnRect.Rect();
		aDataInfo.iClearButton.iInnerRect = cellBtnRect.Rect();
		// move button (VKB)			
		cellBtn = AknLayoutScalable_Avkon::
				  vkb2_top_grid_right_pane_g4(aLayoutParam).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		aDataInfo.iMoveButton.iRect = cellBtnRect.Rect();
		aDataInfo.iMoveButton.iInnerRect = cellBtnRect.Rect();									
		}
	else
		{
		// clear button	
		cellBtn = AknLayoutScalable_Avkon::
				  vkb2_top_grid_right_pane_g1(aLayoutType).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		aDataInfo.iClearButton.iRect = cellBtnRect.Rect();
		aDataInfo.iClearButton.iInnerRect = cellBtnRect.Rect();			
		// left button (FSQ)
		cellBtn = AknLayoutScalable_Avkon::vkb2_top_grid_right_pane_g2(aLayoutType).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		aDataInfo.iLeftButton.iRect = cellBtnRect.Rect();
		aDataInfo.iLeftButton.iInnerRect = cellBtnRect.Rect();
		// right button (FSQ)
		cellBtn = AknLayoutScalable_Avkon::vkb2_top_grid_right_pane_g3(aLayoutType).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		aDataInfo.iRightButton.iRect = cellBtnRect.Rect();
		aDataInfo.iRightButton.iInnerRect = cellBtnRect.Rect();		
		}
	
	// bottom pane
	const TInt VKBParamBottom2Range[] = { 6, 8, 8 };
	const TInt FSQParamBottom2Range[] = { 0, 2, 4 };
	const TInt VKBParamBottom3Range[] = { 7, 9, 9 };
	const TInt FSQParamBottom3Range[] = { 1, 3, 5 };

	
	if( isVKB )
		{
		bottompane = AknLayoutScalable_Avkon::
				  vkb2_area_bottom_pane(KVkbTopAndBottomParam[aLayoutType]).LayoutLine();					
		}
	else
		{
  		bottompane = AknLayoutScalable_Avkon::vkb2_area_bottom_pane(aLayoutParam).LayoutLine();			
		}
  	bottompaneRect.LayoutRect(aParentWndRect, bottompane);
  	
  	// Get keypad rect
  	keypadRect.SetRect( toppaneRect.Rect().iTl.iX, 
  						toppaneRect.Rect().iBr.iY, 
  						bottompaneRect.Rect().iBr.iX,
						bottompaneRect.Rect().iTl.iY);
	aDataInfo.iClientRect.SetRect( toppaneRect.Rect().iTl.iX, 
  								   toppaneRect.Rect().iBr.iY,
  								   bottompaneRect.Rect().iBr.iX,
								   bottompaneRect.Rect().iBr.iY);
	aDataInfo.iQwtRect.SetRect( toppaneRect.Rect().iTl.iX, 
  								toppaneRect.Rect().iTl.iY,
  								bottompaneRect.Rect().iBr.iX ,
								bottompaneRect.Rect().iBr.iY );
	TInt bottomLayoutType = 0;
	if( isVKB )
		{
		bottomLayoutType = iLafOption & EOption3CellRange ? 
						   VKBParamBottom3Range[aLayoutType] : VKBParamBottom2Range[aLayoutType];
		}
	else
		{
		bottomLayoutType = iLafOption & EOption3CellRange ? 
						   FSQParamBottom3Range[aLayoutType] : FSQParamBottom2Range[aLayoutType];			
		}
  	bottomgrid = AknLayoutScalable_Avkon::vkb2_area_bottom_grid_pane(bottomLayoutType).LayoutLine();
  	bottomgridRect.LayoutRect(bottompaneRect.Rect(), bottomgrid);
	
	TInt maxParam = iLafOption & EOption3CellRange ? 4 : 3;
		// shift button  	
  	cellBtn = AknLayoutScalable_Avkon::vkb2_cell_bottom_grid_pane(aLayoutParam, 0, 0).LayoutLine();
	cellBtnRect.LayoutRect(bottomgridRect.Rect(), cellBtn);
	cellBtnInner = AknLayoutScalable_Avkon::vkb2_cell_bottom_grid_pane_g1().LayoutLine();
	cellBtnInnerRect.LayoutRect(cellBtnRect.Rect(), cellBtnInner);	
	aDataInfo.iShiftButton.iRect = cellBtnRect.Rect();
	aDataInfo.iShiftButton.iInnerRect = cellBtnInnerRect.Rect();
	
	TPeninputCnButtonLayoutData btnLayoutData;		
	for( TInt i = 1; i < maxParam; i++ )
		{
#ifdef LAF_MODIFY	
		if( i == 3 )
			{
			btnLayoutData.iRect	= aDataInfo.iRangbar.iCellDataArray[i-2].iRect;
			btnLayoutData.iRect.Move( btnLayoutData.iRect.Width(), 0 );
			btnLayoutData.iInnerRect = btnLayoutData.iRect;
			aDataInfo.iRangbar.iCellDataArray.Append( btnLayoutData );
			break;
			}
#endif //LAF_MODIFY		
		// range cell
		cellBtn = AknLayoutScalable_Avkon::vkb2_cell_bottom_grid_pane(aLayoutParam, i, 0).LayoutLine();
		cellBtnRect.LayoutRect(bottomgridRect.Rect(), cellBtn);
		cellBtnInner = AknLayoutScalable_Avkon::vkb2_cell_bottom_grid_pane_g1().LayoutLine();
		cellBtnInnerRect.LayoutRect(cellBtnRect.Rect(), cellBtnInner);
		btnLayoutData.iRect = cellBtnRect.Rect();
		btnLayoutData.iInnerRect = cellBtnInnerRect.Rect();	
		aDataInfo.iRangbar.iCellDataArray.Append( btnLayoutData );				
		}
		
	TInt count = aDataInfo.iRangbar.iCellDataArray.Count();
	aDataInfo.iRangbar.iRect = TRect( aDataInfo.iRangbar.iCellDataArray[0].iRect.iTl,
									  aDataInfo.iRangbar.iCellDataArray[count - 1].iRect.iBr );
	
		// input mode button ?? need modified later. inner rect is same as outter rect
	cellBtn = AknLayoutScalable_Avkon::vkb2_area_bottom_pane_g1(bottomLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(bottompaneRect.Rect(), cellBtn);
	aDataInfo.iSwitchButton.iRect = cellBtnRect.Rect();
	aDataInfo.iSwitchButton.iInnerRect = cellBtnRect.Rect();	
		// enter button 
	cellBtn = AknLayoutScalable_Avkon::vkb2_area_bottom_pane_g2(bottomLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(bottompaneRect.Rect(), cellBtn);
	aDataInfo.iEnterButton.iRect = cellBtnRect.Rect();
	aDataInfo.iEnterButton.iInnerRect = cellBtnRect.Rect();	
		// space button
	cellBtn = AknLayoutScalable_Avkon::vkb2_area_bottom_pane_g3(bottomLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(bottompaneRect.Rect(), cellBtn);
	aDataInfo.iSpaceButton.iRect = cellBtnRect.Rect();
	aDataInfo.iSpaceButton.iRect.iTl = TPoint( aDataInfo.iRangbar.iRect.iBr.iX,
											   aDataInfo.iRangbar.iRect.iTl.iY );
	aDataInfo.iSpaceButton.iInnerRect = cellBtnRect.Rect();	  			
  	  	// 3 pieces graphics laf info
  	TRect spaceRect, iconRect;
  	cellBtn = 
        AknLayoutScalable_Avkon::vkb2_area_bottom_space_btn_pane(bottomLayoutType).LayoutLine();
  	cellBtnRect.LayoutRect(bottompaneRect.Rect(), cellBtn);
  	spaceRect = cellBtnRect.Rect();
  	cellBtn = 
        AknLayoutScalable_Avkon::vkb2_area_bottom_space_btn_pane_g1(bottomLayoutType).LayoutLine();
  	cellBtnRect.LayoutRect(spaceRect, cellBtn);
  	iconRect = cellBtnRect.Rect();
  	aDataInfo.iSpaceIconLayout.iIconOffset.iWidth = iconRect.iTl.iX - spaceRect.iTl.iX;
  	aDataInfo.iSpaceIconLayout.iIconOffset.iHeight = iconRect.iTl.iY- spaceRect.iTl.iY;
	aDataInfo.iSpaceIconLayout.iSideIconWidth = iconRect.Width();
  	
    // preview popup window 
  	TAknWindowLineLayout previewWnd, previewWndInner;
  	TAknLayoutRect previewWndRect, previewWndInnerRect;
    TAknLayoutText previewWndText;
  	previewWnd = AknLayoutScalable_Avkon::popup_fep_char_preview_window(aLayoutParam).LayoutLine();
  	previewWndRect.LayoutRect( aParentWndRect, previewWnd );
  	previewWndInner = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_g9().LayoutLine();
  	previewWndInnerRect.LayoutRect( previewWndRect.Rect(), previewWndInner );
  	aDataInfo.iPreviewWndRect = previewWndRect.Rect();
  	aDataInfo.iPreviewWndInnerRect = previewWndInnerRect.Rect();
  	aDataInfo.iPreviewWndText = 
        AknLayoutScalable_Avkon::popup_fep_char_preview_window_t1(aLayoutParam).LayoutLine();
	previewWndText.LayoutText(previewWndRect.Rect(), aDataInfo.iPreviewWndText);
	aDataInfo.iPreviewFont = const_cast<CFont*>(previewWndText.Font());
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::SetLayoutType( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
inline void MPeninputCnLafDataCommon::SetLayoutType( TInt aLayoutType )
	{
    iLayoutType = aLayoutType;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::LayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline TInt MPeninputCnLafDataCommon::LayoutType() const
	{
	return iLayoutType;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::SetLafOption
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline void MPeninputCnLafDataCommon::SetLafOption( TPeninputCnVkbLafOption aLafOption )
	{
	iLafOption = aLafOption;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::SetLafOption
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline TPeninputCnVkbLafOption MPeninputCnLafDataCommon::LafOption() const
	{
	return iLafOption;
	}

// ---------------------------------------------------------------------------
// CPeninputCnLafDataVKB::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
CPeninputCnLafDataVKB* CPeninputCnLafDataVKB::NewL( TInt aLayoutType )
	{
    CPeninputCnLafDataVKB* self = NewLC( aLayoutType );
    CleanupStack::Pop();
    return self;
	}
// ---------------------------------------------------------------------------
// CPeninputCnLafDataVKB::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputCnLafDataVKB* CPeninputCnLafDataVKB::NewLC( TInt aLayoutType )
	{
	CPeninputCnLafDataVKB* self = new ( ELeave ) CPeninputCnLafDataVKB( aLayoutType );
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
	}
CPeninputCnLafDataVKB::~CPeninputCnLafDataVKB()
	{
	DestoryDataCommon();	
	}
	
	
// ---------------------------------------------------------------------------
// CPeninputCnLafDataVKB::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputCnLafDataVKB::CPeninputCnLafDataVKB( TInt aLayoutType )
	{
	SetLayoutType( aLayoutType );	
	}
// ---------------------------------------------------------------------------
// CPeninputCnLafDataVKB::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
void CPeninputCnLafDataVKB::ReadLafInfoL()
	{
	// clean up all the data
	iLayoutDataInfo.ResetAndDestroy();
	
	CPeninputCnQwtLayoutDataInfo* dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;
	
	TAknWindowLineLayout appWnd; 
    TAknWindowLineLayout wndLayout;
    TAknWindowLineLayout keypad, rowpane, cellpane;
    
    TAknLayoutRect keypadRect, rowpaneRect, cellpaneRect;
    TAknLayoutRect wndRect;
    TAknLayoutText keyTextLayout;
	TAknTextLineLayout keyText;
	TRect keyRect, keyCharRect;
// -----------------------Portray Mode--------------------------
    appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
    //iAppWndRect = appWnd.Rect();
	
	// ==================10x3====================
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(3).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout10x3, 3, *dataInfo );
	// keypad
  	keyText = AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_t1(0).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	//dataInfo->iKeypad.iTxtRect = keyText.TextRect();
	
    keypad = AknLayoutScalable_Avkon::
    		 vkb2_area_keypad_pane(KVkbTopAndBottomParam[ELayout10x3]).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();

  	//iKeypadRect10x3 = keypadRect.Rect();
  	for( TInt i = 0; i < KKeypadLayoutCn[0][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayoutCn[0][0]; j++)
  			{
  			rowpane = AknLayoutScalable_Avkon::vkb2_row_keypad_pane(2, 0, i).LayoutLine();
  			rowpaneRect.LayoutRect(keypadRect.Rect(), rowpane);
  			cellpane = (*cell_pane_func_cn[j])(0).LayoutLine();
  			cellpaneRect.LayoutRect(rowpaneRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
#ifdef LAF_MODIFY
			keyRect.iTl.iX = KKeypadRectXPoint10x[j];
			keyRect.SetWidth( KKeypadCellWidth10x[j] );
#endif //LAF_MODIFY
  			dataInfo->iKeypad.iRects.AppendL(keyRect);
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
   	iLayoutDataInfo.AppendL( dataInfo ); 
  	
	// ==================11x3====================
	dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(1).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout11x3, 4, *dataInfo );
	// keypad
  	keyText = AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_t1(0).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);

    keypad = AknLayoutScalable_Avkon::
    		 vkb2_area_keypad_pane(KVkbTopAndBottomParam[ELayout11x3]).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();

  	for( TInt i = 0; i < KKeypadLayoutCn[1][1]; i++)
		{
		for( TInt j = 0; j < KKeypadLayoutCn[1][0]; j++)
			{
			rowpane = AknLayoutScalable_Avkon::vkb2_row_keypad_pane(0, 0, i).LayoutLine();
			rowpaneRect.LayoutRect(keypadRect.Rect(), rowpane);
			cellpane = (*cell_pane_func_cn[j])(1).LayoutLine();
			cellpaneRect.LayoutRect(rowpaneRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
#ifdef LAF_MODIFY  			
 			keyRect.iTl.iX = KKeypadRectXPoint11x[j];
			keyRect.SetWidth( KKeypadCellWidth11x[j] );
#endif //LAF_MODIFY  			  				
  			dataInfo->iKeypad.iRects.AppendL(keyRect);
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
			}
		}
   	iLayoutDataInfo.AppendL( dataInfo ); 
		
	// ==================11x4====================
	dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(2).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout11x4, 4, *dataInfo );
	// keypad
  	keyText = AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_t1(0).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);

    keypad = AknLayoutScalable_Avkon::
    		 vkb2_area_keypad_pane(KVkbTopAndBottomParam[ELayout11x4]).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();

  	for( TInt i = 0; i < KKeypadLayoutCn[2][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayoutCn[2][0]; j++)
  			{
  			rowpane = AknLayoutScalable_Avkon::vkb2_row_keypad_pane(1, 0, i).LayoutLine();
  			rowpaneRect.LayoutRect(keypadRect.Rect(), rowpane);
  			cellpane = (*cell_pane_func_cn[j])(1).LayoutLine();
  			cellpaneRect.LayoutRect(rowpaneRect.Rect(), cellpane); 
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
#ifdef LAF_MODIFY  			
			keyRect.iTl.iX = KKeypadRectXPoint11x[j];
			keyRect.SetWidth( KKeypadCellWidth11x[j] );
#endif //LAF_MODIFY  			
  			dataInfo->iKeypad.iRects.AppendL(keyRect);
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
   	iLayoutDataInfo.AppendL( dataInfo ); 
   	
   	// ==================10x3 with candi pane====================
	dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;  	
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(4).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
   	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout10x3, 3, *dataInfo );
	// keypad
  	keyText = AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_t1(0).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	
  	keypad = AknLayoutScalable_Avkon::
    		 vkb2_area_keypad_pane(KVkbTopAndBottomParam[ELayout10x3_withCandi]).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();

  	for( TInt i = 0; i < KKeypadLayoutCn[0][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayoutCn[0][0]; j++)
  			{
  			rowpane = AknLayoutScalable_Avkon::vkb2_row_keypad_pane(2, 0, i).LayoutLine();
  			rowpaneRect.LayoutRect(keypadRect.Rect(), rowpane);
  			cellpane = (*cell_pane_func_cn[j])(0).LayoutLine();
  			cellpaneRect.LayoutRect(rowpaneRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
#ifdef LAF_MODIFY
			keyRect.iTl.iX = KKeypadRectXPoint10x[j];
			keyRect.SetWidth( KKeypadCellWidth10x[j] );
#endif //LAF_MODIFY
  			dataInfo->iKeypad.iRects.AppendL(keyRect);
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
   	iLayoutDataInfo.AppendL( dataInfo ); 
   	
  	// ==================10x4(not create by laf)====================
	dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;  	
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(4).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout10x3, 3, *dataInfo );
	// keypad
  	keyText = AknLayoutScalable_Avkon::vkb2_cell_t_keypad_pane_t1(0).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	//dataInfo->iKeypad.iTxtRect = keyText.TextRect();
	
    keypad = AknLayoutScalable_Avkon::
    		 vkb2_area_keypad_pane(KVkbTopAndBottomParam[ELayout10x3]).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();

  	TInt keyLastRowHeight = 0;
  	TBool bLastRow = EFalse;
  	for( TInt i = 0; i < KKeypadLayoutCn[3][1]; i++)
  		{
  		if( i == KKeypadLayoutCn[3][1] -1 )
  			bLastRow = ETrue;
  		for( TInt j = 0; j < KKeypadLayoutCn[3][0]; j++)
  			{
  			TInt index = bLastRow ? i-1 : i;
  			rowpane = AknLayoutScalable_Avkon::vkb2_row_keypad_pane(2, 0, index).LayoutLine();
  			rowpaneRect.LayoutRect(keypadRect.Rect(), rowpane);
  			cellpane = (*cell_pane_func_cn[j])(0).LayoutLine();
  			cellpaneRect.LayoutRect(rowpaneRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
#ifdef LAF_MODIFY
			keyRect.iTl.iX = KKeypadRectXPoint10x[j];
			keyRect.SetWidth( KKeypadCellWidth10x[j] );
			if( bLastRow )
				{
				keyRect.Move(0, keyRect.Height());
				keyLastRowHeight = keyRect.Height();	
				}			
#endif //LAF_MODIFY
  			dataInfo->iKeypad.iRects.AppendL(keyRect);
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
  	dataInfo->iKeypad.iKaypadRect.iBr.iY += keyLastRowHeight;
   	iLayoutDataInfo.AppendL( dataInfo );    	
	}
// ---------------------------------------------------------------------------
// CPeninputCnLafDataFSQ::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputCnLafDataFSQ* CPeninputCnLafDataFSQ::NewL( TInt aLayoutType )
	{
    CPeninputCnLafDataFSQ* self = NewLC( aLayoutType );
    CleanupStack::Pop();
    return self;		
	}
// ---------------------------------------------------------------------------
// CPeninputCnLafDataFSQ::NewLC( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputCnLafDataFSQ* CPeninputCnLafDataFSQ::NewLC( TInt aLayoutType )
	{
	CPeninputCnLafDataFSQ* self = new ( ELeave ) CPeninputCnLafDataFSQ( aLayoutType );
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
		
	}

CPeninputCnLafDataFSQ::~CPeninputCnLafDataFSQ()
	{
	DestoryDataCommon();	
	}	
	
// ---------------------------------------------------------------------------
// CPeninputCnLafDataFSQ::CPeninputCnLafDataFSQ( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputCnLafDataFSQ::CPeninputCnLafDataFSQ( TInt aLayoutType )
	{
	SetLayoutType( aLayoutType );	
	}
// ---------------------------------------------------------------------------
// CPeninputCnLafDataFSQ::ReadLafInfo()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
void CPeninputCnLafDataFSQ::ReadLafInfoL()
	{
	// clean up all the data
	iLayoutDataInfo.ResetAndDestroy();
	
	CPeninputCnQwtLayoutDataInfo* dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;
	TAknWindowLineLayout appWnd; 
    TAknWindowLineLayout wndLayout;
    TAknWindowLineLayout keypad, cellpane;
    
    // Modify begin
    TAknWindowLineLayout cellPaneBK;
    TAknLayoutRect cellPaneBKRect;
    TRect keyBKRect;
    // Modify end
    
    TAknLayoutRect keypadRect, cellpaneRect;
    TAknLayoutRect wndRect;
    TAknLayoutText keyTextLayout; 
	TAknTextLineLayout keyText;
	TRect keyRect;

	// -----------------------Landscape Mode--------------------------
	appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// ==================10x3====================
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout10x3, 0, *dataInfo );
	dataInfo->iQwtRect = wndRect.Rect();
	// keypad
	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(0).LayoutLine();
	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	//dataInfo->iKeypad.iTxtRect = keyText.TextRect();

    keypad = AknLayoutScalable_Avkon::vkb2_area_keypad_pane(0).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
  	
  	// Modify begin
  	cellPaneBK = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_g1(0, 0, 0).LayoutLine();
  	// Modify end
  	
  	//iKeypadRect10x3 = keypadRect.Rect();
  	for( TInt i = 0; i < KKeypadLayoutCn[0][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayoutCn[0][0]; j++)
  			{
  			cellpane = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane(0, j, i).LayoutLine();
  			cellpaneRect.LayoutRect(keypadRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
  			
  			// Modify begin
            cellPaneBKRect.LayoutRect( keyRect, cellPaneBK );
            keyBKRect = cellPaneBKRect.Rect();
            dataInfo->iKeypad.iRects.AppendL( keyBKRect );
            // Modify end
  			
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);	

  	// Add candate list laf data for ITI features
	// Candidate list's parent Layout window is ITUT window
	TPeninputCnCandidateListLayoutData candidateListLAF;
	TAknWindowLineLayout parentWnd;
	TBool isLandscape = Layout_Meta_Data::IsLandscapeOrientation();
	// Add tooltip box laf data for ITI features.
	TPeninputCnTooltipBoxLayoutData tooltipBoxLAF;
	if (isLandscape)
		{
		parentWnd
				= AknLayoutScalable_Apps::popup_vitu2_window( 1 ).LayoutLine();
		TAknLayoutRect parentRectLayout;
		parentRectLayout.LayoutRect( appWnd.Rect(), parentWnd );
		candidateListLAF = ReadLafForCandidateList( parentRectLayout.Rect() );
		dataInfo->iCandidateList = candidateListLAF;
		tooltipBoxLAF = ReadLafForTooltipBox( wndRect.Rect() );
		dataInfo->iTooltipBox = tooltipBoxLAF;
		}
  	
   	iLayoutDataInfo.AppendL( dataInfo ); 
  	
	// ==================11x3====================
	dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout11x3, 1, *dataInfo );	
	dataInfo->iQwtRect = wndRect.Rect();
	// keypad
    keypad = AknLayoutScalable_Avkon::vkb2_area_keypad_pane(1).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
  	
	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(1).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText;	
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	
  	// Modify begin
  	cellPaneBK = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_g1(1, 0, 0).LayoutLine();
  	// Modify end
  	
  	for( TInt i = 0; i < KKeypadLayoutCn[1][1]; i++)
		{
		for( TInt j = 0; j < KKeypadLayoutCn[1][0]; j++)
			{
  			cellpane = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane(1, j, i).LayoutLine();
  			cellpaneRect.LayoutRect(keypadRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
  			
  			// Modify begin
  			cellPaneBKRect.LayoutRect( keyRect, cellPaneBK );
  			keyBKRect = cellPaneBKRect.Rect();
  			dataInfo->iKeypad.iRects.AppendL( keyBKRect );
  			// Modify end
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
			}
		}

    if ( isLandscape )
        {
      	// Add candate list laf data for ITI features
       	dataInfo->iCandidateList = candidateListLAF;
       	// Add tooltip box laf data for ITI features.
       	dataInfo->iTooltipBox = tooltipBoxLAF;
        }  	
  	
   	iLayoutDataInfo.AppendL( dataInfo ); 
	
	// ==================11x4====================
	dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout11x4, 2, *dataInfo );
	dataInfo->iQwtRect = wndRect.Rect();
	// keypad
    keypad = AknLayoutScalable_Avkon::vkb2_area_keypad_pane(2).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();

 	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(2).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText; 	
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);  	
  	
  	// Modify begin
  	cellPaneBK = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_g1(2, 0, 0).LayoutLine();
  	// Modify end
  	
  	for( TInt i = 0; i < KKeypadLayoutCn[2][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayoutCn[2][0]; j++)
  			{
  			cellpane = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane(2, j, i).LayoutLine();
  			cellpaneRect.LayoutRect(keypadRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
  			
  			// Modify begin
  			cellPaneBKRect.LayoutRect( keyRect, cellPaneBK );
  			keyBKRect = cellPaneBKRect.Rect();
  			dataInfo->iKeypad.iRects.AppendL( keyBKRect );
  			// Modify end            
  			
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}

    if ( isLandscape )
        {
      	// Add candate list laf data for ITI features
       	dataInfo->iCandidateList = candidateListLAF;
       	// Add tooltip box laf data for ITI features.
       	dataInfo->iTooltipBox = tooltipBoxLAF;
        }  	
    
   	iLayoutDataInfo.AppendL( dataInfo ); 

   	// ==================10x3 with candi pane====================
	dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout10x3, 0, *dataInfo );
	dataInfo->iQwtRect = wndRect.Rect();
	
	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(0).LayoutLine();
	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);

    keypad = AknLayoutScalable_Avkon::vkb2_area_keypad_pane(6).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
  	
    // Modify begin
    cellPaneBK = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_g1(0, 0, 0).LayoutLine();
    // Modify end
   
  	TInt keyHeight = keypadRect.Rect().Height()/3;  	
   	for( TInt i = 0; i < KKeypadLayoutCn[0][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayoutCn[0][0]; j++)
  			{
  			cellpane = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane(0, j, 0).LayoutLine();
  			cellpaneRect.LayoutRect(keypadRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.SetHeight( keyHeight );
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY + keyHeight*i);  			
  			
            // Modify begin
            cellPaneBKRect.LayoutRect( keyRect, cellPaneBK );
            keyBKRect = cellPaneBKRect.Rect();
            dataInfo->iKeypad.iRects.AppendL( keyBKRect );
            // Modify end
  			
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
  	dataInfo->iKeypad.iKaypadRect.SetHeight( keyHeight* 3 );	
  	// calculate the text pos offset since no right laf for it	
	TInt offsetTextY = cellpaneRect.Rect().Height() - keyHeight;
	dataInfo->iKeypad.iTextLayout.iB += offsetTextY - 2;
	
    if ( isLandscape )
        {
      	// Add candate list laf data for ITI features
       	dataInfo->iCandidateList = candidateListLAF;
       	// Add tooltip box laf data for ITI features.
       	dataInfo->iTooltipBox = tooltipBoxLAF;
        }  	
    
   	iLayoutDataInfo.AppendL( dataInfo ); 	   	
   	
   	// ==================10x4(not create by laf)====================
	dataInfo = new( ELeave ) CPeninputCnQwtLayoutDataInfo;
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout10x3, 0, *dataInfo );
	dataInfo->iQwtRect = wndRect.Rect();
	
	// keypad
	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(0).LayoutLine();
	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);

    keypad = AknLayoutScalable_Avkon::vkb2_area_keypad_pane(6).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();

    // Modify begin
    cellPaneBK = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_g1(0, 0, 0).LayoutLine();
    // Modify end  	
  	
  	keyHeight = keypadRect.Rect().Height()/3;
  	for( TInt i = 0; i < KKeypadLayoutCn[3][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayoutCn[3][0]; j++)
  			{
  			cellpane = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane(0, j, 0).LayoutLine();
  			cellpaneRect.LayoutRect(keypadRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.SetHeight( keyHeight );
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY + keyHeight*i);  			
  			
            // Modify begin
            cellPaneBKRect.LayoutRect( keyRect, cellPaneBK );
            keyBKRect = cellPaneBKRect.Rect();
            dataInfo->iKeypad.iRects.AppendL( keyBKRect );
            // Modify end
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
  	dataInfo->iKeypad.iKaypadRect.SetHeight( keyHeight*4 );	
  	// calculate the text pos offset since no right laf for it	
	offsetTextY = cellpaneRect.Rect().Height() - keyHeight;
	dataInfo->iKeypad.iTextLayout.iB += offsetTextY - 2;
	
    if ( isLandscape )
        {
      	// Add candate list laf data for ITI features
       	dataInfo->iCandidateList = candidateListLAF;
       	// Add tooltip box laf data for ITI features.
       	dataInfo->iTooltipBox = tooltipBoxLAF;
        }
    
   	iLayoutDataInfo.AppendL( dataInfo ); 
	}	

// ---------------------------------------------------------------------------
// Read LAF data for candidate list
// ---------------------------------------------------------------------------
//        
TPeninputCnCandidateListLayoutData CPeninputCnLafDataFSQ::
                                 ReadLafForCandidateList( const TRect& aRect )
    {    
    // candidate list
    TAknWindowLineLayout candidateList 
                 = AknLayoutScalable_Apps::popup_vitu2_match_list_window( 3 )
                   .LayoutLine();    
    TAknLayoutRect candidateListRect;
    candidateListRect.LayoutRect( aRect, candidateList );    
    
    // list pane, its parent is candidate list
    TAknWindowLineLayout listpane
                 = AknLayoutScalable_Apps::list_vitu2_match_list_pane( 1 )
                   .LayoutLine();
    TAknLayoutRect listpaneRect;
    listpaneRect.LayoutRect( candidateListRect.Rect(), listpane );
    
    // scroll pane, its parent is candidate list
    TAknWindowLineLayout scrollpane
                 = AknLayoutScalable_Apps::vitu2_page_scroll_pane( 1 )
                   .LayoutLine();
    TAknLayoutRect scrollRect;
    scrollRect.LayoutRect( candidateListRect.Rect(), scrollpane );
    
    // Item pane, its parent is list pane
    TAknWindowLineLayout itemPane 
         = AknLayoutScalable_Apps::list_vitu2_match_list_item_pane( 0, 0, 0 )
            .LayoutLine();
    TAknLayoutRect itemPaneRect;
    itemPaneRect.LayoutRect( listpaneRect.Rect(), itemPane );

    TPeninputCnCandidateListLayoutData layoutData;    
    layoutData.iSelectionHorMargin = listpaneRect.Rect().iTl.iX;    
    layoutData.iSelectionVerMargin = listpaneRect.Rect().iTl.iY;        
    layoutData.iSelectionItemSize = itemPaneRect.Rect().Size();
    
    // Up button image. its parent is scroll pane
    TAknWindowLineLayout arrowUpImagePane
                    = AknLayoutScalable_Apps::vitu2_page_scroll_pane_g1()
                      .LayoutLine();
    TAknLayoutRect arrowUpImageRect;
    arrowUpImageRect.LayoutRect( scrollRect.Rect(), arrowUpImagePane );
    // Up button, its parent is scroll pane
    TAknWindowLineLayout arrowUpPane
              = AknLayoutScalable_Apps::bg_button_pane_cp023().LayoutLine();
    TAknLayoutRect arrowUpRect;
    arrowUpRect.LayoutRect( scrollRect.Rect(), arrowUpPane );
    
    // Down button image, its parent is scroll pane
    TAknWindowLineLayout arrowDownImagePane
          = AknLayoutScalable_Apps::vitu2_page_scroll_pane_g2().LayoutLine();
    TAknLayoutRect arrowDownImageRect;
    arrowDownImageRect.LayoutRect( scrollRect.Rect(), arrowDownImagePane );
    
    // Down button, its parent is scroll pane
    TAknWindowLineLayout arrowDownPane 
               = AknLayoutScalable_Apps::bg_button_pane_cp024().LayoutLine();
    TAknLayoutRect arrowDownRect;
    arrowDownRect.LayoutRect( scrollRect.Rect(), arrowDownPane );
    
    layoutData.iSelectionNaviSize = arrowUpRect.Rect().Size();
    layoutData.iSelectionNaviInnerSize = arrowUpImagePane.Rect().Size();
    
    layoutData.iSelectionTextLine 
               = AknLayoutScalable_Apps::list_vitu2_match_list_item_pane_t1()
                 .LayoutLine();
    layoutData.iSelectionPageTextLine 
          = AknLayoutScalable_Apps::vitu2_page_scroll_pane_t1().LayoutLine();
            
    TAknLayoutText list_item_pane_t1_layout_text;
    list_item_pane_t1_layout_text.LayoutText( listpaneRect.Rect(), 
                                              layoutData.iSelectionTextLine );
    layoutData.iFont = list_item_pane_t1_layout_text.Font();
    return layoutData;
    }

// ---------------------------------------------------------------------------
// Read LAF data for tooltip box
// ---------------------------------------------------------------------------
// 
TPeninputCnTooltipBoxLayoutData CPeninputCnLafDataFSQ::
                                    ReadLafForTooltipBox( const TRect& aRect )
    {    
    TAknWindowLineLayout tooltipBoxLayout 
       = AknLayoutScalable_Avkon::popup_fep_tooltip_window( 0 ).LayoutLine();
    
    TAknLayoutRect tooltipBoxRect;
    tooltipBoxRect.LayoutRect( aRect, tooltipBoxLayout );
    
    TPeninputCnTooltipBoxLayoutData layoutData;
    layoutData.iRect = tooltipBoxRect.Rect();
        
    layoutData.iTooltipTextLayout 
                  = AknLayoutScalable_Avkon::popup_fep_tooltip_window_t1( 0 )
                    .LayoutLine();     
    return layoutData;
    }
