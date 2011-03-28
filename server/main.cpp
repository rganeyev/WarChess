#include "Server.h"
#include "CrossDomainPolicyResolver.h"

#include <Windows.h>
#include <process.h>

#include "game/PlayerManager.h"
#include "game/Player.h"
#include "game/Settings.h"
#include "util/String.h"
#include "Console.h"

PlayerManager singleton;
//const char* hostName = "85.249.46.62";
const char* hostName = "localhost";

int main() {
	settings.reload("server.xml");

	Server server;
	server.start(3113);

	CrossDomainPolicyResolver resolver;
	resolver.start();

	Console console;
	console.run();

	return 0;
}