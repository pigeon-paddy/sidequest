#pragma once

#include <string>
#include <vector>

namespace Sidequest
{
	class User;

	class Quest {
	public:
		typedef unsigned long Id;
		enum Status {
			initial,
			done,
			inactive
		};

		Quest( Id id );
		Quest( Id id, Status status, std::string title, std::string description, User* owner, User* editor, Quest* parent );
		~Quest();

		static std::string status_to_string( Status status );
		static Status Quest::string_to_status(std::string status);

		Id id;
		std::string title;
		std::string description;
		Status status;

		User* owner;
		User* editor;

		Quest* parent;
		std::vector<Quest*> subquests;
	};
}