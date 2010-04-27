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
* Description:  CAknFepCtrlBaseWindowExt
*
*/


//  INCLUDES
#include <coemain.h>
#include <bautils.h>
#include <AknsItemID.h> 
#include <AknsSkinInstance.h>
#include <AknsUtils.h> 
#include <aknlayoutscalable_avkon.cdl.h>
 #include <AknUtils.h> 
 
#include <peninputbutton.h>
#include <peninputcommonbutton.h>
#include <peninputdragbutton.h>
#include <peninputbasewindowext.h>
#include <peninputcommonlayoutglobalenum.h>

#include <peninputlayoutinputmodechoice.h>
#include <peninputlayoutmultilineicf.h>

const TInt32 KInvalidResId = -1;
const TInt KNotSupportSkin = -1;
const TInt KInvalidImg = -1 ;
const TUint32 KDefaultTextColor = 0x000000;
 


// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlBaseWindowExt::CAknFepCtrlBaseWindowExt(CFepUiLayout* aFepUiLayout, 
                                          TInt aControlId)
    :CControlGroup(aFepUiLayout,aControlId),
    iCloseButton(NULL),
    iFirstTimeConstruct(ETrue), 
    iCoeEnv(CCoeEnv::Static())
    {
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindowExt::BaseConstructL()
    {
    CControlGroup::ConstructL();
    }
    
// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlBaseWindowExt::~CAknFepCtrlBaseWindowExt()
    {
    }

// ---------------------------------------------------------
// Draw window frame and sub controls
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindowExt::Draw()
    {
    if(Hiden() || !Ready())
        {
        return;
        }

    //Clear();
    
    DrawOpaqueMaskBackground( Rect() );
    CFbsBitGc* gc= static_cast<CFbsBitGc*> ( BitGc());
    /*
    gc->Activate( MaskBitmapDevice() );
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );
    gc->SetBrushColor( MaskBkCol() );
    gc->SetPenSize( PenSize());
    gc->SetPenColor( PenColor() );
    gc->DrawRect( Rect());
	*/
    // ----- draw bitmaps -----

    gc->Activate( BitmapDevice() ); 
    gc->SetBrushColor( TRgb(KOpaqueColor) );
    gc->SetPenColor( TRgb(KOpaqueColor) );
    gc->DrawRect( Rect());
    if( BackgroundBmp() && BackgroundBmp()->SizeInPixels() != Rect().Size() )    
    	AknIconUtils::SetSize(BackgroundBmp(), Rect().Size(), EAspectRatioNotPreserved);
    
	DrawBackground();
	CControlGroup::Draw();
	/*
    for(TInt i=0; i< NumOfControls(); i++)        
        {
        CFepUiBaseCtrl* ctrl = At(i);
        if(!ctrl->Hiden())
            {
            ctrl->Draw();        
            }
        }*/
    }

// ---------------------------------------------------------
// Close window
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindowExt::Close()
    {
    if (DoClose())
        {
        Hide(ETrue);
        }
    }

// ---------------------------------------------------------
// Enable or disable drag window
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindowExt::SetDragEnable(TBool /*aEnable*/)
    {
    /*if (iMoveButton)
        {
        iMoveButton->EnableDragging(aEnable);
        }*/
    }

// ---------------------------------------------------------
// Handle close event
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindowExt::HandleControlEvent(TInt aEventType, 
                                             CFepUiBaseCtrl* aCtrl, 
                                             const TDesC& /*aEventData*/)
    {
    if (aEventType == EEventButtonUp)
        {
        if (aCtrl->ControlId() == EPeninutWindowCtrlIdCloseBtn)
            {
            Close();
            }
        }
    }

EXPORT_C TBool CAknFepCtrlBaseWindowExt::DoClose()
    {
    return ETrue;
    }
    
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlBaseWindowExt::Control(TInt aCtrlId)
    {
    CFepUiBaseCtrl* ctrl = NULL;
    RPointerArray<CFepUiBaseCtrl> todo;
    TInt current = 0;
    TBool notFound = ETrue;
    
    todo.Append(this);
    
    while (current < todo.Count() && notFound)
        {
        ctrl = todo[current];
        if(ctrl->ControlId() == aCtrlId)
            {
            notFound = EFalse;     
            }
        else
            {
            if(ctrl->IsKindOfControl(ECtrlControlGroup))
                {
                CControlGroup* temp = static_cast<CControlGroup*>(ctrl);
                for (int i = 0; i < temp->NumOfControls(); i++)
                    {
                    todo.Append(temp->At(i));
                    }
                }
            current++;
            }
        }
        
    todo.Close();
    
    return notFound ? NULL : ctrl;
    }    

EXPORT_C void CAknFepCtrlBaseWindowExt::ConstructFromResourceL()
    {    
    if (iResourceId == KInvalidResId)
    	{
        User::Leave(KErrArgument);
    	}

    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC(reader, iResourceId);

    TInt supportskin = reader.ReadInt16();
    
    if (iFirstTimeConstruct&& (supportskin != KNotSupportSkin))
        {
    	UiLayout()->AddControlL(this);
        }
        
    // Read information of shadow
    const TInt32 shadowResId = reader.ReadInt32(); 
    ReadShadowInfoL( shadowResId );
     
    // Read information of background 
    const TInt32 backgroundResId = reader.ReadInt32(); 
    ReadBackgroundInfoL( backgroundResId ); 
    
    // Read resource information to create base window controls
     const TInt count = reader.ReadInt16();
	
	if( iFirstTimeConstruct )
		{
		for( TInt i = 0; i < count && iFirstTimeConstruct; i++ )
	        {
	        const TInt16 id = reader.ReadInt16();
	        const TInt32 image = reader.ReadInt32();
	        CreateBaseWindowControlsL( id, image );
	        }    	
		}
	else
		{
		CControlGroup::ConstructFromResourceL();
		// ReConstruct ICF when resource changed
        CFepLayoutMultiLineIcf* multiIcf = 
		    static_cast<CFepLayoutMultiLineIcf*>( Control(EPeninputWindowCtrlIdMultiLineICF));
		if( multiIcf )
			{
			TResourceReader reader;
			iCoeEnv->CreateResourceReaderLC( reader, multiIcf->ResourceId() );  
			ReadMultiLineIcfInforL( reader, multiIcf );     
			CleanupStack::PopAndDestroy( 1 );				
			}
		}
    CleanupStack::PopAndDestroy(); // reader
    
    iFirstTimeConstruct = EFalse;
    }

TBool CAknFepCtrlBaseWindowExt::CreateCustomControlL( TInt16 /*aControlId*/, TInt32 /*aImageId*/ )
	{
	return EFalse;	
	}
	
void CAknFepCtrlBaseWindowExt::AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl)
	{
	AddControlL(aCtrl);
	}

