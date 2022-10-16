package  
{
	public class RegulatorSmall extends Regulator {
		
		public static const SET : int = 0,	RSHIFT_0 : int = 1,	RSHIFT_1 : int = 2,	TSHIFT : int = 3,	TRIG_LEV : int = 4;
		
		public function RegulatorSmall(_type : int, _x : int = 0, _y : int = 0) {
			super(_type, 20 * CONST.MAIN_K, 360 / 18, _x, _y);
			
			_names_ = new Array(
							"RegulatorSet",	"RegulatorRShift0",	"RegulatorRShift1",	"RegulatorTShift",	"RegulatorTrigLev"
			);
		}
		
	}

}