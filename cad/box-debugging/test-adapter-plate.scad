
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>

function taPlateWidth() = 68;
function taPlateDepth() = 88;
function taPlateHeight() = 2;

function screwDistX() = 58;
function screwDistY() = 78;

module testAdapterPlate()
{
	difference()
	//union()
	{
		translate
		([
			0,
			0,
			0.5 * taPlateHeight()
		])
		cube
		([
			taPlateWidth(),
			taPlateDepth(),
			taPlateHeight()
		], center = true);

		cmirror([0, 1, 0])
		cmirror()
		translate([0.5 * screwDistX(), 0.5 * screwDistY(), -taPlateHeight()])
		cylinder(h = 3 * taPlateHeight(), d = 3);
	}
}

