
$fa = 1.0;
$fs = 0.2;

use <../lib/lib.scad>
use <./cable-holder-2.scad>

asztalShow = false;

if (asztalShow)
{
	translate([-25, 0, 0])
	asztal();

	translate([-25, -60, 0])
	asztal();

	rotate([0, -90, 0])
		cableHolder2();
} else {
	cableHolder2();
}

