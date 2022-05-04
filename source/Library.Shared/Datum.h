#pragma once
#include <cassert>
#include <stdexcept>
#include <glm\glm.hpp>
#include "RTTI.h"
#include "Hashmap.h"

namespace FieaGameEngine
{
	class Scope;

	/// <summary>
	/// Datum stores a runtime determined type, and has adjustable size.
	/// Datum can also hold an external array as a shell container.
	/// </summary>
	class Datum final
	{
		friend class Scope;

	public:
		/// <summary>
		/// Enum of available Datum types.
		/// </summary>
		enum class DatumType
		{
			Unknown,
			Integer,
			Float,
			String,
			Vector,
			Matrix,
			Pointer,
			Table,
			End
		};
		
		static Hashmap<std::string, Datum::DatumType> _stringToDatumTypeMap;

		/// <summary>
		/// Default constructor, allocates no memory.
		/// </summary>
		Datum() = default;

		/// <summary>
		/// Copy constructor, creates Datum from other, changing type and storage to match.
		/// </summary>
		Datum(const Datum& other);

		/// <summary>
		/// Copy assignment operator, creates Datum from other, changing type and storage to match.
		/// </summary>
		Datum& operator=(const Datum& other);

		/// <summary>
		/// Scalar copy assignment operator overload, sets this Datum to hold just the given int.
		/// </summary>
		Datum& operator=(int value);

		/// <summary>
		/// Scalar copy assignment operator overload, sets this Datum to hold just the given float.
		/// </summary>
		Datum& operator=(float value);

		/// <summary>
		/// Scalar copy assignment operator overload, sets this Datum to hold just the given string.
		/// </summary>
		Datum& operator=(const std::string& value);

		/// <summary>
		/// Scalar copy assignment operator overload, sets this Datum to hold just the given vector.
		/// </summary>
		Datum& operator=(const glm::vec4& value);

		/// <summary>
		/// Scalar copy assignment operator overload, sets this Datum to hold just the given matrix.
		/// </summary>
		Datum& operator=(const glm::mat4& value);

		/// <summary>
		/// Scalar copy assignment operator overload, sets this Datum to hold just the given pointer.
		/// </summary>
		Datum& operator=(RTTI* const value);

		/// <summary>
		/// Scalar copy assignment operator overload, sets this Datum to hold just the given scope.
		/// </summary>
		Datum& operator=(Scope& value);

		/// <summary>
		/// Returns table at index, similar to GetTable.
		/// </summary>
		Scope& operator[](std::size_t index);

		/// <summary>
		/// Move constructor, shallow copies given into new Datum then zeroes out other.
		/// </summary>
		Datum(Datum&& other) noexcept;

		/// <summary>
		/// Move assignment operator, shallow copies given into new Datum then zeroes out other.
		/// </summary>
		Datum& operator=(Datum&& other) noexcept;

		/// <summary>
		/// Destructor, ensures no memory leaks.
		/// </summary>
		~Datum();

		/// <summary>
		/// Equivalence operator between two Datums.
		/// </summary>
		bool operator==(const Datum& rhs) const;

		/// <summary>
		/// UnEquivalence operator between two Datums.
		/// </summary>
		bool operator!=(const Datum& rhs) const;

		/// <summary>
		/// Equivalence operator for comparing against a single int.
		/// </summary>
		bool operator==(int rhs) const;

		/// <summary>
		/// UnEquivalence operator for comparing against a single int.
		/// </summary>
		bool operator!=(int rhs) const;

		/// <summary>
		/// Equivalence operator for comparing against a single float.
		/// </summary>
		bool operator==(float rhs) const;

		/// <summary>
		/// UnEquivalence operator for comparing against a single float.
		/// </summary>
		bool operator!=(float rhs) const;

		/// <summary>
		/// Equivalence operator for comparing against a single string.
		/// </summary>
		bool operator==(const std::string& rhs) const;

		/// <summary>
		/// UnEquivalence operator for comparing against a single int.
		/// </summary>
		bool operator!=(const std::string& rhs) const;

		/// <summary>
		/// Equivalence operator for comparing against a single vector.
		/// </summary>
		bool operator==(const glm::vec4& rhs) const;

