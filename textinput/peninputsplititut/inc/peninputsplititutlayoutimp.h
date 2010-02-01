/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#ifndef C_SPLITVIRTUALITUTLAYOUTIMP_H
#define C_SPLITVIRTUALITUTLAYOUTIMP_H

#include <peninputlayoutplugininterface.h>
#include <peninputlayout.h>

class CSplitVirtualItutLayoutImp : public CFepUiLayoutInterface
    {
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aInitParams Init data for layout
     * @return Pointer to created CVirtualItutLayoutImp object
     */
    static CSplitVirtualItutLayoutImp* NewL(TAny* aInitParams);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */    
    ~CSplitVirtualItutLayoutImp();

    // From base class CFepUiLayoutInterface
    /**
     * From CFepUiLayoutInterface
     * Create fep ui layout
     *
     * @since S60 v3.2
     * @param aLayoutOwner The layout owner
     * @param aData The data for creating layout
     * @return Pointer to created CFepUiLayout object
     */
    CFepUiLayout* CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,
                                     const TAny* aData);

private:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    CSplitVirtualItutLayoutImp();

    /**
     * Second-phased constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    void ConstructL();
    };

#endif // C_SPLITVIRTUALITUTLAYOUTIMP_H