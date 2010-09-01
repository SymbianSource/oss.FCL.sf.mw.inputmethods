/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef M_SPLITITUTPROPERTYSUBSCRIBER_H
#define M_SPLITITUTPROPERTYSUBSCRIBER_H

class MItutPropertySubscriber
    {
public:

    enum TItutProperty
        {
        EItutPropertyKeypadResourceId,
        EItutPropertyCandidateListResourceId,
        EItutPropertyLandscape
        };

    virtual void SetPropertyL(TItutProperty aPropertyName, const TDesC& aPropertyValue) = 0;
    };

#endif //M_SPLITITUTPROPERTYSUBSCRIBER_H