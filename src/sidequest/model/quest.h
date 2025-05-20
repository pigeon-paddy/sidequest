#pragma once

#include <string>
#include <vector>

namespace Sidequest
{
	enum class Status {
		OPEN,
		CLOSED,
		FINALIZED
	};

	class Quest {
	public:
		typedef unsigned long Id;

		Quest(std::string caption);
		~Quest();

		Id id = 0;
		std::string caption;
		Status status;
		Quest* parent;
		std::vector<Quest*> subquests;
	};
}