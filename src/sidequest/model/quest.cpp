#include "quest.h"

namespace Sidequest 
{

	Quest::Quest(std::string caption)
	{
		this->id++;
		this->caption = caption;
		this->parent = nullptr;
		this->status = Status::OPEN;
		this->subquests = std::vector<Quest*>();
	}
	
	Quest::~Quest()
	{
		for (Quest* subquest : subquests) {
			delete subquest;
		}
		subquests.clear();
	}
}