#include "../../Engine.h"
#include "ScreenViewport.h"

namespace engine
{
	void ScreenViewport::VResize(int width, int height)
	{
		glViewport(0, 0, width, height);
		projection = mat44Orto((float)width / 2, (float)-width / 2, (float)height / 2, (float)-height / 2, 0.01f, 1024.f);
	}
}