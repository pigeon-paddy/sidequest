#pragma once

#include "client_stubs.h"

#include "httplib.h"

#include <string>
#include <iostream>

#include <json.hpp>
using json = nlohmann::json;

#include "network/serialisable_user.h"
#include "network/serialisable_quest.h"

namespace Sidequest
{
    namespace Client
    {

        RemoteCallFailedException::RemoteCallFailedException(const std::string& message)
            : runtime_error( message )
        {
        }

        RemoteCallFailedException::RemoteCallFailedException(const httplib::Error& error_code)
            : runtime_error(httplib::to_string( error_code ) )
        {
        }

        Stubs::Stubs(httplib::Client&http_connection)
            : _http_client( http_connection )
        {
        }

        Stubs::~Stubs()
        {
        }

        Id Stubs::createUser(SerialisableUser* user)
        {
            std::string body = user->to_json().dump();
            auto result = _http_client.Put("/api/user/create", body, "text/plain");
            if (result.error() != httplib::Error::Success)
                throw RemoteCallFailedException( result.error() );
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("create user failed");

            auto json_response = Json::parse(result.value().body); 

            Id id = json_response.at("id");
            return id;
        }

        SerialisableUser* Stubs::readUser(Id id)
        {
            std::string id_string = std::to_string(id);
            auto result = _http_client.Get("/api/user/" + id_string + "/read");
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("read user failed");
            auto json = Json::parse(result->body);
            SerialisableUser* user = new SerialisableUser( id );
            user->from_json(json);
            return user;
        }

        void Stubs::updateUser(SerialisableUser* user)
        {
            std::string body = user->to_json().dump();
            std::string id_string = std::to_string( user->id );
            auto result = _http_client.Put("/api/user/" + id_string + "/update", body, "text/plain");
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("update user failed");
        }

        void Stubs::deleteUser(Id id)
        {
            std::string id_string = std::to_string(id);
            auto result = _http_client.Delete("/api/user/" + id_string + "/delete");
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("read user failed");
        }

        Id Stubs::createQuest(SerialisableQuest* quest)
        {
            std::string body = quest->to_json().dump();
            auto result = _http_client.Put("/api/quest/create", body, "text/plain");
            if (result.error() != httplib::Error::Success)
                throw RemoteCallFailedException(result.error());
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("create quest failed");

            auto json_response = Json::parse(result.value().body);

            Id id = json_response.at("id");
            return id;
        }

        SerialisableQuest* Stubs::readQuest(Id id)
        {
            std::string id_string = std::to_string(id);
            auto result = _http_client.Get("/api/quest/" + id_string + "/read");
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("read quest failed");
            auto json = Json::parse(result->body);
            SerialisableQuest* quest = new SerialisableQuest(id);
            quest->from_json(json);
            return quest;
        }

        void Stubs::updateQuest(SerialisableQuest* quest)
        {
            std::string body = quest->to_json().dump();
            std::string id_string = std::to_string(quest->id);
            auto result = _http_client.Put("/api/quest/" + id_string + "/update", body, "text/plain");
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("update quest failed");
        }

        void Stubs::deleteQuest(Id id)
        {
            std::string id_string = std::to_string(id);
            auto result = _http_client.Delete("/api/quest/" + id_string + "/delete");
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("read user failed");
        }
        
    } // namespace Server
} // namespace Sidequest