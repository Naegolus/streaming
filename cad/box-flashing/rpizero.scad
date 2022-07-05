
use <../lib/lib.scad>

function rpiWidth() = 65;
function rpiDepth() = 30;
function rpiHeight() = 1.6;

function rpiBaseHeight() = 6;

module rpizeroCut
(
	dia = 3
)
{
	cmirror([0, 1, 0])
	cmirror()
	translate([0.5 * 23, 0.5 * 58, -6])
	cylinder(h = 20, d = dia);
}

module rpiBase()
{
	difference()
	//union()
	{
		cmirror([0, 1, 0])
		cmirror()
		translate([0.5 * 23, 0.5 * 58, 0])
		cylinder(h = rpiBaseHeight(), d = 6);

		rpizeroCut(2);
	}
}

module rpizero()
{
	difference()
	//union()
	{
		rotate([0, 0, 90])
		translate([0, 0, 0])
		rcube
		(
			[
				rpiWidth(),
				rpiDepth(),
				rpiHeight()
			],
			aX = 1,
			aY = 1,
			r = 6
		);

		rpizeroCut();
	}

}

