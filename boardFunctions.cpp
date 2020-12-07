#include "definitions.h"

using namespace std;

Board* initializeBoard()
{
	Board* board = (Board*) malloc(sizeof(Board));

	int i = 0;

	//NO_SQUARE specifica spatiile pentru determinarea mutarilor ilegale
	for (i = 0; i < 20; i++)
	{
		board->pieces[i] = NO_SQUARE;
	}

	for (i = 100; i < 120; i++)
	{
		board->pieces[i] = NO_SQUARE;
	}

	for (i = 41; i < 79; i++)
	{
		board->pieces[i] = EMPTY_SQUARE;
	}

	for (i = 20; i < 100; i += 10)
	{
		board->pieces[i] = NO_SQUARE;
	}

	for (i = 29; i < 100; i += 10)
	{
		board->pieces[i] = NO_SQUARE;
	}

	for (i = 31; i < 39; i++)
	{
		board->pieces[i] = bP;
	}

	for (i = 81; i < 89; i++)
	{
		board->pieces[i] = wP;
	}

	board->pieces[A1] = bR;
	board->pieces[B1] = bN;
	board->pieces[C1] = bB;
	board->pieces[D1] = bQ;
	board->pieces[E1] = bK;
	board->pieces[F1] = bB;
	board->pieces[G1] = bN;
	board->pieces[H1] = bR;
	board->pieces[A8] = wR;
	board->pieces[B8] = wN;
	board->pieces[C8] = wB;
	board->pieces[D8] = wQ;
	board->pieces[E8] = wK;
	board->pieces[F8] = wB;
	board->pieces[G8] = wN;
	board->pieces[H8] = wR;

	board->whitePieces[ROOK_POSITION] = A8;
	board->whitePieces[ROOK_POSITION + 1] = H8;
	board->whitePieces[KNIGHT_POSITION] = B8;
	board->whitePieces[KNIGHT_POSITION + 1] = G8;
	board->whitePieces[BISHOP_POSITION] = C8;
	board->whitePieces[BISHOP_POSITION + 1] = F8;
	board->whitePieces[QUEEN_POSITION] = D8;
	board->whitePieces[KING_POSITION] = E8;
	board->blackPieces[ROOK_POSITION] = A1;
	board->blackPieces[ROOK_POSITION + 1] = H1;
	board->blackPieces[KNIGHT_POSITION] = B1;
	board->blackPieces[KNIGHT_POSITION + 1] = G1;
	board->blackPieces[BISHOP_POSITION] = C1;
	board->blackPieces[BISHOP_POSITION + 1] = F1;
	board->blackPieces[QUEEN_POSITION] = D1;
	board->blackPieces[KING_POSITION] = E1;


	//"whitePawns" si "blackPawns" sunt utilizati la stocarea pozitiile pionilor
	for (i = 0; i < 8; i++)
	{
		board->whitePieces[PAWN_POSITION + i] = A7 + i;
		board->blackPieces[PAWN_POSITION + i] = A2 + i;
	}

	for (i = 0; i < 4; i++)
	{
		board->rookMoved[i] = false;
	}

	board->kingMoved[WHITE] = false;
	board->kingMoved[BLACK] = false;

	board->usedCastling[WHITE] = false;
	board->usedCastling[BLACK] = false;

	board->enPassant[WHITE] = false;
	board->enPassant[BLACK] = false;


	return board;
}


