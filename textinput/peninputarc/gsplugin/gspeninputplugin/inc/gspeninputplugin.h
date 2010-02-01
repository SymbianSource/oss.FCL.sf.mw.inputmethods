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
* Description:  gspeninputplugin header file.
*
*/


#ifndef C_GSPENINPUTPLUGIN_H
#define C_GSPENINPUTPLUGIN_H

// INCLUDES
#include <gsbaseview.h>
#include <gsfwviewuids.h>
#include <gsplugininterface.h>

// FORWARD CLASS DECLARATION
class CAknViewAppUi;
class CGSPenInputContainer;

// CONSTANTS
const TUid KGSInputPenPluginUID = { 0x1027510B };
_LIT(KGSInputPenPluginResourceFileName, "z:GSPenInputPluginRsc.rsc");
                                               
/**
 *  GSPenInputPlugin view class
 *
 *  This class is GSPenInputPlugin view class. It offers not only CAknView
 *  but also CGSBaseView behavior. General settings framework will use the class
 *  to show  GSPenInputPlugin main view.
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSPenInputPlugin : public CGSBaseView
    {

public: 

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aInitParams Framework parameter
     * @return Pointer to created CGSPenInputPlugin object
     */
    static CGSPenInputPlugin* NewL(TAny* aInitParams);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSPenInputPlugin();
        
// From CAknView

    /**
     * From CAknView
     * Returns views id
     *
     * @since S60 v3.2
     * @return id for this view.
     */
    TUid Id() const;

    /**
     * From CAknView
     * Handle client area change
     *
     * @since S60 v3.2
     * @return None
     */
    void HandleClientRectChange();

    /** 
     * From CAknView
     * Activate GsPenInputPlugin view
     *
     * @since S60 v3.2 
     * @param aViewId Identifies the view to activate.
     * @param aCustomMessageId Specifies the message type.
     * @param aCustomMessage The message passed to the activated view.
     * @return None
     */
    void DoActivateL(const TVwsViewId& aPrevViewId,
                     TUid aCustomMessageId,
                     const TDesC8& aCustomMessage);

    /** 
     * From CAknView
     * Deactivation of GsPenInputPlugin view
     *
     * @since S60 v3.2 
     * @return None
     */
    void DoDeactivate();

    /** 
     * From CAknView
     * Command handling function called by application frame work
     * 
     * @since S60 v3.2 
     * @param aCommand ID of the command to respond to.
     * @return None 
     */
    void HandleCommandL(TInt aCommand);

// From CGSPluginInterface

    /**
     * From CGSPluginInterface
     * Method for getting caption of this plugin
     *
     * @since S60 v3.2     
     * @param aCaption Reference to caption variable
     * @return None
     */
    void GetCaptionL(TDes& aCaption) const;

    /**
     * From CGSPluginInterface
     * Method for reading the ID of the plugin provider category. See
     * TGSPluginProviderCategory. PluginProviderCategory can be used for
     * sorting plugins.
     *
     * Default value is EGSPluginProvider3rdParty. Override this function
     * to change the category.
     *
     * @since S60 v3.2     
     * @return Plugin provider category ID defined by 
     *         TGSPluginProviderCategory
     */
    TInt PluginProviderCategory() const;

// From CGSBaseView
 
    /**
     * From CGSBaseView
     * Returns the container this view has.
     *
     * @since S60 v3.2 
     * @return pointer to container
     */
    CGSPenInputContainer* Container() const;

    
// From MEikMenuObserver
    
    /**
     * From MEikMenuObserver
     * 
     * @since S60 v3.2
     * @param aResourceId resource id 
     * @param aMenuPane 
     * @return None
     */
    virtual void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane);
    
protected:
    
    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    CGSPenInputPlugin();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    void ConstructL();     

private: // From CGSBaseView
    
    /*
     * From CGSBaseView
     * Functionality for creating a container.
     * 
     * @since S60 v3.2
     * @return None
     */
    void NewContainerL();

    /*
     * From CGSBaseView
     * Handles listbox selection.
     *
     * @since S60 v3.2
     * @return None
     */
    void HandleListBoxSelectionL();
    // From CGSPluginInterface

    /*     
     * Creates a new icon of desired type.
     *     
     * @return None
     */
    CGulIcon* CreateIconL( const TUid aIconType );
    
    
public:
    TUid iDestrouctorIDKey;
    
    };

#endif // C_GSPENINPUTPLUGIN_H

// End Of File
