package {
	
	import flash.text.*;
	import flash.display.*;
	import flash.events.FocusEvent;
	
	public class DebugWindow extends Sprite {
		
		private var text : TextField = new TextField(),
					numMessage : int = 0,
					scrolled : Boolean = true;
		
		private static const	WIDTH : int = 500,
								HEIGHT : int = 130;
		
		public function DebugWindow() {
			text.width = WIDTH;
			text.height = HEIGHT;
			text.border = true;
			text.borderColor = 0x0;
			text.wordWrap = true;
			addChild(text);
			
			addEventListener(FocusEvent.FOCUS_IN, OnFocusIn);
			addEventListener(FocusEvent.FOCUS_OUT, OnFocusOut);
		}
		
		public function AddString(_string : String) : void {
			text.appendText((numMessage++).toString() + " : " + _string + "\n");
			if(scrolled) {
				text.scrollV = text.numLines;
			}
		}
		
		private function OnFocusIn(_event : FocusEvent) : void {
			scrolled = false;
		}
		
		private function OnFocusOut(_event : FocusEvent) : void {
			scrolled = true;
		}
		
	}

}