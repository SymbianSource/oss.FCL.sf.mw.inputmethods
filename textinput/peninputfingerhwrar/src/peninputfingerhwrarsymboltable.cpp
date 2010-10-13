/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of symbol table.
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
#include <akniconutils.h>
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>

#include <peninputlayout.h>
#include <peninputrepeatbutton.h>

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
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrSymbolTable::CPeninputArabicFingerHwrSymbolTable( 
    CFepUiLayout* aFepUiLayout, TInt aControlId )
    : CControlGroup( aFepUiLayout, aControlId )
    {
	SetControlType(ECtrlControlGroup|ECtrlTactileFeedback);
	SetTactileFeedbackType(ETouchFeedbackSensitiveInput);
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
	UiLayout()->LockArea(UiLayout()->Rect(),this);  
    NavigatePage(0,EPagePosPageNo);
	}

// ---------------------------------------------------------------------------
// cancel the popup.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::CloseSymbolTable()
    {
    CapturePointer( EFalse );
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
	
    if(!ctrl)
	   {
	   ReportEvent(EHwrEventOutsideEvent);
	   }
	else
       {
	    #ifdef RD_TACTILE_FEEDBACK
        if (UiLayout()->SupportTactileFeedback())
            {
            UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveInput);
            }
        #endif //RD_TACTILE_FEEDBACK
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
		HandlePointerEnter(aPoint);
		ctrl->UpdateArea(ctrl->Rect(),EFalse);
		}
	else
	    {
	    HandlePointerLeave(aPoint);
	    UpdateArea(iRect,EFalse);
	    }
	
    return ctrl;
    }
    
// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::ConstructL()
    {
    BaseConstructL(); 
	
    // disable border
    SetBorderSize( TSize(0,0) );
    
	// construct virtual keypad
	CreateVirtualKeypadL();
	
	// construct button group
    CreateButtonGroupL();
    }

// ---------------------------------------------------------------------------
// SizeChanged
// ---------------------------------------------------------------------------
//	
void CPeninputArabicFingerHwrSymbolTable::SizeChanged(
                     const TRect aVirtualKeypadRect, const RArray<TRect> aBtnRects,
                     const TInt aKeypadRow, const TInt aKeypadCol, TBool aIsLandscape)
    {
	iIsLandscape = aIsLandscape;
	
	// relayout the button
	TRect pageBtnRect(aBtnRects[0]);

	if(aIsLandscape)
	    {
	    i2Page1Btn->Hide(EFalse);
	    i2Page2Btn->Hide(EFalse);
	    
	    i3Page1Btn->Hide(ETrue);
	    i3Page2Btn->Hide(ETrue);
	    i3Page3Btn->Hide(ETrue);
		MoveIconButton(i2Page1Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	    MoveIconButton(i2Page2Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
		}
	else
        {
	    i2Page1Btn->Hide(ETrue);
        i2Page2Btn->Hide(ETrue);
        
        i3Page1Btn->Hide(EFalse);
        i3Page2Btn->Hide(EFalse);
        i3Page3Btn->Hide(EFalse);
		MoveIconButton(i3Page1Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	    MoveIconButton(i3Page2Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
	    MoveIconButton(i3Page3Btn,pageBtnRect,KSymbolButtonInnerPadding,KSymbolButtonInnerPadding);
		}
    
	// relayout the virtual key pad
	iMutiPageKeypad->SetRect(aVirtualKeypadRect);
	
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
        EHwrCtrlIdSymbolTableVkb,
        spec );

    AddControlL( iMutiPageKeypad );
    iMutiPageKeypad->AddEventObserver( UiLayout() );
    
	iMutiPageKeypad->SetKeySkinId( EKeyBmpNormal, KAknsIIDQsnFrKeypadButtonFrNormal );
    iMutiPageKeypad->SetKeySkinId( EKeyBmpHighlight, KAknsIIDQsnFrKeypadButtonFrPressed );
    iMutiPageKeypad->SetKeySkinId( EKeyBmpDim, KAknsIIDQsnFrKeypadButtonFrInactive );
    iMutiPageKeypad->SetResourceId( KInvalidResId );
        
    iMutiPageKeypad->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );
    iMutiPageKeypad->SetDrawOpaqueBackground( EFalse );    
    }

void CPeninputArabicFingerHwrSymbolTable::CreateButtonGroupL()
    {
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
	}

// ---------------------------------------------------------------------------
//  Read control's background info.
// ---------------------------------------------------------------------------
//	
void CPeninputArabicFingerHwrSymbolTable::ConstructFromResourceL()
    {
	if ( iResourceId == KInvalidResId )
        {
        User::Leave( KErrArgument );
        }
    
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, iResourceId );
    
	const TInt32 bgImageResId = reader.ReadInt32();
	LoadBackgroundFromResourceL(bgImageResId);
	
	CleanupStack::PopAndDestroy( 1 ); //reader
	
	
	CControlGroup::ConstructFromResourceL();
	}
	
// ---------------------------------------------------------------------------
//  Read control's background info.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::LoadBackgroundFromResourceL( const TInt aResId )
    {
    if ( aResId == 0 )
        return;
    
    if ( BackgroundBmp() )
        {
        CFbsBitmap* bkBmp = BackgroundBmp();
        delete bkBmp;   
        SetBackgroundBmp( NULL );
        }
    if ( BkMaskBmp() )
        {
        CFbsBitmap* bkMaskBmp = BkMaskBmp();
        delete bkMaskBmp;
        SetBackgroundMaskBmp( NULL );
        }    

    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, aResId ); 
     
    TPtrC bmpFileName = resReader.ReadTPtrC();
    TInt32 imgMajorSkinId = resReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    const TInt16 bmpId = resReader.ReadInt16();
    const TInt16 bmpMskId = resReader.ReadInt16();
    skinitemid = resReader.ReadInt16();   
    id.Set( TInt( imgMajorSkinId ), skinitemid );
    
    if ( bmpId != KInvalidImg )
        {
        CFbsBitmap* backgroundImg = NULL;
        CFbsBitmap* backgroundMaskImg = NULL;
        if ( bmpMskId != KInvalidImg )
            {
            AknsUtils::CreateIconL( skininstance, id, backgroundImg,
                    backgroundMaskImg, bmpFileName, bmpId, bmpMskId );
            }
        else
            {
            AknsUtils::CreateIconL( skininstance, id, backgroundImg,
                    bmpFileName, bmpId );
            }
       
        SetBackgroundBmp( backgroundImg );
        SetBackgroundMaskBmp( backgroundMaskImg );           
        }
    CleanupStack::PopAndDestroy( 1 ); //reader
    }
	
