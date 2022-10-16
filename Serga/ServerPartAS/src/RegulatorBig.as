package {
	
	public class RegulatorBig extends Regulator {

		public static const RANGE_0 : int = 0,	RANGE_1 : int = 1,	TBASE : int = 2;
							
		public function RegulatorBig(_type : int, _x : int = 0, _y : int = 0) {
			super(_type, 30 * CONST.MAIN_K, 360 / 6, _x, _y);
			_names_ = new Array(
							"RegulatorRange0",	"RegulatorRange1", "RegulatorTBase"
			);
		}
	}
}