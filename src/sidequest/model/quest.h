#pragma once

#include <string>
#include <vector>

namespace Sidequest
{
	enum class Status {
		FINALIZED
	};

	class Quest {
	public:
		typedef unsigned long Id;

		Quest(std::string caption);
		~Quest();

		Id id;
		std::string caption;
		Quest* parent;
		std::vector<Quest*> subquests;
	};
}