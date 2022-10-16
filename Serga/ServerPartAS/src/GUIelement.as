package  {
	import flash.display.Sprite;
	/**
	 * ...
	 * Sasha7b9@tut.by
	 */
	
	public class GUIelement extends Sprite {
		
		private static var lastID : int = 0;
		
		// Type
		public static const TEXT_BUTTON : int = 0,
							SWITCHER_BUTTON : int = 1,
							REGULATOR : int = 2;
							
		private var _type_ : int,
					_id_ : int,
					_subType_ : int;		// for _names_
		
		protected var _names_ : Array;
		
		public function GUIelement(_type : int, _subType : int) {
			_type_ = _type;
			lastID++;
			_id_ = lastID;
			_subType_ = _subType;
		}
		
		public function get Type() : int {
			return _type_;
		}
		
		public function ID() : int {
			return _id_;
		}
		
		public function SubType() : int {
			return _subType_;
		}
		
		public function Name() : String {
			return _names_[_subType_];
		}
	}

}
