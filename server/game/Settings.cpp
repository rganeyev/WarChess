#include "game/Settings.h"
#include "util/LookUp.h"
#include "pugixml/pugixml.hpp"

#define readInt(node)  (unsigned int)atoi(v.child(node).child_value())
#define readIntVar(node) unsigned int node = readInt(#node)

bool Settings::reload( const char* fileName ) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName);
	if (!result) {
		printf("Failed to parse xml: %s\n", result.description());
		return false;
	}

	try {
		rwmutex.lock();

		pugi::xml_node server = doc.child("server");
		//TODO: connect to server to get exp

	} catch (...) {
		rwmutex.unlock();

		return false;
	}

	rwmutex.unlock();

	return true;
}

/*
unsigned int Settings::getLevelByExp( int exp )
{
	return find(&level_info[0], (unsigned int)level_info.size(), SortByExp(exp)) - 1;
}
*/

Settings settings;