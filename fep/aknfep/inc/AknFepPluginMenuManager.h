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
* Description:            Plugin menu manager class header
*
*/












#ifndef C_AKNFEPPLUGINMENUMANAGER_H
#define C_AKNFEPPLUGINMENUMANAGER_H

//Include
#include <aknfepuimenus.h>

//Foward declaration
class CAknFepManager;

/**
 *  Class of management of menu launched by plugin IME
 *
 *  @since S60 v3.2
 */
class CAknFepPluginMenuManager: public CBase, public MAknFepUiInterfaceMenuObserver
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @return Pointer to created CAknFepPluginMenuManager object
     */
    static CAknFepPluginMenuManager* NewL(CAknFepManager* aManager);
  
    /**
     * Destructor.
     */
    ~CAknFepPluginMenuManager();

public:
    /**
     * Show speicfied menu pane
     * 
     * @since S60 v3.2
     * @param aResourceId Menu resource id.
     * @return
     */
    void ShowL(TInt aResourceId);

    /**
     * Get menu show status
     * 
     * @since S60 v3.2
     * @return ETrue if show, otherwise EFalse.
     */
    TBool IsShowing();
    
    /**
     * Hide current showed menu.
     * 
     * @since S60 v3.2
     * @return None.
     */
    void Hide();
    
    /**
     * Get current menu resource id.
     * 
     * @since S60 v3.2
     * @return Resource id.
     */
    TInt MenuPaneId();

public: 
    // MAknFepUiInterfaceMenuObserver
    /**
     * Called when menu get or lose focus.
     * The function does nothing, for FEP will handle menu command anyway.
     * @since S60 v3.2
     * @param aMenuControl The menu control
     * @param aEmphasis ETrue to emphasize the menu, EFalse otherwise
     * @return None.
     */
    virtual void SetEmphasis(CBase* aMenuControl, TBool aEmphasis);
    
    /**
     * Process command user selects.
     * The function does nothing, for FEP will handle menu command anyway.
     * @since S60 v3.2
     * @param 
     * @return None.
     */
    virtual void ProcessCommandL(TInt aCommandId);

private:
    /**
     * C++ default constructor.
     *
     * @param aFepManager Fep manager object.
     */
    CAknFepPluginMenuManager(CAknFepManager* aFepManager);

private:
    /**
     * Current menu bar. Own.
     */
    CAknFepUiInterfaceMenuBar*    iOptionMenuBar;
    /**
     * Fep manager. Not own.
     */
    CAknFepManager* iFepManager;
    };

#endif

// End of file
