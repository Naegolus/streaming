
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <../lib/st-link-v2-clone.scad>
use <../lib/usb-to-uart.scad>

use <./test-adapter-plate.scad>

function taBoxGroundHeight() = 6;
taBoxWallTh = 3;

function taBoxWidth() = taPlateWidth();
function taBoxDepth() = taPlateDepth() + 30;
function taBoxHeight() = 40;

function taBoxPlateHeight() = taBoxHeight();

function programmerOffsetX() = 20;
function uartOffsetX() = 10;

function intBoxHeight() = stLinkCloneWidthM() + 0.5 * (stLinkCloneWidth() - stLinkCloneWidthM()) - 2;

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

	cmirror()
	translate
	([
		0.5 * taBoxWidth() - 9,
		0.5 * taBoxDepth() - taPlateDepth(),
		0
	])
	difference()
	{
		cube([9, 10, taBoxPlateHeight()]);

		translate([4, 5, taBoxPlateHeight() - 10])
		cylinder(h = 20, d = 2);
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
		0.5 * taBoxDepth() - stLinkCloneHeight() - 50,
		taBoxGroundHeight() + 0.5 * stLinkCloneWidth()
	])
	rotate([-90, 90, 0])
	stLinkClone();

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
				- 4,
				- 35,
				taBoxGroundHeight()
			])
			cube
			([
				20,
				36,
				intBoxHeight()
			]);
		}

		devicesCut();

		translate([19.25, -9, 0])
		//cmirror()
		translate
		([
			7.75,
			0,
			intBoxHeight() - 10
		])
		cylinder(h = 20, d = 2);

		translate([-23, -0.5 * taBoxDepth() + 20, -10])
		cylinder(h = 60, d = 3);

		translate([23, 0.5 * taBoxDepth() - 20, -10])
		cylinder(h = 60, d = 3);

		translate([10, 110, 12])
		rotate([90, 0, 0])
		union()
		{
			cmirror()
			translate([9, 0, 0])
			cylinder(h = 60, d = 3);

			rcube
			(
				[
					10,
					6,
					60
				],
				aX = 1,
				aY = 1,
				r = 1
			);
		}
	}
}

