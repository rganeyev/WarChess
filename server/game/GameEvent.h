#pragma once

enum GameEvent
{
//TODO:implement game events
	Register = 0,
	Auth = 1,
	GetOnlinePlayers = 2, 
	InviteToPlay = 3,
	ReceiveInvite = 4,
	AcceptInvite = 5, 
	Move = 6,
	InformMove = 7,
	GameStart = 8,
	GameEnd   = 9,
	AddOnlinePlayer = 10,
	RemoveOnlinePlayer = 11,
	GameEvent_MAX = 12,
};