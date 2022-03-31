
$fa = 1.0;
$fs = 0.4;

//rotate([180, 0, 0])
{
	difference()
	{
		union()
		{
			cylinder(h = 60.0, d = 23.5);
			cylinder(h = 5.0, d = 40);

			// top
			translate([0, -13, 0])
				cube([70, 26, 5]);
		}

		translate([0, 0, -0.1])
			cylinder(h = 60.2, d = 17);
	}

	// plate
	translate([40, 6, 0.0])
	{
		difference()
		{
			cube([30, 7, 40]);
			translate([15, 10, 28])
				rotate([90, 0, 0])
					cylinder(h = 15.0, d = 7);
		}
	}

	c = [
		[ 0, 0],
		[10, 0],
		[0, 10],
	];


	translate([40, 8, 5])
	rotate([180, 0, 0])
	rotate([0, 90, 0])
	linear_extrude(30)
		polygon(c);
}

