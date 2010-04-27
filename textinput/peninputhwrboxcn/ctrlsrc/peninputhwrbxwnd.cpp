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
* Description:  CAknFepHwrBxWnd
*
*/


#include <peninputinputcontextfield.h>
#include <peninputhwrbxcn.rsg>
#include <peninputdropdownlist.h>
#include <peninputdragbutton.h>
#include <peninputrepeatbutton.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <AknIconUtils.h>
#include <coemain.h>
#include <peninputlayoutchoicelist.h>
#include <AknFepGlobalEnums.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputrepeatbutton.h>

#include "peninputhwrdebug.h"
#include "peninputhwrbxwnd.h"
#include "peninputhwrbxlayout.h"
#include "peninputhwrevent.h"
#include "peninputhwrctrlid.h"
#include "peninputhwrbxctrlbar.h"
#include "peninputhwrboxgroup.h"
#include "peninputhwrbxdatastore.h"
#include "aknlayoutscalable_avkon.cdl.h"

const TInt KBoxCount = 2;
const TInt KInvalidImg = -1 ;
const TInt KNotSupportSkin = -1;
const TUint32 KDefaultTextColor = 0x000000;
const TInt32 KInvalidResId = -1;

const TInt KWrittingFactor = 255;  // 0% transparency
const TInt KNormalFactor = 229; // 10% transparency

