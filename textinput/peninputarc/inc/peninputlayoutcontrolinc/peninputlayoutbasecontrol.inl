/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline function for base controls
*
*/


// ---------------------------------------------------------------------------
// get control type
// ---------------------------------------------------------------------------
//
inline TControlType CFepUiBaseCtrl::ControlType()
    {
    return iControlType;
    }

// ---------------------------------------------------------------------------
// get control id
// ---------------------------------------------------------------------------
//
inline TInt CFepUiBaseCtrl::ControlId()
    {
    return iControlId;
    } 

// ---------------------------------------------------------------------------
// get control rect
// ---------------------------------------------------------------------------
//
inline const TRect& CFepUiBaseCtrl::Rect()
    {
    return iRect;
    }

// ---------------------------------------------------------------------------
// get control shadow rect
// ---------------------------------------------------------------------------
//
inline const TRect& CFepUiBaseCtrl::ShadowRect()
    {
    return iShadowRect;
    }    
    
// ---------------------------------------------------------------------------
// Control status: Hide/Display
// ---------------------------------------------------------------------------
//   
inline TBool CFepUiBaseCtrl::Hiden()
    {
    return iHiden;
    }

// ---------------------------------------------------------------------------
// Set control backgroud color
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetBkColor(const TRgb& aCol) 
    {
    iBackgroundCol = aCol;
    }

// ---------------------------------------------------------------------------
// Set backgroud mask color
// ---------------------------------------------------------------------------
//    
inline void CFepUiBaseCtrl::SetMaskBkCol(const TRgb& aCol)
    {
    iMaskBkCol = aCol;
    }

// ---------------------------------------------------------------------------
// Set pen size
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetPenSize(const TSize& aSize)
    {
    iPenSize = aSize;
    }

// ---------------------------------------------------------------------------
// Set pen color
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetPenColor(const TRgb& aCol)
    {
    iPenColor = aCol;    
    }

// ---------------------------------------------------------------------------
// Get pen size
// ---------------------------------------------------------------------------
//
inline const TSize& CFepUiBaseCtrl::PenSize()
    {
    return iPenSize;
    }

// ---------------------------------------------------------------------------
// get pen color
// ---------------------------------------------------------------------------
//
inline const TRgb& CFepUiBaseCtrl::PenColor()
    {
    return iPenColor;
    }

// ---------------------------------------------------------------------------
// get backgroud color
// ---------------------------------------------------------------------------
//    
inline const TRgb& CFepUiBaseCtrl::BkColor()
    {
    return iBackgroundCol;
    }
    
// ---------------------------------------------------------------------------
// Get background mask color
// ---------------------------------------------------------------------------
//    
inline const TRgb& CFepUiBaseCtrl::MaskBkCol()
    {
    return iMaskBkCol;
    }
    
// ---------------------------------------------------------------------------
// Control ready staus
// ---------------------------------------------------------------------------
//
inline TBool CFepUiBaseCtrl::Ready()
    {
    return iReady;
    }

// ---------------------------------------------------------------------------
// Set control ready staus
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetReady(TBool aReadyFlag)
    {
    iReady = aReadyFlag;
    }

// ---------------------------------------------------------------------------
// get control valid region
// ---------------------------------------------------------------------------
//
inline const RRegion& CFepUiBaseCtrl::Region()
    {
    return iValidRegion;
    }

// ---------------------------------------------------------------------------
// set control valid region
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetRegion(const RRegion& aRegion)
    {
    iValidRegion.Close(); //frees the memory first
    iValidRegion = aRegion;
    }


// ---------------------------------------------------------------------------
// Test whether this control is a kind of specified control
// ---------------------------------------------------------------------------
//  
inline TBool CFepUiBaseCtrl::IsKindOfControl(TStockBaseCtrlType aCtrlClass)
    {    
    return (iControlType & aCtrlClass) == aCtrlClass ;    
    }
    
// ---------------------------------------------------------------------------
// get control active status
// ---------------------------------------------------------------------------
//    
inline TBool CFepUiBaseCtrl::IsActive()
    {
    return iIsActive;
    }

// ---------------------------------------------------------------------------
// Set control type
// ---------------------------------------------------------------------------
//       
inline void CFepUiBaseCtrl::SetControlType(TControlType aCtrlType)
    {
    iControlType = aCtrlType;
    }

// ---------------------------------------------------------------------------
// Get ui layout
// ---------------------------------------------------------------------------
//   
inline CFepUiLayout* CFepUiBaseCtrl::UiLayout()
    {
    return iUiLayout;
    }

// ---------------------------------------------------------------------------
// get layout root control
// ---------------------------------------------------------------------------
//
inline CFepUiLayoutRootCtrl* CFepUiBaseCtrl::RootControl()
    {
    return iRootCtrl;
    }
    
// ---------------------------------------------------------------------------
// get graphics context for sprite or window
// ---------------------------------------------------------------------------
//   
inline CBitmapContext* CFepUiBaseCtrl::BitGc()
    {
    return iLayoutOwner->BitmapContext();
    }

