package  {
	
	import flash.display.*;
	import flash.text.*;
	import flash.text.engine.*;
	
	public class Image extends Sprite {
		
		private var _colorBackground : uint = 0xffffff,
					_alpha : Number = 1.0,
					_colorDraw : uint = 0,
					_thickness : uint = 1;
		
		public function Image() {
			
		}
		
		public function Clear() : void {
			removeChildren(0);
		}
		
		public function SetDrawStyle(_colorBackground_ : uint = 0xffffff, _colorDraw_ : uint = 0x0, _thickness_ : uint = 1, _alpha_ : Number = 1.0) : void {
			_colorBackground = _colorBackground_;
			_thickness = _thickness_;
			_alpha = _alpha_;
			_colorDraw = _colorDraw_;
		}
		
		public function get colorBackground() : uint {
			return _colorBackground;
		}
		
		public function set colorBackground(_color : uint) : void {
			_colorBackground = _color;
		}
		
		override public function get alpha() : Number {
			return _alpha;
		}
		
		override public function set alpha(_a : Number) : void {
			_alpha = _a;
		}
		
		public function get colorDraw() : uint {
			return _colorDraw;
		}
		
		public function set colorDraw(_color : uint) : void {
			_colorDraw = _color;
		}
		
		public function get thickness() : uint {
			return _thickness;
		}
		
		public function set thickness(_val : uint) : void {
			_thickness = _val;
		}
		
		public function DrawRoundRect(_x : int, _y : int, _width : int, _height : int, _roundW : int, _roundH : int) : void {
			var sprite : Sprite = SetParameters();
			sprite.graphics.drawRoundRect(_x, _y, _width, _height, _roundH, _roundW);
			AddSprite(sprite);
		}
		
		public function DrawCorneredRect(_x : int, _y : int, _width : int, _height : int, _corner : int) : void {
			var sprite : Sprite = SetParameters();
			
		}
		
		public function DrawLine(_x0 : int, _y0 : int, _x1 : int, _y1 : int) : void {
			var sprite : Sprite = SetParameters();
			sprite.graphics.moveTo(_x0, _y0);
			sprite.graphics.lineTo(_x1, _y1);
			AddSprite(sprite);
		}
				
		public function DrawRect(_x : int, _y : int, _width : int, _height : int) : void {
			var sprite : Sprite = SetParameters();
			sprite.graphics.drawRect(_x, _y, _width, _height);
			AddSprite(sprite);
		}
		
		public function DrawText(_string : String, _x : int, _y : int, _width : int = 0, _height : int = 0, _color : uint = 0, _align : String = "center", _sizeFont : int = CONST.SIZE_MAIN_FONT, _nameFont : String = CONST.NAME_MAIN_FONT) : void {
			var sprite : Sprite = SetParameters();
			var textField : TextField = GF.GetTextField(_string, _x, _y, _width, _height, _color, _align, _sizeFont, _nameFont);
			textField.background = false;
			sprite.addChild(textField);
			AddSprite(sprite);
		}
		
		private function AddSprite(_sprite : Sprite) : void {
			_sprite.graphics.endFill();
			addChild(_sprite);
			
			var bmData : BitmapData = new BitmapData(width, height, true, 0);
			bmData.draw(this);
			
			var bitmap : Bitmap = new Bitmap(bmData);

			removeChildren(1);
			removeChildren(0);
			addChild(bitmap);
		}
		
		public function DrawImage(_image : Image) : void {
			var bm : Bitmap = _image.BitMap();
			bm.x = _image.x;
			bm.y = _image.y;
			addChild(bm);
			
			var bmData : BitmapData = new BitmapData(width, height, true, 0);
			bmData.draw(this);
			
			var bitmap : Bitmap = new Bitmap(bmData);
			
			removeChildren(1);
			removeChildren(0);
			addChild(bitmap);
		}
		
		private function SetParameters() : Sprite {
			var sprite : Sprite = new Sprite();
			sprite.graphics.beginFill(_colorBackground, _alpha);
			sprite.graphics.lineStyle(_thickness, _colorDraw, _alpha);
			return sprite;
		}
		
		public function BitMap() : Bitmap {
			return getChildAt(0) as Bitmap;
		}
	}

}