const TInt KDefaultStrokeEndMarkX = -65536;
const TInt KDefaultStrokeEndMarkY = -65536;

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepHwrBxWnd* CAknFepHwrBxWnd::NewL(CFepUiLayout* aFepUiLayout, 
                                       TInt aControlId,
                                       TBool aLandscapeStyle)
    {
    PRINTF((_L("CAknFepHwrBxWnd::NewL(), aCtrlId = %d\n"), aControlId));
    CAknFepHwrBxWnd* self = NewLC(aFepUiLayout, aControlId, aLandscapeStyle);
    CleanupStack::Pop(self);
    
    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepHwrBxWnd* CAknFepHwrBxWnd::NewLC(CFepUiLayout* aFepUiLayout, 
                                        TInt aControlId,
                                        TBool aLandscapeStyle)
    {
    PRINTF((_L("CAknFepHwrBxWnd::NewLC(), aCtrlId = %d\n"), aControlId));
    CAknFepHwrBxWnd* self = new (ELeave) CAknFepHwrBxWnd(aFepUiLayout, aControlId);
    CleanupStack::PushL(self);
    self->ConstructL(aLandscapeStyle);
    
    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepHwrBxWnd::CAknFepHwrBxWnd( CFepUiLayout* aFepUiLayout, TInt aControlId)
    :CAknFepCtrlBaseWindow(aFepUiLayout,aControlId), 
     iFirstTimeConstruct(ETrue),
     iUnitShadowSizeSet(EFalse),
     iControlPosSet(EFalse)
    {
    CAknFepHwrBxLayout* layout = static_cast<CAknFepHwrBxLayout*>(aFepUiLayout);
    iDataStore = layout->DataStore();
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::ConstructL(TBool aLandscapeStyle)
    {
    PRINTF((_L("CAknFepHwrBxWnd::ConstructL()\n")));
    BaseConstructL();

    AddContextFieldL();

    AddBackspaceBtnL();
    
    AddOptionButtonL();
    
    AddControlBarL();
    
    AddBoxGroupL();

    AddDropdownListL(aLandscapeStyle);

    //add layout as window's observer
    AddEventObserver(UiLayout());
    MoveButton()->AddEventObserver(UiLayout());
    
    //add window as the subscriber
    iDataStore->AddSubscriber(this);
    
    PRINTF((_L("CAknFepHwrBxWnd::ConstructL()......done!\n")));
    }
    
// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxWnd::~CAknFepHwrBxWnd()
    {
    PRINTF((_L("CAknFepHwrBxWnd::~CAknFepHwrBxWnd()\n")));
    }

// ---------------------------------------------------------
// ConstructFromResourceL
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::ConstructFromResourceL(TResourceReader& aReader)
    {
    TInt supportskin = aReader.ReadInt16();

    if (iFirstTimeConstruct && (supportskin != KNotSupportSkin))
        {
    	UiLayout()->AddControlL(this);
        }

    // read shadow and icf resources
    TPtrC bmpFileName = aReader.ReadTPtrC();
    
    TInt32 imgMajorSkinId = aReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();

    for (TInt i = 0; i < KShadowBmpBr + 1; i++)
    	{
        const TInt16 bmpId = aReader.ReadInt16();
        const TInt16 bmpMskId = aReader.ReadInt16();
        skinitemid = aReader.ReadInt16();
        
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
            
            SetShadowBmp(shadowImg, shadowMaskImg, TShadowBitmapIndex(i));
        	}
    	}

    const TInt16 icfbgId = aReader.ReadInt16();
    const TInt16 icfbgmaskId = aReader.ReadInt16();
    skinitemid = aReader.ReadInt16();

    id.Set(TInt(imgMajorSkinId), skinitemid);
    
    if (icfbgId != KInvalidImg)
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
            iContextField->SetBackgroundMaskBitmapL(icfmaskImg);
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
    	iContextField->SetBackgroundBitmapL(icfImg);
    	}
  
    // read text color
    TRgb textcolor;
    TInt32 colorMajorSkinId = aReader.ReadInt32();
    skinitemid = aReader.ReadInt16();
    
    id.Set(TInt(colorMajorSkinId), skinitemid);

    const TInt16 textcoloridx = aReader.ReadInt16();
    
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           textcolor,
                                           id,
                                           textcoloridx);

    if (error == KErrNone)
        {
        iContextField->SetTextColor(textcolor);
        }
    else
        {
        iContextField->SetTextColor(TRgb(KDefaultTextColor));
        }    

    if (iFirstTimeConstruct)
        {
        const TInt32 moveResId = aReader.ReadInt32();
        const TInt32 closeResId = aReader.ReadInt32();
        const TInt32 backResId = aReader.ReadInt32();
        const TInt32 optionResId = aReader.ReadInt32();
        const TInt32 ctrlbarResId = aReader.ReadInt32();
        const TInt32 boxgrpResId = aReader.ReadInt32();
        const TInt32 bgImageResId = aReader.ReadInt32();
        
        MoveButton()->SetResourceId(moveResId);
        MoveButton()->ConstructFromResourceL();

        CloseButton()->SetResourceId(closeResId);
        CloseButton()->ConstructFromResourceL();
        
        iBackspace->SetResourceId(backResId);
        iBackspace->ConstructFromResourceL();
        
        iOptionButton->SetResourceId(optionResId);
        iOptionButton->ConstructFromResourceL();
    
        CCoeEnv* coeEnv = CCoeEnv::Static();
        TResourceReader ctrlbarreader;

        coeEnv->CreateResourceReaderLC(ctrlbarreader, ctrlbarResId);
        iControlBar->ConstructFromResourceL(ctrlbarreader);
        CleanupStack::PopAndDestroy();  // ctrlbarreader
        
        iBoxGroup->SetResourceId(boxgrpResId);
        iBoxGroup->ConstructFromResourceL();
        
        ReadBackgroundInfoL( bgImageResId );
        }
    else
    	{
    	CControlGroup::ConstructFromResourceL();
    	}    

    iFirstTimeConstruct = EFalse;
    
    TInt newValue = iDataStore->GetPenTrailColor();
    iDataStore->SetBoxPenColor(TRgb(newValue));    
    }
    
