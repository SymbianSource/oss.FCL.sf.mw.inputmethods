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
* Description:  image layout implementation
*
*/


#include "peninputvkbImlayout.h"

// CONSTANTS
_LIT(KPipeSympol,'|');
const TInt KBufferSize = 5;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepControlInfo* CAknFepControlInfo::NewL(TResourceReader& aReader)
    {
    CAknFepControlInfo* self = new (ELeave) CAknFepControlInfo();

    CleanupStack::PushL(self);
    self->ConstructFromResourceL(aReader); 
    CleanupStack::Pop();
    
    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepControlInfo::~CAknFepControlInfo()
    {
    }
    
// ---------------------------------------------------------
// Construct from resource file
// ---------------------------------------------------------
//    
void CAknFepControlInfo::ConstructFromResourceL(TResourceReader& aReader)
    {
    iControlID = aReader.ReadInt16();
    iBeginRow = aReader.ReadInt16();
    iBeginColumn = aReader.ReadInt16();
    iEndRow = aReader.ReadInt16();
    iEndColumn = aReader.ReadInt16();
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepVkbImLayout* CAknFepVkbImLayout::NewL(TResourceReader& aReader)
    {
    CAknFepVkbImLayout* self = NewLC(aReader);
    CleanupStack::Pop(self);
  
    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepVkbImLayout* CAknFepVkbImLayout::NewLC(TResourceReader& aReader)
    {
    CAknFepVkbImLayout* self = new (ELeave) CAknFepVkbImLayout();

    CleanupStack::PushL(self);
    self->ConstructL(aReader);
  
    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepVkbImLayout::~CAknFepVkbImLayout()
    {
    delete iPermittedLanguage;
    iControlInfoList.ResetAndDestroy();
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CAknFepVkbImLayout::ConstructL(TResourceReader& aReader)
    {
    ConstructFromResourceL(aReader);
    }

// ---------------------------------------------------------
// Construct from resource file
// ---------------------------------------------------------
//          
void CAknFepVkbImLayout::ConstructFromResourceL(TResourceReader& aReader)
    {
    iLayoutID = aReader.ReadInt16();
    iPermittedLanguage = aReader.ReadHBufCL();
    iColumns = aReader.ReadInt16();
    iRows = aReader.ReadInt16();
    
    const TInt count = aReader.ReadInt16();
    
    for (TInt i = 0; i < count; i++)
        {
        CAknFepControlInfo* controlInfo = CAknFepControlInfo::NewL(aReader);
        AddControlInfo(controlInfo);
        }
    }

// ---------------------------------------------------------
// Construct from resource file
// ---------------------------------------------------------
// 
TBool CAknFepVkbImLayout::IsAvailableForLang(const TInt aLangID)
    {
    TBuf<KBufferSize> buf;
    buf.Append(KPipeSympol);
    buf.AppendNum(aLangID);
    buf.Append(KPipeSympol);
    
    if (iPermittedLanguage->Find(buf) != KErrNotFound)
        {
        return ETrue;
        }

    return EFalse;
    }

// ---------------------------------------------------------
// Get control infor based on control id
// ---------------------------------------------------------
// 
CAknFepControlInfo* CAknFepVkbImLayout::FindControlInfo(const TInt aControlID)
    {
    const TInt count = iControlInfoList.Count();
    
    for (TInt i = 0; i < count; i++)
        {
        if (iControlInfoList[i]->ControlID() == aControlID)
            {
            return iControlInfoList[i];
            }
        }

    return NULL;
    }

// ---------------------------------------------------------
// Add a control info to info list
// ---------------------------------------------------------
// 
void CAknFepVkbImLayout::AddControlInfo(CAknFepControlInfo* aControlInfo)
    {
    iControlInfoList.Append(aControlInfo);
    }
 
// ---------------------------------------------------------
// Remove a control info from info list
// ---------------------------------------------------------
// 
void CAknFepVkbImLayout::RmoveControlInfo(CAknFepControlInfo* aControlInfo)
    {
    CAknFepControlInfo* controlInfo = FindControlInfo(aControlInfo->ControlID());
    if (controlInfo)
        {
        RmoveControlInfo(controlInfo->ControlID());
        }
    }

// ---------------------------------------------------------
// Remove a control info from info list based on control id
// ---------------------------------------------------------
//
void CAknFepVkbImLayout::RmoveControlInfo(const TInt aControlID)
    {
    const TInt count = iControlInfoList.Count();

    for (TInt i = 0; i < count; i++)
        {
        if (iControlInfoList[i]->ControlID() == aControlID)
            {
            CAknFepControlInfo* controlInfo = iControlInfoList[i];
            delete controlInfo;
            iControlInfoList.Remove(i);
            break;
            }
        }      
    }

//  End Of File
