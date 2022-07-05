
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>

module bridge()
{
	difference()
	//union()
	{
		union()
		{
			cube([29, 8, 3]);
			cube([14, 8, 3.8]);
		}

		translate([25.5, 4, -10])
		cylinder(h = 20, d = 3);

		translate([10, 4, -10])
		cylinder(h = 20, d = 3);

		translate([-2, -5, -5])
		cube([18, 40, 40]);
	}
}

singleShow = false;

if (singleShow)
	bridge();

