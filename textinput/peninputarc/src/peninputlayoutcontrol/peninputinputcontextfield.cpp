/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for CFepInputContextField
*
*/


// INCLUDE FILES
#include "peninputlayout.h"
#include "peninputinputcontextfield.h"


// CONSTANTS


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CFepInputContextField::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CFepInputContextField* CFepInputContextField::NewL(TRect aRect,
                                      CFepUiLayout* aUiLayout,TInt aControlId)
    {
    CFepInputContextField* self = new (ELeave) CFepInputContextField(aRect,
                                                        aUiLayout,aControlId);
    
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// ---------------------------------------------------------------------------
// CFepInputContextField::CFepInputContextField
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
EXPORT_C CFepInputContextField::CFepInputContextField(TRect aRect, CFepUiLayout* aUiLayout,
                                                      TInt aControlId):
            CFepLayoutEditAreaBase(aRect,aUiLayout,aControlId)
    {
    SetControlType(ECtrlInputContextArea);
    }

// ---------------------------------------------------------------------------
// CFepInputContextField::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
EXPORT_C void CFepInputContextField::ConstructL()
    {
    BaseConstructL();
    }

// Destructor
EXPORT_C CFepInputContextField::~CFepInputContextField()
    {
    }

// ---------------------------------------------------------------------------
// CFepInputContextField::SetTextL
// Gets the text from the current control and puts it into
// the edit area.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepInputContextField::SetTextL(const TFepInputContextFieldData& aData)
    {
    CFepLayoutEditAreaBase::SetTextL( aData );
    }


// ---------------------------------------------------------------------------
// Handle pointer up event
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepInputContextField::HandlePointerUpEventL(const TPoint& aPt)
    {
   
    if ( !CFepLayoutEditAreaBase::HandlePointerUpEventL(aPt) )
        {
        return NULL;
        }

    TPtrC eventData((const TUint16 *)&iSelectedCompositionText,sizeof(iSelectedCompositionText));
    ReportEvent(EEventICFSelectionChanged,eventData);
    
    UiLayout()->SetAppEditorCursor(iSelectedCompositionText);
    
    return this;    
    }
    
// ---------------------------------------------------------------------------
// Handle pointer up event
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CFepInputContextField::HandlePointerDownEventL(const TPoint& aPt)
    {
    iLastInsertPos = PositionOfInsertionPointOnWindow();
    
    if ( !CFepLayoutEditAreaBase::HandlePointerDownEventL(aPt) )
        {
        return NULL;
        }
    
    return this;    
    }

EXPORT_C void CFepInputContextField::UpdateContent(const TCursorSelection& aCursorSel)
    {
    TInt cursorPos = aCursorSel.iCursorPos;
    if (cursorPos < 0 )
        {
        cursorPos = 0;
        }
    else if (cursorPos > iBuffer->Length())
        {
        cursorPos = iBuffer->Length();
        }   
    UpdateCursorSelection(TCursorSelection(cursorPos, aCursorSel.iAnchorPos));    
    
    TPtrC eventData((const TUint16 *)&iSelectedCompositionText,sizeof(iSelectedCompositionText));
    ReportEvent(EEventICFSelectionChanged,eventData);
    UiLayout()->SetAppEditorCursor(iSelectedCompositionText);
    }
//  End of File 
