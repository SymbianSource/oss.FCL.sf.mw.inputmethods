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
* Description:  peninput hwr laf data manager
*
*/


// System includes
#include <peninputdataconverter.h>
#include <peninputcommonlayoutglobalenum.h>
#include <AknUtils.h> 
#include <aknlayoutscalable_avkon.cdl.h>

// User includes
#include "peninputgenerichwrlafdatamgr.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputHwrLayoutDataInfo::NewLC
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputHwrLayoutDataInfo* CPeninputHwrLayoutDataInfo::NewLC()
	{
    CPeninputHwrLayoutDataInfo* self = 
        new ( ELeave ) CPeninputHwrLayoutDataInfo();
    CleanupStack::PushL( self );
    return self;
	}

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericHwrLafMgr* CPeninputGenericHwrLafMgr::NewL(TPeninputHwrLayoutType aLayoutType)
	{
	CPeninputGenericHwrLafMgr* self = NewLC(aLayoutType);
    CleanupStack::Pop(self);
    
    return self;
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::NewLC
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericHwrLafMgr* CPeninputGenericHwrLafMgr::NewLC(TPeninputHwrLayoutType aLayoutType)
	{
    CPeninputGenericHwrLafMgr* self = 
        new ( ELeave ) CPeninputGenericHwrLafMgr(aLayoutType);
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
	
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericHwrLafMgr::ConstructL()
	{
	ReadLafInfoL();
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::~CPeninputGenericHwrLafMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericHwrLafMgr::~CPeninputGenericHwrLafMgr()
	{
	iLayoutData.ResetAndDestroy();
	iLayoutData.Close();
	}
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::GetButtonRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericHwrLafMgr::GetButtonRect( TPeninputCommonCtrlID aButtonID, 
										  	   TRect& aOutterRect,
										       TRect& aInnerRect )
	{
	ASSERT( IsValid() );

	switch( aButtonID )
		{
		case EPeninutWindowCtrlIdMoveBtn:
			{
			aOutterRect = iLayoutData[iCurLayoutType]->iMoveButton.iRect;
			aInnerRect  = iLayoutData[iCurLayoutType]->iMoveButton.iInnerRect;
			}
			break;
		case EPeninutWindowCtrlIdCloseBtn:
			{
			aOutterRect = iLayoutData[iCurLayoutType]->iCloseButton.iRect;
			aInnerRect  = iLayoutData[iCurLayoutType]->iCloseButton.iInnerRect;
			}
			break;
		case EPeninutWindowCtrlIdOptionBtn:
			{
			aOutterRect = iLayoutData[iCurLayoutType]->iOptionButton.iRect;
			aInnerRect  = iLayoutData[iCurLayoutType]->iOptionButton.iInnerRect;
			}
			break;
		case EPeninutWindowCtrlIdSwitchToVkbBtn:
			{
			aOutterRect = iLayoutData[iCurLayoutType]->iSwitchButton.iRect;
			aInnerRect  = iLayoutData[iCurLayoutType]->iSwitchButton.iInnerRect;							
			}
			break;
		case EPeninutWindowCtrlIdSymbolBtn:
			{
			aOutterRect = iLayoutData[iCurLayoutType]->iRangbar.iSymbolButton.iRect;
			aInnerRect  = iLayoutData[iCurLayoutType]->iRangbar.iSymbolButton.iInnerRect;							
			}
			break;
		case EPeninutWindowCtrlIdNativeCharBtn:
			{
			aOutterRect = iLayoutData[iCurLayoutType]->iRangbar.iAlphaButton.iRect;
			aInnerRect  = iLayoutData[iCurLayoutType]->iRangbar.iAlphaButton.iInnerRect;							
			}
			break;
		case EPeninutWindowCtrlIdInputLangSwitcherBtn:
			{
			aOutterRect = iLayoutData[iCurLayoutType]->iLanguageButton.iRect;
			aInnerRect  = iLayoutData[iCurLayoutType]->iLanguageButton.iInnerRect;							
			}
			break;	
		case EPeninutWindowCtrlIdLatinNumberBtn:
			{
			aOutterRect = iLayoutData[iCurLayoutType]->iRangbar.iNumberButton.iRect;
			aInnerRect  = iLayoutData[iCurLayoutType]->iRangbar.iNumberButton.iInnerRect;							
			}
			break;
		default:
			ASSERT( EFalse );
			break;
		}
	}

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::EntirePaneRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline const TRect& CPeninputGenericHwrLafMgr::EntirePaneRect() const
	{
	ASSERT( IsValid() );	
	return 	iLayoutData[iCurLayoutType]->iHwrWndRect;
	}	
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::RangbarRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline const TRect& CPeninputGenericHwrLafMgr::RangbarRect() const
	{
	ASSERT( IsValid() );
	return 	iLayoutData[iCurLayoutType]->iRangbar.iRect;
	}	
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::TextAreaRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline const TRect& CPeninputGenericHwrLafMgr::TextAreaRect() const
	{
	ASSERT( IsValid() );
	return 	iLayoutData[iCurLayoutType]->iICF.iRect;
	}	
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::WriteAreaRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline const TRect& CPeninputGenericHwrLafMgr::WriteAreaRect() const
	{
	ASSERT( IsValid() );
	return 	iLayoutData[iCurLayoutType]->iWriteAreaRect;
	}	
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::CPeninputGenericHwrLafMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericHwrLafMgr::CPeninputGenericHwrLafMgr(TPeninputHwrLayoutType aLayoutType) 
	: iCurLayoutType(aLayoutType),iIsValid(EFalse) 
	{
		
	}	
	
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::SetLayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline void CPeninputGenericHwrLafMgr::SetLayoutType(TPeninputHwrLayoutType aLayoutType)
	{
	iCurLayoutType = aLayoutType;
	}

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::LayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline TPeninputHwrLayoutType CPeninputGenericHwrLafMgr::LayoutType() const
	{
	return iCurLayoutType;
	}

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::IsValid
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
inline TBool CPeninputGenericHwrLafMgr::IsValid() const
	{
	return iIsValid && iCurLayoutType != ELayoutNone;
	}

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::ReadLafInfoL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericHwrLafMgr::ReadLafInfoL()
	{
    TAknWindowLineLayout appWnd, mainPane, wndLayout;
	TAknLayoutRect wndLayoutRect;
	
    // Get hwr window layout
    appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();

    if (appWnd.iW > appWnd.iH) // landscape
        {	    
	    mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
        wndLayout = AknLayoutScalable_Avkon::popup_fep_hwr_window(2).LayoutLine();
        }
    else  // portrait	
        {        
        mainPane = AknLayoutScalable_Avkon::main_pane(1).LayoutLine();
        wndLayout = AknLayoutScalable_Avkon::popup_fep_hwr_window(0).LayoutLine();
        }
	wndLayoutRect.LayoutRect(appWnd.Rect(), wndLayout);
	
	// Get latin layout
	CPeninputHwrLayoutDataInfo* pDataInfo = CPeninputHwrLayoutDataInfo::NewLC();
	GetHwrLayoutDataInfo( wndLayoutRect.Rect(), 0, *pDataInfo );
	iLayoutData.AppendL( pDataInfo );
	CleanupStack::Pop( pDataInfo );
	
	// Get non latin layout
	pDataInfo = CPeninputHwrLayoutDataInfo::NewLC();
	GetHwrLayoutDataInfo( wndLayoutRect.Rect(), 2, *pDataInfo );
	iLayoutData.AppendL( pDataInfo );
	CleanupStack::Pop( pDataInfo );	
	
	iIsValid = ETrue;
	}

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLafMgr::GetHwrLayoutDataInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericHwrLafMgr::GetHwrLayoutDataInfo( const TRect& aParentWndRect, 
							   						  TInt aLayoutType, 
							                          CPeninputHwrLayoutDataInfo& aDataInfo )
	{
	TAknWindowLineLayout topPane, sidePane, topTextPane;
	TAknWindowLineLayout cellBtn, cellBtnInner, writePane;
	
	TAknLayoutRect topPaneRect, sidePaneRect;
	TAknLayoutRect topTextPaneRect, writePaneRect;
	TAknLayoutRect cellBtnRect, cellBtnInnerRect;
	TAknTextLineLayout cellText;

	aDataInfo.iHwrWndRect = aParentWndRect;
	
	// Get top pane laf info
	topPane = AknLayoutScalable_Avkon::fep_hwr_top_pane(aLayoutType).LayoutLine();
	topPaneRect.LayoutRect(aParentWndRect, topPane);
		// move button
	cellBtn = AknLayoutScalable_Avkon::fep_hwr_top_pane_g1(aLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(topPaneRect.Rect(), cellBtn);
	aDataInfo.iMoveButton.iRect = cellBtnRect.Rect();
		// close button
	cellBtn = AknLayoutScalable_Avkon::fep_hwr_top_pane_g2(aLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(topPaneRect.Rect(), cellBtn);
	aDataInfo.iCloseButton.iRect = cellBtnRect.Rect();	
		// option button
	cellBtn = AknLayoutScalable_Avkon::fep_hwr_top_pane_g3(aLayoutType).LayoutLine();
	cellBtnRect.LayoutRect(topPaneRect.Rect(), cellBtn);
	aDataInfo.iOptionButton.iRect = cellBtnRect.Rect();
		// ICF and it's text font
	topTextPane = AknLayoutScalable_Avkon::fep_hwr_top_text_pane(aLayoutType).LayoutLine();
	topTextPaneRect.LayoutRect(topPaneRect.Rect(), topTextPane);
	aDataInfo.iICF.iRect = topTextPaneRect.Rect();	
	cellText = AknLayoutScalable_Avkon::fep_hwr_top_text_pane_t1().LayoutLine();
	aDataInfo.iICF.iTextFont = AknLayoutUtils::FontFromId(cellText.iFont, NULL);

	// Get side pane laf info
	sidePane = AknLayoutScalable_Avkon::fep_hwr_side_pane(aLayoutType).LayoutLine();
	sidePaneRect.LayoutRect(aParentWndRect, sidePane);
		// alphabet button
	cellBtn = AknLayoutScalable_Avkon::cell_hwr_side_pane(aLayoutType, 0, 0).LayoutLine();
	cellBtnRect.LayoutRect(sidePaneRect.Rect(), cellBtn);
	aDataInfo.iRangbar.iAlphaButton.iRect = cellBtnRect.Rect();
		// number button
	cellBtn = AknLayoutScalable_Avkon::cell_hwr_side_pane(aLayoutType, 1, 0).LayoutLine();
	cellBtnRect.LayoutRect(sidePaneRect.Rect(), cellBtn);
	aDataInfo.iRangbar.iNumberButton.iRect = cellBtnRect.Rect();	
		// symbol button
	cellBtn = AknLayoutScalable_Avkon::cell_hwr_side_pane(aLayoutType, 2, 0).LayoutLine();
	cellBtnRect.LayoutRect(sidePaneRect.Rect(), cellBtn);
	aDataInfo.iRangbar.iSymbolButton.iRect = cellBtnRect.Rect();		
		// set range bar rect
	aDataInfo.iRangbar.iRect = TRect( aDataInfo.iRangbar.iAlphaButton.iRect.iTl,
										aDataInfo.iRangbar.iSymbolButton.iRect.iBr );	
		// language button
	cellBtn = AknLayoutScalable_Avkon::cell_hwr_side_pane(aLayoutType, 3, 0).LayoutLine();
	cellBtnRect.LayoutRect(sidePaneRect.Rect(), cellBtn);
	aDataInfo.iLanguageButton.iRect = cellBtnRect.Rect();
		// switch button
	cellBtn = AknLayoutScalable_Avkon::cell_hwr_side_pane(aLayoutType, 4, 0).LayoutLine();
	cellBtnRect.LayoutRect(sidePaneRect.Rect(), cellBtn);
	aDataInfo.iSwitchButton.iRect = cellBtnRect.Rect();
	
	// Get write pane laf info
	writePane = AknLayoutScalable_Avkon::fep_hwr_write_pane(aLayoutType).LayoutLine();
	writePaneRect.LayoutRect(aParentWndRect, writePane);
	aDataInfo.iWriteAreaRect = writePaneRect.Rect();	
	}
    
