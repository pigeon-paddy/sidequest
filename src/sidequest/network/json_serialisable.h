#pragma once

#include <json.hpp>
typedef nlohmann::json Json;

namespace Sidequest
{

	/* All classes that can be transferred via network should implement this interface */
	class JsonSerialisable
	{
	public:
		virtual Json& to_json() = 0;
		virtual void  from_json( const Json& json ) = 0;

	protected:
		Json _json;
	};

}