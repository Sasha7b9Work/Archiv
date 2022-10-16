package  
{

	public class Color 
	{
		public static const BUTTON_BORDER : uint = 0x202020,
							F_BACKGROUND_UP : uint = 0xefefef,
							F_BACKGROUND_OVER : uint = 0xbfbfbf,
							F_BACKGROUND_DOWN : uint = 0x7f7f7f,
							F_FONT : uint = 0x0,
							BUTTON_BACKGROUND_UP : uint = 0xafefef,
							BUTTON_BACKGROUND_OVER : uint = 0xbfbf00,
							BUTTON_BACKGROUND_DOWN : uint = 0x7f7f00,
							BUTTON_FONT : uint = 0x00007f,
							REGULATOR : uint = 0xafafaa,
							
							// For display, 15 items
							WHITE : uint = RGB(255, 255, 255),
							BLACK : uint = RGB(0, 0, 0),
							R211 : uint = RGB(211, 197, 250),
							R238 : uint = RGB(238, 119, 134),
							R048 : uint = RGB(48, 24, 27),
							R064 : uint = RGB(64, 64, 96),
							R154 : uint = RGB(154, 154, 230),
							R128 : uint = RGB(128, 128, 192);
							
		public function Color() 
		{
			
		}
		
		static public function RGB(_r : int, _g : int, _b : int) : uint {
			return (_r << 16) | (_g << 8) | (_b);
		}
		
	}

}