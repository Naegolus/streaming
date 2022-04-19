
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <./pcb.scad>

function stencilWidth() = 100;
function stencilDepth() = stencilWidth();
function stencilHeight() = 0.1;

function stencilFrameWidth() = 1.4 * stencilWidth();
function stencilFrameDepth() = 1.2 * stencilDepth();
function stencilFrameHeight() = 8;

module stencil()
{
	translate
	([
		-0.5 * stencilWidth(),
		-0.5 * stencilDepth(),
		0
	])
	cube
	([
		stencilWidth(),
		stencilDepth(),
		stencilHeight()
	]);
}

module stencilFrame()
{
	difference()
	//union()
	{
		union()
		{
			translate([-0.5 * stencilFrameWidth(), -0.5 * stencilFrameDepth(), 0])
			cube
			([
				stencilFrameWidth(),
				stencilFrameDepth(),
				stencilFrameHeight()
			]);

			// shoulder
			cmirror([0, 1, 0])
			translate
			([
				-0.5 * stencilFrameWidth(),
				0.5 * pcbDepth(),
				stencilFrameHeight()
			])
			cube
			([
				stencilFrameWidth(),
				0.5 * (stencilFrameDepth() - pcbDepth()),
				pcbHeight()
			]);

			mirror([1, 0, 0])
			translate
			([
				0.5 * pcbWidth(),
				-0.5 * stencilFrameDepth(),
				stencilFrameHeight()
			])
			cube
			([
				0.5 * (stencilFrameWidth() - pcbWidth()),
				stencilFrameDepth(),
				pcbHeight()
			]);
			}

		translate
		([
			0.5 * stencilFrameWidth() - 30,
			-40,
			-stencilFrameHeight()
		])
		cube
		([
			50,
			80,
			3 * stencilFrameHeight()
		]);

		translate
		([
			0.5 * stencilFrameWidth() - 15,
			-0.6 * stencilFrameDepth(),
			-stencilFrameHeight()
		])
		cube
		([
			50,
			1.2 * stencilFrameDepth(),
			3 * stencilFrameHeight()
		]);

	}

}

