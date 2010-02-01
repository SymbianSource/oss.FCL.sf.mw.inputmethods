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
* Description:  hwr layout
*
*/


#ifndef C_CPENINPUTHWRBOXLAYOUTIMP_H
#define C_CPENINPUTHWRBOXLAYOUTIMP_H

//  INCLUDES
#include <peninputlayout.h>
#include <peninputlayoutplugininterface.h>


/** class forward decalaration */

/**
 *  peninputhwrboxjp layout interface implemtation
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBoxLayoutImp : public CFepUiLayoutInterface
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aInitParams Init data for layout
     * @return Pointer to created CCPeninputHwrBoxLayoutImp object
     */
    static CPeninputHwrBoxLayoutImp* NewL(TAny* aInitParams);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputHwrBoxLayoutImp();

    /**
     * From CFepUiLayoutInterface.
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aLayoutOwner The layout owner
     * @param aData The data for creating layout
     * @return Pointer to created CFepUiLayout object
     */
    CFepUiLayout* CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,const TAny* aData);

private:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */
    CPeninputHwrBoxLayoutImp();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL();

private:// data

    };

#endif // C_CPENINPUTHWRBOXLAYOUTIMP_H

//End Of File
