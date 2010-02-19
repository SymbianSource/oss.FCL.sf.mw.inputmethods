/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of char range selector.
*
*/


// INCLUDE
#include <peninputfingerhwrarwnd.rsg>

#include <coemain.h>
#include <s32mem.h>
#include <barsread.h>

#include <aknlayoutscalable_apps.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <AknIconUtils.h>
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>

#include <peninputlayout.h>
#include <peninputrepeatbutton.h>
#include <peninputmultiimagebutton.h>
#include <peninputpluginutils.h>

#include "hbufcarrayar.h"
#include "peninputfingerhwrarevent.h"
#include "peninputfingerhwrarcontrolid.h"
#include "peninputfingerhwrarstoreconstants.h"
#include "peninputfingerhwrarmultipagevkb.h"
#include "peninputfingerhwrarsymboltable.h"


//CONST DEFINATION
const TInt KSymbolButtonInnerPadding = 6;

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrSymbolTable* CPeninputArabicFingerHwrSymbolTable::NewL( 
    CFepUiLayout* aUiLayout, TInt aId )
    {
    CPeninputArabicFingerHwrSymbolTable* self = CPeninputArabicFingerHwrSymbolTable::NewLC( 
            aUiLayout, aId );
    
    CleanupStack::Pop( self ); // self;
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrSymbolTable* CPeninputArabicFingerHwrSymbolTable::NewLC( 
    CFepUiLayout* aUiLayout, TInt aId )
    {
    CPeninputArabicFingerHwrSymbolTable* self = new (ELeave) CPeninputArabicFingerHwrSymbolTable( 
            aUiLayout, aId );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrSymbolTable::~CPeninputArabicFingerHwrSymbolTable()
    {
    }

// ---------------------------------------------------------------------------
// popup the list.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::OpenSymbolTable()
    {
    CapturePointer( ETrue );
    iPopupVisible = ETrue;
    iPointerMoveBack = EFalse;
	Draw();
    UpdateArea(iRect );    
	UiLayout()->LockArea(UiLayout()->Rect(),this);
    }

// ---------------------------------------------------------------------------
// cancel the popup.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::CloseSymbolTable()
    {
    CapturePointer( EFalse );
    iPenDownCtrl = NULL;
    iPopupVisible = EFalse;
    UiLayout()->UnLockArea(UiLayout()->Rect(),this);
    }

// ---------------------------------------------------------------------------
// get visibility of popup.
// ---------------------------------------------------------------------------
//
TBool CPeninputArabicFingerHwrSymbolTable::IsPopup()
    {
    return iPopupVisible;
    }

// ---------------------------------------------------------------------------
// Handle pointer down event.
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputArabicFingerHwrSymbolTable::HandlePointerDownEventL(
    const TPoint& aPoint )
    {
    CancelPointerDownL();
    CFepUiBaseCtrl* ctrl = CControlGroup::HandlePointerDownEventL( aPoint );
    iPenDownCtrl = ctrl;
    
    if(!ctrl)
	   {
	   ReportEvent(EHwrEventOutsideEvent);
	   }
	else
       {
	   ctrl->Draw();
	   ctrl->UpdateArea(ctrl->Rect(),EFalse);
	   }	
	   
    return ctrl;
    }

// ---------------------------------------------------------------------------
// Handle pointer up event.
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputArabicFingerHwrSymbolTable::HandlePointerUpEventL(
    const TPoint& aPoint )
    {
    CFepUiBaseCtrl* ctrl = CControlGroup::HandlePointerUpEventL( aPoint );

    if(ctrl)
	   {
	   ctrl->Draw();
	   ctrl->UpdateArea(ctrl->Rect(),EFalse);
	   }

	return ctrl;
    }

// ---------------------------------------------------------------------------
// Handle pointer move event.
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputArabicFingerHwrSymbolTable::HandlePointerMoveEventL(
    const TPoint& aPoint )
    {
	CFepUiBaseCtrl* ctrl = CControlGroup::HandlePointerMoveEventL( aPoint );
	if(ctrl)
	    {
	    ctrl->Draw();
		ctrl->UpdateArea(ctrl->Rect(),EFalse);
		iPointerMoveBack = ETrue;
		}
	else
	    {
	    if(iPointerMoveBack)
	        {
            if(iPenDownCtrl)
                {
                iPenDownCtrl->HandlePointerLeave(aPoint);
                }
            iPointerMoveBack = EFalse;
	        }
	    }

    return ctrl;
    }

void CPeninputArabicFingerHwrSymbolTable::HandlePointerLeave(const TPoint& aPoint)
    {
    CControlGroup::HandlePointerLeave(aPoint);
    Draw();
    UpdateArea(Rect(),EFalse);
    }
// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrSymbolTable::CPeninputArabicFingerHwrSymbolTable( 
    CFepUiLayout* aFepUiLayout, TInt aControlId )
    : CControlGroup( aFepUiLayout, aControlId )
    {
    }
    
// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::ConstructL()
    {
    BaseConstructL(); 
	
    //disable border
    SetBorderSize( TSize(0,0) );
    
	CreateVirtualKeypadL();
	
    CreateButtonGroupL();
    }

void CPeninputArabicFingerHwrSymbolTable::SizeChanged(
                     const TSize /*aKeypadSize*/, const TSize aButtonSize,
                     const TInt aKeypadRow, const TInt aKeypadCol, TBool aIsLandscape)
    {
	iIsLandscape = aIsLandscape;
	
	// relayout the button group
	TRect spaceBtnRect(iRect.iTl,iRect.iTl+TPoint(aButtonSize.iWidth,aButtonSize.iHeight));
	MoveIconButton(iSpaceBtn,spaceBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	
	TRect innerRect = spaceBtnRect;
	innerRect.Shrink(4*KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	
	// that's a not good algrithm in funciton AknPenInputDrawUtils::Draw3PiecesColorIcon for drawing 3Pieces icons 
	// as when the  outter rect's height was not equal to the inter rect's height, the target rects the left icons and right icons 
    // would be bitblited onto are Rect(0.0.0.0.0) [[which should not be that behavior]].
	// Please theck the codes: AknPenInputDrawUtils::Draw3PiecesColorIcon: this function should be Updated.
	iSpaceBtn->SetDraw3PieceFrameInfo(
									  TDraw3PiecesFrame(KAknsIIDQgnIndiInputSpaceL,
									  KAknsIIDQgnIndiInputSpaceMiddle,
									  KAknsIIDQgnIndiInputSpaceR,
									  innerRect));
	TRect enterBtnRect = spaceBtnRect;
	enterBtnRect.Move(TPoint(aButtonSize.iWidth,0));
	
	MoveIconButton(iEnterBtn,enterBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	
	TRect pageBtnRect = enterBtnRect;
	pageBtnRect.Move(TPoint(aButtonSize.iWidth,0));
	
	if(aIsLandscape)
	    {
		i3Page1Btn->Hide(ETrue);
		i3Page2Btn->Hide(ETrue);
		i3Page3Btn->Hide(ETrue);
		i2Page1Btn->Hide(EFalse);
		i2Page2Btn->Hide(EFalse);
		MoveIconButton(i2Page1Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	    MoveIconButton(i2Page2Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
		}
	else
        {
		i2Page1Btn->Hide(EFalse);
		i2Page2Btn->Hide(EFalse);
		i3Page1Btn->Hide(EFalse);
        i3Page2Btn->Hide(ETrue);
        i3Page3Btn->Hide(ETrue);
		MoveIconButton(i3Page1Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	    MoveIconButton(i3Page2Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	    MoveIconButton(i3Page3Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
		}	

	// relayout the virtual key pad
	TRect keypadRect = iRect;
	keypadRect.iTl = keypadRect.iTl + TPoint(0,aButtonSize.iHeight);
	
	iMutiPageKeypad->SetRect(keypadRect);
	
	iMutiPageKeypad->UpdatePaging(aKeypadRow,aKeypadCol);
	}
	
// ---------------------------------------------------------------------------
// create symbol table keypad.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::CreateVirtualKeypadL()
    {
    TFontSpec spec;

    iMutiPageKeypad = CMultiPageVirtualKeyboard::NewL( 
        TRect(0,0,0,0),
        UiLayout(),
        EHwrCtrlIdSctpad,
        spec );

    AddControlL( iMutiPageKeypad );
    iMutiPageKeypad->AddEventObserver( UiLayout() );
    
    iMutiPageKeypad->SetResourceId( KInvalidResId );
        
    iMutiPageKeypad->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );
    iMutiPageKeypad->SetDrawOpaqueBackground( EFalse );    
    }

void CPeninputArabicFingerHwrSymbolTable::CreateButtonGroupL()
    {
    iEnterBtn     = CreateEventBtnL(EHwrCtrlIdBtnEnter,R_AKN_FEP_HWR_ENTER);
	iSpaceBtn     = CreateEventBtnL(EHwrCtrlIdBtnSpace,R_AKN_FEP_HWR_SPACE);
	// Create 3-pieces color icon for space button
	CPenInput3PiecesColorIcons* colorIcons = 
				CPenInput3PiecesColorIcons::NewL( R_AKN_FEP_VKB_SPACE_ICON1,
												  R_AKN_FEP_VKB_SPACE_ICON2,
												  R_AKN_FEP_VKB_SPACE_ICON3 );
	iSpaceBtn->Set3PiecesColorIcons( colorIcons );											  
	
	// create the page button for portrait
	i3Page1Btn  = CreateEventBtnL(EHwrCtrlId3Page1Btn, R_AKN_FEP_ARABIC_3PAGE_HWR_SCT_PAGE_1);
    i3Page2Btn  = CreateEventBtnL(EHwrCtrlId3Page2Btn, R_AKN_FEP_ARABIC_3PAGE_HWR_SCT_PAGE_2);
    i3Page3Btn  = CreateEventBtnL(EHwrCtrlId3Page3Btn, R_AKN_FEP_ARABIC_3PAGE_HWR_SCT_PAGE_3);
	
	// create the page button for landscape
	i2Page1Btn  = CreateEventBtnL(EHwrCtrlId2Page1Btn, R_AKN_FEP_ARABIC_2PAGE_HWR_SCT_PAGE_1);
    i2Page2Btn  = CreateEventBtnL(EHwrCtrlId2Page2Btn, R_AKN_FEP_ARABIC_2PAGE_HWR_SCT_PAGE_2);
	}
	
// ---------------------------------------------------------------------------
// EventButton creation helper.
// ---------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputArabicFingerHwrSymbolTable::CreateEventBtnL( 
    TInt aCtrlId, TInt32 aResId, TInt aEvent/*= 0xFFFF*/,TInt aUnicode/*=0*/ )
    {
    CAknFepCtrlEventButton* button = CAknFepCtrlEventButton::NewL( 
        UiLayout(), aCtrlId, aEvent, aUnicode,
        KAknsIIDQsnFrFunctionButtonNormal,
        KAknsIIDQsnFrFunctionButtonPressed,
        KAknsIIDQsnFrFunctionButtonInactive );
    
    button->SetResourceId( aResId );
    button->ConstructFromResourceL();
    button->AddEventObserver( UiLayout() );        
    AddControlL( button );
    return button;
    }

// ---------------------------------------------------------------------------
//  EventButton layout helper. Move button to specified rect.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::MoveIconButton( CAknFepCtrlEventButton* aButton, 
    const TRect& aRect, TInt aXPadding, TInt aYPadding, TBool aReloadImages )
    {
    if ( !aButton )
        {
        return;
        }
    
    aButton->SetRect( aRect );
    TRect rcInner = aRect;
    rcInner.Shrink( aXPadding, aYPadding );
    aButton->SizeChanged( aRect, rcInner, aReloadImages );
    }

void CPeninputArabicFingerHwrSymbolTable::OnActivate()
    {
    CControlGroup::OnActivate();
	if(IsPopup())
	    {
	    CapturePointer( ETrue );
	    NavigatePage(0,EPagePosPageNo);
		}
	}
	
void CPeninputArabicFingerHwrSymbolTable::LoadVkbKeyImageL(TInt aResId, const TSize& aKeySize)
    {
	TResourceReader reader;    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );      
    
    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TAknsItemID id;
    
    TSize keySize = aKeySize;
    
    for ( TInt index = 0; index <= EKeyBmpLastType ; index += 2 )
        { 
        // Get the image ids and mask ids from resource
        TInt bmpId = reader.ReadInt16(); 
        TInt bmpMskId = reader.ReadInt16();
        
        // read skin item id
        const TInt skinitemid = reader.ReadInt16();
        id.Set( TInt( imgMajorSkinId ), skinitemid );
        
        if ( bmpId != KInvalidImg )
            {
            CFbsBitmap* bmp = NULL;
            CFbsBitmap* maskbmp = NULL;

            if ( bmpMskId != KInvalidImg )
                {
                AknsUtils::CreateIconL( AknsUtils::SkinInstance(),
                   id, bmp, maskbmp, bmpFileName, bmpId, bmpMskId );
                
                // set maskbmp and size
                AknIconUtils::SetSize( maskbmp, keySize, EAspectRatioNotPreserved );
                iMutiPageKeypad->SetNonIrregularKeyBitmapL( 
                TVirtualKeyBmpType( EKeyBmpNormal + index + 1 ), maskbmp );
                }
            else
                {
                AknsUtils::CreateIconL( AknsUtils::SkinInstance(), id, 
                    bmp, bmpFileName, bmpId );
                }
            // set bmp and size
            AknIconUtils::SetSize( bmp, keySize, EAspectRatioNotPreserved );
            iMutiPageKeypad->SetNonIrregularKeyBitmapL( 
                TVirtualKeyBmpType( EKeyBmpNormal + index ), bmp );
            }       
        }
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );        
	}
	
void CPeninputArabicFingerHwrSymbolTable::LoadVirtualKeypadKeyL(const TInt aResId, const RArray<TRect>& aCellRects)
    {
	iMutiPageKeypad->SetResourceId(aResId);
	
	TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );

    // construct keys
    TInt resKeyCount = reader.ReadInt16();
    TInt existsKeyCount = iMutiPageKeypad->KeyArray().Count();
    TInt rectCount = aCellRects.Count();
    
    for ( TInt i = 0; i < resKeyCount; i++ )
        {
        if ( i < existsKeyCount )
            {
            CVirtualKey* vk = iMutiPageKeypad->KeyArray()[i];
            UpdateVkbKeyL( vk, reader, aCellRects[i%rectCount] );
            }
        else
            {
            CVirtualKey* vk = CreateVkbKeyL( reader, aCellRects[i%rectCount] );
            CleanupStack::PushL( vk );
            iMutiPageKeypad->AddKeyL( vk );
            CleanupStack::Pop( vk );
            }
        }
    
    CleanupStack::PopAndDestroy( 1 ); // reader
    
    iMutiPageKeypad->Draw();
    iMutiPageKeypad->UpdateArea( iMutiPageKeypad->Rect() );
	}

// ---------------------------------------------------------------------------
// create virtual key.
// ---------------------------------------------------------------------------
//
CVirtualKey* CPeninputArabicFingerHwrSymbolTable::CreateVkbKeyL( TResourceReader& aReader, 
    const TRect aKeyRect )
    {
    CHBufCArray* keytexts = CHBufCArray::NewL();
    CleanupStack::PushL( keytexts );
    
    for ( TInt i = 0; i <= EPosLast; i++ )
        {
        HBufC* unicode = aReader.ReadHBufCL();
        keytexts->Array().AppendL( unicode );
        }

    TInt keyscancode = aReader.ReadInt16();

    HBufC* text = keytexts->Array()[0];
    
    CVirtualKey* vk = NULL;
    if ( text )
        {
        vk = CVirtualKey::NewL( *text, keyscancode, aKeyRect, aKeyRect, 0 );
        }
    else 
        {
        vk = CVirtualKey::NewL( KNullDesC, keyscancode, aKeyRect, aKeyRect, 0 );
        }

    CleanupStack::PopAndDestroy( keytexts ); //keytexts


    TRect innerrect = aKeyRect;
    innerrect.Shrink( TSize(10, 10) );
    vk->SetInnerRect( innerrect );
    
    return vk;
    }


// ---------------------------------------------------------------------------
// update virtual key info.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::UpdateVkbKeyL( CVirtualKey* aVirtualKey, 
    TResourceReader& aReader, const TRect aKeyRect )
    {
    CHBufCArray* keytexts = CHBufCArray::NewL();
    CleanupStack::PushL( keytexts );
    
    for ( TInt i = 0; i <= EPosLast; i++ )
        {
        HBufC* unicode = aReader.ReadHBufCL();
        keytexts->Array().AppendL( unicode );
        }

    TInt keyscancode = aReader.ReadInt16();

    HBufC* text = keytexts->Array()[0];
    if ( text )
        {
        aVirtualKey->SetKeyData( *text );
        }
    else
        {
        aVirtualKey->SetKeyData( KNullDesC );
        }
    
    aVirtualKey->SetKeyScancode( keyscancode );

    CleanupStack::PopAndDestroy( keytexts ); //keytexts

    aVirtualKey->SetRect(aKeyRect);
    TRect innerrect = aKeyRect;
    innerrect.Shrink( TSize(10,10) );
    aVirtualKey->SetInnerRect( innerrect );
    }

// ---------------------------------------------------------------------------
// Change SCT page.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::NavigatePage( TInt aPageNo, TInt aPos )
    {
    switch ( aPos )
        {
        case EPagePosPageNo:
            {
            iMutiPageKeypad->NavToPage( aPageNo );
            }
            break;
        case EPagePosNextPage:
            {
            iMutiPageKeypad->NavToNextPage();
            }
            break;
            
        case EPagePosPrevPage:
            {
            iMutiPageKeypad->NavToPrevPage();
            }
            break;
            
        case EPagePosLastPage:
            {
            iMutiPageKeypad->NavToLastPage( );
            }
            break;
            
        case EPagePosFirstPage:
            {
            iMutiPageKeypad->NavToFirstPage();
            }
            break;
            
        default:
            break;
        }
		
	iMutiPageKeypad->Draw();		
    UpdatePageButtonsUi();
    UpdateAllVirtualKeysFeedback( EFingerHwrSymbolRange );
	iEnterBtn->Draw();
	iSpaceBtn->Draw();
	UpdateArea(iRect);
    }

// ---------------------------------------------------------------------------
// sync sct paging buttons.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::UpdatePageButtonsUi()
    {
    TInt curpage = iMutiPageKeypad->CurPageIndex();
    TInt pagecount = iMutiPageKeypad->PageCount();
    
	if(iIsLandscape)
	    {
		i2Page1Btn->Hide( curpage != 0 );
        i2Page2Btn->Hide( curpage != 1 );
		}
	else
        {
		i3Page1Btn->Hide( curpage != 0 );
        i3Page2Btn->Hide( curpage != 1 );
        i3Page3Btn->Hide( curpage != 2 );
		}
    }

// ---------------------------------------------------------------------------
//  update feedback state of all virtual keys.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::UpdateAllVirtualKeysFeedback( TInt aType )
    {
    TBool isSctpad = ( aType == EFingerHwrSymbolRange ) ? ETrue : EFalse;
    
    //update sctpad keys
    TInt keyCount = iMutiPageKeypad->KeyArray().Count();
    TInt pageSize = iMutiPageKeypad->PageSize();
    TInt curPage = iMutiPageKeypad->CurPageIndex();
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iMutiPageKeypad->KeyArray()[i];
        TInt page = i / pageSize;
        TBool enable = !vk->Dimmed() &&  isSctpad && ( page == curPage );
        iMutiPageKeypad->EnableKeyFeedback( vk, enable );
        }
    }
// End Of File
