
$fa = 1.0;
$fs = 0.4;

use <../../lib/lib.scad>

dist = 24.8;
hei = 4;
wid = 12;

difference()
{
	union()
	{
		cylinder(h = hei, d = wid);

		translate([dist, 0, 0])
			cylinder(h = hei, d = wid);

		translate([0, -0.5*wid, 0])
			cube([dist, wid, hei]);
	}

	translate([0, 0, -1])
		cylinder(h = hei + 2, d = 5);

	translate([dist, 0, -1])
		cylinder(h = hei + 2, d = 4);
}

