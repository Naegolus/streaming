
$fa = 1.0;
$fs = 0.4;

use <../../hw-conn-microscope/microscope.scad>
use <hw-conn-base.scad>
use <hw-conn-cam.scad>
// use <hw-conn-microscope.scad>

connBaseShow = true;
connCamShow = true;
microscopeShow = true;

if (connBaseShow)
	connBase();

if (connCamShow)
{
	translate([32, 0, bridgeHeight()])
	rotate([180, 0, 0])
	rotate([0, 0, -40])
		connCam();
}

if (microscopeShow)
{
	color([0.2, 0.5, 0.0, 0.4])
	translate([0, 60, shoulderHeight()])
		microscope();
}

