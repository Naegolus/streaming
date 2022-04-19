
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <usb-hub-base.scad>

wallThickness = 2;

flapWidth = 25;
flapHeight = 3;

mountWidth = hubWidth() + 2 * wallThickness;
mountHeight = hubHeight() + 1 * wallThickness;
mountLen = 20;

topLen = 3;
sideHeight = 20;

holeDiameter = 4.5;

// cylinder(h = 60.0, d = 23.5);
// cylinder(h = 5.0, d = 40);

// cylinder(h = 60.2, d = 17);
// cube([70, 26, 5]);
// linear_extrude(30)
// polygon(c);
// translate([40, 8, 5])
// rotate([180, 0, 0])
// difference()
// union()

module usbHubWallMount()
{
difference()
{
	union()
	{
		cube
		([
			mountWidth,
			mountHeight + flapHeight,
			mountLen
		]);

		translate([-flapWidth, 0, 0])
		cube
		([
			mountWidth + 2 * flapWidth,
			flapHeight,
			mountLen
		]);

		translate([mountWidth / 2, 0, 0])
		cmirror()
		translate([-mountWidth / 2, flapHeight, 0])
		linear_extrude(mountLen)
			polygon([[0, 0], [0, 5], [-5, 0]]);
	}

	translate([wallThickness, flapHeight, wallThickness])
	color([0.2, 0.5, 0.0, 0.6])
	cube
	([
		hubWidth(),
		hubHeight(),
		2 * mountLen
	]);

	translate([-flapWidth, sideHeight, wallThickness + topLen])
	color([0.2, 0.5, 0.0, 0.6])
	cube
	([
		mountWidth + 2 * flapWidth,
		mountHeight + flapHeight,
		mountLen
	]);

	translate([mountWidth / 2, 0, 0])
	cmirror()
	translate
	([
		-(mountWidth + flapWidth) / 2 - 2.5,
		2 * flapHeight,
		mountLen / 2
	])
	rotate([90, 0, 0])
	color([0.2, 0.5, 0.0, 0.6])
	cylinder
	(
		h = 3 * flapHeight,
		d = holeDiameter
	);

	translate
	([
		wallThickness + 20.6,
		flapHeight + 12,
		-wallThickness
	])
	color([0.2, 0.5, 0.0, 0.6])
	cylinder(h = 3 * wallThickness, d = 11);

	translate
	([
		wallThickness + hubWidth() - 13 - 4,
		flapHeight + 6.8,
		-wallThickness
	])
	color([0.2, 0.5, 0.0, 0.6])
	cube
	([
		13,
		16,
		3 * wallThickness
	]);

}
}

