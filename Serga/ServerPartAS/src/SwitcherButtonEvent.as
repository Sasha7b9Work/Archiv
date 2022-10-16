package  {
	import flash.events.Event;
	/**
	 * ...
	 * Sasha7b9@tut.by
	 */
	public class SwitcherButtonEvent extends Event {
		
		public static const SWITCHING : String = "switching";
		
		public var state : int = -1;
		
		public function SwitcherButtonEvent(_type : String, _state : int) {
			super(_type);
			state = _state;
		}
		
	}

}