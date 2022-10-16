package  
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.IBitmapDrawable;
	import flash.display.InteractiveObject;
	import flash.display.Sprite;
	
	public class Display extends Image
	{
		public static const LEFT_X : uint = 10,
						    TOP_Y : uint = LEFT_X,
							WIDTH : uint = 320,
							HEIGHT : uint = 240,
							ROUND : uint = 15,
							widthGrid : int = 301,
							heightGrid : int = 201,
							xGrid : int = (WIDTH - (widthGrid - 1)) / 2,
							yGrid : int = 10,
							RIGHT_GRID : int = xGrid + widthGrid,
							TOP_GRID : int = yGrid,
							BOTTOM_GRID : int = yGrid + heightGrid;
					
		public function Display() 
		{
			Draw();
			x = LEFT_X;
			y = TOP_Y; 
		}
		
		public function DrawGrid() : void {
			Clear();
			SetDrawStyle(0, 0);
			DrawRoundRect(0, 0, WIDTH, HEIGHT, ROUND, ROUND);
			
			var grid : Image = new Image();
			grid.SetDrawStyle(0, 0xffffff);
			
			const	deltaX : int = (widthGrid - 1)/ 12,
					deltaY : int = (heightGrid - 1) / 8;
			
			for (var i : int = 0; i < widthGrid; i += deltaX) {
				grid.DrawLine(i, 0, i, heightGrid);
			}

			for (i = 0; i < heightGrid; i += deltaY) {
				grid.DrawLine(0, i, widthGrid, i);
			}
			
			for (i  = 0; i < widthGrid; i += deltaX) {
				grid.DrawLine(i, 0, i, heightGrid);
			}
			
			grid.x = xGrid;
			grid.y = yGrid;
			DrawImage(grid);			
		}
		
		public function DrawCursors() : void {
			
		}
		
		private function Draw() : void {
			DrawGrid();
		}
		
		public function XmenuItem() : int 			{	return XmenuPage();	}
		
		public function XmenuPage() : int 			{	return widthGrid - WidthTitleMenuPage() + xGrid - 1;	}
		public function YmenuPage() : int 			{	return 10; }	
		public function WidthTitleMenuPage() : int 	{	return (widthGrid - 1) / 12 * 3;	}
	}
}