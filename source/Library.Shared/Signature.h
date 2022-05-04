#pragma once
#include "Datum.h"

namespace FieaGameEngine
{
	struct Signature final
	{
		std::string _name;
		Datum::DatumType _type;
		std::size_t _count;
		std::size_t _offset;
	};
}