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
* Description:               Fep pen support interface
*
*/















#ifndef M_AKNFEPPENSUPPORTINTERFACE_H
#define M_AKNFEPPENSUPPORTINTERFACE_H

/**
 *  Class MAknFepPenSupportInterface
 *  Interface for support pen FEP mode
 *
 *  @since S60 v3.2
*/
class MAknFepPenSupportInterface
    {

public:

    /**
     * Start pen input method
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void ActivatePenInputL() = 0;
    
    /**
     * Close pen input method
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void DeactivatePenInputL() = 0;
    
    };

#endif //M_AKNFEPPENSUPPORTINTERFACE_H

//  End Of File