// ---------------------------------------------------------
// ConstructFromResourceL
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::ConstructFromResourceL()
	{
    if (iResourceId == KInvalidResId)
    	{
        User::Leave(KErrArgument);
    	}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    TInt supportskin = reader.ReadInt16();
    
    if (iFirstTimeConstruct && (supportskin != KNotSupportSkin))
        {
    	UiLayout()->AddControlL(this);
        }
    
    // read shadow and icf resources
    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = UiLayout()->SkinInstance();

    for (TInt i = 0; i < KShadowBmpBr + 1; i++)
    	{
        const TInt16 bmpId = reader.ReadInt16();
        const TInt16 bmpMskId = reader.ReadInt16();
        skinitemid = reader.ReadInt16();
        
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
            
            SetShadowBmp(shadowImg, shadowMaskImg, TShadowBitmapIndex(i));
        	}
    	}

    const TInt16 icfbgId = reader.ReadInt16();
    const TInt16 icfbgmaskId = reader.ReadInt16();
    skinitemid = reader.ReadInt16();

    id.Set(TInt(imgMajorSkinId), skinitemid);
    
    if (icfbgId != KInvalidImg)
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
            iContextField->SetBackgroundMaskBitmapL(icfmaskImg);
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
    	iContextField->SetBackgroundBitmapL(icfImg);
    	}
  
    // read text color
    TRgb textcolor;
    TInt32 colorMajorSkinId = reader.ReadInt32();
    skinitemid = reader.ReadInt16();
    
    id.Set(TInt(colorMajorSkinId), skinitemid);

    const TInt16 textcoloridx = reader.ReadInt16();
    
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           textcolor,
                                           id,
                                           textcoloridx);

    if (error == KErrNone)
        {
        iContextField->SetTextColor(textcolor);
        }
    else
        {
        iContextField->SetTextColor(TRgb(KDefaultTextColor));
        }    

    if (iFirstTimeConstruct)
        {
        const TInt32 moveResId = reader.ReadInt32();
        const TInt32 closeResId = reader.ReadInt32();
        const TInt32 backResId = reader.ReadInt32();
        const TInt32 optionResId = reader.ReadInt32();
        const TInt32 ctrlbarResId = reader.ReadInt32();
        const TInt32 boxgrpResId = reader.ReadInt32();
        const TInt32 bgImageResId = reader.ReadInt32();
        
        MoveButton()->SetResourceId(moveResId);
        MoveButton()->ConstructFromResourceL(); 
        
        CloseButton()->SetResourceId(closeResId);
        CloseButton()->ConstructFromResourceL();
        
        iBackspace->SetResourceId(backResId);
        iBackspace->ConstructFromResourceL();
        
        iOptionButton->SetResourceId(optionResId);
        iOptionButton->ConstructFromResourceL();

        CCoeEnv* coeEnv = CCoeEnv::Static();
        TResourceReader ctrlbarreader;

        coeEnv->CreateResourceReaderLC(ctrlbarreader, ctrlbarResId);
        iControlBar->ConstructFromResourceL(ctrlbarreader);
        CleanupStack::PopAndDestroy();  // ctrlbarreader

        iBoxGroup->SetResourceId(boxgrpResId);
        iBoxGroup->ConstructFromResourceL();
        
        ReadBackgroundInfoL( bgImageResId );
        }
    else
        {
        CControlGroup::ConstructFromResourceL();
        }    

    CleanupStack::PopAndDestroy(); // reader

    iFirstTimeConstruct = EFalse;
    TInt newValue = iDataStore->GetPenTrailColor();
    iDataStore->SetBoxPenColor(TRgb(newValue));  
	}

