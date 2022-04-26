
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>

function stLinkCloneWidth() = 20;
function stLinkCloneDepth() = 9;
function stLinkCloneHeight() = 42;

function stLinkCloneWidthM() = 17;
function stLinkCloneDepthM() = 7;

function stLinkConnWidth() = 17.5;
function stLinkConnDepth() = 8;

module stLinkClone()
{
	translate
	([
		-0.5 * stLinkCloneWidth(),
		-0.5 * stLinkCloneDepth(),
		0
	])
	union()
	{
		color([0, 0, 0, 1])
		cube([stLinkCloneWidth(), stLinkCloneDepth(), 9]);

		translate
		([
			0.5 * (stLinkCloneWidth() - stLinkConnWidth()),
			(stLinkCloneDepth() - stLinkConnDepth()),
			-10 + eps()
		])
		color([0, 0, 0, 1])
		cube([stLinkConnWidth(), stLinkConnDepth(), 10]);

		translate
		([
			0.5 * (stLinkCloneWidth() - stLinkCloneWidthM()),
			0.5 * (stLinkCloneDepth() - stLinkCloneDepthM()),
			0.01
		])
		rcube
		([
			stLinkCloneWidthM(),
			stLinkCloneDepthM(),
			stLinkCloneHeight()
		]);

		color([0.7, 0.7, 0.7, 1])
		translate
		([
			0.5 * (stLinkCloneWidth() - 12),
			0.5 * (stLinkCloneDepth() - 4.5),
			0.01
		])
		cube([12, 4.5, 54]);
	}
}

