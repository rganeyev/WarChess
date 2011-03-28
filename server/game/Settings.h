#pragma once

#include <boost/thread/shared_mutex.hpp>

namespace pugi {
	class xml_node;
}

class Settings {
public:
	boost::shared_mutex rwmutex;

	/*
	unsigned int getLevelByExp(int exp);
	*/

	bool reload(const char* fileName);

};

extern Settings settings;