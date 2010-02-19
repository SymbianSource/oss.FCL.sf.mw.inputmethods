/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
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
	
#ifndef PTIKOREANQWERTYCOREDEBUG_H_
#define PTIKOREANQWERTYCOREDEBUG_H_

#ifdef _DEBUG
#include <e32svr.h>
    #if __WINS__
        #define LOG_ASSERT(a) RDebug::Print(_L("PKQC: "a)); __ASSERT_DEBUG(EFalse,User::Panic(_L("PKQC: Not Implemented"),__LINE__))
        #define LOG(a) RDebug::Print(_L("PKQC: "a))
        #define LOG1(a,b) RDebug::Print(_L("PKQC: "a),b)
        #define LOG2(a,b,c) RDebug::Print(_L("PKQC: "a),b,c)
        #define LOG3(a,b,c,d) RDebug::Print(_L("PKQC: "a),b,c,d)
        #define LOG4(a,b,c,d,e) RDebug::Print(_L("PKQC: "a),b,c,d,e)
    #else
        #include <flogger.h>
        _LIT(KLogFile,"PKQC.txt");
        _LIT(KLogFolder,"PKQC");
        #define LOG_ASSERT(a) RFileLogger::Write(KLogFolder(),KLogFile(),EFileLoggingModeAppend,_L(a)); __ASSERT_DEBUG(EFalse,User::Panic(_L("PKQC: Not Implemented"),__LINE__))
        #define LOG(a) RFileLogger::Write(KLogFolder(),KLogFile(),EFileLoggingModeAppend,_L(a))
        #define LOG1(a,b) RFileLogger::WriteFormat(KLogFolder(),KLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(a)),b)
        #define LOG2(a,b,c) RFileLogger::WriteFormat(KLogFolder(),KLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_S16(a)),b,c)
        #define LOG3(a,b,c,d) RFileLogger::WriteFormat(KLogFolder(),KLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(a)),b,c,d)
        #define LOG4(a,b,c,d,e) RFileLogger::WriteFormat(KLogFolder(),KLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(_L(a)),b,c,d,e)
    #endif
#else
    #define LOG_ASSERT(a)
    #define LOG(a)
    #define LOG1(a,b)
    #define LOG2(a,b,c)
    #define LOG3(a,b,c,d)
    #define LOG4(a,b,c,d,e)
#endif

#endif /* PTIKOREANQWERTYCOREDEBUG_H_ */
