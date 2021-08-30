#include "GLib.h"
#include "GLSandbox.h"

namespace GLib
{
	GLApplication* CreateApplication()
	{
		return new GLSandboxApplication();
	}
}