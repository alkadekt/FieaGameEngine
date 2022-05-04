#pragma once

namespace FieaGameEngine
{
	struct IncrementFunctor final
	{
		std::size_t operator()(std::size_t size, std::size_t capacity) const
		{
			capacity;
			return std::max(std::size_t(1), size * 2);
		}
	};
}