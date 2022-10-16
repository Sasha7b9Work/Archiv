//  [3/15/2010 Sasha]
#pragma once
#include "MathDevice.h"

#define HEIGHT_ROW 18
// Максимальное количество секунд
#define MAX_SECONDS 24 * 60 * 60

class TableProg : public QTableWidget{
    Q_OBJECT
public:
    TableProg(QWidget *parent, int width, int height);

    void FillTable(int row = 0, int column = 0);       // Заполнить таблицу данными из вектора

    void WriteParameters();
	void WriteToCell(int row, int column, QString str);
	bool DeleteCurrentRow();		// Удаляет строку, на которой находится выделенная ячейка
								    // Если строк для удаления не осталось или не было, возвращает false
	bool InsertRowAfterCurrent();	// Вставляет новую строку после текущей

protected:
    virtual void keyPressEvent(QKeyEvent *);

private slots:
    void ChangeValue(int, int);
};