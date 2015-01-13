#pragma once
#include "Square.h"
#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "GameManagement.h"
#include "Etat.h"
#include <algorithm>

class IA
{
private:
	std::vector<std::vector <Square > > *square_;
	GameManagement *gameManagement_;
	Etat *etat_;
	std::pair<int, int> coupJoue;
	std::pair<int, int> lastCoupJoueur;
	int N_;
	int nbAlignToWin_;


public:
	IA(std::vector<std::vector <Square> > *square, GameManagement *gameManagement, Etat *etat) :
		square_(square),
		gameManagement_(gameManagement),
		etat_(etat)
	{
		lastCoupJoueur.first = 0;
		lastCoupJoueur.second = 0;
		coupJoue.first = -1;
		coupJoue.second = -1;
	}

	int getIaType();
	void toPlay();
	void iaRandom();
	void iaMinMax();
	void iaAlphaBeta();
	std::vector<std::pair<int, int>> coupJouables(std::vector<std::vector <Square > > &) const;
	std::pair<int, int> decisionMinMax(std::vector<std::vector <Square > >);
	void appliqueCoup(std::vector<std::vector <Square > > &, std::pair<int, int>);
	int valeurMinMax(std::vector<std::vector <Square > > square, bool ordi_joue, int prof, int profMax, std::pair<int, int>);
	void jouerCoup(std::vector<std::vector <Square> > &pos_next, std::vector<std::vector <Square> > &pos_courante, bool, std::pair<int, int> coup);
	int comptePions(std::vector<std::vector <Square> > &jeu);
	int estimation(std::vector<std::vector <Square> > &square);
	int analyse(std::vector<std::vector <Square> > &square, bool);
	void setLastCoupJoueur(int x, int y);
	void appliqueCouleur(std::vector<std::vector <Square > > &square, std::pair<int, int> coup);
	void calcIA(std::vector<std::vector <Square > > &square, bool ordi_joue, int prof, int profMax);
	int valeurAlphaBeta(std::vector<std::vector <Square > > square, bool ordi_joue, int prof, int profMax, std::pair<int, int> lastCoupJ, int alpha, int beta);
	int calcMin(std::vector<std::vector <Square > > &square, bool ordi_joue, int prof, int profMax, std::pair<int, int> lastCoupJ, int alpha, int beta);
	int calcMax(std::vector<std::vector <Square > > &square, bool ordi_joue, int prof, int profMax, std::pair<int, int> lastCoupJ, int alpha, int beta);
	int lignePeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns);
	int colonnePeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns);
	int diagoBasHautPeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns);
	int diagoHautBasPeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns);
};
