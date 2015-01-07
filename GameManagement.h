#pragma once
#include "Square.h"

class GameManagement
{
private:
	unsigned int N_;          // Nombre de cases du morpion (N x N)
	unsigned int typeJ1_;     // 0 : humain    1 : random    2 : minimax    3 : alphaBeta
	unsigned int typeJ2_;     // 0 : humain    1 : random    2 : minimax    3 : alphaBeta
	std::vector<std::vector <Square> > *square_; // Contient les cases du plateau de jeu
	int currentPlayer_;		  // 0 : J1		1 : J2

public:
	GameManagement(std::vector<std::vector <Square> > *square) :
		square_(square),
		N_(3),
		typeJ1_(0),
		typeJ2_(0),
		currentPlayer_(0)
	{};

	int getN() const;
	int getTypeJ1() const;
	int getTypeJ2() const;
	void setN(int n);
	void setTypeJ1(int j1);
	void setTypeJ2(int j2);
	void drawSquare();
	int verifVainqueur(int x, int y, std::vector<std::vector <Square > > &);
	int verifVainqueurForIA(int x, int y, std::vector<std::vector <Square > > &);
	std::vector<std::vector <Square> > getSquare();
	float getSquareSide() const;
	int getNbAlignToWin();
	bool resteCaseLibre();
	int getCurrentPlayer() const;	// 0 : J1		1 : J2
	void endPlayerTurn();
	void resetGame();
	int nbCoupsPossibles() const;

};