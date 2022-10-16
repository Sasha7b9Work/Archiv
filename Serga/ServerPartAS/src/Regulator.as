package  {
	
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.ui.Mouse;
	import flash.ui.Keyboard;
	import flash.ui.MouseCursor;
	
	public class Regulator extends GUIelement {
		
		protected var RADIUS : uint = 0;
		
		private var prevMouseX : Number = 0,
					prevMouseY : Number = 0,
					deltaRotation : Number = 0,
					prevRotation : Number = 0,
					currentRotation : Number,
					spritePoint : Sprite,
					mouseIsDown : Boolean = false;
		
		public function Regulator(_type : int, _radius : uint, _deltaRotation : Number, _x : int, _y : int) {
			
			super(GUIelement.REGULATOR, _type);
			
			RADIUS = _radius;
			DrawTrivial();
			
			spritePoint = new Sprite();
			addChild(spritePoint);
			spritePoint.graphics.beginFill(0);
			spritePoint.graphics.lineStyle(1);
			spritePoint.graphics.drawCircle(0, - RADIUS * 0.5, RADIUS / 6);
			spritePoint.graphics.endFill();
			
			x = _x;
			y = _y;
			
			deltaRotation = _deltaRotation;
			SetRotation((int)(Math.random() * 360 / deltaRotation) * deltaRotation);
			
			addEventListener(MouseEvent.MOUSE_OVER, OnMouseEnter);
			addEventListener(MouseEvent.MOUSE_OUT, OnMouseLeave);
			addEventListener(MouseEvent.MOUSE_DOWN, OnMouseDown);
			addEventListener(MouseEvent.MOUSE_UP, OnMouseUp);
			addEventListener(MouseEvent.MOUSE_MOVE, OnMouseMove);
		}
		
		public function Draw() : void {
			graphics.clear();
			graphics.beginFill(Color.REGULATOR);
			graphics.lineStyle(1, Color.BUTTON_BORDER);
			graphics.drawCircle(0, 0, RADIUS);
			graphics.endFill();
		}
		
		private function DrawTrivial() : void {
			Draw();
		}
		
		private function DrawExtended() : void {
			Draw();
			parent.addChild(this);
			graphics.beginFill(0, 0.0);
			graphics.lineStyle(1, 0, 0);
			graphics.drawCircle(0, 0, 500);
			graphics.endFill();
		}
		
		private function OnMouseEnter(_event : MouseEvent) : void {
			Mouse.cursor = MouseCursor.HAND;
		}
		
		private function OnMouseLeave(_event : MouseEvent) : void {
			Mouse.cursor = MouseCursor.AUTO;
			/*
			DrawTrivial();
			*/
		}
		
		private function OnMouseDown(_event : MouseEvent) : void {
			mouseIsDown = true;
			DrawExtended();
			prevMouseX = parent.mouseX;
			prevMouseY = parent.mouseY;
			prevRotation = currentRotation;
		}
		
		private function OnMouseUp(_event : MouseEvent) : void {
			//Main.debugWindow.AddString("Mouse is Up");
			Mouse.cursor = MouseCursor.AUTO;
			mouseIsDown = false;
			DrawTrivial();
		}
		
		private function OnMouseMove(_event : MouseEvent) : void {
			Mouse.cursor = MouseCursor.HAND;
			if (mouseIsDown && _event.buttonDown) {
				var deltaX : Number = parent.mouseX - prevMouseX,
					deltaY : Number = parent.mouseY - prevMouseY;
				prevRotation -= (deltaY + deltaX) * 6;
				if ((prevRotation - currentRotation) > deltaRotation) {
					SetRotation(currentRotation + deltaRotation);
					prevRotation -= deltaRotation;
					dispatchEvent(new RegulatorEvent(RegulatorEvent.INCREASE, this));
				} else if ((currentRotation - prevRotation) > deltaRotation) {
					SetRotation(currentRotation - deltaRotation);
					prevRotation += deltaRotation;
					dispatchEvent(new RegulatorEvent(RegulatorEvent.DECREASE, this));
				}
				
				prevMouseX = parent.mouseX;
				prevMouseY = parent.mouseY;
			}
		}
		
		private function SetRotation(_rotation : Number) : void {
			currentRotation = _rotation;
			_rotation %= 360;
			if (_rotation > 180) {
				_rotation -= 360;
			}
			spritePoint.rotation = _rotation;
		}
		
	}

}