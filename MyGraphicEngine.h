#pragma once
#include "Engine.h"
#include "GraphicPrimitives.h"
#include "Square.h"
#include "ComputingTime.h"
#include "Timer.h"
#include "Etat.h"
#include "GameManagement.h"

class MyGraphicEngine :public GraphicEngine {
	float squareSide_;
	ComputingTime computingTime_;
	Timer timer_;
	Etat *etat_;
	GameManagement *gameManagement_;
	std::vector<std::vector <Square> > *square_;
	char * str;
public:

	MyGraphicEngine(GameManagement *gameManagement, std::vector<std::vector <Square> > *square, Etat *etat) :
		timer_(10),
		square_(square),
		gameManagement_(gameManagement),
		etat_(etat)
	{
		timer_.launch();
	}

	virtual void Draw();

};