// ---------------------------------------------------------
// CAknFepHwrBxWnd::ReadUnitSize
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::ReadLafInfo(TBool aLandscapeStyle)
	{
	if (iDataStore->SizeChanging() || (!iUnitShadowSizeSet))
		{
        // if size is changing, or unit size, shadow size have not been set yet
        TAknWindowLineLayout appWnd;
        TAknWindowLineLayout mainPane;
        TAknWindowLineLayout wndLayout;
        TAknWindowLineLayout unitLayout;
        TAknWindowLineLayout shadowPane;
        TAknWindowLineLayout shadowTl;
        TAknWindowLineLayout shadowBr;
        TAknLayoutRect mainPaneRect;
        TAknLayoutRect wndRect;
        TAknLayoutRect unitRect;
        TAknLayoutRect shadowRect;
        TAknLayoutRect shadowTlRect;
        TAknLayoutRect shadowBrRect;

        // Get hwr window layout
        appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
        
        if (aLandscapeStyle)
        	{
    	    // landscape
    	    mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
            wndLayout = AknLayoutScalable_Avkon::popup_fep_hwr_window(2).LayoutLine();
        	}
        else
        	{
            // portrait	
            mainPane = AknLayoutScalable_Avkon::main_pane(1).LayoutLine();
            wndLayout = AknLayoutScalable_Avkon::popup_fep_hwr_window(0).LayoutLine();
        	}	

        mainPaneRect.LayoutRect(appWnd.Rect(), mainPane);
        wndRect.LayoutRect(mainPaneRect.Rect(), wndLayout);
    
        unitLayout = AknLayoutScalable_Avkon::fep_hwr_aid_pane(2).LayoutLine();
        unitRect.LayoutRect(wndRect.Rect(), unitLayout);
    
        TRect hwrWndRect = TRect(0, 
                                 0,
                                 wndRect.Rect().Width(),
                                 wndRect.Rect().Height());
                             
        iUnitWidth = unitRect.Rect().Width();
        iUnitHeight = unitRect.Rect().Height();
        
      //  iUnitWidth = 34;
      //  iUnitHeight = 34;        

        shadowPane = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane(1).LayoutLine();
        shadowRect.LayoutRect(hwrWndRect, shadowPane);
    
        shadowTl = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g1().LayoutLine();
        shadowTlRect.LayoutRect(shadowRect.Rect(), shadowTl);
    
        shadowBr = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g8().LayoutLine();
        shadowBrRect.LayoutRect(shadowRect.Rect(), shadowBr);
    
        iShadowTl = shadowTlRect.Rect().Size();
        iShadowBr = shadowBrRect.Rect().Size();
        
        iUnitShadowSizeSet = ETrue;
		}
 	}
 	
// ---------------------------------------------------------
// Get top left corner shadow size
// ---------------------------------------------------------
//
TSize CAknFepHwrBxWnd::ShadowTlSize()
	{
	return iShadowTl;
	}

// ---------------------------------------------------------
// Get bottom right corner shadow size
// ---------------------------------------------------------
//
TSize CAknFepHwrBxWnd::ShadowBrSize()
	{
	return iShadowBr;
	}
	
// ---------------------------------------------------------
// Set setting button enable
// ---------------------------------------------------------
//     
void CAknFepHwrBxWnd::SetEnableSettingBtn(TBool aEnable)
    {
    if ( aEnable )    
        {
        iOptionButton->SetDimmed(EFalse);
        }
    else
        {
        iOptionButton->SetDimmed(ETrue);
        iOptionButton->SetActive(EFalse);
        } 
    }
    
