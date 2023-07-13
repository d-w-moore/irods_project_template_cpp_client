#include <iostream>
#include <irods/lsUtil.h> // For load_client_api_plugins()
#include <irods/rodsClient.h> // For load_client_api_plugins()
#include <irods/client_connection.hpp>
#include <irods/filesystem.hpp>
#include <irods/irods_exception.hpp>

#include <fmt/format.h>
#include <irods/irods_query.hpp>

int main(int _argc, char* _argv[]) // NOLINT(modernize-use-trailing-return-type)
{
    (void) _argc;
    (void) _argv;
    /*
	if (_argc != 2) {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		fmt::print("usage: {} LOGICAL_PATH\n", _argv[0]);
		return 1;
	}

        */

	load_client_api_plugins();

	namespace fs = irods::experimental::filesystem;

	try {
		irods::experimental::client_connection conn;

                RcComm* connPtr {conn};

                rodsEnv client_environment;
                auto status = getRodsEnv(&client_environment);
                if (status < 0) {
                    std::cerr << "bad environment" << std::endl;
                    exit(1);
                }

                char MyTicket[]{"myticket"};
                setSessionTicket(connPtr, MyTicket);
                irods::query<RcComm> q{connPtr,"select COLL_NAME,DATA_NAME"};
                for (const auto & i:q) {
                    std::cout << i[0] <<'/'<<i[1]<< std::endl;
                }

                /**
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		for (const auto& e : fs::client::collection_iterator{conn, _argv[1]}) {
			fmt::print("{}\n", e.path().c_str());
		}
                **/

		return 0;
	}
	catch (const fs::filesystem_error& e) {
		fmt::print(stderr, "error: {}\n", e.what());
	}
	catch (const irods::exception& e) {
		fmt::print(stderr, "error: {}\n", e.client_display_what());
	}
	catch (const std::exception& e) {
		fmt::print(stderr, "error: {}\n", e.what());
	}

	return 1;
}