// ---------------------------------------------------------------------------
// get Bitmap device for sprite or window
// ---------------------------------------------------------------------------
//
inline CFbsBitmapDevice* CFepUiBaseCtrl::BitmapDevice()
    {
    return iLayoutOwner->BitmapDevice();
    }

// ---------------------------------------------------------------------------
// get Mask bitmap device for sprite or window
// ---------------------------------------------------------------------------
//
inline CFbsBitmapDevice* CFepUiBaseCtrl::MaskBitmapDevice()
    {
    return iLayoutOwner->MaskBmpDevice();
    }
 

// ---------------------------------------------------------------------------
// get control pointer down status
// ---------------------------------------------------------------------------
//
inline TBool CFepUiBaseCtrl::PointerDown()
    {
    return iPointerDown;
    }
    
// ---------------------------------------------------------------------------
// change control pointer down status
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetPointerDown(TBool aFlag)
    {
    iPointerDown = aFlag;
    if(!aFlag)
        iPointerLeft = EFalse;
    }
// ---------------------------------------------------------------------------
// get control background maks bmp
// ---------------------------------------------------------------------------
//
inline CFbsBitmap* CFepUiBaseCtrl::BkMaskBmp()
    {
    return iBkMaskBmp;
    }
    
// ---------------------------------------------------------------------------
// get control background bmp
// ---------------------------------------------------------------------------
//
inline CFbsBitmap* CFepUiBaseCtrl::BackgroundBmp()
    {
    return iBackgroundBmp;
    }

// ---------------------------------------------------------------------------
// set control background bmp
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetBackgroundBmp(CFbsBitmap* aBmp)
    {
    iBackgroundBmp = aBmp;
    }

// ---------------------------------------------------------------------------
// set control background mask bmp
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetBackgroundMaskBmp(CFbsBitmap* aBmp)
    {
    iBkMaskBmp = aBmp;
    }

// ---------------------------------------------------------------------------
// Get overlap falg. Tell whether it allows other control moving on top of it
// ---------------------------------------------------------------------------
//
inline TBool CFepUiBaseCtrl::AllowOverlap()
    {
    return iAllowOverlap;
    }
    
// ---------------------------------------------------------------------------
// Set overlap falg.
// ---------------------------------------------------------------------------
//
inline void CFepUiBaseCtrl::SetAllowOverlap(TBool aFlag)
    {
    iAllowOverlap = aFlag;
    }
  
// ---------------------------------------------------------------------------
// Get shadow area rect
// ---------------------------------------------------------------------------
//   
inline TRect CFepUiBaseCtrl::ShadowAreaRect(TShadowBitmapIndex aIndex)
    {
    return iShadowAreaRect[aIndex];
    }
    
// ---------------------------------------------------------------------------
// Get window based control
// ---------------------------------------------------------------------------
//       
inline CCoeControl* CFepUiBaseCtrl::WndBasedControl()
    {
    return iWndControl;
    }

// ---------------------------------------------------------------------------
// set pen size for control border
// ---------------------------------------------------------------------------	
inline void CFepUiBaseCtrl::SetBorderSize(const TSize& aSize)
	{
	iBorderSize = aSize;
	}
    
// ---------------------------------------------------------------------------
// get pen size for control border
// ---------------------------------------------------------------------------	 
inline const TSize& CFepUiBaseCtrl::BorderSize()
	{
	return iBorderSize;
	}

// ---------------------------------------------------------------------------
// set border color
// ---------------------------------------------------------------------------		
inline void CFepUiBaseCtrl::SetBorderColor(const TRgb& aCol)
	{
	iBorderColor = aCol;
	}

// ---------------------------------------------------------------------------
// Get border color
// ---------------------------------------------------------------------------	
inline const TRgb& CFepUiBaseCtrl::BorderColor()
	{
	return iBorderColor;
	}

// ---------------------------------------------------------------------------
// Get resource Id
// ---------------------------------------------------------------------------
inline TInt32 CFepUiBaseCtrl::ResourceId()
    {
    return iResourceId;
    }

inline CFepUiBaseCtrl* CFepUiBaseCtrl::ParentCtrl()    
    {
    return iParentCtrl;
    }
inline void CFepUiBaseCtrl::EnableRegionUpdating()
    {
    iUpdateFlagEnabled = ETrue;    
    }
    
inline void CFepUiBaseCtrl::DisableRegionUpdating()
    {
    iUpdateFlagEnabled = EFalse;    
    }
   
inline void CFepUiBaseCtrl::SetHidenFlag(TBool aFlag)
    {
    iHiden = aFlag;
    }
    
inline CCoeControl* CFepUiBaseCtrl::WndControl()
    {
    return iWndControl;
    }
    
    
inline TInt CFepUiBaseCtrl::OrderPos() const
    {
    return iOrdinalPos;
    }

// ---------------------------------------------------------------------------
// Advanced Tactile feedback REQ417-47932
// get tactile feedback type
// ---------------------------------------------------------------------------
//
inline TInt CFepUiBaseCtrl::TactileFeedbackType() const
    {
    return reinterpret_cast<CFepUiBaseCtrlExtension*>(iReservered1)->TactileFeedbackType();	
    }
//end of file
