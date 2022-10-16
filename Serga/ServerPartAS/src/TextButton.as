package
{
	import flash.display.*;
	import flash.events.*;
	import adobe.utils.*;
	
	public class TextButton extends GUIelement {	
		
		public static const MENU : int = 0,			F1 : int = 1,			F2 : int = 2,			F3 : int = 3,			F4 : int = 4,
							F5 : int = 5,			SERVICE : int = 6,		DISPLAY : int = 7,		MEMORY : int = 8,		CURSOR : int = 9,
							AUTO_MEAS : int = 10,	INF : int = 11,			CHAN_0 : int = 12,		CHAN_1 : int = 13,		TIME_MENU : int = 14,
							TRIG_MENU : int = 15,	START : int = 16;

		private var format:TextButtonFormat = new TextButtonFormat();
		
		private var button:CustomButton;

		public function TextButton(_type : int, _text:String = "")
		{
			super(GUIelement.TEXT_BUTTON, _type);
			
			_names_ = new Array(
							"ButtonMenu",			"ButtonF1",				"ButtonF2",				"ButtonF3",				"ButtonF4",
							"ButtonF5",				"ButtonService",		"ButtonDisplay",		"ButtonMemory",			"ButtonCursor",
							"ButtonAutoMeasures",	"ButtonInfo",			"ButtonChannel 0",		"ButtonChannel1",		"ButtonTimeMenu",
							"ButtonTrig Menu",		"ButtonStart"
			);

			button = new CustomButton(format, _text);
			addChild(button);
			button.addEventListener(MouseEvent.MOUSE_DOWN, OnMouseDown);
			button.addEventListener(MouseEvent.MOUSE_UP, OnMouseUp);
		}
		
		private function OnMouseUp(_event : MouseEvent) : void {
			dispatchEvent(new TextButtonEvent(TextButtonEvent.UP, this));
		}
		
		private function OnMouseDown(_event : MouseEvent) : void {
			dispatchEvent(new TextButtonEvent(TextButtonEvent.DOWN, this));
		}
		
		public function GetFormat() : TextButtonFormat
		{
			return format;
		}
		
		public function SetFormat(_nameFormat : uint) : void
		{
			format = new TextButtonFormat(_nameFormat);
			button.SetFormat(format);
		}
		
		public function SetPosition(_x:int, _y:int) : void
		{
			x = _x;
			y = _y;
		}
		
		public function SetFont(_nameFont:String) : void
		{
			format.nameFont = _nameFont;
			button.SetFormat(format);
		}
		
		public function SetSizeFont(_sizeFont:uint) : void
		{
			format.sizeFont = _sizeFont;
			button.SetFormat(format);
		}
		
		public function SetBorderColorAll(_color:uint) : void
		{
			format.colorsBorder[0] = format.colorsBorder[1] = format.colorsBorder[2] = format.colorsBorder[3] = _color;
			button.SetFormat(format);
		}
		
		public function SetSize(_width:uint, _height:uint) : void
		{
			format.width = _width;
			format.height = _height;
			button.SetFormat(format);
		}
		
		public function SetWidthBorder(_widthBorder:uint) : void
		{
			format.widthBorder = _widthBorder;
			button.SetFormat(format);
		}
		
		public function GetWidth() : uint
		{
			return format.width;
		}
		
		public function GetHeight() : uint
		{
			return format.height;
		}
		
		public function SetAlpha(_alpha:uint) : void
		{
			format.alpha = _alpha;
			button.SetFormat(format);
		}
	}
}

	import flash.display.*;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextLineMetrics;
	import flashx.textLayout.accessibility.TextAccImpl;
	import flash.text.TextFieldAutoSize;
	import flash.events.MouseEvent;
	
	class CustomButton extends SimpleButton {
		
		private var format : TextButtonFormat,
					buttons : Array = new Array;
		
		public function CustomButton(_format:TextButtonFormat, _text:String) {
			format = _format;
			downState = new ButtonDisplayState(TextButtonFormat.STATE_DOWN, format, _text);
			overState = new ButtonDisplayState(TextButtonFormat.STATE_OVER, format, _text);
			upState = new ButtonDisplayState(TextButtonFormat.STATE_UP, format, _text);
			hitTestState = new ButtonDisplayState(TextButtonFormat.STATE_HINT, format, _text);
			
			buttons[0] = downState;
			buttons[1] = overState;
			buttons[2] = upState;
			buttons[3] = hitTestState;
		}
		
		public function SetFormat(_format:TextButtonFormat) : void
		{
			for each(var button:ButtonDisplayState in buttons)
				button.SetFormat(_format);
		}
	}
	
	class ButtonDisplayState extends Sprite
	{
		private var text:TextField = new TextField;
		private var state:uint;
		private var format:TextButtonFormat;
		
		public function ButtonDisplayState(_state:uint, _format:TextButtonFormat, _text:String) : void
		{
			super();
			state = _state;
			format = _format;
			text.text = _text;
			Draw();
			
			addChild(text);
		}
		
		private function Draw() : void
		{
			graphics.clear();
			graphics.beginFill(format.colorsBackground[state], format.alpha);
			graphics.lineStyle(format.widthBorder, format.colorsBorder[state]);
			graphics.drawRoundRect(0, 0, format.width, format.height, format.round, format.round);
			graphics.endFill();
			text.width = format.width;
			text.height = format.height;
			text.mouseEnabled = false;
			var textFormat : TextFormat = text.getTextFormat();
			textFormat.size = format.sizeFont;
			textFormat.font = format.nameFont;
			textFormat.align = "center";
			text.setTextFormat(textFormat);
			var metrics : TextLineMetrics = text.getLineMetrics(0);
            text.y = (text.height + metrics.descent) / 2 - (metrics.height + metrics.descent * 2) / 2;
		} 
		
		public function SetFormat(_format:TextButtonFormat) : void
		{
			format = _format;
			Draw();
		}
	}