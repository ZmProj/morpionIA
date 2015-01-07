#pragma once

#include "Engine.h"
#include "Square.h"
#include "GameManagement.h"
#include "Etat.h"
#include "IA.h"

class MyGameEngine:public GameEngine {

	GameManagement *gameManagement_;
	Etat *etat_;
	std::vector<std::vector <Square> > *square_;
	IA *ia_;
	
public:
    
	MyGameEngine(GameManagement *gameManagement, std::vector<std::vector <Square> > *square, Etat *etat, IA *ia):
		gameManagement_(gameManagement),
		square_(square),
		etat_(etat),
		ia_(ia)
	{}
    
    virtual void idle();
    
};
