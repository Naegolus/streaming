
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>

function usbWidth() = 12;
function usbDepth() = 4.5;
function usbHeight() = 11.5;

usbConnHeight = 19;

module usbToUart()
{
	color([0.7, 0.7, 0.7, 1])
	translate([0, 0, 0.5 * usbConnHeight])
	cube
	([
		usbWidth(),
		usbDepth(),
		usbConnHeight
	], center = true);

	color([0, 0, 1.0, 1])
	translate
	([
		0,
		- 0.5 * usbDepth(),
		0.5 * 30 + 15
	])
	cube
	([
		15,
		1.6,
		30
	], center = true);
}

