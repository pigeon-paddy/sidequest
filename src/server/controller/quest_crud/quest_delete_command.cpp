#pragma once

#include "quest_delete_command.h"

#include <string>

#include "model/server_quest.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        QuestDeleteCommand::QuestDeleteCommand(Database* database)
            : database(database)
        {
        }

        void QuestDeleteCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            std::cout << "calling UserReadCommand" << std::endl;
            Id user_id = std::stoul(request.path_params.at("id"));
            auto user = new ServerQuest(database, user_id);

            try {
                user->delete_on_database();
            }
            catch (UnableToCreateObjectException& e) 
            {
                response.set_content(Json("unable to delete user"), "text/plain");
                response.status = httplib::StatusCode::BadRequest_400;
                std::cout << "unable to delete user" << std::endl;
                return;
            }

            response.set_content("", "text/plain");
            response.status = httplib::StatusCode::OK_200;
            std::cout << "user deletes successfully" << std::endl;
        }

        std::string QuestDeleteCommand::endpoint()
        {
            return "/api/quest/:id/delete";
        }

    } // namespace Server
} // namespace Sidequest