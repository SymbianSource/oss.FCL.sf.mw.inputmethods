/**
 * @file    hangulctype.c
 * @brief   hangulctype source file
 */

/* libhangul
 * Copyright (c) 2005,2006 Choe Hwanjin
 * All rights reserved.
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "hangul.h"

static const ucschar syllable_base  = 0xac00;
static const ucschar choseong_base  = 0x1100;
static const ucschar jungseong_base = 0x1161;
static const ucschar jongseong_base = 0x11a7;
static const int njungseong = 21;
static const int njongseong = 28;

/**
 * @brief check for a choseong
 * @param c ucs4 code value
 * @return true if the character c falls into choseong class
 *
 * This function check whether c, which must have ucs4 value, falls into
 * choseong (leading consonants) class.
 */
bool
hangul_is_choseong(ucschar c)
{
    return c >= 0x1100 && c <= 0x1159;
}

/**
 * @brief check for a jungseong
 * @param c ucs4 code value
 * @return true if the character c falls into jungseong class
 *
 * This function check whether c, which must have ucs4 value, falls into
 * jungseong (vowels) class.
 */
bool
hangul_is_jungseong(ucschar c)
{
    return c >= 0x1161 && c <= 0x11a2;
}

/**
 * @brief check for a jongseong
 * @param c ucs4 code value
 * @return true if the character c falls into jongseong class
 *
 * This function check whether c, which must have ucs4 value, falls into
 * jongseong (trailing consonants) class.
 */
bool
hangul_is_jongseong(ucschar c)
{
    return c >= 0x11a8 && c <= 0x11f9;
}

bool
hangul_is_combining_mark(ucschar c)
{
    return  c == 0x302e || c == 0x302f  ||
	   (c >= 0x0300 && c <= 0x036F) ||
	   (c >= 0x1dc0 && c <= 0x1dff) ||
	   (c >= 0xfe20 && c <= 0xfe2f);
}

bool
hangul_is_choseong_conjoinable(ucschar c)
{
    return c >= 0x1100 && c <= 0x1112;
}

bool
hangul_is_jungseong_conjoinable(ucschar c)
{
    return c >= 0x1161 && c <= 0x1175;
}

bool
hangul_is_jongseong_conjoinable(ucschar c)
{
    return c >= 0x11a7 && c <= 0x11c2;
}

/**
 * @brief check for a syllable
 * @param c ucs4 code value
 * @return true if the character c falls into syllable class
 *
 * This function check whether c, which must have ucs4 value, falls into
 * syllable class; that is from U+AC00 to 0xD7A3.
 */
bool
hangul_is_syllable(ucschar c)
{
    return c >= 0xac00 && c <= 0xd7a3;
}

/**
 * @brief check for a jaso
 * @param c ucs4 code value
 * @return true if the character c falls into jaso class
 *
 * This function check whether c, which must have ucs4 value, falls into
 * jaso class; that is choseong, jungseong or jongseong.
 */
bool
hangul_is_jaso(ucschar c)
{
    return hangul_is_choseong(c) ||
	   hangul_is_jungseong(c) ||
	   hangul_is_jongseong(c);
}

/**
 * @brief check for a compatibility jamo
 * @param c ucs4 code value
 * @return true if the character c falls into compatibility class
 *
 * This function check whether c, which must have ucs4 value, falls into
 * compatibility jamo class.
 */
bool
hangul_is_jamo(ucschar c)
{
    return c >= 0x3131 && c <= 0x318e;
}

/**
 * @brief convert a jaso to the compatibility jamo
 * @param c ucs4 code value
 * @return converted value, or c
 *
 * This function converts the jaso c, which must have ucs4 value, to
 * comaptibility jamo or c if the conversion is failed
 */
