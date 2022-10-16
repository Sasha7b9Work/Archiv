// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/defines_.h"

#ifdef WIN32
    #define __attribute__(x)
    #define __packed__
    #define __STATIC_INLINE inline
    #define __RBIT(VAL) 0
    #define __CLZ(x) 0
    #define __regControl
    #define __asm(x)
    #define __weak
#else
    #define nullptr 0 //-V1059
#endif


#define NUMBER_VER "1.3.1"
#define NAME_MODEL_RU "C8-53/1"
#define NAME_MODEL_EN "S8-53/1"
#define NAME_MODEL (LANG_RU ? NAME_MODEL_RU : NAME_MODEL_EN)

#ifndef __IO
#define __IO volatile
#endif


#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))

#define BIN_U8( bits ) _bitset(0##bits)

#define DISABLE_RU  "����"
#define DISABLE_EN  "Disable"
#define ENABLE_RU   "���"
#define ENABLE_EN   "Enable"

#define EXIT_RU "�����"
#define EXIT_EN "Exit"
#define EXIT_ILLUSTRATION_RU "������ ��� ������ � ���������� ����"
#define EXIT_ILLUSTRATION_EN "Button for return to previous menu"


#include "common/Log_.h"
#include "common/Utils/Debug_.h"