// ---------------------------------------------------------
// Resize
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::SizeChanged(TBool aLandscapeStyle)
    {
    ReadLafInfo(aLandscapeStyle);
 
    if (iDataStore->SizeChanging() || (!iControlPosSet))
    	{
    	// if size changing, or sub controls 
    	// size and position have not been set
        
        TPoint startPos = TPoint(iShadowTl.iWidth, iShadowTl.iHeight);
        TRect hwrWnd = TRect(startPos,
                             TPoint(startPos.iX + 10*iUnitWidth,
                                    startPos.iY + 7*iUnitHeight));
        SetRect(hwrWnd);

        // title bar
        startPos = hwrWnd.iTl;
        TRect titleBar = TRect(startPos,
                               TPoint(startPos.iX + iUnitWidth*10,
                                      startPos.iY + iUnitHeight));
        TitleBar()->SetRect(titleBar);

        // close button
        startPos = titleBar.iTl;
        TRect closeRect = TRect(startPos,
                                TPoint(startPos.iX + iUnitWidth,
                                       startPos.iY + iUnitHeight));
        TRect closeInnerRect;
        AknPenImageUtils::CalculateGraphicRect(closeRect,closeInnerRect);
        CloseButton()->SizeChanged(closeRect, closeInnerRect, ETrue);
    
        // header pane
        startPos = TPoint(closeRect.iBr.iX, closeRect.iTl.iY);
        TRect headerPane = TRect(startPos,
                                 TPoint(startPos.iX + iUnitWidth*8,
                                        startPos.iY + iUnitHeight));
        HeaderPane()->SetRect(headerPane);

        // icf
        startPos = headerPane.iTl;
        TRect icfRect = TRect(startPos,
                              TPoint(startPos.iX + iUnitWidth*7,
                                     startPos.iY + iUnitHeight));

        TAknTextLineLayout textPaneTextLayout;
        textPaneTextLayout = AknLayoutScalable_Avkon::fep_hwr_top_text_pane_t1().LayoutLine();
        iContextField->SetFont(AknLayoutUtils::FontFromId(textPaneTextLayout.iFont, NULL));
        iContextField->SetRect(icfRect);

        // Backspace button
        startPos = TPoint(icfRect.iBr.iX, icfRect.iTl.iY);
        TRect backspaceRect = TRect(startPos,
                                 TPoint(startPos.iX + iUnitWidth,
                                        startPos.iY + iUnitHeight));
        TRect backspaceInnerRect;
        AknPenImageUtils::CalculateGraphicRect(backspaceRect, backspaceInnerRect);
        iBackspace->SizeChanged(backspaceRect, backspaceInnerRect, ETrue);
        
        // move button
        startPos = TPoint(headerPane.iBr.iX, headerPane.iTl.iY);
        TRect moveRect = TRect(startPos, 
                               TPoint(startPos.iX + iUnitWidth,
                                      startPos.iY + iUnitHeight)); 
        TRect moveInnerRect;
        AknPenImageUtils::CalculateGraphicRect(moveRect, moveInnerRect);
        MoveButton()->SizeChanged(moveRect, moveInnerRect,ETrue);
    
        // client pane
        startPos = TPoint(titleBar.iTl.iX, titleBar.iBr.iY);
        TRect clientPane = TRect(startPos,
                                 TPoint(startPos.iX + iUnitWidth*10,
                                        startPos.iY + iUnitHeight*6));
        ClientPane()->SetRect(clientPane);
    
        // option button
        startPos = clientPane.iTl;
        TRect optionInnerRect;
        TRect optionRect = TRect(startPos,
                                 TPoint(startPos.iX + iUnitWidth,
                                        startPos.iY + iUnitHeight));
        AknPenImageUtils::CalculateGraphicRect(optionRect, optionInnerRect); 
        iOptionButton->SizeChanged(optionRect, optionInnerRect, ETrue);
        
        // drop down list
        startPos = TPoint(optionRect.iBr.iX, optionRect.iTl.iY);
        TAknTextLineLayout candTextLayout;
        candTextLayout = AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();
        iDropdownList->SetFont(AknLayoutUtils::FontFromId(candTextLayout.iFont, NULL));
    
        iDropdownList->SizeChanged(iUnitWidth,
                                   iUnitHeight,
                                   iUnitWidth,
                                   startPos,
                                   8,
                                   4);

        // box group
        startPos = TPoint(clientPane.iTl.iX, clientPane.iTl.iY + iUnitHeight);
        TRect boxGrp = TRect(startPos,
                             TPoint(startPos.iX + iUnitWidth*9,
                                    startPos.iY + iUnitHeight*5));
        iBoxGroup->SizeChanged(boxGrp);
    
        // control bar
        startPos = TPoint( boxGrp.iBr.iX, boxGrp.iTl.iY );
        TRect ctrlBar = TRect(startPos,
                              TPoint(startPos.iX + iUnitWidth,
                                     startPos.iY + iUnitHeight*5));
        TRect topgridRect = TRect(startPos,
                                  TPoint(startPos.iX + iUnitWidth,
                                         startPos.iY + iUnitHeight));
        TRAP_IGNORE(iControlBar->SizeChangedL(ctrlBar, topgridRect));
    
        // shadow
        SetShadowRect(TRect(0,
                            0,
                            hwrWnd.iBr.iX + iShadowBr.iWidth,
                            hwrWnd.iBr.iY + iShadowBr.iHeight));
    	}
    
    iControlPosSet = ETrue;	
    }