ucschar
hangul_jaso_to_jamo(ucschar c)
{
    static
#ifdef __SYMBIAN32__
    const 
#endif
    ucschar choseong[] = {
	0x3131,	    /* 0x1100 */
	0x3132,	    /* 0x1101 */
	0x3134,	    /* 0x1102 */
	0x3137,	    /* 0x1103 */
	0x3138,	    /* 0x1104 */
	0x3139,	    /* 0x1105 */
	0x3141,	    /* 0x1106 */
	0x3142,	    /* 0x1107 */
	0x3143,	    /* 0x1108 */
	0x3145,	    /* 0x1109 */
	0x3146,	    /* 0x110a */
	0x3147,	    /* 0x110b */
	0x3148,	    /* 0x110c */
	0x3149,	    /* 0x110d */
	0x314a,	    /* 0x110e */
	0x314b,	    /* 0x110f */
	0x314c,	    /* 0x1110 */
	0x314d,	    /* 0x1111 */
	0x314e,	    /* 0x1112 */
    };

    static 
#ifdef __SYMBIAN32__
    const 
#endif
    ucschar jungseong[] = {
	0x314f,	    /* 0x1161 */
	0x3150,	    /* 0x1162 */
	0x3151,	    /* 0x1163 */
	0x3152,	    /* 0x1164 */
	0x3153,	    /* 0x1165 */
	0x3154,	    /* 0x1166 */
	0x3155,	    /* 0x1167 */
	0x3156,	    /* 0x1168 */
	0x3157,	    /* 0x1169 */
	0x3158,	    /* 0x116a */
	0x3159,	    /* 0x116b */
	0x315a,	    /* 0x116c */
	0x315b,	    /* 0x116d */
	0x315c,	    /* 0x116e */
	0x315d,	    /* 0x116f */
	0x315e,	    /* 0x1170 */
	0x315f,	    /* 0x1171 */
	0x3160,	    /* 0x1172 */
	0x3161,	    /* 0x1173 */
	0x3162,	    /* 0x1174 */
	0x3163	    /* 0x1175 */
    };

    static 
#ifdef __SYMBIAN32__
    const 
#endif
    ucschar jongseong[] = {
	0x3131,	    /* 0x11a8 */
	0x3132,	    /* 0x11a9 */
	0x3133,	    /* 0x11aa */
	0x3134,	    /* 0x11ab */
	0x3135,	    /* 0x11ac */
	0x3136,	    /* 0x11ad */
	0x3137,	    /* 0x11ae */
	0x3139,	    /* 0x11af */
	0x313a,	    /* 0x11b0 */
	0x313b,	    /* 0x11b1 */
	0x313c,	    /* 0x11b2 */
	0x313d,	    /* 0x11b3 */
	0x313e,	    /* 0x11b4 */
	0x313f,	    /* 0x11b5 */
	0x3140,	    /* 0x11b6 */
	0x3141,	    /* 0x11b7 */
	0x3142,	    /* 0x11b8 */
	0x3144,	    /* 0x11b9 */
	0x3145,	    /* 0x11ba */
	0x3146,	    /* 0x11bb */
	0x3147,	    /* 0x11bc */
	0x3148,	    /* 0x11bd */
	0x314a,	    /* 0x11be */
	0x314b,	    /* 0x11bf */
	0x314c,	    /* 0x11c0 */
	0x314d,	    /* 0x11c1 */
	0x314e	    /* 0x11c2 */
    };

    if (c >= 0x1100 && c <= 0x1112) {
	return choseong[c - 0x1100];
    } else if (c >= 0x1161 && c <= 0x1175) {
	return jungseong[c - 0x1161];
    } else if (c >= 0x11a8 && c <= 0x11c2) {
	return jongseong[c - 0x11a8];
    }

    return c;
}

