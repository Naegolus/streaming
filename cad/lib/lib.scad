
function sel(v, indices) = [ for (i = indices) v[i] ];
function eps() = 0.01;

module cmirror(v = [1, 0, 0])
{
	children();
	mirror(v)
		children();
}

