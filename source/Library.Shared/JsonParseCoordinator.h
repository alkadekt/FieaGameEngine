#pragma once
#include <gsl/gsl>
#include <json/json.h>
#include <fstream>
#include "RTTI.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class IJsonParseHelper;

	/// <summary>
	/// Parse coordinator uses the chain of responsibility pattern to parse a JSON grammer
	/// using helper classes designed to handle a specific grammer type.
	/// </summary>
	class JsonParseCoordinator final
	{
	public:
		/// <summary>
		/// ParsedDataContainer is an abstract class nested within ParseCoordinator that is used to 
		/// share parsed data between parse helper classes and the parse coordinator. Data parsed is 
		/// stored in a specialized data type determined by derived classes.
		/// </summary>
		class ParsedDataContainer : public RTTI
		{
			RTTI_DECLARATIONS(ParsedDataContainer, RTTI)

			friend class JsonParseCoordinator;

		public:
			/// <summary>
			/// Creates a cloned version of this container within a shared_ptr
			/// </summary>
			virtual gsl::owner<ParsedDataContainer*> Create() const = 0;

			/// <summary>
			/// Initializes any members that require it
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// Cleans up any members that require it.
			/// </summary>
			virtual void Cleanup();

			/// <summary>
			/// Pure virtual method to be overriden by derived classes.
			/// </summary>
			virtual ~ParsedDataContainer() = 0;

			/// <summary>
			/// Returns pointer to associated coordinator.
			/// </summary>
			JsonParseCoordinator* GetJsonParseCoordinator() const;

			/// <summary>
			/// Increases depth value by 1.
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// Decreases depth value by 1.
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// Returns current hierachy depth.
			/// </summary>
			/// <returns></returns>
			std::size_t Depth() const;

		private:
			void SetJsonParseCoordinator(JsonParseCoordinator* coordinator);
			JsonParseCoordinator* _coordinator{ nullptr };
			std::size_t _depth{ 0 };
		};

		/// <summary>
		/// Constructor that takes a smart ptr to a container.
		/// </summary>
		JsonParseCoordinator(ParsedDataContainer* container);

		JsonParseCoordinator(const JsonParseCoordinator& other) = delete;
		JsonParseCoordinator& operator=(const JsonParseCoordinator& other) = delete;

		/// <summary>
		/// Move constructor makes a shallow copy and leaves argument coordinator in
		/// an untouchable state.
		/// </summary>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Move assignment operator makes a shallow copy and leaves argument coordinator in
		/// an untouchable state.
		/// </summary>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Destructor calls cleanup on data members.
		/// </summary>
		~JsonParseCoordinator();

		/// <summary>
		/// Calls initialize on all data members.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Calls cleanup on all data members.
		/// </summary>
		void Cleanup();

		/// <summary>
		/// Creates a heap allocated JsonParseCoordinator that end user must delete.
		/// </summary>
		gsl::owner<JsonParseCoordinator*> Clone() const;

		/// <summary>
		/// Returns whether this coordinator is a clone.
		/// </summary>
		bool IsClone() const;

		/// <summary>
		/// Adds a new parse helper to this coordinator. Passed as smart pointer.
		/// </summary>
		void AddHelper(std::shared_ptr<IJsonParseHelper> helper);

		/// <summary>
		/// Removes given parse helper from the this coordinator.
		/// </summary>
		void RemoveHelper(const std::shared_ptr<IJsonParseHelper>& helper);

		/// <summary>
		/// Returns vector holding smart pointers to associated helpers.
		/// </summary>
		const Vector<std::shared_ptr<IJsonParseHelper>>& Helpers();

		/// <summary>
		/// Parses a string and stores data in container.
		/// </summary>
		void Parse(const std::string& data);

		/// <summary>
		/// Parses an istream and stores data in container.
		/// </summary>
		void Parse(std::istream& data);

		/// <summary>
		/// Parses data from given filename.
		/// </summary>
		void ParseFromFile(const std::string& filename);

		/// <summary>
		/// Returns the ParsedDataContainer associated with this coordinator.
		/// </summary>
		ParsedDataContainer* GetParsedDataContainer();

		/// <summary>
		/// Sets the ParsedDataContainer associated with this coordinator.
		/// </summary>
		/// <param name="address"></param>
		void SetParsedDataContainer(ParsedDataContainer* address);

	private:
		void ParseMembers(Json::Value& value);
		void Parse(const std::string& key, Json::Value& value, bool isArray);

		bool _isClone{ false };
		ParsedDataContainer* _dataContainer;
		Vector<std::shared_ptr<IJsonParseHelper>> _helpers;
	};
}