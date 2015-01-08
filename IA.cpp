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
		std::cout << "IA WIN !" << std::endl;
		(*etat_) = Etat::END_GAME;
	}
	else if ((*gameManagement_).verifVainqueur(casesLibres[aleaNumber].first, casesLibres[aleaNumber].second, *square_) == ((type + 1) % 2)){
		std::cout << "pas encore gagne" << std::endl;
	}

}

void IA::iaMinMax(){

	valeurMinMax(*square_, true, 0, 2, lastCoupJoueur);
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
	//valeurAlphaBeta(*square_, true, 0, 5, lastCoupJoueur,-INFINITY,INFINITY);
	calcIA(*square_, true, 0, 5); // /!\ Ne marche que pour les pronfondeurs impairs !

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

// Return un vector de pairs correspondant aux cases jouables (libres)
std::vector<std::pair<int, int>> IA::coupJouables(std::vector<std::vector <Square > > &square) const{
	typedef std::pair<int, int> intPair;
	std::vector<std::pair<int, int>> coupJouables;
	for (int i = 0; i < square.size(); i++){
		for (int j = 0; j < square.size(); j++){
			if (square[j][i].getClickedBy() == -1){
				coupJouables.push_back(intPair(j, i));
			}
		}
	}
	return coupJouables;
}

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
		if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == 1){
			return 1000 - comptePions(square);
		}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == 0){
			return -1000 + comptePions(square);
		}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == -1){
			return 0;
		}
	}
	if (prof == profMax) {
		return estimation(square);
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
		pos_next[coup.first][coup.second].setClickedBy(1);
	}
	else pos_next[coup.first][coup.second].setClickedBy(0);
}

int IA::comptePions(std::vector<std::vector <Square> > &jeu){
	int cnt = 0;
	for (int i = 0; i < (*gameManagement_).getN(); i++){
		for (int j = 0; j < (*gameManagement_).getN(); j++){
			if (jeu[i][j].getClickedBy() != -1){
				cnt++;
			}
		}
	}
	return cnt;
}

int IA::estimation(std::vector<std::vector <Square> > &square){
	int estimation = 0; //estimation globale de la position

	for (int i = 0; i<(*gameManagement_).getN(); i++){
		for (int j = 0; j<(*gameManagement_).getN(); j++){
			if (square[i][j].getClickedBy() == -1) continue;
			//estimation de la valeur de ce jeton et ajout au calcul d'estimation global
			switch (square[i][j].getClickedBy()){
			case 0:
				estimation -= analyse(square, i, j);
				break;
			case 1:
				estimation += analyse(square, i, j);
				break;
			}
		}
	}
	return estimation;
}


