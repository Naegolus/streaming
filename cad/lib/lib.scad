
function sel(v, indices) = [ for (i = indices) v[i] ];
function eps() = 0.01;

module cmirror(v = [1, 0, 0])
{
	children();
	mirror(v)
		children();
}

module rcube2d
(
	s = [10, 10, 10],
	r = 2,
	singleSided = false,
)
{
	width = s[0];
	depth = s[1];
	height = s[2];

	union()
	{
		cmirror([0, 1, 0])
		cmirror()
		translate([width / 2 - r, depth / 2 - r, 0])
		cylinder(h = height, r = r);
		translate([r - width / 2, -depth / 2, 0])
		cube([width - r*2, depth, height]);
		translate([-width / 2, r - depth / 2, 0])
		cube([width, depth - r*2, height]);
	}

	if (singleSided)
	{
		translate([-width / 2, -depth / 2, 0])
		cube([width, r*2, height]);
	}
}

module rcube3d
(
	s = [10, 10, 10],
	r = 2,
	singleSided = false,
)
{
	width = s[0];
	depth = s[1];
	height = s[2];

	w2 = width - r*2;
	d2 = depth - r*2;
	h2 = height - r*2;

	translate([0, 0, r])
	rcube2d([width, depth, h2], r);

	translate([0, 0, height / 2])
	cmirror([0, 0, 1])
	cmirror([0, 1, 0])
	translate([0, d2 / 2, height / 2 - r])
	translate([-w2 / 2, 0, 0])
	rotate([0, 90, 0])
	cylinder(h = w2, r = r);

	translate([0, 0, height / 2])
	cmirror([0, 0, 1])
	cmirror()
	translate([w2 / 2, 0, h2 / 2])
	translate([0, d2 / 2, 0])
	rotate([90, 0, 0])
	cylinder(h = d2, r = r);

	translate([-w2 / 2, -d2 / 2, 0])
	cube([w2, d2, height]);

	translate([0, 0, height / 2])
	cmirror([0, 0, 1])
	cmirror([0, 1, 0])
	cmirror()
	translate([w2 / 2, d2 / 2, h2 / 2])
	sphere(r);

	if (singleSided)
	{
		translate([0, -depth / 2, height / 2])
		rotate([-90, 0, 0])
		rcube2d([width, height, r*2], r);
	}
}

module rcube
(
	s = [10, 10, 10],
	r = 2,
	flat = true,
	singleSided = false,
	aX = 0,
	aY = 0,
	aZ = 0,
)
{
	width = s[0];
	depth = s[1];
	height = s[2];

	rw = r > width ? width : r;

	translate
	([
		(1 - aX) * width / 2,
		(1 - aY) * depth / 2,
		-aZ * height / 2
	])
	if (flat)
		rcube2d(s, rw, singleSided);
	else
	{
		rh = rw > height ? height : rw;
		rcube3d(s, rh, singleSided);
	}
}

module chamfer
(
	s = [10, 20, 6],
	round = true,
	aX = 0,
	aY = 0,
	aZ = 0,
	rotation = 0,
)
{
	width = s[0];
	depth = s[1];
	height = s[2];

	r = width < height ? width : height;

	rotate([0, rotation * 90, 0])
	translate
	([
		-aX * r / 2,
		-aY * depth / 2,
		-aZ * r / 2
	])
	if (round)
	{
		difference()
		{
			cube(s);

			translate([r, -depth / 2, 0])
			union()
			{
				translate([0, 0, r])
				rotate([-90, 0, 0])
				cylinder(d = 2 * r, h = 2 * depth);

				translate([0, 0, -r / 2])
				cube([2 * width, 2 * depth, 2 * r]);

				translate([-1.5 * r, 0, r])
				cube([2 * r, 2 * depth, 2 * height]);
			}
		}
	}
	else
	{
		difference()
		{
			cube(s);

			translate([0, 1.5 * depth, 0])
			rotate([90, 0, 0])
			linear_extrude(2 * depth)
			polygon
			([
				[ 2 * width, -1 * height],
				[ 2 * width,  2 * height],
				[-1 * width,  2 * height],
			]);
		}
	}
}