// muta piesa trimisa de xboard, input-ul va fi de forma "e2e4"
void movePiece(Board* board, char* input)
{
	int firstSquare, secondSquare;
	Move move;

	convertMoveToSquares(input, &firstSquare, &secondSquare);

	move.pieceType = board->pieces[firstSquare];
	move.firstSquare = firstSquare;
	move.secondSquare = secondSquare;

	/* pentru castling, se tine cont de mutarea
	pieselor de tip king sau rook */
	if (move.pieceType == wK && board->kingMoved[WHITE] == false)
	{
		board->kingMoved[WHITE] = true;
	}
	else if (move.pieceType == bK && board->kingMoved[BLACK] == false)
	{
		board->kingMoved[BLACK] = true;
	}
	else if (move.pieceType == wR)
	{
		if (move.firstSquare == A8 && board->rookMoved[0] == false)
		{
			board->rookMoved[0] = true;
		}
		else if (move.firstSquare == H8 && board->rookMoved[1] == false)
		{
			board->rookMoved[1] = true;
		}
	}
	else if (move.pieceType == bR)
	{
		if (move.firstSquare == A1 && board->rookMoved[2] == false)
		{
			board->rookMoved[2] = true;
		}
		else if (move.firstSquare == H1 && board->rookMoved[3] == false)
		{
			board->rookMoved[3] = true;
		}
	}

	/* tratarea cazului cand se executa castling
	mutarea este indicata prin deplasarea regelui cu doua
	spatii la stanga sau la dreapta lui plecand din pozitia initiala */
	if (move.pieceType == wK && firstSquare == E8)
	{
		if (secondSquare == C8)
		{
			board->pieces[D8] = wR;
			board->pieces[A8] = EMPTY_SQUARE;
			board->whitePieces[ROOK_POSITION] = D8;
			board->usedCastling[WHITE] = true;
		}
		else if (secondSquare == G8)
		{
			board->pieces[F8] = wR;
			board->pieces[H8] = EMPTY_SQUARE;
			board->whitePieces[ROOK_POSITION + 1] = F8;
			board->usedCastling[WHITE] = true;
		}
	}
	else if (move.pieceType == bK && firstSquare == E1)
	{
		if (secondSquare == C1)
		{
			board->pieces[D1] = bR;
			board->pieces[A1] = EMPTY_SQUARE;
			board->blackPieces[ROOK_POSITION] = D1;
			board->usedCastling[BLACK] = true;
		}
		else if (secondSquare == G1)
		{
			board->pieces[F1] = bR;
			board->pieces[H1] = EMPTY_SQUARE;
			board->blackPieces[ROOK_POSITION + 1] = F1;
			board->usedCastling[BLACK] = true;
		}
	}

	/* marcheaza un pion ca fiind in trecere daca acesta se deplaseaza
	cu doua spatii in fata */
	else if (move.pieceType == wP && firstSquare / 10 == RANK_7 && secondSquare / 10 == RANK_5)
	{
		board->enPassant[WHITE] = true;
		board->enPassantSquare[WHITE] = firstSquare - 10;
	}
	else if (move.pieceType == bP && firstSquare / 10 == RANK_2 && secondSquare / 10 == RANK_4)
	{
		board->enPassant[BLACK] = true;
		board->enPassantSquare[BLACK] = firstSquare + 10;
	}

	/* daca se executa o mutare diagonala a unui pion pe EMPTY_SQUARE, atunci
	pionul va lua un pion advers in trecere */
	else if (move.pieceType == wP && board->pieces[secondSquare] == EMPTY_SQUARE
		&& ((secondSquare - firstSquare == -11) || (secondSquare - firstSquare == -9)))
	{
		for (int i = PAWN_POSITION; i < PAWN_POSITION + 8; i++)
		{
			if (board->blackPieces[i] == secondSquare + 10)
			{
				board->blackPieces[i] = -1;

				break;
			}
		}

		board->pieces[secondSquare + 10] = EMPTY_SQUARE;
	}
	else if (move.pieceType == bP && board->pieces[secondSquare] == EMPTY_SQUARE
		&& ((secondSquare - firstSquare == 9) || (secondSquare - firstSquare == 11)))
	{
		for (int i = PAWN_POSITION; i < PAWN_POSITION + 8; i++)
		{
			if (board->whitePieces[i] == secondSquare - 10)
			{
				board->whitePieces[i] = -1;

				break;
			}
		}

		board->pieces[secondSquare - 10] = EMPTY_SQUARE;
	}

	// se executa mutarea piesei pe baza culorii acesteia
	if (getColor(move.pieceType) == BLACK)
	{
		// se muta piesa neagra
		for (int i = 0; i < 16; i++)
		{
			if (board->blackPieces[i] == firstSquare)
			{
				board->blackPieces[i] = secondSquare;
			}
		}

		// se eliminina piesa alba din joc daca este cazul
		if (board->pieces[secondSquare] != EMPTY_SQUARE)
		{
			for (int i = 0; i < 16; i++)
			{
				if (board->whitePieces[i] == secondSquare)
				{
					board->whitePieces[i] = -1;
				}
			}
		}

		board->enPassant[WHITE] = false;
	}
	else
	{
		// se muta piesa alba
		for (int i = 0; i < 16; i++)
		{
			if (board->whitePieces[i] == firstSquare)
			{
				board->whitePieces[i] = secondSquare;
			}
		}

		// se elimina piesa neagra din joc daca este cazul 
		if (board->pieces[secondSquare] != EMPTY_SQUARE)
		{
			for (int i = 0; i < 16; i++)
			{
				if (board->blackPieces[i] == secondSquare)
				{
					board->blackPieces[i] = -1;
				}
			}
		}

		board->enPassant[BLACK] = false;
	}


	board->pieces[secondSquare] = board->pieces[firstSquare];
	board->pieces[firstSquare] = EMPTY_SQUARE;
}

