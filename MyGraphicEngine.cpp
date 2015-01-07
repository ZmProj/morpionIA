
#include "MyGraphicEngine.h"
#include "Square.h"
#include "utility.h"
#include <cstring>
#include "Etat.h"


void MyGraphicEngine::Draw(){
	// dessiner que quand nécessaire pour améliorer les perfs
	// utiliser donc un observer pattern
	// probleme : si on ne redessine pas à chaque tick d'horloge, l'affichage est repeint et tt est supprimé de l'écran (= ecran noir)

	// dessine les lignes du morpion
	/*computingTime_.start();
	for (unsigned int i = 0; i < N_; i++){
	GraphicPrimitives::drawLine2D(-1 + i * squareSide_, 1.f, -1 + i * squareSide_, -1.f, 1.0f, 1.0f, 1.0f);
	GraphicPrimitives::drawLine2D(-1, 1 - i * squareSide_, 1, 1 - i * squareSide_, 1.0f, 1.0f, 1.0f);
	}
	//std::cout << "execution time : " << computingTime_.getTimeElapsed() << std::endl;

	if (timer_.isNotFinished()){
	//std::cout << "second : " << timer_.getSecondsElapsed() << " total time : " << computingTime_.getTotalTime() << std::endl;
	}*/

	char * cRows = new char[10];
	char * cColumns = new char[10];

	switch (*etat_){
	case MENU:
		// Nombre de cases 3 X 3
		GraphicPrimitives::drawFillRect2D(-0.5f, 0.7f, 0.33f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.49f, 0.71f, 0.31f, 0.18f, 0.f, 0.f, 0.f);

		// Nombre de cases 5 X 5
		GraphicPrimitives::drawFillRect2D(-0.17f, 0.7f, 0.33f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.16f, 0.71f, 0.31f, 0.18f, 0.f, 0.f, 0.f);

		// Nombre de cases 10 X 10
		GraphicPrimitives::drawFillRect2D(0.16f, 0.7f, 0.33f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.17f, 0.71f, 0.31f, 0.18f, 0.f, 0.f, 0.f);

		switch ((*gameManagement_).getN())
		{
		case 3:
			GraphicPrimitives::drawFillRect2D(-0.49f, 0.71f, 0.31f, 0.18f, 0.f, 1.f, 0.f);
			break;
		case 5:
			GraphicPrimitives::drawFillRect2D(-0.16f, 0.71f, 0.31f, 0.18f, 0.f, 1.f, 0.f);
			break;
		case 10:
			GraphicPrimitives::drawFillRect2D(0.17f, 0.71f, 0.31f, 0.18f, 0.f, 1.f, 0.f);
			break;
		default:
			break;
		}

		GraphicPrimitives::drawText2D("3 X 3", -0.4f, 0.78f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("5 X 5", -0.07f, 0.78f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("10 X 10", 0.22f, 0.78f, 1.f, 1.f, 1.f);

		// J1
		GraphicPrimitives::drawFillRect2D(-0.5f, 0.4f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.49f, 0.41f, 0.48f, 0.18f, 0.f, 0.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.5f, 0.2f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.49f, 0.21f, 0.48f, 0.18f, 0.0f, 0.0f, 0.0f);
		GraphicPrimitives::drawFillRect2D(-0.5f, 0.0f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.49f, 0.01f, 0.48f, 0.18f, 0.f, 0.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.5f, -0.2f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.49f, -0.19f, 0.48f, 0.18f, 0.f, 0.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.5f, -0.4f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.49f, -0.39f, 0.48f, 0.18f, 0.f, 0.f, 0.f);

		switch ((*gameManagement_).getTypeJ1())
		{
		case 0:
			GraphicPrimitives::drawFillRect2D(-0.49f, 0.21f, 0.48f, 0.18f, 0.0f, 1.0f, 0.0f);
			break;
		case 1:
			GraphicPrimitives::drawFillRect2D(-0.49f, 0.01f, 0.48f, 0.18f, 0.f, 1.f, 0.f);
			break;
		case 2:
			GraphicPrimitives::drawFillRect2D(-0.49f, -0.19f, 0.48f, 0.18f, 0.f, 1.f, 0.f);
			break;
		case 3:
			GraphicPrimitives::drawFillRect2D(-0.49f, -0.39f, 0.48f, 0.18f, 0.f, 1.f, 0.f);
			break;
		default:
			break;
		}

		GraphicPrimitives::drawText2D("J1", -0.27f, 0.49f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("HUMAIN", -0.36f, 0.29f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("RANDOM", -0.37f, 0.09f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("MINI-MAX", -0.39f, -0.12f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("ALPHA-BETA", -0.44f, -0.32f, 1.f, 1.f, 1.f);

		// J2
		GraphicPrimitives::drawFillRect2D(0.f, 0.4f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.01f, 0.41f, 0.48f, 0.18f, 0.f, 0.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.f, 0.2f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.01f, 0.21f, 0.48f, 0.18f, 0.f, 0.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.f, 0.0f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.01f, 0.01f, 0.48f, 0.18f, 0.f, 0.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.f, -0.2f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.01f, -0.19f, 0.48f, 0.18f, 0.f, 0.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.f, -0.4f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(0.01f, -0.39f, 0.48f, 0.18f, 0.f, 0.f, 0.f);

		switch ((*gameManagement_).getTypeJ2())
		{
		case 0:
			GraphicPrimitives::drawFillRect2D(0.01f, 0.21f, 0.48f, 0.18f, 0.0f, 1.0f, 0.0f);
			break;
		case 1:
			GraphicPrimitives::drawFillRect2D(0.01f, 0.01f, 0.48f, 0.18f, 0.f, 1.f, 0.f);
			break;
		case 2:
			GraphicPrimitives::drawFillRect2D(0.01f, -0.19f, 0.48f, 0.18f, 0.f, 1.f, 0.f);
			break;
		case 3:
			GraphicPrimitives::drawFillRect2D(0.01f, -0.39f, 0.48f, 0.18f, 0.f, 1.f, 0.f);
			break;
		default:
			break;
		}

		GraphicPrimitives::drawText2D("J2", 0.23f, 0.49f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("HUMAIN", 0.14f, 0.29f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("RANDOM", 0.13f, 0.09f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("MINI-MAX", 0.11f, -0.12f, 1.f, 1.f, 1.f);
		GraphicPrimitives::drawText2D("ALPHA-BETA", 0.06f, -0.32f, 1.f, 1.f, 1.f);

		// Jouer
		GraphicPrimitives::drawFillRect2D(-0.25f, -0.8f, 0.5f, 0.2f, 0.f, 1.f, 0.f);
		GraphicPrimitives::drawFillRect2D(-0.24f, -0.79f, 0.48f, 0.18f, 0.f, 0.f, 0.f);

		GraphicPrimitives::drawText2D("JOUER", -0.1f, -0.72f, 1.f, 1.f, 1.f);

		break;

	case END_GAME:
		for (unsigned int rows = 0; rows < (*gameManagement_).getN(); rows++){
			for (unsigned int columns = 0; columns < (*gameManagement_).getN(); columns++){
				(*square_)[rows][columns].draw();

				utility::itoa(rows + 1, cRows);
				utility::itoa(columns + 1, cColumns);

				GraphicPrimitives::drawText2D(
					cRows,
					(*square_)[rows][columns].getPosX() + (*square_)[rows][columns].getSide() / 7,
					(*square_)[rows][columns].getPosY() + (*square_)[rows][columns].getSide() / 2,
					0.3f,
					0.3f,
					0.3f);

				GraphicPrimitives::drawText2D(
					cColumns,
					(*square_)[rows][columns].getPosX() + (*square_)[rows][columns].getSide() / 2,
					(*square_)[rows][columns].getPosY() + (*square_)[rows][columns].getSide() / 2,
					0.3f,
					0.3f,
					0.3f);

				GraphicPrimitives::drawText2D(
					"CLICK TO PLAY AGAIN",
					-0.33f,
					-0.98f,
					0.f,
					0.f,
					1.f);
			}
		}
		break;

	default:

		for (unsigned int rows = 0; rows < (*gameManagement_).getN(); rows++){
			for (unsigned int columns = 0; columns < (*gameManagement_).getN(); columns++){
				(*square_)[rows][columns].draw();

				utility::itoa(rows + 1, cRows);
				utility::itoa(columns + 1, cColumns);

				GraphicPrimitives::drawText2D(
					cRows,
					(*square_)[rows][columns].getPosX() + (*square_)[rows][columns].getSide() / 7,
					(*square_)[rows][columns].getPosY() + (*square_)[rows][columns].getSide() / 2,
					0.3f,
					0.3f,
					0.3f);

				GraphicPrimitives::drawText2D(
					cColumns,
					(*square_)[rows][columns].getPosX() + (*square_)[rows][columns].getSide() / 2,
					(*square_)[rows][columns].getPosY() + (*square_)[rows][columns].getSide() / 2,
					0.3f,
					0.3f,
					0.3f);
			}
		}
		break;
	}
	delete[] cRows;
	delete[] cColumns;
}