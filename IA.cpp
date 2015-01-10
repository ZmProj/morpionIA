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

	std::cout << "value min max : " << valeurMinMax(*square_, true, 0, 3, lastCoupJoueur);
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
	calcIA(*square_, true, 0, 3); // /!\ Ne marche que pour les pronfondeurs impairs !

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
		pos_next[coup.first][coup.second].setClickedBy((*gameManagement_).getCurrentPlayer());
	}
	else pos_next[coup.first][coup.second].setClickedBy(((*gameManagement_).getCurrentPlayer() + 1) % 2);
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
	/*const int couleur = square[x][y].getClickedBy();
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

	const int couleurAdversaire = (couleur + 1) % 2; 
	int malus = 0; // pts lié aux jetons adverses ds cette direction 
	const int taillePlateau = (*gameManagement_).getN();
	const int nbAlignToWin = (*gameManagement_).getNbAlignToWin();

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
		else if (square[i][y].getClickedBy() == couleurAdversaire){
			malus++;
		}
		else{
			if (pass){
				i = nmax; //il n'y aura plus de liberté supplémentaire, on arrête la recherche ici
			}
			else{
				//on réinitialise la recherche
				compteur = 0;
				bonus = 0;
				malus = 0;
			}
		}
	}
	if (compteur >= (*gameManagement_).getNbAlignToWin()){
		//il est possible de gagner dans cette direction
		estimation += compteur*pLiberte + bonus*pBonus + (1 - std::abs(centre / (compteur - 1) - 0.5))*compteur*pCentre + malus * 100;
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
		else if (square[x][j].getClickedBy() == couleurAdversaire){
			malus++;
		}
		else{
			if (pass){
				j = nmax; //il n'y aura plus de liberté supplémentaire, on arrête la recherche ici
			}
			else{
				//on réinitialise la recherche
				compteur = 0;
				bonus = 0;
				malus = 0;
			}
		}
	}
	if (compteur >= (*gameManagement_).getNbAlignToWin()){
		//il est possible de gagner dans cette direction
		estimation += compteur*pLiberte + bonus*pBonus + (1 - std::abs(centre / (compteur - 1) - 0.5))*compteur*pCentre + malus * 100;
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
		else if (square[i][j].getClickedBy() == couleurAdversaire){
			malus++;
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
		else if (square[i][j].getClickedBy() == couleurAdversaire){
			malus++;
		}
		else{
			i = nmax;
		}
	}
	if (compteur >= (*gameManagement_).getNbAlignToWin()){
		//il est possible de gagner dans cette direction
		estimation += compteur*pLiberte + bonus*pBonus + (1 - std::abs(centre / (compteur - 1) - 0.5))*compteur*pCentre + malus * 100;
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
		else if (square[i][j].getClickedBy() == couleurAdversaire){
			malus++;
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
		else if (square[i][j].getClickedBy() == couleurAdversaire){
			malus++;
		}
		else{
			i = nmax;
		}
	}
	if (compteur >= (*gameManagement_).getNbAlignToWin()){
		//il est possible de gagner dans cette direction
		estimation += compteur*pLiberte + bonus*pBonus + (1 - std::abs(centre / (compteur - 1) - 0.5))*compteur*pCentre + malus * 100;
	}
	return estimation;*/

	int compteur1, compteur2, i, j;
	const int couleur = square[x][y].getClickedBy();
	const int couleurAdversaire = (couleur + 1) % 2;
	int series_j11 = 0;
	int series_j12 = 0;
	int series_j13 = 0;
	int series_j14 = 0;
	int series_j21 = 0;
	int series_j22 = 0;
	int series_j23 = 0;
	int series_j24 = 0;
	int coeffSerie1 = 1;
	int coeffSerie2 = 2;
	int coeffSerie3 = 6;
	int coeffSerie4 = 18;
	int serie1 = 1;
	int serie2 = 2;
	int serie3 = 3;
	int serie4 = 4;
	const int taillePlateau = (*gameManagement_).getN();
	const int nbAlignToWin = (*gameManagement_).getNbAlignToWin();

	compteur1 = 0;
	compteur2 = 0;

	//Diagonale descendante
	for (i = 0; i<taillePlateau; i++)
	{
		if (square[i][i].getClickedBy() == couleur)
		{
			compteur1++;
			compteur2 = 0;

			if (compteur1 == serie4){
				series_j14++;
			}
			else if (compteur1 == serie3){
				series_j13++;
			}
			else if (compteur1 == serie2)
			{
				series_j12++;
			}
			else if (compteur1 == serie1){
				series_j11++;
			}
		}
		else if (square[i][i].getClickedBy() == couleurAdversaire)
		{
			compteur2++;
			compteur1 = 0;

			if (compteur2 == serie4){
				series_j24++;
			}
			else if(compteur2 == serie3){
				series_j23++;
			}
			if (compteur2 == serie2)
			{
				series_j22++;
			}
			else if (compteur2 == serie1){
				series_j21++;
			}
		}
	}

	compteur1 = 0;
	compteur2 = 0;

	//Diagonale montante
	for (i = 0; i<taillePlateau; i++)
	{
		if (square[i][2 - i].getClickedBy() == couleur)
		{
			compteur1++;
			compteur2 = 0;

			if (compteur1 == serie4){
				series_j14++;
			}
			else if(compteur1 == serie3){
				series_j13++;
			}
			else if (compteur1 == serie2)
			{
				series_j12++;
			}
			else if (compteur1 == serie1){
				series_j11++;
			}
		}
		else if (square[i][2 - i].getClickedBy() == couleurAdversaire)
		{
			compteur2++;
			compteur1 = 0;

			if (compteur2 == serie4){
				series_j24++;
			}
			else if(compteur2 == serie3){
				series_j23++;
			}
			else if (compteur2 == serie2)
			{
				series_j22++;
			}
			else if (compteur2 == serie1){
				series_j21++;
			}
		}
	}

	//En ligne
	for (i = 0; i<taillePlateau; i++)
	{
		compteur1 = 0;
		compteur2 = 0;

		//Horizontalement
		for (j = 0; j<taillePlateau; j++)
		{
			if (square[i][j].getClickedBy() == couleur)
			{
				compteur1++;
				compteur2 = 0;
			
				if (compteur1 == serie4){
					series_j14++;
				}
				else if(compteur1 == serie3){
					series_j13++;
				}
				else if (compteur1 == serie2)
				{
					series_j12++;
				}
				else if (compteur1 == serie1){
					series_j11++;
				}
			}
			else if (square[i][j].getClickedBy() == couleurAdversaire)
			{
				compteur2++;
				compteur1 = 0;

				if (compteur2 == serie4){
					series_j24++;
				}
				else if(compteur2 == serie3){
					series_j23++;
				}
				else if (compteur2 == serie2)
				{
					series_j22++;
				}
				else if (compteur2 == serie1){
					series_j21++;
				}
			}
		}

		compteur1 = 0;
		compteur2 = 0;

		//Verticalement
		for (j = 0; j<taillePlateau; j++)
		{
			if (square[j][i].getClickedBy() == couleur)
			{
				compteur1++;
				compteur2 = 0;

				if (compteur1 == serie4){
					series_j14++;
				}
				else if(compteur1 == serie3){
					series_j13++;
				}
				else if (compteur1 == serie2)
				{
					series_j12++;
				}
				else if (compteur1 == serie1){
					series_j11++;
				}
			}
			else if (square[j][i].getClickedBy() == couleurAdversaire)
			{
				compteur2++;
				compteur1 = 0;

				if (compteur2 == serie4){
					series_j24++;
				}
				else if(compteur2 == serie3){
					series_j23++;
				}
				if (compteur2 == serie2)
				{
					series_j22++;
				}
				else if (compteur2 == serie1){
					series_j21++;
				}
			}
		}
	}

	return (series_j11 * coeffSerie1 + series_j12 * coeffSerie2 + series_j13 * coeffSerie3 + series_j14 * coeffSerie4) - 
				(series_j21 * coeffSerie1 + series_j22 * coeffSerie2 + series_j23 * coeffSerie3 + series_j24 * coeffSerie4);
}

