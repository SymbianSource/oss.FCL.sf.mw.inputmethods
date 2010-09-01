/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline function for CFepUiLayout
*
*/


// -----------------------------------------------------------------------------
// get control list for this group
// -----------------------------------------------------------------------------
//    
inline const RPointerArray<CFepUiBaseCtrl>& CControlGroup::ControlList() const
    {
    return iCtrlList;
    }
    
inline const RPointerArray<CFepUiBaseCtrl>& CControlGroup::PopCtrlList() const
    {
    return iPopCtrlList;
    }

inline TBool CControlGroup::NeedRedrawBg() const
    {
    return iNeedRedrawBg;
    }

inline void CControlGroup::SetNeedRedrawBg(TBool aNeedRedrawBg)
    {
    iNeedRedrawBg = aNeedRedrawBg;
    }

//end of file    
