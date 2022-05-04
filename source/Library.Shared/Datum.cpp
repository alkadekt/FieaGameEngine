#include "pch.h"
#include "Datum.h"
#include "Scope.h"

using namespace std;

namespace FieaGameEngine
{
	const std::size_t Datum::_datumTypeSizes[]
	{
		0,
		sizeof(std::int32_t),
		sizeof(float),
		sizeof(std::string),
		sizeof(glm::vec4),
		sizeof(glm::mat4),
		sizeof(void*),
		sizeof(Scope*)
	};

	Hashmap<std::string, Datum::DatumType> Datum::_stringToDatumTypeMap
	{
		std::make_pair("integer", Datum::DatumType::Integer),
		std::make_pair("float", Datum::DatumType::Float),
		std::make_pair("string", Datum::DatumType::String),
		std::make_pair("vector", Datum::DatumType::Vector),
		std::make_pair("matrix", Datum::DatumType::Matrix),
		std::make_pair("table", Datum::DatumType::Table),
	};

	Datum::Datum(const Datum& other) :
		_type(other._type), _isExternal(other._isExternal)
	{
		if (other._type == DatumType::Unknown && other._capacity == 0)
		{
			return;
		}
		if (other._isExternal)
		{
			_dataPtr = other._dataPtr;
			_size = other._size;
			_capacity = other._capacity;
		}
		else
		{
			Reserve(other._capacity);

			if (other._type == DatumType::String)
			{
				for (std::size_t i = 0; i < other._size; ++i)
				{
					PushBackString(other.GetString(i));
				}
			}
			else
			{
				memcpy_s(_dataPtr.vp, _capacity * _datumTypeSizes[static_cast<int>(_type)], 
					other._dataPtr.vp, other._capacity * _datumTypeSizes[static_cast<int>(other._type)]);
				_size = other._size;
			}
		}
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (&other != this)
		{
			if (other._type == DatumType::Unknown && _type != DatumType::Unknown)
			{
				throw std::runtime_error("Cannot use this operator on an Unknown datum type.");
			}

			if (!_isExternal)
			{
				Resize(0);
				_dataPtr.vp = nullptr;
			}

			_type = other._type;

			if (other._isExternal)
			{
				_isExternal = true;
				_dataPtr = other._dataPtr;
				_size = other._size;
				_capacity = other._capacity;
			}
			else
			{
				Resize(other._capacity);

				if (other._type == DatumType::String)
				{
					for (std::size_t i = 0; i < other._size; ++i)
					{
						SetString(other.GetString(i), i);
					}
				}
				else
				{
					memcpy_s(_dataPtr.vp, _size * _datumTypeSizes[static_cast<int>(_type)], 
						other._dataPtr.vp, other._size * _datumTypeSizes[static_cast<int>(other._type)]);
				}
			}
		}
		return *this;
	}

	Datum& Datum::operator=(int value)
	{
		SetType(DatumType::Integer);
		Resize(0);
		PushBackInteger(value);
		return *this;
	}

	Datum& Datum::operator=(float value)
	{
		SetType(DatumType::Float);
		Resize(0);
		PushBackFloat(value);
		return *this;
	}

	Datum& Datum::operator=(const std::string& value)
	{
		SetType(DatumType::String);
		Resize(0);
		PushBackString(value);
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& value)
	{
		SetType(DatumType::Vector);
		Resize(0);
		PushBackVector(value);
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& value)
	{
		SetType(DatumType::Matrix);
		Resize(0);
		PushBackMatrix(value);
		return *this;
	}

	Datum& Datum::operator=(RTTI* const value)
	{
		SetType(DatumType::Pointer);
		Resize(0);
		PushBackPointer(value);
		return *this;
	}

	Datum& Datum::operator=(Scope& value)
	{
		SetType(DatumType::Table);
		Resize(1);
		SetTable(value);
		return *this;
	}

	Scope& Datum::operator[](std::size_t index)
	{
		return GetTable(index);
	}

