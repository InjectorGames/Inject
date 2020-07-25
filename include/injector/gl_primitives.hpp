#pragma once
#include <injector/defines.hpp>

#include <vector>
#include <cstdint>

namespace INJECTOR_NAMESPACE
{
	struct GlPrimitives
	{
		static const std::vector<float> squareVertices;
		static const std::vector<float> squareNormals;
		static const std::vector<uint8_t> squareIndices;

		static const std::vector<float> cubeVertices;
		static const std::vector<float> cubeNormals;
		static const std::vector<uint8_t> cubeIndices;

		static const std::vector<float> axisVertices;
		static const std::vector<float> axisColors;
		static const std::vector<uint8_t> axisIndices;

		// TODO: replace with sphere skybox
		static const std::vector<float> gradientSkyVertices;
		static const std::vector<uint8_t> gradientSkyIndices;
	};
}