ucschar
hangul_choseong_to_jongseong(ucschar c)
{
    static 
#ifdef __SYMBIAN32__
    const 
#endif
    ucschar table[] = {
	0x11a8,  /* choseong kiyeok      -> jongseong kiyeok      */
	0x11a9,  /* choseong ssangkiyeok -> jongseong ssangkiyeok */
	0x11ab,  /* choseong nieun       -> jongseong nieun       */
	0x11ae,  /* choseong tikeut      -> jongseong tikeut      */
	0x0,     /* choseong ssangtikeut -> jongseong tikeut      */
	0x11af,  /* choseong rieul       -> jongseong rieul       */
	0x11b7,  /* choseong mieum       -> jongseong mieum       */
	0x11b8,  /* choseong pieup       -> jongseong pieup       */
	0x0,     /* choseong ssangpieup  -> jongseong pieup       */
	0x11ba,  /* choseong sios        -> jongseong sios        */
	0x11bb,  /* choseong ssangsios   -> jongseong ssangsios   */
	0x11bc,  /* choseong ieung       -> jongseong ieung       */
	0x11bd,  /* choseong cieuc       -> jongseong cieuc       */
	0x0,     /* choseong ssangcieuc  -> jongseong cieuc       */
	0x11be,  /* choseong chieuch     -> jongseong chieuch     */
	0x11bf,  /* choseong khieukh     -> jongseong khieukh     */
	0x11c0,  /* choseong thieuth     -> jongseong thieuth     */
	0x11c1,  /* choseong phieuph     -> jongseong phieuph     */
	0x11c2   /* choseong hieuh       -> jongseong hieuh       */
    };
    if (c < 0x1100 || c > 0x1112)
	return 0;
    return table[c - 0x1100];
}

ucschar
hangul_jongseong_to_choseong(ucschar c)
{
    static 
#ifdef __SYMBIAN32__
    const 
#endif
    ucschar table[] = {
      0x1100,  /* jongseong kiyeok        -> choseong kiyeok       */
      0x1101,  /* jongseong ssangkiyeok   -> choseong ssangkiyeok  */
      0x1109,  /* jongseong kiyeok-sios   -> choseong sios         */
      0x1102,  /* jongseong nieun         -> choseong nieun        */
      0x110c,  /* jongseong nieun-cieuc   -> choseong cieuc        */
      0x1112,  /* jongseong nieun-hieuh   -> choseong hieuh        */
      0x1103,  /* jongseong tikeut        -> choseong tikeut       */
      0x1105,  /* jongseong rieul         -> choseong rieul        */
      0x1100,  /* jongseong rieul-kiyeok  -> choseong kiyeok       */
      0x1106,  /* jongseong rieul-mieum   -> choseong mieum        */
      0x1107,  /* jongseong rieul-pieup   -> choseong pieup        */
      0x1109,  /* jongseong rieul-sios    -> choseong sios         */
      0x1110,  /* jongseong rieul-thieuth -> choseong thieuth      */
      0x1111,  /* jongseong rieul-phieuph -> choseong phieuph      */
      0x1112,  /* jongseong rieul-hieuh   -> choseong hieuh        */
      0x1106,  /* jongseong mieum         -> choseong mieum        */
      0x1107,  /* jongseong pieup         -> choseong pieup        */
      0x1109,  /* jongseong pieup-sios    -> choseong sios         */
      0x1109,  /* jongseong sios          -> choseong sios         */
      0x110a,  /* jongseong ssangsios     -> choseong ssangsios    */
      0x110b,  /* jongseong ieung         -> choseong ieung        */
      0x110c,  /* jongseong cieuc         -> choseong cieuc        */
      0x110e,  /* jongseong chieuch       -> choseong chieuch      */
      0x110f,  /* jongseong khieukh       -> choseong khieukh      */
      0x1110,  /* jongseong thieuth       -> choseong thieuth      */
      0x1111,  /* jongseong phieuph       -> choseong phieuph      */
      0x1112   /* jongseong hieuh         -> choseong hieuh        */
    };
    if (c < 0x11a8 || c > 0x11c2)
	return 0;
    return table[c - 0x11a8];
}

