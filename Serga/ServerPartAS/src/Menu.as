package  {
	import adobe.utils.CustomActions;
	import flash.events.*;
	import flash.utils.*;
	/**
	 * ...
	 * Sasha7b9@tut.by
	 */
	public class Menu extends EventDispatcher {
		
		public static const HEIGHT_TITLE : int = 25;
		
		private var display : Display,
					currentPage : int = TextButton.CHAN_0,
					timerForButton : Timer = new Timer(1000, 1),
					buttonTimer : int = -1;
					
		private static var 	pages : Array = new Array();
		
		public function Menu(_display : Display) {
			display = _display;
			
			pages[TextButton.CHAN_0] = new MenuPage(MenuPage.CHAN_0, display);
			pages[TextButton.CHAN_1] = new MenuPage(MenuPage.CHAN_1, display);
			pages[TextButton.CURSOR] = new MenuPage(MenuPage.CURSORS, display);
			pages[TextButton.DISPLAY] = new MenuPage(MenuPage.DISPLAY, display);
			pages[TextButton.MEMORY] = new MenuPage(MenuPage.MEMORY, display);
			pages[TextButton.TRIG_MENU] = new MenuPage(MenuPage.SYNCHRONIZATION, display);
			pages[TextButton.AUTO_MEAS] = new MenuPage(MenuPage.MEASURES, display);
			
			timerForButton.addEventListener(TimerEvent.TIMER_COMPLETE, OnTimerForButton);
		}
		
		public function PressButton(_button : int) : void {
			timerForButton.start();
			buttonTimer = _button;
			
			switch(_button) {
				case TextButton.F1:
				case TextButton.F2:
				case TextButton.F3:
				case TextButton.F4:
				case TextButton.F5:
				case TextButton.MENU:
					(pages[currentPage] as MenuPage).PressButton(_button);
					break;
				case TextButton.CHAN_0 :
				case TextButton.CHAN_1 :
				case TextButton.TRIG_MENU :
				case TextButton.CURSOR:
				case TextButton.DISPLAY:
				case TextButton.AUTO_MEAS:
				case TextButton.SERVICE:
				case TextButton.MEMORY:
				case TextButton.INF:
					FuncProcessing(_button);
					break;
				default:
					break;
			}
			
			Draw();			
			

		}
		
		public function ReleaseButton(_button : int) : void {

			
			timerForButton.stop();
		}
		
		private function OnTimerForButton(_event : TimerEvent) : void {
			switch(buttonTimer) {
				case TextButton.F1:
				case TextButton.F2:
				case TextButton.F3:
				case TextButton.F4:
				case TextButton.F5:
					(pages[currentPage] as MenuPage).LongPressTheButton(buttonTimer);
					break;
				default:
					break;
			}
			
			Draw();
		}
		
		private function Draw() : void {
			display.Clear();
			display.DrawGrid();
			display.DrawCursors();
			(pages[currentPage] as MenuPage).Draw();
		}
		
		private function FuncProcessing(_button : int) : void {
			if (currentPage == _button) {
				(pages[currentPage] as MenuPage).SetEnabled(!(pages[currentPage] as MenuPage).IsEnabled());
			} else {
				currentPage = _button;
				(pages[currentPage] as MenuPage).SetEnabled(true);
			}
		}
	}
}



