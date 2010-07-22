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
* Description:            Input mode plugin interac
*
*/













#ifndef C_AKNFEPPENINPUTIMEPLUGIN_H
#define C_AKNFEPPENINPUTIMEPLUGIN_H

//Include 
#include <ecom/ecom.h>

//Forward declaration
class MAknFepManagerInterface;
class MAknFepManagerUIInterface;
class CAknFepUiInterfaceMenuPane;
class CPtiEngine;

/** UID of IME plugin this interface */
const TInt KImePluginInterfaceId = 0x10207363;

/** Maxlength of IME plugin name */
const TInt KMaxImePluginName = 20;

/**
 *  Pen input mode plugin interface
 *
 *  @since S60 v3.2
 */
class CAknFepPenInputImePlugin: public CBase
    {
public:
    /**
     *  IME plugin implemetation detail struct
     *
     *  @since S60 v3.2
     */
    class TImePlguinImplDetail
        {
    public:
        TInt iImplementationId;
        TInt iMode;
        TInt iMeritValue;
        TInt iLanguage;
        TBuf<KMaxImePluginName> iDisplayName;
        };
        
    /**
     *  IME plugin weight number, if there are more than one
     *  IME plugin for some language, the system select the
     *  biggest meritvalue one.
     *
     *  @since S60 v3.2
     */
    enum TImePluginMerit
        {
        EImeMerit_NotUsed = 0,
        EImeMerit_Normal = 100,
        EImeMerit_Preferred = 200
        };
        
    /**
     * Destructor
     *
     * @since S60 v3.2
     */
     virtual ~CAknFepPenInputImePlugin() 
        { 
        REComSession::DestroyedImplementation(iDtor_ID_Key);                        
        }

public:
    /**
     * Pure virtual functions to implement the interface.
     */

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
                                        TInt      aMode,
                                        const TDesC8& aData,
                                        const TBool aIsSplitView = EFalse) = 0;
    
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
                                        const TDesC8& aData) = 0;
    
    /**
     * Activate current IME plugin.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual void Activate() = 0;

    /**
     * Deactivate current IME plugin.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual void DeActivate() = 0;

    /**
     * Get current IME plugin implementation ID.
     * 
     * @since S60 v3.2
     *
     * @return IME plugin implementation id.
     */
    virtual TInt ImeImplId() = 0;

    /**
     * Get current layout UI implementation id.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual TInt LayoutUiImplId() = 0;

    /**
     * Handle pen input server event.
     * 
     * @param aEventId The event id, defined in <peninputcmd.h>
     * @param aData The event related data.
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual TBool HandleServerEventL(TInt aEventId, const TDesC& aData) = 0;

    /**
     * Get suppored layout UI modes(VKB/HWR).
     * 
     * @since S60 v3.2
     *
     * @return Supported modes.
     */
    virtual TInt SupportModes(CPtiEngine* aPtiEngine, 
                              RArray<TImePlguinImplDetail>& aSupportList) const = 0;
    
    /**
     * Get current layout UI mode.
     * 
     * @since S60 v3.2
     *
     * @return Current layout UI mode.
     */
    virtual TInt CurrentMode() const = 0;
    
    /**
     * Handle the menu command.The menu is launched by FEP
     * The menu command is first passed to IME plugin, if 
     * this function return EFalse, FEP handles it defaultly.
     * 
     * @param aCommandId The menu command id
     * @return ETrue if IME plugin handled the command; 
     *         EFalse otherwise.
     */
    virtual TBool HandleMenuCommandL(TInt aCommandId) = 0;
    
    /**
     * Dynamiclly update menu items during it displays. 
     * 
     * aMenuPane The menu pane pointer.
     * @return None.
     */
    virtual void DynInitMenuPaneL(CAknFepUiInterfaceMenuPane* aMenuPane) = 0;
    
protected:
    /**
     * C++ default constructor.
     *
     * @param aServer Pen input server pointer
     */
    CAknFepPenInputImePlugin()
        {
        }
    
public:

    /**
     * Destructor key.
     */
    TUid iDtor_ID_Key;

protected:
    /**
     * Current language.
     */
    TLanguage iLanguage;

    /**
     * Fep manager UI interface. Not own.
     */
    MAknFepManagerUIInterface* iFepManager;
    };

#endif //C_AKNFEPPENINPUTIMEPLUGIN_H
//End Of File