/* returneaza un vector cu toate mutarile posibile
pentru culoarea introdusa, fara a lua in considerare
posibilitate de a fi sau a intra in sah
pentru obtinerea tuturor mutarilor valide prin verificarea
acesteia, se apeleaza getValidMoves */
vector<Move> getAvailableMoves(Board* board, COLOR color, bool checkBreak)
{
	vector<Move> moves, movesBreak;
	Move move;
	int* pieces;
	int position, square;
	int positions[8];
	int rates[8];

	if (color == WHITE)
	{
		pieces = board->whitePieces;
	}
	else
	{
		pieces = board->blackPieces;
	}

	// se parcurge array-ul de piese al culorii respective
	for(int i = 0; i < 16; i++)
	{
		position = pieces[i];
		move.pieceType = board->pieces[pieces[i]];
		move.firstSquare = position;

		switch(board->pieces[pieces[i]])
		{
			case wR:
			case bR:
				rates[0] = 1;
				rates[1] = -1;
				rates[2] = 10;
				rates[3] = -10;

				/* se considera mutarea piesei rook pe orizontala sau verticala
				pana la intalnirea unui obstacol sau spatiu invalid,
				cu posibilitatea de a lua piesa adversa */
				for (int j = 0; j < 4; j++)
				{
					position = pieces[i];
					position += rates[j];

					while (board->pieces[position] != NO_SQUARE)
					{
						move.secondSquare = position;

						if (board->pieces[position] == EMPTY_SQUARE)
						{
							moves.push_back(move);
						}
						else if (getColor(board->pieces[position]) != color)
						{
							moves.push_back(move);
							CHECK_BREAK;

							break;
						}
						else
						{
							break;
						}

						position += rates[j];
					}
				}
				
				
				break;
			case wN:
			case bN:
				// cele 8 cazuri de deplasare ale calului
				positions[0] = position + 21;
				positions[1] = position + 12;
				positions[2] = position + 19;
				positions[3] = position + 8;
				positions[4] = position - 19;
				positions[5] = position - 8;
				positions[6] = position - 21;
				positions[7] = position - 12;

				for (int j = 0; j < 8; j++)
				{
					square = board->pieces[positions[j]];

					if (square != NO_SQUARE && getColor(square) != color)
					{
						move.secondSquare = positions[j];
						moves.push_back(move);
						CHECK_BREAK;
					}
				}

				break;

			case wB:
			case bB:
				rates[0] = 11;
				rates[1] = 9;
				rates[2] = -9;
				rates[3] = -11;

				/* se considera mutarea diagonala a piesei bishop
				pana la intalnirea unui obstacol sau spatiu invalid,
				cu posibilitatea de a lua piesa adversa */
				for (int j = 0; j < 4; j++)
				{
					position = pieces[i];
					position += rates[j];

					while (board->pieces[position] != NO_SQUARE)
					{
						move.secondSquare = position;

						if (board->pieces[position] == EMPTY_SQUARE)
						{
							moves.push_back(move);
						}
						else if (getColor(board->pieces[position]) != color)
						{
							moves.push_back(move);
							CHECK_BREAK;
							break;
						}
						else
						{
							break;
						}

						position += rates[j];
					}
				}

				break;
			case wQ:
			case bQ:
				rates[0] = -11;
				rates[1] = -10;
				rates[2] = -9;
				rates[3] = -1;
				rates[4] = 1;
				rates[5] = 9;
				rates[6] = 10;
				rates[7] = 11;

				/* mutarile posibile ale reginei vor fi mutarile posibile ale
				piesei rook si mutarile piesei bishop */
				for (int j = 0; j < 8; j++)
				{
					position = pieces[i];
					position += rates[j];

					while (board->pieces[position] != NO_SQUARE)
					{
						move.secondSquare = position;

						if (board->pieces[position] == EMPTY_SQUARE)
						{
							moves.push_back(move);
						}
						else if (getColor(board->pieces[position]) != color)
						{
							moves.push_back(move);
							CHECK_BREAK;
							break;
						}
						else
						{
							break;
						}

						position += rates[j];
					}
				}

				break;
			case wK:
			case bK:
				positions[0] = position - 11;
				positions[1] = position - 10;
				positions[2] = position - 9;
				positions[3] = position - 1;
				positions[4] = position + 1;
				positions[5] = position + 9;
				positions[6] = position + 10;
				positions[7] = position + 11;

				// regele se deplaseaza un spatiu in orice directie
				for (int j = 0; j < 8; j++)
				{
					square = board->pieces[positions[j]];

					if ((getColor(square) != color && square != NO_SQUARE) || square == EMPTY_SQUARE)
					{
						move.secondSquare = positions[j];
						moves.push_back(move);
						CHECK_BREAK;
					}
				}

				// prin respectarea conditiilor, regele va putea executa castling
				if (board->usedCastling[color] == false && board->kingMoved[color] == false)
				{
					// rocada mica
					if (board->rookMoved[2 * color] == false && board->pieces[position - 1] == EMPTY_SQUARE
						&& board->pieces[position - 2] == EMPTY_SQUARE && board->pieces[position - 3] == EMPTY_SQUARE
						&& pieces[ROOK_POSITION] != -1)
					{
						move.secondSquare = position - 2;
						moves.push_back(move);
					}

					// rocada mare
					if (board->rookMoved[2 * color + 1] == false && board->pieces[position + 1] == EMPTY_SQUARE
						&& board->pieces[position + 2] == EMPTY_SQUARE && pieces[ROOK_POSITION + 1] != -1)
					{
						move.secondSquare = position + 2;
						moves.push_back(move);
					}
				}

				break;
			case wP:
			case bP:

				if (color == WHITE)
				{
					if (board->enPassant[BLACK] == true)
					{
						// se considera mutarea en passant la indeplinirea conditiilor
						if (board->enPassantSquare[BLACK] == position - 11)
						{
							move.secondSquare = position - 11;
							moves.push_back(move);
						}
						else if (board->enPassantSquare[BLACK] == position - 9)
						{
							move.secondSquare = position - 9;
							moves.push_back(move);
						}
					}
					
					if (board->pieces[position - 10] == EMPTY_SQUARE)
					{
						if (position / 10 == 8 && board->pieces[position - 20] == EMPTY_SQUARE)
						{
							move.secondSquare = position - 20;
							moves.push_back(move);
						}

						move.secondSquare = position - 10;
						moves.push_back(move);
					}


					square = board->pieces[position - 11];

					if (getColor(square) == BLACK)
					{
						move.secondSquare = position - 11;
						moves.push_back(move);
						CHECK_BREAK;

					}

					square = board->pieces[position - 9];

					if (getColor(square) == BLACK)
					{
						move.secondSquare = position - 9;
						moves.push_back(move);
						CHECK_BREAK;
					}
				}
				else
				{
					if (board->enPassant[WHITE] == true)
					{
						// se considera mutarea en passant la indeplinirea conditiilor
						if (board->enPassantSquare[WHITE] == position + 11)
						{
							move.secondSquare = position + 11;
							moves.push_back(move);
						}
						else if (board->enPassantSquare[WHITE] == position + 9)
						{
							move.secondSquare = position + 9;
							moves.push_back(move);
						}
					}
					
					if (board->pieces[position + 10] == EMPTY_SQUARE)
					{
						if (position / 10 == 3 && board->pieces[position + 20] == EMPTY_SQUARE)
						{
							move.secondSquare = position + 20;
							moves.push_back(move);
						}

						move.secondSquare = position + 10;
						moves.push_back(move);
					}

					square = board->pieces[position + 11];

					if (getColor(square) == WHITE)
					{
						move.secondSquare = position + 11;
						moves.push_back(move);
						CHECK_BREAK;
					}

					square = board->pieces[position + 9];

					if (getColor(square) == WHITE)
					{
						move.secondSquare = position + 9;
						moves.push_back(move);
						CHECK_BREAK;
					}
				}
				break;
			default:
				break;

		}
	}

	return moves;
}

