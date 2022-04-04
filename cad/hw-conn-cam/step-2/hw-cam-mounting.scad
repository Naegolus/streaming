
$fa = 1.0;
$fs = 0.4;

// cylinder(h = 60.0, d = 23.5);
// cylinder(h = 5.0, d = 40);

// cylinder(h = 60.2, d = 17);
// cube([70, 26, 5]);
// linear_extrude(30)
// polygon(c);
// translate([40, 8, 5])
// rotate([180, 0, 0])
// difference()
// union()

difference()
{
	union()
	{
		translate([0, -5.5, 0])
			cube([8.5, 11, 90]);

		translate([0, 0, 90])
			rotate([0, 90, 0])
				cylinder(h = 8.5, d = 11);

	}

	translate([-10, 0, 90])
		rotate([0, 90, 0])
			cylinder(h = 20, d = 4.2);
}

difference()
{
	union()
	{
		translate([30, 0, 0])
			cylinder(h = 40, d = 32);

		difference()
		{
		// bridge
		translate([0, -5.5, 0])
			cube([20, 11, 35]);

		translate([11.25, 10, 35])
			rotate([90, 0, 0])
				cylinder(h = 20.0, d = 5.5);
		}
	}

	translate([30, 0, -10])
		cylinder(h = 60.0, d = 23.6);
}

