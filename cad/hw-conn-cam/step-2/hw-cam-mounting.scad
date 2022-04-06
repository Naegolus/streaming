
$fa = 1.0;
$fs = 0.4;

use <../../lib/lib.scad>

// cylinder(h = 60.2, d = 17);
// cube([70, 26, 5]);
// linear_extrude(30)
// polygon(c);
// translate([40, 8, 5])
// rotate([180, 0, 0])
// difference()
// union()

difference()
//union()
{
	union()
	{
		rotate([0, 0, 90])
		rotate([90, 0, 0])
		rcube
		(
			[20, 100, 8.5],
			r = 8,
			singleSided = true,
			aX = 1
		);

		translate([-5, 0, 0])
		rotate([90, 0, 0])
		rcube
		(
			[20, 65, 12],
			r = 8,
			flat = false,
			singleSided = true,
			aX = 1,
			aZ = 1
		);

		translate([32, 0, 0])
			cylinder(h = 40, d = 32);

		// bridge
		translate([0, -10, 0])
			cube([40, 20, 35]);

		// chamfer
		translate([8.5, 0, 35])
		chamfer
		(
			[5, 20, 5],
			aY = 1
		);
	}

	translate([-10, 0, 90])
		rotate([0, 90, 0])
			cylinder(h = 20, d = 4.2);

	translate([32, 0, -10])
		cylinder(h = 60.0, d = 23.6);
}