char* applyMove(Board* board, Move move, bool useOutput)
{
	int color = getColor(board->pieces[move.firstSquare]);
	bool isCastling = false;
	char* output;
	int* pieces;
	int i;

	/* pentru castling, se tine cont de mutarea
	pieselor de tip king si rook */
	if (move.pieceType == wK && board->kingMoved[WHITE] == false)
	{
		board->kingMoved[WHITE] = true;
	}
	else if (move.pieceType == bK && board->kingMoved[BLACK] == false)
	{
		board->kingMoved[BLACK] = true;
	}
	else if (move.pieceType == wR)
	{
		if (move.firstSquare == A8 && board->rookMoved[0] == false)
		{
			board->rookMoved[0] = true;
		}
		else if (move.firstSquare == H8 && board->rookMoved[1] == false)
		{
			board->rookMoved[1] = true;
		}
	}
	else if (move.pieceType == bR)
	{
		if (move.firstSquare == A1 && board->rookMoved[2] == false)
		{
			board->rookMoved[2] = true;
		}
		else if (move.firstSquare == H1 && board->rookMoved[3] == false)
		{
			board->rookMoved[3] = true;
		}
	}

	// aplicarea mutarii de castling
	if (move.pieceType == wK && move.firstSquare == E8)
	{
		if (move.secondSquare == C8)
		{
			board->pieces[C8] = wK;
			board->pieces[D8] = wR;
			board->pieces[E8] = EMPTY_SQUARE;
			board->pieces[A8] = EMPTY_SQUARE;

			board->whitePieces[KING_POSITION] = C8;
			board->whitePieces[ROOK_POSITION] = D8;

			board->usedCastling[WHITE] = true;

			isCastling = true;
		}
		else if (move.secondSquare == G8)
		{
			board->pieces[G8] = wK;
			board->pieces[F8] = wR;
			board->pieces[E8] = EMPTY_SQUARE;
			board->pieces[H8] = EMPTY_SQUARE;

			board->whitePieces[KING_POSITION] = G8;
			board->whitePieces[ROOK_POSITION + 1] = F8;

			board->usedCastling[WHITE] = true;

			isCastling = true;
		}
	}
	else if (move.pieceType == bK && move.firstSquare == E1)
	{
		if (move.secondSquare == C1)
		{
			board->pieces[C1] = bK;
			board->pieces[D1] = bR;
			board->pieces[E1] = EMPTY_SQUARE;
			board->pieces[A1] = EMPTY_SQUARE;

			board->blackPieces[KING_POSITION] = C1;
			board->blackPieces[ROOK_POSITION] = D1;

			board->usedCastling[BLACK] = true;

			isCastling = true;
		}
		else if (move.secondSquare == G1)
		{
			board->pieces[G1] = bK;
			board->pieces[F1] = bR;
			board->pieces[E1] = EMPTY_SQUARE;
			board->pieces[H1] = EMPTY_SQUARE;

			board->blackPieces[KING_POSITION] = G1;
			board->blackPieces[ROOK_POSITION + 1] = F1;

			board->usedCastling[BLACK] = true;

			isCastling = true;
		}
	}

	// mutarea pionului de tip en passant
	else if (move.pieceType == wP && move.firstSquare / 10 == RANK_7 && move.secondSquare / 10 == RANK_5)
	{
		board->enPassant[WHITE] = true;
		board->enPassantSquare[WHITE] = move.firstSquare - 10;
	}
	else if (move.pieceType == bP && move.firstSquare / 10 == RANK_2 && move.secondSquare / 10 == RANK_4)
	{
		board->enPassant[BLACK] = true;
		board->enPassantSquare[BLACK] = move.firstSquare + 10;
	}

	else if (move.pieceType == wP && board->pieces[move.secondSquare] == EMPTY_SQUARE
		&& ((move.secondSquare - move.firstSquare == -11) || (move.secondSquare - move.firstSquare == -9)))
	{
		for (int i = PAWN_POSITION; i < PAWN_POSITION + 8; i++)
		{
			if (board->blackPieces[i] == move.secondSquare + 10)
			{
				board->blackPieces[i] = -1;

				break;
			}
		}

		board->pieces[move.secondSquare + 10] = EMPTY_SQUARE;
	}
	else if (move.pieceType == bP && board->pieces[move.secondSquare] == EMPTY_SQUARE
		&& ((move.secondSquare - move.firstSquare == 9) || (move.secondSquare - move.firstSquare == 11)))
	{
		for (int i = PAWN_POSITION; i < PAWN_POSITION + 8; i++)
		{
			if (board->whitePieces[i] == move.secondSquare - 10)
			{
				board->whitePieces[i] = -1;

				break;
			}
		}

		board->pieces[move.secondSquare - 10] = EMPTY_SQUARE;
	}

	// executarea mutarii standard de tip "e2e4"
	if (isCastling == false)
	{
		if (board->pieces[move.secondSquare] != EMPTY_SQUARE)
		{
			if (color == WHITE)
			{
				pieces = board->blackPieces;
			}
			else
			{
				pieces = board->whitePieces;
			}

			i = 0;

			while (pieces[i] != move.secondSquare)
			{
				i++;
			}

			
			pieces[i] = -1;			
			
		}

		board->pieces[move.secondSquare] = board->pieces[move.firstSquare];
		board->pieces[move.firstSquare] = EMPTY_SQUARE;

		if (color == WHITE)
		{
			pieces = board->whitePieces;
		}
		else
		{
			pieces = board->blackPieces;
		}

		i = 0;

		while (pieces[i] != move.firstSquare)
		{
			i++;
		}

		
		pieces[i] = move.secondSquare;
	}

	if (color == WHITE)
	{
		board->enPassant[BLACK] = false;
	}
	else
	{
		board->enPassant[WHITE] = false;
	}

	output = convertSquaresToMove(move.firstSquare, move.secondSquare);

	if (useOutput == false)
	{
		free(output);

		return NULL;
	}

	return output;

}

