#pragma once

#include <string>
#include <vector>

namespace Sidequest
{
	class Quest;

	class User {
	public:
		typedef unsigned long Id;
		User(Id id);
		User(std::string email, std::string display_name, std::string password);
		~User();

		Id id;
		std::string display_name;
		std::string email;
		std::string password;

		std::vector<Quest*> main_quests;
	};
}