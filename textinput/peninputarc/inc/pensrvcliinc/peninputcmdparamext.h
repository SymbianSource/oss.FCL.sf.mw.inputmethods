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
* Description:  Peninput command data structure
*
*/


#ifndef _PENINPUTPARAM_EXT_H
#define _PENINPUTPARAM_EXT_H

typedef struct TUpdateDataTag
    {
    TRect iRect;
    TBool iUpdateFlag;
    } TUpdateDataArea;    

#ifdef RD_TACTILE_FEEDBACK 
struct TTactileFeedbackArea
    {
        TInt iId;		//unique id for this area
        TRect iNewRect; //The requested area
        TRect iPrevRect; //Previous area
        TInt iTactileType; //Tactile feedback type     
    } ;
#endif // RD_TACTILE_FEEDBACK
    
struct TUpdateArea
    {
public:
    // the rect to update
    TRect iRect;

    //flag indicates whether full updated needed.        
    // see MAnimSpriteFunctions::UpdateMember
    TBool iFullUpdateFlag;
    };
#endif //_PENINPUTPARAM_EXT_H
