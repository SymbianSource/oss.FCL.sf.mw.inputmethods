/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PENINPUTPLUGINKR_H__
#define __PENINPUTPLUGINKR_H__

#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include <aknfeppeninputimeplugin.h>
#include <centralrepository.h>

class RPeninputServer;
class CPtiEngine;
class CPluginKrFepManagerBase;

class CPeninputPluginKr : public CAknFepPenInputImePlugin
    {
public:

    static CPeninputPluginKr* NewLC(TAny* aInitParams);
    
    static CPeninputPluginKr* NewL(TAny* aInitParams);
    
    virtual ~CPeninputPluginKr();

private:

    CPeninputPluginKr(RPeninputServer* aServer);
    
    void ConstructL();

private: // from CAknFepPenInputImePlugin

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
    virtual MAknFepManagerInterface* GetInputMethodUiL(
        MAknFepManagerUIInterface* aFepManager,
        TLanguage aLanguage, 
        TInt aMode,
        const TDesC8& aData,
        const TBool aIsSplitView = EFalse);
    
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
    virtual MAknFepManagerInterface* GetInputMethodUiL(
        MAknFepManagerUIInterface* aFepManager,
        TInt aLayoutId,
        const TDesC8& aData);
    
    /**
     * Activate current IME plugin.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual void Activate();

    /**
     * Deactivate current IME plugin.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual void DeActivate();

    /**
     * Get current IME plugin implementation ID.
     * 
     * @since S60 v3.2
     *
     * @return IME plugin implementation id.
     */
    virtual TInt ImeImplId();

    /**
     * Get current layout UI implementation id.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual TInt LayoutUiImplId();

    /**
     * Handle pen input server event.
     * 
     * @param aEventId The event id, defined in <peninputcmd.h>
     * @param aData The event related data.
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual TBool HandleServerEventL(
        TInt aEventId, 
        const TDesC& aData);

    /**
     * Get suppored layout UI modes(VKB/HWR).
     * 
     * @since S60 v3.2
     *
     * @return Supported modes.
     */
    virtual TInt SupportModes(
        CPtiEngine* aPtiEngine, 
        RArray<TImePlguinImplDetail>& aSupportList) const;
    
    /**
     * Get current layout UI mode.
     * 
     * @since S60 v3.2
     *
     * @return Current layout UI mode.
     */
    virtual TInt CurrentMode() const;
    
    /**
     * Handle the menu command.The menu is launched by FEP
     * The menu command is first passed to IME plugin, if 
     * this function return EFalse, FEP handles it defaultly.
     * 
     * @param aCommandId The menu command id
     * @return ETrue if IME plugin handled the command; 
     *         EFalse otherwise.
     */
    virtual TBool HandleMenuCommandL(TInt aCommandId);
    
    /**
     * Dynamiclly update menu items during it displays. 
     * 
     * aMenuPane The menu pane pointer.
     * @return None.
     */
    virtual void DynInitMenuPaneL(
        CAknFepUiInterfaceMenuPane* aMenuPane);
    

private:
    
    void FindUiLayoutImplementationL();
    
    CPluginKrFepManagerBase* GetPluginUiL( TInt aMode );
    
    TInt SupportModesL(
        CPtiEngine* /*aPtiEngine*/,
        RArray<TImePlguinImplDetail>& aSupportList) const; 

    CPluginKrFepManagerBase* CPeninputPluginKr::GetCurrentUi();

    /**
     * Get VKB UI interface
     *
     * @return VKB UI pointer
     */
    CPluginKrFepManagerBase* GetVkbUiL();

    /**
     * Get Itut UI interface
     *
     * @return Itut UI pointer
     */
    CPluginKrFepManagerBase* GetItutUiL();

    /**
     * Get FSQ UI interface
     *
     * @return FSQ UI pointer
     */
    CPluginKrFepManagerBase* GetFSQUiL(TInt aLang);
    
    void ShowListQueryL( const TInt aResourceId );
    
private:
    
    RPeninputServer& iPeninputServer;

    RArray<TInt> iUiLayoutImpIdList;

    TInt iPenInputMode;
    
     /**
     * Pen input server pointer. Not own.
     */
    RPointerArray<CPluginKrFepManagerBase> iAllUiManagers;
    
    CPluginKrFepManagerBase* iPluginKrFepManager;
    
    CRepository* iRepository;
    
    TInt iResId;
    
    };

#endif  // __PENINPUTPLUGINKR_H__

