#pragma once

#include "../../../Commun/IStrategyLib.h"

class StrategyTest
{
public:
	StrategyTest(unsigned int id, unsigned int nbPlayer, const SMap* map);
	StrategyTest(const StrategyTest &obj) =delete;
	StrategyTest(StrategyTest &&obj) =delete;
	virtual ~StrategyTest();
	StrategyTest& operator=(const StrategyTest &obj) =delete;
	StrategyTest& operator=(StrategyTest &&obj) =delete;

public:
	bool PlayTurn(unsigned int gameTurn, const SGameState *state, STurn *turn);

protected:
	const unsigned int Id;
	const unsigned int NbPlayer;
	SMap Map;
};
