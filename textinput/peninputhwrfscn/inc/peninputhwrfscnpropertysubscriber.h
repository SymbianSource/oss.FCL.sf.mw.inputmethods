/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  subscribers interface
*
*/


#ifndef M_PENINPUTHWRFSCNPROPERTYSUBSCRIBER_H
#define M_PENINPUTHWRFSCNPROPERTYSUBSCRIBER_H

/**
 *  Class MPeninputHwrfscnPropertySubscriber
 *  Interface for get property
 *
 *  @lib peninputhwrfscn.lib
 *  @since S60 v5.0
*/
class MPeninputHwrfscnPropertySubscriber
    {

public:

    /**
     * TPeninputHwrfscnProperty defines the property names of datasotre
     */
    enum TPeninputHwrfscnProperty
        {
        EPeninputHwrfscnPropertyCase,
        EPeninputHwrfscnPropertyRange,
        EPeninputHwrfscnPropertyPermittedRange,
        EPeninputHwrfscnPropertyLanguageChanged,
        EPeninputHwrfscnPropertyWriteSpeed,
        EPeninputHwrfscnPropertyPenSize,
        EPeninputHwrfscnPropertyPenColor,      
        EPeninputHwrfscnPropertyCandidates,
        EPeninputHwrfscnPropertyHomephonicCandidates,
        EPeninputHwrfscnPropertyPredictives,
        EPeninputHwrfscnPropertyStrokeEndMark,
        EPeninputHwrfscnPropertyPermittedCases,
        EPeninputHwrfscnPropertyAddPredictives,
        EPeninputHwrfscnPropertyNumberMode
        };
    /**
    * Check if the property could be changed before changing it.
    *
    * @since S60 v5.0
    * @param aProperty One of @see TPeninputHwrfscnProperty changed
    * @return TBool ETrue if the property needed to be changed, EFalse otherwise
    */
    virtual TBool BeforePropertyChangedL(TPeninputHwrfscnProperty aProperty) = 0;
	
	/**
	* Notify the property subscriber the property changed.
	*
	* @since S60 v5.0
	* @param aProperty One of @see TPeninputHwrfscnProperty changed
	*/
	virtual void PropertyChangedL(TPeninputHwrfscnProperty aProperty) = 0;
   
    };

#endif //M_PeninputHwrfscnPROPERTYSUBSCRIBER_H

//  End Of File