void
hangul_jongseong_dicompose(ucschar c, ucschar* jong, ucschar* cho)
{
    static 
#ifdef __SYMBIAN32__
    const 
#endif
    ucschar table[][2] = {
    { 0,      0x1100 }, /* jong kiyeok	      = cho  kiyeok               */
    { 0x11a8, 0x1100 }, /* jong ssangkiyeok   = jong kiyeok + cho kiyeok  */
    { 0x11a8, 0x1109 }, /* jong kiyeok-sios   = jong kiyeok + cho sios    */
    { 0,      0x1102 }, /* jong nieun	      = cho  nieun                */
    { 0x11ab, 0x110c }, /* jong nieun-cieuc   = jong nieun  + cho cieuc   */
    { 0x11ab, 0x1112 }, /* jong nieun-hieuh   = jong nieun  + cho hieuh   */
    { 0,      0x1103 }, /* jong tikeut	      = cho  tikeut               */
    { 0,      0x1105 }, /* jong rieul         = cho  rieul                */
    { 0x11af, 0x1100 }, /* jong rieul-kiyeok  = jong rieul  + cho kiyeok  */
    { 0x11af, 0x1106 }, /* jong rieul-mieum   = jong rieul  + cho mieum   */
    { 0x11af, 0x1107 }, /* jong rieul-pieup   = jong rieul  + cho pieup   */
    { 0x11af, 0x1109 }, /* jong rieul-sios    = jong rieul  + cho sios    */
    { 0x11af, 0x1110 }, /* jong rieul-thieuth = jong rieul  + cho thieuth */
    { 0x11af, 0x1111 }, /* jong rieul-phieuph = jong rieul  + cho phieuph */
    { 0x11af, 0x1112 }, /* jong rieul-hieuh   = jong rieul  + cho hieuh   */
    { 0,      0x1106 }, /* jong mieum         = cho  mieum                */
    { 0,      0x1107 }, /* jong pieup         = cho  pieup                */
    { 0x11b8, 0x1109 }, /* jong pieup-sios    = jong pieup  + cho sios    */
    { 0,      0x1109 }, /* jong sios          = cho  sios                 */
    { 0x11ba, 0x1109 }, /* jong ssangsios     = jong sios   + cho sios    */
    { 0,      0x110b }, /* jong ieung         = cho  ieung                */
    { 0,      0x110c }, /* jong cieuc         = cho  cieuc                */
    { 0,      0x110e }, /* jong chieuch       = cho  chieuch              */
    { 0,      0x110f }, /* jong khieukh       = cho  khieukh              */
    { 0,      0x1110 }, /* jong thieuth       = cho  thieuth              */
    { 0,      0x1111 }, /* jong phieuph       = cho  phieuph              */
    { 0,      0x1112 }  /* jong hieuh         = cho  hieuh                */
    };

    *jong = table[c - 0x11a8][0];
    *cho  = table[c - 0x11a8][1];
}

/**
 * @brief compose a hangul syllable
 * @param choseong UCS4 code value
 * @param jungseong UCS4 code value
 * @param jongseong UCS4 code value
 * @return syllable code compose from choseong, jungseong and jongseong
 *
 * This function compose hangul jaso choseong, jungseong and jongseong and
 * return the syllable code.
 */
ucschar
hangul_jaso_to_syllable(ucschar choseong, ucschar jungseong, ucschar jongseong)
{
    ucschar c;

    /* we use 0x11a7 like a Jongseong filler */
    if (jongseong == 0)
	jongseong = 0x11a7;         /* Jongseong filler */

    if (!hangul_is_choseong_conjoinable(choseong))
	return 0;
    if (!hangul_is_jungseong_conjoinable(jungseong))
	return 0;
    if (!hangul_is_jongseong_conjoinable(jongseong))
	return 0;

    choseong  -= choseong_base;
    jungseong -= jungseong_base;
    jongseong -= jongseong_base;

    c = ((choseong * njungseong) + jungseong) * njongseong + jongseong
	+ syllable_base;
    return c;
}

void
hangul_syllable_to_jaso(ucschar syllable,
			ucschar* choseong,
			ucschar* jungseong,
			ucschar* jongseong)
{
    if (jongseong != NULL)
	*jongseong = 0;
    if (jungseong != NULL)
	*jungseong = 0;
    if (choseong != NULL)
	*choseong = 0;

    if (!hangul_is_syllable(syllable))
	return;

    syllable -= syllable_base;
    if (jongseong != NULL) {
	if (syllable % njongseong != 0)
	    *jongseong = jongseong_base + syllable % njongseong;
    }
    syllable /= njongseong;

    if (jungseong != NULL) {
	*jungseong = jungseong_base + syllable % njungseong;
    }
    syllable /= njungseong;

    if (choseong != NULL) {
	*choseong = choseong_base + syllable;
    }
}

