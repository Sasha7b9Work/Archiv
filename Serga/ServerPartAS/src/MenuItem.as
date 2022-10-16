package  {
	/**
	 * ...
	 * sasha7b9@tut.by
	 */
	public class MenuItem {
		
		public static const	WIDTH_TITLE : int = 100,
							HEIGHT_TITLE : int = 25,
							HEIGHT : int = 35,
							HEIGHT_RECT_FOR_VALUE : int = 15,
							WIDTH_RECT_FOR_VALUE : int = 69,
							WIDTH : int = 75;
		
		public static const // Канал1, 2
							CHAN_1_ENTER : int = 0,
							CHAN_1_COUPLING : int = 1,
							CHAN_2_ENTER : int = 2,
							CHAN_2_COUPLING : int = 3,
							CHAN_1_FACTOR : int = 4,
							CHAN_2_FACTOR : int = 5,
							
							// Курсоры
							CURS_CHOISE : int = 10,
							CURS_CURSORS_X : int = 11,
							CURS_MEAS_T : int = 12,
							CURS_CURSORS_Y : int = 13,
							CURS_MEAS_V : int = 14,
							
							// Дисплей
							DISPLAY_MAPPING : int = 20,
							DISPLAY_GRID : int = 21,
							DISPLAY_ACCUMULATION : int = 22,
							DISPLAY_UPDATE : int = 23,
							DISPLAY_SCREEN : int = 24,
							
							// Память
							MEM_OPERATION : int = 30,
							MEM_SOURCE : int = 31,
							MEM_STORAGE : int = 32,
							MEM_MEMORY : int = 33,
							MEM_EXECUTION : int = 34,
							
							// Синхронизация
							TRIG_MODE : int = 40,
							TRIG_SOURCE : int = 41,
							TRIG_POLARITY : int = 42,
							TRIG_COUPLING : int = 43,
							TRIG_FILTR : int = 44,
							
							// Измерения
							MEAS_FREQ : int = 50,
							MEAS_T : int = 51;
		
		private var display : Display,
					subItems : Array = new Array(),
					isOpened : Boolean = false,
					selected : Boolean = false,
					name : String,
					page : MenuPage,
					position : int = -1;
		
		public function MenuItem(_type : int, _display : Display, _page : MenuPage) {
			display = _display;
			page = _page;

			if (_type == CHAN_1_ENTER) {
				name = "ВХОД";
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_1_ENTER_ENABLE, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_1_ENTER_DISABLE, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_1_ENTER_INVERSE, display));
			} else if (_type == CHAN_1_COUPLING) {
				name = "СВЯЗЬ";
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_1_COUPLING_DC, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_1_COUPLING_AC, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_1_COUPLING_GND, display));
			} else if (_type == CHAN_2_ENTER) {
				name = "ВХОД";
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_2_ENTER_ENABLE, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_2_ENTER_DISABLE, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_2_ENTER_INVERSE, display));
			} else if (_type == CHAN_2_COUPLING) {
				name = "СВЯЗЬ";
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_2_COUPLING_DC, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_2_COUPLING_AC, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_2_COUPLING_GND, display));
			} else if (_type == CHAN_1_FACTOR) {
				name = "МНОЖИТЕЛЬ";
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_1_FACTOR_1x, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_1_FACTOR_10x, display));
			} else if (_type == CHAN_2_FACTOR) {
				name = "МНОЖИТЕЛЬ";
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_2_FACTOR_1x, display));
				subItems.push(new MenuSubItem(MenuSubItem.CHAN_2_FACTOR_10x, display));
			} else if (_type == CURS_CHOISE) {
				name = "ВЫБОР";
				subItems.push(new MenuSubItem(MenuSubItem.CURS_CHOICE_VERTICAL, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_CHOICE_HORIZONTAL, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_CHOICE_DISABLE, display));
			} else if (_type == CURS_CURSORS_X) {
				name = "КУРСОРЫ Х";
				subItems.push(new MenuSubItem(MenuSubItem.CURS_X_CURSOR_1, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_X_CURSOR_2, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_X_CURSOR_1_2, display));
			} else if (_type == CURS_MEAS_T) {
				name = "ИЗМЕР.Т";
				subItems.push(new MenuSubItem(MenuSubItem.CURS_MEAS_T_T1, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_MEAS_T_T2, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_MEAS_T_dT, display));
			} else if (_type == CURS_CURSORS_Y) {
				name = "КУРСОРЫ Y";
				subItems.push(new MenuSubItem(MenuSubItem.CURS_Y_CURSOR_1, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_Y_CURSOR_2, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_Y_CURSOR_1_2, display));
			} else if (_type == CURS_MEAS_V) {
				name = "ИЗМЕР.V";
				subItems.push(new MenuSubItem(MenuSubItem.CURS_MEAS_V_V1, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_MEAS_V_V2, display));
				subItems.push(new MenuSubItem(MenuSubItem.CURS_MEAS_V_dV, display));
			} else if (_type == DISPLAY_MAPPING) {
				name = "ОТОБРАЖ.";
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_MAPPING_VECTOR, display));
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_MAPPING_POINTS, display));
			} else if (_type == DISPLAY_GRID) {
				name = "СЕТКА";
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_GRID_STANDARD, display));
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_GRID_COORD, display));
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_GRID_DISABLE, display));
			} else if (_type == DISPLAY_ACCUMULATION) {
				name = "НАКОПЛ.";
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_ACCUMULATION_ENABLE, display));
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_ACCUMULATION_DISABLE, display));
			} else if (_type == DISPLAY_UPDATE) {
				name = "ОБНОВЛ.";
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_UPDATE, display));
			} else if (_type == DISPLAY_SCREEN) {
				name = "ЭКРАН";
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_SCREEN_1, display));
				subItems.push(new MenuSubItem(MenuSubItem.DISPLAY_SCREEN_2, display));
			} else if (_type == MEM_OPERATION) {
				name = "ОПЕРАЦИЯ";
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_OPERATION_EXTRACTION, display));
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_OPERATION_SAVING, display));
			} else if (_type == MEM_SOURCE) {
				name = "ИСТОЧНИК";
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_SOURCE_CHAN_1, display));
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_SOURCE_CHAN_2, display));
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_SOURCE_SETTING, display));
			} else if (_type == MEM_STORAGE) {
				name = "ХРАНИЛИЩЕ";
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_STORAGE_INT, display));
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_STORAGE_EXT, display));
			} else if (_type == MEM_MEMORY) {
				name = "ПАМЯТЬ";
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_MEMORY_M1, display));
			} else if (_type == MEM_EXECUTION) {
				name = "ВЫПОЛН.";
				subItems.push(new MenuSubItem(MenuSubItem.MEMORY_EXECUTION_EXE, display));
			} else if (_type == TRIG_MODE) {
				name = "РЕЖИМ";
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_MODE_AUTO, display));
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_MODE_WAITING, display));
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_MODE_SINGLE, display));
			} else if (_type == TRIG_SOURCE) {
				name = "ИСТОЧНК";
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_SOURCE_CHAN_1, display));
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_SOURCE_CHAN_2, display));
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_SOURCE_EXT, display));
			} else if (_type == TRIG_POLARITY) {
				name = "ПОЛЯРНОСТЬ";
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_POLARITY_FRONT, display));
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_POLARITY_FALL, display));
			} else if (_type == TRIG_COUPLING) {
				name = "СВЯЗЬ";
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_COUPLING_DC, display));
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_COUPLING_AC, display));
			} else if (_type == TRIG_FILTR) {
				name = "ФИЛЬТР";
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_FILTR_DISABLE, display));
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_FILTR_LPF, display));
				subItems.push(new MenuSubItem(MenuSubItem.TRIG_FILTR_HPF, display));
			} else if (_type == MEAS_FREQ) {
				name = "ЧАСТОТА";
				subItems.push(new MenuSubItem(MenuSubItem.MEAS_FREQ_1, display));
				subItems.push(new MenuSubItem(MenuSubItem.MEAS_FREQ_2, display));
			} else if (_type == MEAS_T) {
				name = "ПЕРИОД";
				subItems.push(new MenuSubItem(MenuSubItem.MEAS_T_1, display));
				subItems.push(new MenuSubItem(MenuSubItem.MEAS_T_2, display));
			}
			
			(subItems[0] as MenuSubItem).SetSelected(true);
		}
		
		public function SetSelected(_selected : Boolean) : void {
			selected = _selected;
		}
		
		public function ChooseNextValue() : void {
			var indexSelected : int = NumberSelectedSubItem();
			indexSelected++;
			if (indexSelected > subItems.length - 1) {
				indexSelected = 0;
			}
			(subItems[NumberSelectedSubItem()] as MenuSubItem).SetSelected(false);
			(subItems[indexSelected] as MenuSubItem).SetSelected(true);
		}
		
		public function ChoosePrevValue() : void {
			var indexSelected : int = NumberSelectedSubItem();
			indexSelected--;
			if (indexSelected < 0) {
				indexSelected = subItems.length - 1;
			}
			(subItems[NumberSelectedSubItem()] as MenuSubItem).SetSelected(false);
			(subItems[indexSelected] as MenuSubItem).SetSelected(true);
		}
		
		private function NumberSelectedSubItem() : int {
			for (var i : int = 0; i < subItems.length; i++) {
				if ((subItems[i] as MenuSubItem).IsSelected()) {
					return i;
				}
			}
			return -1;
		}
	
		public function Draw() : void {
			if (PositionOnScreenRel() < MenuPage.ITEMS_ON_PAGE) {
				var _x : int = display.XmenuItem(),
					_y : int = PositionOnScreenAbsY();
			
				display.SetDrawStyle(Color.R238, Color.WHITE);
				display.DrawRect(_x, _y, WIDTH, HEIGHT);
				display.DrawText(name, _x, _y, WIDTH, HEIGHT, Color.WHITE, "center", CONST.SIZE_MENU_ITEM_FONT, CONST.NAME_MENU_ITEM_FONT);

				if (selected) {
					display.SetDrawStyle(Color.WHITE, 0);
				} else {
					display.SetDrawStyle(Color.R048, Color.R048);
				}
				var yRect : int = _y + 17;
				display.DrawRect(_x + 3, yRect, WIDTH_RECT_FOR_VALUE, HEIGHT_RECT_FOR_VALUE);
				var color : uint = Color.WHITE;
				if (selected) {
					color = Color.BLACK;
				}
				display.DrawText(subItems[NumberSelectedSubItem()].Text(), _x + 3, yRect, WIDTH_RECT_FOR_VALUE, HEIGHT_RECT_FOR_VALUE, color, "center", 
								CONST.SIZE_MENU_ITEM_FONT, CONST.NAME_MENU_ITEM_FONT);				
			}
		}
		
		private function PositionOnScreenRel() : int {
			return position - page.FirstItemOnScreen();
		}
		
		private function PositionOnScreenAbsY() : int {
			return Menu.HEIGHT_TITLE + Display.TOP_GRID + PositionOnScreenRel() * HEIGHT;
		}
		
		public function DrawOpened() : void {
			var y : int = 0,
				x : int = Display.RIGHT_GRID - WIDTH_TITLE - 1;
			if (PositionOnScreenAbsY() + HeightOpened() <= Display.BOTTOM_GRID) {
				y = PositionOnScreenAbsY();
			} else {
				y = Display.BOTTOM_GRID - HeightOpened() - 1;
			}
			display.SetDrawStyle(Color.R064, Color.WHITE);
			display.DrawRect(x, y, WIDTH_TITLE, HEIGHT_TITLE);
			display.DrawText(name, x, y + 5, WIDTH_TITLE, HEIGHT_TITLE, Color.WHITE);
			y += HEIGHT_TITLE;
			for (var i : int = 0; i < subItems.length; i++) {
				(subItems[i] as MenuSubItem).DrawOpened(x, y);
				y += MenuSubItem.HEIGHT_OPENED;
			}
		}
		
		private function HeightOpened() : int {
			return HEIGHT_TITLE + subItems.length * MenuSubItem.HEIGHT_OPENED;
		}
		
		public function SetOpened(_opened : Boolean) : void {
			isOpened = _opened;
		}
		
		public function IsOpened() : Boolean {
			return isOpened;
		}
		
		public function SetPosition(_position : int) : void {
			position = _position;
		}
	}
}