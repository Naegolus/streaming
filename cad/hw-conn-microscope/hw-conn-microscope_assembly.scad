
$fa = 1.0;
$fs = 0.4;

use <../lib/hw-conn-cam.scad>
use <hw-conn-microscope.scad>
use <microscope.scad>
use <wall.scad>

hwConnCamShow = true;
hwConnMicroscopeShow = true;
wallShow = true;
microscopeShow = true;

if (hwConnMicroscopeShow)
{
	rotate([0, 180, 0])
	hwConnMicoscope();
}

if (wallShow)
	wall();

if (hwConnCamShow)
{
	color([0.2, 0.5, 0.0, 0.4])
	hwConnCam();
}

if (microscopeShow)
{
	color([0.2, 0.5, 0.0, 0.4])
	translate([refPtX(), refPtY(), refPtZ()])
		microscope();
}

