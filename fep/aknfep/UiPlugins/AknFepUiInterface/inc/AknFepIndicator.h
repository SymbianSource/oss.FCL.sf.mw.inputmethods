/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepIndicator definition.
*
*/












#ifndef AKNFEPIND_H
#define AKNFEPIND_H

#include <e32base.h>
#include <aknEditStateIndicator.h>  //MAknEditingStateIndicator

class CAknNavigationDecorator;
class CAknNavigationControlContainer;
class CEikonEnv;

class CAknFepIndicator : public CBase, public MAknEditingStateIndicator
	{
public:
    IMPORT_C ~CAknFepIndicator();
	
public: // new functions    
    /**
     * Sets flag that copy mode is ON
     */    
    IMPORT_C void SetCopyMode(TBool aCopyMode); 
    
protected:
    TBool                       iCopyMode;
    TBool                       iCopyModeChanged;
	};

#endif // AknFepIND_H

// End of file