// ---------------------------------------------------------
// Just broadcast the close event
// ---------------------------------------------------------
//
TBool CAknFepHwrBxWnd::DoClose()
    {
    PRINTF((_L("CAknFepHwrBxWnd::DoClose()\n")));
    
    ReportEvent(EHwrEventWindowClose);

    return EFalse;
    }

// ---------------------------------------------------------
// Handle the property change
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::SetPropertyL(
                              const MAknFepHwrPropertySubscriber::TAknFepHwrProperty aPropertyName,
                              const TDesC& aPropertyValue)
    {
    switch (aPropertyName)
        {
        case MAknFepHwrPropertySubscriber::EAknFepHwrPropertyCandidates:
            {
            const TInt candiCount = iDataStore->CandidateCount();
            RArray<TInt> secondaryList;
            
            for (TInt i = 0; i < candiCount; i++)
                {
                if (i > candiCount - iDataStore->PermittedRangesCount()) 
                    {
                    //cell index start from 1
                    secondaryList.Append(i);
                    }
                } 
            iDropdownList->SetSecondaryList(secondaryList);  

            secondaryList.Close();
            
            RPointerArray<HBufC>* candidates = (RPointerArray<HBufC>*)aPropertyValue.Ptr();
            iDropdownList->SetCandidatesL(*candidates, CFepCtrlDropdownList::EListNoExpandable);
            break;
            }
        case MAknFepHwrPropertySubscriber::EAknFepHwrPropertyPredictives:
            {
            RPointerArray<HBufC>* candidates = (RPointerArray<HBufC>*)aPropertyValue.Ptr();
            iDropdownList->SetCandidatesL(*candidates, CFepCtrlDropdownList::EListExpandable);
            break;
            }
       	case MAknFepHwrPropertySubscriber::EAknFepHwrPropertyAddPredictives:
       	    {
            RPointerArray<HBufC>* candidates = (RPointerArray<HBufC>*)aPropertyValue.Ptr();
            iDropdownList->AddCandidatesL(*candidates, CFepCtrlDropdownList::EListExpandable);
       	    }
       	    break;
        case MAknFepHwrPropertySubscriber::EAknFepHwrPropertySizeChanged:
            {
            static_cast<CAknFepHwrBxLayout*>(UiLayout())->SetLayoutPositionL();
            break;
            }
        default:
            break;
        }
    }
    
// ---------------------------------------------------------
// Stroke end mark
// ---------------------------------------------------------
//
TPoint CAknFepHwrBxWnd::StrokeEndMark()
    {
    return TPoint(KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY);
    }

// ---------------------------------------------------------
// Send input context field text
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::SetEditorTextL(const TFepInputContextFieldData& aData)
    {
    if(iContextField)
        {
        iContextField->SetTextL(aData);
        }
    }

// ---------------------------------------------------------
// Highlight a dropdownlist cell
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::HighlightCell(TInt aIndex)
    {
    iDropdownList->SetHighlightCell(aIndex);
    }
    
// ---------------------------------------------------------
// Add input context field to window
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::AddContextFieldL()
    {
    CControlGroup* headerPane = NULL;
    
    //get reference CControlGroup of headerPane
    headerPane = HeaderPane();
    User::LeaveIfNull(headerPane);

    //input context field
    iContextField = CFepInputContextField::NewL(TRect(), UiLayout(), EHwrCtrlIdInputContextField);
    
    TAknTextLineLayout textPaneTextLayout;

    textPaneTextLayout = AknLayoutScalable_Avkon::fep_hwr_top_text_pane_t1().LayoutLine();
    
    iContextField->SetFont(AknLayoutUtils::FontFromId(textPaneTextLayout.iFont, NULL));

    //transfer to headerPane
    headerPane->AddControlL(iContextField);
    
    //add layout as event observer
    iContextField->AddEventObserver(UiLayout());
    
    iContextField->SetFocus();
    }
    
