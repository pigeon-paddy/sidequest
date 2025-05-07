#include "user.h"

#include <stdlib.h>

#include <json.hpp>
using namespace nlohmann;

namespace Sidequest 
{

	User::User(Id id)
		: id(id)
	{
	}

	User::User(std::string email, std::string display_name, std::string password)
		: id(0), email(email), display_name(display_name), password(password)
	{
	}

	User::~User()
	{
	}

	Json& User::to_json()
	{
		_json = {
			{ "id", std::to_string(id) },
			{ "display_name", display_name },
			{ "email", email }
		};
		return _json;
	}

	void User::from_json(const Json& json)
	{
		json.at("id").get_to(id);
		json.at("display_name").get_to(display_name);
		json.at("email").get_to(email);
	}
}
