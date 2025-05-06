#pragma once

#include "get_user_command.h"

#include <string>

namespace Sidequest
{
    namespace Server
    {
        void GetUserCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            auto user_id = request.path_params.at("id");
            std::string response_data = "user data for id: " + user_id;
            response.set_content( response_data, "text/plain");
        }

        std::string GetUserCommand::endpoint()
        {
            return "/user/:id/get";
        }
    } // namespace Server
} // namespace Sidequest