// ---------------------------------------------------------
// Add option button to window
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::AddOptionButtonL()
    {
    //option button
    iOptionButton = CAknFepCtrlCommonButton::NewL(UiLayout(), 
    						EHwrCtrlIdTouchInputOption,
    						KAknsIIDQsnFrFunctionButtonNormal,
						    KAknsIIDQsnFrFunctionButtonPressed,
						    KAknsIIDQsnFrFunctionButtonInactive);
 
    iInputModeSwitch = CPeninputLayoutInputmodelChoice::NewL(
                                     UiLayout(),
                                     EHwrCtrlIdHwrModeSwitch,
                                     EPluginInputModeHwr );
    iInputModeSwitch->SetListSkinID( KAknsIIDQsnFrList, KAknsIIDQsnFrPopupSub );
    AddControlL( iInputModeSwitch );
    iInputModeSwitch->AddEventObserver( UiLayout() );   
    
    ClientPane()->AddControlL(iOptionButton);
    
    //add layout as event observer
    iOptionButton->AddEventObserver(UiLayout());
    }

// ---------------------------------------------------------
// Pup up pen input mode switcher
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::PopUpPenInputModeSwitcher()
    {
    CFepUiBaseCtrl* vkbControl = iControlBar->ControlById(EHwrCtrlIdVkb);
   	TRAP_IGNORE( iInputModeSwitch->PopUpSwitchListL( vkbControl->Rect() ) );
    }
    
// ---------------------------------------------------------
// Add controlbar to window
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::AddControlBarL()
    {
    CControlGroup* clientPane = NULL;

    clientPane = ClientPane();
    User::LeaveIfNull(clientPane);

    //control bar
    iControlBar = CAknFepHwrBxCtrlBar::NewL(UiLayout(), EHwrCtrlIdControlBar);
    
    //transfer to clientPane
    clientPane->AddControlL(iControlBar);

    //add control bar as property subscriber
    iDataStore->AddSubscriber(iControlBar);
    
    //no need to add layout as control bar's observer because its sub control 
    //will send event directly to layout
    }

// ---------------------------------------------------------
// Add box group to window
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::AddBoxGroupL()
    {
    CControlGroup* clientPane = NULL;

    clientPane = ClientPane();
    User::LeaveIfNull(clientPane);

    iBoxGroup = CAknFepHwrBoxGroup::NewL(UiLayout(), EHwrCtrlIdHwrBoxGroup, KBoxCount);
    iBoxGroup->SetTransparency(KWrittingFactor, KNormalFactor);
        
    //transfer to clientPane
    clientPane->AddControlL(iBoxGroup);

    //add layout as event observer
    iBoxGroup->AddEventObserver(UiLayout());

    //add BoxGroup as property subscriber
    iDataStore->AddSubscriber(iBoxGroup);
    iDataStore->SetStrokeEndMarkControl(iBoxGroup);
    }
    
