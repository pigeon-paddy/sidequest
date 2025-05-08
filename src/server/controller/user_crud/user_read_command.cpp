#pragma once

#include "user_read_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        UserReadCommand::UserReadCommand(Database* database)
            : database(database)
        {
        }

        void UserReadCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            std::cout << "calling UserReadCommand" << std::endl;
            Id user_id = std::stoul( request.path_params.at("id") );
            auto user = new ServerUser(database, user_id);

            try {
                user->read_on_database();
            }
            catch (UnableToReadObjectException& e) 
            {
                response.set_content(Json("no such user"), "text/plain");
                response.status = httplib::StatusCode::NotFound_404;
                return;
            }

            std::string user_as_json = user->to_json().dump();
            response.set_content(user_as_json, "text/plain");
            response.status = httplib::StatusCode::OK_200;
        }

        std::string UserReadCommand::endpoint()
        {
            return "/api/user/:id/read";
        }


    } // namespace Server
} // namespace Sidequest