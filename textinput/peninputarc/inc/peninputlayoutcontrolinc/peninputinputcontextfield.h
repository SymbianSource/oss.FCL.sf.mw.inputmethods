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
* Description:  header file of input context field
*
*/


#ifndef C_CFEPINPUTCONTECTFIELD_H
#define C_CFEPINPUTCONTECTFIELD_H

#include <peninputlayouteditareabase.h>

/**
 * CFepInputContextField
 *
 * Class for Edwin-like editing area
 *
 * @lib feplayoutcontrol.lib
 * @since S60 V4.0
 */
class CFepInputContextField : public CFepLayoutEditAreaBase
    {
public:  // Method

    // Constructors and destructor
    /**
     * Static constructor.
     *
     * @since S60 V4.0
     * @param aRect Control area
     * @param aUiLayout The layout which contains this control
     * @param aControlId The control Id
     * @return An instance of CFepInputContextField
     */
    IMPORT_C static CFepInputContextField* NewL(TRect aRect, CFepUiLayout* aUiLayout,
                                                TInt aControlId);
    
    /**
     * Destructor.
     *
     * @since S60 V4.0
     */
    IMPORT_C virtual ~CFepInputContextField();

    /**
     * Get the editor max length. 
     * This is the max char length which this editor can holds
     *
     * @since S60 V4.0
     * @return The editor max length
     */
    TInt GetInputContextFieldMaxLen();
   
    /**
     * Sets the text to the editor
     *
     * @since S60 V4.0
     * @param aEditTextLen The total length for the text in application editor.
     *		  This editor area must know it to parse the text
     * @param aTextStartPos The start position where the text is selected.
     * @param aCursorSel Current cursor selection
     * @param aText The text to be set in fep ui
     */
    IMPORT_C void SetTextL(const TFepInputContextFieldData& aData);

protected:  // Methods
    /**
     * C++ default constructor.
     *
     * @since S60 V4.0
     * @param aRect The control area
     * @param aUiLayout The layout which contains this control
     * @param aControlId control Id
     */
    IMPORT_C CFepInputContextField(TRect aRect,CFepUiLayout* aUiLayout,TInt aControlId);

    /**
     * 2nd phase constructor.
     *
     * @since S60 V4.0 
     */
    IMPORT_C void ConstructL();                                                     

    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 v4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 v4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CFepLayoutEditAreaBase
     * Update content when scrolling
     *
     * @since S60 v4.0
     * @param aCursorSel The selection info
     * @return None
     */                  
    IMPORT_C void UpdateContent(const TCursorSelection& aCursorSel);

private: //data
    
    /**
     * Last insert pointer position on window to judge move direction
     */                  
    TPoint iLastInsertPos;
    
    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;    
    };
#endif      // C_CFEPINPUTCONTECTFIELD_H              
// End of File
