#include <glpch.h>
#include "GLibEntry.h"
#include "GLSandbox.h"

namespace GLib
{
	GLApplication* CreateApplication()
	{
		return new GLSandboxApplication();
	}
}