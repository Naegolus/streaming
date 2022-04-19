
$fa = 1.0;
$fs = 0.4;

use <../lib/lib.scad>
use <./pcb.scad>
use <./stencil-frame.scad>

extShow = true;

color([0.6, 0.6, 0.6, 1])
stencilFrame();

if (extShow)
{

	color([0, 0.6, 0, 1])
	translate([0, 0, stencilFrameHeight()])
	pcb();

	color([0.8, 0.8, 0.8, 0.6])
	translate([0, 0, stencilFrameHeight() + pcbHeight() + 0.1])
	stencil();

}

