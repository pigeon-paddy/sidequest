#pragma once

#include "quest_create_command.h"

#include <string>

#include "model/server_quest.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        QuestCreateCommand::QuestCreateCommand(Database* database)
            : database(database)
        {
        }

        void QuestCreateCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            auto json = Json::parse(request.body);
            auto quest = new ServerQuest(database, 0);
            quest->from_json(json);

            try {
                quest->create_on_database();
            }
            catch (UnableToCreateObjectException& e) 
            {
                response.set_content(Json("unable to create quest"), "text/plain");
                response.status = httplib::StatusCode::BadRequest_400;
                return;
            }

            response.set_content(Json({"id", quest->id } ).dump(), "text/plain");
            response.status = httplib::StatusCode::OK_200;
        }

        std::string QuestCreateCommand::endpoint()
        {
            return "/api/quest/create";
        }


    } // namespace Server
} // namespace Sidequest