/*
* Copyright (c) 2005,2006 Choe Hwanjin
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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


#include "OssCombination.h"
#include "hangul.h"

TInt CombinationOrderFunction(
        const TOssCombinationItem& aFirst,
        const TOssCombinationItem& aSecond)
    {
    TUint keyFirst=aFirst.iFirst<<16|aFirst.iSecond;
    TUint keySecond=aSecond.iFirst<<16|aSecond.iSecond;
    return keyFirst-keySecond;
    }

TBool TOssCombinationItem::operator == (const TOssCombinationItem& aCombination) const
    {
    return iFirst==aCombination.iFirst && iSecond==aCombination.iSecond;
    }

EXPORT_C COssCombination* COssCombination::NewLC()
    {
    COssCombination* self = new (ELeave) COssCombination();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

EXPORT_C COssCombination* COssCombination::NewL()
    {
    COssCombination* self = COssCombination::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

COssCombination::COssCombination()
    : iHangulCombination(NULL)
    {
    }

void COssCombination::ConstructL()
    {
    }

EXPORT_C COssCombination::~COssCombination()
    {
    iCombinationList.Reset();
    iCombinationList.Close();
    if (iHangulCombination)
        hangul_combination_delete(iHangulCombination);    
    }

EXPORT_C TInt COssCombination::AddCombinationL(
        const TOssCombinationItem& aCombination)
    {
    TInt ret(KErrAlreadyExists);
    
    TInt index=FindCombination(aCombination);
    TLinearOrder<TOssCombinationItem> orderCombination(CombinationOrderFunction);
    
    if (index==KErrNotFound)
        {
        //iCombinationList.Append(aCombination);
        iCombinationList.InsertInOrderL(aCombination,orderCombination);
        ret=KErrNone;
        }
    
    return ret;
    }

EXPORT_C TInt COssCombination::RemoveCombination(
        const TOssCombinationItem& aCombination)
    {
    TInt ret(KErrNotFound);
    
    TInt index=FindCombination(aCombination);
    if (index!=KErrNotFound)
        {
        iCombinationList.Remove(index);
        ret=KErrNone;
        }
    return ret;
    }

EXPORT_C TInt COssCombination::FindCombination(
        const TOssCombinationItem& aCombination)
    {
    TInt ret(KErrNotFound);
    for (TInt i=0;i<iCombinationList.Count();i++)
        {
        if (iCombinationList[i]==aCombination)
            {
            ret=i;
            break;
            }
        }
    return ret;
    }

TBool COssCombination::PopulateListL()
    {
    if (iHangulCombination)
        {
        hangul_combination_delete(iHangulCombination);
        iHangulCombination=NULL;
        }
    iHangulCombination=hangul_combination_new();
    TUint cnt=iCombinationList.Count();
    TText* listFirst=new (ELeave) TText[cnt];
    TText* listSecond=new (ELeave) TText[cnt];
    TText* listResult=new (ELeave) TText[cnt];
    
    for (TInt i=0;i<cnt;i++)
        {
        listFirst[i]=iCombinationList[i].iFirst;
        listSecond[i]=iCombinationList[i].iSecond;
        listResult[i]=iCombinationList[i].iResult;
        }
    
    TBool ret=hangul_combination_set_data(
        iHangulCombination,
        listFirst,
        listSecond,
        listResult,
        cnt);
    
    delete[] listFirst;
    delete[] listSecond;
    delete[] listResult;
    return ret;
    }
