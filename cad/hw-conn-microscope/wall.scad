
module wall()
{
	difference()
	{
		translate([-30, -20, -10])
		cube([5, 40, 25 + 10]);

		translate([-5 - 30, -26 / 2, -30])
		cube([15, 26, 30]);

		translate([-5 - 30, -5 / 2, 15])
		cube([15, 5, 30]);
	}
}