// ---------------------------------------------------------
// Add dropdown list to window
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::AddDropdownListL(TBool aLandscapeStyle)
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader,R_AKN_FEP_DROP_DOWN_LIST);
    
    iSecondRangeColor = TRgb(0xFF0000);

    ReadLafInfo(aLandscapeStyle);
    TAknTextLineLayout candTextLayout = 
                      AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();

    TInt textMargin = (iUnitHeight - 
                      AknLayoutUtils::FontFromId(candTextLayout.iFont, NULL)->HeightInPixels())/2;

    TDropdownListDrawInfo candiDrawInfo(KAknsIIDQsnFrFunctionButtonInactive, 
    									KAknsIIDQsnFrFunctionButtonNormal,
    								    KAknsIIDQsnFrFunctionButtonPressed,
    						            KAknsIIDQsnFrItutButtonCandiSideL,
    						            KAknsIIDQsnFrItutButtonCandiMiddle,
    						            KAknsIIDQsnFrItutButtonCandiSideR,
    						            KAknsIIDQsnFrItutButtonCandiPressedSideL,
                                        KAknsIIDQsnFrItutButtonCandiPressedMiddle,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideR,
    						            TRgb(194, 221, 242),
    						            ETrue);

    iDropdownList = CFepCtrlDropdownList::NewL(TPoint(Rect().iTl.iX, Rect().iTl.iY + iUnitHeight),
                                               UiLayout(),
                                               255,
                                               reader,                                            
                                               iUnitWidth,
                                               iUnitHeight,
                                               iUnitWidth,
                                               9,
                                               4,
                                               AknLayoutUtils::FontFromId(candTextLayout.iFont, 
                                                                          NULL),
                                               NULL,
                                               CGraphicsContext::ECenter,
                                               textMargin,
                                               KRgbBlack,       // border color 
                                               TRgb(0xeeeeee),  // content bg color
                                               KRgbWhite,       // navi bg color
                                               AKN_LAF_COLOR_STATIC(candTextLayout.iC),
                                               KRgbRed);

    ClientPane()->AddControlL(iDropdownList);
    iDropdownList->SetResourceId(R_AKN_FEP_DROP_DOWN_LIST);
    iDropdownList->SetEventIdForCandidateSelected(EHwrEventCandidateSelected);
    iDropdownList->SetEventIdForNextPageCandidate(EHwrEventGetNextCandidatePage);
    iDropdownList->SetEventIdForPreviousPageCandidate(EHwrEventGetPreviousCandidatePage);
    iDropdownList->SetEventIdForCandidateExisted(EHwrEventCandidateExisted);
    iDropdownList->AddEventObserver(UiLayout());
    iDropdownList->SetDropdownListImgID(candiDrawInfo);
    CleanupStack::PopAndDestroy(); // reader
    }    

// ---------------------------------------------------------
// Cancel writing
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::CancelWriting()
    {
    if (iBoxGroup)
        {
        iBoxGroup->CancelWriting();
        }
    }

// ---------------------------------------------------------
// Cancel repeat
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::CancelRepeat()
    {
    CAknFepCtrlRepeatButton* btn = static_cast<CAknFepCtrlRepeatButton*> 
                                   (Control(EHwrCtrlIdBackspace));
    if (btn)
        {
        btn->CancelRepeat();
        }
    }
    

// ---------------------------------------------------------
// Draw window and shadow
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::Draw()
	{
	CAknFepCtrlBaseWindow::Draw();
	
	// draw shadow
	DrawShadow();

	UpdateArea(Rect(), EFalse);
	}

// ---------------------------------------------------------
// Draw window and shadow
// ---------------------------------------------------------
//
void CAknFepHwrBxWnd::DimInputContextField(TBool aFlag)
	{
	if (iContextField)
	    {
	    iContextField->SetDimmed(aFlag);
	    }
	}
	
// ---------------------------------------------------------
// Get drop down list
// ---------------------------------------------------------
//
CFepCtrlDropdownList* CAknFepHwrBxWnd::DropdownList()
    {
	return iDropdownList;
    }

void CAknFepHwrBxWnd::AddBackspaceBtnL()
    {
    CControlGroup* headerPane = NULL;
    
    //get reference CControlGroup of headerPane
    headerPane = HeaderPane();
    User::LeaveIfNull(headerPane);
    
    iBackspace = CAknFepCtrlRepeatButton::NewL( UiLayout(), EHwrCtrlIdBackspace,
                            EHwrEventKeyBack, EKeyBackspace,												
                            KAknsIIDQsnFrFunctionButtonNormal,
                            KAknsIIDQsnFrFunctionButtonPressed,
                            KAknsIIDQsnFrFunctionButtonInactive);
    headerPane->AddControlL(iBackspace);
    iBackspace->AddEventObserver( UiLayout() );
    }

// ---------------------------------------------------------------------------
// CAknFepHwrBxWnd::ReadBackgroundInfoL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepHwrBxWnd::ReadBackgroundInfoL( TInt aResId )
    {
    if( aResId == 0 )
    	return;
    
    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, aResId );
     
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

void CAknFepHwrBxWnd::SetTextAlignmentL( TInt aAlignment )	
	{
	if ( iContextField )
		{
		iContextField->SetTextAlignmentL( aAlignment );
		}
	}
	
//  End Of File