/* mutarea de tip castling se poate executa daca pozitiile prin care
trece regele pana la pozitia finala nu sunt atacate de piese adverse */
bool checkValidCastling(Board* board, Move move, COLOR color)
{
	Move moveCopy = move;
	int step;

	if (inCheck(board, color) == true)
	{
		return false;
	}

	Board* boardCopy = (Board*)malloc(sizeof(Board));
	memcpy(boardCopy, board, sizeof(Board));

	if (move.secondSquare - move.firstSquare > 0)
	{
		step = 1;
	}
	else
	{
		step = -1;
	}

	moveCopy.secondSquare = moveCopy.firstSquare;
	moveCopy.secondSquare += step;

	applyMove(boardCopy, moveCopy, false);

	if (inCheck(boardCopy, color) == true)
	{
		free(boardCopy);
		return false;
	}

	moveCopy.firstSquare += step;
	moveCopy.secondSquare += step;

	applyMove(boardCopy, moveCopy, false);

	if (inCheck(boardCopy, color) == true)
	{
		free(boardCopy);
		return false;
	}

	free(boardCopy);

	return true;
}

/* returneaza toate miscarile posibile luand in considerare
posibilitatea de a fi sau a intra in sah in urma executarii mutarii */
vector<Move> getValidMoves(Board* board, COLOR color)
{
	vector<Move> moves = getAvailableMoves(board, color, false);

	// afisarea miscarilor posibile
	if (DEBUG_MODE == 1)
	{
		for (int j = 0; j < moves.size(); j++)
	    {
	        printf("%d, %d %d\n", moves[j].pieceType, moves[j].firstSquare, moves[j].secondSquare);
	        fflush(stdout);
	    }
	    printf("DONE\n\n");
	    fflush(stdout);
	}

    /* se creeaza o copie la care se revine
    dupa executarea fiecarei mutari */
	Board* boardCopy = (Board*)malloc(sizeof(Board));
	memcpy(boardCopy, board, sizeof(Board));

	for (int i = 0; i < moves.size(); i++)
	{
		// se verifica posibilitatea executarii mutarii de castling
		if ((moves[i].pieceType == wK || moves[i].pieceType == bK)
			&& (moves[i].secondSquare - moves[i].firstSquare == 2
				|| moves[i].secondSquare - moves[i].firstSquare == -2))
		{
			 if (!checkValidCastling(board, moves[i], color))
			 {
				moves.erase(moves.begin() + i);
				i--;
			 }

			continue;
		}


		applyMove(boardCopy, moves[i], false);
		checkPromotion(boardCopy);

		if (inCheck(boardCopy, color) == true)
		{
			moves.erase(moves.begin() + i);
			i--;
		}

		memcpy(boardCopy, board, sizeof(Board));
	}

	free(boardCopy);
	
	return moves;
}

