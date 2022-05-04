#pragma once
#include "Scope.h"
#include "Vector.h"
#include "Signature.h"
#include "TypeRegistry.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Attributed derivatives hold scripting objects that can build a hierarchy.
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)

	public:
		/// <summary>
		/// Assignment operator. Meant to be called from derived classes's assignment operator.
		/// </summary>
		Attributed& operator=(const Attributed& other);

		/// <summary>
		/// Move Assignment operator. Meant to be called from derived classes's move assignment operator.
		/// </summary>
		Attributed& operator=(Attributed&& other) noexcept;

		/// <summary>
		/// Virtual Destructor. Ensures no memory leaks.
		/// </summary>
		virtual ~Attributed();

		/// <summary>
		/// Returns whether given name corresponds to an attribute of this object.
		/// </summary>
		bool IsAttribute(const std::string& name) const;

		/// <summary>
		/// Returns whether given name corresponds to an attribute of this class.
		/// </summary>
		bool IsPrescribedAttribute(const std::string& name) const;

		/// <summary>
		/// Returns whether given name corresponds to an auxiliary attribute of this object.
		/// </summary>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// Appends a new Datum to this object as an auxiliary attribute.
		/// </summary>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// Pure virtual clone method so derived classes can create copies of themselves.
		/// </summary>
		virtual gsl::owner<Attributed*> Clone() const override = 0;

	protected:
		/// <summary>
		/// Default constructor. Takes an ID in order to populate the object with the registered 
		/// method signatures of the type.
		/// </summary>
		Attributed(RTTI::IdType id);

		/// <summary>
		/// Copy constructor. Deep copies scope data over, and adjusts external datums.
		/// </summary>
		Attributed(const Attributed& other);

		/// <summary>
		/// Move constructor. Shallow copy.
		/// </summary>
		Attributed(Attributed&& other) noexcept;

		/// <summary>
		/// Clears out all data.
		/// </summary>
		virtual void Clear() override;

		RTTI::IdType _id;

	private:
		void Populate();
		void UpdateExternalStorage();
	};
}