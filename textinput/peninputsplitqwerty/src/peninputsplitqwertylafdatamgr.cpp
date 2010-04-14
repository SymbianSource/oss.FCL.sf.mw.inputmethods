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
#include "peninputsplitqwertylafdatamgr.h"

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputQwtLayoutDataInfo::~CPeninputQwtLayoutDataInfo()
    {
    iKeypad.iRects.Close();
    iKeypad.iCharRects.Close();
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
MPeninputLafDataCommon::~MPeninputLafDataCommon()
    {
    }

// ---------------------------------------------------------------------------
// Release laf data
// ---------------------------------------------------------------------------
//
void MPeninputLafDataCommon::DestoryDataCommon()
    {
    iLayoutDataInfo.ResetAndDestroy();
    iLayoutDataInfo.Close();
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void MPeninputLafDataCommon::ConstructL()
    {
    ReadLafInfoL();
    }

// ---------------------------------------------------------------------------
// Retrieve laf data of button specified by aButtonID
// ---------------------------------------------------------------------------
//
const TPeninputButtonLayoutData& MPeninputLafDataCommon::ButtonLayoutData( 
                                                        TInt aButtonID ) const
    {
    ASSERT( IsValid() );
    const TPeninputButtonLayoutData* pData = NULL;

    switch ( aButtonID )
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
            
        case EPeninutWindowCtrlIdRangeBtn:
            {
            pData = &(iLayoutDataInfo[iLayoutType]->iRangeButton);
            }
            break;
        default:
            ASSERT( EFalse );
            break;
        }
    ASSERT( pData );
    return *pData;
    }
    
// ---------------------------------------------------------------------------
// Retrieve laf data of keypad
// ---------------------------------------------------------------------------
//        
const TPeninputKeyLayoutData& MPeninputLafDataCommon::KeypadLayoutData() const
    {
    ASSERT( IsValid() );
    return iLayoutDataInfo[iLayoutType]->iKeypad;
    }

// ---------------------------------------------------------------------------
// Retrieve laf data of qwerty layout
// ---------------------------------------------------------------------------
//        
const CPeninputQwtLayoutDataInfo& MPeninputLafDataCommon::QwtLayoutData() const
    {
    ASSERT( IsValid() );
    return *iLayoutDataInfo[iLayoutType];
    }

// ---------------------------------------------------------------------------
// Set layout type
// ---------------------------------------------------------------------------
//    
void MPeninputLafDataCommon::SetLayoutType( TInt aLayoutType )
    {
    iLayoutType = aLayoutType;
    }

// ---------------------------------------------------------------------------
// Get current layout type
// ---------------------------------------------------------------------------
//
TInt MPeninputLafDataCommon::LayoutType() const
    {
    return iLayoutType;
    }

// ---------------------------------------------------------------------------
// Set laf option)
// ---------------------------------------------------------------------------
//
void MPeninputLafDataCommon::SetLafOption( TPeninputVkbLafOption aLafOption )
    {
    iLafOption = aLafOption;
    }

// ---------------------------------------------------------------------------
// Get current laf option
// ---------------------------------------------------------------------------
//
TPeninputVkbLafOption MPeninputLafDataCommon::LafOption() const
    {
    return iLafOption;
    }

// ---------------------------------------------------------------------------
// Check whether laf data is valid or not
// ---------------------------------------------------------------------------
//        
TBool MPeninputLafDataCommon::IsValid() const
    {
    return ( iLayoutDataInfo.Count() && iLayoutType != ELayoutNone ) ? ETrue : EFalse;
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//        
CPeninputLafDataFSQ* CPeninputLafDataFSQ::NewL( TInt aLayoutType )
    {
    CPeninputLafDataFSQ* self = NewLC( aLayoutType );
    CleanupStack::Pop();
    return self;        
    }
// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//        
CPeninputLafDataFSQ* CPeninputLafDataFSQ::NewLC( TInt aLayoutType )
    {
    CPeninputLafDataFSQ* self = new( ELeave ) CPeninputLafDataFSQ( aLayoutType );
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputLafDataFSQ::~CPeninputLafDataFSQ()
    {
    DestoryDataCommon();    
    }    
    
// ---------------------------------------------------------------------------
// Read all laf data
// ---------------------------------------------------------------------------
//
void CPeninputLafDataFSQ::ReadLafInfoL()
    {
    // clean up all the data
    iLayoutDataInfo.ResetAndDestroy();

    CPeninputQwtLayoutDataInfo* info = NULL;

    //ELayout10x3
    info = ReadLC( ELayout10x3, 3, 10 );
    iLayoutDataInfo.AppendL( info ); 
    CleanupStack::Pop( info );

    //ELayout11x3
    info = ReadLC( ELayout11x3, 3, 11 );
    iLayoutDataInfo.AppendL( info );
    CleanupStack::Pop( info );

    //ELayout11x4
    info = ReadLC( ELayout11x4, 4, 11 );
    iLayoutDataInfo.AppendL( info );
    CleanupStack::Pop( info );
    }    
    
// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//        
CPeninputLafDataFSQ::CPeninputLafDataFSQ( TInt aLayoutType )
    {
    SetLayoutType( aLayoutType );    
    }

// ---------------------------------------------------------------------------
// Read laf data
// ---------------------------------------------------------------------------
//
CPeninputQwtLayoutDataInfo* CPeninputLafDataFSQ::ReadLC( const TInt aLayoutType, 
                                                         const TInt aRows, 
                                                         const TInt aCols )
    {
    //variety v1, value 0/1 = 10cols/11cols, applay to:
    //    grid_vkbss_keypad_pane     
    //    aid_vkbss_key_offset    
    TInt v1 = ( aCols == 10 ) ? 0 : 1; 
    
    //variety v2, value 0/1/2/3 = 10x3/10x4/11x3/11x4, applay to:
    //    cell_vkbss_key_pane        
    //    bg_cell_vkbss_key_g1       
    //    cell_vkbss_key_t1          
    //    cell_vkbss_key_g1          
    //    cell_vkbss_key_3p_pane   
    TInt v2 = 0; 
    switch ( aLayoutType )
        {
        case ELayout10x3: v2 = 0; break;
        case ELayout11x3: v2 = 2; break;
        case ELayout11x4: v2 = 3; break;
        default: v2 = 0;
        }

    CPeninputQwtLayoutDataInfo* data = new( ELeave ) CPeninputQwtLayoutDataInfo;
    CleanupStack::PushL( data );

    TAknWindowLineLayout linelayout;
    TAknLayoutRect layoutrect;

    TAknLayoutRect keypadRect, keypaneRect, keycellRect, keylabelRect;  
    TAknLayoutRect pic3paneRect;

    TAknTextLineLayout keyText;

    TRect rectAppWin, rectMainWin;
    TPoint base;
    TInt pic3pLeftWidth, pic3pRightWidth;

    linelayout = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
    rectAppWin = linelayout.Rect();

    linelayout = AknLayoutScalable_Avkon::popup_fep_vkbss_window(0).LayoutLine();
    layoutrect.LayoutRect( rectAppWin, linelayout );
    rectMainWin = layoutrect.Rect();

    linelayout = AknLayoutScalable_Avkon::grid_vkbss_keypad_pane(v1).LayoutLine();
    keypadRect.LayoutRect( rectMainWin, linelayout );

    linelayout = AknLayoutScalable_Avkon::cell_vkbss_key_pane(v2).LayoutLine();
    keypaneRect.LayoutRect( keypadRect.Rect(), linelayout );

    linelayout = AknLayoutScalable_Avkon::bg_cell_vkbss_key_g1(v2).LayoutLine();
    keycellRect.LayoutRect( keypaneRect.Rect(), linelayout );

    keyText = AknLayoutScalable_Avkon::cell_vkbss_key_t1(v2).LayoutLine();
    keyText.iB += 3; 
    
    linelayout = AknLayoutScalable_Avkon::cell_vkbss_key_g1(v2).LayoutLine();
    keylabelRect.LayoutRect( keypaneRect.Rect(), linelayout );

    linelayout = AknLayoutScalable_Avkon::cell_vkbss_key_3p_pane(v2).LayoutLine();
    pic3paneRect.LayoutRect( keypaneRect.Rect(), linelayout );

    linelayout = AknLayoutScalable_Avkon::cell_vkbss_key_3p_pane_g1(0).LayoutLine();
    layoutrect.LayoutRect( pic3paneRect.Rect(), linelayout );
    pic3pLeftWidth = layoutrect.Rect().Width();

    linelayout = AknLayoutScalable_Avkon::cell_vkbss_key_3p_pane_g3(0).LayoutLine();
    layoutrect.LayoutRect( pic3paneRect.Rect(), linelayout );
    pic3pRightWidth = layoutrect.Rect().Width();

    linelayout = AknLayoutScalable_Avkon::aid_vkbss_key_offset(v1).LayoutLine();    
    layoutrect.LayoutRect( keypadRect.Rect(), linelayout );
    TInt rowIndentWidth = layoutrect.Rect().Width();

    base = rectMainWin.iTl;

    data->iClientRect =  RelativeRect( rectMainWin, base );
    data->iQwtRect =  rectMainWin; //screen coord-sys ? 

    TRect rectOfKeypad = RelativeRect( keypadRect.Rect(), base );
    TRect rectOfButtons = rectOfKeypad;
    rectOfKeypad.iBr.iY -= keypaneRect.Rect().Height(); 
    rectOfButtons.iTl.iY += rectOfKeypad.Height();

    //keypad
    data->iKeypad.iKaypadRect = rectOfKeypad;
    data->iKeypad.iTextLayout = keyText;
    data->iKeypad.iFont = AknLayoutUtils::FontFromId( keyText.iFont, NULL );

    //all template rects are relative to TL of keypad
    TRect rectXPane = RelativeRect( keypaneRect.Rect(), base ); 
    TRect rectXBorder = RelativeRect( keycellRect.Rect(), base );
    TRect rectXInner = RelativeRect( keylabelRect.Rect(), base );

    rectXPane.Move( - rectOfKeypad.iTl ); //translate to keypad coord-sys
    rectXBorder.Move( - rectOfKeypad.iTl );
    rectXInner.Move( - rectOfKeypad.iTl );

    for ( TInt i = 0; i < aRows; i++ )
        {
        TInt indent = ( i % 2 == 1 ) ? rowIndentWidth : 0;
        for ( TInt j = 0; j < aCols; j++ )
            {
			TRect bound = rectXPane;
            bound.Move( j * rectXPane.Width(), i * rectXPane.Height() );
            bound.Move( indent, 0 );
            
            data->iKeypad.iRects.AppendL( bound );
            
            TRect inner = rectXPane;
            inner.Move( j * rectXPane.Width(), i * rectXPane.Height() );
            inner.Move( indent, 0 );        
            
            TAknLayoutText keyTextLayout; 
            keyTextLayout.LayoutText( inner, keyText );
            data->iKeypad.iCharRects.AppendL( keyTextLayout.TextRect() );
            }
        }

    //buttons

    //all template rects are relative to TL of buttons row
    rectXPane = keypaneRect.Rect();
    rectXBorder = RelativeRect( keycellRect.Rect(), rectXPane.iTl );
    rectXInner = RelativeRect( keylabelRect.Rect(), rectXPane.iTl );
    TRect rect3PicPane = RelativeRect( pic3paneRect.Rect(), rectXPane.iTl );
    rectXPane.Move( - rectXPane.iTl ); 

    TInt spaceBtnWidth = rectOfButtons.Width() - rectXPane.Width() * 8;
    TInt dx = rectOfButtons.iTl.iX;
    TInt dy = rectOfButtons.iTl.iY;
    
    data->iCloseButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iCloseButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    dx += rectXPane.Width();

    data->iShiftButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iShiftButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    dx += rectXPane.Width();

    data->iRangeButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iRangeButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    dx += rectXPane.Width();

    data->iLeftButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iLeftButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    dx += rectXPane.Width();

    TInt expand = spaceBtnWidth - rectXPane.Width();
    data->iSpaceButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iSpaceButton.iRect.Resize( expand, 0 );
    data->iSpaceButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    data->iSpaceButton.iInnerRect.Resize( expand, 0 );
    data->iSpaceButtonEx.iIconsFrameRect = OffsetRect( rect3PicPane, dx, dy );
    data->iSpaceButtonEx.iIconsFrameRect.Resize( expand, 0 );
    data->iSpaceButtonEx.iMiddleIconRect = data->iSpaceButtonEx.iIconsFrameRect;
    data->iSpaceButtonEx.iMiddleIconRect.iTl.iX += pic3pLeftWidth;
    data->iSpaceButtonEx.iMiddleIconRect.iBr.iX -= pic3pRightWidth;    
    dx += spaceBtnWidth;

    data->iRightButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iRightButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    dx += rectXPane.Width();

    data->iOptionButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iOptionButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    dx += rectXPane.Width();

    data->iEnterButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iEnterButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    dx += rectXPane.Width();

    data->iClearButton.iRect = OffsetRect( rectXBorder, dx, dy );
    data->iClearButton.iInnerRect = OffsetRect( rectXInner, dx, dy );
    dx += rectXPane.Width();   

    keyText = AknLayoutScalable_Avkon::popup_fep_char_pre_window_t1(0).LayoutLine();
    
    linelayout = AknLayoutScalable_Avkon::popup_fep_char_pre_window(0).LayoutLine();
    layoutrect.LayoutRect( pic3paneRect.Rect(), linelayout );
    TRect preBubbleRect = layoutrect.Rect();
    preBubbleRect.Move( -preBubbleRect.iTl );
    TRect preBubbleInner = preBubbleRect;
    preBubbleInner.Shrink( 10, 10 );
    
    data->iPreviewWndText = keyText;
    data->iPreviewWndRect = preBubbleRect;
    data->iPreviewWndInnerRect = preBubbleInner;
    
    return data;
    }

// ---------------------------------------------------------------------------
// calculate relative rect base on given point
// ---------------------------------------------------------------------------
//
TRect CPeninputLafDataFSQ::RelativeRect( const TRect& aRect, const TPoint& aPoint )
    {
    TRect rect = aRect;
    rect.Move( - aPoint.iX, - aPoint.iY );
    return rect;
    }

// ---------------------------------------------------------------------------
// Offset rect
// ---------------------------------------------------------------------------
//
TRect CPeninputLafDataFSQ::OffsetRect( const TRect& aRect, TInt aDx, TInt aDy )
    {
    TRect rect = aRect;
    rect.Move( aDx, aDy );
    return rect;    
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLafMgr* CPeninputSplitQwertyLafMgr::NewL()
    {
    CPeninputSplitQwertyLafMgr* self = NewLC();
    CleanupStack::Pop(self);
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLafMgr* CPeninputSplitQwertyLafMgr::NewLC()
    {
    CPeninputSplitQwertyLafMgr* self = new ( ELeave ) CPeninputSplitQwertyLafMgr();
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLafMgr::~CPeninputSplitQwertyLafMgr()
    {
    delete iLafData;
    iLafData = NULL;
    }

// ---------------------------------------------------------------------------
// Set input mode and reload laf 
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::SetInputModeL( TPluginInputMode aInputMode )
    {
    if ( iInputMode == aInputMode )
        {
        return;
        }

    if ( iLafData )
        {
        delete iLafData;
        iLafData = NULL;
        }

    if ( aInputMode == EPluginInputModeFSQ )
        {
        iLafData = CPeninputLafDataFSQ::NewL( ELayoutNone );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    }    

// ---------------------------------------------------------------------------
// Get current input mode
// ---------------------------------------------------------------------------
//
TPluginInputMode CPeninputSplitQwertyLafMgr::InputMode() const
    {
    return iInputMode;
    }

// ---------------------------------------------------------------------------
// Set layout type
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::SetLayoutType( TPeninputQwtLayoutType aLayoutType )
    {
    ASSERT( iLafData );
    if ( iLafData )
        {
        iLafData->SetLayoutType( aLayoutType );
        }
    }
    
// ---------------------------------------------------------------------------
// Get layout type by row and column count
// ---------------------------------------------------------------------------
//
TPeninputQwtLayoutType CPeninputSplitQwertyLafMgr::LayoutTypeFromRowsColumns(
                                                         const TInt aRows, 
                                                         const TInt aColumns )
    {
    TPeninputQwtLayoutType ret = ELayoutNone;
    
    if ( aRows == 3 && aColumns == 10 )
        {
        ret = ELayout10x3;
        }
    else if ( aRows == 3 && aColumns == 11 )
        {
        ret = ELayout11x3;
        }
    else if ( aRows == 4 && aColumns == 11 )    
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
// Set option ( and reload laf )
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::SetLafOption( TPeninputVkbLafOption aOption )
    {
    if ( iLafData->LafOption() == aOption )
        {
        return;
        }
    
    iLafData->SetLafOption( aOption );    
    TRAP_IGNORE( iLafData->ConstructL() );
    }

// ---------------------------------------------------------------------------
// Get current option
// ---------------------------------------------------------------------------
//
TPeninputVkbLafOption CPeninputSplitQwertyLafMgr::OptionType() const
    {
    return iLafData->LafOption();
    }

// ---------------------------------------------------------------------------
// Check whether laf data is valid or not
// ---------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyLafMgr::IsValid()
    {
    if ( !iLafData )
        {
        return EFalse;
        }
    
    return iLafData->IsValid();
    }

// ---------------------------------------------------------------------------
// Reload laf data
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::UpdateLafDataL()
    {
    iLafData->ConstructL(); 
    }

// ---------------------------------------------------------------------------
// Get rect of entire ui, screen coord-sys
// ---------------------------------------------------------------------------
//      
const TRect& CPeninputSplitQwertyLafMgr::EntirePaneRect() const
    {
    ASSERT( iLafData );
    return iLafData->QwtLayoutData().iQwtRect;
    }

// ---------------------------------------------------------------------------
// Get rect of entire ui, client coord-sys
// ---------------------------------------------------------------------------
//
const TRect& CPeninputSplitQwertyLafMgr::ClientRect() const
    {
    ASSERT( iLafData );
    return iLafData->QwtLayoutData().iClientRect;
    }

// ---------------------------------------------------------------------------
// Get rect of button ctrl
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::GetButtonRect( TPeninputCommonCtrlID aButtonID, 
                                                TRect& aOutterRect,
                                                TRect& aInnerRect )
    {
    ASSERT( iLafData );        
    const TPeninputButtonLayoutData& data = iLafData->ButtonLayoutData( aButtonID );
    aOutterRect = data.iRect;
    aInnerRect  = data.iInnerRect;
    }


// ---------------------------------------------------------------------------
// Get rect of virtual keypad ctrl
// ---------------------------------------------------------------------------
//
const TRect& CPeninputSplitQwertyLafMgr::KeypadRect() const
    {
    ASSERT( iLafData );
    const TPeninputKeyLayoutData& data = iLafData->KeypadLayoutData();
    return data.iKaypadRect;
    }

// ---------------------------------------------------------------------------
// Get rect of virtual key ctrl
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::GetKeyRect( TInt aCol, TInt aRow, 
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
    index = aRow * colNum + aCol;
    aKeyRect = iLafData->KeypadLayoutData().iRects[index];
    aCharRect = iLafData->KeypadLayoutData().iCharRects[index];
    }

// ---------------------------------------------------------------------------
// Get text font of virtual keypad ctrl
// ---------------------------------------------------------------------------
//
const CFont* CPeninputSplitQwertyLafMgr::KeyTextFont() const
    {
    ASSERT( iLafData );
    return iLafData->KeypadLayoutData().iFont;    
    }

// ---------------------------------------------------------------------------
// Get text layout of virtual keypad ctrl
// ---------------------------------------------------------------------------
//    
TAknTextLineLayout CPeninputSplitQwertyLafMgr::KeyTextLayout() const
    {
    ASSERT( iLafData );
    return iLafData->KeypadLayoutData().iTextLayout;
    }

// ---------------------------------------------------------------------------
// Get rect of preview bubble ctrl
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::GetPreviewWndRect( TRect& aOutRect, 
                                                    TRect& aInnerRect ) const
    {
    aOutRect = iLafData->QwtLayoutData().iPreviewWndRect;
    aInnerRect = iLafData->QwtLayoutData().iPreviewWndInnerRect;
    }

// ---------------------------------------------------------------------------
// Get text layout of preview bubble
// ---------------------------------------------------------------------------
//
TAknTextLineLayout CPeninputSplitQwertyLafMgr::PreviewWndText() const
    {
    return iLafData->QwtLayoutData().iPreviewWndText;
    }

// ---------------------------------------------------------------------------
// Get 3-pieces info of space button
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::GetSpaceButtonExLayoutInfo( TRect& aIconsFrameRect, 
                                                             TRect& aMiddleIconRect ) const
    {
    ASSERT( iLafData );
    aIconsFrameRect = iLafData->QwtLayoutData().iSpaceButtonEx.iIconsFrameRect;
    aMiddleIconRect = iLafData->QwtLayoutData().iSpaceButtonEx.iMiddleIconRect;        
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLafMgr::CPeninputSplitQwertyLafMgr() 
    : iInputMode(EPluginInputModeNone)
    {
    }

// ---------------------------------------------------------------------------
// CPeninputSplitQwertyLafMgr::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLafMgr::ConstructL()
    {
    }
// End Of File