/* verifica daca player-ul de culoarea introdusa se afla
in sah in starea curenta a joc */
bool inCheck(Board* board, COLOR color)
{
	int kingPosition;

	if (color == WHITE)
	{
		kingPosition = board->whitePieces[KING_POSITION];
	}
	else
	{
		kingPosition = board->blackPieces[KING_POSITION];
	}

	color = (COLOR)((color + 1) % 2);

	vector<Move> moves = getAvailableMoves(board, color, true);

	if (moves.size() == 0)
	{
		return true;
	}

	return false;
}

/* verifica daca player-ul de culoarea introdusa se afla
in sah mat in starea curenta a joc */
bool isCheckMate(Board* board, vector<Move> moves, COLOR color)
{

	int kingPosition;
	bool isCheck = false;
	bool isSpace = false;
	vector<int> emptySpaces;

	if (color == WHITE)
	{
		kingPosition = board->blackPieces[KING_POSITION];
	}
	else
	{
		kingPosition = board->whitePieces[KING_POSITION];
	}

	for (int i = 0; i < moves.size(); i++)
	{
		if (moves[i].secondSquare == kingPosition)
		{
			isCheck = true;
			break;
		}
	}

	if (isCheck == false)
	{
		return false;
	} else if (moves.size() == 0)
	{
		return true;
	}

	return false;
}

