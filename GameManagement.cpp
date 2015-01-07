#include <algorithm>
#include "GameManagement.h"


int GameManagement::verifVainqueur(int x, int y, std::vector<std::vector <Square > > &square){
	int col = square[x][y].getClickedBy();
	int alignH = 1;
	int alignV = 1;
	int alignDiag1 = 1;
	int alignDiag2 = 1;
	int xt = 0;
	int yt = 0;


	//vérification verticale
	xt = x - 1;
	yt = y;
	while (xt >= 0 && square[xt][yt].getClickedBy() == col){
		xt--;
		alignV++;
	}
	xt = x + 1;
	while (xt<getN() && square[xt][yt].getClickedBy() == col){
		xt++;
		alignV++;
	}

	//vérification horizontale
	xt = x;
	yt = y - 1;
	while (yt >= 0 && square[xt][yt].getClickedBy() == col){
		yt--;
		alignH++;
	}
	yt = y + 1;
	while (yt<getN() && square[xt][yt].getClickedBy() == col){
		yt++;
		alignH++;
	}

	//vérification diagonale NO-SE
	xt = x - 1;
	yt = y - 1;
	while (xt >= 0 && yt >= 0 && square[xt][yt].getClickedBy() == col){
		xt--;
		yt--;
		alignDiag1++;
	}
	xt = x + 1;
	yt = y + 1;
	while (xt<getN() && yt<N_ && square[xt][yt].getClickedBy() == col){
		xt++;
		yt++;
		alignDiag1++;
	}

	//vérification diagonale SO-NE
	xt = x - 1;
	yt = y + 1;
	while (xt >= 0 && yt<N_ && square[xt][yt].getClickedBy() == col){
		xt--;
		yt++;
		alignDiag2++;
	}
	xt = x + 1;
	yt = y - 1;
	while (xt<N_ && yt >= 0 && square[xt][yt].getClickedBy() == col){
		xt++;
		yt--;
		alignDiag2++;
	}

	//parmis tous ces résultats on regarde s'il y en a un qui dépasse le nombre nécessaire pour gagner
	if ((std::max)((std::max)(alignH, alignV), (std::max)(alignDiag1, alignDiag2)) >= getNbAlignToWin()){
		return col; // le gagnant
	}
	else if(nbCoupsPossibles() == 0){ // match nul
		return -1;
	}
	else { // la partie continue 
		return -2;
	}
}

int GameManagement::verifVainqueurForIA(int x, int y, std::vector<std::vector <Square > > &square){
	int col = square[x][y].getClickedBy();
	int alignH = 1;
	int alignV = 1;
	int alignDiag1 = 1;
	int alignDiag2 = 1;
	int xt = 0;
	int yt = 0;


	//vérification verticale
	xt = x - 1;
	yt = y;
	while (xt >= 0 && square[xt][yt].getClickedBy() == col){
		xt--;
		alignV++;
	}
	xt = x + 1;
	while (xt<getN() && square[xt][yt].getClickedBy() == col){
		xt++;
		alignV++;
	}

	//vérification horizontale
	xt = x;
	yt = y - 1;
	while (yt >= 0 && square[xt][yt].getClickedBy() == col){
		yt--;
		alignH++;
	}
	yt = y + 1;
	while (yt<getN() && square[xt][yt].getClickedBy() == col){
		yt++;
		alignH++;
	}

	//vérification diagonale NO-SE
	xt = x - 1;
	yt = y - 1;
	while (xt >= 0 && yt >= 0 && square[xt][yt].getClickedBy() == col){
		xt--;
		yt--;
		alignDiag1++;
	}
	xt = x + 1;
	yt = y + 1;
	while (xt<getN() && yt<N_ && square[xt][yt].getClickedBy() == col){
		xt++;
		yt++;
		alignDiag1++;
	}

	//vérification diagonale SO-NE
	xt = x - 1;
	yt = y + 1;
	while (xt >= 0 && yt<N_ && square[xt][yt].getClickedBy() == col){
		xt--;
		yt++;
		alignDiag2++;
	}
	xt = x + 1;
	yt = y - 1;
	while (xt<N_ && yt >= 0 && square[xt][yt].getClickedBy() == col){
		xt++;
		yt--;
		alignDiag2++;
	}

	//parmis tous ces résultats on regarde s'il y en a un qui dépasse le nombre nécessaire pour gagner
	if ((std::max)((std::max)(alignH, alignV), (std::max)(alignDiag1, alignDiag2)) >= getNbAlignToWin()){
		return col; // le gagnant
	}
	else { // la partie continue 
		return -1;
	}
}

void GameManagement::setN(int n){
	N_ = n;
}

int GameManagement::getN() const{
	return N_;
}

void GameManagement::setTypeJ1(int j1){
	typeJ1_ = j1;
}

int GameManagement::getTypeJ1() const{
	return typeJ1_;
}

void GameManagement::setTypeJ2(int j2){
	typeJ2_ = j2;
}

int GameManagement::getTypeJ2() const{
	return typeJ2_;
}

float GameManagement::getSquareSide() const{
	return 2.f / N_;
}

void GameManagement::drawSquare(){
	for (unsigned int rows = 0; rows < N_; rows++) {
		(*square_).emplace_back();
		for (unsigned int columns = 0; columns < N_; columns++){
			(*square_)[rows].emplace_back(-1 + columns*getSquareSide(), 1.f - getSquareSide() - rows*getSquareSide(), getSquareSide(), 0.8f, 0.8f, 0.8f);
		}
	}
}

std::vector<std::vector <Square> > GameManagement::getSquare(){
	return *square_;
}

int GameManagement::getNbAlignToWin(){
	switch (N_)
	{
	case 3:
		return 3;
	case 5:
		return 4;
	case 10:
		return 5;
	default:
		return 3;
	}
}

bool GameManagement::resteCaseLibre(){
	for (int i = 0; i < square_->size(); i++){
		for (int j = 0; j < square_->size(); j++){
			if ((*square_)[j][i].getClickedBy() == -1) return true;
		}
	}
	return false;
}

int GameManagement::getCurrentPlayer() const{
	return currentPlayer_;
}

void GameManagement::endPlayerTurn(){
	currentPlayer_ = (currentPlayer_ + 1) % 2;
}

void GameManagement::resetGame(){
	N_ = 3;
	typeJ1_ = 0;
	typeJ2_ = 0;
	currentPlayer_ = 0;
	(*square_).clear();
}

int GameManagement::nbCoupsPossibles() const{
	int nbCoupsPossibles = 0;
	for (int i = 0; i < square_->size(); i++){
		for (int j = 0; j < square_->size(); j++){
			if ((*square_)[j][i].getClickedBy() == 0) nbCoupsPossibles++;
		}
	}
	return nbCoupsPossibles;
}