int IA::lignePeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns){
	const int taillePlateau = (*gameManagement_).getN();
	const int nbAlignToWin = (*gameManagement_).getNbAlignToWin();
	int compteur = 0;
	int dist = 0;

	// ligne gauche droite
	for (int i = columns; dist < nbAlignToWin && i < taillePlateau; i++){
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
	dist = 0;
	for (int i = columns; dist < nbAlignToWin && i >= 0; i--){
		if (square[rows][i].getClickedBy() == -1 || square[rows][i].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else {
			break;
		}
	}

	return compteur - nbAlignToWin + 1;
}

int IA::colonnePeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns){
	const int taillePlateau = (*gameManagement_).getN();
	const int nbAlignToWin = (*gameManagement_).getNbAlignToWin();
	int compteur = 0;
	int dist = 0;

	// ligne verticale haut bas 
	for (int i = rows; dist < nbAlignToWin && i < taillePlateau; i++){
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
	dist = 0;
	for (int i = rows; dist < nbAlignToWin && i >= 0; i--){
		if (square[i][columns].getClickedBy() == -1 || square[i][columns].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else {
			break;
		}
	}

	return compteur - nbAlignToWin + 1;
}

int IA::diagoBasHautPeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns){
	const int taillePlateau = (*gameManagement_).getN();
	const int nbAlignToWin = (*gameManagement_).getNbAlignToWin();
	int compteur = 0;
	int dist = 0;

	// diago de bas en haut
	for (int i = rows, j = columns; dist < nbAlignToWin && i >= 0 && j < taillePlateau; i--, j++){
		if (square[i][j].getClickedBy() == -1 || square[i][j].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}

	// diago de haut en bas
	compteur--;
	dist = 0;
	for (int i = rows, j = columns; dist < nbAlignToWin && i < taillePlateau && j >= 0; i++, j--){
		if (square[i][j].getClickedBy() == -1 || square[i][j].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}

	return compteur - nbAlignToWin + 1;
}

int IA::diagoHautBasPeutGagner(std::vector<std::vector <Square > > &square, int couleur, int couleurAdversaire, int rows, int columns){
	const int taillePlateau = (*gameManagement_).getN();
	const int nbAlignToWin = (*gameManagement_).getNbAlignToWin();
	int compteur = 0;
	int dist = 0;

	// diago de bas en haut
	for (int i = rows, j = columns; dist < nbAlignToWin && i >= 0 && j >= 0; i--, j--){
		if (square[i][j].getClickedBy() == -1 || square[i][j].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}


	// diago de haut en bas
	compteur--;
	dist = 0;
	for (int i = rows, j = columns; dist < nbAlignToWin && i < taillePlateau && j < taillePlateau; i++, j++){
		if (square[i][j].getClickedBy() == -1 || square[i][j].getClickedBy() == couleur){
			compteur++;
			dist++;
		}
		else{
			break;
		}
	}

	return compteur - nbAlignToWin + 1;
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
	std::cout << "ligne" << lignePeutGagner(square, (*gameManagement_).getCurrentPlayer(), ((*gameManagement_).getCurrentPlayer() + 1) % 2, coupJoue.first, coupJoue.second) << std::endl;
	std::cout << "colonne" << colonnePeutGagner(square, (*gameManagement_).getCurrentPlayer(), ((*gameManagement_).getCurrentPlayer() + 1) % 2, coupJoue.first, coupJoue.second) << std::endl;
	std::cout << "bas haut" << diagoBasHautPeutGagner(square, (*gameManagement_).getCurrentPlayer(), ((*gameManagement_).getCurrentPlayer() + 1) % 2, coupJoue.first, coupJoue.second) << std::endl;
	std::cout << "haut bas" << diagoHautBasPeutGagner(square, (*gameManagement_).getCurrentPlayer(), ((*gameManagement_).getCurrentPlayer() + 1) % 2, coupJoue.first, coupJoue.second) << std::endl;
	std::cout << std::endl;
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

