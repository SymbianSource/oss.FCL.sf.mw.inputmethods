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
* Description:  Trigger string
*
*/


#ifndef C_CAKNFEPHWRTRIGGERSTR_H
#define C_CAKNFEPHWRTRIGGERSTR_H

//  INCLUDES
#include <e32base.h>

const TInt KMaxTriggerStrLen = 128;

/**
 *  Trigger string
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrTriggerStr : public CBase
    {
public:

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrTriggerStr();

    /**
     * Append the char code to trigger string 
     *
     * @since S60 v3.2
     * @param aChar The char code.
     * @return None
     */
    void Append(const TDesC& aChar);
    
    /**
     * Set the char code to trigger string 
     *
     * @since S60 v3.2
     * @param aChar The char code.
     * @return None
     */
    void Set(const TDesC& aChar);

    /**
     * Using the last char code as trigger string
     *
     * @since S60 v3.2
     * @return None
     */            
    void SetLast();

    /**
     * delete the last char code
     *
     * @since S60 v3.2
     * @return None
     */            
    void DeleteLast();

    /**
     * Is the trigger string empty?
     *
     * @since S60 v3.2
     * @return ETrue if empty;otherwise EFalse
     */            
    TBool IsEmpty();
    
    /**
     * Get trigger string
     *
     * @since S60 v3.2
     * @param aString Carry trigger string on return
     * @return None
     */            
    TDesC& GetString();

private://data
    
    /**
     * trigger string
     */
    TBuf<KMaxTriggerStrLen> iString;

    };

#endif // C_CAKNFEPHWRTRIGGERSTR_H

// End Of File
