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
* Description:  header file of peninput layout engine
*
*/



#ifndef C_CPENINPUTLAYOUTENGINE_H
#define C_CPENINPUTLAYOUTENGINE_H

// INCLUDES
#include <e32base.h>
#include <badesca.h> 
#include <peninputuilayoutbase.h>

#include <AknFepGlobalEnums.h>

class CFepUiLayoutInterface;
class MLayoutOwner;
class CImePluginFinder;
class CPtiEngine;
/**
* CFepLayoutEngine class.
*
* This is main class for outside users to create layout engine
*
* @lib feplayouteng.lib
* @since S60 V4.0
*/
class CFepLayoutEngine : public CBase
    {
public:
    /**
     * Factory creator.
     *
     * @since S60 V4.0
     * @return The layout engine instance
     */
    IMPORT_C static CFepLayoutEngine* NewL();

    /**
     * Factory creator.
     *
     * @since S60 V4.0
	 * @param aPtiEngine The predictive engine instance
     * @return The layout engine instance
     */
    IMPORT_C static CFepLayoutEngine* NewL(CPtiEngine* aPtiEngine);
    
    /**
     * Destructor.
     *
     * @since S60 V4.0
     */
    ~CFepLayoutEngine();
    

    /**
     * Ceate ui layout 
     *
     * @since S60 V4.0
     * @param aInputMethodId The input method layout id
     * @param aLayoutOwner The layout owner
     * @param aData Data for layout creation. E.g, The screen resolution data
     * @return Pointer to Fep ui layout.
     */
    IMPORT_C MPenUiLayoutBase* GetUiLayout(TUid aInputMethodId, 
                                           MLayoutOwner* aLayoutOwner,const TAny* aData = NULL);

    /**
     * Return internal array
     *
     * @since S60 V4.0
     * @param aInputMethodId The input method layout id
     */
    IMPORT_C const RArray<TInt>& UiLayoutListL(TInt aInputMethodId);
    
    /**
     * Return internal array to requried Ime plugin list
     *
     * @since S60 V4.0
     * @param aLanguage The required supporting language
     * @param aPluginMode The required supporting mode
     */
    IMPORT_C const RArray<TInt>& ImePluginListL(TInt aLanguage, TInt aPluginMode);
    
    /**
     * Return internal array to all supporing touch input languages
     *
     * @since S60 V4.0
     */
    IMPORT_C const RArray<TInt>& TouchSupportLanguagesL();    
    
        
    /**
     * Is support TPluginInputMode for aLanguage. 
     *
     * @since S60 v4.0
     * @param aLanguage Value of TLanguage
     * @param aMode Value of TPluginInputMode
     * @return ETrue if supported. Otherwise EFalse.
     */
    IMPORT_C TBool IsSupportPluginMode( TLanguage aLanguage, TPluginInputMode aMode ) ;
    
    /**
     * Initialize finder. 
     *
     * @since S60 v4.0
     */
    IMPORT_C void InitializeL();
    
private:
    /**
     * 2nd phase constructor.
    *
     * @since S60 V4.0 
     */
    void ConstructL();

    /**
     * Destructor
     *
     * @since S60 V4.0 
     */
    CFepLayoutEngine(CPtiEngine* aPtiEngine = NULL);

    /**
     * Create UI layout plugin list after input method changed 
     *
     * @since S60 V4.0 
     * @param aInputMethodId The layout implementation id.
     */
    void FindImeLayoutL(TInt aInputMethodId);

private:
    /**
     * current layout id
     */
    TUid	iLayoutId;
    /**
     * current input method id. HWR, VKB ...
     */
    TInt	iInputMethodId;
    /**
     * layout id list for current input method
     * Own
     */
    RArray<TInt> iLayoutIdList;
    /**
     * current layout implementation
     */
    CFepUiLayoutInterface* iCurInterface;
    
    /**
     * current layout implementation
     */
    CImePluginFinder* iImeFinder;
    
    /**
     * current language support
     */
    RArray<TInt>    iTouchSupportLanguages;
    
    /**
     * current ime plugin list
     */
    RArray<TInt>    iImePluginIdList;
    
    /**
     * pti engine, NOT OWN
     */
    CPtiEngine*     iPtiEngine;
    };
    
#endif  C_CPENINPUTLAYOUTENGINE_H

// End of file
