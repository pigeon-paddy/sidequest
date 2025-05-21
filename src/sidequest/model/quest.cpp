#include "quest.h"
#include <algorithm>

namespace Sidequest
{
	Quest::Id Quest::global_id_counter = 1;

	Quest::Quest(std::string caption, Quest* parent) : 		
		id(global_id_counter), caption(caption), parent(parent), status(Status::initial), subquests(std::vector<Quest*>()) {

		++global_id_counter;
	}

	Quest::Quest(std::string caption)
	{
		Quest(caption, nullptr);
	}

	Quest::~Quest()
	{
		for (Quest* subquest : subquests) {
			delete (subquest);
		}
	}

	void Quest::addSubquest(Quest* subquest) {
		subquests.push_back(subquest);
	}

	void Quest::removeSubquest(Id id) {
		subquests.erase(std::remove_if(subquests.begin(), subquests.end(),
			[id](Quest* q) {
				if (q->getId() == id) {
					delete (q);
					return true;
				}
				return false;
			}),
			subquests.end());
	}

	Quest* Quest::findSubquest(Id id) {
		for (Quest* sub : subquests) {
			if (sub->getId() == id) return sub;
		}
		return nullptr;
	}

	void Quest::updateCaption(const std::string& newCaption) {
		caption = newCaption;
	}

	void Quest::updateStatus(Status newStatus) {
		status = newStatus;
	}

	Quest::Id Quest::getId() const { return id; }
	std::string Quest::getCaption() const { return caption; }
	Status Quest::getStatus() const { return status; }
}
