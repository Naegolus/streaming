
$fa = 1.0;
$fs = 0.2;

use <../lib/lib.scad>
use <../lib/usb-hub-wall-mount.scad>
use <./cable-holder.scad>

mountShow = false;

if (mountShow)
{
	translate([0, -3, 53 + 25 + 4])
	rotate([0, 90, 0])
	usbHubWallMount();
}

cableHolder();

