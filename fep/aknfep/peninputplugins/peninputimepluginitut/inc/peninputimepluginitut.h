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
* Description:            input mode plugin header file
*
*/












#ifndef C_PENINPUTIMEPLUGINITUT_H
#define C_PENINPUTIMEPLUGINITUT_H   

// System includes
#include <e32base.h>
#include <peninputclient.h>
#include <aknfeppeninputimeplugin.h>
#include <eikon.hrh>
#include <AknFepManagerInterface.h>
//#include <AknFepManagerUIInterface.h>
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>

// Forward declaration
class MAknFepManagerInterface;
class CAknEdwinState;
class CRepository;
class CPluginFepManagerBase;

/**
 *  Pen input mode plugin chinese implementation
 *
 *  @lib peninputimeplugingeneric.lib
 *  @since S60 v3.2
 */
class CPenInputImePluginItut : public CAknFepPenInputImePlugin
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aInitParams Init data plugin, actually it is 
     *        a pointer of RPeninputServer.
     * @return Pointer to created CPenInputImePluginItut object
     */
    static CPenInputImePluginItut* NewL( TAny* aInitParams );

    static CPenInputImePluginItut* NewLC( TAny* aInitParams );

    /**
     * Destructor.
     */
    ~CPenInputImePluginItut();

public: // From CAknFepPenInputImePlugin

    /**
     * Get layout UI interface
     * 
     * @since S60 v3.2
     * @param aFepManager Fep manager interface pointer
     * @param aLanguage Language that layout UI need to support
     * @param aMode Speicifed layout UI mode to create
     * @param aData Initialization data.
     *
     * @return The layout UI interface..
     */
    MAknFepManagerInterface* GetInputMethodUiL( 
                            MAknFepManagerUIInterface* aFepManager,
                            TLanguage aLanguage,
                            TInt aMode,
                            const TDesC8& aData,
                            const TBool aIsSplitView = EFalse );

    /**
     * Get layout UI interface
     * 
     * @since S60 v3.2
     * @param aFepManager Fep manager interface pointer
     * @param aLayoutId Speicifed layout UI id to create
     * @param aData Initialization data.
     *
     * @return The layout UI interface..
     */
    MAknFepManagerInterface* GetInputMethodUiL( 
                            MAknFepManagerUIInterface* aFepManager, 
                            TInt aLayoutId, 
                            const TDesC8& aData );

    /**
     * Activate current IME plugin.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    void Activate();

    /**
     * Deactivate current IME plugin.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    void DeActivate();

    /**
     * Get current IME plugin implementation ID.
     * 
     * @since S60 v3.2
     *
     * @return IME plugin implementation id.
     */
    TInt ImeImplId();

    /**
     * Get current layout UI implementation id.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
     TInt LayoutUiImplId();

    /**
     * Handle pen input server event.
     * 
     * @param aEventId The event id, defined in <peninputcmd.h>
     * @param aData The event related data.
     * @since S60 v3.2
     *
     * @return None.
     */
     TBool HandleServerEventL( TInt aEventId, const TDesC& aData );

    /**
     * Get suppored layout UI modes(VKB/HWR).
     * 
     * @since S60 v3.2
     *
     * @param aPtiEngine  A pointer to ptiengine
     * @param aSupportList A reference to result ime plugin list
     * @return Meaningless.
     */
     TInt SupportModes(CPtiEngine* aPtiEngine, 
                       RArray<TImePlguinImplDetail>& aSupportList) const;

    /**
     * Get current layout UI mode.
     * 
     * @since S60 v3.2
     *
     * @return Current layout UI mode.
     */
     TInt CurrentMode() const;

    /**
     * Handle the menu command.The menu is launched by FEP
     * The menu command is first passed to IME plugin, if 
     * this function return EFalse, FEP handles it defaultly.
     * 
     * @param aCommandId The menu command id
     * @return ETrue if IME plugin handled the command; 
     *         EFalse otherwise.
     */
    TBool HandleMenuCommandL( TInt aCommandId );

    /**
     * Dynamiclly update menu items during it displays. 
     * 
     * aMenuPane The menu pane pointer.
     * @return None.
     */
    void DynInitMenuPaneL( CAknFepUiInterfaceMenuPane* aMenuPane );
    
public:

    enum TScriptIndex
        {
        ELatinLang,
        ECyrillicLang,
        EGreekLang,
        EHebrewLang,
        EArabicLang,
        EFarsiLang,
        EUrduLang,
        EThaiLang,
        EChnLang,
        EIndicLang
        };
        
private:        
    
    /**
     * C++ default constructor.
     *
     * @param aServer Pen input server pointer
     */
    CPenInputImePluginItut( RPeninputServer* aServer );
    
    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @return None
     */
    void ConstructL();

    /**
     * Enumuate all layout UI ids and save them to iUiLayoutImpIdList.
     *
     * @return None
     */
    void FindUiLayoutImplementationL();
    
    /**
     * Get plugin UI interface
     *
     * @return plugin UI pointer
     */
    CPluginFepManagerBase* GetPluginUiL( TInt aMode );
    
    /**
     * Get current UI interface
     *
     * @return current UI pointer
     */
    inline CPluginFepManagerBase* GetCurrentUi();
        
    /**
     * Get suppored layout UI modes(VKB/HWR).
     * 
     * @since S60 v3.2
     *
     * @param aPtiEngine  A pointer to ptiengine
     * @param aSupportList A reference to result ime plugin list
     * @return Meaningless.
     */
    TInt SupportModesL(CPtiEngine* aPtiEngine, 
                       RArray<TImePlguinImplDetail>& aSupportList) const;

    static const TDesC& ResFileNameByScriptIdx(const TScriptIndex aIndex);

    static const TDesC& ResFileNameByScriptIdxForSplit(const TScriptIndex aIndex);

private:

    /**
     * Pen input server pointer. Not own.
     */
    RPeninputServer* iPenInputServer;

    /**
     * Pen input server pointer. Not own.
     */
    RArray<TInt> iUiLayoutImpIdList;

    /**
     * Current layout UI mode.
     */
    TInt iPenInputMode;

    /**
     * Pen input server pointer. Not own.
     */
    CPluginFepManagerBase* iPluginUiManager;
    };

#include "peninputimepluginitut.inl"

#endif // C_PENINPUTIMEPLUGINITUT_H

//End Of File
