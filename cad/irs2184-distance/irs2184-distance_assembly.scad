
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <./irs2184-distance.scad>

for(u = [0 : 10 : 20])
{
	for(v = [0 : 10 : 20])
	{
		translate([u, v, 0])
			irs2184_distance();
	}
}

