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
inline TInt CAknFepCtrlRawKeyButton::DownEvent() const
    {                           
    return iDownEvent;
    }

inline TInt CAknFepCtrlRawKeyButton::UpEvent() const
    {                           
    return iUpEvent;
    }

inline TInt CAknFepCtrlRawKeyButton::ScanCode() const
    {             
    return iScanCode;                                     
    }

inline void CAknFepCtrlRawKeyButton::SetDownEvent(TInt aEvent)
    {               
    iDownEvent = aEvent;
    }

inline void CAknFepCtrlRawKeyButton::SetUpEvent(TInt aEvent)
    {               
    iUpEvent = aEvent;
    }

inline void CAknFepCtrlRawKeyButton::SetScanCode(TInt aScanCode)
    {                                                
    iScanCode = aScanCode;  
    }

// End Of File
