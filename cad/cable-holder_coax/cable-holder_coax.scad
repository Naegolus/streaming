
$fa = 1.0;
$fs = 0.2;

use <../lib/lib.scad>

lenxPlateBase = 200;
lenyPlateBase = 50;
lenzPlateBase = 5;

lenxFork = lenxPlateBase + 2*15;
lenyFork = 6;
distFork = 7;
posyFork = 12;

depthFront = 4;
heightFront = 30;
phiFront = 5;

diaHole = 6.1;
diaHoleOutside = 13;

depthMountCable = 1;
distPairCable = 15;

distPairOutside = 32;

module pairCable(offset = 0)
{
	translate([offset, 0, 0])
	cmirror([1, 0, 0])
	translate([-0.5*distPairCable, 0, 0.5*heightFront])
	rotate([-90, 0, 0])
	translate([0, 0, depthMountCable])
	union()
	{
		translate([0, 0, -depthFront])
			cylinder(h = 2*depthFront, d = diaHole);
		cylinder(h = 2*depthFront, d = diaHoleOutside);
	}
}

module plateFront()
{
	//union()
	difference()
	{
		union()
		{
			translate([-0.5*lenxPlateBase, 0, 0])
				cube([lenxPlateBase,
					depthFront,
					heightFront]);

			rotate([0, -90, 0])
			translate([0, 0, -0.5*lenxPlateBase])
			linear_extrude(height = lenxPlateBase)
			polygon(
				points = [
					[0, 0],
					[10, 0],
					[0, 10]],
				paths = [
					[0, 1, 2]]);
		}

		offsetPairFirst = -0.5*lenxPlateBase + distPairOutside;
		spacePairs = lenxPlateBase - 2*distPairOutside;
		spacePair = spacePairs / (4 - 1);
		echo(spacePair=spacePair);

		for (i = [0 : 3])
			pairCable(i*spacePair + offsetPairFirst);
	}
}

module forks()
{
	translate([0, posyFork, 0])
	cmirror([0, 1, 0])
	translate([-0.5*lenxFork, -lenyFork - 0.5*distFork, -lenzPlateBase])
		cube([lenxFork,
			lenyFork,
			lenzPlateBase]);
}

module cableHolderCoax()
{
	// Base Plate
	translate([-0.5*lenxPlateBase, 0, -lenzPlateBase])
		cube([lenxPlateBase,
			lenyPlateBase,
			lenzPlateBase]);

	rotate([-phiFront, 0, 0])
		plateFront();

	forks();
}