	Datum::Datum(Datum&& other) noexcept :
		_dataPtr(other._dataPtr), _type(other._type), _size(other._size),
		_capacity(other._capacity), _isExternal(other._isExternal)
	{
		other._dataPtr.vp = nullptr;
		other._type = DatumType::Unknown;
		other._size = 0;
		other._capacity = 0;
		other._isExternal = false;
	}

	Datum& Datum::operator=(Datum&& other) noexcept
	{
		if (&other != this)
		{
			if (!_isExternal)
			{
				Clear();
				free(_dataPtr.vp);
			}

			_dataPtr = other._dataPtr;
			_type = other._type;
			_size = other._size;
			_capacity = other._capacity;
			_isExternal = other._isExternal;

			other._dataPtr.vp = nullptr;
			other._type = DatumType::Unknown;
			other._size = 0;
			other._capacity = 0;
			other._isExternal = false;
		}
		return *this;
	}

	Datum::~Datum()
	{
		if (!_isExternal)
		{
			if (_type == DatumType::String)
			{
				Clear();
			}
			free(_dataPtr.vp);
		}
	}

	bool Datum::operator==(const Datum& rhs) const
	{
		bool result = false;
		if (rhs._type == _type && rhs._size == _size && rhs._isExternal == _isExternal)
		{
			if (_isExternal)
			{
				if (_dataPtr.vp == rhs._dataPtr.vp)
				{
					result = true;
				}
			}
			else
			{
				if (_type == DatumType::String)
				{
					for (std::size_t i = 0; i < _size; ++i)
					{
						if (_dataPtr.s[i] != rhs._dataPtr.s[i])
						{
							return result;
						}
					}
					result = true;
				}
				else if (_type == DatumType::Pointer || _type == DatumType::Table)
				{
					for (std::size_t i = 0; i < _size; ++i)
					{
						RTTI* lhsPointer = _dataPtr.r[i];
						RTTI* rhsPointer = rhs._dataPtr.r[i];
						if (lhsPointer != rhsPointer && lhsPointer != nullptr && !(lhsPointer->Equals(rhsPointer)))
						{
							return result;
						}
					}
					result = true;
				}
				else
				{
					if (memcmp(_dataPtr.vp, rhs._dataPtr.vp, _size) == 0)
					{
						result = true;
					}
				}				
			}
		}
		return result;
	}