// ---------------------------------------------------------------------------
// Load virtual keys image
// ---------------------------------------------------------------------------
//	
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

// ---------------------------------------------------------------------------
// Load virtual keys
// ---------------------------------------------------------------------------
//	
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
    innerrect.Shrink( TSize(2, 2) );
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
    innerrect.Shrink( TSize(2,2) );
    aVirtualKey->SetInnerRect( innerrect );
    }

// ---------------------------------------------------------------------------
// Navigate the symbol page
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
		
	// Show page button
    UpdatePageButtonsUi();
	
	// Draw the symbol table 
    Draw();
	
	// Update the feedback for the virtual key
    UpdateAllVirtualKeysFeedback();
	
	// Bitblt the whole symbol table to screen
	UpdateArea(iRect);
    }

// ---------------------------------------------------------------------------
// Show the button page which should be visible
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
void CPeninputArabicFingerHwrSymbolTable::UpdateAllVirtualKeysFeedback()
    {
    //update sctpad keys
    TInt keyCount = iMutiPageKeypad->KeyArray().Count();
    TInt pageSize = iMutiPageKeypad->PageSize();
    TInt curPage = iMutiPageKeypad->CurPageIndex();
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iMutiPageKeypad->KeyArray()[i];
        TInt page = i / pageSize;
        TBool enable = !vk->Dimmed() && ( page == curPage );
        iMutiPageKeypad->EnableKeyFeedback( vk, enable );
        }
    }

// ---------------------------------------------------------------------------
//  Draw new content to the screen
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrSymbolTable::Draw()
    {
	if(AbleToDraw())
        {
        //Draw button area background.
//		DrawOpaqueMaskBackground( Rect() );
//	    DrawOpaqueMaskBackground();
        DrawOpaqueMaskBackground(iMutiPageKeypad->Rect());
		if( BackgroundBmp() && BackgroundBmp()->SizeInPixels() != Rect().Size() )
			{
			AknIconUtils::SetSize(BackgroundBmp(), Rect().Size(), EAspectRatioNotPreserved);
			}
        // draw background
//		DrawBackground();    
        DrawBackground(iMutiPageKeypad->Rect());
        // draw group		
		CControlGroup::Draw();
		
		// bitblt the new content to screen
		UpdateArea(iRect);
		}
	}
// End Of File
