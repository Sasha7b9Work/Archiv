//  [4/3/2009 Sasha]
#pragma once

#include "FastLog.h"
#include "TextBuffer.h"

#pragma warning(disable:4127)
#include <QMouseEvent>
#include <QString>
#include <QObject>
#include <QColorDialog>
#include <QTableWidget>
#include <QTimer>
#include <QFrame>
#include <QPainter>
#include <QTextCodec>
#include <QComboBox>
#include <QFileDialog>
#include <QSettings>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QThread>
#pragma warning(default:4127)

#include <Windows.h>

#include <cmath>
#include <stdio.h>
#include <iostream>
#include <new>

#define SAFE_DELETE(x)       if(x) { delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x) if(x) { delete []x; x = NULL; }

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

#define INVALID_VALUE_DOUBLE 1e100
#define EMPTY_STRING "---.---"
// ����� ���������� ��� �����, � ������� �������� ������������� ������������ ��� ������� � ������� �������,
// � ����� ����_��� � ����_���� ��� �������������
// ���� ����� �� ��� ������ �������
// ������ � ����� �������:
// �����. ������. 0, �����. ������. 1, ����_���, ����_����
#define CALIBRATE_NAME_FILE "calibrate.cfg"
