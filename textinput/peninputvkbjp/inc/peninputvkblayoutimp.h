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
* Description:  vkb layout
*
*/


#ifndef C_CPENINPUTVKBLAYOUTIMP_H
#define C_CPENINPUTVKBLAYOUTIMP_H

//  INCLUDES
#include <peninputlayout.h>
#include <peninputlayoutplugininterface.h>


/** class forward decalaration */

/**
 *  peninputvkbjp layout interface implemtation
 *
 *  @lib peninputvkbjp.lib
 *  @since S60 v3.2
 */
class CPeninputVkbLayoutImp : public CFepUiLayoutInterface
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aInitParams Init data for layout
     * @return Pointer to created CCPeninputVkbLayoutImp object
     */
    static CPeninputVkbLayoutImp* NewL(TAny* aInitParams);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputVkbLayoutImp();

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
    CPeninputVkbLayoutImp();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL();

private:// data

    };

#endif // C_CPENINPUTVKBLAYOUTIMP_H

//End Of File