	bool Datum::operator!=(const Datum& rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator==(int rhs) const
	{
		bool result = false;
		if (_type == DatumType::Integer && _size == 1 && _isExternal == false &&
			FrontInteger() == rhs)
		{
			result = true;
		}
		return result;
	}

	bool Datum::operator!=(int rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator==(float rhs) const
	{
		bool result = false;
		if (_type == DatumType::Float && _size == 1 && _isExternal == false &&
			FrontFloat() == rhs)
		{
			result = true;
		}
		return result;
	}

	bool Datum::operator!=(float rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator==(const std::string& rhs) const
	{
		bool result = false;
		if (_type == DatumType::String && _size == 1 && _isExternal == false &&
			FrontString() == rhs)
		{
			result = true;
		}
		return result;
	}

	bool Datum::operator!=(const std::string& rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator==(const glm::vec4& rhs) const
	{
		bool result = false;
		if (_type == DatumType::Vector && _size == 1 && _isExternal == false &&
			FrontVector() == rhs)
		{
			result = true;
		}
		return result;
	}

	bool Datum::operator!=(const glm::vec4& rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator==(const glm::mat4& rhs) const
	{
		bool result = false;
		if (_type == DatumType::Matrix && _size == 1 && _isExternal == false &&
			FrontMatrix() == rhs)
		{
			result = true;
		}
		return result;
	}

	bool Datum::operator!=(const glm::mat4& rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator==(const RTTI* const rhs) const
	{
		bool result = false;
		if (_type == DatumType::Pointer && _size == 1 && _isExternal == false &&
			FrontPointer() == rhs)
		{
			result = true;
		}
		return result;
	}

	bool Datum::operator!=(const RTTI* const rhs) const
	{
		return !operator==(rhs);
	}

	Datum::DatumType Datum::Type() const
	{
		return _type;
	}

	void Datum::SetType(DatumType type)
	{
		if (_type != type)
		{
			if (_type != DatumType::Unknown)
			{
				throw std::runtime_error("Cannot reassign Datum type unless it is Unknown.");
			}
			_type = type;
		}
	}

	std::size_t Datum::Size() const
	{
		return _size;
	}

	std::size_t Datum::Capacity() const
	{
		return _capacity;
	}

	void Datum::Resize(std::size_t size)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot resize external data.");
		}
		if (_type == DatumType::Unknown && size != 0)
		{
			throw std::runtime_error("Cannot resize unknown data.");
		}
		if (size == 0)
		{
			Clear();
			free(_dataPtr.vp);
			_capacity = 0;
			_dataPtr.vp = nullptr;
			return;
		}
		if (_type == DatumType::String && size < _size)
		{
			for (std::size_t i = size; i < _size; ++i)
			{
				_dataPtr.s[i].~basic_string();
			}
		}

		void* data = realloc(_dataPtr.vp, size * _datumTypeSizes[static_cast<int>(_type)]);
		assert(data != nullptr);
		_dataPtr.vp = data;
		
		if (_type == DatumType::String && size > _size)
		{
			for (std::size_t i = _size; i < size; ++i)
			{
				new (_dataPtr.s + i) std::string("empty"s);
			}
		}
		_size = size;
		_capacity = size;
	}

	bool Datum::IsExternal() const
	{
		return _isExternal;
	}

	void Datum::Clear()
	{
		if (_type == DatumType::String)
		{
			for (std::size_t i = 0; i < _size; ++i)
			{
				_dataPtr.s[i].~basic_string();
			}
		}
		_size = 0;
	}

	void Datum::Reserve(std::size_t capacity)
	{
		if (_type == DatumType::Unknown)
		{
			throw std::runtime_error("Cannot reserve Unknown type data or external data.");
		}

		if (_isExternal)
		{
			throw std::runtime_error("Cannot reserve external data");
		}

		if (capacity > _capacity)
		{
			void* data = realloc(_dataPtr.vp, capacity * _datumTypeSizes[static_cast<int>(_type)]);
			assert(data != nullptr);
			_dataPtr.vp = data;
			_capacity = capacity;
		}
	}

	void Datum::SetStorageInteger(int* array, std::size_t size)
	{
		SetType(DatumType::Integer);
		if (array == nullptr)
		{
			throw std::runtime_error("Passed in array is nullptr.");
		}
		assert(_capacity == 0);
		_isExternal = true;
		_dataPtr.i = array;
		_size = _capacity = size;
	}

	void Datum::SetStorageFloat(float* array, std::size_t size)
	{
		SetType(DatumType::Float);
		if (array == nullptr)
		{
			throw std::runtime_error("Passed in array is nullptr.");
		}
		assert(_capacity == 0);
		_isExternal = true;
		_dataPtr.f = array;
		_size = _capacity = size;
	}

	void Datum::SetStorageString(std::string* array, std::size_t size)
	{
		SetType(DatumType::String);
		if (array == nullptr)
		{
			throw std::runtime_error("Passed in array is nullptr.");
		}
		assert(_capacity == 0);
		_isExternal = true;
		_dataPtr.s = array;
		_size = _capacity = size;
	}

	void Datum::SetStorageVector(glm::vec4* array, std::size_t size)
	{
		SetType(DatumType::Vector);
		if (array == nullptr)
		{
			throw std::runtime_error("Passed in array is nullptr.");
		}
		assert(_capacity == 0);
		_isExternal = true;
		_dataPtr.v = array;
		_size = _capacity = size;
	}

	void Datum::SetStorageMatrix(glm::mat4* array, std::size_t size)
	{
		SetType(DatumType::Matrix);
		if (array == nullptr)
		{
			throw std::runtime_error("Passed in array is nullptr.");
		}
		assert(_capacity == 0);
		_isExternal = true;
		_dataPtr.m = array;
		_size = _capacity = size;
	}

	void Datum::SetStoragePointer(RTTI** array, std::size_t size)
	{
		SetType(DatumType::Pointer);
		if (array == nullptr)
		{
			throw std::runtime_error("Passed in array is nullptr.");
		}
		assert(_capacity == 0);
		_isExternal = true;
		_dataPtr.r = array;
		_size = _capacity = size;
	}

	void Datum::SetInteger(int value, std::size_t index)
	{
		SetType(DatumType::Integer);
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		_dataPtr.i[index] = value;
	}

	void Datum::SetFloat(float value, std::size_t index)
	{
		SetType(DatumType::Float);
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		_dataPtr.f[index] = value;
	}

	void Datum::SetString(const std::string& value, std::size_t index)
	{
		SetType(DatumType::String);
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		_dataPtr.s[index] = value;
	}

	void Datum::SetVector(const glm::vec4& value, std::size_t index)
	{
		SetType(DatumType::Vector);
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		_dataPtr.v[index] = value;
	}

	void Datum::SetMatrix(const glm::mat4& value, std::size_t index)
	{
		SetType(DatumType::Matrix);
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		_dataPtr.m[index] = value;
	}

	void Datum::SetPointer(RTTI* value, std::size_t index)
	{
		SetType(DatumType::Pointer);
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		_dataPtr.r[index] = value;
	}

	void Datum::SetTable(Scope& value, std::size_t index)
	{
		SetType(DatumType::Table);
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		_dataPtr.t[index] = &value;
	}

	void Datum::SetFromString(const std::string& given, std::size_t index)
	{
		switch (_type)
		{
		case FieaGameEngine::Datum::DatumType::Unknown:
			throw std::runtime_error("Cannot set string to unknown Datum type.");
			break;
		case FieaGameEngine::Datum::DatumType::Integer:
			SetInteger(FromStringToInteger(given), index);
			break;
		case FieaGameEngine::Datum::DatumType::Float:
			SetFloat(FromStringToFloat(given), index);
			break;
		case FieaGameEngine::Datum::DatumType::String:
			SetString(given, index);
			break;
		case FieaGameEngine::Datum::DatumType::Vector:
			SetVector(FromStringToVector(given), index);
			break;
		case FieaGameEngine::Datum::DatumType::Matrix:
			SetMatrix(FromStringToMatrix(given), index);
			break;
		case FieaGameEngine::Datum::DatumType::Pointer:
			throw std::runtime_error("Cannot set string to pointer Datum type.");
			break;
		}
	}

	int Datum::FromStringToInteger(const std::string& given) const
	{
		int result;
		sscanf_s(given.c_str(), "%d", &result);
		return result;
	}

	float Datum::FromStringToFloat(const std::string& given) const
	{
		float result;
		sscanf_s(given.c_str(), "%f", &result);
		return result;
	}

	glm::vec4 Datum::FromStringToVector(const std::string& given) const
	{
		float x, y, z, w;
		sscanf_s(given.c_str(), "bvec4(%f, %f, %f, %f)", &x, &y, &z, &w);
		glm::vec4 result(x, y, z, w);
		return result;
	}

	glm::mat4 Datum::FromStringToMatrix(const std::string& given) const
	{
		float a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
		sscanf_s(given.c_str(),
			"mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p);
		glm::mat4 result(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);
		return result;
	}

	std::string Datum::ToString(std::size_t index) const
	{
		switch (_type)
		{
		case FieaGameEngine::Datum::DatumType::Unknown:
			throw std::runtime_error("Cannot make string from Unknown Datum type.");
			break;
		case FieaGameEngine::Datum::DatumType::Integer:
			return std::to_string(GetInteger(index));
			break;
		case FieaGameEngine::Datum::DatumType::Float:
			return std::to_string(GetFloat(index));
			break;
		case FieaGameEngine::Datum::DatumType::String:
			return GetString(index);
			break;
		case FieaGameEngine::Datum::DatumType::Vector:
			glm::vec4 d = GetVector(index);
			return "bvec4("s + std::to_string(d.x) + ", "s + std::to_string(d.y) +
				", "s + std::to_string(d.z) + ", "s + std::to_string(d.w) + ")"s;
			break;
		case FieaGameEngine::Datum::DatumType::Matrix:
			glm::mat4 m = GetMatrix(index);
			return "mat4x4(("s +
				std::to_string(m[0][0]) + ", "s + std::to_string(m[0][1]) + ", "s +
				std::to_string(m[0][2]) + ", "s + std::to_string(m[0][3]) + "), ("s +
				std::to_string(m[1][0]) + ", "s + std::to_string(m[1][1]) + ", "s +
				std::to_string(m[1][2]) + ", "s + std::to_string(m[1][3]) + "), ("s +
				std::to_string(m[2][0]) + ", "s + std::to_string(m[2][1]) + ", "s +
				std::to_string(m[2][2]) + ", "s + std::to_string(m[2][3]) + "), ("s +
				std::to_string(m[3][0]) + ", "s + std::to_string(m[3][1]) + ", "s +
				std::to_string(m[3][2]) + ", "s + std::to_string(m[3][3]) + "))"s;
			break;
		case FieaGameEngine::Datum::DatumType::Pointer:
			{ const RTTI* temp = GetPointer(index);
			if (temp == nullptr)
			{
				return "nullptr"s;
			}
			else
			{
				return temp->ToString();
			}
			}
			break;
		default:
			return "error"s;
			break;
		}
	}

	int Datum::GetInteger(std::size_t index) const
	{
		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return _dataPtr.i[index];
	}

	float Datum::GetFloat(std::size_t index) const
	{
		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return _dataPtr.f[index];
	}

	const std::string& Datum::GetString(std::size_t index) const
	{
		if (_type != DatumType::String)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return _dataPtr.s[index];
	}

	const glm::vec4& Datum::GetVector(std::size_t index) const
	{
		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return _dataPtr.v[index];
	}

	const glm::mat4& Datum::GetMatrix(std::size_t index) const
	{
		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return _dataPtr.m[index];
	}

	RTTI* const Datum::GetPointer(std::size_t index) const
	{
		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return _dataPtr.r[index];
	}

	Scope& Datum::GetTable(std::size_t index) const
	{
		if (_type != DatumType::Table)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return *_dataPtr.t[index];
	}

	void Datum::PushBackInteger(int value)
	{
		SetType(DatumType::Integer);
		Reserve(_size + 1);
		++_size;
		_dataPtr.i[_size - 1] = value;
	}

	void Datum::PushBackFloat(float value)
	{
		SetType(DatumType::Float);
		Reserve(_size + 1);
		++_size;
		_dataPtr.f[_size - 1] = value;
	}

	void Datum::PushBackString(const std::string& value)
	{
		SetType(DatumType::String);
		Reserve(_size + 1);
		++_size;
		new (_dataPtr.s + _size - 1) std::string(value);
	}

	void Datum::PushBackVector(const glm::vec4& value)
	{
		SetType(DatumType::Vector);
		Reserve(_size + 1);
		++_size;
		_dataPtr.v[_size - 1] = value;
	}

	void Datum::PushBackMatrix(const glm::mat4& value)
	{
		SetType(DatumType::Matrix);
		Reserve(_size + 1);
		++_size;
		_dataPtr.m[_size - 1] = value;
	}

	void Datum::PushBackPointer(RTTI* const value)
	{
		SetType(DatumType::Pointer);
		Reserve(_size + 1);
		++_size;
		_dataPtr.r[_size - 1] = value;
	}

	void Datum::PushBackTable(Scope& value)
	{
		SetType(DatumType::Table);
		Reserve(_size + 1);
		++_size;
		_dataPtr.t[_size - 1] = &value;
	}

	void Datum::PushBackFromString(std::string given)
	{
		switch (_type)
		{
		case FieaGameEngine::Datum::DatumType::Unknown:
			throw std::runtime_error("Cannot pushback string to unknown Datum type.");
			break;
		case FieaGameEngine::Datum::DatumType::Integer:
			PushBackInteger(FromStringToInteger(given));
			break;
		case FieaGameEngine::Datum::DatumType::Float:
			PushBackFloat(FromStringToFloat(given));
			break;
		case FieaGameEngine::Datum::DatumType::String:
			PushBackString(given);
			break;
		case FieaGameEngine::Datum::DatumType::Vector:
			PushBackVector(FromStringToVector(given));
			break;
		case FieaGameEngine::Datum::DatumType::Matrix:
			PushBackMatrix(FromStringToMatrix(given));
			break;
		case FieaGameEngine::Datum::DatumType::Pointer:
			throw std::runtime_error("Cannot pushback string to pointer Datum type.");
			break;
		}
	}

	void Datum::PopBack()
	{
		Resize(_size - 1);
	}

	int& Datum::FrontInteger()
	{
		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.i[0];
	}

	float& Datum::FrontFloat()
	{
		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.f[0];
	}

	std::string& Datum::FrontString()
	{
		if (_type != DatumType::String)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.s[0];
	}

	glm::vec4& Datum::FrontVector()
	{
		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.v[0];
	}

	glm::mat4& Datum::FrontMatrix()
	{
		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.m[0];
	}

	RTTI*& Datum::FrontPointer()
	{
		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.r[0];
	}

	int Datum::FrontInteger() const
	{
		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.i[0];
	}

	float Datum::FrontFloat() const
	{
		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.f[0];
	}

	const std::string& Datum::FrontString() const
	{
		if (_type != DatumType::String)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.s[0];
	}

	const glm::vec4& Datum::FrontVector() const
	{
		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.v[0];
	}

	const glm::mat4& Datum::FrontMatrix() const
	{
		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.m[0];
	}

	const RTTI* Datum::FrontPointer() const
	{
		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.r[0];
	}

	int& Datum::BackInteger()
	{
		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.i[_size - 1];
	}

	float& Datum::BackFloat()
	{
		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.f[_size - 1];
	}

	std::string& Datum::BackString()
	{
		if (_type != DatumType::String)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.s[_size - 1];
	}

	glm::vec4& Datum::BackVector()
	{
		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.v[_size - 1];
	}

	glm::mat4& Datum::BackMatrix()
	{
		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.m[_size - 1];
	}

	RTTI*& Datum::BackPointer()
	{
		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.r[_size - 1];
	}

	int Datum::BackInteger() const
	{
		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.i[_size - 1];
	}

	float Datum::BackFloat() const
	{
		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.f[_size - 1];
	}

	const std::string& Datum::BackString() const
	{
		if (_type != DatumType::String)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.s[_size - 1];
	}

	const glm::vec4& Datum::BackVector() const
	{
		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.v[_size - 1];
	}

	const glm::mat4& Datum::BackMatrix() const
	{
		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.m[_size - 1];
	}

	const RTTI* Datum::BackPointer() const
	{
		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access empty list.");
		}
		return _dataPtr.r[_size - 1];
	}

	bool Datum::RemoveInteger(int value)
	{
		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasRemoved = false;
		std::pair findResult = FindInteger(value);
		if (findResult.second)
		{
			RemoveAt(findResult.first);
			wasRemoved = true;
		}
		return wasRemoved;
	}

	bool Datum::RemoveFloat(float value)
	{
		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasRemoved = false;
		std::pair findResult = FindFloat(value);
		if (findResult.second)
		{
			RemoveAt(findResult.first);
			wasRemoved = true;
		}
		return wasRemoved;
	}

	bool Datum::RemoveString(const std::string& value)
	{
		if (_type != DatumType::String)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasRemoved = false;
		std::pair findResult = FindString(value);
		if (findResult.second)
		{
			RemoveAt(findResult.first);
			wasRemoved = true;
		}
		return wasRemoved;
	}

	bool Datum::RemoveVector(const glm::vec4& value)
	{
		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasRemoved = false;
		std::pair findResult = FindVector(value);
		if (findResult.second)
		{
			RemoveAt(findResult.first);
			wasRemoved = true;
		}
		return wasRemoved;
	}

	bool Datum::RemoveMatrix(const glm::mat4& value)
	{
		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasRemoved = false;
		std::pair findResult = FindMatrix(value);
		if (findResult.second)
		{
			RemoveAt(findResult.first);
			wasRemoved = true;
		}
		return wasRemoved;
	}

	bool Datum::RemovePointer(const RTTI* value)
	{
		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasRemoved = false;
		std::pair findResult = FindPointer(value);
		if (findResult.second)
		{
			RemoveAt(findResult.first);
			wasRemoved = true;
		}
		return wasRemoved;
	}

	void Datum::RemoveAt(std::size_t index)
	{
		if (_type == DatumType::Unknown || _isExternal)
		{
			throw std::runtime_error("Cannot remove Unknown type data or external data.");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		if (_type == DatumType::String)
		{
			_dataPtr.s[index].~basic_string();
		}
		if (index != _size - 1)
		{
			std::uint8_t* startPtr = _dataPtr.b + index * _datumTypeSizes[static_cast<int>(_type)];
			std::uint8_t* endPtr = _dataPtr.b + (index + 1) * _datumTypeSizes[static_cast<int>(_type)];
			std::size_t destSize = (_size - index) * _datumTypeSizes[static_cast<int>(_type)];
			std::size_t sourceSize = (_size - index - 1) * _datumTypeSizes[static_cast<int>(_type)];
			memmove_s(startPtr, destSize, endPtr, sourceSize);
		}
		--_size;
	}

	std::pair<std::size_t, bool> Datum::FindInteger(int value) const
	{
		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasFound = false;
		std::size_t index = 0;
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (_dataPtr.i[i] == value)
			{
				wasFound = true;
				index = i;
				break;
			}
		}
		return std::make_pair(index, wasFound);
	}

	std::pair<std::size_t, bool> Datum::FindFloat(float value) const
	{
		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasFound = false;
		std::size_t index = 0;
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (_dataPtr.f[i] == value)
			{
				wasFound = true;
				index = i;
				break;
			}
		}
		return std::make_pair(index, wasFound);
	}

	std::pair<std::size_t, bool> Datum::FindString(const std::string& value) const
	{
		if (_type != DatumType::String)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasFound = false;
		std::size_t index = 0;
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (_dataPtr.s[i] == value)
			{
				wasFound = true;
				index = i;
				break;
			}
		}
		return std::make_pair(index, wasFound);
	}

	std::pair<std::size_t, bool> Datum::FindVector(const glm::vec4& value) const
	{
		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasFound = false;
		std::size_t index = 0;
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (_dataPtr.v[i] == value)
			{
				wasFound = true;
				index = i;
				break;
			}
		}
		return std::make_pair(index, wasFound);
	}

	std::pair<std::size_t, bool> Datum::FindMatrix(const glm::mat4& value) const
	{
		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasFound = false;
		std::size_t index = 0;
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (_dataPtr.m[i] == value)
			{
				wasFound = true;
				index = i;
				break;
			}
		}
		return std::make_pair(index, wasFound);
	}

	std::pair<std::size_t, bool> Datum::FindPointer(const RTTI* value) const
	{
		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasFound = false;
		std::size_t index = 0;
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (_dataPtr.r[i] == value)
			{
				wasFound = true;
				index = i;
				break;
			}
		}
		return std::make_pair(index, wasFound);
	}

	std::pair<std::size_t, bool> Datum::FindTable(Scope* value) const
	{
		if (_type != DatumType::Table)
		{
			throw std::runtime_error("Incorrect method overload for datum type.");
		}
		bool wasFound = false;
		std::size_t index = 0;
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (_dataPtr.t[i] == value)
			{
				wasFound = true;
				index = i;
				break;
			}
		}
		return std::make_pair(index, wasFound);
	}

	void Datum::SetStorage(Datum::DatumType type, void* array, std::size_t size)
	{
		SetType(type);
		if (array == nullptr)
		{
			throw std::runtime_error("Passed in array is nullptr.");
		}
		//if (size == 0)
		//{
		//	throw std::runtime_error("Array must have size > 0.");
		//}
		//assert(_capacity == 0);
		_isExternal = true;
		_dataPtr.vp = array;
		_size = _capacity = size;
	}
}

