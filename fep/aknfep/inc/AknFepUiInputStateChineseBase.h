/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepInputStateChineseBase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUTSTATE_CHINESE_BASE_H__
#define __AKN_FEP_UI_INPUTSTATE_CHINESE_BASE_H__

// INCLUDES
#include <e32std.h>

#include <avkon.hrh>
#include <uikon.hrh>
#include <AknFepGlobalEnums.h>
#include "AknFepUiInputStateBase.h"
#include "AknFepUiCtrlContainerChinese.h"

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION
//class TAknFepInputStateBase;
class MAknFepUIManagerStateInterface;
//class MAknFepUICtrlContainerChinese;

/**
*  TAknFepInputStateChineseBase class.
*
*  @lib AknFep.lib
*  @since 2.6
*/
class TAknFepInputStateChineseBase : public TAknFepInputStateBase
    {
public: // Constructors and destructor
    /**
     * Constructor
     * @since 2.6
     * @param aOwner 
     * @param aUIContainer 
     */
    TAknFepInputStateChineseBase(MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerChinese* aUIContainer);
    
public:
    // A patch to force the vtable to be copied during an assignment.
    void operator=(const TAknFepInputStateBase& aState);

    /**
     * A patch to force the vtable to be copied during an assignment.
     * 
     * @param aState
     */
    void operator=(const TAknFepInputStateChineseBase& aState);
    /**
     * Getting UI container object
     * @since 2.6
     * @return Japanese UI container object
     */
    MAknFepUICtrlContainerChinese* UIContainer();
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );

protected:
    MAknFepUICtrlContainerChinese* iUIContainer;
    
   TBool iUseforResourcechange;    
      
    };
#endif//__AKN_FEP_UI_INPUTSTATE_CHINESE_BASE_H__
// End of file
