package  {
	import flash.events.Event;
	/**
	 * ...
	 * Sasha7b9@tut.by
	 */
	public class RegulatorEvent extends Event {
		
		public static const DECREASE : String = "decrease",
							INCREASE : String = "increase";
							
		public var regulator : Regulator;
		
		public function RegulatorEvent(_type : String, _regulator : Regulator) {
			super(_type);
			regulator = _regulator;
		}
		
	}

}