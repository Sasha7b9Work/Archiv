package  {
	import flash.text.engine.*;
	import flash.text.*;
	import flash.utils.*;
	import flash.errors.*;
	
	public class GF {
		
		public function GF() {
			
		}
		
		public static function GetTextField(_string : String, _x : int, _y : int, _width : int = 0, _height : int = 0, _color : uint = 0, _align : String = "center", _sizeFont : int = CONST.SIZE_MAIN_FONT, _nameFont : String = CONST.NAME_MAIN_FONT) : TextField {
			var textField : TextField = new TextField();
			textField.selectable = false;
			textField.text = _string;
			var textFormat : TextFormat = textField.getTextFormat();
			textFormat.font = _nameFont;
			textFormat.align = _align;
			textFormat.size = _sizeFont;
			textField.setTextFormat(textFormat);
			textField.x = _x;
			textField.y = _y;
			textField.textColor = _color;
			var metrics : TextLineMetrics = textField.getLineMetrics(0);
			if(_width == 0) {
				textField.width = metrics.width + 4;
			} else {
				textField.width = _width;
			}
			if(_height == 0) {
				textField.height = metrics.height + 2 + metrics.descent;
			} else {
				textField.height = _height * 3;
			}
			
			return textField;
		}
		
		public static function ByteArrayToStringINT(_array : ByteArray, _radix : int) : String {
			var string : String = "",
				byte0 : int,
				byte1 : int,
				byte2 : int,
				byte3 : int,
				pointer : int = 0;
			
			try {
				while(pointer < _array.length) {
					byte0 = byte1 = byte2 = byte3 = 0;
					byte3 = _array[pointer++];
					byte2 = _array[pointer++];
					byte1 = _array[pointer++];
					byte0 = _array[pointer++];
					string += (byte0 + (byte1 << 8) + (byte2 << 16) + (byte3 << 24)).toString(_radix) + " ";
				}
			} catch (_e : EOFError) {
				string += (byte0 + (byte1 << 8) + (byte2 << 16) + (byte3 << 24)).toString(_radix) + " ";
			}
			
			return string;
		}
		
		public static function DBG_MSG(_message : String) : void {
			const DO_MESSAGE : uint = 0;
			
			if (DO_MESSAGE) {
				trace(_message);
			}
		}
		
		public static function VectorBytesToString(_vector : Vector.<int>, _radix :int) : String {
			var string : String = "";
			for (var i : int = 0; i < _vector.length; i++) {
				string += _vector[i].toString(_radix) + " ";
			}
			return string;
		}
		
	}

}