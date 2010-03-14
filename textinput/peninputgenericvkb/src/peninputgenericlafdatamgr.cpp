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
#include <peninputpluginutils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>

// User includes
#include "peninputgenericvkblafdatamgr.h"

#define LAF_MODIFY 1

const TInt KKeypadLayout[][2] = 
	{
		{ 10, 3 },		// 10x3
		{ 11, 3 },		// 11x3
		{ 11, 4 },		// 11x4
	};
	
const TInt KVkbTopAndBottomParam[] = { 5, 3, 4 };	
typedef TAknWindowComponentLayout (*Pfun)(TInt aVariety, TInt aCol = 0, TInt aRow = 0);

const Pfun cell_pane_func[]=
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
// CPeninputGenericVkbLafMgr::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLafMgr* CPeninputGenericVkbLafMgr::NewL()
    {
    CPeninputGenericVkbLafMgr* self = NewLC();
    CleanupStack::Pop(self);
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::NewLC
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLafMgr* CPeninputGenericVkbLafMgr::NewLC()
    {
    CPeninputGenericVkbLafMgr* self = 
        new ( ELeave ) CPeninputGenericVkbLafMgr();
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::CPeninputGenericVkbLafMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLafMgr::CPeninputGenericVkbLafMgr():
    iInputMode(EPluginInputModeNone)
    {
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::~CPeninputGenericVkbLafMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLafMgr::~CPeninputGenericVkbLafMgr()
    {
	delete iLafData;
	iLafData = NULL;    
    }

TBool CPeninputGenericVkbLafMgr::IsValid()
    {
    if (!iLafData) 
    return EFalse;
    
    return iLafData->IsValid();
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLafMgr::ConstructL()
	{
		
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::SetInputMode
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLafMgr::SetInputModeL( TPluginInputMode aInputMode)
	{
	if (iInputMode == aInputMode)
	    {
	    return;
	    }
	
	
	if (iLafData)
	    {
	    delete iLafData;
	    iLafData = NULL;
	    }
	    
     if (aInputMode == EPluginInputModeVkb)
        {
        iLafData = CPeninputLafDataVKB::NewL(ELayoutNone);
        }
     else if (aInputMode == EPluginInputModeFSQ)
        {
        iLafData = CPeninputLafDataFSQ::NewL(ELayoutNone);
        }
     else 
        {
        User::Leave(KErrNotSupported);
        }
	}	

TPluginInputMode CPeninputGenericVkbLafMgr::InputMode() const
    {
    return iInputMode;
    }
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::SetLayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLafMgr::SetLayoutType( TPeninputQwtLayoutType aLayoutType )
	{
	ASSERT(iLafData);
	iLafData->SetLayoutType( aLayoutType );	
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::SetLafOption
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLafMgr::SetLafOption( TPeninputVkbLafOption aOption )
	{
	if( iLafData->LafOption() == aOption )
		return;
	
	iLafData->SetLafOption( aOption );	
	TRAP_IGNORE(iLafData->ConstructL());
	}

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::SetLafOption
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TPeninputVkbLafOption CPeninputGenericVkbLafMgr::OptionType() const
	{
	return iLafData->LafOption();
	}

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::GetButtonRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLafMgr::GetButtonRect( TPeninputCommonCtrlID aButtonID, 
								TRect& aOutterRect,
								TRect& aInnerRect )
	{
	ASSERT(iLafData);		
	const TPeninputButtonLayoutData& data = iLafData->ButtonLayoutData( aButtonID );
	aOutterRect = data.iRect;
	aInnerRect  = data.iInnerRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::KeyRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TRect& CPeninputGenericVkbLafMgr::KeypadRect() const
	{
	ASSERT(iLafData);
	const TPeninputKeyLayoutData& data = iLafData->KeypadLayoutData();
	return data.iKaypadRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::ICFRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
const TRect& CPeninputGenericVkbLafMgr::ICFRect() const
	{
	ASSERT(iLafData);
	return iLafData->ICFLayoutData().iRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::ICFPromptTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
const CFont* CPeninputGenericVkbLafMgr::ICFPromptTextFont() const
	{
	ASSERT(iLafData);
	return iLafData->ICFLayoutData().iPromptTextFont;
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::ICFTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
const CFont* CPeninputGenericVkbLafMgr::ICFTextFont() const
	{
	ASSERT(iLafData);
	return iLafData->ICFLayoutData().iTextFont;
	}


// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::KeyTextLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
TAknTextLineLayout CPeninputGenericVkbLafMgr::KeyTextLayout() const
	{
	ASSERT(iLafData);
	return iLafData->KeypadLayoutData().iTextLayout;
	}

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::ICFPromptTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
void CPeninputGenericVkbLafMgr::GetICFConfig( TPeninputICFConfig& aICFConfig )
	{
	ASSERT(iLafData);	
	const TPeninputEntryLayoutData& icfLayout = iLafData->ICFLayoutData();
	aICFConfig.iLeftMargin = icfLayout.iPromptTextRect.iTl.iX - icfLayout.iRect.iTl.iX;
	aICFConfig.iRightMargin = icfLayout.iRect.iBr.iX - icfLayout.iPromptTextRect.iBr.iX;
	aICFConfig.iTopMargin = icfLayout.iPromptTextRect.iTl.iY - icfLayout.iRect.iTl.iY;
	aICFConfig.iTextLineHeight = icfLayout.iPromptTextRect.Height();
	aICFConfig.iLineSpaceMargin = icfLayout.iTextRect.iTl.iY - icfLayout.iPromptTextRect.iBr.iY;
	}
	
//		
const TRect& CPeninputGenericVkbLafMgr::EntirePaneRect() const
	{
	ASSERT(iLafData);
	return iLafData->QwtLayoutData().iQwtRect;
	}

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::KeyRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLafMgr::GetKeyRect( TInt aColume, 
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

TPeninputQwtLayoutType CPeninputGenericVkbLafMgr::LayoutTypeFromRowsColumns(const TInt aRows, 
                                                                            const TInt aColumns)
    {
    TPeninputQwtLayoutType ret;
    
    if (aRows == 3 && aColumns == 10)
        {
        ret = ELayout10x3;
        }
    else if (aRows == 3 && aColumns == 11)
        {
        ret = ELayout11x3;
        }
    else if (aRows == 4 && aColumns == 11)    
        {
        ret = ELayout11x4;
        }
    else
        {
        ret = ELayoutNone;
        }

     return ret;
    }
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::KeyTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const CFont* CPeninputGenericVkbLafMgr::KeyTextFont() const
	{
	ASSERT(iLafData);
	return iLafData->KeypadLayoutData().iFont;	
	}

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::KeyTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TRect& CPeninputGenericVkbLafMgr::RangbarRect() const
	{
	ASSERT(iLafData);
	return iLafData->RangbarLayoutData().iRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::KeyTextFont
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TRect& CPeninputGenericVkbLafMgr::TitlebarRect() const
	{
	ASSERT(iLafData);
	return iLafData->QwtLayoutData().iTitlebarRect;
	}

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::ClientRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TRect& CPeninputGenericVkbLafMgr::ClientRect() const
	{
	ASSERT(iLafData);
	return iLafData->QwtLayoutData().iClientRect;
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::RangbarCellCount
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputGenericVkbLafMgr::RangbarCellCount() const
	{
	ASSERT(iLafData);	
	return 	iLafData->RangbarLayoutData().iCellDataArray.Count();
	}

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::RangbarCellCount
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLafMgr::RangbarCellAt( TInt aIndex, 
												 TRect& aOutterRect, 
												 TRect& aInnerRect )
	{
	ASSERT(iLafData);	
	aOutterRect = iLafData->RangbarLayoutData().iCellDataArray[aIndex].iRect;
	aInnerRect  = iLafData->RangbarLayoutData().iCellDataArray[aIndex].iInnerRect;  
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLafMgr::GetSpaceButtonExLayoutInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLafMgr::GetSpaceButtonExLayoutInfo( TRect& aIconsFrameRect, 
															TRect& aMiddleIconRect ) const
	{
	ASSERT(iLafData);
	aIconsFrameRect = iLafData->QwtLayoutData().iSpaceButtonEx.iIconsFrameRect;
	aMiddleIconRect = iLafData->QwtLayoutData().iSpaceButtonEx.iMiddleIconRect;		
	}

// ---------------------------------------------------------------------------
// Return layout data for candidate list
// ---------------------------------------------------------------------------
//
const TPeninputCandidateListLayoutData& 
                   CPeninputGenericVkbLafMgr::CandidateListLayoutData() const
    {
    ASSERT(iLafData);
    return iLafData->QwtLayoutData().iCandidateList;
    }
	
// ---------------------------------------------------------------------------
// Return layout data for candidate list
// ---------------------------------------------------------------------------
//
const TPeninputTooltipBoxLayoutData& 
                      CPeninputGenericVkbLafMgr::TooltipBoxLayoutData() const
    {
    ASSERT(iLafData);
    return iLafData->QwtLayoutData().iTooltipBox;
    }

// ---------------------------------------------------------------------------
// CPeninputQwtLayoutDataInfo::~CPeninputQwtLayoutDataInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputQwtLayoutDataInfo::~CPeninputQwtLayoutDataInfo()
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
void MPeninputLafDataCommon::DestoryDataCommon()
	{
	iLayoutDataInfo.ResetAndDestroy();
	iLayoutDataInfo.Close();
	}
	
MPeninputLafDataCommon::~MPeninputLafDataCommon()	
	{
		
	}
// CPeninputLafDataCommon::ButtonLayoutData(TInt aButtonID)
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TPeninputButtonLayoutData& MPeninputLafDataCommon::ButtonLayoutData( TInt aButtonID ) const
	{
	ASSERT( IsValid() );
	const TPeninputButtonLayoutData* pData = NULL;

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
const TPeninputEntryLayoutData& MPeninputLafDataCommon::ICFLayoutData() const
	{
	ASSERT( IsValid() );
	return iLayoutDataInfo[iLayoutType]->iICF;
	}
	
// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::KeypadLayoutData()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
const TPeninputKeyLayoutData& MPeninputLafDataCommon::KeypadLayoutData() const
	{
	ASSERT( IsValid() );
	return iLayoutDataInfo[iLayoutType]->iKeypad;
	}
	
// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::RangbarLayoutData(TInt aRangbarID)
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
const TPeninputRangeBarLayoutData& MPeninputLafDataCommon::RangbarLayoutData() const
	{
	ASSERT( IsValid() );
	return iLayoutDataInfo[iLayoutType]->iRangbar;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::RangbarLayoutData(TInt aRangbarID)
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
const CPeninputQwtLayoutDataInfo& MPeninputLafDataCommon::QwtLayoutData() const
	{
	ASSERT( IsValid() );
	return 	*iLayoutDataInfo[iLayoutType];
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::ConstructL()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
void MPeninputLafDataCommon::ConstructL()
	{
	ReadLafInfoL();
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::ConstructL()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
TBool MPeninputLafDataCommon::IsValid() const
	{
	return ( iLayoutDataInfo.Count() && iLayoutType != ELayoutNone ) ? ETrue : EFalse;
	}


// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::GetTopAndBottomPaneInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
void MPeninputLafDataCommon::GetTopAndBottomPaneInfo( const TRect& aParentWndRect, 
													  TInt aLayoutType,
													  TInt aLayoutParam,
													  CPeninputQwtLayoutDataInfo& aDataInfo )
	{
	const TInt endFSQType  = 2;



	// judge which case 	
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
	TRect innerRect;
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
	AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
	aDataInfo.iCloseButton.iRect = cellBtnRect.Rect();
	aDataInfo.iCloseButton.iInnerRect = innerRect;
	// option button
	cellBtn = AknLayoutScalable_Avkon::vkb2_top_cell_left_pane(aLayoutParam, 0 ,1).LayoutLine();
	cellBtnRect.LayoutRect(topleftpaneRect.Rect(), cellBtn);
	AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
	aDataInfo.iOptionButton.iRect = cellBtnRect.Rect();
	aDataInfo.iOptionButton.iInnerRect = innerRect;
	// entry pane
	topentrypane = AknLayoutScalable_Avkon::vkb2_top_entry_pane(aLayoutParam).LayoutLine();
	topentrypaneRect.LayoutRect(toppaneRect.Rect(), topentrypane);	
	aDataInfo.iICF.iRect = topentrypaneRect.Rect();
	cellText = AknLayoutScalable_Avkon::vkb2_top_entry_pane_t1(aLayoutParam).LayoutLine();
	aDataInfo.iICF.iPromptTextFont = AknLayoutUtils::FontFromId(cellText.iFont, NULL);
	txt.LayoutText( topentrypaneRect.Rect(), cellText );
	aDataInfo.iICF.iPromptTextRect = txt.TextRect();
	cellText = AknLayoutScalable_Avkon::vkb2_top_entry_pane_t2(aLayoutParam).LayoutLine();
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
		AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );	
		aDataInfo.iClearButton.iRect = cellBtnRect.Rect();
		aDataInfo.iClearButton.iInnerRect = innerRect;
		// move button (VKB)			
		cellBtn = AknLayoutScalable_Avkon::
				  vkb2_top_grid_right_pane_g4(aLayoutParam).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
		aDataInfo.iMoveButton.iRect = cellBtnRect.Rect();
		aDataInfo.iMoveButton.iInnerRect = innerRect;									
										
		}
	else
		{
		// clear button	
		cellBtn = AknLayoutScalable_Avkon::vkb2_top_grid_right_pane_g1(aLayoutType).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
		aDataInfo.iClearButton.iRect = cellBtnRect.Rect();
		aDataInfo.iClearButton.iInnerRect = innerRect;			
		// left button (FSQ)
		cellBtn = AknLayoutScalable_Avkon::vkb2_top_grid_right_pane_g2(aLayoutType).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
		aDataInfo.iLeftButton.iRect = cellBtnRect.Rect();
		aDataInfo.iLeftButton.iInnerRect = innerRect;
		// right button (FSQ)
		cellBtn = AknLayoutScalable_Avkon::vkb2_top_grid_right_pane_g3(aLayoutType).LayoutLine();
		cellBtnRect.LayoutRect(toprightpaneRect.Rect(), cellBtn);
		AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
		aDataInfo.iRightButton.iRect = cellBtnRect.Rect();
		aDataInfo.iRightButton.iInnerRect = innerRect;		
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
  	
	TRect bottomRect = bottompaneRect.Rect();

	// Get keypad rect
	keypadRect.SetRect( toppaneRect.Rect().iTl.iX, 
  						toppaneRect.Rect().iBr.iY, 
  						bottomRect.iBr.iX,
						bottomRect.iTl.iY);
	aDataInfo.iClientRect.SetRect( toppaneRect.Rect().iTl.iX, 
  								   toppaneRect.Rect().iBr.iY,
  								   bottomRect.iBr.iX,
								   bottomRect.iBr.iY);
	if( isVKB )
		{
		aDataInfo.iQwtRect.SetRect( toppaneRect.Rect().iTl.iX, 
  								    toppaneRect.Rect().iTl.iY,
  								    bottomRect.iBr.iX,
								    bottomRect.iBr.iY );	
		}
	else
		{
		aDataInfo.iQwtRect = aParentWndRect;	
		}
				
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
  	bottomgridRect.LayoutRect(bottomRect, bottomgrid);
	
	TInt maxParam = iLafOption & EOption3CellRange ? 4 : 3;
	// shift button  	
	cellBtn = AknLayoutScalable_Avkon::vkb2_cell_bottom_grid_pane(aLayoutParam, 0, 0).LayoutLine();
	cellBtnRect.LayoutRect(bottomgridRect.Rect(), cellBtn);
	AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
	aDataInfo.iShiftButton.iRect = cellBtnRect.Rect();
	aDataInfo.iShiftButton.iInnerRect = innerRect;
	
	TPeninputButtonLayoutData btnLayoutData;		
	for( TInt i = 1; i < maxParam; i++ )
		{
#ifdef LAF_MODIFY	
		if( i == 3 )
			{
			btnLayoutData.iRect	= aDataInfo.iRangbar.iCellDataArray[i-2].iRect;
			btnLayoutData.iRect.Move( btnLayoutData.iRect.Width(), 0 );
			AknPenImageUtils::CalculateGraphicRect( btnLayoutData.iRect, innerRect );
			btnLayoutData.iInnerRect = innerRect;
			aDataInfo.iRangbar.iCellDataArray.Append( btnLayoutData );
			break;
			}
#endif //LAF_MODIFY		
		// range cell
		cellBtn = AknLayoutScalable_Avkon::vkb2_cell_bottom_grid_pane(aLayoutParam, i, 0).LayoutLine();
		cellBtnRect.LayoutRect(bottomgridRect.Rect(), cellBtn);
		AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
		btnLayoutData.iRect = cellBtnRect.Rect();
		btnLayoutData.iInnerRect = innerRect;	

		aDataInfo.iRangbar.iCellDataArray.Append( btnLayoutData );				
		}
		
	TInt count = aDataInfo.iRangbar.iCellDataArray.Count();
	aDataInfo.iRangbar.iRect = TRect( aDataInfo.iRangbar.iCellDataArray[0].iRect.iTl,
									  aDataInfo.iRangbar.iCellDataArray[count - 1].iRect.iBr );
	
		// input mode button ?? need modified later. inner rect is same as outter rect
	cellBtn = AknLayoutScalable_Avkon::vkb2_area_bottom_pane_g1(bottomLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(bottomRect, cellBtn);
	AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
	aDataInfo.iSwitchButton.iRect = cellBtnRect.Rect();
	aDataInfo.iSwitchButton.iInnerRect = innerRect;	
		// enter button 
	cellBtn = AknLayoutScalable_Avkon::vkb2_area_bottom_pane_g2(bottomLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(bottomRect, cellBtn);
	AknPenImageUtils::CalculateGraphicRect( cellBtnRect.Rect(), innerRect );
	aDataInfo.iEnterButton.iRect = cellBtnRect.Rect();
	aDataInfo.iEnterButton.iInnerRect = innerRect;	
		// space button 
	cellBtn = AknLayoutScalable_Avkon::vkb2_area_bottom_pane_g3(bottomLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(bottomRect, cellBtn);
	aDataInfo.iSpaceButton.iRect = cellBtnRect.Rect();
	aDataInfo.iSpaceButton.iRect.iTl = TPoint( aDataInfo.iRangbar.iRect.iBr.iX,
											   aDataInfo.iRangbar.iRect.iTl.iY );
	aDataInfo.iSpaceButton.iInnerRect = cellBtnRect.Rect();	  			
	// 3 pieces graphics laf info
	TRect spaceRect, iconRect, tempRect;
	cellBtn = 
        AknLayoutScalable_Avkon::vkb2_area_bottom_space_btn_pane(bottomLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(bottomRect, cellBtn);
	spaceRect = cellBtnRect.Rect();
	cellBtn = 
        AknLayoutScalable_Avkon::vkb2_area_bottom_space_btn_pane_g1(bottomLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(spaceRect, cellBtn);
	iconRect = cellBtnRect.Rect();
	TInt nOffsetX = iconRect.iTl.iX - spaceRect.iTl.iX;
	tempRect = spaceRect;
	tempRect.Shrink( nOffsetX, 0 );
	aDataInfo.iSpaceButtonEx.iIconsFrameRect = tempRect;
	tempRect.Shrink( iconRect.Width(), 0 );
	aDataInfo.iSpaceButtonEx.iMiddleIconRect = tempRect;  	
  	
	// preview popup window 
	TAknWindowLineLayout previewWnd, previewWndInner;
	TAknLayoutRect previewWndRect, previewWndInnerRect;
	previewWnd = AknLayoutScalable_Avkon::popup_fep_char_preview_window(aLayoutParam).LayoutLine();
	previewWndRect.LayoutRect( aParentWndRect, previewWnd );
	previewWndInner = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_g9().LayoutLine();
	previewWndInnerRect.LayoutRect( previewWndRect.Rect(), previewWndInner );
	aDataInfo.iPreviewWndRect = previewWndRect.Rect();
	aDataInfo.iPreviewWndInnerRect = previewWndInnerRect.Rect();
	aDataInfo.iPreviewWndText = AknLayoutScalable_Avkon::popup_fep_char_preview_window_t1(
                              aLayoutParam).LayoutLine();
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::SetLayoutType( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
inline void MPeninputLafDataCommon::SetLayoutType( TInt aLayoutType )
	{
    iLayoutType = aLayoutType;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::LayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline TInt MPeninputLafDataCommon::LayoutType() const
	{
	return iLayoutType;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::SetLafOption
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline void MPeninputLafDataCommon::SetLafOption( TPeninputVkbLafOption aLafOption )
	{
	iLafOption = aLafOption;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataCommon::SetLafOption
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline TPeninputVkbLafOption MPeninputLafDataCommon::LafOption() const
	{
	return iLafOption;
	}

// ---------------------------------------------------------------------------
// CPeninputLafDataVKB::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//	
CPeninputLafDataVKB* CPeninputLafDataVKB::NewL( TInt aLayoutType )
	{
    CPeninputLafDataVKB* self = NewLC( aLayoutType );
    CleanupStack::Pop();
    return self;
	}
// ---------------------------------------------------------------------------
// CPeninputLafDataVKB::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputLafDataVKB* CPeninputLafDataVKB::NewLC( TInt aLayoutType )
	{
	CPeninputLafDataVKB* self = new ( ELeave ) CPeninputLafDataVKB( aLayoutType );
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
	}
CPeninputLafDataVKB::~CPeninputLafDataVKB()
	{
	DestoryDataCommon();	
	}
	
	
// ---------------------------------------------------------------------------
// CPeninputLafDataVKB::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputLafDataVKB::CPeninputLafDataVKB( TInt aLayoutType )
	{
	SetLayoutType( aLayoutType );	
	}
// ---------------------------------------------------------------------------
// CPeninputLafDataVKB::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
void CPeninputLafDataVKB::ReadLafInfoL()
	{
	// clean up all the data
	iLayoutDataInfo.ResetAndDestroy();
	
	CPeninputQwtLayoutDataInfo* dataInfo = new( ELeave ) CPeninputQwtLayoutDataInfo;
	
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
  	for( TInt i = 0; i < KKeypadLayout[0][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayout[0][0]; j++)
  			{
  			rowpane = AknLayoutScalable_Avkon::vkb2_row_keypad_pane(2, 0, i).LayoutLine();
  			rowpaneRect.LayoutRect(keypadRect.Rect(), rowpane);
  			cellpane = (*cell_pane_func[j])(0).LayoutLine();
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
	dataInfo = new( ELeave ) CPeninputQwtLayoutDataInfo;
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
  	for( TInt i = 0; i < KKeypadLayout[1][1]; i++)
		{
		for( TInt j = 0; j < KKeypadLayout[1][0]; j++)
			{
			rowpane = AknLayoutScalable_Avkon::vkb2_row_keypad_pane(0, 0, i).LayoutLine();
			rowpaneRect.LayoutRect(keypadRect.Rect(), rowpane);
			cellpane = (*cell_pane_func[j])(1).LayoutLine();
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
	dataInfo = new( ELeave ) CPeninputQwtLayoutDataInfo;
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
  	for( TInt i = 0; i < KKeypadLayout[2][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayout[2][0]; j++)
  			{
  			rowpane = AknLayoutScalable_Avkon::vkb2_row_keypad_pane(1, 0, i).LayoutLine();
  			rowpaneRect.LayoutRect(keypadRect.Rect(), rowpane);
  			cellpane = (*cell_pane_func[j])(1).LayoutLine();
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
	}
// ---------------------------------------------------------------------------
// CPeninputLafDataFSQ::NewL( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputLafDataFSQ* CPeninputLafDataFSQ::NewL( TInt aLayoutType )
	{
    CPeninputLafDataFSQ* self = NewLC( aLayoutType );
    CleanupStack::Pop();
    return self;		
	}
// ---------------------------------------------------------------------------
// CPeninputLafDataFSQ::NewLC( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputLafDataFSQ* CPeninputLafDataFSQ::NewLC( TInt aLayoutType )
	{
	CPeninputLafDataFSQ* self = new ( ELeave ) CPeninputLafDataFSQ( aLayoutType );
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
		
	}

CPeninputLafDataFSQ::~CPeninputLafDataFSQ()
	{
	DestoryDataCommon();	
	}	
	
// ---------------------------------------------------------------------------
// CPeninputLafDataFSQ::CPeninputLafDataFSQ( TInt aLayoutType )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
CPeninputLafDataFSQ::CPeninputLafDataFSQ( TInt aLayoutType )
	{
	SetLayoutType( aLayoutType );	
	}
// ---------------------------------------------------------------------------
// CPeninputLafDataFSQ::ReadLafInfo()
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//		
void CPeninputLafDataFSQ::ReadLafInfoL()
	{
	// clean up all the data
	iLayoutDataInfo.ResetAndDestroy();
	
	CPeninputQwtLayoutDataInfo* dataInfo = new( ELeave ) CPeninputQwtLayoutDataInfo;
	TAknWindowLineLayout appWnd; 
    TAknWindowLineLayout wndLayout;
    TAknWindowLineLayout keypad, cellpane;
    
    TAknWindowLineLayout cellPaneBK;
    TAknLayoutRect cellPaneBKRect;
    TRect keyBKRect;
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
	// keypad
	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(0).LayoutLine();
	dataInfo->iKeypad.iTextLayout = keyText;
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	//dataInfo->iKeypad.iTxtRect = keyText.TextRect();

    keypad = AknLayoutScalable_Avkon::vkb2_area_keypad_pane(0).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
  	//iKeypadRect10x3 = keypadRect.Rect();
  	cellPaneBK = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_g1(0, 0, 0).LayoutLine();
  	for( TInt i = 0; i < KKeypadLayout[0][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayout[0][0]; j++)
  			{
  			cellpane = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane(0, j, i).LayoutLine();
  			cellpaneRect.LayoutRect(keypadRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
            cellPaneBKRect.LayoutRect( keyRect, cellPaneBK );
            keyBKRect = cellPaneBKRect.Rect();
            dataInfo->iKeypad.iRects.AppendL(keyBKRect);
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
 	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(0).LayoutLine();
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	//dataInfo->iKeypad.iTxtRect = keyText.TextRect();	
  	
  	// Add candate list laf data for ITI features
  	// Candidate list's parent Layout window is ITUT window
    TPeninputCandidateListLayoutData candidateListLAF;
    TAknWindowLineLayout parentWnd;
    TBool isLandscape = Layout_Meta_Data::IsLandscapeOrientation();
   	// Add tooltip box laf data for ITI features.
   	TPeninputTooltipBoxLayoutData tooltipBoxLAF;    	                                
    if ( isLandscape )
        {
      	parentWnd = AknLayoutScalable_Apps::popup_vitu2_window( 1 ).LayoutLine();
        TAknLayoutRect parentRectLayout;
        parentRectLayout.LayoutRect( appWnd.Rect(), parentWnd );
        candidateListLAF = ReadLafForCandidateList( parentRectLayout.Rect() );
       	dataInfo->iCandidateList = candidateListLAF;
        tooltipBoxLAF = ReadLafForTooltipBox( wndRect.Rect() );
       	dataInfo->iTooltipBox = tooltipBoxLAF;
        }
   	
   	
   	iLayoutDataInfo.AppendL( dataInfo ); 
  	
	// ==================11x3====================
	dataInfo = new( ELeave ) CPeninputQwtLayoutDataInfo;
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout11x3, 1, *dataInfo );	
	// keypad
    keypad = AknLayoutScalable_Avkon::vkb2_area_keypad_pane(1).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
  	dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
    cellPaneBK = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_g1(1, 0, 0).LayoutLine();
  	
  	for( TInt i = 0; i < KKeypadLayout[1][1]; i++)
		{
		for( TInt j = 0; j < KKeypadLayout[1][0]; j++)
			{
  			cellpane = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane(1, j, i).LayoutLine();
  			cellpaneRect.LayoutRect(keypadRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
            cellPaneBKRect.LayoutRect( keyRect, cellPaneBK );
            keyBKRect = cellPaneBKRect.Rect();
            dataInfo->iKeypad.iRects.AppendL(keyBKRect);
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
			}
		}
	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(1).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText;	
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	//dataInfo->iKeypad.iTxtRect = keyText.TextRect();	  	
  	
    if ( isLandscape )
        {
      	// Add candate list laf data for ITI features
       	dataInfo->iCandidateList = candidateListLAF;
       	// Add tooltip box laf data for ITI features.
       	dataInfo->iTooltipBox = tooltipBoxLAF;
        }
   	

   	iLayoutDataInfo.AppendL( dataInfo ); 
	
	// ==================11x4====================
	dataInfo = new( ELeave ) CPeninputQwtLayoutDataInfo;
	wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb2_window(0).LayoutLine();
	wndRect.LayoutRect(appWnd.Rect(), wndLayout);
	// top pane and bottom pane
	GetTopAndBottomPaneInfo( wndRect.Rect(), ELayout11x4, 2, *dataInfo );
	// keypad
    keypad = AknLayoutScalable_Avkon::vkb2_area_keypad_pane(2).LayoutLine();
  	keypadRect.LayoutRect(wndRect.Rect(), keypad);
    dataInfo->iKeypad.iKaypadRect = keypadRect.Rect();
    cellPaneBK = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_g1(2, 0, 0).LayoutLine();
  	for( TInt i = 0; i < KKeypadLayout[2][1]; i++)
  		{
  		for( TInt j = 0; j < KKeypadLayout[2][0]; j++)
  			{
  			cellpane = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane(2, j, i).LayoutLine();
  			cellpaneRect.LayoutRect(keypadRect.Rect(), cellpane);
  			keyRect = cellpaneRect.Rect();
  			keyRect.Move( -keypadRect.Rect().iTl.iX, -keypadRect.Rect().iTl.iY );
            cellPaneBKRect.LayoutRect( keyRect, cellPaneBK );
            keyBKRect = cellPaneBKRect.Rect();
            dataInfo->iKeypad.iRects.AppendL(keyBKRect);
  			keyTextLayout.LayoutText(keyRect, keyText);
  			dataInfo->iKeypad.iCharRects.AppendL(keyTextLayout.TextRect());
  			}
  		}
 	keyText = AknLayoutScalable_Avkon::vkb2_cell_keypad_pane_t1(2).LayoutLine();
  	dataInfo->iKeypad.iTextLayout = keyText; 	
  	dataInfo->iKeypad.iFont = AknLayoutUtils::FontFromId(keyText.iFont, NULL);
  	//dataInfo->iKeypad.iTxtRect = keyText.TextRect();	 	
  	
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
TPeninputCandidateListLayoutData CPeninputLafDataFSQ::
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

    TPeninputCandidateListLayoutData layoutData;    
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
TPeninputTooltipBoxLayoutData CPeninputLafDataFSQ::
                                    ReadLafForTooltipBox( const TRect& aRect )
    {    
    TAknWindowLineLayout tooltipBoxLayout 
       = AknLayoutScalable_Avkon::popup_fep_tooltip_window( 0 ).LayoutLine();
    
    TAknLayoutRect tooltipBoxRect;
	tooltipBoxRect.LayoutRect( aRect, tooltipBoxLayout );
	
    TPeninputTooltipBoxLayoutData layoutData;
    layoutData.iRect = tooltipBoxRect.Rect();
        
    layoutData.iTooltipTextLayout 
                    = AknLayoutScalable_Avkon::popup_fep_tooltip_window_t1( 0 )
                    .LayoutLine(); 
    
    layoutData.iFont = AknLayoutUtils::FontFromId
                               ( layoutData.iTooltipTextLayout.iFont, NULL );
    return layoutData;
    }
    
// End Of File
