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
#include "peninputlayoutvkb.h"
#include "peninputfingerhwrarnumsymboltable.h"


// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrNumSymbolTable* CPeninputArabicFingerHwrNumSymbolTable::NewL( 
    CFepUiLayout* aUiLayout, TInt aId )
    {
    CPeninputArabicFingerHwrNumSymbolTable* self = CPeninputArabicFingerHwrNumSymbolTable::NewLC( 
            aUiLayout, aId );
    
    CleanupStack::Pop( self ); // self;
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrNumSymbolTable* CPeninputArabicFingerHwrNumSymbolTable::NewLC( 
    CFepUiLayout* aUiLayout, TInt aId )
    {
    CPeninputArabicFingerHwrNumSymbolTable* self = new (ELeave) CPeninputArabicFingerHwrNumSymbolTable( 
            aUiLayout, aId );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrNumSymbolTable::CPeninputArabicFingerHwrNumSymbolTable( 
    CFepUiLayout* aFepUiLayout, TInt aControlId )
    : CControlGroup( aFepUiLayout, aControlId ),iIsNativeNumMode(EFalse)
    {
	SetControlType(ECtrlControlGroup|ECtrlTactileFeedback);
	SetTactileFeedbackType(ETouchFeedbackSensitiveInput);
    }
	
// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrNumSymbolTable::~CPeninputArabicFingerHwrNumSymbolTable()
    {
    }

// ---------------------------------------------------------------------------
// popup the list.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::OpenSymbolTable()
    {
    CapturePointer( ETrue );
    iPopupVisible = ETrue; 
    iCurrentNumSCTType = ENumSCTLatin;
	UiLayout()->LockArea(UiLayout()->Rect(),this);  
    UpdateNumSymbolTable(ENumSCTLatin);
	}

// ---------------------------------------------------------------------------
// cancel the popup.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::CloseSymbolTable()
    {
    CapturePointer( EFalse );
    iPopupVisible = EFalse;
    UiLayout()->UnLockArea(UiLayout()->Rect(),this);
    }

// ---------------------------------------------------------------------------
// get visibility of popup.
// ---------------------------------------------------------------------------
//
TBool CPeninputArabicFingerHwrNumSymbolTable::IsPopup()
    {
    return iPopupVisible;
    }

// ---------------------------------------------------------------------------
// Handle pointer down event.
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputArabicFingerHwrNumSymbolTable::HandlePointerDownEventL(
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
CFepUiBaseCtrl* CPeninputArabicFingerHwrNumSymbolTable::HandlePointerUpEventL(
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
CFepUiBaseCtrl* CPeninputArabicFingerHwrNumSymbolTable::HandlePointerMoveEventL(
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
void CPeninputArabicFingerHwrNumSymbolTable::ConstructL()
    {
    BaseConstructL(); 
	
    // disable border
    SetBorderSize( TSize(0,0) );
    
	// construct virtual keypad
	CreateVirtualKeypadL();
    }

// ---------------------------------------------------------------------------
// SizeChanged
// ---------------------------------------------------------------------------
//	
void CPeninputArabicFingerHwrNumSymbolTable::SizeChanged(
                     const TRect aVirtualKeypadRect, const RArray<TRect> /*aBtnRects*/,
                     const TInt /*aKeypadRow*/, const TInt /*aKeypadCol*/, TBool aIsLandscape)
    {
	iIsLandscape = aIsLandscape;
	
	// relayout the virtual key pad
	iNumKeypad->SetRect(aVirtualKeypadRect);
	
	}
	
// ---------------------------------------------------------------------------
// create symbol table keypad.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::CreateVirtualKeypadL()
    {
    TFontSpec spec;

    iNumKeypad = CVirtualKeyboard::NewL( 
        TRect(0,0,0,0),
        UiLayout(),
        EHwrCtrlIdNumSymbolTableVkb,
        spec );

    AddControlL( iNumKeypad );
    iNumKeypad->AddEventObserver( UiLayout() );
    
	iNumKeypad->SetKeySkinId( EKeyBmpNormal, KAknsIIDQsnFrKeypadButtonFrNormal );
    iNumKeypad->SetKeySkinId( EKeyBmpHighlight, KAknsIIDQsnFrKeypadButtonFrPressed );
    iNumKeypad->SetKeySkinId( EKeyBmpDim, KAknsIIDQsnFrKeypadButtonFrInactive );
    iNumKeypad->SetResourceId( KInvalidResId );
        
    iNumKeypad->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );
    iNumKeypad->SetDrawOpaqueBackground( EFalse );    
    }

void CPeninputArabicFingerHwrNumSymbolTable::OnActivate()
    {
    CControlGroup::OnActivate();
	}

// ---------------------------------------------------------------------------
//  Read control's background info.
// ---------------------------------------------------------------------------
//	
void CPeninputArabicFingerHwrNumSymbolTable::ConstructFromResourceL()
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
void CPeninputArabicFingerHwrNumSymbolTable::LoadBackgroundFromResourceL( const TInt aResId )
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
void CPeninputArabicFingerHwrNumSymbolTable::LoadVkbKeyImageL(TInt aResId, const TSize& aKeySize)
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
                iNumKeypad->SetNonIrregularKeyBitmapL( 
                TVirtualKeyBmpType( EKeyBmpNormal + index + 1 ), maskbmp );
                }
            else
                {
                AknsUtils::CreateIconL( AknsUtils::SkinInstance(), id, 
                    bmp, bmpFileName, bmpId );
                }
            // set bmp and size
            AknIconUtils::SetSize( bmp, keySize, EAspectRatioNotPreserved );
            iNumKeypad->SetNonIrregularKeyBitmapL( 
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
void CPeninputArabicFingerHwrNumSymbolTable::LoadVirtualKeypadKeyL(const TInt aResId, const RArray<TRect>& aCellRects)
    {
	iNumKeypad->SetResourceId(aResId);
	
	TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );

    // construct keys
    TInt resKeyCount = reader.ReadInt16();
    TInt existsKeyCount = iNumKeypad->KeyArray().Count();
    TInt rectCount = aCellRects.Count();
    
    for ( TInt i = 0; i < resKeyCount; i++ )
        {
        if ( i < existsKeyCount )
            {
            CVirtualKey* vk = iNumKeypad->KeyArray()[i];
            UpdateVkbKeyL( vk, reader, aCellRects[i%rectCount] );
            }
        else
            {
            CVirtualKey* vk = CreateVkbKeyL( reader, aCellRects[i%rectCount] );
            CleanupStack::PushL( vk );
            iNumKeypad->AddKeyL( vk );
            
            CleanupStack::Pop( vk );
            }
        }
    
    CleanupStack::PopAndDestroy( 1 ); // reader
    
    iNumKeypad->Draw();
    iNumKeypad->UpdateArea( iNumKeypad->Rect() );
	}

// ---------------------------------------------------------------------------
// create virtual key.
// ---------------------------------------------------------------------------
//
CVirtualKey* CPeninputArabicFingerHwrNumSymbolTable::CreateVkbKeyL( TResourceReader& aReader, 
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
void CPeninputArabicFingerHwrNumSymbolTable::UpdateVkbKeyL( CVirtualKey* aVirtualKey, 
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
// accept editor's number mapping restriction.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::SetNumericMapping( const TDesC& aNumMapping )
    {
    //format of aNumMapping is "0123456789******"
    
    //char offset in aNumMapping
    //cell 0 using aNumMapping[KNumOffsets[0]]
    //cell 1 using aNumMapping[KNumOffsets[1]]
    const TInt KNumOffsets[] = 
        {
         12,  1,  2, 3,
         13,  4,  5, 6,
         14,  7,  8, 9,
         15,  10, 0, 11
        };
    
    TInt maxMappingItemCount = sizeof(KNumOffsets)/sizeof(TInt);
    TInt mappingItemCount = aNumMapping.Length();
    
    //numberpad
    TInt keyCount = iNumKeypad->KeyArray().Count();
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iNumKeypad->KeyArray()[i];
        
        TInt offset = ( i < maxMappingItemCount ) ? KNumOffsets[i] : -1;
        
        if ( offset > -1  && offset < mappingItemCount )
            {
            TUint16 unicode = aNumMapping[offset];
            TUint16 mappedCode = MapLatinNumAccordingToNumMode(unicode);
            TBuf<1> keydata;
            keydata.Append(mappedCode);
            vk->SetKeyData(keydata);
            vk->SetKeyScancode( mappedCode);
            vk->SetDimmed( EFalse );
            }
        else
            {
            vk->SetKeyData( KNullDesC );
            vk->SetKeyScancode( 0xFFFF );
            vk->SetDimmed( EFalse );
            }
        }
    
    
    //sync feedback
    UpdateAllVirtualKeysFeedback();
    }

// ---------------------------------------------------------------------------
// Navigate the symbol page
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::UpdateNumSymbolTable( TInt aNumSctType)
    {	

    iCurrentNumSCTType = aNumSctType;
	// Draw the symbol table 
    Draw();
	
	// Update the feedback for the virtual key
    UpdateAllVirtualKeysFeedback();
	
	// Bitblt the whole symbol table to screen
	UpdateArea(iRect);
    }

// ---------------------------------------------------------------------------
//  update feedback state of all virtual keys.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::UpdateAllVirtualKeysFeedback()
    {
    //update sctpad keys
    TInt keyCount = iNumKeypad->KeyArray().Count();
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iNumKeypad->KeyArray()[i];
        TBool enable = !vk->Dimmed();
        iNumKeypad->EnableKeyFeedback( vk, enable );
        }
    }

// ---------------------------------------------------------------------------
//  Draw new content to the screen
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::Draw()
    {
	if(AbleToDraw())
        {
		DrawOpaqueMaskBackground( Rect() );

		if( BackgroundBmp() && BackgroundBmp()->SizeInPixels() != Rect().Size() )
			{
			AknIconUtils::SetSize(BackgroundBmp(), Rect().Size(), EAspectRatioNotPreserved);
			}
        // draw background
		DrawBackground();    
		
        // draw group		
		CControlGroup::Draw();
		
		// bitblt the new content to screen
		UpdateArea(iRect);
		}
	}
// ---------------------------------------------------------------------------
//  set native number mode on or off.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::SetNativeNumMode(
        const TBool aIsNativeNumMode)
    {
    iIsNativeNumMode = aIsNativeNumMode;
    }
// ---------------------------------------------------------------------------
//  map latin number to arabic native number.
// ---------------------------------------------------------------------------
//
TInt16 CPeninputArabicFingerHwrNumSymbolTable:: MapLatinNumAccordingToNumMode(
        TInt16 aUnicode)
    {
    TInt16 ret = aUnicode;
    if(aUnicode >= 0x30 && aUnicode <= 0x39)
        {
        if(iIsNativeNumMode)
            {
            ret = aUnicode + 0x0660 - 0x30;
            }
        }
    return ret;
    }
// End Of File
