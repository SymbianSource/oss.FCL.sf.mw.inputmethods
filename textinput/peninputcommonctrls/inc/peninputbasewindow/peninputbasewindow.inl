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
* Description:  inline file of aknfepctrlbasewindow.h
*
*/


inline CControlGroup* CAknFepCtrlBaseWindow::HeaderPane() const
    {
    return iHeaderPane;
    }
        
inline CControlGroup* CAknFepCtrlBaseWindow::ClientPane() const
    {
    return iClientPane;
    }

inline CControlGroup* CAknFepCtrlBaseWindow::TitleBar() const
    {
    return iTitleBar;
    }

inline CAknFepCtrlDragButton* CAknFepCtrlBaseWindow::MoveButton() const
    {
    return iMoveButton;
    }

inline CAknFepCtrlCommonButton* CAknFepCtrlBaseWindow::CloseButton() const
    {
    return iCloseButton;
    }

// End Of File
