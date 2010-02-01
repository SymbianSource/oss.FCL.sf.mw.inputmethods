@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem

testexecute.exe z:\fepbasetest\fepbasetest_t_appmorse.script
testexecute.exe z:\fepbasetest\fepbasetest_t_defocusingedwin.script
testexecute.exe z:\fepbasetest\fepbasetest_t_fepgen.script
testexecute.exe z:\fepbasetest\fepbasetest_t_fep1testtarget.script
testexecute.exe z:\fepbasetest\fepbasetest_t_keyboardlogger.script


:: Ignore the following if using this script for emulator testing.
:: Logs are automatically copied on hardware to MMC
:: which is assumed to be drive E:
@echo off
if not exist c:\logs\testexecute\ goto :EOF
md e:\logs
md e:\logs\testexecute
copy c:\logs\testexecute\fepbasetest*.htm e:\logs\testexecute\