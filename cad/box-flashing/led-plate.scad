
use <../lib/lib.scad>

use <./test-adapter-plate.scad>
use <./test-adapter-box.scad>

function ledPlateWidth() = taBoxWidth();
function ledPlateDepth() = 30;
function ledPlateHeight() = taPlateHeight();

module ledPlate()
{
	difference()
	//union()
	{
		rcube
		(
			[
				ledPlateWidth(),
				ledPlateDepth(),
				ledPlateHeight()
			],
			aX = 1,
			aY = 1,
			r = 0
		);

		cmirror()
		translate
		([
			0.5 * ledPlateWidth() - 5,
			-0.5 * ledPlateDepth() + 5,
			-10
		])
		cylinder(h = 20, d = 3);

		translate([-15, 0, -10])
		cylinder(h = 20, d = 5);
	}

}

