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
* Description:  Implementation of HWR ctonrol bar
*
*/


// system includes
#include <barsread.h>
#include <peninputhwrbxcn.rsg>
#include <aknfeppeninputenums.h>

#include <peninputlayoutbutton.h>
#include <peninputrangebar.h>
//#include <peninputmultimodebutton.h>
#include <peninputeventbutton.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <peninputpluginutils.h>

// user includes
#include "peninputhwrbxlayout.h"
#include "peninputhwrbxctrlbar.h"
#include "peninputhwrctrlid.h"
#include "peninputhwrevent.h"
#include "aknlayoutscalable_avkon.cdl.h"



 




const TInt KDefaultStrokeEndMarkX = -65536;
const TInt KDefaultStrokeEndMarkY = -65536;

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//    
CAknFepHwrBxCtrlBar* CAknFepHwrBxCtrlBar::NewL(CFepUiLayout* aUiLayout, TInt aControlId)
    {
    CAknFepHwrBxCtrlBar* self = NewLC(aUiLayout,aControlId);
    CleanupStack::Pop(self);
    
    return self;
    }
    
// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//    
CAknFepHwrBxCtrlBar* CAknFepHwrBxCtrlBar::NewLC(CFepUiLayout* aUiLayout, TInt aControlId)
    {
    CAknFepHwrBxCtrlBar* self = new (ELeave) CAknFepHwrBxCtrlBar(aUiLayout, aControlId);
    CleanupStack::PushL(self);
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//    
CAknFepHwrBxCtrlBar::CAknFepHwrBxCtrlBar(CFepUiLayout* aUiLayout,TInt aControlId)
    :CControlGroup(aUiLayout,aControlId, EFalse)
    {
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor
// ---------------------------------------------------------------------------
//    
void CAknFepHwrBxCtrlBar::ConstructL()
    {
    CControlGroup::ConstructL();

    CAknFepHwrBxLayout* layout = static_cast<CAknFepHwrBxLayout*>(UiLayout());
    
    ASSERT(layout != NULL);
    
    iRangeBar = CAknFepCtrlRangeBar::NewL(layout, EHwrCtrlIdControlBar, 
                                          CAknFepCtrlRangeBar::EBtnGroupVertical);
    iRangeBar->SetActionStyle( CAknFepCtrlRangeBar::ERangeBarActionStyleHighLight );
    iRangeBar->SetEventIdForRange(EHwrEventSetRange);
    iRangeBar->SetEventIdForCase(EHwrEventSetCase);
    AddControlL(iRangeBar);
    iRangeBar->AddEventObserver(layout);
    
    iVkb = CAknFepCtrlCommonButton::NewL( layout, 
    									  EHwrCtrlIdVkb,
    									  KAknsIIDQsnFrFunctionButtonNormal,
    									  KAknsIIDQsnFrFunctionButtonPressed,
    									  KAknsIIDQsnFrFunctionButtonInactive );      
    AddControlL(iVkb);
    iVkb->AddEventObserver(layout);

    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
CAknFepHwrBxCtrlBar::~CAknFepHwrBxCtrlBar()
    {
    }

// ---------------------------------------------------------
// ConstructFromResourceL
// ---------------------------------------------------------
//
void CAknFepHwrBxCtrlBar::ConstructFromResourceL(TResourceReader& aReader)
    {
    const TInt32 rangebarResId = aReader.ReadInt32();
    const TInt32 vkbResId = aReader.ReadInt32();

    CCoeEnv* coeEnv = CCoeEnv::Static();
    TResourceReader reader;
    coeEnv->CreateResourceReaderLC(reader, rangebarResId);
    iRangeBar->ConstructFromResourceL(reader);
    CleanupStack::PopAndDestroy(); // reader
    
    iVkb->SetResourceId(vkbResId);
    iVkb->ConstructFromResourceL();
    }

// ---------------------------------------------------------
// ConstructFromResourceL
// ---------------------------------------------------------
//
void CAknFepHwrBxCtrlBar::ConstructFromResourceL()
    {
    CControlGroup::ConstructFromResourceL();
    }
// ---------------------------------------------------------------------------
// Handles the SizeChanged event
// ---------------------------------------------------------------------------
//    
void CAknFepHwrBxCtrlBar::SizeChangedL(const TRect& aRect, const TRect& aGridRect)
    {
    SetRect(aRect);

    // range bar  
    TPoint startPos = TPoint(aGridRect.iTl);
    // range bar grid
    TRect rangeBarGridRect = TRect(startPos,
                               TPoint(startPos.iX + aGridRect.Width(),
                                      startPos.iY + aGridRect.Height()));
    iRangeBar->SizeChanged(rangeBarGridRect);
    // whole range bar
    TRect rangeBarRect = TRect(startPos,
                              TPoint(startPos.iX + aGridRect.Width(),
                                     startPos.iY + aGridRect.Height()*iRangeBar->NumOfControls()));
    
    iRangeBar->RefreshUI();
    // vkb
    startPos = TPoint(rangeBarRect.iTl.iX, rangeBarRect.iBr.iY);
    
    TRect vkbRect = TRect(startPos,
                          TPoint(startPos.iX + aGridRect.Width(),
                                 startPos.iY + aGridRect.Height()));
    TRect vkbInnerRect;
    AknPenImageUtils::CalculateGraphicRect(vkbRect, vkbInnerRect);                            
    iVkb->SizeChanged(vkbRect, vkbInnerRect, ETrue);
    }

// ---------------------------------------------------------------------------
// Set active input range
// ---------------------------------------------------------------------------
//    
TBool CAknFepHwrBxCtrlBar::SetRange(const TInt aRange)
    {
    if (iRangeBar)
        {
        return iRangeBar->SetRange(aRange);
        }
        
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// Set permitted input ranges
// ---------------------------------------------------------------------------
//    
void CAknFepHwrBxCtrlBar::SetPermittedRanges(const TInt aPermitted)
    {
    if (iRangeBar)
        {
        iRangeBar->SetPermittedRanges(aPermitted);
        }
    }

// ---------------------------------------------------------------------------
// Set case of given range
// ---------------------------------------------------------------------------
//    
void CAknFepHwrBxCtrlBar::SetCaseL(const TInt aCase)
    {
    if (iRangeBar)
        {
        iRangeBar->SetCaseL(ERangeEnglish, aCase);
        }
    }

// ---------------------------------------------------------------------------
// Set case of given range
// ---------------------------------------------------------------------------
//    
void CAknFepHwrBxCtrlBar::SetPermittedCasesL(TInt aPermittedCases)
    {
    if (iRangeBar)
        {
        //
        iRangeBar->SetPermittedCase(ERangeEnglish, ECaseUpper, 
                            (aPermittedCases & ECaseUpper) != 0 );
        iRangeBar->SetPermittedCase(ERangeEnglish, ECaseLower, 
                            (aPermittedCases & ECaseLower) != 0 );
        iRangeBar->SetPermittedCase(ERangeEnglish, ECaseText, 
                            (aPermittedCases & ECaseText) != 0 );
        }
    }

// ---------------------------------------------------------------------------
// Set property
// ---------------------------------------------------------------------------
//    
void CAknFepHwrBxCtrlBar::SetPropertyL(
                              const MAknFepHwrPropertySubscriber::TAknFepHwrProperty aPropertyName,
                              const TDesC& aPropertyValue)
    {
    const TInt value = *(TInt*)aPropertyValue.Ptr();
    
    switch (aPropertyName) 
        {
        case MAknFepHwrPropertySubscriber::EAknFepHwrPropertyPermittedCases:
            SetPermittedCasesL(value);
            break;
        case MAknFepHwrPropertySubscriber::EAknFepHwrPropertyCase:
            SetCaseL(value);
            break;
        case MAknFepHwrPropertySubscriber::EAknFepHwrPropertyRange:
            SetRange(value);
            break;
        case MAknFepHwrPropertySubscriber::EAknFepHwrPropertyPermittedRange:
            SetPermittedRanges(value);
            break;
        default:
            break;
        }
    }
    
// ---------------------------------------------------------------------------
// Stroke End Mark
// ---------------------------------------------------------------------------
//    
TPoint CAknFepHwrBxCtrlBar::StrokeEndMark()
    {
    return TPoint(KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY);
    }
    
// End Of File