/* se verifica posibilitatea schimbarii unui pion cu o regina dupa
ce acesta a parcurs toata tabla de sah */
void checkPromotion(Board* board)
{
	for (int i = PAWN_POSITION; i < PAWN_POSITION + 8; i++)
	{
		if (board->whitePieces[i] / 10 == RANK_1)
		{
			board->pieces[board->whitePieces[i]] = wQ;
		}

		if (board->blackPieces[i] / 10 == RANK_8)
		{
			board->pieces[board->blackPieces[i]] = bQ;
		}
	}
}

// converteste mutarea de tip "e2e4" in indecsi ai matricii board->pieces
void convertMoveToSquares(char* move, int* firstSquare, int* secondSquare)
{
	*firstSquare = (11 - (move[1] - '0' + 1)) * 10;
	*firstSquare += move[0] - 'a' + 1; 

	*secondSquare = (11 - (move[3] - '0' + 1)) * 10;
	*secondSquare += move[2] - 'a' + 1;
}

// converteste indecsi ai matricii board->pieces in mutarea de tip "e2e4"
char* convertSquaresToMove(int firstSquare, int secondSquare)
{
	char* move = (char*)malloc(STR_BUFF * sizeof(char));
	char command[STR_BUFF] = "move ";

	fflush(stdout);

	move[0] = (firstSquare % 10) + 'a' - 1;
	move[1] = 11 - (firstSquare / 10) + '0' - 1; 
	move[2] = (secondSquare % 10) + 'a' - 1;
	move[3] = 11 - (secondSquare / 10) + '0' - 1;
	move[4] = '\0';

	strcat(command, move);
	strcpy(move, command);

	return move;
}

/* se verifica daca se poate efectua o mutare fara a se
intra accidental in sah */
vector<Move> getValidCaptures(Board* board, COLOR color)
{
	vector<Move> moves = getAvailableCaptures(board, color);

    /* se creeaza o copie la care se revine
    dupa executarea fiecarei mutari */
	Board* boardCopy = (Board*)malloc(sizeof(Board));
	memcpy(boardCopy, board, sizeof(Board));

	for (int i = 0; i < moves.size(); i++)
	{
		applyMove(boardCopy, moves[i], false);
		checkPromotion(boardCopy);

		if (inCheck(boardCopy, color) == true)
		{
			moves.erase(moves.begin() + i);
			i--;
		}

		memcpy(boardCopy, board, sizeof(Board));
	}

	free(boardCopy);
	
	return moves;
}

