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
* Description:  Inline function for edit area base
*
*/


inline TCursorSelection CFepLayoutEditAreaBase::Selection() const
    {
    return iSelectedCompositionText;
    }

inline TInt CFepLayoutEditAreaBase::TextLength() const
    {
    return (iBuffer ? iBuffer->Length() : 0);
    }

inline TBool CFepLayoutEditAreaBase::IsDimmed() const
    {
    return iDimmed;
    }
    
inline void CFepLayoutEditAreaBase::SetTextIsSecret( TBool aIsSecret )
    {
    iTextIsSecret = aIsSecret;
    }    
    
inline TBool CFepLayoutEditAreaBase::TextIsSecret() const
    {
    return iTextIsSecret;
    }   
