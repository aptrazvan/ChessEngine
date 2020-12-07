#include "definitions.h"

using namespace std;


int eval (Board* board, COLOR color, int movesSize) 
{
	int x, y, countWhitePieces = 0, countBlackPieces = 0;
	int score = 0;

	// sunt numarate toate piesele dupa culoare
	for (int i = 0; i < 16; ++i)
	{
		if (board->whitePieces[i] != -1)
		{
			countWhitePieces++;
		}

		if (board->blackPieces[i] != -1)
		{
			countBlackPieces++;
		}
	}

	// scorul actualizat dupa fiecare piesa alba
	for (int i = 0 ; i < 16; ++i)
	{
		if (board->whitePieces[i] == -1)
		{
			continue;
		}

		convertSqToPosWhite(board->whitePieces[i], x, y);

		if (DEBUG_MODE == 1)
		{
			printf("Poz pe tabla %d ,x %d y %d\n", board->whitePieces[i], x, y);
			fflush(stdout);
		}

		switch (board->pieces[board->whitePieces[i]])
		{
			case wR: 
			{
				score += WHITE_ROOK_VALUES;
				score += MATRIX[ROOK][x][y];
				break;
			}

			case wN:
			{
				score += WHITE_KNIGHT_VALUES;
				score += MATRIX[KNIGHT][x][y];
				break;
			}

			case wB:
			{
				score += WHITE_BISHOP_VALUES;
				score += MATRIX[BISHOP][x][y];
				break;
			}

			case wQ:
			{
				score += WHITE_QUEEN_VALUES;
				score += MATRIX[QUEEN][x][y];
				break;
			}

			case wK:
			{
				score += WHITE_KING_VALUES;

				/* valoarea pozitiei regelui se calculeaza in functie de
				strategia aleasa; daca sunt mai putin de 10 piese pe tabla
				de sah, regele joaca ofensiv, incercand sa atace */ 
				if (countWhitePieces + countBlackPieces >= 10)
				{
					score += MATRIX[KING][x][y];
				} else {
					score += MATRIX[KING2][x][y];
				}
				break;
			}

			case wP:
			{
				score += WHITE_PAWN_VALUES;
				score += MATRIX[PAWN][x][y];
				break;
			}
		}

	}

	//  scorul actualizat dupa fiecare piesa neagra
	for (int i = 0 ; i < 16; ++i)
	{
		if (board->blackPieces[i] == -1)
		{
			continue;
		}

		convertSqToPosBlack(board->blackPieces[i], x, y);

		if (DEBUG_MODE == 1)
		{
			printf("Poz pe tabla %d ,x %d y %d\n", board->blackPieces[i], x, y);
			fflush(stdout);
		}

		switch (board->pieces[board->blackPieces[i]])
		{
			case bR: 
			{
				score += BLACK_ROOK_VALUES;
				score -= MATRIX[ROOK][x][y];
				break;
			}

			case bN:
			{
				score += BLACK_KNIGHT_VALUES;
				score -= MATRIX[KNIGHT][x][y];
				break;
			}

			case bB:
			{
				score += BLACK_BISHOP_VALUES;
				score -= MATRIX[BISHOP][x][y];
				break;
			}

			case bQ:
			{
				score += BLACK_QUEEN_VALUES;
				score -= MATRIX[QUEEN][x][y];
				break;
			}

			case bK:
			{
				score += BLACK_KING_VALUES;

				/* valoarea pozitiei regelui se calculeaza in functie de
				strategia aleasa; daca sunt mai putin de 10 piese pe tabla
				de sah, regele joaca ofensiv, incercand sa atace */ 
				if (countWhitePieces + countBlackPieces >= 10)
				{
					score += MATRIX[KING][x][y];
				} else {
					score += MATRIX[KING2][x][y];
				}
				break;
			}

			case bP:{
				score += BLACK_PAWN_VALUES;
				score += MATRIX[PAWN][x][y];
				break;
			}
		}
	}

	// se actualizeaza scorul in functie de culoare
	if (color == WHITE)
	{
		return score;		
	}
	else
	{
		return -score;
	}

}

// functie folosita pentru compararea miscarilor in algoritmul minimax
bool orderPriority(Move firstMove, Move secondMove)
{
	if (firstMove.priority == true && secondMove.priority == false)
	{
		return true;
	}

	return false;
}

