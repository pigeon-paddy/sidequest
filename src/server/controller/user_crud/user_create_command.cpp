#pragma once

#include "user_create_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        UserCreateCommand::UserCreateCommand(Database* database)
            : database(database)
        {
        }

        void UserCreateCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            auto json = Json::parse(request.body);
            auto user = new ServerUser(database, 0);
            user->from_json(json);

            try {
                user->create_on_database();
            }
            catch (UnableToCreateObjectException& e) 
            {
                response.set_content(Json("unable to create user"), "text/plain");
                response.status = httplib::StatusCode::BadRequest_400;
                return;
            }

            Json json_response;
            json_response["id"] = user->id;
            response.set_content(json_response.dump(), "text/plain");
            response.status = httplib::StatusCode::OK_200;
        }

        std::string UserCreateCommand::endpoint()
        {
            return "/api/user/create";
        }


    } // namespace Server
} // namespace Sidequest