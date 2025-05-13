#pragma once

#include <string>

#include "httplib.h"

#include "model/id.h"

namespace Sidequest
{
	class SerialisableUser;
	class SerialisableQuest;

	namespace Client
	{
		typedef httplib::Request Request;
		typedef httplib::Response Response;

		class RemoteCallFailedException : public std::runtime_error
		{
		public:
			explicit RemoteCallFailedException(const std::string& message);
			explicit RemoteCallFailedException(const httplib::Error& error_code);
		};

		class Stubs
		{
		public:
			Stubs( httplib::Client& http_connection );
			~Stubs();

			Id createUser(SerialisableUser* user);
			SerialisableUser* readUser(Id id);
			void updateUser(SerialisableUser* user);
			void deleteUser(Id id);

			Id createQuest(SerialisableQuest* user);
			SerialisableQuest* readQuest(Id id);
			void updateQuest(SerialisableQuest* user);
			void deleteQuest(Id id);

		protected:
			httplib::Client& _http_client;
		};

	}
}