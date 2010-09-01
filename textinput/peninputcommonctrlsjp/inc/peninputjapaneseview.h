/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#ifndef C_CPENINPUTJAPANESEVIEW_H
#define C_CPENINPUTJAPANESEVIEW_H

#include <coecntrl.h>

class CEikListBox;

class CPeninputJapaneseView: public CCoeControl
    {
public:
    IMPORT_C void ConstructL();
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    void SetListBox(CEikListBox* aListBox);
    void Draw(const TRect& aRect) const;

private:
    // Not Owning
    CEikListBox* iListBox;
    };

#endif //C_CPENINPUTJAPANESEVIEW_H

// End Of File