/* algoritmul minimax de tip alpha-beta functioneaza recursiv pana la 
depth de 1 unde se apeleaza euristica quiescenceSearch*/
pair<int, Move> minimax(Board* board, COLOR color, int depth, int alpha, int beta, int pieces)
{
	
	// algoritmul se opreste la depth 0 sau can nu mai sunt miscari valide
  	if (depth == 0)
  	{
   		return {eval(board, color, 0), initMove}; 
  	}

  	vector<Move> moves = getValidMoves(board, color);
  	int movesSize = moves.size();

  	// se verifica daca s-a dat sah mat
  	if (pieces < 20 && depth < 4 && isCheckMate(board, moves, otherColor(color)))
  	{
  		return {INT_MAX - 1, initMove}; 
  	}

  	if (movesSize == 0)
  	{
   		return {eval(board, color, movesSize), initMove}; 
  	}

  	/* se sorteaza vectorul de miscari in functie de miscarile ce pot lua
  	din piesele adversarului; sunt verificate mai intai miscarile ce au ca
  	rezultat un scor mai bun in functia eval */
  	for (int i = 0; i < movesSize; i++)
  	{
  		if (getColor(board->pieces[moves[i].secondSquare]) == -1)
  		{
  			moves[i].priority = false;
  		}
  		else
  		{
  			moves[i].priority = true;
  		}
  	}

  	sort(moves.begin(), moves.end(), orderPriority);


  	Move best_move = initMove;

	for (auto &move : moves)
	{
		/* daca o miscare exista de mai multe ori in vectorul de miscari
		aceasta se ignora si se merge la urmatoarea*/
		if (depth == DEPTH && inHistory(board, move) && moves.size() > 1)
		{
			continue;
		}

		// se creaza o copie a tablei de joc pe care se va efectua miscarea curenta
		Board* new_board = (Board*)malloc(sizeof(Board));
		memcpy(new_board, board, sizeof(Board));

		// se efectueaza miscarea
		applyMove(new_board, move, false);

		// se verifica daca vreun pion s-a schimbat cu alta piesa
		checkPromotion(new_board);

		pair<int, Move> res;

		// se trece pe euristica quiescenceSearch daca depth e 1
		if (depth != 1)
		{
			res = minimax(new_board, otherColor(color), depth - 1, -beta, -alpha, pieces);			
		}
		else
		{
			res = quiescenceSearch(new_board, otherColor(color), depth - 1, -beta, -alpha);	
		}

		// se pastreaza scorul cel mai favorabil in parametrul alpha
		int score = -res.first;
		if (score > alpha)
		{
			alpha = score;
			best_move = move;
		}


		free(new_board);

		if (alpha >= beta)
		{
			return {beta, initMove};
		}
	}

  // returnam pereche: cel mai bun score, mutare aferenta
	return {alpha, best_move};
}

// se alege o miscare ce captureaza piesele adversarului
pair<int, Move> quiescenceSearch(Board* board, COLOR color, int depth, int alpha, int beta)
{
	
	// algoritmul se opreste la depth 0 sau can nu mai sunt miscari valide
  	if (depth == -1)
  	{
   		return {eval(board, color, 0), initMove}; 
  	}

  	/* se apeleaza functia de evaluare cu miscarile ce captureaza 
  	piesele adversarului creand o tactica ofensiva */
  	vector<Move> moves = getValidCaptures(board, color);
  	int movesSize = moves.size();

  	if (movesSize == 0)
  	{
   		return {eval(board, color, movesSize), initMove}; 
  	}

  	Move best_move = initMove;

	for (auto &move : moves)
	{
		// se creaza o copie a tablei de joc pe care se va efectua miscarea curenta
		Board* new_board = (Board*)malloc(sizeof(Board));
		memcpy(new_board, board, sizeof(Board));

		// se efectueaza miscarea
		applyMove(new_board, move, false);

		// se verifica daca vreun pion s-a schimbat cu alta piesa
		checkPromotion(new_board);

		auto res = quiescenceSearch(new_board, otherColor(color), depth - 1, -beta, -alpha);	

		// se pastreaza scorul cel mai favorabil in parametrul alpha
		int score = -res.first;
		if (score > alpha)
		{
			alpha = score;
			best_move = move;
		}


		free(new_board);

		if (alpha >= beta)
		{
			return {beta, initMove};
		}
	}

  // returnam pereche: cel mai bun score, mutare aferenta
	return {alpha, best_move};
}
