
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

function bridgeHeight() = 48;
function shoulderHeight() = bridgeHeight() - 5;

function microscopePosX() = 10;
function microscopePosY() = -60;
function microscopePosZ() = 15;

module holderMicro()
{
	difference()
	//union()
	{
		union()
		{
			translate
			([
				microscopePosX(),
				microscopePosY(),
				0,
			])
				cylinder(h = microscopePosZ(), d = 50 + 10);

			translate([-13, -30, 5])
			rotate([0, 0, -70])
				cube([60, 35, 10], center = true);

			translate([-14, -19, 14.5])
			rotate([0, 0, -70])
			rotate([0, 23, 0])
			cube([40, 5, 14], center = true);
		}

		translate
		([
			microscopePosX(),
			microscopePosY(),
			-1,
		])
		cylinder(h = microscopePosZ() + 2, d = 50);
	}
}

module connBase()
{
	rotate([0, 0, 180])
	difference()
	//union()
	{
		union()
		{
			rotate([0, 0, 90])
			rotate([90, 0, 0])
			rcube
			(
				[20, 100, 4.5],
				r = 8,
				singleSided = true,
				aX = 1
			);

			translate([32, 0, 0])
				cylinder(h = bridgeHeight(), d = 32);

			// bridge
			translate([0, -15, 0])
				cube([35, 30, bridgeHeight() - 5]);

			cmirror([0, 1, 0])
			translate([0, 10, bridgeHeight() - 5])
			rotate([0, 0, 90])
			chamfer
			(
				[3, 4.5, 3],
				aY = 2
			);

			rotate([0, 0, 180])
				holderMicro();
		}

		translate([-10, 0, 91])
			rotate([0, 90, 0])
				cylinder(h = 20, d = 4.2);

		translate([32, 0, -10])
			cylinder(h = 60.0, d = 23.6);

		// iron plate
		translate([4.5, -8, -1])
			cube([2.1, 16, bridgeHeight() + 2]);

		// screw nut
		translate([4.5, -4, -1])
			cube([6.1, 8, bridgeHeight() + 2]);

		translate([-10, 0, 10])
			rotate([0, 90, 0])
				cylinder(h = 20, d = 5.5);

		translate([-10, 0, 39.5])
			rotate([0, 90, 0])
				cylinder(h = 20, d = 5.5);

		translate([-10, 0, 62.5])
			rotate([0, 90, 0])
				cylinder(h = 20, d = 5.5);
	}
}

