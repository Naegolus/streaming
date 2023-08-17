
$fa = 1.0;
$fs = 0.2;

use <../lib/lib.scad>

holeDiameter = 4.5;

module cableHolder
(
)
{
	//union()
	difference()
	{
		union()
		{
			cube([20, 3, 20]);

			translate([-6, -3, 0])
			cube([6, 30, 20]);

			translate([0, 7, 0])
			rotate([0, 0, 90])
			rcube
			(
				[30, 26, 20],
				r = 4,
				flat = true,
				singleSided = false,
				aX = 0,
				aY = 0,
				aZ = 0
			);

			translate([0, 3, 0])
			rotate([90, 0, 0])
			chamfer
			(
				[5, 20, 5],
				round = true,
				rotation = 1,
				aX = 0,
				aY = 0,
				aZ = 0
			);
		}

		color([0.2, 0.5, 0.0, 0.6])
		union()
		{
			translate([10, 6, 10])
			rotate([90, 0, 0])
				cylinder(h = 9, d = 4.5);

			translate([-6, 11, -2])
			rotate([0, 0, 90])
			rcube
			(
				[22, 16, 24],
				r = 4,
				flat = true,
				singleSided = false,
				aX = 0,
				aY = 0,
				aZ = 0
			);

			translate([-12, 4, -2])
			cube([6, 10, 24]);

		}
	}

	if (false)
	{
	rcube
	(
		[15, 20, 5],
		r = 4,
		flat = false,
		singleSided = true,
		aX = 0,
		aY = 0,
		aZ = 0
	);
	}
}

