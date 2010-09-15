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
#include <AknIconUtils.h>
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
#include "peninputfingerhwarvkbutility.h"

const TUint16 KInvalidChar = 0xFFFF;
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
    UiLayout()->LockArea(UiLayout()->Rect(),this);
    // signal server to add the pop area        
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        struct SData
            {
            TRect rr;
            TBool flag;
            } data;
            
        data.rr = iNumKeypad->Rect();
        data.flag = ETrue;
        TPtrC ptrForAdd;
        ptrForAdd.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));

        UiLayout()->SignalOwner(ESignalPopupArea,ptrForAdd);
        }
    
    RefreshNumSymbolTable();;
	}

// ---------------------------------------------------------------------------
// cancel the popup.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::CloseSymbolTable()
    {
    CapturePointer( EFalse );
    iPopupVisible = EFalse;
    // signal server to remove the pop area        
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        struct SData
            {
            TRect rr;
            TBool flag;
            } data;
            
        data.rr = iNumKeypad->Rect();
        data.flag = EFalse;
        TPtrC ptrForRemove;
        ptrForRemove.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));

        UiLayout()->SignalOwner(ESignalPopupArea,ptrForRemove);
        }    
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
void CPeninputArabicFingerHwrNumSymbolTable::SizeChanged(const TRect& aVirtualKeypadRect)
    {
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
    PeninputFingerHwrArVkbUtility::LoadVkbKeyImageL(*iNumKeypad,aResId,aKeySize);
	}

// ---------------------------------------------------------------------------
// Load virtual keys
// ---------------------------------------------------------------------------
//	
void CPeninputArabicFingerHwrNumSymbolTable::LoadVirtualKeypadKeyL(const TInt aResId, const RArray<TRect>& aCellRects)
    {
	PeninputFingerHwrArVkbUtility::LoadVirtualKeypadKeyL(*iNumKeypad,aResId,aCellRects);
	}

// ---------------------------------------------------------------------------
// accept editor's number mapping restriction.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::UpdateTableSymbol( const TDesC& aNumMapping )
    {
    HBufC* charTable = GenerateCharTable(aNumMapping);
    if(charTable)
    	{
    	TInt charTalbeCount = charTable->Length();
     
	    //numberpad
	    TInt keyCount = iNumKeypad->KeyArray().Count();
	    for ( TInt i = 0; i < keyCount; i++ )
	        {
	        CVirtualKey* vk = iNumKeypad->KeyArray()[i];
	        
	        if(i < charTalbeCount && (*charTable)[i] != KInvalidChar)
	            {
	            TBuf<1> keydata;
                TUint16 keyCode = (*charTable)[i];	            
	            keydata.Append(keyCode);
	            vk->SetKeyData(keydata);
	            vk->SetKeyScancode(keyCode);
	            vk->SetDimmed( EFalse );
	            }
	        else
	            {
	            vk->SetKeyData( KNullDesC );
	            vk->SetKeyScancode( KInvalidChar );
	            vk->SetDimmed( EFalse );
	            }
	        }
	    
	    delete charTable;
	    
	    //sync feedback
	    UpdateAllVirtualKeysFeedback();
    	}
    }

// ---------------------------------------------------------------------------
// Get reordered char table for key pad to render them
// ---------------------------------------------------------------------------
//
HBufC* CPeninputArabicFingerHwrNumSymbolTable::GenerateCharTable(const TDesC& aNumMapping)
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
    TInt tableSize = sizeof(KNumOffsets)/sizeof(TInt);
    TInt mappingItemCount = aNumMapping.Length();
    
    HBufC* reorderedCharTable = HBufC::New(mappingItemCount +1);
    if(reorderedCharTable)
    	{
    	for(TInt i = 0; i < tableSize; i++)
    	    {    	    
    	    if(KNumOffsets[i] < mappingItemCount)
    	    	{
    	    	TUint16 unicode = aNumMapping[KNumOffsets[i]];
                TUint16 mappedCode = MapLatinNumAccordingToNumMode(unicode);	
                reorderedCharTable->Des().Append(mappedCode); 
    	    	}
    	    else
    	    	{
    	        reorderedCharTable->Des().Append(KInvalidChar);
    	    	}	
    	    }
    	}
    
    return reorderedCharTable;	    
	}
	
// ---------------------------------------------------------------------------
// Navigate the symbol page
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::RefreshNumSymbolTable()
    {
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
    //update numpad keys
    TInt keyCount = iNumKeypad->KeyArray().Count();
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iNumKeypad->KeyArray()[i];
        TBool enable = !vk->Dimmed();
        iNumKeypad->EnableKeyFeedback( vk, enable );
        }
    }

// ---------------------------------------------------------------------------
//  update rect of all virtual keys.
// ---------------------------------------------------------------------------
//
void CPeninputArabicFingerHwrNumSymbolTable::UpdateAllVirtualKeysRect(const RArray<TRect> & aCellRects)
    {
    //update numpad keys rect
    TInt keyCount = iNumKeypad->KeyArray().Count();
    TInt rectCount = aCellRects.Count();
    if(keyCount != rectCount)
        {
        return;
        }
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iNumKeypad->KeyArray()[i];
        TRect rect = aCellRects[i%rectCount];        
        vk->SetRect(aCellRects[i%rectCount]);
        TRect innerrect = rect;
        innerrect.Shrink( TSize(10,10) );
        vk->SetInnerRect( innerrect );
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
