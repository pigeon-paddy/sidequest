#pragma once

#include <string>
#include <vector>

#include "network/json_serialisable.h"

namespace Sidequest
{
	class Quest;

	class User : public JsonSerialisable {
	public:
		typedef unsigned long Id;
		User(Id id);
		User(std::string email, std::string display_name, std::string password);
		~User();

		// implementation of JsonSerialisable
		virtual Json& to_json() override;
		virtual void  from_json(const Json& json) override;

		Id id;
		std::string display_name;
		std::string email;
		std::string password;

		std::vector<Quest*> main_quests;
	};
}