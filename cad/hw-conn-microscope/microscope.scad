
use <../lib/lib.scad>

function microscopeWidth() = 60;
function microscopeDepth() = 60;
function microscopeHeight() = 55;

function microscopeMountingHoleD() = 6;

function microscopeMountZ() = - 16 - 4;
function microscopeMountDia() = 50;

module microscope()
{
	translate([0, 0, -microscopeMountZ()])
	union()
	{
		difference()
		{
			translate
			([
				-microscopeWidth() / 2,
				-microscopeDepth() / 2,
				-4
			])
			cube([microscopeWidth(), microscopeDepth(), microscopeHeight()]);

			cmirror()
			translate([microscopeWidth() / 2 - 5, 0, 0])
			rotate([0, 90, 0])
			cylinder(h = 10.0, d = microscopeMountingHoleD());
		}

		rotate([180, 0, 0])
		cylinder(h = 100.0, d = 40);

		translate([0, 0, microscopeMountZ()])
		cylinder(h = 3, d = 58);

		translate([0, 0, - 16 - 18.5 - 4])
		cylinder(h = 18.5, d = microscopeMountDia());
	}
}

