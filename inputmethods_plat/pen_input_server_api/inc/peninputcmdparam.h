/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               Peninput command data structure
*
*/















#ifndef _PENINPUTPARAM_H
#define _PENINPUTPARAM_H

#include <frmtlay.h>
#include <badesca.h>

enum TPenICFCmd
    {
    EPeninputICFInitial,
    EPeninputICFDelete,
    EPeninputICFReplace,
    EPeninputICFSetCurSel
    };    

enum TFepInputContextFieldDataFlag
    {
    EFepICFDataInValid,
    EFepICFDataInlineNoMatch,
    EFepICFDataDirectionLTR,
    EFepICFDataDirectionRTL,
    EFepICFDataDirectionMFNE
    };
enum TPeninputAppInfo
    {
    EAppTitle,
    EAppEditorPrompt,
    EAppIndicatorMsg,
    /* Teleca change begin, 19.05.2009 ssal */
    EAppByteWarningMsg,
    /* Teleca change end, 19.05.2009 ssal */
    /* Teleca change begin, 09.09.2009 ssal */
    EAppMessageTypeChangeMsg
    /* Teleca change begin, 9.09.2009 ssal */
    };
        
//structure for input context field data
struct TFepInputContextFieldData
    {    
    TPenICFCmd iCmd;
    TCursorSelection iCurSel; //current cursor selection 
    TPtrC iText; // The text sent to input context field
    TInt iStartPos;
    TInt iLength;
    TInt iMidPos;
    TBool iCursorVisibility; // Cursor visibility flag, indicates whether cursor need be shown in input context field.
    TBool iCursorSelVisible; // The flag of the cursor if can be moved
    TRect iRect; // Stands for current cursor area in screen coordinate
    TFepInputContextFieldDataFlag iFlag;
    TBool iLeadingEdge;

    /**
     * The unique handle for current text content. Each editor has different handle.
     */    
    TInt iContentHandle;
    
    /**
     * Reservered field, for future extension
     */
    TInt iReservered; 
    
    TFepInputContextFieldData(TInt aHandle = -1)
        :iMidPos(-1), iFlag(EFepICFDataInValid), iContentHandle(aHandle),iReservered(0)
        {
        }

    };
    
struct TFepInputCandidateList
    {
    TPtrC iCandidate[3];
    };
struct TFepInputAllCandidates
    {
    RArray<TPtrC> iCandidates;
    };
struct TFepIndicatorInfo
    {
    TInt iIndicatorImgID;
    TInt iIndicatorMaskID;
    TInt iIndicatorTextImgID;
    TInt iIndicatorTextMaskID;
    };

struct TFepPromptText
    {
    TInt iLines;
    TPtrC iText;
    TBool iCleanContent;
    };
    
/**
 * ITI tooltip text data
 */
struct TFepITITooltipText
    {
    TInt iDataSize;
    TPtrC iText;    
    };

/**
 * ITI candidate list data
 */
struct TFepITICandidateList
    {
    CDesCArray* iItemArray;
    TInt iActiveIndex;
    };

class CPtiEngine;
class CCoeControl;
struct TPeninputLayoutData
{
	CPtiEngine* iPtiEngine;
	CCoeControl* iPenUiWnd;
};
struct TFepSymbolOfHardwareOne
	{
    TInt iSymbol1;
    TInt iSymbol2;
	TInt iSymbol3;
	};
#endif