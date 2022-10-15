#pragma once

typedef unsigned char uint8_t;
typedef signed int int8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef unsigned   int uintptr_t;

typedef   signed  long long intmax_t;
typedef unsigned  long long uintmax_t;

/* These types MUST be 16-bit or 32-bit */
typedef int				INT;
typedef unsigned int	UINT;

/* This type MUST be 8-bit */
typedef unsigned char	BYTE;

/* These types MUST be 16-bit */
typedef short			SHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types MUST be 32-bit */
typedef long			LONG;
typedef unsigned long	DWORD;

/* This type MUST be 64-bit (Remove this for ANSI C (C89) compatibility) */
typedef unsigned long long QWORD;