static
#ifndef __SYMBIAN32__
inline
#endif
bool 
is_syllable_boundary(ucschar prev, ucschar next)
{
    if (hangul_is_choseong(prev)) {
	if (hangul_is_choseong(next))
	    return false;
	if (hangul_is_jungseong(next))
	    return false;
	if (hangul_is_syllable(next))
	    return false;
	if (hangul_is_combining_mark(next))
	    return false;
	if (next == HANGUL_JUNGSEONG_FILLER)
	    return false;
    } else if (prev == HANGUL_CHOSEONG_FILLER) {
	if (hangul_is_jungseong(next))
	    return false;
	if (next == HANGUL_JUNGSEONG_FILLER)
	    return false;
    } else if (hangul_is_jungseong(prev)) {
	if (hangul_is_jungseong(next))
	    return false;
	if (hangul_is_jongseong(next))
	    return false;
	if (hangul_is_combining_mark(next))
	    return false;
    } else if (prev == HANGUL_JUNGSEONG_FILLER) {
	if (hangul_is_jongseong(next))
	    return false;
    } else if (hangul_is_jongseong(prev)) {
	if (hangul_is_jongseong(next))
	    return false;
	if (hangul_is_combining_mark(next))
	    return false;
    } else if (hangul_is_syllable(prev)) {
	if ((prev - syllable_base) % njongseong == 0) {
	    // 醫낆꽦�씠 �뾾�뒗 �쓬�젅: LV
	    if (hangul_is_jungseong(next))
		return false;
	    if (hangul_is_jongseong(next))
		return false;
	} else {
	    // 醫낆꽦�씠 �엳�뒗 �쓬�젅: LVT
	    if (hangul_is_jongseong(next))
		return false;
	}
	if (hangul_is_combining_mark(next))
	    return false;
    }
    
    return true;
}

static 
#ifndef __SYMBIAN32__
inline 
#endif
ucschar
choseong_compress(ucschar a, ucschar b)
{
    if (a == 0)
	return b;

    if (a == 0x1100 && b == 0x1100)
	return 0x1101;
    if (a == 0x1103 && b == 0x1103)
	return 0x1104;
    if (a == 0x1107 && b == 0x1107)
	return 0x1108;
    if (a == 0x1109 && b == 0x1109)
	return 0x110A;
    if (a == 0x110c && b == 0x110c)
	return 0x110d;
    return 0;
}

static 
#ifndef __SYMBIAN32__
inline 
#endif
ucschar
jungseong_compress(ucschar a, ucschar b)
{
    if (a == 0)
	return b;

    if (a == 0x1169) {
	if (b == 0x1161)
	    return 0x116a;
	if (b == 0x1162)
	    return 0x116b;
	if (b == 0x1175)
	    return 0x116c;
    }
    if (a == 0x116e) {
	if (b == 0x1165)
	    return 0x116f;
	if (b == 0x1166)
	    return 0x1170;
	if (b == 0x1175)
	    return 0x1171;
    }
    if (b == 0x1175) {
	if (a == 0x1173)
	    return 0x1174;
	if (a == 0x1161)
	    return 0x1162;
	if (a == 0x1163)
	    return 0x1164;
	if (a == 0x1165)
	    return 0x1166;
	if (a == 0x1167)
	    return 0x1168;
    }

    return 0;
}

static 
#ifndef __SYMBIAN32__
inline 
#endif
ucschar
jongseong_compress(ucschar a, ucschar b)
{
    if (a == 0)
	return b;
    
    if (a == 0x11a8) {
	if (b == 0x11a8)
	    return 0x11a9;
	if (b == 0x11ba)
	    return 0x11aa;
    }
    if (a == 0x11ab) {
	if (b == 0x11b0)
	    return 0x11ab;
	if (b == 0x11c2)
	    return 0x11ad;
    }
    if (a == 0x11af) {
	if (b == 0x11a8)
	    return 0x11b0;
	if (b == 0x11b7)
	    return 0x11b1;
	if (b == 0x11b8)
	    return 0x11b2;
	if (b == 0x11ba)
	    return 0x11b3;
	if (b == 0x11c0)
	    return 0x11b4;
	if (b == 0x11c1)
	    return 0x11b5;
	if (b == 0x11c2)
	    return 0x11b6;
    }
    if (a == 0x11b8 && b == 0x11ba)
	return 0x11b9;
    if (a == 0x11ba && b == 0x11ba)
	return 0x11bb;

    return 0;
}

