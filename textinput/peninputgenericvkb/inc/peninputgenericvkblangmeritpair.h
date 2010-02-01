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
class TLangMeritPair 
    {
public:
    TText iDeadKey;
    TText iChar;
    TText iAccentedChar;
    };
    
const TLangMeritPair KSupportLanguages[] = 
    {
        {0x005E, 0x0041, 0x00C2},// ^ + A
        {0x005E, 0x0061, 0x00E2},// ^ + a
        {0x005E, 0x0045, 0x00CA},// ^ + E
        {0x005E, 0x0065, 0x00EA},// ^ + e
        {0x005E, 0x0049, 0x00CE},// ^ + I
        {0x005E, 0x0069, 0x00EE},// ^ + i
        {0x005E, 0x004F, 0x00D4},// ^ + O
        {0x005E, 0x006F, 0x00F4},// ^ + o
        {0x005E, 0x0055, 0x00DB},// ^ + U
        {0x005E, 0x0075, 0x00FB},// ^ + u
        {0x0060, 0x0041, 0x00C0},// \ + A
        {0x0060, 0x0061, 0x00E0},// \ + a
        {0x0060, 0x0045, 0x00C8},// \ + E
        {0x0060, 0x0065, 0x00E8},// \ + e
        {0x0060, 0x0049, 0x00CC},// \ + I
        {0x0060, 0x0069, 0x00EC},// \ + i
        {0x0060, 0x004F, 0x00D2},// \ + O
        {0x0060, 0x006F, 0x00F2},// \ + o
        {0x0060, 0x0055, 0x00D9},// \ + U
        {0x0060, 0x0075, 0x00F9},// \ + u
        {0x00A8, 0x0045, 0x00CB},// .. + E
        {0x00A8, 0x0065, 0x00EB},// .. + e
        {0x00A8, 0x0049, 0x00CF},// .. + I
        {0x00A8, 0x0069, 0x00EF},// .. + i
        {0x00A8, 0x006F, 0x00F6},// .. + o
        {0x00A8, 0x004F, 0x00D6},// .. + O
        {0x00A8, 0x0055, 0x00DC},// .. + u
        {0x00A8, 0x0075, 0x00FC},// .. + U
        {0x00A8, 0x0059, 0x0178},// .. + Y
        {0x00A8, 0x0079, 0x00FF},// .. + y
        {0x007E, 0x0041, 0x00C3},// ~ + A
        {0x007E, 0x0061, 0x00E3},// ~ + a
        {0x007E, 0x004F, 0x00D5},// ~ + O
        {0x007E, 0x006F, 0x00F5},// ~ + o
        {0x00AF, 0x0041, 0x0100},// - + A
        {0x00AF, 0x0061, 0x0101},// - + a
        {0x00AF, 0x0045, 0x0112},// - + E
        {0x00AF, 0x0065, 0x0113},// - + e
        {0x00AF, 0x0049, 0x012A},// - + I
        {0x00AF, 0x0069, 0x012B},// - + i
        {0x00AF, 0x0055, 0x016A},// - + U
        {0x00AF, 0x0075, 0x016B},// - + u
        {0x00B4, 0x0041, 0x00C1},// / + A
        {0x00B4, 0x0061, 0x00E1},// / + a
        {0x00B4, 0x0043, 0x0106},// / + C
        {0x00B4, 0x0063, 0x0107},// / + c
        {0x00B4, 0x0045, 0x00C9},// / + E
        {0x00B4, 0x0065, 0x00E9},// / + e
        {0x00B4, 0x0049, 0x00CD},// / + I
        {0x00B4, 0x0069, 0x00ED},// / + i
        {0x00B4, 0x004C, 0x0139},// / + L
        {0x00B4, 0x006C, 0x013A},// / + l
        {0x00B4, 0x004E, 0x0143},// / + N
        {0x00B4, 0x006E, 0x0144},// / + n
        {0x00B4, 0x004F, 0x00D3},// / + O
        {0x00B4, 0x006F, 0x00F3},// / + o
        {0x00B4, 0x0052, 0x0154},// / + R
        {0x00B4, 0x0072, 0x0155},// / + r
        {0x00B4, 0x0053, 0x015A},// / + S
        {0x00B4, 0x0073, 0x015B},// / + s
        {0x00B4, 0x0055, 0x00DA},// / + U
        {0x00B4, 0x0075, 0x00FA},// / + u
        {0x00B4, 0x0059, 0x00DD},// / + Y
        {0x00B4, 0x0079, 0x00FD},// / + y
        {0x00B4, 0x005A, 0x0179},// / + Z
        {0x00B4, 0x007A, 0x017A},// / + z
        {0x00B8, 0x0047, 0x0122},// , + G 
        {0x00B8, 0x0067, 0x0123},// , + g
        {0x00B8, 0x004B, 0x0136},// , + K
        {0x00B8, 0x006B, 0x0137},// , + k
        {0x00B8, 0x004C, 0x013B},// , + L
        {0x00B8, 0x006C, 0x013C},// , + l
        {0x00B8, 0x004E, 0x0145},// , + N
        {0x00B8, 0x006E, 0x0146},// , + n
        {0x00B8, 0x0052, 0x0156},// , + R
        {0x00B8, 0x0072, 0x0157},// , + r
        {0x02C7, 0x0043, 0x010C},// ^ + C
        {0x02C7, 0x0063, 0x010D},// ^ + c
        {0x02C7, 0x0044, 0x010E},// ^ + D
        {0x02C7, 0x0064, 0x010F},// ^ + d
        {0x02C7, 0x0045, 0x011A},// ^ + E
        {0x02C7, 0x0065, 0x011B},// ^ + e
        {0x02C7, 0x004E, 0x0147},// ^ + N
        {0x02C7, 0x006E, 0x0148},// ^ + n
        {0x02C7, 0x0052, 0x0158},// ^ + R
        {0x02C7, 0x0072, 0x0159},// ^ + r
        {0x02C7, 0x0053, 0x0160},// ^ + S
        {0x02C7, 0x0073, 0x0161},// ^ + s
        {0x02C7, 0x0054, 0x0164},// ^ + T
        {0x02C7, 0x0074, 0x0165},// ^ + t
        {0x02C7, 0x005A, 0x017D},// ^ + Z
        {0x02C7, 0x007A, 0x017E},// ^ + z
        {0x02DB, 0x0041, 0x0104},// \ + A
        {0x02DB, 0x0061, 0x0105},//   + a
        {0x02DB, 0x0045, 0x0118},//   + E
        {0x02DB, 0x0065, 0x0119},//   + e
        {0x02DB, 0x0049, 0x012E},//   + I
        {0x02DB, 0x0069, 0x012F},//   + i
        {0x02DB, 0x0055, 0x0172},//   + U
        {0x02DB, 0x0075, 0x0173},//   + u
        {0x02DD, 0x004F, 0x0150},// ~ + O
        {0x02DD, 0x006F, 0x0151},// ~ + o
        {0x02DD, 0x0055, 0x0170},// ~ + U
        {0x02DD, 0x0075, 0x0171}, // ~ + u
        {0x00A8, 0x0399, 0x03AA},// .. + ��
        {0x00A8, 0x03B9, 0x03CA},// .. + ��
        {0x00A8, 0x03A5, 0x03AB},// .. + Y
        {0x00A8, 0x03C5, 0x03CB},// .. + u
        {0x0384, 0x0391, 0x0386},
        {0x0384, 0x03B1, 0x03AC},
        {0x0384, 0x0395, 0x0388},
        {0x0384, 0x03B5, 0x03AD},
        {0x0384, 0x0397, 0x0389},
        {0x0384, 0x03B7, 0x03AE},
        {0x0384, 0x0399, 0x038A},
        {0x0384, 0x03B9, 0x03AF},
        {0x0384, 0x039F, 0x038C},
        {0x0384, 0x03BF, 0x03CC},
        {0x0384, 0x03A5, 0x038E},
        {0x0384, 0x03C5, 0x03CD},
        {0x0384, 0x03A9, 0x038F},
        {0x0384, 0x03C9, 0x03CE},
        {0x0385, 0x03B9, 0x0390},
        {0x0385, 0x03C5, 0x03B0}
    };
