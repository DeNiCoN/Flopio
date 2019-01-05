#include "../../Engine.h"
#include "ScreenViewport.h"

namespace engine
{
	void ScreenViewport::VResize(int width, int height)
	{
		glViewport(0, 0, width, height);
		projection = mat44Orto(width / 2, -width / 2, height / 2, -height / 2, 0.01, 1024);
	}
}