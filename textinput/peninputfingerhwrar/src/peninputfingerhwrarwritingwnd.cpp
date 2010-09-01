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
* Description:  CPeninputArabicFingerHwrWritingWnd
*
*/
#include <peninputlayout.h>
#include "peninputfingerhwrarwritingwnd.h"

// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrWritingWnd::NewL
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputArabicFingerHwrWritingWnd* CPeninputArabicFingerHwrWritingWnd::NewL( const TRect& aRect,
                                                 CFepUiLayout* aUiLayout,
                                                 TInt aControlId,
                                                 TBool aFullScreenFlag, 
                                                 TBool aShowGuideLine )
	{
	CPeninputArabicFingerHwrWritingWnd* self = new (ELeave)
	                                    CPeninputArabicFingerHwrWritingWnd(aRect,aUiLayout,aControlId,aFullScreenFlag,aShowGuideLine);
	CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();	
	return self;
	}

// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrWritingWnd::CPeninputArabicFingerHwrWritingWnd
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//	
CPeninputArabicFingerHwrWritingWnd::CPeninputArabicFingerHwrWritingWnd( const TRect& aRect, 
                                        CFepUiLayout* aUiLayout,
                                        TInt aControlId, 
                                        TBool aFullScreenFlag, 
                                        TBool aShowGuideLine )
										:CTransparentHwrWndExt(aRect,aUiLayout,aControlId,aFullScreenFlag,aShowGuideLine)
    {
	SetTactileFeedbackType(ETouchFeedbackSensitiveInput);
	}

// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrWritingWnd::~CPeninputArabicFingerHwrWritingWnd
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//	
CPeninputArabicFingerHwrWritingWnd::~CPeninputArabicFingerHwrWritingWnd()
    {
	delete iIndicator;
	}

// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrWritingWnd::InstallIndicator
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//		
void CPeninputArabicFingerHwrWritingWnd::InstallIndicator(CFepUiBaseCtrl* aIndicator)
    {
	delete iIndicator;
	iIndicator = aIndicator;
	}

// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrWritingWnd::Draw
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CPeninputArabicFingerHwrWritingWnd::Draw()
    {
	CTransparentHwrWndExt::Draw();
	if(iIsShowingIndicator)
	    {
		if(iIndicator)
		    {
			iIndicator->Draw();
			}
		}
	}

// ---------------------------------------------------------------------------
// CPeninputArabicFingerHwrWritingWnd::ShowIndicator
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//	
void CPeninputArabicFingerHwrWritingWnd::ShowIndicator(TBool aShowIndicator)
    {
	iIsShowingIndicator = aShowIndicator;
	Draw();
	}

// End Of File
