
#include "MyGameEngine.h"


void MyGameEngine::idle(){

	switch ((*etat_))
	{
	case MENU:
		break;
	case INGAME:
		if (!gameManagement_->resteCaseLibre()){
			(*etat_) = Etat::END_GAME;
		}
		break;
	case END_TURN:
		if (!gameManagement_->resteCaseLibre()){
			(*etat_) = Etat::END_GAME;
			break;
		}
		if ((*gameManagement_).getCurrentPlayer() == 1 && (*gameManagement_).getTypeJ1() == 0){
			(*etat_) = Etat::INGAME;
		}
		else if ((*gameManagement_).getCurrentPlayer() == 0 && (*gameManagement_).getTypeJ2() != 0){
			(*etat_) = Etat::IA_TO_PLAY;
		}
		else (*etat_) = Etat::INGAME;
		(*gameManagement_).endPlayerTurn();
		break;
	case IA_TO_PLAY:
			
		if (gameManagement_->resteCaseLibre()){
			(*etat_) = Etat::END_TURN;
			(*ia_).toPlay();
		}
		break;
	case END_GAME:
		break;
	default:
		break;
	}

}