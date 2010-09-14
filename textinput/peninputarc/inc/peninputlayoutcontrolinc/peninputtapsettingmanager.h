/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of peninput ui layout
*
*/


#ifndef PENINPUTTAPSETTINGMANAGER_H
#define PENINPUTTAPSETTINGMANAGER_H

#include <e32cmn.h>
#include <e32base.h>
#include <gdi.h>

NONSHARABLE_CLASS(CPeninputTapSettingManager) : public CBase
    {
public: //constructors
    /**
     * Symbian constructor.
     * 
     * @since Symbian^3
     * @return Pointer to created object
     */
    static CPeninputTapSettingManager* NewL();

    /**
     * Symbian constructor.
     * 
     * @since Symbian^3
     * @return Pointer to created object
     */
    static CPeninputTapSettingManager* NewLC();
    
    /*
     * Standard c++ destructor.
     * @since Symbian^3
     */
    virtual ~CPeninputTapSettingManager();

public:
    /**
     * Load all parameters of tap accuracy enhancement for specified layout.
     * 
     * @since Symbian^3
     * @param aLayoutType layout type, refer to TPluginInputMode
     * @return none
     */
    void Load( TInt aLayoutType );
    
    /**
     * Load all parameters of tap accuracy enhancement for specified layout.
     * 
     * @since Symbian^3
     * @param aLayoutType layout type, refer to TPluginInputMode
     * @param aFileName path name of configuration file
     * @return none
     */
    void Load( TInt aLayoutType, const TDesC& aFileName );
    
    /**
     * Get configuration of PointerMove event suppressor.
     * 
     * @since Symbian^3
     * @param aMaxMovement output max movement of PointerMove event
     * @param aTimeout output timeout(in microsecond) of PointerMove event
     * @return none
     */
    void GetPointerMoveSuppressor( TSize& aMaxMovement, TInt& aTimeout );
    
    /**
     * Get configuration of PointerUp event suppressor.
     * 
     * @since Symbian^3
     * @param aMaxMovement output max movement of PointerUp event
     * @param aTimeout output timeout(in microsecond) of PointerUp event
     * @return none
     */
    void GetPointerUpSuppressor( TSize& aMaxMovement, TInt& aTimeout );
    
    /**
     * Get extra response area of virtual key controls.
     * 
     * @since Symbian^3
     * @param aMargins output the margins around the original response area.
     * @return none
     */
    void GetKeyExtResponseArea( TMargins& aMargins );
    
    /**
     * Get extra response area of button controls.
     * 
     * @since Symbian^3
     * @param aMargins output the margins around the original response area.
     * @return none
     */
    void GetButtonExtResponseArea( TMargins& aMargins );

private:
    /**
     * C++ constructor
     * 
     * @since Symbian^3
     * @return None
     */
    CPeninputTapSettingManager();
    
    /**
     * Symbian second-phase constructor
     *
     * @since Symbian^3
     * @return None
     */ 
    void ConstructL();
private:
    /**
     * Load all parameters of tap accuracy enhancement for specified layout.
     * 
     * @since Symbian^3
     * @param aLayoutType layout type, refer to TPluginInputMode
     * @return none
     */
    void LoadFromRepositoryL( TInt aLayoutType );
    
    /**
     * Load all parameters of tap accuracy enhancement for specified layout
     * 
     * @since Symbian^3
     * @param aLayoutType layout type, refer to TPluginInputMode
     * @param aFileName path name of configuration file
     * @return none
     */
    void LoadFromFileL( TInt aLayoutType, const TDesC& aFileName );
    
    /**
     * Load default parameters
     * 
     * @since Symbian^3
     * @return none
     */
    void LoadDefault();
    
    /**
     * parse a TPoint from a comma separated values string.
     * 
     * @since Symbian^3
     * @return number of values in aText
     */
    TInt ParsePoints( TPoint& aPoint, const TDesC& aText );
    
    /**
     * parse a TMargins from a comma separated values string.
     * 
     * @since Symbian^3
     * @param aMargins output parse result
     * @param aText source string
     * @return number of values in aText
     */
    TInt ParseMargins( TMargins& aMargins, const TDesC& aText ); 
    
private:
    /**
     * max movement of PointerMove event
     */
    TSize iPointerMoveMaxMovement;
    
    /**
     * timeout of PointerMove event, microsecond
     */
    TInt  iPointerMoveTimeout;
    
    /**
     * max movement of PointerUp event
     */
    TSize iPointerUpMaxMovement;
    
    /**
     * timeout of PointerUp event, microsecond
     */
    TInt  iPointerUpTimeout;
    
    /**
     * extra response area of virtual key controls
     */
    TMargins iKeyMargins;
    
    /**
     * extra response area of button controls
     */
    TMargins iButtonMargins;
    };

#endif // PENINPUTTAPSETTINGMANAGER_H
