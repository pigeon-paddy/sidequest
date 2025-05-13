#pragma once

#include "user_update_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        UserUpdateCommand::UserUpdateCommand(Database* database)
            : database(database)
        {
        }

        void UserUpdateCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            auto json = Json::parse(request.body);
            auto user = new ServerUser(database, 0);
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

        std::string UserUpdateCommand::endpoint()
        {
            return "/api/user/:id/update";
        }

    } // namespace Server
} // namespace Sidequest