		/// <summary>
		/// UnEquivalence operator for comparing against a single vector.
		/// </summary>
		bool operator!=(const glm::vec4& rhs) const;

		/// <summary>
		/// Equivalence operator for comparing against a single matrix.
		/// </summary>
		bool operator==(const glm::mat4& rhs) const;

		/// <summary>
		/// UnEquivalence operator for comparing against a single matrix.
		/// </summary>
		bool operator!=(const glm::mat4& rhs) const;

		/// <summary>
		/// Equivalence operator for comparing against a single rtti pointer.
		/// </summary>
		bool operator==(const RTTI* const rhs) const;

		/// <summary>
		/// UnEquivalence operator for comparing against a single rtti pointer.
		/// </summary>
		bool operator!=(const RTTI* const rhs) const;

		/// <summary>
		/// Returns Datum's current type.
		/// </summary>
		DatumType Type() const;

		/// <summary>
		/// Sets the Datum's type to given.
		/// </summary>
		void SetType(DatumType type);

		/// <summary>
		/// Returns number of elements within Datum.
		/// </summary>
		std::size_t Size() const;

		/// <summary>
		/// Returns total data slots within Datum.
		/// </summary>
		std::size_t Capacity() const;

		/// <summary>
		/// Adjusts Datum capacity to match given size, then fills empty slots with default items of type.
		/// Can shrink Datum's capacity and size if given is smaller than current.
		/// </summary>
		void Resize(std::size_t size);

		/// <summary>
		/// Returns whether this Datum is holding an array of data it does not own.
		/// </summary>
		bool IsExternal() const;

		/// <summary>
		/// Destroys items in Datum but doesn't adjust capacity.
		/// </summary>
		void Clear();
		
		/// <summary>
		/// Sets Datum as a container interface for given array.
		/// </summary>
		/// <param name="size">Size of given array.</param>
		void SetStorageInteger(int* array, std::size_t size);

		/// <summary>
		/// Sets Datum as a container interface for given array.
		/// </summary>
		/// <param name="size">Size of given array.</param>
		void SetStorageFloat(float* array, std::size_t size);

		/// <summary>
		/// Sets Datum as a container interface for given array.
		/// </summary>
		/// <param name="size">Size of given array.</param>
		void SetStorageString(std::string* array, std::size_t size);

		/// <summary>
		/// Sets Datum as a container interface for given array.
		/// </summary>
		/// <param name="size">Size of given array.</param>
		void SetStorageVector(glm::vec4* array, std::size_t size);

		/// <summary>
		/// Sets Datum as a container interface for given array.
		/// </summary>
		/// <param name="size">Size of given array.</param>
		void SetStorageMatrix(glm::mat4* array, std::size_t size);

		/// <summary>
		/// Sets Datum as a container interface for given array.
		/// </summary>
		/// <param name="size">Size of given array.</param>
		void SetStoragePointer(RTTI** array, std::size_t size);

		/// <summary>
		/// Sets item at given index equal to given value.
		/// </summary>
		void SetInteger(int value, std::size_t index = 0);

		/// <summary>
		/// Sets item at given index equal to given value.
		/// </summary>
		void SetFloat(float value, std::size_t index = 0);

		/// <summary>
		/// Sets item at given index equal to given value.
		/// </summary>
		void SetString(const std::string& value, std::size_t index = 0);

		/// <summary>
		/// Sets item at given index equal to given value.
		/// </summary>
		void SetVector(const glm::vec4& value, std::size_t index = 0);

		/// <summary>
		/// Sets item at given index equal to given value.
		/// </summary>
		void SetMatrix(const glm::mat4& value, std::size_t index = 0);

		/// <summary>
		/// Sets item at given index equal to given value.
		/// </summary>
		void SetPointer(RTTI* value, std::size_t index = 0);

		/// <summary>
		/// Sets item at given index equal to given value.
		/// </summary>
		void SetTable(Scope& value, std::size_t index = 0);

		/// <summary>
		/// Sets item at given index equal to value parsed from given string.
		/// Parsed type will be decided from Datum's current type.
		/// </summary>
		void SetFromString(const std::string& given, std::size_t index = 0);

