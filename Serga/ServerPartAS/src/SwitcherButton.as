package  {
	
	import flash.display.*;
	import flash.events.*;
	import flash.text.*;
	import flash.ui.*;
	
	public class SwitcherButton extends GUIelement {
		
		public static const	MODE_WORK : int = 0,	SOURCE : int = 1;

		private static const	WIDTH : int = 30,
								HEIGHT : int = 15;
		
		private var state : int = 0,
					bmArray : Array = new Array();
		
		public function SwitcherButton(_type : int, _text0 : String, _text1 : String) {
			
			super(GUIelement.SWITCHER_BUTTON, _type);
			
			_names_ = new Array(
				"ButtonModeWork",		"ButtonSource"
			);
			
			var sprite0 : Image = new Image(),
				sprite1 : Image = new Image();
				
			sprite0.DrawRect(0, 0, WIDTH - 1, HEIGHT - 1);
			sprite0.DrawRect(2, 2, WIDTH / 2 - 5, WIDTH / 2 - 5);
			sprite0.DrawText(_text0, WIDTH, 0);
			
			sprite1.DrawRect(0, 0, WIDTH - 1, HEIGHT - 1);
			sprite1.DrawRect(WIDTH / 2 + 2, 2, WIDTH / 2 - 5, WIDTH / 2 - 5);
			sprite1.DrawText(_text1, WIDTH, 0);
			
			bmArray.push(sprite0.BitMap());
			bmArray.push(sprite1.BitMap());
			
			addEventListener(MouseEvent.MOUSE_OVER, OnMouseEnter);
			addEventListener(MouseEvent.CLICK, OnMouseClick);
			addEventListener(MouseEvent.MOUSE_OUT, OnMouseLeave);
			
			addChild(bmArray[0]);
		}
		
		private function OnMouseEnter(_event : MouseEvent) : void {
			Mouse.cursor = MouseCursor.BUTTON;
		}
		
		private function OnMouseLeave(_event : MouseEvent) : void {
			Mouse.cursor = MouseCursor.AUTO;
		}
		
		private function OnMouseClick(_event : MouseEvent) : void {
			removeChild(bmArray[state]);
			state = (++state) & 0x1;
			addChild(bmArray[state]);
			dispatchEvent(new SwitcherButtonEvent(SwitcherButtonEvent.SWITCHING, state));
		}
	}

}