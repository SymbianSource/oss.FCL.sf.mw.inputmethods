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
* Description:  generic VKB layout
*
*/


#ifndef C_CPENINPUTGENERICVKBLAYOUT_H
#define C_CPENINPUTGENERICVKBLAYOUT_H

// User includes
#include <AknFepGlobalEnums.h>
#include "peninputcommonlayoutext.h"

/**
 *  Pen input VKB layout class
 *  This class define the behaviors of VKB layout
 *
 *  @lib peninputgenericvkb.lib
 *  @since S60 v3.2
 */
class CPeninputGenericVkbLayout : public CPeninputCommonLayoutExt
    {
    
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @param aLayoutOwner The owner of layout
     * @param aInitData Init data of layout
     * @return The pointer to CPeninputGenericVkbLayout object
     */
    static CPeninputGenericVkbLayout* NewL( MLayoutOwner* aLayoutOwner, 
                                            const TAny* aInitData, TBool aIsFsq
                                             );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputGenericVkbLayout();

    /**
     * Check if FSQ is opened and ITI is enabled.
     *
     * @return if ETrue: current input method is FSQ and ITI is enabled.
     */
    inline TBool IsEnableITIOnFSQ()
        {
        return iITIEnabled && iLayoutType == EPluginInputModeFSQ;
        };

// From base class CPeninputCommonLayout

    /**
     * From CPeninputCommonLayout
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aCmd Command Id
     * @param aData Data for command
     * @return Errors when return value smaller than zero    
     */
    TInt HandleCommand( TInt aCmd, TUint8* aData );

    /**
     * From CPeninputCommonLayout
     * Handle control event
     *
     * @since S60 v3.2
     * @param aEventType event type
     * @param aCtrl Controler
     * @param aEventData The event data
     * @return None    
     */
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData );

    void HandleAppInfoChange(const TDesC& aInfo, TPeninputAppInfo aType); 
   
    /* Teleca change begin, 21.05.2009 ssal */
    static TInt HideByteWarningBubble(TAny* aPointer);
    void HideByteWarningBubble();
	/* Teleca change end, 21.05.2009 ssal */ 
protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @param aLayoutOwner The owner of layout
     * @return None
     */
    CPeninputGenericVkbLayout( MLayoutOwner* aLayoutOwner );
     
// From base class CPeninputCommonLayout

    /**
     * From CPeninputCommonLayout
     * Factory method for creation of layout window. The decision which 
     * layout window(VKB or HWR) is created will be postboned in subclasses 
     * of this class
     *
     * @since S60 v3.2
     * @return None
     */
    void CreateLayoutWindowL();

    /**
     * From CPeninputCommonLayout
     * Factory method for creation resource config. It is enough at present 
     * to create and initialize resource config. Any necessary extension 
     * will be implemented in subclasses
     * 
     * @since S60 v3.2
     * @return None
     */
    void CreateDataMgrL( const TAny* aInitData );

    /**
     * From CPeninputCommonLayout
     * Get the layout type, VKB or HWR, child class must rewrite the function
     * 
     * @since S60 v3.2
     * @return The layout type
     */
     TInt LayoutType();

     TInt SizeChanged( const TAny* aData );

    /**
     * SemiTransparencyRequired. Tell whether this layout require transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v3.2
     * @return ETrue if layout require transparency, otherwise EFalse.
     */
    TBool SemiTransparencyRequired();
protected:

    /**
     * Second-phased constructor
     *
     * @since S60 v3.2
     * @param aInitData Init data of layout
     * @return None
     */
     void ConstructL( const TAny *aInitData, TBool aIsFsq );
     
private: 
    void HandleVirtualKeyUpL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    /**
     * Handle virtual key down event
     *
     * @since S60 v5.0
     * @param None
     * @return None
     */
    void HandleVirtualKeyDown();
    
    /**
     * Handle show tooltip command
     *
     *     
     * @aData pointer is sent from FEP, point to a TPtrC
     * @return None
     */    
    void HandleShowTooltipCmdL( TUint8* aData );
    
    /**
     * Handle show candidate list command
     *
     *     
     * @aData pointer is sent from FEP, point to a TPtrC
     * @return None
     */    
    void HandleShowCandidateListCmdL( TUint8* aData );
    
    /**
     * Read text stored in a block of memory into HBufC
     *
     *     
     * @aData aStartPtr the starting address of a block of memory with a string in it
     * @aLength The length of the string
     * @return pointer of a HBufC storing the string in it
     */ 
    HBufC* ReadTextInfoHBufCL( TUint16* aStartPtr, TInt aLength );
    
    TBool TranslateMirrorUnicode( TUint16 aInputCode, TUint16& aOutputCode );
        
private:

    TInt iLayoutType;     
    TBool iInEditWordQueryDlg;
    
    /*
    * Indicate if ITI is enabled.
    */    
    TBool iITIEnabled;
    /* Teleca change begin, 21.05.2009 ssal */
    CPeriodic* iInfoTimer;
    /* Teleca change end, 21.05.2009 ssal */   
    };


/**
 *  Pen input VKB layout implementation class
 *  This class define the implementation interface of VKB layout
 *
 *  @lib peninputgenericvkb.lib
 *  @since S60 v3.2
 */
class CPeninputGenericVkbLayoutImp : public CFepUiLayoutInterface
    {
      
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aInitParams Init data for layout
     * @return Pointer to created CPeninputGenericVkbLayoutImp object
     */
    static CPeninputGenericVkbLayoutImp* NewL( TAny* aInitParams );
    static CPeninputGenericVkbLayoutImp* NewFsqL( TAny* aInitParams );
    static CPeninputGenericVkbLayoutImp* NewPrtFsqL( TAny* aInitParams );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */    
    virtual ~CPeninputGenericVkbLayoutImp();

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
    CFepUiLayout* CreateFepUiLayoutL( MLayoutOwner* aLayoutOwner,
                                      const TAny* aData );

private:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    CPeninputGenericVkbLayoutImp();

    /**
     * Second-phased constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    void Construct( TBool aIsFsq );
 
 private:
 
    TBool iIsFsq;   
    };

#endif // C_CPENINPUTGENERICVKBLAYOUT_H