		/// <summary>
		/// Converts string to int.
		/// </summary>
		int FromStringToInteger(const std::string& given) const;

		/// <summary>
		/// Converts string to float.
		/// </summary>
		float FromStringToFloat(const std::string& given) const;

		/// <summary>
		/// Converts string to vector.
		/// </summary>
		glm::vec4 FromStringToVector(const std::string& given) const;

		/// <summary>
		/// Converts string to matrix.
		/// </summary>
		glm::mat4 FromStringToMatrix(const std::string& given) const;

		/// <summary>
		/// Returns item at given index as a string.
		/// </summary>
		std::string ToString(std::size_t index = 0) const;

		/// <summary>
		/// Returns value at given index.
		/// </summary>
		int GetInteger(std::size_t index = 0) const;

		/// <summary>
		/// Returns value at given index.
		/// </summary>
		float GetFloat(std::size_t index = 0) const;

		/// <summary>
		/// Returns value at given index.
		/// </summary>
		const std::string& GetString(std::size_t index = 0) const;

		/// <summary>
		/// Returns value at given index.
		/// </summary>
		const glm::vec4& GetVector(std::size_t index = 0) const;

		/// <summary>
		/// Returns value at given index.
		/// </summary>
		const glm::mat4& GetMatrix(std::size_t index = 0) const;

		/// <summary>
		/// Returns value at given index.
		/// </summary>
		RTTI* const GetPointer(std::size_t index = 0) const;

		/// <summary>
		/// Returns value at given index.
		/// </summary>
		Scope& GetTable(std::size_t index = 0) const;

		/// <summary>
		/// Adds given item to back of Datum.
		/// </summary>
		void PushBackInteger(int value);

		/// <summary>
		/// Adds given item to back of Datum.
		/// </summary>
		void PushBackFloat(float value);

		/// <summary>
		/// Adds given item to back of Datum.
		/// </summary>
		void PushBackString(const std::string& value);

		/// <summary>
		/// Adds given item to back of Datum.
		/// </summary>
		void PushBackVector(const glm::vec4& value);

		/// <summary>
		/// Adds given item to back of Datum.
		/// </summary>
		void PushBackMatrix(const glm::mat4& value);

		/// <summary>
		/// Adds given item to back of Datum.
		/// </summary>
		void PushBackPointer(RTTI* const);

		/// <summary>
		/// Adds item parsed from given string to back of Datum.
		/// Type parsed depends on Datum's current type.
		/// </summary>
		void PushBackFromString(std::string given);

		/// <summary>
		/// Removes last item in Datum.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Returns reference to item at front of Datum.
		/// </summary>
		int& FrontInteger();

		/// <summary>
		/// Returns reference to item at front of Datum.
		/// </summary>
		float& FrontFloat();

		/// <summary>
		/// Returns reference to item at front of Datum.
		/// </summary>
		std::string& FrontString();

		/// <summary>
		/// Returns reference to item at front of Datum.
		/// </summary>
		glm::vec4& FrontVector();

		/// <summary>
		/// Returns reference to item at front of Datum.
		/// </summary>
		glm::mat4& FrontMatrix();

		/// <summary>
		/// Returns reference to item at front of Datum.
		/// </summary>
		RTTI*& FrontPointer();

		/// <summary>
		/// Returns item at front of Datum.
		/// </summary>
		int FrontInteger() const;

		/// <summary>
		/// Returns item at front of Datum.
		/// </summary>
		float FrontFloat() const;

		/// <summary>
		/// Returns item at front of Datum.
		/// </summary>
		const std::string& FrontString() const;

		/// <summary>
		/// Returns item at front of Datum.
		/// </summary>
		const glm::vec4& FrontVector() const;

		/// <summary>
		/// Returns item at front of Datum.
		/// </summary>
		const glm::mat4& FrontMatrix() const;

		/// <summary>
		/// Returns item at front of Datum.
		/// </summary>
		const RTTI* FrontPointer() const;

		/// <summary>
		/// Returns reference to item at back of Datum.
		/// </summary>
		int& BackInteger();

