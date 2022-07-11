
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <../lib/st-link-v2-clone.scad>
use <../lib/usb-to-uart.scad>

use <./rpizero.scad>
use <./test-adapter-plate.scad>

function taBoxGroundHeight() = 6;
taBoxWallTh = 3;

function taBoxWidth() = taPlateWidth() + 30;
function taBoxDepth() = taPlateDepth() + 30;
function taBoxHeight() = 50;

function taBoxPlateHeight() = taBoxHeight();

function intBoxHeight() = stLinkCloneWidthM() + 0.5 * (stLinkCloneWidth() - stLinkCloneWidthM()) - 2;

module plateSupport
(
	x = 10,
	y = 10,
)
{
	cmirror([0, 1, 0])
	cmirror()
	translate
	([
		0.5 * x - 10,
		0.5 * y - 10,
		0
	])
	difference()
	{
		cube([10, 10, taBoxPlateHeight()]);

		translate([5, 5, taBoxPlateHeight() - 10])
		cylinder(h = 20, d = 2);
	}
}

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

	translate
	([
		- 0.5 * (taBoxWidth() - taPlateWidth()),
		0.5 * (taBoxDepth() - taPlateDepth()),
		0,
	])
	plateSupport(taPlateWidth(), taPlateDepth());

	plateSupport(taBoxWidth(), taBoxDepth());

	translate
	([
		- 0.5 * taBoxWidth() + taPlateWidth() - 10,
		- 0.5 * taBoxDepth(),
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
	translate
	([
		0,
		0,
		taBoxGroundHeight() + 0.5 * stLinkCloneWidth()
	])
	rotate([-90, 90, 180])
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
				- 0.5 * taBoxWidth() + taPlateWidth(),
				0.5 * taBoxDepth() - 36 - 10 - 5,
				taBoxGroundHeight()
			])
			cube
			([
				20,
				36,
				intBoxHeight()
			]);

			translate
			([
				- 0.5 * (taBoxWidth() - rpiDepth()) + 5,
				0.5 * (taBoxDepth() - rpiWidth()) - 11.5,
				taBoxGroundHeight()
			])
			rpiBase();

			translate
			([
				- 0.5 * taBoxWidth() + taPlateWidth() - taBoxWallTh,
				- 0.5 * taBoxDepth(),
				0,
			])
			cube
			([
				taBoxWallTh,
				taBoxDepth() - taPlateDepth(),
				taBoxHeight(),
			]);
		}

		translate
		([
			30,
			42,
			0,
		])
		devicesCut();

		//translate([19.25, -9, 0])
		//cmirror()
		//translate
		//([
			//7.75,
			//0,
			//intBoxHeight() - 10
		//])
		//cylinder(h = 20, d = 2);

		translate([- 0.5 * taBoxWidth() + 10, -0.5 * taBoxDepth() + 20, -10])
		cylinder(h = 60, d = 3);

		translate([0.5 * taBoxWidth() - 10, 0, -10])
		cylinder(h = 60, d = 3);

		translate
		([
			- 0.5 * taBoxWidth() + taPlateWidth() - 22,
			100,
			taBoxGroundHeight() + 4.5
		])
		rotate([90, 0, 0])
		union()
		{
			cmirror()
			translate([8.5, 0, 0])
			cylinder(h = 60, d = 3.2);

			rcube
			(
				[
					12,
					8,
					60
				],
				aX = 1,
				aY = 1,
				r = 1
			);
		}
	}
}

