#pragma once

#include <string>
#include <vector>

namespace Sidequest
{
	class User;


	enum class Status {
		initial,
		done,
		inactive
	};

	class Quest {
	public:
		typedef unsigned long Id;

		Quest(std::string caption, Quest* parent);
		Quest(std::string caption);

		~Quest();

		void addSubquest(Quest* subquest);
		void removeSubquest(Quest::Id id);
		Quest* findSubquest(Quest::Id id);
		void updateCaption(const std::string& newCaption);
		void updateStatus(Status newStatus);

		Id getId() const;
		std::string getCaption() const;
		Status getStatus() const;

	private:
		static Id global_id_counter;

		Id id;
		std::string caption;
		Status status;
		Quest* parent;
		std::vector<Quest*> subquests;
	};
}