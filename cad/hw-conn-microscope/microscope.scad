
use <../lib/lib.scad>

function microscopeWidth() = 60;
function microscopeDepth() = 60;
function microscopeHeight() = 55;

function microscopeMountingHoleD() = 6;

module microscope()
{
	translate([-microscopeWidth() / 2, 0, 0])
	union()
	{
		difference()
		{
			translate([-microscopeWidth() / 2, -microscopeDepth() / 2, -4])
			cube([microscopeWidth(), microscopeDepth(), microscopeHeight()]);

			cmirror()
			translate([microscopeWidth() / 2 - 5, 0, 0])
			rotate([0, 90, 0])
			cylinder(h = 10.0, d = microscopeMountingHoleD());
		}

		rotate([180, 0, 0])
		cylinder(h = 100.0, d = 40);
	}
}

