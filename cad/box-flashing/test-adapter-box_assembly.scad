
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <../lib/st-link-v2-clone.scad>
use <../lib/usb-to-uart.scad>

use <./test-adapter-plate.scad>
use <./test-adapter-box.scad>
use <./bridge.scad>
use <./rpizero.scad>
use <./led-plate.scad>

extShow = true;
platesShow = false;

//intersection()
//{
//translate([0, -5, 0])
//cube([34, 60, 40]);
testAdapterBox();
//}

translate([-15, 15, taBoxGroundHeight()])
color([0.4, 0.4, 0.4, 1.0])
rpiBase();

if (extShow)
{

	translate([1.5, -5, taBoxGroundHeight() + intBoxHeight() + 5])
	rotate([180, 0, 0])
	bridge();

	translate
	([
		programmerOffsetX(),
		0.5 * taBoxDepth() - stLinkCloneHeight() - 50,
		taBoxGroundHeight() + 0.5 * stLinkCloneWidth()
	])
	rotate([-90, 90, 0])
	stLinkClone();

	translate([-15, 15, taBoxGroundHeight()])
	translate([0, 0, rpiBaseHeight() + eps()])
	rpizero();

	if (platesShow)
	{
		color([0, 0.4, 0, 0.4])
		union()
		{
			translate([0, 15, taBoxPlateHeight() + 0.01])
			testAdapterPlate();

			translate([
				0,
				-0.5 * (taBoxDepth() - ledPlateDepth()),
				taBoxHeight() + eps()
			])
			ledPlate();
		}
	}
}

