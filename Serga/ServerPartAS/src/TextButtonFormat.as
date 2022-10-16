package 
{
	public class TextButtonFormat
	{
		static public const STATE_UP:uint = 0;
		static public const STATE_OVER:uint = 1;
		static public const STATE_DOWN:uint = 2;
		static public const STATE_HINT:uint = 3;
		
        static public const FORMAT_MAIN_MENU_BUTTON:uint = 0;
		static public const FORMAT_SCREEN_OPTIONS_BUTTON:uint = 1;
		static public const FORMAT_GAME_BUTTON:uint = 2;
		static public const FORMAT_GAME_OPTIONS_BUTTON:uint = 3;
		
		static public const F1_F5 : uint = 4;
		static public const BUTTONS : uint = 5;
		static public const ADD_BUTTONS : uint = 6;
		
		public var width:uint = 150;
		public var height:uint = 30;
		public var round:int = 0;
		public var sizeFont:int = 18;
		public var nameFont:String = "Terminal";
		public var widthBorder:uint = 5;
		public var alpha : Number = 1;
		
		public var colorsBackground:Array = [0xffcc00, 0xccff00, 0x00ccff];
		public var colorsBorder:Array = [0xffffff];
		public var colorsText:Array = [0];
		
		public function TextButtonFormat(_typeFormat:uint = 9999)
		{
			colorsBackground[3] = colorsBackground[2];
			colorsBorder[1] = colorsBorder[2] = colorsBorder[3] = colorsBorder[0];
			colorsText[1] = colorsText[2] = colorsText[3] = colorsText[0];
			
            if (_typeFormat == FORMAT_MAIN_MENU_BUTTON)
            {
                sizeFont = 20;
                nameFont = "Terminal";
                round = 5;
            }
			else if (_typeFormat == FORMAT_SCREEN_OPTIONS_BUTTON)
			{
				sizeFont = 20;
                nameFont = "Terminal";
			}
			else if (_typeFormat == FORMAT_GAME_BUTTON)
			{
				colorsBorder[0] = colorsBorder[1] = colorsBorder[2] = colorsBorder[3] = 0xffffff;
                widthBorder = 1;
                width = 75;
                height = 25;
                sizeFont = 14;
			}
			else if (_typeFormat == FORMAT_GAME_OPTIONS_BUTTON)
			{
				colorsBorder[0] = colorsBorder[1] = colorsBorder[2] = colorsBorder[3] = 0xffffff;
                widthBorder = 1;
                sizeFont = 14;
                width = 100;
                height = 30;
			} else if (_typeFormat == F1_F5) {
				colorsBorder[STATE_UP] = colorsBorder[STATE_OVER] = colorsBorder[STATE_DOWN] = colorsBorder[STATE_HINT] = Color.BUTTON_BORDER;
				colorsBackground[STATE_UP] = Color.F_BACKGROUND_UP;
				colorsBackground[STATE_OVER] = Color.F_BACKGROUND_OVER;
				colorsBackground[STATE_DOWN] = Color.F_BACKGROUND_DOWN;
				colorsBackground[STATE_HINT] = 0x0;
				
				colorsText[STATE_UP] = colorsText[STATE_OVER] = colorsText[STATE_DOWN] = colorsText[STATE_HINT] = Color.F_FONT;
				widthBorder = 2;
				round = 15 * CONST.MAIN_K;
				width = 45 * CONST.MAIN_K;
				height = 22 * CONST.MAIN_K;
				sizeFont = 12 * CONST.MAIN_K;
			} else if (_typeFormat == BUTTONS) {
				colorsBorder[STATE_UP] = colorsBorder[STATE_OVER] = colorsBorder[STATE_DOWN] = colorsBorder[STATE_HINT] = Color.BUTTON_BORDER;
				colorsBackground[STATE_UP] = Color.BUTTON_BACKGROUND_UP;
				colorsBackground[STATE_OVER] = Color.BUTTON_BACKGROUND_OVER;
				colorsBackground[STATE_DOWN] = Color.BUTTON_BACKGROUND_DOWN;
				colorsBackground[STATE_HINT] = 0x0;
				
				colorsText[STATE_UP] = colorsText[STATE_OVER] = colorsText[STATE_DOWN] = colorsText[STATE_HINT] = Color.BUTTON_FONT;
				
				widthBorder = 2;
				round = 15 * CONST.MAIN_K;
				width = 43 * CONST.MAIN_K;
				height = 28 * CONST.MAIN_K;
				sizeFont = 8 * CONST.MAIN_K;
			} else if (_typeFormat == ADD_BUTTONS) {
				colorsBorder[STATE_UP] = colorsBorder[STATE_OVER] = colorsBorder[STATE_DOWN] = colorsBorder[STATE_HINT] = Color.BUTTON_BORDER;
				colorsBackground[STATE_UP] = Color.BUTTON_BACKGROUND_UP;
				colorsBackground[STATE_OVER] = Color.BUTTON_BACKGROUND_OVER;
				colorsBackground[STATE_DOWN] = Color.BUTTON_BACKGROUND_DOWN;
				colorsBackground[STATE_HINT] = 0x0;
				
				colorsText[STATE_UP] = colorsText[STATE_OVER] = colorsText[STATE_DOWN] = colorsText[STATE_HINT] = Color.BUTTON_FONT;
				
				widthBorder = 2;
				round = 15 * CONST.MAIN_K;
				width = 43 * CONST.MAIN_K;
				height = 28 * CONST.MAIN_K;
				sizeFont = 8 * CONST.MAIN_K;				
			}
		}
	}
}