int IA::analyse(std::vector<std::vector <Square> > &square, int x, int y){
	const int couleur = square[x][y].getClickedBy();
	int estimation = 0; //estimation pour toutes les directions
	int compteur = 0; //compte le nombre de possibilités pour une direction
	int centre = 0; //regarde si le jeton a de l'espace de chaque côté
	int bonus = 0; //point bonus liée aux jetons alliés dans cette même direction
	int i, j; //pour les coordonnées temporaires
	bool pass = false; //permet de voir si on a passé la case étudiée
	int pLiberte = 1; //pondération sur le nombre de liberté
	int pBonus = 1; //pondération Bonus
	int pCentre = 2; //pondération pour l'espace situé de chaque côté
	int nmax = (*gameManagement_).getN();

	//recherche horizontale
	for (i = 0; i<nmax; i++){
		if (i == x){
			centre = compteur++;
			pass = true;
			continue;
		}
		if (square[i][y].getClickedBy() == -1){
			compteur++;
		}
		else if (square[i][y].getClickedBy() == couleur){
			compteur++;
			bonus++;
		}
		else{
			if (pass){
				i = nmax; //il n'y aura plus de liberté supplémentaire, on arrête la recherche ici
			}
			else{
				//on réinitialise la recherche
				compteur = 0;
				bonus = 0;
			}
		}
	}
	if (compteur >= (*gameManagement_).getNbAlignToWin()){
		//il est possible de gagner dans cette direction
		estimation += compteur*pLiberte + bonus*pBonus + (1 - std::abs(centre / (compteur - 1) - 0.5))*compteur*pCentre;
	}

	//recherche verticale
	compteur = 0;
	bonus = 0;
	pass = false;
	for (j = 0; j<nmax; j++){
		if (j == y){
			centre = compteur++;
			pass = true;
			continue;
		}
		if (square[x][j].getClickedBy() == -1){
			compteur++;
		}
		else if (square[x][j].getClickedBy() == couleur){
			compteur++;
			bonus++;
		}
		else{
			if (pass){
				j = nmax; //il n'y aura plus de liberté supplémentaire, on arrête la recherche ici
			}
			else{
				//on réinitialise la recherche
				compteur = 0;
				bonus = 0;
			}
		}
	}
	if (compteur >= (*gameManagement_).getNbAlignToWin()){
		//il est possible de gagner dans cette direction
		estimation += compteur*pLiberte + bonus*pBonus + (1 - std::abs(centre / (compteur - 1) - 0.5))*compteur*pCentre;
	}

	//recherche diagonale (NO-SE)
	compteur = 0;
	bonus = 0;
	i = x;
	j = y;
	while (i-->0 && j-->0){
		if (square[i][j].getClickedBy() == -1){
			compteur++;
		}
		else if (square[i][j].getClickedBy() == couleur){
			compteur++;
			bonus++;
		}
		else{
			i = 0;
		}
	}
	centre = compteur++;
	i = x;
	j = y;
	while (++i<nmax && ++j<nmax){
		if (square[i][j].getClickedBy() == -1){
			compteur++;
		}
		else if (square[i][j].getClickedBy() == couleur){
			compteur++;
			bonus++;
		}
		else{
			i = nmax;
		}
	}
	if (compteur >= (*gameManagement_).getNbAlignToWin()){
		//il est possible de gagner dans cette direction
		estimation += compteur*pLiberte + bonus*pBonus + (1 - std::abs(centre / (compteur - 1) - 0.5))*compteur*pCentre;
	}

	//recherche diagonale (NE-SO)
	compteur = 0;
	bonus = 0;
	i = x;
	j = y;
	while (i-->0 && ++j<nmax){
		if (square[i][j].getClickedBy() == -1){
			compteur++;
		}
		else if (square[i][j].getClickedBy() == couleur){
			compteur++;
			bonus++;
		}
		else{
			i = 0;
		}
	}
	centre = compteur++;
	i = x;
	j = y;
	while (++i<nmax && j-->0){
		if (square[i][j].getClickedBy() == -1){
			compteur++;
		}
		else if (square[i][j].getClickedBy() == couleur){
			compteur++;
			bonus++;
		}
		else{
			i = nmax;
		}
	}
	if (compteur >= (*gameManagement_).getNbAlignToWin()){
		//il est possible de gagner dans cette direction
		estimation += compteur*pLiberte + bonus*pBonus + (1 - std::abs(centre / (compteur - 1) - 0.5))*compteur*pCentre;
	}

	return estimation;
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
	if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == 0){
		return -1000 + comptePions(square);
	}
	else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == 1){
		return 1000 - comptePions(square);
	}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == -1){
			return 0;
		}
	}
	if (prof == profMax){
		tmp = estimation(square);
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
		if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == 1){
			return 1000 - comptePions(square);
		}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == 0){
			return -1000 + comptePions(square);
		}
		else if ((*gameManagement_).verifVainqueurForIA(lastCoupJ.first, lastCoupJ.second, square) == -1){
			return 0;
		}
	}
	if (prof == profMax){
		tmp = estimation(square);
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

