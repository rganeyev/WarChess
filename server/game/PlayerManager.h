#pragma once

#include "util/FixedArray.h"
#include "util/AtomicPointer.h"
#include <set>

class Player;

class PlayerManager {
public:
	PlayerManager();
	~PlayerManager();

	static PlayerManager* instance();

	Player* getPlayer(unsigned int id);
	Player* addPlayer(Player* player);
	bool addPlayerToOnline(unsigned int id);
	bool removePlayerFromOnline(unsigned int id);
	std::set<unsigned int> getOnlinePlayers();

private:
	AtomicPointer< FixedArray<Player*> > _players;
	std::set<unsigned int> onlinePlayers;
	static PlayerManager* _instance;
};