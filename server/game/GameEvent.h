#pragma once

enum GameEvent
{
	Register = 0,
	Auth = 1,
	GetOnlinePlayers = 2, 
	InviteToPlay = 3,
	ReceiveInvite = 4,
	AcceptInvite = 5, 
	Move = 6,
	AddOnlinePlayer = 7,
	RemoveOnlinePlayer = 8,
	GameEvent_MAX = 9,
};