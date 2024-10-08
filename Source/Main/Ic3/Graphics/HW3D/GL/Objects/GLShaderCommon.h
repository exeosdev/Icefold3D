
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_COMMON_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_COMMON_H__

#include "../GLPrerequisites.h"
#include <map>

namespace Ic3::Graphics::GCI
{

	struct GLShaderDataLayoutMap
	{
		using ElementBindingMap = std::map<std::string, GLuint>;
		ElementBindingMap attributeLocations;
		ElementBindingMap fragDataLocations;
		ElementBindingMap samplerBindings;
		ElementBindingMap uniformBlockBindings;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return attributeLocations.empty() && fragDataLocations.empty() && samplerBindings.empty() && uniformBlockBindings.empty();
		}
	};

	namespace rcutil
	{

		bool processGLShaderSourceExplicitLayoutQualifiers(
				const Version & pSupportedAPIVersion,
				std::string & pShaderSource,
				GLShaderDataLayoutMap & pOutputLayoutMap );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_COMMON_H__
