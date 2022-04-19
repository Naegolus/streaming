
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <./test-adapter-plate.scad>
use <./st-link-v2-clone.scad>
use <./usb-to-uart.scad>

function taBoxGroundHeight() = 6;
taBoxWallTh = 3;

function taBoxWidth() = taPlateWidth() + 2 * taBoxWallTh;
function taBoxDepth() = taPlateDepth() + 2 * taBoxWallTh;
function taBoxHeight() = 30;

function taBoxPlateHeight() = taBoxHeight() - taPlateHeight();

function programmerOffsetX() = 12;
function uartOffsetX() = -12;

module testAdapterBoxBase()
{
	difference()
	{
		translate
		([
			0,
			0,
			0.5 * taBoxHeight()
		])
		cube
		([
			taBoxWidth(),
			taBoxDepth(),
			taBoxHeight()
		], center = true);

		translate
		([
			0,
			0,
			0.5 * taBoxHeight() + taBoxGroundHeight()
		])
		cube
		([
			taPlateWidth(),
			taPlateDepth(),
			taBoxHeight()
		], center = true);
	}

	cmirror([0, 1, 0])
	cmirror()
	translate
	([
		0.5 * taBoxWidth() - 10 - taBoxWallTh,
		0.5 * taBoxDepth() - 10 - taBoxWallTh,
		0
	])
	difference()
	{
		cube([10, 10, taBoxPlateHeight()]);

		translate([5, 5, taBoxPlateHeight() - 10])
		cylinder(h = 20, d = 2);
	}
}

module testAdapterBox()
{
	difference()
	//union()
	{
		union()
		{
			color([0.4, 0.4, 0.4, 1.0])
			testAdapterBoxBase();
		}

		color([1, 0, 0, 1])
		translate
		([
			programmerOffsetX() - 0.5 * stLinkCloneWidthM(),
			0.5 * taBoxDepth() - 2 * taBoxWallTh,
			0.5 * (stLinkCloneDepth() - stLinkCloneDepthM())
			+ taBoxGroundHeight()
		])
		cube
		([
			stLinkCloneWidthM(),
			3 * taBoxWallTh,
			stLinkCloneDepthM()
		]);

		color([1, 0, 0, 1])
		translate
		([
			uartOffsetX() - 0.5 * usbWidth(),
			0.5 * taBoxDepth() - 2 * taBoxWallTh,
			0.5 * (stLinkCloneDepth() - usbDepth())
			+ taBoxGroundHeight()
		])
		cube
		([
			usbWidth(),
			3 * taBoxWallTh,
			usbDepth()
		]);
	}
}

