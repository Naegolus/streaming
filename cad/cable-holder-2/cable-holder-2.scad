
$fa = 1.0;
$fs = 0.2;

use <../lib/lib.scad>

holeDiameter = 4.5;

module asztal()
{
	cube([50, 50, 10]);
}

module screwCut()
{
	translate([0, 0, -20])
	cylinder(d = 4, h = 40);

	cylinder(d = 8, h = 20);
}

module cableHolder2
(
)
{
	difference()
	{
		union()
		{
			translate([0, 0, 15])
			rotate([0, 90, 0])
			rcube
			(
				[15, 40, 12],
				r = 4,
				flat = true,
				singleSided = true,
				aX = 0,
				aY = 0,
				aZ = 0
			);

			translate([7, 0, 15])
			mirror([0, 1, 0])
			rotate([0, 90, 0])
			rcube
			(
				[15, 25, 5],
				r = 4,
				flat = true,
				singleSided = true,
				aX = 0,
				aY = 0,
				aZ = 0
			);

			translate([12, -15, 0])
			rotate([90, 0, -90])
			rcube
			(
				[10, 30, 5],
				r = 4,
				flat = true,
				singleSided = true,
				aX = 0,
				aY = 0,
				aZ = 0
			);
		}

		translate([3, 30, 7.5])
		rotate([0, 90, 0])
		screwCut();

		translate([3, 10, 7.5])
		rotate([0, 90, 0])
		screwCut();
	}
}

