#include "serialisable_quest.h"

#include <stdexcept>

#include "model/user.h"
#include "network/serialisable_user.h"

namespace Sidequest 
{

	SerialisableQuest::SerialisableQuest(Id id)
		: Quest(id)
	{
	}

	SerialisableQuest::SerialisableQuest(Status status, std::string title, std::string description, User* owner, User* editor, Quest* parent)
		: Quest( status, title, description, owner, editor, parent )
	{
	}

	SerialisableQuest::~SerialisableQuest()
	{
	}

	Json SerialisableQuest::to_json( bool full_serialise )
	{
		if (!full_serialise)
			return Json( std::to_string(id) );

		Json json = {
			{ "id", id },
			{ "title", title },
			{ "description", description },
			{ "status", status },
		};

		json["parent"] = serialize_loadable_pointer(dynamic_cast<JsonSerialisable*>(parent), parent_id, serialise_parent_recursive);
		json["owner"]  = serialize_loadable_pointer(dynamic_cast<JsonSerialisable*>(owner),  owner_id,  serialise_owner_recursive);
		json["editor"] = serialize_loadable_pointer(dynamic_cast<JsonSerialisable*>(editor), editor_id, serialise_editor_recursive);

		/*json["subquests"] = Json::array();
		for (auto subquest : subquests)
		{
			auto _subquest = dynamic_cast<JsonSerialisable*>(subquest);
			json["subquests"].push_back( _subquest->to_json(!serialise_subquests_recursive) );
		}*/

		return json;
	}

	void SerialisableQuest::from_json(const Json& json)
	{
		// id might not exist of the user data is send as a create-user request,
		// such the id is not yet assigned
		if (json.contains("id"))
			json.at("id").get_to(id);

		json.at("title").get_to(title);
		json.at("description").get_to(description);
		json.at("status").get_to(status);
		parent = deserialize_loadable_pointer<SerialisableQuest>(json["parent"], parent_id);
		owner  = deserialize_loadable_pointer<SerialisableUser> (json["owner"],  owner_id);
		editor = deserialize_loadable_pointer<SerialisableUser> (json["editor"], editor_id);
	}

}