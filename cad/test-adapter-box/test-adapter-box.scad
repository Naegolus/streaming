
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <./test-adapter-plate.scad>
use <./st-link-v2-clone.scad>
use <./usb-to-uart.scad>

function taBoxGroundHeight() = 6;
taBoxWallTh = 3;

function taBoxWidth() = taPlateWidth();
function taBoxDepth() = taPlateDepth();
function taBoxHeight() = 30;

function taBoxPlateHeight() = taBoxHeight();

function programmerOffsetX() = 20;
function uartOffsetX() = 10;

intBoxHeight = stLinkCloneWidthM() + 0.5 * (stLinkCloneWidth() - stLinkCloneWidthM()) - 2;

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
			taBoxWidth() - 2 * taBoxWallTh,
			taBoxDepth() - 2 * taBoxWallTh,
			taBoxHeight()
		], center = true);
	}

	cmirror([0, 1, 0])
	cmirror()
	translate
	([
		0.5 * taBoxWidth() - 10,
		0.5 * taBoxDepth() - 10,
		0
	])
	difference()
	{
		cube([10, 10, taBoxPlateHeight()]);

		translate([5, 5, taBoxPlateHeight() - 10])
		cylinder(h = 20, d = 2);
	}
}

module devicesCut()
{
//	linear_extrude
//	(
//		stLinkCloneWidthM() +
//		0.5 * (stLinkCloneWidth() - stLinkCloneWidthM())
//		+ 0.1
//	)
//	projection(cut = false)
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

}

module testAdapterBox()
{
	difference()
	//union()
	{
		color([0.4, 0.4, 0.4, 1.0])
		union()
		{
			testAdapterBoxBase();

			translate
			([
				0.5 * (uartOffsetX() + programmerOffsetX())
				- 16,
				0,
				taBoxGroundHeight()
			])
			cube
			([
				32,
				36,
				intBoxHeight
			]);
		}

		devicesCut();

		translate
		([
			5,
			25,
			intBoxHeight - 10
		])
		cylinder(h = 20, d = 2);

		translate
		([
			27,
			25,
			intBoxHeight - 10
		])
		cylinder(h = 20, d = 2);

		color([1, 0, 0, 1])
		translate
		([
			programmerOffsetX() - 0.5 * stLinkCloneDepthM(),
			0.5 * taBoxDepth() - 2 * taBoxWallTh,
			taBoxGroundHeight()
		])
		cube
		([
			stLinkCloneDepthM(),
			3 * taBoxWallTh,
			taBoxHeight() - taBoxGroundHeight() + 0.1
		]);

		color([1, 0, 0, 1])
		translate
		([
			uartOffsetX() - 0.5 * usbDepth(),
			0.5 * taBoxDepth() - 2 * taBoxWallTh,
			taBoxGroundHeight() + 0.1
		])
		cube
		([
			usbDepth(),
			3 * taBoxWallTh,
			taBoxHeight() - taBoxGroundHeight() + 0.1
		]);
	}
}

