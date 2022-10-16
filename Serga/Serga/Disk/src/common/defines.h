#pragma once

#define SAFE_DELETE(x)       if(x) { delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x) if(x) { delete []x; x = NULL; }

#define INVALID_VALUE_DOUBLE 1e100
#define EMPTY_STRING "---.---"
// ����� ���������� ��� �����, � ������� �������� ������������� ������������ ��� ������� � ������� �������,
// � ����� ����_��� � ����_���� ��� �������������
// ���� ����� �� ��� ������ �������
// ������ � ����� �������:
// �����. ������. 0, �����. ������. 1, ����_���, ����_����
#define CALIBRATE_NAME_FILE "calibrate.cfg"

// ����� � �������������, ����������� ��� ����� ������ ����� �� ������
#define TIME_ONE_BYTE 100.0

// ������� ��� ������� �����/������ �/�� ���������/���������
#define LEVEL_INPUT_PROC 10
// ������� ��� ����������� ���������� ���������
#define LEVEL_TRACE_PROC 11