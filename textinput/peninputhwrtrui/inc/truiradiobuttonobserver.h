/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Control radio button's observer
*
*/

#ifndef M_TRUIRADIOBUTTONOBSERVER_H
#define M_TRUIRADIOBUTTONOBSERVER_H

// CLASS DECLARATION

/**
* MTruiRadioButtonObserver.
* An instance of a class derived from MTruiRadioButtonObserver is an object
* which can be informed when an event of state-changed occure.
*/
class MTruiRadioButtonObserver 
    {
public:
    /**
     * Notify that which radio button is selected.
     * @param aIndex The index of currently selected index.
     */
    virtual void SelectChangedL( TInt aIndex ) = 0;
    };

#endif //M_TRUIRADIOBUTTONOBSERVER_H



