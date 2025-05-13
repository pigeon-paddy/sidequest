#pragma once

#include "quest_update_command.h"

#include <string>

#include "model/server_quest.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        QuestUpdateCommand::QuestUpdateCommand(Database* database)
            : database(database)
        {
        }

        void QuestUpdateCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            auto json = Json::parse(request.body);
            std::cout << json << std::endl;
            auto user = new ServerQuest(database, 0);
            user->from_json(json);

            try {
                user->update_on_database();
            }
            catch (UnableToCreateObjectException& e) 
            {
                response.set_content(Json("unable to update user"), "text/plain");
                response.status = httplib::StatusCode::BadRequest_400;
                return;
            }

            response.set_content(Json().dump(), "text/plain");
            response.status = httplib::StatusCode::OK_200;
        }

        std::string QuestUpdateCommand::endpoint()
        {
            return "/api/quest/:id/update";
        }

    } // namespace Server
} // namespace Sidequest