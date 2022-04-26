
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>

function usbWidth() = 12;
function usbToUartWidth() = 15.4;
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
		usbToUartWidth(),
		1.6,
		30
	], center = true);

	color([0, 0, 0, 1])
	union()
	{
		translate
		([
			0,
			- 0.5 * (usbDepth()) + 0.5 * 3.0 - eps(),
			0.5 * 30 + 15
		])
		cube
		([
			usbToUartWidth(),
			3.0 - 1.6,
			30
		], center = true);

		translate
		([
			0,
			0,
			43
		])
		cube
		([
			usbToUartWidth() - eps(),
			8,
			2.5
		], center = true);

		translate
		([
			0,
			4 - 0.5 * 2.5,
			43 + 0.5 * 16
		])
		cube
		([
			usbToUartWidth() - eps(),
			2.5,
			16
		], center = true);
	}
}

