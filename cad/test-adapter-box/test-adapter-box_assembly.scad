
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <./test-adapter-plate.scad>
use <./test-adapter-box.scad>
use <./st-link-v2-clone.scad>
use <./usb-to-uart.scad>

extShow = false;

intersection()
{
translate([0, -5, 0])
cube([34, 60, 40]);
testAdapterBox();
}

if (extShow)
{

	translate
	([
		programmerOffsetX(),
		0.5 * taBoxDepth() - stLinkCloneHeight(),
		taBoxGroundHeight() + 0.5 * stLinkCloneWidth()
	])
	rotate([-90, 90, 0])
	stLinkClone();

	translate
	([
		uartOffsetX(),
		0.5 * taBoxDepth() + usbHeight(),
		taBoxGroundHeight() + 0.5 * stLinkCloneWidth()
	])
	rotate([90, -90, 0])
	usbToUart();

	if (false)
	{
	color([0, 0.4, 0, 0.4])
	translate([0, 0, taBoxPlateHeight() + 0.01])
	testAdapterPlate();
	}

}

