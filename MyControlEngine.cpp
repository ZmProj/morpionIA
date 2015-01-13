
#include "MyControlEngine.h"


void MyControlEngine::MouseCallback(int button, int state, int x, int y){
	bool isHumainToPlay;

	int indiceY;
	int indiceX;

	float clickX = (x - (600 / 2.f)) / 300;
	float clickY = (-y + (600 / 2.f)) / 300;
	
	switch (*etat_){
	case MENU:
		// Nombre de cases
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= -0.49f && clickX <= -0.18f && clickY >= 0.71f && clickY <= 0.89f){
			(*gameManagement_).setN(3);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= -0.16f && clickX <= 0.15f && clickY >= 0.71f && clickY <= 0.89f){
			(*gameManagement_).setN(5);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= 0.17f && clickX <= 0.48f && clickY >= 0.71f && clickY <= 0.89f){
			(*gameManagement_).setN(10);
		}

		// J1
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= -0.49f && clickX <= -0.01f && clickY >= 0.21f && clickY <= 0.39f){
			(*gameManagement_).setTypeJ1(0);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= -0.49f && clickX <= -0.01f && clickY >= 0.01f && clickY <= 0.19f){
			(*gameManagement_).setTypeJ1(1);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= -0.49f && clickX <= -0.01f && clickY >= -0.19f && clickY <= -0.01f){
			(*gameManagement_).setTypeJ1(2);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= -0.49f && clickX <= -0.01f && clickY >= -0.39f && clickY <= -0.21f){
			(*gameManagement_).setTypeJ1(3);
		}

		// J2
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= 0.01f && clickX <= 0.49f && clickY >= 0.21f && clickY <= 0.39f){
			(*gameManagement_).setTypeJ2(0);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= 0.01f && clickX <= 0.49f && clickY >= 0.01f && clickY <= 0.19f){
			(*gameManagement_).setTypeJ2(1);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= 0.01f && clickX <= 0.49f && clickY >= -0.19f && clickY <= -0.01f){
			(*gameManagement_).setTypeJ2(2);
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= 0.01f && clickX <= 0.49f && clickY >= -0.39f && clickY <= -0.21f){
			(*gameManagement_).setTypeJ2(3);
		}

		// JOUER
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickX >= -0.24f && clickX <= 0.24f && clickY >= -0.79f && clickY <= -0.61f){
			(*gameManagement_).drawSquare();
			if ((*gameManagement_).getTypeJ1() == 0 && (*gameManagement_).getCurrentPlayer() == 0
				|| (*gameManagement_).getTypeJ2() == 0 && (*gameManagement_).getCurrentPlayer() == 1){
				*etat_ = INGAME;
			}
			else{
				*etat_ = IA_TO_PLAY;
			}
			
		}
		break;
	case INGAME:
		indiceY = y / ((*square_)[0][0].getSide() * (600 / 2.f)) + 1;    // 600 correspond à la taille(height) de la fenêtre
		indiceX = x / ((*square_)[0][0].getSide() * (600 / 2.f)) + 1; // 600 correspond à la taille(width) de la fenêtre
		if ((*gameManagement_).getTypeJ1() == 0 && (*gameManagement_).getCurrentPlayer() == 0 
			|| (*gameManagement_).getTypeJ2() == 0 && (*gameManagement_).getCurrentPlayer() == 1){
			isHumainToPlay = true;
		}
		else{
			isHumainToPlay = false;
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (*gameManagement_).getSquare()[indiceY - 1][indiceX - 1].getClickedBy() == -1 && isHumainToPlay) {
			if ((*gameManagement_).getCurrentPlayer() == 0){
				(*square_)[indiceY - 1][indiceX - 1].changeColorJ1();
				(*square_)[indiceY - 1][indiceX - 1].setClickedBy(0);
			}
			else {
				(*square_)[indiceY - 1][indiceX - 1].changeColorJ2();
				(*square_)[indiceY - 1][indiceX - 1].setClickedBy(1);
			}
			(*ia_).setLastCoupJoueur(indiceY - 1, indiceX - 1);
			if ((*gameManagement_).verifVainqueur(indiceY - 1, indiceX - 1, *square_) == 0){
				std::cout << "J1 WIN !" << std::endl;
				*etat_ = END_GAME;
				break;
			}
			else if ((*gameManagement_).verifVainqueur(indiceY - 1, indiceX - 1, *square_) == 1){
				std::cout << "J2 WIN !" << std::endl;
				*etat_ = END_GAME;
				break;
			}
			else if ((*gameManagement_).verifVainqueur(indiceY - 1, indiceX - 1, *square_) == -1){
				std::cout << "MATCH NUL !" << std::endl;
				*etat_ = END_GAME;
				break;
			}
			(*etat_) = Etat::END_TURN;
		}
		break;
	case END_GAME:
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
			(*etat_) = MENU;
			(*gameManagement_).resetGame();
		}
		break;
	}
}