static 
#ifndef __SYMBIAN32__
inline 
#endif
ucschar
build_syllable(const ucschar* str, size_t len)
{
    int i;
    ucschar cho = 0, jung = 0, jong = 0;

    i = 0;
    while (i < len && hangul_is_choseong_conjoinable(str[i])) {
	cho = choseong_compress(cho, str[i]);
	if (cho == 0)
	    return 0;
	i++;
    }

    while (i < len && hangul_is_jungseong_conjoinable(str[i])) {
	jung = jungseong_compress(jung, str[i]);
	if (jung == 0)
	    return 0;
	i++;
    }

    while (i < len && hangul_is_jongseong_conjoinable(str[i])) {
	jong = jongseong_compress(jong, str[i]);
	if (jong == 0)
	    return 0;
	i++;
    }

    if (i < len)
	return 0;

    return hangul_jaso_to_syllable(cho, jung, jong);
}

/**
 * @brief �븳 �쓬�젅�뿉 �빐�떦�븯�뒗 肄붾뱶�쓽 媛��닔瑜� 援ы븳�떎
 * @param str �쓬�젅�쓽 湲몄씠瑜� 援ы븷 �뒪�듃留�
 * @param max_len @a str �뿉�꽌 �씫�쓣 湲몄씠�쓽 �젣�븳媛�
 * @return �븳 �쓬�젅�뿉 �빐�떦�븯�뒗 肄붾뱶�쓽 媛��닔
 *
 * �씠 �븿�닔�뒗 @a str �뿉�꽌 �븳 �쓬�젅�뿉 �빐�떦�븯�뒗 肄붾뱶�쓽 媛��닔瑜� 援ы븳�떎. 
 * �븳 �쓬�젅�뿉 �빐�떦�븯�뒗 肄붾뱶�쓽 媛��닔媛� @a max_len 蹂대떎 留롫떎硫� @a max_len �쓣 
 * 諛섑솚�븳�떎. �븳 �쓬�젅�씠�씪怨� �뙋�떒�븯�뒗 湲곗����� L*V*T+ �뙣�꽩�뿉 �뵲瑜몃떎. �씠 �뙣�꽩���
 * regular expression�쓽 而⑤깽�뀡�쓣 �뵲瑜� 寃껋쑝濡�, 1媛� �씠�긽�쓽 珥덉꽦怨� 以묒꽦, 0媛�
 * �씠�긽�쓽 醫낆꽦�씠 紐⑥씤 �옄紐� �뒪�듃留곸쓣 �븳 �쓬�젅濡� �씤�떇�븳�떎�뒗 �쑜�씠�떎. �삁瑜� �뱾硫�
 * �떎�쓬怨� 媛숈�� �옄紐� �뒪�듃留곷룄 �븳 �쓬�젅濡� �씤�떇�븳�떎.
 *
 *  �삁) "�뀆 �뀆 �뀥 �뀛 �꽮 �꽦" -> "���"
 * 
 * �뵲�씪�꽌 �쐞 寃쎌슦�뿉�뒗 6�쓣 諛섑솚�븯寃� �맂�떎. 
 *
 * �씪諛섏쟻�쑝濡쒕뒗 諛⑹젏(U+302E, U+302F)源뚯�� �븳 �쓬�젅濡� �씤�떇�븯寃좎��留�, �씠 �븿�닔�뒗
 * �쓬�젅怨� �옄紐④컙 蹂��솚�쓣 �렪由ы븯寃� �븯湲� �쐞�빐 援ы쁽�맂 寃껋쑝濡� 諛⑹젏��� �떎瑜� �쓬�젅濡� 
 * �씤�떇�븳�떎.
 *
 * @a str �씠 �옄紐� 肄붾뱶�뿉 �빐�떦�븯吏� �븡�뒗 寃쎌슦�뿉�뒗 1�쓣 諛섑솚�븳�떎.
 *
 * �씠 �븿�닔�뒗 �옄紐� �뒪�듃留곸뿉�꽌 珥� �쓬�젅�쓽 媛��닔瑜� 援ы븯�뒗 �븿�닔媛� �븘�떂�뿉 二쇱쓽�븳�떎.
 */