vector<Move> getAvailableCaptures(Board* board, COLOR color)
{
	vector<Move> moves;
	Move move;
	int* pieces;
	int position, square;
	int positions[8];
	int rates[8];

	if (color == WHITE)
	{
		pieces = board->whitePieces;
	}
	else
	{
		pieces = board->blackPieces;
	}

	// se parcurge array-ul de piese al culorii respective
	for(int i = 0; i < 16; i++)
	{
		position = pieces[i];
		move.pieceType = board->pieces[pieces[i]];
		move.firstSquare = position;

		switch(board->pieces[pieces[i]])
		{
			case wR:
			case bR:
				rates[0] = 1;
				rates[1] = -1;
				rates[2] = 10;
				rates[3] = -10;

				/* se considera mutarea piesei rook pe orizontala sau verticala
				pana la intalnirea unui obstacol sau spatiu invalid,
				cu posibilitatea de a lua piesa adversa */
				for (int j = 0; j < 4; j++)
				{
					position = pieces[i];
					position += rates[j];

					while (board->pieces[position] != NO_SQUARE)
					{
						move.secondSquare = position;

						if (board->pieces[position] == EMPTY_SQUARE)
						{

						}
						else if (getColor(board->pieces[position]) != color)
						{
							moves.push_back(move);
							break;
						}
						else
						{
							break;
						}

						position += rates[j];
					}
				}
				
				
				break;
			case wN:
			case bN:
				// cele 8 cazuri de deplasare ale calului
				positions[0] = position + 21;
				positions[1] = position + 12;
				positions[2] = position + 19;
				positions[3] = position + 8;
				positions[4] = position - 19;
				positions[5] = position - 8;
				positions[6] = position - 21;
				positions[7] = position - 12;

				for (int j = 0; j < 8; j++)
				{
					square = board->pieces[positions[j]];

					if (square != NO_SQUARE && square != EMPTY_SQUARE && getColor(square) == (color + 1) % 2)
					{
						move.secondSquare = positions[j];
						moves.push_back(move);
					}
				}

				break;

			case wB:
			case bB:
				rates[0] = 11;
				rates[1] = 9;
				rates[2] = -9;
				rates[3] = -11;

				/* se considera mutarea diagonala a piesei bishop
				pana la intalnirea unui obstacol sau spatiu invalid,
				cu posibilitatea de a lua piesa adversa */
				for (int j = 0; j < 4; j++)
				{
					position = pieces[i];
					position += rates[j];

					while (board->pieces[position] != NO_SQUARE)
					{
						move.secondSquare = position;

						if (board->pieces[position] == EMPTY_SQUARE)
						{

						}
						else if (getColor(board->pieces[position]) != color)
						{
							moves.push_back(move);
							break;
						}
						else
						{
							break;
						}

						position += rates[j];
					}
				}

				break;
			case wQ:
			case bQ:
				rates[0] = -11;
				rates[1] = -10;
				rates[2] = -9;
				rates[3] = -1;
				rates[4] = 1;
				rates[5] = 9;
				rates[6] = 10;
				rates[7] = 11;

				/* mutarile posibile ale reginei vor fi mutarile posibile ale
				piesei rook si mutarile piesei bishop */
				for (int j = 0; j < 8; j++)
				{
					position = pieces[i];
					position += rates[j];

					while (board->pieces[position] != NO_SQUARE)
					{
						move.secondSquare = position;

						if (board->pieces[position] == EMPTY_SQUARE)
						{

						}
						else if (getColor(board->pieces[position]) != color)
						{
							moves.push_back(move);
							break;
						}
						else
						{
							break;
						}

						position += rates[j];
					}
				}

				break;
			case wK:
			case bK:
				positions[0] = position - 11;
				positions[1] = position - 10;
				positions[2] = position - 9;
				positions[3] = position - 1;
				positions[4] = position + 1;
				positions[5] = position + 9;
				positions[6] = position + 10;
				positions[7] = position + 11;

				// regele se deplaseaza un spatiu in orice directie
				for (int j = 0; j < 8; j++)
				{
					square = board->pieces[positions[j]];

					if (getColor(square) == (color + 1) % 2)
					{
						move.secondSquare = positions[j];
						moves.push_back(move);
					}
				}

				break;
			case wP:
			case bP:

				if (color == WHITE)
				{
					if (board->enPassant[BLACK] == true)
					{
						// se considera mutarea en passant la indeplinirea conditiilor
						if (board->enPassantSquare[BLACK] == position - 11)
						{
							move.secondSquare = position - 11;
							moves.push_back(move);
						}
						else if (board->enPassantSquare[BLACK] == position - 9)
						{
							move.secondSquare = position - 9;
							moves.push_back(move);
						}
					}

					square = board->pieces[position - 11];

					if (getColor(square) == BLACK)
					{
						move.secondSquare = position - 11;
						moves.push_back(move);
					}

					square = board->pieces[position - 9];

					if (getColor(square) == BLACK)
					{
						move.secondSquare = position - 9;
						moves.push_back(move);
					}
				}
				else
				{
					if (board->enPassant[WHITE] == true)
					{
						// se considera mutarea en passant la indeplinirea conditiilor
						if (board->enPassantSquare[WHITE] == position + 11)
						{
							move.secondSquare = position + 11;
							moves.push_back(move);
						}
						else if (board->enPassantSquare[WHITE] == position + 9)
						{
							move.secondSquare = position + 9;
							moves.push_back(move);
						}
					}
					
					square = board->pieces[position + 11];

					if (getColor(square) == WHITE)
					{
						move.secondSquare = position + 11;
						moves.push_back(move);
					}

					square = board->pieces[position + 9];

					if (getColor(square) == WHITE)
					{
						move.secondSquare = position + 9;
						moves.push_back(move);
					}
				}
				break;
			default:
				break;

		}
	}

	return moves;
}
