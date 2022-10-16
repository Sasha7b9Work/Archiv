package  {
	/**
	 * ...
	 * sasha7b9@tut.by
	 */
	public class MenuSubItem {
		
		public static const HEIGHT_OPENED : int = 20;
		
		public static const CHAN_1_ENTER_ENABLE : int = 0,
							CHAN_1_ENTER_DISABLE : int = 1,
							CHAN_1_ENTER_INVERSE : int = 2,
							CHAN_1_COUPLING_AC : int = 3,
							CHAN_1_COUPLING_DC : int = 4,
							CHAN_1_COUPLING_GND : int = 5,
							CHAN_1_FACTOR_1x : int = 6,
							CHAN_1_FACTOR_10x : int = 7,
							CHAN_2_ENTER_ENABLE : int = 10,
							CHAN_2_ENTER_DISABLE : int = 11,
							CHAN_2_ENTER_INVERSE : int = 12,
							CHAN_2_COUPLING_AC : int = 13,
							CHAN_2_COUPLING_DC : int = 14,
							CHAN_2_COUPLING_GND : int = 15,
							CHAN_2_FACTOR_1x : int = 16,
							CHAN_2_FACTOR_10x : int = 17,
							CURS_CHOICE_VERTICAL : int = 20,
							CURS_CHOICE_HORIZONTAL : int = 21,
							CURS_CHOICE_DISABLE : int = 22,
							CURS_X_CURSOR_1 : int = 23,
							CURS_X_CURSOR_2 : int = 24,
							CURS_X_CURSOR_1_2 : int = 25,
							CURS_MEAS_T_T1 : int = 26,
							CURS_MEAS_T_T2 : int = 27,
							CURS_MEAS_T_dT : int = 28,
							CURS_Y_CURSOR_1 : int = 29,
							CURS_Y_CURSOR_2 : int = 30,
							CURS_Y_CURSOR_1_2 : int = 31,
							CURS_MEAS_V_V1 : int = 32,
							CURS_MEAS_V_V2 : int = 33,
							CURS_MEAS_V_dV : int = 34,
							DISPLAY_MAPPING_VECTOR : int = 40,
							DISPLAY_MAPPING_POINTS : int = 41,
							DISPLAY_GRID_STANDARD : int = 42,
							DISPLAY_GRID_COORD : int = 43,
							DISPLAY_GRID_DISABLE : int = 44,
							DISPLAY_ACCUMULATION_ENABLE : int = 45, 
							DISPLAY_ACCUMULATION_DISABLE : int = 46,
							DISPLAY_UPDATE : int = 47,
							DISPLAY_SCREEN_1 : int = 48,
							DISPLAY_SCREEN_2 : int = 49,
							MEMORY_OPERATION_EXTRACTION : int = 60,
							MEMORY_OPERATION_SAVING : int = 61,
							MEMORY_SOURCE_CHAN_1 : int = 62,
							MEMORY_SOURCE_CHAN_2 : int = 63,
							MEMORY_SOURCE_SETTING : int = 64,
							MEMORY_STORAGE_INT : int = 65,
							MEMORY_STORAGE_EXT : int = 66,
							MEMORY_MEMORY_M1 : int = 67,
							MEMORY_EXECUTION_EXE : int = 68,
							TRIG_MODE_AUTO : int = 80,
							TRIG_MODE_WAITING : int = 81,
							TRIG_MODE_SINGLE : int = 82,
							TRIG_SOURCE_CHAN_1 : int = 83,
							TRIG_SOURCE_CHAN_2 : int = 84,
							TRIG_SOURCE_EXT : int = 85,
							TRIG_POLARITY_FRONT : int = 86,
							TRIG_POLARITY_FALL : int = 87,
							TRIG_FILTR_DISABLE : int = 88,
							TRIG_FILTR_LPF : int = 89,
							TRIG_FILTR_HPF : int = 90,
							TRIG_COUPLING_DC : int = 91,
							TRIG_COUPLING_AC : int = 92,
							MEAS_FREQ_1 : int = 100,
							MEAS_FREQ_2 : int = 101,
							MEAS_T_1 : int = 102,
							MEAS_T_2 : int = 103;
							
		private static var names : Array = new Array();
							
		private var type : int,
					selected : Boolean = false,
					display : Display;
		
		public function MenuSubItem(_type : int, _display : Display) {
			type = _type;
			display = _display;
			
			if(names[CHAN_1_ENTER_ENABLE] == undefined) {
				names[CHAN_1_ENTER_ENABLE] = "ВКЛ.";
				names[CHAN_1_ENTER_DISABLE] = "ВЫКЛ.";
				names[CHAN_1_ENTER_INVERSE] = "ИНВЕРСИЯ";
				names[CHAN_1_COUPLING_AC] = "ПЕРЕМ.";
				names[CHAN_1_COUPLING_DC] = "ПОСТ.";
				names[CHAN_1_COUPLING_GND] = "ЗЕМЛЯ";
				names[CHAN_1_FACTOR_1x] = "1x";
				names[CHAN_1_FACTOR_10x] = "10x";
				names[CHAN_2_ENTER_ENABLE] = "ВКЛ.";
				names[CHAN_2_ENTER_DISABLE] = "ВЫКЛ.";
				names[CHAN_2_ENTER_INVERSE] = "ИНВЕРСИЯ";
				names[CHAN_2_COUPLING_AC] = "ПЕРЕМ.";
				names[CHAN_2_COUPLING_DC] = "ПОСТ.";
				names[CHAN_2_COUPLING_GND] = "ЗЕМЛЯ";
				names[CHAN_2_FACTOR_1x] = "1x";
				names[CHAN_2_FACTOR_10x] = "10x";
				names[CURS_CHOICE_VERTICAL] = "ВЕРТИК.";
				names[CURS_CHOICE_HORIZONTAL] = "ГОРИЗОНТ.";
				names[CURS_CHOICE_DISABLE] = "ВЫКЛ.";
				names[CURS_X_CURSOR_1] = "КУРСОР 1";
				names[CURS_X_CURSOR_2] = "КУРСОР 2";
				names[CURS_X_CURSOR_1_2] = "КУРСОРЫ 1+2";
				names[CURS_MEAS_T_T1] = "Т1 = ";
				names[CURS_MEAS_T_T2] = "Т2 = ";
				names[CURS_MEAS_T_dT] = "dT = ";
				names[CURS_Y_CURSOR_1] = "КУРСОР 1";
				names[CURS_Y_CURSOR_2] = "КУРСОР 2";
				names[CURS_Y_CURSOR_1_2] = "КУРСОРЫ 1+2";
				names[CURS_MEAS_V_V1] = "V1 = ";
				names[CURS_MEAS_V_V2] = "V2 = ";
				names[CURS_MEAS_V_dV] = "dV = ";
				names[DISPLAY_MAPPING_VECTOR] = "ВЕКТОР";
				names[DISPLAY_MAPPING_POINTS] = "ТОЧКИ";
				names[DISPLAY_GRID_STANDARD] = "СТАНДАРТ";
				names[DISPLAY_GRID_COORD] = "КООРДИНАТ.";
				names[DISPLAY_GRID_DISABLE] = "ВЫКЛ.";
				names[DISPLAY_ACCUMULATION_ENABLE] = "ВКЛ.";
				names[DISPLAY_ACCUMULATION_DISABLE] = "ВЫКЛ.";
				names[DISPLAY_UPDATE] = "16";
				names[DISPLAY_SCREEN_1] = "1";
				names[DISPLAY_SCREEN_2] = "2";
				names[MEMORY_OPERATION_EXTRACTION] = "ИЗВЛЕЧ.";
				names[MEMORY_OPERATION_SAVING] = "СОХРАН.";
				names[MEMORY_SOURCE_CHAN_1] = "КАНАЛ 1";
				names[MEMORY_SOURCE_CHAN_2] = "КАНАЛ 2";
				names[MEMORY_SOURCE_SETTING] = "УСТАН.";
				names[MEMORY_STORAGE_INT] = "ВНУТР.";
				names[MEMORY_STORAGE_EXT] = "ВНЕШН.";
				names[MEMORY_MEMORY_M1] = "M1";
				names[MEMORY_EXECUTION_EXE] = "ВЫПОЛН.";
				names[TRIG_MODE_AUTO] = "АВТ.";
				names[TRIG_MODE_WAITING] = "ЖДУЩИЙ";
				names[TRIG_MODE_SINGLE] = "ОДНОКР.";
				names[TRIG_SOURCE_CHAN_1] = "КАНАЛ 1";
				names[TRIG_SOURCE_CHAN_2] = "КАНАЛ 2";
				names[TRIG_SOURCE_EXT] = "ВНЕШН.";
				names[TRIG_POLARITY_FRONT] = "ФРОНТ";
				names[TRIG_POLARITY_FALL] = "СРЕЗ";
				names[TRIG_COUPLING_DC] = "ПОСТ.";
				names[TRIG_COUPLING_AC] = "ПЕРЕМ.";
				names[TRIG_FILTR_LPF] = "ФНЧ";
				names[TRIG_FILTR_HPF] = "ФВЧ";
				names[TRIG_FILTR_DISABLE] = "ВЫКЛ.";
				names[MEAS_FREQ_1] = "1:";
				names[MEAS_FREQ_2] = "2:";
				names[MEAS_T_1] = "1:";
				names[MEAS_T_2] = "2:";
			}
		}
		
		public function Text() : String {
			return names[type];
		}
		
		public function SetSelected(_selected : Boolean) : void {
			selected = _selected;
		}
		
		public function IsSelected() : Boolean {
			return selected;
		}
		
		public function DrawOpened(_x : int, _y : int) : void {
			display.SetDrawStyle(Color.R128, Color.WHITE);
			display.DrawRect(_x, _y, MenuItem.WIDTH_TITLE, HEIGHT_OPENED);
			if (selected) {
				display.SetDrawStyle(Color.R154, Color.R128);
				var delta : int = 3;
				display.DrawRect(_x + delta, _y + delta, MenuItem.WIDTH_TITLE - 2 * delta, HEIGHT_OPENED - 2 * delta);
			}
			display.DrawText(names[type], _x, _y + 3, MenuItem.WIDTH_TITLE, HEIGHT_OPENED, Color.BLACK, "center", CONST.SIZE_MENU_ITEM_FONT, CONST.NAME_MENU_ITEM_FONT);
		}
	}

}