int
hangul_syllable_len(const ucschar* str, int max_len)
{
    int i = 0;

    if (max_len == 0)
	return 0;

    if (str[i] != 0) {
	for (i = 1; i < max_len; i++) {
	    if (str[i] == 0)
		break;

	    if (is_syllable_boundary(str[i - 1], str[i]))
		break;
	}
    }

    return i;
}

/**
 * @brief @a iter瑜� 湲곗���쑝濡� �씠�쟾 �쓬�젅�쓽 泥レ옄紐� 湲��옄�뿉 ����븳 �룷�씤�꽣瑜� 援ы븳�떎.
 * @param iter �쁽�옱 �쐞移�
 * @param begin �뒪�듃留곸쓽 �떆�옉�쐞移�, �룷�씤�꽣媛� �씠�룞�븷 �븳怨꾧컪
 * @return �씠�쟾 �쓬�젅�쓽 泥ル쾲吏� �옄紐⑥뿉 ����븳 �룷�씤�꽣
 *
 * �씠 �븿�닔�뒗 @a iter濡� 二쇱뼱吏� �옄紐� �뒪�듃留곸쓽 �룷�씤�꽣瑜� 湲곗���쑝濡� �씠�쟾 �쓬�젅�쓽 
 * 泥ル쾲吏� �옄紐⑥뿉 ����븳 �룷�씤�꽣瑜� 由ы꽩�븳�떎. �쓬�젅�쓣 李얘린�쐞�빐�꽌 begin蹂대떎 
 * �븵履쎌쑝濡� �씠�룞�븯吏� �븡�뒗�떎. 
 *
 * �븳 �쓬�젅�씠�씪怨� �뙋�떒�븯�뒗 湲곗����� L*V*T+M? �뙣�꽩�뿉 �뵲瑜몃떎.
 */
const ucschar*
hangul_syllable_iterator_prev(const ucschar* iter, const ucschar* begin)
{
    if (iter > begin)
	iter--;

    while (iter > begin) {
	ucschar prev = iter[-1];
	ucschar curr = iter[0];
	if (is_syllable_boundary(prev, curr))
	    break;
	iter--;
    }

    return iter;
}

/**
 * @brief @a iter瑜� 湲곗���쑝濡� �떎�쓬 �쓬�젅�쓽 泥レ옄紐� 湲��옄�뿉 ����븳 �룷�씤�꽣瑜� 援ы븳�떎.
 * @param iter �쁽�옱 �쐞移�
 * @param end �뒪�듃留곸쓽 �걹�쐞移�, �룷�씤�꽣媛� �씠�룞�븷 �븳怨꾧컪
 * @return �떎�쓬 �쓬�젅�쓽 泥ル쾲吏� �옄紐⑥뿉 ����븳 �룷�씤�꽣
 *
 * �씠 �븿�닔�뒗 @a iter濡� 二쇱뼱吏� �옄紐� �뒪�듃留곸쓽 �룷�씤�꽣瑜� 湲곗���쑝濡� �떎�쓬 �쓬�젅�쓽 
 * 泥ル쾲吏� �옄紐⑥뿉 ����븳 �룷�씤�꽣瑜� 由ы꽩�븳�떎. �쓬�젅�쓣 李얘린�쐞�빐�꽌 end瑜� �꽆�뼱
 * �씠�룞�븯吏� �븡�뒗�떎. 
 *
 * �븳 �쓬�젅�씠�씪怨� �뙋�떒�븯�뒗 湲곗����� L*V*T+M? �뙣�꽩�뿉 �뵲瑜몃떎.
 */
