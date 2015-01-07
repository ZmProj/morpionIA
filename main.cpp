
#include <iostream>
#include "Engine.h"

#include "MyGraphicEngine.h"
#include "MyGameEngine.h"
#include "MyControlEngine.h"
#include "Square.h"
#include "GameManagement.h"
#include "Etat.h"


int main(int argc, char * argv[])
{

	std::vector<std::vector <Square > > square;
	GameManagement gameManagemement(&square);

	Etat etat;
	etat = Etat::MENU;

	IA ia(&square, &gameManagemement, &etat);

	Engine e(argc, argv, 600, 600, "Morpion_IA");

	GraphicEngine * ge = new MyGraphicEngine(&gameManagemement, &square, &etat);
	GameEngine * gme = new MyGameEngine(&gameManagemement, &square, &etat, &ia);
	ControlEngine * ce = new MyControlEngine(&gameManagemement, &square, &etat, &ia);


	e.setGraphicEngine(ge);
	e.setGameEngine(gme);
	e.setControlEngine(ce);

	e.start();
	return 0;
}