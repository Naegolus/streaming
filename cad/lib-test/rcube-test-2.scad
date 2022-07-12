
use <../lib/lib.scad>

$fa = 0.03;
$fs = 0.03;

//mount = [1, 2, 3];
//mount = [3, 1, 2];
//mount = [2, 3, 1];
//mount = [2, 1, 3];
//mount = [3, 2, 1];
mount = [1, 3, 2];
r=3;

color("gold")
translate([mount.x, mount.y, 0])
rcube(mount, flat=true, singleSided=true, r=r, aX=1, aY=1, aZ=0);

color("blue")
translate([-mount.x, mount.y, 0])
rcube(mount, flat=false, singleSided=true, r=r, aX=1, aY=1, aZ=0);

color("red")
translate([-mount.x, -mount.y, 0])
rcube(mount, flat=true, singleSided=false, r=r, aX=1, aY=1, aZ=0);

color("green")
translate([mount.x, -mount.y, 0])
rcube(mount, flat=false, singleSided=false, r=r, aX=1, aY=1, aZ=0);

