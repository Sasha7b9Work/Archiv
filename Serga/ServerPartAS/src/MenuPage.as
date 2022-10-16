package  {
	import flash.ui.ContextMenuClipboardItems;
	/**
	 * ...
	 * @author 
	 */
	public class MenuPage {
	
		public static const ITEMS_ON_PAGE : int = 5;
		
		public static const CHAN_0 : int = 0,
							CHAN_1 : int = 1,
							CURSORS : int = 2,
							DISPLAY : int = 3,
							MEMORY : int = 4,
							SYNCHRONIZATION : int = 5,
							MEASURES : int = 6;
							
		private var items : Array = new Array(),
					firstItem : int = 0,
					name : String,
					display : Display,
					enabled : Boolean = false;
		
		public function MenuPage(_type : int, _display : Display) {
			display = _display;
			
			if (_type == CHAN_0) {
				name = "КАНАЛ 1";
				items.push(new MenuItem(MenuItem.CHAN_1_ENTER, display, this));
				items.push(new MenuItem(MenuItem.CHAN_1_COUPLING, display, this));
				items.push(new MenuItem(MenuItem.CHAN_1_FACTOR, display, this));
			} else if (_type == CHAN_1) {
				name = "КАНАЛ 2";
				items.push(new MenuItem(MenuItem.CHAN_2_ENTER, display, this));
				items.push(new MenuItem(MenuItem.CHAN_2_COUPLING, display, this));
				items.push(new MenuItem(MenuItem.CHAN_2_FACTOR, display, this));
			} else if (_type == CURSORS) {
				name = "КУРСОРЫ";
				items.push(new MenuItem(MenuItem.CURS_CHOISE, display, this));
				items.push(new MenuItem(MenuItem.CURS_CURSORS_X, display, this));
				items.push(new MenuItem(MenuItem.CURS_MEAS_T, display, this));
				items.push(new MenuItem(MenuItem.CURS_CURSORS_Y, display, this));
				items.push(new MenuItem(MenuItem.CURS_MEAS_V, display, this));
			} else if (_type == DISPLAY) {
				name = "ДИСПЛЕЙ";
				items.push(new MenuItem(MenuItem.DISPLAY_MAPPING, display, this));
				items.push(new MenuItem(MenuItem.DISPLAY_GRID, display, this));
				items.push(new MenuItem(MenuItem.DISPLAY_ACCUMULATION, display, this));
				items.push(new MenuItem(MenuItem.DISPLAY_UPDATE, display, this));
				items.push(new MenuItem(MenuItem.DISPLAY_SCREEN, display, this));
			} else if (_type == MEMORY) {
				name = "ПАМЯТЬ";
				items.push(new MenuItem(MenuItem.MEM_OPERATION, display, this));
				items.push(new MenuItem(MenuItem.MEM_SOURCE, display, this));
				items.push(new MenuItem(MenuItem.MEM_STORAGE, display, this));
				items.push(new MenuItem(MenuItem.MEM_MEMORY, display, this));
				items.push(new MenuItem(MenuItem.MEM_EXECUTION, display, this));
			} else if (_type == SYNCHRONIZATION) {
				name = "ЗАПУСК";
				items.push(new MenuItem(MenuItem.TRIG_MODE, display, this));
				items.push(new MenuItem(MenuItem.TRIG_SOURCE, display, this));
				items.push(new MenuItem(MenuItem.TRIG_POLARITY, display, this));
				items.push(new MenuItem(MenuItem.TRIG_COUPLING, display, this));
				items.push(new MenuItem(MenuItem.TRIG_FILTR, display, this));
			} else if (_type == MEASURES) {
				name = "ИЗМЕРЕНИЯ";
				items.push(new MenuItem(MenuItem.MEAS_FREQ, display, this));
				items.push(new MenuItem(MenuItem.MEAS_T, display, this));
			}
			
			(items[0] as MenuItem).SetSelected(true);
			
			for (var i : int = 0; i < items.length; i++) {
				(items[i] as MenuItem).SetPosition(i);
			}
		}
		
		public function IsEnabled() : Boolean {
			return enabled;
		}
		
		public function SetEnabled(_enabled : Boolean) : void {
			enabled = _enabled;
		}
		
		public function PressButton(_button : int) : void {
			switch(_button) {
				case TextButton.MENU:
					enabled = !enabled;
					break;
				case TextButton.F1:
				case TextButton.F2:
				case TextButton.F3:
				case TextButton.F4:
				case TextButton.F5:
					if (enabled) {
						var item : MenuItem = ItemUnderFuncKey(_button);
						if (item) {
							var openedItem : MenuItem = OpenedItem();
							if(!openedItem || item == openedItem) {
								UnselectAllItems();
								item.SetSelected(true);
								item.ChooseNextValue();
							} else if (openedItem != item) {
								openedItem.SetOpened(false);
							}
						}
					}
					break;
				default:
					break;
			}
		}
		
		private function ItemUnderFuncKey(_button : int) : MenuItem {
			var index : int = firstItem + _button - 1;
			return items[index] as MenuItem;
		}
		
		public function LongPressTheButton(_button : int) : void {
			switch(_button) {
				case TextButton.F1:
				case TextButton.F2:
				case TextButton.F3:
				case TextButton.F4:
				case TextButton.F5:
					if (enabled) {
						var item : MenuItem = ItemUnderFuncKey(_button);
						if (item.IsOpened()) {
							item.SetOpened(false);
							item.ChoosePrevValue();
						} else if (item) {
							item.SetOpened(true);
							item.ChoosePrevValue();
						}
					}
					break;
				default:
					break;
			}
		}
		
		private function UnselectAllItems() : void {
			for each(var item : MenuItem in items) {
				item.SetSelected(false);
			}
		}
		
		public function Draw() : void {
			if (!enabled) {
				return;
			}
			
			display.SetDrawStyle(Color.R211, Color.WHITE);
			display.DrawRect(display.XmenuPage(), display.YmenuPage(), display.WidthTitleMenuPage(), Menu.HEIGHT_TITLE);
			display.DrawText(name, display.XmenuPage(), display.YmenuPage() + 5, display.WidthTitleMenuPage(), Menu.HEIGHT_TITLE, Color.BLACK);
			
			for each(var item : MenuItem in items) {
				item.Draw();
			}
			
			item = OpenedItem();
			
			if (item) {
				item.DrawOpened();
			}
		}
		
		private function OpenedItem() : MenuItem {
			for each(var item : MenuItem in items) {
				if (item.IsOpened()) {
					return item;
				}
			}
			return undefined;
		}
		
		public function FirstItemOnScreen() : int {
			return firstItem;
		}
	
	}

}