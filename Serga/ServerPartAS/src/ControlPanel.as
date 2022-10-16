package  {
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.display.InteractiveObject;
	import flash.display.Sprite;
	import flash.text.StaticText;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.events.MouseEvent;
	
	public class ControlPanel extends Sprite {
		
		private var buttonsF : Array = new Array(),
					buttonMenu : TextButton,
					buttonCursor : TextButton,
					buttonAutoMeasures : TextButton,
					buttonInf : TextButton,
					buttonService : TextButton,
					buttonDisplay : TextButton,
					buttonMemory : TextButton,
					buttonChan0 : TextButton,
					buttonChan1 : TextButton,
					buttonTime : TextButton,
					buttonTrig : TextButton,
					buttonStart : TextButton,
					regRShift0 : RegulatorSmall,
					regRShift1 : RegulatorSmall,
					regTShift : RegulatorSmall,
					regTrigLev : RegulatorSmall,
					regSet : RegulatorSmall,
					regRange0 : RegulatorBig,
					regRange1 : RegulatorBig,
					regTBase : RegulatorBig,
					buttons : Array = new Array(),
					regulators : Array = new Array(),
					
					mouseDown : Boolean = false,
					prevMouseX : int,
					prevMouseY : int,
					
					main : Main;
		
		public function ControlPanel(_main : Main) {
			
			main = _main;
			
			var sprite : Sprite = new Sprite();
			
			var textMenu : TextField = GF.GetTextField("МЕНЮ", 46, 15);
			sprite.addChild(textMenu);
			
			buttonMenu = new TextButton(TextButton.MENU);
			buttonMenu.SetFormat(TextButtonFormat.F1_F5);
			buttonMenu.y = 35;
			buttonMenu.x = 45;
			
			var textSet : TextField = GF.GetTextField("УСТАНОВКА", textMenu.x - 15, buttonMenu.y + 40);
			sprite.addChild(textSet);
			
			regSet = new RegulatorSmall(RegulatorSmall.SET, buttonMenu.x + 22, 120);
			
			var ides : Array = new Array(0, TextButton.F1, TextButton.F2, TextButton.F3, TextButton.F4, TextButton.F5);
			
			for (var i : int = 1; i <= 5; i++) {
				var button : TextButton = new TextButton(ides[i]);
				button.SetFormat(TextButtonFormat.F1_F5);
				button.y = buttonMenu.y + 135 + (i - 1 + 0.65) * Main.HEIGHT / 11.7;
				//sprite.addChild(GF.GetTextField("F" + i.toString(), 10, button.y - 1, 25, 0, 0, "center", 40));
				button.x += buttonMenu.x;
				buttonsF.push(button);
				AddButton(button);
			}

			DrawButtons(sprite, buttonMenu.x + 80, buttonMenu.y - 15, "ОСНОВНЫЕ", 
														"СЕРВИС", 55, buttonService = new TextButton(TextButton.SERVICE), 
														"ДИСПЛЕЙ", 0, buttonDisplay = new TextButton(TextButton.DISPLAY), 
														"ПАМЯТЬ", 0, buttonMemory = new TextButton(TextButton.MEMORY));
			DrawButtons(sprite, buttonMenu.x + 80, buttonMenu.y + 55, "ИЗМЕРЕНИЯ", 
														"КУРСОР", 55, buttonCursor = new TextButton(TextButton.CURSOR), 
														"АВТ.ИЗМ.", 0, buttonAutoMeasures = new TextButton(TextButton.AUTO_MEAS), 
														"СБОР ИНФ", 0, buttonInf = new TextButton(TextButton.INF));
														
			var deltaX : int = 95;
			var startX : int = buttonMenu.x + 110;
			var startY : int = buttonMenu.y + 170;
			
			textField = GF.GetTextField("     -------------Y--------------                            X                       СИНХР.", startX - 30, startY - 45, 0, 0, 0, "left");
			sprite.addChild(textField);
			textField.width = 500;
			
			DrawRegulators(sprite, startX, startY, regRShift0				= new RegulatorSmall(RegulatorSmall.RSHIFT_0),	"КАНАЛ 1",	buttonChan0 = new TextButton(TextButton.CHAN_0),	"ВОЛЬТ/ДЕЛ", regRange0 = new RegulatorBig(RegulatorBig.RANGE_0));
			DrawRegulators(sprite, startX + deltaX, startY, regRShift1		= new RegulatorSmall(RegulatorSmall.RSHIFT_1),	"КАНАЛ 2",	buttonChan1 = new TextButton(TextButton.CHAN_1),	"ВОЛЬТ/ДЕЛ", regRange1 = new RegulatorBig(RegulatorBig.RANGE_1));
			DrawRegulators(sprite, startX + 2 * deltaX, startY, regTShift	= new RegulatorSmall(RegulatorSmall.TSHIFT),	"МЕНЮ",		buttonTime = new TextButton(TextButton.TIME_MENU),	"ВРЕМЯ/ДЕЛ", regTBase = new RegulatorBig(RegulatorBig.TBASE));
			DrawRegulators(sprite, startX + 3 * deltaX, startY, regTrigLev	= new RegulatorSmall(RegulatorSmall.TRIG_LEV),	"МЕНЮ",		buttonTrig = new TextButton(TextButton.TRIG_MENU));
			
			var textField : TextField;
			sprite.addChild(textField = GF.GetTextField("ПУСК/СТОП", startX + 265 - 29, startY + 120 - 25, 100));
			sprite.addChild(textField = GF.GetTextField("ПУСК/СТОП", startX + 265 - 29, startY + 120 - 25, 100));
			
			buttonStart = new TextButton(TextButton.START);
			buttonStart.SetFormat(TextButtonFormat.ADD_BUTTONS);
			buttonStart.x = textField.x + 29;
			buttonStart.y = textField.y + 25;
			
			sprite.graphics.lineStyle(1, 0);
			var x1 : int = startX + 142;
			var x2 : int = startX + 237;
			var y1 : int = startY - 20;
			var y2 : int = startY + 160;
			sprite.graphics.moveTo(x1, y1);
			sprite.graphics.lineTo(x1, y2);
			sprite.graphics.moveTo(x2, y1);
			sprite.graphics.lineTo(x2, y2);
			
			sprite.height += 20;
			
			var bmData : BitmapData = new BitmapData(sprite.width, sprite.height, true, 0xffffff);
			bmData.draw(sprite);
			var bitmap : Bitmap = new Bitmap(bmData);
			addChild(bitmap);

			AddButton(buttonMenu);
			AddButton(buttonCursor);
			AddButton(buttonAutoMeasures);
			AddButton(buttonInf);
			AddButton(buttonService);
			AddButton(buttonDisplay);
			AddButton(buttonMemory);
			AddButton(buttonChan0);
			AddButton(buttonChan1);
			AddButton(buttonTime);
			AddButton(buttonTrig);
			AddButton(buttonStart);
			AddRegulator(regRShift0);
			AddRegulator(regRShift1);
			AddRegulator(regTShift);
			AddRegulator(regTrigLev);
			AddRegulator(regSet);
			AddRegulator(regRange0);
			AddRegulator(regRange1);
			AddRegulator(regTBase);
		}
		
		private function OnButtonEvent(_event : TextButtonEvent) : void {
			if(_event.type == TextButtonEvent.DOWN) {
				main.OnPressButton(_event.button);
			} else {
				main.OnReleaseButton(_event.button);
			}
		}
		
		private function OnRegulatorEvent(_event : RegulatorEvent) : void {
			if (_event.type == RegulatorEvent.DECREASE) {
				main.OnRotateRegulatorDecrease(_event.regulator);
			} else {
				main.OnRotateRegulatorIncrease(_event.regulator);
			}
		}
		
		private function DrawRegulators(_sprite : Sprite, _startX : int, _startY : int, _regSmall : RegulatorSmall, _nameButton : String, _button : TextButton, _nameRegBig : String = "", _regBig : RegulatorBig = undefined) : void {
			_regSmall.x = _startX,
			_regSmall.y = _startY;
			addChild(_regSmall);
			
			_sprite.addChild(GF.GetTextField(_nameButton, _startX - 50, _startY + 30, 100));
			
			addChild(_button);
			_button.SetFormat(TextButtonFormat.ADD_BUTTONS);
			_button.x = _startX - 20;
			_button.y = _startY + 50;
			
			
			if (_regBig) {
				_sprite.addChild(GF.GetTextField(_nameRegBig, _startX - 50, _startY + 90, 100));
				_regBig.x = _startX;
				_regBig.y = _startY + 140;
				addChild(_regBig);
			}
		}
		
		private function DrawButtons(_sprite : Sprite, _startX : int, _startY : int, _name : String, _label1 : String, _delta1 : int, _button1 : TextButton, _label2 : String, _delta2 : int, _button2 : TextButton, _label3 : String, _delta3 : int, _button3 : TextButton) : void {
			_sprite.graphics.beginFill(CONST.COLOR_PACKAGE, 0.0);
			_sprite.graphics.lineStyle(1, 0x0);
			_sprite.graphics.drawRoundRect(_startX, _startY, 350, 50, 10, 10);
			_sprite.graphics.lineStyle(1, CONST.COLOR_PACKAGE);
			_sprite.graphics.moveTo(_startX + 205 - 80, _startY);
			_sprite.graphics.lineTo(_startX + 293 - 80, _startY);
			_sprite.graphics.endFill();
			
		    var delta : int = 110;
			
			_sprite.addChild(GF.GetTextField(_name, _startX + 205 - 75, _startY - 8));
			
			//////////////////////////////////////////////////////////////////////////////////////////////////
			var text1 : TextField = GF.GetTextField(_label1, _startX + _delta1, _startY + 18);
			_sprite.addChild(text1);
			
			addChild(_button1);
			_button1.SetFormat(TextButtonFormat.BUTTONS);
			_button1.x = _startX + 10;
			_button1.y = _startY + 12;
			
			/////////////////////////////////////////////////////////////////////////////////////////////////
			var text2 : TextField = GF.GetTextField(_label2, text1.x + delta + _delta2, text1.y);
			_sprite.addChild(text2);
			
			addChild(_button2);
			_button2.SetFormat(TextButtonFormat.BUTTONS);
			_button2.x = _button1.x + delta;
			_button2.y = _button1.y;
			
			//////////////////////////////////////////////////////////////////////////////////////////////////
			var text3 : TextField = GF.GetTextField(_label3, text2.x + delta + _delta3, text2.y);
			_sprite.addChild(text3);
			
			addChild(_button3);
			_button3.SetFormat(TextButtonFormat.BUTTONS);
			_button3.x = _button2.x + delta;
			_button3.y = _button2.y;
		}
		
		private function AddButton(_button : TextButton) : void {
			addChild(_button);
			buttons.push(_button);
			_button.addEventListener(TextButtonEvent.DOWN, OnButtonEvent);
			_button.addEventListener(TextButtonEvent.UP, OnButtonEvent);
		}
		
		private function AddRegulator(_regualator : Regulator) : void {
			addChild(_regualator);
			regulators.push(_regualator);
			_regualator.addEventListener(RegulatorEvent.DECREASE, OnRegulatorEvent);
			_regualator.addEventListener(RegulatorEvent.INCREASE, OnRegulatorEvent);
		}
		
	}

}