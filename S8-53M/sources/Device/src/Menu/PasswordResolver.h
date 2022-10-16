// 2021/07/22 14:50:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panel/Panel.h"


/*
*  Класс для дешфировки клавиатурных последовательностей
*  Нужнно для открытия страницы "ОТЛАДКА"
*/


namespace PasswordResolver
{
	void ProcessEvent(const KeyboardEvent &event);
}
