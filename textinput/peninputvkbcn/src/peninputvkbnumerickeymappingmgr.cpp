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
* Description:  vkb number keymapping manager
*
*/


#include <eikon.hrh>

#include "peninputvkbnumerickeymappingmgr.h"

_LIT(KNumberString,"0123456789");
const TInt KMaxNumericString = 32;

// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//
CAknFepVkbNumericKepMappingMgr* CAknFepVkbNumericKepMappingMgr::NewL(TResourceReader aReader)
    {
    CAknFepVkbNumericKepMappingMgr* self = NewLC(aReader);
    CleanupStack::Pop(self);

    return self;        
    }
 
// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//    
CAknFepVkbNumericKepMappingMgr* CAknFepVkbNumericKepMappingMgr::NewLC(TResourceReader aReader)
    {
    CAknFepVkbNumericKepMappingMgr* self = new (ELeave) CAknFepVkbNumericKepMappingMgr();

    CleanupStack::PushL(self);
    self->ConstructL(aReader);

    return self;        
    }

// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//   
void CAknFepVkbNumericKepMappingMgr::ConstructL(TResourceReader aReader) 
    {
    const TInt count = aReader.ReadInt16();
    
    
    for (TInt i = 0; i < count; i++)
        {
        iKeyMappingIdList.Append(aReader.ReadInt16());
        TPtrC ptr = aReader.ReadTPtrC();
        iKeyMappingStringList.Append(ptr.Alloc());
        }
    }

// ------------------------------------------------------------------------
// Destructor
// ------------------------------------------------------------------------
//         
CAknFepVkbNumericKepMappingMgr::~CAknFepVkbNumericKepMappingMgr()    
    {
    iKeyMappingIdList.Reset();
    iKeyMappingIdList.Close();
    
    iKeyMappingStringList.ResetAndDestroy();
    iKeyMappingStringList.Close();
    }

// ------------------------------------------------------------------------
// Get number keymapping string
// ------------------------------------------------------------------------
//   
HBufC* CAknFepVkbNumericKepMappingMgr::KeyMappingStringL(const TInt aKeyMappingId) const
    {
    TLocale locale;
    TBuf<KMaxNumericString> rs = KNumberString();
    const TInt index = iKeyMappingIdList.Find(aKeyMappingId);
    
    if (index != KErrNotFound)
        {
        HBufC* buf = iKeyMappingStringList[index];
        
        rs.Append(*buf);
        
        if ((aKeyMappingId== EAknEditorCalculatorNumberModeKeymap) ||
            (aKeyMappingId== EAknEditorConverterNumberModeKeymap))
            {
            rs.Append(locale.DecimalSeparator()); 
            }       
        }
        
    return rs.AllocL();          
    }

// ------------------------------------------------------------------------
// Get Keymapping id array item count
// ------------------------------------------------------------------------
// 
TInt CAknFepVkbNumericKepMappingMgr::Count() const
    {
    return iKeyMappingIdList.Count();
    }

// ------------------------------------------------------------------------
// Remove all keymapping id and pointer in array            
// ------------------------------------------------------------------------
// 
void CAknFepVkbNumericKepMappingMgr::Reset()
    {
    iKeyMappingIdList.Reset();
    iKeyMappingStringList.ResetAndDestroy();
    }

//  End Of File
