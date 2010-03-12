/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for Floating window
*
*/


#include "peninputfloatctrl.h"

const TInt KDefaultRoundSize = 2;
const TInt KDefaultOrdinalPosition = 0;
const TInt KDefaultOrdinalPriority = ECoeWinPriorityAlwaysAtFront + 100;
// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFloatingCtrl::~CPeninputFloatingCtrl()
    {
    }

// ---------------------------------------------------------------------------
// Draw this control
// ---------------------------------------------------------------------------
//
void CPeninputFloatingCtrl::Draw( const TRect& aRect ) const
    {
    CWindowGc& gc = SystemGc();
    TRect rect = Rect();
    gc.Clear( rect ); 

    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.SetBrushColor( KRgbGray );
    gc.SetPenStyle( CGraphicsContext::ESolidPen );
    gc.SetPenColor( KRgbBlack );
    gc.DrawRoundRect( rect, TSize( KDefaultRoundSize, KDefaultRoundSize ) );
    }

// ---------------------------------------------------------------------------
// Show this floating control
// ---------------------------------------------------------------------------
//
void CPeninputFloatingCtrl::Show( const TRect& aRect )
    {
    Show( aRect, KDefaultOrdinalPosition, KDefaultOrdinalPriority );
    }

// ---------------------------------------------------------------------------
// Show this floating control
// ---------------------------------------------------------------------------
//
void CPeninputFloatingCtrl::Show( const TRect& aRect, TInt aOrdinalPos, 
                                  TInt aOrdinalPriority )
    {
    SetRect( aRect );
    DrawableWindow()->SetOrdinalPosition( aOrdinalPos, aOrdinalPriority );
    MakeVisible( ETrue );    
    }
// ---------------------------------------------------------------------------
// Hide this floating control
// ---------------------------------------------------------------------------
//
void CPeninputFloatingCtrl::Hide()
    {
    MakeVisible( EFalse );
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CPeninputFloatingCtrl::BaseConstructL()
    {
    RWindowGroup& wg = CCoeEnv::Static()->RootWin();
    CreateWindowL( wg );
    SetComponentsToInheritVisibility();
    Window().SetRequiredDisplayMode( EColor16MA );
    MakeVisible( EFalse );
    ActivateL();
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFloatingCtrl::CPeninputFloatingCtrl()
    {
    // No implementation required
    }

