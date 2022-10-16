//  [3/15/2010 Sasha]
#pragma once
#include "MathDevice.h"

#define HEIGHT_ROW 18
// ������������ ���������� ������
#define MAX_SECONDS 24 * 60 * 60

class TableProg : public QTableWidget{
    Q_OBJECT
public:
    TableProg(QWidget *parent, int width, int height);

    void FillTable(int row = 0, int column = 0);       // ��������� ������� ������� �� �������

    void WriteParameters();
	void WriteToCell(int row, int column, QString str);
	bool DeleteCurrentRow();		// ������� ������, �� ������� ��������� ���������� ������
								    // ���� ����� ��� �������� �� �������� ��� �� ����, ���������� false
	bool InsertRowAfterCurrent();	// ��������� ����� ������ ����� �������

protected:
    virtual void keyPressEvent(QKeyEvent *);

private slots:
    void ChangeValue(int, int);
};