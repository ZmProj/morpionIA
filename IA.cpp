#include "IA.h"

void IA::toPlay(){
	switch (getIaType())
	{
	case 1:
		iaRandom();
		break;
	case 2:
		iaMinMax();
		break;
	case 3:
		iaAlphaBeta();
		break;
	default:
		break;
	}

}

int IA::getIaType(){
	N_ = (*gameManagement_).getN();
	nbAlignToWin_ = (*gameManagement_).getNbAlignToWin();
	if ((*gameManagement_).getCurrentPlayer() == 0){
		return (*gameManagement_).getTypeJ1();
	}
	else return (*gameManagement_).getTypeJ2();
}

void IA::iaRandom(){
	
	/* initialize random seed: */
	srand(time(NULL));

	std::vector<std::pair<int, int>> casesLibres = coupJouables(*square_);

	/* generate secret number between 0 and casesLibres size: */
	int aleaNumber = rand() % casesLibres.size();

	appliqueCoup(*square_, casesLibres[aleaNumber]);
	appliqueCouleur(*square_, casesLibres[aleaNumber]);
	lastCoupJoueur = casesLibres[aleaNumber];


	int type = (*square_)[casesLibres[aleaNumber].first][casesLibres[aleaNumber].second].getClickedBy();
	(*etat_) = Etat::END_TURN;

	if ((*gameManagement_).verifVainqueur(casesLibres[aleaNumber].first, casesLibres[aleaNumber].second, *square_) == type){
		(*etat_) = Etat::END_GAME;
	}
}

void IA::iaMinMax(){
	valeurMinMax(*square_, true, 0, 3, lastCoupJoueur);
	appliqueCoup(*square_, coupJoue);
	appliqueCouleur(*square_, coupJoue);
	lastCoupJoueur = coupJoue;

	int type = (*square_)[coupJoue.first][coupJoue.second].getClickedBy();
	(*etat_) = Etat::END_TURN;

	if ((*gameManagement_).verifVainqueur(coupJoue.first, coupJoue.second, *square_) == type){
		std::cout << "IA WIN !" << std::endl;
		(*etat_) = Etat::END_GAME;
	}
	else if ((*gameManagement_).verifVainqueur(coupJoue.first, coupJoue.second, *square_) == ((type + 1) % 2)){
		std::cout << "pas encore gagne" << std::endl;
	}
}

void IA::iaAlphaBeta(){
	calcIA(*square_, true, 0, 3);

	int type = (*square_)[coupJoue.first][coupJoue.second].getClickedBy();
	(*etat_) = Etat::END_TURN;

	if ((*gameManagement_).verifVainqueur(coupJoue.first, coupJoue.second, *square_) == type){
		(*etat_) = Etat::END_GAME;
	}
}

// Return un vector de pairs correspondant aux cases jouables (libres)
std::vector<std::pair<int, int>> IA::coupJouables(std::vector<std::vector <Square > > &square) const{
	typedef std::pair<int, int> intPair;
	std::vector<std::pair<int, int>> coupJouables;
	for (int i = 0; i < N_; i++){
		for (int j = 0; j < N_; j++){
			if (square[j][i].getClickedBy() == -1){
				coupJouables.push_back(intPair(j, i));
			}
		}
	}
	return coupJouables;
}

/*std::vector<std::pair<int, int>> IA::coupJouables(std::vector<std::vector <Square > > &square) const{
	typedef std::pair<int, int> intPair;
	std::vector<std::pair<int, int>> coupJouables;
	int sizeSurDeux = N_ / 2;
	for (int i = 0; i < N_; i++){
		for (int j = 0; j < sizeSurDeux; j++){
			if (square[j][i].getClickedBy() == -1){
				coupJouables.push_back(intPair(j, i));
			}
		}
		for (int j = sizeSurDeux; j < N_; j++){
			if (square[j][i].getClickedBy() == -1){
				coupJouables.push_back(intPair(j, i));
			}
		}
	}
	return coupJouables;
}*/

