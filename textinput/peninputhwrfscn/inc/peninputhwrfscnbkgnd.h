/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen hwr controlbar background
*
*/

#ifndef PENINPUTHWRFSCNBKGND_H_
#define PENINPUTHWRFSCNBKGND_H_

// INCLUDES
#include <peninputlayoutctrlgroup.h>


/**
 *  class CPeninputHwrfscnBkgnd.
 *
 *  background window.
 *  CPeninputHwrfscnBkgnd is a container control with an opaque background.
 *
 *  @lib peninputhwrfscn.lib
 *  @since S60 v5.0
 */
class CPeninputHwrfscnBkgnd : public CControlGroup
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * 
     * @param aLayout The layout
     * @param aCtrlId The control id
     * @return Pointer to created object
     */    
    static CPeninputHwrfscnBkgnd* NewL( CFepUiLayout* aLayout, TInt aCtrlId );
    
    
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * 
     * @param aLayout The layout
     * @param aCtrlId The control id
     * @return Pointer to created object
     */    
    static CPeninputHwrfscnBkgnd* NewLC( CFepUiLayout* aLayout, TInt aCtrlId );
    
    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */    
    virtual ~CPeninputHwrfscnBkgnd();
    
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */      
    void Draw();
    
    /**
     * From CFepUiBaseCtrl.
     * Construct from resource. Handle resource change.
     *
     * @since S60 v4.0
     */    
    void ConstructFromResourceL();
    
protected:
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aLayout A Ui Layout environment (CFepUiLayout)
     * @param aCtrlId Control id
     * @return None
     */    
    CPeninputHwrfscnBkgnd( CFepUiLayout* aLayout, TInt aCtrlId );

private:
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    void ConstructL();
    
private: //datas
    /**
     * First time construct flag
     */    
    TBool iFirstTimeConstruct;
    
    };
#endif /*PENINPUTHWRFSCNBKGND_H_*/
