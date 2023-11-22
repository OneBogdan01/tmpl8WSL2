#include "DebugRenderer.h"
#include <GLES2/gl2.h>


void DebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	glColor4f(color.x(), color.y(), color.z(), 1.0f);
	glVertex3f(from.x(), from.y(), from.z());
	glVertex3f(to.x(), to.y(), to.z());
}
