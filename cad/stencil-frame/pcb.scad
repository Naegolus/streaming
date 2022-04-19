
function pcbWidth() = 44;
function pcbDepth() = 30;
function pcbHeight() = 1.6;

module pcb()
{
	translate([-0.5 * pcbWidth(), -0.5 * pcbDepth(), 0])
	cube
	([
		pcbWidth(), 
		pcbDepth(),
		pcbHeight()
	]);
}