void IA::appliqueCoup(std::vector<std::vector <Square > > &square, std::pair<int, int> coup){
	square[coup.first][coup.second].setClickedBy((*gameManagement_).getCurrentPlayer());
}

void IA::appliqueCouleur(std::vector<std::vector <Square > > &square, std::pair<int, int> coup){
	if ((*gameManagement_).getCurrentPlayer() == 0){
		square[coup.first][coup.second].changeColorJ1();
	}
	else {
		square[coup.first][coup.second].changeColorJ2();
	}
}

int IA::valeurMinMax(std::vector<std::vector <Square > > square, bool ordi_joue, int prof, int profMax, std::pair<int, int> lastCoupJ){
	std::vector<std::pair<int, int>> coupJouable = coupJouables(square);
	std::vector<int> tab_valeurs;
	std::vector<std::pair<int, int>> coupAJoue;
	std::vector<std::vector <Square > > squareNext;
	if (coupJouable.size() == 0 || (*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) != -1){
		if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == (*gameManagement_).getCurrentPlayer()){
			return 1000 - comptePions(square);
		}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == ((*gameManagement_).getCurrentPlayer() + 1) % 2){
			return -1000 + comptePions(square);
		}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == -1){
			return 0;
		}
	}
	if (prof == profMax) {
		if (ordi_joue){
			return analyse(square,ordi_joue);
		}
		else return -analyse(square,ordi_joue);
	}

	for (int i = 0; i < coupJouable.size(); i++){
		jouerCoup(squareNext, square, ordi_joue, coupJouable[i]);
		tab_valeurs.emplace_back(valeurMinMax(squareNext, !ordi_joue, (prof + 1), profMax, coupJouable[i]));
	}

	int res;
	if (ordi_joue){
		res = *max_element(tab_valeurs.begin(), tab_valeurs.end()); //max de tab_valeurs
	}
	else {
		res = *min_element(tab_valeurs.begin(), tab_valeurs.end()); //min de tab_valeurs
	}
	if (prof == 0){
		for (int i = 0; i < tab_valeurs.size(); i++){
			if (tab_valeurs[i] == res){
				coupJoue = coupJouable[i];
			}
		}
	}
	return res;
}


void IA::jouerCoup(std::vector<std::vector <Square> > &pos_next, std::vector<std::vector <Square> > &pos_courante, bool ordi_joue, std::pair<int, int> coup){
	pos_next = pos_courante;
	if (ordi_joue){
		pos_next[coup.first][coup.second].setClickedBy((*gameManagement_).getCurrentPlayer());
	}
	else pos_next[coup.first][coup.second].setClickedBy(((*gameManagement_).getCurrentPlayer() + 1) % 2);
}

int IA::comptePions(std::vector<std::vector <Square> > &jeu){
	int cnt = 0;
	for (int i = 0; i < N_; i++){
		for (int j = 0; j < N_; j++){
			if (jeu[i][j].getClickedBy() != -1){
				cnt++;
			}
		}
	}
	return cnt;
}

int IA::analyse(std::vector<std::vector <Square> > &square, bool ordi_joue){

	int couleur;
	int couleurAdversaire;
	if (ordi_joue){
		couleur = (*gameManagement_).getCurrentPlayer();
		couleurAdversaire = (couleur + 1) % 2;
	}
	else{
		couleurAdversaire = (*gameManagement_).getCurrentPlayer();
		couleur = (couleurAdversaire + 1) % 2;
	}


	int serieJ1 = 0;
	int serieJ2 = 0;

	for (int i = 0; i < N_; i++){
		for (int j = 0; j < N_; j++){
			serieJ1 += lignePeutGagner(square, couleur, couleurAdversaire, i, j) + colonnePeutGagner(square, couleur, couleurAdversaire, i, j)
				+ diagoBasHautPeutGagner(square, couleur, couleurAdversaire, i, j) + diagoHautBasPeutGagner(square, couleur, couleurAdversaire, i, j);
			serieJ2 += lignePeutGagner(square, couleurAdversaire, couleur, i, j) + colonnePeutGagner(square, couleurAdversaire, couleur, i, j)
				+ diagoBasHautPeutGagner(square, couleurAdversaire, couleur, i, j) + diagoHautBasPeutGagner(square, couleurAdversaire, couleur, i, j);
		}
	}

	return serieJ1 - serieJ2;
}

