package  {
	/*
	 * Sasha7b9@tut.by
	 */
	import flash.events.Event;
	
	public class TextButtonEvent extends Event {
		
		public static const DOWN : String = "down",
							UP : String = "up";
		
		public var button : TextButton;
		
		public function TextButtonEvent(_type : String, _button : TextButton) {
			super(_type);
			button = _button;
		}
		
	}

}