/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface of vkb ui state
*
*/


#ifndef C_PENINPUTVKBCTRLEXTJP_H
#define C_PENINPUTVKBCTRLEXTJP_H

// system includes
#include <peninputvkbctrlext.h>


// class declarations
/**
 *  vkb extenting control class for japanese
 *
 *  @lib peninputvkbjp.lib
 *  @since S60 v3.2
 */
class CPeninputVkbCtrlExtJp : public CPeninputVkbCtrlExt
    {
public:
    /**
     * Create one CPeninputVkbCtrlExt object
     *
     * @since S60 v3.2
     * @param aRect The rectangle of the control
     * @param aUiLayout The layout plugin
     * @param aControlId The control ID
     * @param TFontSpec The specification of font
     * @return The pointer to CPeninputVkbCtrlExtJp object
     */
    static CPeninputVkbCtrlExtJp* NewL( const TRect& aRect, 
                                               CFepUiLayout* aUiLayout,
                                               TInt aControlId,
                                               const TFontSpec& aFontSpec );

    /**
     * Create one CPeninputVkbCtrlExt object
     *
     * @since S60 v3.2
     * @param aRect The rectangle of the control
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aFont The pointer to CFont object
     * @return The pointer to CPeninputVkbCtrlExtJp object
     */
    static CPeninputVkbCtrlExtJp* NewL( const TRect& aRect,
                                               CFepUiLayout* aUiLayout,
                                               TInt aControlId,
                                               const CFont* aFont );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputVkbCtrlExtJp();

    /**
     * Dim all keys
     *
     * @since S60 V4.0
     * @param aFlag The dim status
     */
    void SetDimmed(TBool aDimFlag) const;

protected:

    /* 
     * Constructor
     *
     * @since S60 v3.2
     * @param aRect The rectangle of the control
     * @param aUiLayout The layout plugin
     * @param aControlId The control ID
     * @param aFontSpec The specification of font
     * @return None
     */
    CPeninputVkbCtrlExtJp( const TRect& aRect, 
                                  CFepUiLayout* aUiLayout,
                                  TInt aControlId,
                                  const TFontSpec& aFontSpec );

    };

#endif // C_PENINPUTVKBCTRLEXTJP_H

// End Of File