		/// <summary>
		/// Returns reference to item at back of Datum.
		/// </summary>
		float& BackFloat();

		/// <summary>
		/// Returns reference to item at back of Datum.
		/// </summary>
		std::string& BackString();

		/// <summary>
		/// Returns reference to item at back of Datum.
		/// </summary>
		glm::vec4& BackVector();

		/// <summary>
		/// Returns reference to item at back of Datum.
		/// </summary>
		glm::mat4& BackMatrix();

		/// <summary>
		/// Returns reference to item at back of Datum.
		/// </summary>
		RTTI*& BackPointer();

		/// <summary>
		/// Returns item at back of Datum.
		/// </summary>
		int BackInteger() const;

		/// <summary>
		/// Returns item at back of Datum.
		/// </summary>
		float BackFloat() const;

		/// <summary>
		/// Returns item at back of Datum.
		/// </summary>
		const std::string& BackString() const;

		/// <summary>
		/// Returns item at back of Datum.
		/// </summary>
		const glm::vec4& BackVector() const;

		/// <summary>
		/// Returns item at back of Datum.
		/// </summary>
		const glm::mat4& BackMatrix() const;

		/// <summary>
		/// Returns item at back of Datum.
		/// </summary>
		const RTTI* BackPointer() const;

		/// <summary>
		/// Removes first item matching given from the Datum.
		/// </summary>
		bool RemoveInteger(int value);

		/// <summary>
		/// Removes first item matching given from the Datum.
		/// </summary>
		bool RemoveFloat(float value);

		/// <summary>
		/// Removes first item matching given from the Datum.
		/// </summary>
		bool RemoveString(const std::string& value);

		/// <summary>
		/// Removes first item matching given from the Datum.
		/// </summary>
		bool RemoveVector(const glm::vec4& value);

		/// <summary>
		/// Removes first item matching given from the Datum.
		/// </summary>
		bool RemoveMatrix(const glm::mat4& value);

		/// <summary>
		/// Removes first item matching given from the Datum.
		/// </summary>
		bool RemovePointer(const RTTI* const value);
		
		/// <summary>
		/// Removes item at given index in Datum.
		/// </summary>
		void RemoveAt(std::size_t index);

		/// <summary>
		/// Finds given item, and returns pair holding item index and bool telling whether item was found.
		/// </summary>
		std::pair<std::size_t, bool> FindInteger(int value) const;

		/// <summary>
		/// Finds given item, and returns pair holding item index and bool telling whether item was found.
		/// </summary>
		std::pair<std::size_t, bool> FindFloat(float value) const;

		/// <summary>
		/// Finds given item, and returns pair holding item index and bool telling whether item was found.
		/// </summary>
		std::pair<std::size_t, bool> FindString(const std::string& value) const;

		/// <summary>
		/// Finds given item, and returns pair holding item index and bool telling whether item was found.
		/// </summary>
		std::pair<std::size_t, bool> FindVector(const glm::vec4& value) const;

		/// <summary>
		/// Finds given item, and returns pair holding item index and bool telling whether item was found.
		/// </summary>
		std::pair<std::size_t, bool> FindMatrix(const glm::mat4& value) const;

		/// <summary>
		/// Finds given item, and returns pair holding item index and bool telling whether item was found.
		/// </summary>
		std::pair<std::size_t, bool> FindPointer(const RTTI* value) const;


	private:
		void Reserve(std::size_t capacity);
		void PushBackTable(Scope& value);
		std::pair<std::size_t, bool> FindTable(Scope* value) const;
		friend class Attributed;
		void SetStorage(DatumType type, void* array, std::size_t size);

		union DatumValue final
		{
			int* i;
			float* f;
			std::string* s;
			RTTI** r;
			glm::vec4* v;
			glm::mat4* m;
			void* vp;
			std::uint8_t* b;
			Scope** t;
		};
		static const std::size_t _datumTypeSizes[static_cast<std::size_t>(DatumType::End)];

		DatumValue _dataPtr{ nullptr };
		DatumType _type{ DatumType::Unknown };
		std::size_t _size{ 0 };
		std::size_t _capacity{ 0 };
		bool _isExternal = false;
	};
}