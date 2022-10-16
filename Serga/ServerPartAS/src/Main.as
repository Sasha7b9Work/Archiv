package 
{
	import flash.display.*;
	import flash.net.*;
	import flash.text.TextField;
	import flash.ui.*;
	import flash.events.*;
	
	public class Main extends Sprite 
	{	
		public static const WIDTH : int = 1000,
							HEIGHT : int = 400;
							
		public static var	STAGE : Stage,
							debugWindow : DebugWindow,
							controlPanel : ControlPanel,
							transmitter : Transmitter = new Transmitter();
							
		private var display : Display,
					menu : Menu,
					bMode : SwitcherButton,
					bSource : SwitcherButton;
		
		public function Main():void   
		{
			if (stage) {
				Init();
			}
			else {
				addEventListener(Event.ADDED_TO_STAGE, init);
			}
		}
		
		private function Init(e:Event = null):void 
		{
			STAGE = stage;
			
			stage.quality = StageQuality.BEST;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			removeEventListener(Event.ADDED_TO_STAGE, init);

			addChild(display = new Display());
			menu = new Menu(display);
			display.x = 200;
			display.y = 150;
			addChild(controlPanel = new ControlPanel(this));
			controlPanel.x = 500;
			
			addChild(debugWindow = new DebugWindow());
			debugWindow.x = debugWindow.y = 10;
			
			var _x0 : int = 10,
				_x1 : int = 70,
				_width : int = 60,
				_deltaY : int = 20;

			var text : TextField = GF.GetTextField("Реж. раб.", _x0, 150);
			addChild(text);
			
			var text1 : TextField = GF.GetTextField("Ист. сигн.", _x0, text.y + _deltaY);
			addChild(text1);

			addChild(bMode = new SwitcherButton(SwitcherButton.MODE_WORK, "Автономн.", "Реальный"));
			bMode.x = _x1;
			bMode.y = 150;
			
			addChild(bSource = new SwitcherButton(SwitcherButton.SOURCE, "Эмулятор", "АЦП"));
			bSource.x = _x1;
			bSource.y = bMode.y + _deltaY;

			DrawPackage();
			
			bMode.addEventListener(SwitcherButtonEvent.SWITCHING, OnSwitchingButtonMode);
		}
		
		public function OnPressButton(_button : TextButton) : void {
			//trace("Click button " + _button.Name() + " id = " + _button.ID() + " subType = " + _button.SubType());
			menu.PressButton(_button.SubType());
		}
		
		public function OnReleaseButton(_button : TextButton) : void {
			menu.ReleaseButton(_button.SubType());
		}
		
		public function OnRotateRegulatorDecrease(_regulator : Regulator) : void {
			//trace("Rotate regulator decrease " + _regulator.Name() + " id = " + _regulator.ID() + " subType = " + _regulator.SubType());
		}
		
		public function OnRotateRegulatorIncrease(_regulator : Regulator) : void {
			//trace("Rotate regulator increase " + _regulator.Name() + " id = " + _regulator.ID() + " subType = " + _regulator.SubType());
		}
		
		private function OnSwitchingButtonMode(_event : SwitcherButtonEvent) : void {
			if (_event.state == 0) {
				transmitter.Disconnect();
			} else {
				transmitter.Connect();
			}
		}
		
		private function OnSwitchingButtonSource(_event : SwitcherButtonEvent) : void {
			
		}
		
		private function DrawPackage() : void {
			graphics.beginFill(CONST.COLOR_PACKAGE);
			graphics.lineStyle(3, 0x000000);
			graphics.drawRoundRect(1, 1, WIDTH, HEIGHT, 15, 15);
			graphics.endFill(); 
		}
	}
	
}