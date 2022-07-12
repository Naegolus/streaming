
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>

use <./test-adapter-plate.scad>
use <./test-adapter-box.scad>

function plateSideDim() =
[
	taBoxWidth() - taPlateWidth(),
	taBoxDepth(),
	taPlateHeight(),
];

module plateSide()
{
	difference()
	//union()
	{
		rcube
		(
			plateSideDim(),
			aX = 1,
			aY = 1,
			r = 0
		);

		cmirror([0, 1, 0])
		translate
		([
			0.5 * plateSideDim().x - 5,
			-0.5 * plateSideDim().y + 5,
			-10
		])
		cylinder(h = 20, d = 3);
	}

}

// plateSide();