int IA::lignePeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns){
	int compteur = 0;
	int dist = 0;
	int adj = 0;
	bool isAdj = true;

	// ligne gauche droite
	for (int i = columns; dist < nbAlignToWin_ && i < N_; i++){
		if (square[rows][i].getClickedBy() == couleur && isAdj){
			adj++;
		}
		else{
			isAdj = false;
		}
		if (square[rows][i].getClickedBy() == -1 || square[rows][i].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}

	// ligne droite gauche
	compteur--;
	adj--;
	isAdj = true;
	dist = 0;
	for (int i = columns; dist < nbAlignToWin_ && i >= 0; i--){
		if (square[rows][i].getClickedBy() == couleur && isAdj){
			adj++;
		}
		else{
			isAdj = false;
		}
		if (square[rows][i].getClickedBy() == -1 || square[rows][i].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else {
			break;
		}
	}

	return max(0, (adj * adj)*(compteur - nbAlignToWin_ + 1));
}

int IA::colonnePeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns){
	int compteur = 0;
	int dist = 0;
	int adj = 0;
	bool isAdj = true;

	// ligne verticale haut bas 
	for (int i = rows; dist < nbAlignToWin_ && i < N_; i++){
		if (square[i][columns].getClickedBy() == couleur && isAdj){
			adj++;
		}
		else{
			isAdj = false;
		}
		if (square[i][columns].getClickedBy() == -1 || square[i][columns].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}

	// ligne verticale bas haut
	compteur--;
	adj--;
	isAdj = true;
	dist = 0;
	for (int i = rows; dist < nbAlignToWin_ && i >= 0; i--){
		if (square[i][columns].getClickedBy() == couleur && isAdj){
			adj++;
		}
		else{
			isAdj = false;
		}
		if (square[i][columns].getClickedBy() == -1 || square[i][columns].getClickedBy() == couleur){
			if (square[i][columns].getClickedBy() == couleur && isAdj){
				adj++;
			}
			else{
				isAdj = false;
			}
			compteur++;
			dist++;
		}
		else {
			break;
		}
	}

	return max(0, (adj * adj)*(compteur - nbAlignToWin_ + 1));
}

int IA::diagoBasHautPeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns){
	int compteur = 0;
	int dist = 0;
	int adj = 0;
	bool isAdj = true;

	// diago de bas en haut
	for (int i = rows, j = columns; dist < nbAlignToWin_ && i >= 0 && j < N_; i--, j++){
		if (square[i][j].getClickedBy() == couleur && isAdj){
			adj++;
		}
		else{
			isAdj = false;
		}
		if (square[i][j].getClickedBy() == -1 || square[i][j].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}

	// diago de haut en bas
	adj--;
	isAdj = true;
	compteur--;
	dist = 0;
	for (int i = rows, j = columns; dist < nbAlignToWin_ && i < N_ && j >= 0; i++, j--){
		if (square[i][j].getClickedBy() == couleur && isAdj){
			adj++;
		}
		else{
			isAdj = false;
		}
		if (square[i][j].getClickedBy() == -1 || square[i][j].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}

	return max(0, (adj * adj)*compteur - nbAlignToWin_ + 1);
}

int IA::diagoHautBasPeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns){
	int compteur = 0;
	int dist = 0;
	int adj = 0;
	bool isAdj = true;

	// diago de bas en haut
	for (int i = rows, j = columns; dist < nbAlignToWin_ && i >= 0 && j >= 0; i--, j--){
		if (square[i][j].getClickedBy() == couleur && isAdj){
			adj++;
		}
		else{
			isAdj = false;
		}
		if (square[i][j].getClickedBy() == -1 || square[i][j].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}


	// diago de haut en bas
	adj--;
	isAdj = true;
	compteur--;
	dist = 0;
	for (int i = rows, j = columns; dist < nbAlignToWin_ && i < N_ && j < N_; i++, j++){
		if (square[i][j].getClickedBy() == couleur && isAdj){
			adj++;
		}
		else{
			isAdj = false;
		}
		if (square[i][j].getClickedBy() == -1 || square[i][j].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}

	return max(0, (adj * adj)*compteur - nbAlignToWin_ + 1);
}



void IA::setLastCoupJoueur(int x, int y){
	lastCoupJoueur.first = x;
	lastCoupJoueur.second = y;
}

void IA::calcIA(std::vector<std::vector <Square > > &square, bool ordi_joue, int prof, int profMax){
	std::vector<std::pair<int, int>> coupJouable = coupJouables(square);
	std::vector<int> tab_valeurs;
	std::vector<std::pair<int, int>> coupAJoue;
	std::vector<std::vector <Square > > squareNext;
	int alpha = -100000;
	int beta = 100000;
	int tmp;
	if (prof != profMax || (coupJouable.size() != 0 && (*gameManagement_).verifVainqueurForIA(coupJoue.first, coupJoue.second, square) == -1)){
		for (int i = 0; i < coupJouable.size(); i++){
			jouerCoup(squareNext, square, ordi_joue, coupJouable[i]);
			tmp = calcMin(squareNext, !ordi_joue, prof + 1, profMax, coupJouable[i], alpha, beta);
			if (alpha < tmp){
				alpha = tmp;
				coupJoue = coupJouable[i];
			}
		}
	}
	appliqueCoup(*square_, coupJoue);
	appliqueCouleur(*square_, coupJoue);
	lastCoupJoueur = coupJoue;
}

int IA::calcMin(std::vector<std::vector <Square > > &square, bool ordi_joue, int prof, int profMax, std::pair<int, int> lastCoupJ, int alpha, int beta){
	std::vector<std::pair<int, int>> coupJouable = coupJouables(square);
	int tmp;
	std::vector<std::vector <Square > > squareNext;
	if (coupJouable.size() == 0 || (*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) != -1){
		if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == ((*gameManagement_).getCurrentPlayer() + 1) % 2){
			return -1000 + comptePions(square);
	}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == (*gameManagement_).getCurrentPlayer()){
			return 1000 - comptePions(square);
	}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == -1){
			return 0;
		}
	}
	if (prof == profMax){
		tmp = -analyse(square, ordi_joue);
		return tmp;
	}
	for (int i = 0; i < coupJouable.size(); i++){
		jouerCoup(squareNext, square, ordi_joue, coupJouable[i]);
		tmp = calcMax(squareNext, !ordi_joue, prof + 1, profMax, coupJouable[i], alpha, beta);

			if (beta > tmp){
				beta = tmp;
			}

			if (beta <= alpha){
				return beta;
			}
		}
		return beta;
	}

int IA::calcMax(std::vector<std::vector <Square > > &square, bool ordi_joue, int prof, int profMax, std::pair<int, int> lastCoupJ, int alpha, int beta){
	std::vector<std::pair<int, int>> coupJouable = coupJouables(square);
	int tmp;
	std::vector<std::vector <Square > > squareNext;
	if (coupJouable.size() == 0 || (*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) != -1){
		if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == (*gameManagement_).getCurrentPlayer()){
			return 1000 - comptePions(square);
		}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == ((*gameManagement_).getCurrentPlayer() + 1) % 2){
			return -1000 + comptePions(square);
		}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == -1){
			return 0;
		}
	}
	if (prof == profMax){
			tmp = analyse(square, ordi_joue);
			return tmp;
	}
	for (int i = 0; i < coupJouable.size(); i++){
		jouerCoup(squareNext, square, ordi_joue, coupJouable[i]);
		tmp = calcMin(squareNext, !ordi_joue, prof + 1, profMax, coupJouable[i], alpha, beta);

		if (alpha < tmp){
			alpha = tmp;
		}

		if (beta <= alpha){
			return alpha;
		}
	}
	return alpha;
}