EXPORT_C void CAknFepCtrlBaseWindowExt::CreateBaseWindowControlsL( TInt16 aControlId, 
                                                                   TInt32 aImageId )
    {
    if (CreateCustomControlL(aControlId, aImageId))
    	{
    	return;	
    	}
    
    switch ( aControlId )
        {
        case EPeninutWindowCtrlIdCloseBtn:
            {
            CAknFepCtrlCommonButton* closeButton = 
            	CAknFepCtrlCommonButton::NewL(UiLayout(), aControlId, 
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive);

            AddCustomControlGroupL( closeButton );

            closeButton->AddEventObserver(this);
            closeButton->SetResourceId( aImageId );
            closeButton->ConstructFromResourceL();
            SetCloseButton(closeButton);
            }
            break;
        case EPeninutWindowCtrlIdInputContextField:
            {
            CFepInputContextField* icf= CFepInputContextField::NewL( TRect(), 
                UiLayout(), aControlId ); 
                
            AddCustomControlGroupL( icf );

            icf->SetFocus(ETrue);
            icf->AddEventObserver( UiLayout() );
            
            TResourceReader reader;
            iCoeEnv->CreateResourceReaderLC( reader, aImageId );  
            ReadIcfInfoL( reader, icf);     
            CleanupStack::PopAndDestroy( 1 );
            }
            break;
        case EPeninputWindowCtrlIdMultiLineICF:
            {
            const CFont* icfFont = NULL;
            TAknTextLineLayout textPaneTextLayout;
    
            textPaneTextLayout = 
            AknLayoutScalable_Avkon::fep_vkb_top_text_pane_t1().LayoutLine();
                        
            icfFont = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, NULL );    
            CFepLayoutMultiLineIcf* multiLineIcf = NULL;

            if ( icfFont )
                {
                multiLineIcf = CFepLayoutMultiLineIcf::NewL( TRect(), 
                    UiLayout(), aControlId, icfFont->HeightInPixels(),
                    icfFont->FontMaxHeight(),icfFont );    
                
                AddCustomControlGroupL( multiLineIcf );

                // Add layout as event observer
                multiLineIcf->AddEventObserver( UiLayout() );
                multiLineIcf->SetResourceId( aImageId );
                multiLineIcf->SetFocus(ETrue);

	            TResourceReader reader;
	            iCoeEnv->CreateResourceReaderLC( reader, aImageId );  
	            ReadMultiLineIcfInforL( reader,multiLineIcf );     
	            CleanupStack::PopAndDestroy( 1 );
                }
            }
            break;
        case EPeninutWindowCtrlIdOptionBtn:
            {
            CAknFepCtrlEventButton* touchInputOptionButton = CAknFepCtrlEventButton::NewL( 
        									  UiLayout(),
        									  aControlId, EPeninputLayoutEventOption, 0, 
        									  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
            
            AddCustomControlGroupL( touchInputOptionButton );

            touchInputOptionButton->AddEventObserver( UiLayout() );
            touchInputOptionButton->SetResourceId( aImageId ); 
            touchInputOptionButton->ConstructFromResourceL();  
            }
            break;
        case EPeninutWindowCtrlIdBackspaceBtn:
            {
            CAknFepCtrlRepeatButton* backspaceButton = CAknFepCtrlRepeatButton::NewL( 
        									  UiLayout(), aControlId, EPeninputLayoutEventBack, EKeyBackspace, 
        									  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
                
            AddCustomControlGroupL( backspaceButton );

            backspaceButton->AddEventObserver( UiLayout() );
            backspaceButton->SetResourceId( aImageId );
            backspaceButton->ConstructFromResourceL();  
            }
            break;
        case EPeninutWindowCtrlIdSpaceBtn:
            {
            CAknFepCtrlEventButton* spaceButton = CAknFepCtrlEventButton::NewL( 
        									  UiLayout(), aControlId, EPeninputLayoutEventSpace, EKeySpace,
        									  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
            AddCustomControlGroupL( spaceButton );
            spaceButton->AddEventObserver( UiLayout() );
            spaceButton->SetResourceId( aImageId );
            spaceButton->ConstructFromResourceL();  
            }
            break;
        case EPeninutWindowCtrlIdEnterBtn:
            {
            CAknFepCtrlEventButton* enterButton = CAknFepCtrlEventButton::NewL( 
        									  UiLayout(), aControlId, EPeninputLayoutEventEnter, EKeyEnter, 
        									  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive  );
            
            AddCustomControlGroupL( enterButton );
            enterButton->AddEventObserver(UiLayout());
            enterButton->SetResourceId( aImageId );
            enterButton->ConstructFromResourceL();  
            }
            break;
        case EPeninutWindowCtrlIdSwitcherPopupWindow:
            {
            CPeninputLayoutInputmodelChoice* modeSwitchChoice = 
                                                CPeninputLayoutInputmodelChoice::NewL( UiLayout(),
                 EPeninutWindowCtrlIdSwitcherPopupWindow, 
                 (TPluginInputMode)(UiLayout()->PenInputType()));
                 modeSwitchChoice->SetListSkinID( KAknsIIDQsnFrList, KAknsIIDQsnFrPopupSub );
            AddCustomControlGroupL( modeSwitchChoice );
            modeSwitchChoice->AddEventObserver( UiLayout() );   
            }   
            break;
        default:
            {
            }
            break;
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlBaseWindowExt::ReadShadowInfoL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepCtrlBaseWindowExt::ReadShadowInfoL( TInt aResId )
    {
    TResourceReader resReader;
    iCoeEnv->CreateResourceReaderLC( resReader, aResId );
     
    TPtrC bmpFileName = resReader.ReadTPtrC();
    TInt32 imgMajorSkinId = resReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();

    for ( TInt i = 0; i < KShadowBmpBr + 1; i++ )
    	{
        const TInt16 bmpId = resReader.ReadInt16();
        const TInt16 bmpMskId = resReader.ReadInt16();
        skinitemid = resReader.ReadInt16();
        
        id.Set(TInt(imgMajorSkinId), skinitemid);
    	
        if (bmpId != KInvalidImg)
        	{
            CFbsBitmap* shadowImg = NULL;
            CFbsBitmap* shadowMaskImg = NULL;

            if (bmpMskId != KInvalidImg)
                {
                AknsUtils::CreateIconL(skininstance, 
                                       id,
                                       shadowImg,
                                       shadowMaskImg,
                                       bmpFileName,
                                       bmpId,
                                       bmpMskId);
                }
            else
                {
                AknsUtils::CreateIconL(skininstance,
                                       id,
                                       shadowImg,
                                       bmpFileName,
                                       bmpId);
                }
            
            UiLayout()->SetShadowBmp(shadowImg, shadowMaskImg, TShadowBitmapIndex(i));
        	}
    	}
    CleanupStack::PopAndDestroy( 1 );
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlBaseWindowExt::ReadBackgroundInfoL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepCtrlBaseWindowExt::ReadBackgroundInfoL( TInt aResId )
    {
    if( aResId == 0 )
    	return;
    
    if( BackgroundBmp() )
    	{
    	CFbsBitmap* bkBmp = BackgroundBmp();
    	delete bkBmp;	
    	SetBackgroundBmp( NULL );
    	}
    if( BkMaskBmp() )
    	{
    	CFbsBitmap* bkMaskBmp = BkMaskBmp();
    	delete bkMaskBmp;
		SetBackgroundMaskBmp( NULL );
    	}    
    
    TResourceReader resReader;
    iCoeEnv->CreateResourceReaderLC( resReader, aResId );
     
    TPtrC bmpFileName = resReader.ReadTPtrC();
    TInt32 imgMajorSkinId = resReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();
    const TInt16 bmpId = resReader.ReadInt16();
    const TInt16 bmpMskId = resReader.ReadInt16();
    skinitemid = resReader.ReadInt16();   
    id.Set(TInt(imgMajorSkinId), skinitemid);
	
    if (bmpId != KInvalidImg)
    	{
    	CFbsBitmap* backgroundImg = NULL;
    	CFbsBitmap* backgroundMaskImg = NULL;
        if (bmpMskId != KInvalidImg)
            {
            AknsUtils::CreateIconL(skininstance, 
                                   id,
                                   backgroundImg,
                                   backgroundMaskImg,
                                   bmpFileName,
                                   bmpId,
                                   bmpMskId);
            }
        else
            {
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   backgroundImg,
                                   bmpFileName,
                                   bmpId);
            }
       
        SetBackgroundBmp( backgroundImg );
        SetBackgroundMaskBmp( backgroundMaskImg );           
    	}
    CleanupStack::PopAndDestroy( 1 );
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlBaseWindowExt::ReadIcfInfoL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepCtrlBaseWindowExt::ReadIcfInfoL(TResourceReader aResReader,
                                           CFepInputContextField* icf )
    {
    TPtrC bmpFileName = aResReader.ReadTPtrC();  
    TInt32 imgMajorSkinId = aResReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();

    const TInt16 icfbgId = aResReader.ReadInt16();
    const TInt16 icfbgmaskId = aResReader.ReadInt16();
    skinitemid = aResReader.ReadInt16();

    id.Set( TInt( imgMajorSkinId ), skinitemid );
    
    if ( icfbgId != KInvalidImg )
    	{
        CFbsBitmap* icfImg = NULL;

        if (icfbgmaskId != KInvalidImg)
            {
            CFbsBitmap* icfmaskImg = NULL;
            
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   icfImg,
                                   icfmaskImg,
                                   bmpFileName,
                                   icfbgId,
                                   icfbgmaskId);
            
            AknIconUtils::SetSize(icfmaskImg, TSize(1,1), EAspectRatioNotPreserved);
            icf->SetBackgroundMaskBitmapL(icfmaskImg);
            }
        else
            {
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   icfImg,
                                   bmpFileName,
                                   icfbgId);
            }
    	
    	AknIconUtils::SetSize(icfImg, TSize(1,1), EAspectRatioNotPreserved);
    	icf->SetBackgroundBitmapL(icfImg);
    	}

    // read text color
    TRgb textcolor;
    TInt32 colorMajorSkinId = aResReader.ReadInt32();
    skinitemid = aResReader.ReadInt16();
    
    id.Set(TInt(colorMajorSkinId), skinitemid);

    const TInt16 textcoloridx = aResReader.ReadInt16();
    
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           textcolor,
                                           id,
                                           textcoloridx);

    if (error == KErrNone)
        {
        icf->SetTextColor( textcolor );
        }
    else
        {
        icf->SetTextColor( TRgb( KDefaultTextColor ) );
        }    
    }    
    
void CAknFepCtrlBaseWindowExt::ReadMultiLineIcfInforL( TResourceReader aResReader ,
													  CFepLayoutMultiLineIcf* icf)
    {
    TPtrC bmpFileName = aResReader.ReadTPtrC();  
    TInt32 imgMajorSkinId = aResReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();

    const TInt16 icfbgId = aResReader.ReadInt16();
    const TInt16 icfbgmaskId = aResReader.ReadInt16();
    skinitemid = aResReader.ReadInt16();

    id.Set( TInt( imgMajorSkinId ), skinitemid );
    
    if ( icfbgId != KInvalidImg )
    	{
        CFbsBitmap* icfImg = NULL;

        if (icfbgmaskId != KInvalidImg)
            {
            CFbsBitmap* icfmaskImg = NULL;
            
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   icfImg,
                                   icfmaskImg,
                                   bmpFileName,
                                   icfbgId,
                                   icfbgmaskId);
            
            AknIconUtils::SetSize(icfmaskImg, TSize(1,1), EAspectRatioNotPreserved);
            icf->SetBackgroundMaskBitmapL(icfmaskImg);
            }
        else
            {
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   icfImg,
                                   bmpFileName,
                                   icfbgId);
            }
    	
    	AknIconUtils::SetSize(icfImg, TSize(1,1), EAspectRatioNotPreserved);
    	icf->SetBackgroundBitmapL(icfImg);
    	}

    // read text color
    TRgb textcolor;
    TInt32 colorMajorSkinId = aResReader.ReadInt32();
    skinitemid = aResReader.ReadInt16();
    
    id.Set(TInt(colorMajorSkinId), skinitemid);

    const TInt16 textcoloridx = aResReader.ReadInt16();
    
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           textcolor,
                                           id,
                                           textcoloridx);

    if (error == KErrNone)
        {
        icf->SetTextColorL( textcolor );
        }
    else
        {
        icf->SetTextColorL( TRgb( KDefaultTextColor ) );
        }    
    }
    
    
    
// End Of File
