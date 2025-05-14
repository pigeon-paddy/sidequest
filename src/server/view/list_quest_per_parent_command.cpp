#pragma once

#include "list_quest_per_parent_command.h"

#include <string>

#include "storage/database.h"

#include "model/server_user.h"
#include "model/server_quest.h"


namespace Sidequest
{
    namespace Server
    {

        ListQuestPerParentCommand::ListQuestPerParentCommand(Database* database)
            : database(database)
        {
        }

        void ListQuestPerParentCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            Id parent_id = std::stoul(request.path_params.at("id"));

            auto query = Query(database, "SELECT * FROM quest WHERE parent = ?;");
            query.bind(1, parent_id);
            query.next_row();
            
            Json result = { };
            while (query.has_rows())
            {
                ServerQuest* quest;
                try {
                    quest = new ServerQuest(database);
                    quest->read_from_query(query);
                }
                catch (UnableToReadObjectException& e)
                {
                    response.set_content(Json("no such domain object"), "text/plain");
                    response.status = httplib::StatusCode::NotFound_404;
                    return;
                }

                result.push_back( quest->to_json() );
                query.next_row();
            }
    
            std::string result_as_json_string = result.dump();
            response.set_content(result_as_json_string, "text/plain");
            response.status = httplib::StatusCode::OK_200;

        }

    } // namespace Server
} // namespace Sidequest
