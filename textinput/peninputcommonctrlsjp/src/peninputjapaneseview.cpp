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
* Description:  japanese popup view
**
*/

#include "peninputjapaneseview.h"
#include <eiklbx.h>   // ScrollBarFrame()

EXPORT_C void CPeninputJapaneseView::ConstructL()
    {
    CreateBackedUpWindowL(iCoeEnv->RootWin());
    }

TInt CPeninputJapaneseView::CountComponentControls() const
    {
    if (iListBox)
        {
        if (iListBox->ScrollBarFrame() && iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical))
            {
            return 2;
            }
        return 1;
        }
    return 0;
    }

CCoeControl* CPeninputJapaneseView::ComponentControl(TInt aIndex) const
    {
    if (aIndex
        && iListBox
        && iListBox->ScrollBarFrame()
        && iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical))
        {
        return iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical);
        }
    return iListBox;
    }

void CPeninputJapaneseView::SetListBox(CEikListBox* aListBox)
    {
    iListBox = aListBox;
    }

void CPeninputJapaneseView::Draw(const TRect& /*aRect*/) const
    {
    }
// End Of File
