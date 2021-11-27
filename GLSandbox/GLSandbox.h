#pragma once
#include "GLApplication.h"

class GLSandboxApplication : public GLApplication
{
public:
	GL_APPLICATION(GLSandboxApplication, "GLSanbox")

	virtual void OnStart() override;
};

