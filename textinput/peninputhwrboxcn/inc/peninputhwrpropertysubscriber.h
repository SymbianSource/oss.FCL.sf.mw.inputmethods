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
* Description:  subscribers interface
*
*/


#ifndef M_AKNFEPHWRPROPERTYSUBSCRIBER_H
#define M_AKNFEPHWRPROPERTYSUBSCRIBER_H

/**
 *  Class MAknFepHwrPropertySubscriber
 *  Interface for get property
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
*/
class MAknFepHwrPropertySubscriber
    {

public:

    /**
     * TAknFepHwrProperty defines the property names of datasotre
     */
    enum TAknFepHwrProperty
        {
        EAknFepHwrPropertyCase,
        EAknFepHwrPropertyRange,
        EAknFepHwrPropertyPermittedRange,
        EAknFepHwrPropertyLanguage,
        EAknFepHwrPropertyWriteSpeed,
        EAknFepHwrPropertyPenSize,
        EAknFepHwrPropertyPenColor,
        EAknFepHwrPropertyCandidates,
        EAknFepHwrPropertyPredictives,
        EAknFepHwrPropertySizeChanged,
        EAknFepHwrPropertyStrokeEndMark,
        EAknFepHwrPropertyPermittedCases,
        EAknFepHwrPropertyAddPredictives
        };

    /**
     * Set property 
     *
     * @since S60 v3.2
     * @param aPropertyName The properyt name
     * @param aPropertyValue The properyt value
     * @return None
     */
    virtual void SetPropertyL(const TAknFepHwrProperty aPropertyName, 
                              const TDesC& aPropertyValue) = 0;
    
    /**
     * Return stroke end mark
     *
     * @since S60 v3.2
     * @return None
     */
    virtual TPoint StrokeEndMark() = 0;
    };

#endif //M_AKNFEPHWRPROPERTYSUBSCRIBER_H

//  End Of File
