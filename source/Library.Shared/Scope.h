#pragma once
#include "RTTI.h"
#include "Hashmap.h"
#include "Vector.h"
#include "Datum.h"
#include "Factory.h"
#include <gsl/gsl>

using namespace std::string_literals;

namespace FieaGameEngine
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	public:
		/// <summary>
		/// Default constructor, can be initialized with capacity.
		/// </summary>
		Scope(std::size_t capacity = 11);

		/// <summary>
		/// Copy constructor, constructs Scope from given scope.
		/// Does not copy the given's parent reference.
		/// </summary>
		Scope(const Scope& other);

		/// <summary>
		/// Move constructor, constructs Scope from given R value Scope.
		/// </summary>
		Scope(Scope&& other) noexcept;

		/// <summary>
		/// Copy assignment operator, sets Scope equal to rhs Scope.
		/// </summary>
		Scope& operator=(const Scope& other);

		/// <summary>
		/// Move assignment operator, sets Scope equal to rhs R value Scope.
		/// rhs Scope then has values "zeroed out".
		/// </summary>
		Scope& operator=(Scope&& other) noexcept;

		/// <summary>
		/// Destructor, ensures no memory leaks.
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Equivalence operator returns whether this Scope is equal to rhs Scope.
		/// </summary>
		bool operator==(const Scope& rhs) const;

		/// <summary>
		/// UnEquivalence operator returns whether this Scope is unequal to rhs Scope.
		/// </summary>
		/// <param name="rhs"></param>
		bool operator!=(const Scope& rhs) const;

		/// <summary>
		/// Override of RTTI equals method to determine equality between two RTTI pointers.
		/// </summary>
		virtual bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns number of elements stored within this scope.
		/// </summary>
		/// <returns></returns>
		std::size_t Size() const;

		/// <summary>
		/// Allocates a new Scope object on heap at returned address.
		/// </summary>
		virtual gsl::owner<Scope*> Clone() const;

		/// <summary>
		/// Returns address of Datum associated with given key found within this Scope. If not found, nullptr.
		/// </summary>
		Datum* Find(const std::string& name);

		/// <summary>
		/// Returns address of const Datum associated with given key found within this Scope. If not found, nullptr.
		/// </summary>
		const Datum* Find(const std::string& name) const;

		/// <summary>
		/// Returns address of Datum associated with given key found within this Scope or its ancestors. If not found, nullptr.
		/// </summary>
		Datum* Search(const std::string& name);

		/// <summary>
		/// Returns address of const Datum associated with given key found within this Scope or its ancestors. If not found, nullptr.
		/// </summary>
		const Datum* Search(const std::string& name) const;

		/// <summary>
		/// Returns address of Datum associated with given key found within this Scope or its ancestors. If not found, nullptr.
		/// container output parameter will hold address of Scope holding found Datum.
		/// </summary>
		Datum* Search(const std::string& name, Scope*& foundScope);

		/// <summary>
		/// Returns address of const Datum associated with given key found within this Scope or its ancestors. If not found, nullptr.
		/// container output parameter will hold address of Scope holding found Datum.
		/// </summary>
		const Datum* Search(const std::string& name, const Scope*& foundScope) const;

		/// <summary>
		/// Returns address of Datum associated with given key found within this Scope or its decendants. If not found, nullptr.
		/// </summary>
		std::pair<Scope*, std::size_t> DownSearch(const std::string& name);

		/// <summary>
		/// Uses both Search and Find to locate the first datum with this name in full hierarchy.
		/// </summary>
		Datum* SearchForTargetDatum(const std::string& name);

		/// <summary>
		/// Uses both Search and Find to locate the first Datum with this name in full hierarchy.
		/// Returns the containing Scope and index at which Datum is located.
		/// </summary>
		std::pair<Scope*, std::size_t> SearchForTargetContainedDatum(const std::string& name);

		/// <summary>
		/// Returns ref to Datum associated with given name within this Scope. If not found, new Datum is created for that name.
		/// </summary>
		Datum& Append(const std::string& name);

		/// <summary>
		/// Returns ref to Datum associated with given name within this Scope. If not found, new Datum is created for that name.
		/// </summary>
		Datum& Append(const std::string& name, bool& wasInserted);

		/// <summary>
		/// Returns ref to nested Scope associated with given name within this Scope. 
		/// If not found, new Scope is created for that name.
		/// </summary>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		/// Takes given Scope and adds it as a child to this scope under given name. 
		/// Removes reference to given scope from its old parent. Throws exception if 
		/// name exists already with different Datum type.
		/// </summary>
		void Adopt(Scope& child, const std::string& name);

		/// <summary>
		/// Get address of Scope's parent, nullptr if it has none.
		/// </summary>
		Scope* GetParent() const;

		/// <summary>
		/// Calls Append.
		/// </summary>
		Datum& operator[](const std::string& name);

		/// <summary>
		/// Returns Datum ref if found, throws exception if not found.
		/// </summary>
		const Datum& operator[](const std::string& name) const;

		/// <summary>
		/// Returns ref to Datum at given index. Indices correspond to order of insert.
		/// </summary>
		Datum& operator[](std::size_t index);
		
		/// <summary>
		/// Returns ref to Datum at given index. Indices correspond to order of insert.
		/// </summary>
		const Datum& operator[](std::size_t index) const;

		/// <summary>
		/// Returns Datum holding found Scope with given address and index within Datum at 
		/// which Scope is located.
		/// </summary>
		std::pair<Datum*, std::size_t> FindContainedScope(const Scope& scope) const;

		/// <summary>
		/// Clears Scope of all entries.
		/// </summary>
		virtual void Clear();

		/// <summary>
		/// Removes child Scope from this Scope.
		/// </summary>
		/// <param name="child"></param>
		void Orphan(Scope& child);

		/// <summary>
		/// ToString override for Scope.
		/// </summary>
		virtual std::string ToString() const override
		{
			return "Scope";
		}

	protected:
		std::size_t IndexOf(const std::string& name) const;

	private:
		bool IsAncestorOf(const Scope& scope) const;
		bool IsDescendantOf(const Scope& scope) const;

		Scope* _parent{ nullptr };
		Hashmap<std::string, Datum> _unorderedMap;
		Vector<typename Hashmap<std::string, Datum>::pair_type*> _orderedVector;
	};

	ConcreteFactory(Scope, RTTI)
}