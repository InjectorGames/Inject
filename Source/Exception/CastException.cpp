#include "Injector/Exception/CastException.hpp"

namespace Injector
{
	CastException::CastException(
		const std::string& functionName,
		const std::string& variableName) noexcept :
		Exception(functionName,
			"Failed to cast \"" + variableName + "\"")
	{
	}
}
