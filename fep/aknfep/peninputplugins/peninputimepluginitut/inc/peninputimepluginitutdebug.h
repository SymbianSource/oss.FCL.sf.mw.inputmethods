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
* Description:            Debug utils API
*
*/












#ifndef PENINPUTIMEPLUGINCHSDEBUG_H
#define PENINPUTIMEPLUGINCHSDEBUG_H
 
#ifdef _DEBUG
    #define PRINT( txt )                    RDebug::Print( txt );
    #define PRINTF( txt )                   RDebug::Print txt;
    #define PRINT1( txt, arg1 )             RDebug::Print( txt, arg1 );
    #define PRINT2( txt, arg1, arg2 )       RDebug::Print( txt, arg1, arg2 );
    #define PRINT3( txt, arg1, arg2, arg3 ) RDebug::Print( txt, arg1, arg2, arg3 ); 
#else 
    #define PRINT( txt ) 
    #define PRINTF( txt )                    
    #define PRINT1( txt, arg1 ) 
    #define PRINT2( txt, arg1, arg2 ) 
    #define PRINT3( txt, arg1, arg2, arg3 ) 
#endif


// Record into log files
#define USE_FILE_LOGGER

#if defined (_DEBUG) && !defined(USE_FILE_LOGGER)                   
    #define USE_SERIAL_LOGGER 
#endif
                                    
#if defined (USE_FILE_LOGGER)
    #include <flogger.h>
    _LIT(KLogFile,"imepluginchn.txt");
    _LIT(KLogFolder,"peninputime");
    #define LOGTEXT(AAA)                RFileLogger::Write(KLogFolder(), \
                                                           KLogFile(), \
                                                           EFileLoggingModeAppend, \
                                                           AAA)
    #define LOGTEXT2(AAA,BBB)           RFileLogger::WriteFormat(KLogFolder(), \
                                                                 KLogFile(), \
                                                                 EFileLoggingModeAppend, \
                                                                 TRefByValue<const TDesC>(AAA), \
                                                                 BBB)
    #define LOGTEXT3(AAA,BBB,CCC)       RFileLogger::WriteFormat(KLogFolder(), \
                                                                 KLogFile(), \
                                                                 EFileLoggingModeAppend, \
                                                                 TRefByValue<const TDesC>(AAA), \
                                                                 BBB, \
                                                                 CCC)
    #define LOGHEXDUMP(AAA,BBB,CCC,DDD) RFileLogger::HexDump(KLogFolder(), \
                                                             KLogFile(), \
                                                             EFileLoggingModeAppend, \
                                                             AAA,\
                                                             BBB,\
                                                             CCC,\
                                                             DDD)    
#elif defined (USE_SERIAL_LOGGER) 
    #include <e32svr.h>
    #define LOGTEXT(AAA)                RDebug::Print(AAA)
    #define LOGTEXT2(AAA,BBB)           RDebug::Print(AAA,BBB)
    #define LOGTEXT3(AAA,BBB,CCC)       RDebug::Print(AAA,BBB,CCC)
    #define LOGHEXDUMP(AAA,BBB,CCC,DDD)
#else
    #define LOGTEXT(AAA)                
    #define LOGTEXT2(AAA,BBB)           
    #define LOGTEXT3(AAA,BBB,CCC)       
    #define LOGHEXDUMP(AAA,BBB,CCC,DDD) 
#endif      

#endif      // PENINPUTIMEPLUGINCHSDEBUG   
            
// End Of File
