/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  UI class of CPenInputSettingAppUi
*
*/


#ifndef C_PENINPUTSETTINGAPPUI_H
#define C_PENINPUTSETTINGAPPUI_H

#include <aknViewAppUi.h>

// Forward reference
class CPenInputSettingAppView;
class CGSPenInputPlugin;

/**
 *  Instance of CPenInputSettingAppUi is the User Interface part of the AVKON
 *  application framework for the TrainingUI application
 *
 *  @code
 *   CEikAppUi* appUi = new (ELeave) CPenInputSettingAppUi;
 *  @endcode
 *
 *  @lib avkon.lib eikcore.lib eiksrv.lib
 */
class CPenInputSettingAppUi : public CAknViewAppUi
    {
    
public:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */      
    void ConstructL();

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */        
    CPenInputSettingAppUi();


    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    ~CPenInputSettingAppUi();
    inline TInt ParentWindowGroupId() const
        {
        return iParentWgId;
        };


public: // from CAknAppUi

    /**
     * Handle user menu selections
     *
     * @param aCommand the enumerated code for the option selected
     */
    void HandleCommandL(TInt aCommand);
    TBool ProcessCommandParametersL(CApaCommandLine& aCommandLine);
    void HandleWsEventL(const TWsEvent& aEvent,CCoeControl* aDestination);
    
private:

	/**
     * The pointer to app view.
     *
     * Not own
     */
    CGSPenInputPlugin* iAppView;        
    
	/**
     * Caller of the application
     *
     * Not own
     */    
    TUid iCallerUid;
    
	/**
     * The pointer to the plugin loaded by ecom
     *
     * Not own
     */
    TUid iDynamicUid;
    TInt iParentWgId;
    };


#endif // C_PENINPUTSETTINGAPPUI_H

