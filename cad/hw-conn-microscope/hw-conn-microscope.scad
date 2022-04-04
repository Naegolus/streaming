
use <../lib/lib.scad>
use <microscope.scad>

// x .. Width
// y .. Depth
// z .. Height

function refPtX() = -22;
function refPtY() = -40;
function refPtZ() = 38;

function forkHeight() = 16;
forkWidth = 5;
forkDepth = abs(refPtY()) + forkHeight() / 2;

module hwConnMicoscope()
{
	difference()
	{
		union()
		{
		translate([0, 0, -60])
		cylinder(h = refPtZ() + forkHeight() / 2 + 60, d = 17);

		// fork
		translate
		([
			refPtX(),
			refPtY() + forkDepth - forkHeight() / 2,
			refPtZ()
		])
		translate([-microscopeWidth() / 2, 0, 0])
		cmirror()
		translate([-microscopeWidth() / 2, 0, -forkHeight() / 2])
		rotate([0, 0, 180])
		difference()
		{
			union()
			{
				cube([forkWidth, forkDepth, forkHeight()]);

				translate([0, 2.5, 0])
				rotate([0, 0, 90])
				linear_extrude(forkHeight())
				polygon([[0, 0], [5, 0], [0, 5]]);
			}

			translate([-forkWidth, forkDepth - forkHeight() / 2, forkHeight() / 2])
			rotate([0, 90, 0])
			cylinder(h = 3 * forkWidth, d = 7);
		}

		// bridge
		rotate([0, 0, 180])
		translate([0, -2.5, 0])
		cube
		([
			abs(refPtX()) + microscopeWidth() + forkWidth,
			5,
			refPtZ() + forkHeight() / 2
		]);
		}

		translate([0, 0, -70])
		cylinder(h = refPtZ() + forkHeight() / 2 + 80, d = 10);

		translate([-30, -10, 0])
		cube([5, 20, 15]);
	}
}