const ucschar*
hangul_syllable_iterator_next(const ucschar* iter, const ucschar* end)
{
    if (iter < end)
	iter++;

    while (iter < end) {
	ucschar prev = iter[-1];
	ucschar curr = iter[0];
	if (is_syllable_boundary(prev, curr))
	    break;
	iter++;
    }

    return iter;
}

/**
 * @brief �옄紐� �뒪�듃留곸쓣 �쓬�젅 �뒪�듃留곸쓣 蹂��솚�븳�떎
 * @param dest �쓬�젅�삎�쑝濡� 蹂��솚�맂 寃곌낵媛� ����옣�맆 踰꾪띁
 * @param destlen 寃곌낵瑜� ����옣�븷 踰꾪띁�쓽 湲몄씠(ucschar 肄붾뱶 �떒�쐞)
 * @param src 蹂��솚�븷 �옄紐� �뒪�듃留�
 * @param srclen 蹂��솚�븷 �옄紐� �뒪�듃留곸쓽 湲몄씠(ucschar 肄붾뱶 �떒�쐞)
 * @return @a destlen �뿉 ����옣�븳 肄붾뱶�쓽 媛��닔
 *
 * �씠 �븿�닔�뒗 L+V+T*M? �뙣�꽩�뿉 �뵲�씪 �옄紐� �뒪�듃留� 蹂��솚�쓣 �떆�룄�븳�떎. �븳 �쓬�젅�쓣 
 * �뙋�떒�븯�뒗 湲곗����� @ref hangul_syllable_len �쓣 李몄“�븳�떎.
 * 留뚯씪 @a src 媛� �쟻�젅�븳 �쓬�젅�삎�깭濡� 蹂��솚�씠 遺덇���뒫�븳 寃쎌슦�뿉�뒗 �옄紐� �뒪�듃留곸씠
 * 洹몃��濡� 蹂듭궗�맂�떎.
 *
 * �씠 �븿�닔�뒗 �옄紐� �뒪�듃留� @a src 瑜� �쓬�젅�삎�쑝濡� 蹂��솚�븯�뿬 @a dest �뿉 ����옣�븳�떎.
 * @a srclen �뿉 吏��젙�맂 媛��닔留뚰겮 �씫怨�, @a destlen �뿉 吏��젙�맂 湲몄씠 �씠�긽 �벐吏�
 * �븡�뒗�떎.  @a srclen �씠 -1�씠�씪硫� @a src �뒗 0�쑝濡� �걹�굹�뒗 �뒪�듃留곸쑝濡� 媛��젙�븯怨�
 * 0�쓣 �젣�쇅�븳 湲몄씠源뚯�� 蹂��솚�쓣 �떆�룄�븳�떎. �뵲�씪�꽌 蹂��솚�맂 寃곌낵 �뒪�듃留곸�� 0�쑝濡� 
 * �걹�굹吏� �븡�뒗�떎. 留뚯씪 0�쑝濡� �걹�굹�뒗 �뒪�듃留곸쓣 留뚮뱾怨� �떢�떎硫� �떎�쓬怨� 媛숈씠 �븳�떎.
 *
 * @code
 * int n = hangul_jamos_to_syllables(dest, destlen, src, srclen);
 * dest[n] = 0;
 * @endcode
 */
int
hangul_jamos_to_syllables(ucschar* dest, int destlen, const ucschar* src, int srclen)
{
    ucschar* d;
    const ucschar* s;

    int inleft;
    int outleft;
    int n;

    if (srclen < 0) {
	s = src;
	while (*s != 0)
	    s++;
	srclen = s - src;
    }

    s = src;
    d = dest;
    inleft = srclen;
    outleft = destlen;

    n = hangul_syllable_len(s, inleft);
    while (n > 0 && inleft > 0 && outleft > 0) {
	ucschar c = build_syllable(s, n);
	if (c != 0) {
	    *d = c;
	    d++;
	    outleft--;
	} else {
	    int i;
	    for (i = 0; i < n && i < outleft; i++) {
		d[i] = s[i];
	    }
	    d += i;
	    outleft -= i;
	}

	s += n;
	inleft -= n;
	n = hangul_syllable_len(s, inleft);
    }

    return destlen - outleft;
}
