#pragma once

enum Result 
{
	Success = 0,
	PlayerAlreadyExists = 1,
	PlayerNotFound = 2,
	PasswordIsIncorrect = 3,
	PlayerNotAuthorized = 4,
	IncorrectMessage = 5,
	UnknownMethod = 6,
	OutOfMemory = 7,
	PlayerAlreadyAuthorized = 8,
	PlayerAlreadyChallenged = 9,
	UnknownProblem = 10,
};
