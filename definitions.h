#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <utility>

#define DEBUG_MODE 0  // 0 ~ off ; 1 ~ on
#define STR_BUFF 256
#define BOARD_SIZE 120
#define PIECES_THRESHOLD 5
#define DEPTH 5

#define CHECK_BREAK if (checkBreak == true) if (board->pieces[move.secondSquare] == wK || board->pieces[move.secondSquare] == bK) return movesBreak;

typedef unsigned long long U64;

/* tipurile de piese, prefixul w pentru piesele albe
iar b pentru piesele negre */
enum {
	EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK
};

/* dimensiunile si spatiile tablei de joc */
enum {
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

enum {
	RANK_NONE = 1, RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

/* liniile 0, 1, 10, 11 si coloanele 0, 9 sunt utilizate
la determinarea mutarilor ilegale */
enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8,
	EMPTY_SQUARE = 99,
	NO_SQUARE = 100
};

enum {
	FALSE, TRUE
};

enum XBOARD_MODE {
	CONNECTED,
	UNCONNECTED,
	DISCONNECTED
};

enum FORCE_MODE {
	ACTIVE,
	INACTIVE
};

enum COLOR {
	WHITE,
	BLACK,
};

enum POSITION {
	ROOK_POSITION = 0,
	KNIGHT_POSITION = 2,
	BISHOP_POSITION = 4,
	QUEEN_POSITION = 6,
	KING_POSITION = 7,
	PAWN_POSITION = 8
};

/* valorile pieselor de sah necesare calcularii scorului in functia
eval: pozitive pentru piese albe si negative pentru piese negre */
enum VALUES {
	WHITE_PAWN_VALUES = 100,
	WHITE_KNIGHT_VALUES = 320,
	WHITE_BISHOP_VALUES = 330,
	WHITE_ROOK_VALUES = 500,
	WHITE_QUEEN_VALUES = 900,
	WHITE_KING_VALUES = 20000,
	WHITE_BONUS = 10000, // bonus pentru sah mat

	BLACK_PAWN_VALUES = -100,
	BLACK_KNIGHT_VALUES = -320,
	BLACK_BISHOP_VALUES = -330,
	BLACK_ROOK_VALUES = -500,
	BLACK_QUEEN_VALUES = -900,
	BLACK_KING_VALUES = -20000,
	BLACK_BONUS = -10000 // bonus pentru sah mat
};

/* ordinea nivelelor matricilor de valori in functie de piesele de joc */
enum MATRIX_POS {
	ROOK, KNIGHT, BISHOP, QUEEN, KING, KING2, PAWN
};

/* matricea valorilor corespunzatoare pozitiilor pieselor pe tabla de sah.
Aceasta e reprezentata din perspectiva pieselor albe */
const int MATRIX[7][8][8] = 
{
	{ // ROOK
		{ 0,  0,  0,  0,  0,  0,  0,  0},
        { 5, 10, 10, 10, 10, 10, 10,  5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        { 0,  0,  0,  5,  5,  0,  0,  0}
	},
	{ // KNIGHT
		{-50, -40, -30, -30, -30, -30, -40, -50},
        {-40, -20,   0,   0,   0,   0, -20, -40},
        {-30,   0,  10,  15,  15,  10,   0, -30},
        {-30,   5,  15,  20,  20,  15,   5, -30},
        {-30,   0,  15,  20,  20,  15,   0, -30},
        {-30,   5,  10,  15,  15,  10,   5, -30},
        {-40, -20,   0,   5,   5,   0, -20, -40},
        {-50, -40, -20, -30, -30, -20, -40, -50}
	}, 
	{ // BISHOP
		{-20, -10, -10, -10, -10, -10, -10, -20},
        {-10,   0,   0,   0,   0,   0,   0, -10},
        {-10,   0,   5,  10,  10,   5,   0, -10},
        {-10,   5,   5,  10,  10,   5,   5, -10},
        {-10,   0,  10,  10,  10,  10,   0, -10},
        {-10,  10,  10,  10,  10,  10,  10, -10},
        {-10,   5,   0,   0,   0,   0,   5, -10},
        {-20, -10, -10, -10, -10, -10, -10, -20}
	},
	{ // QWEEN
		{-20, -10, -10, -5, -5, -10, -10, -20},
        {-10,   0,   0,  0,  0,   0,   0, -10},
        {-10,   0,   5,  5,  5,   5,   0, -10},
        { -5,   0,   5,  5,  5,   5,   0,  -5},
        {  0,   0,   5,  5,  5,   5,   0,  -5},
        {-10,   5,   5,  5,  5,   5,   0, -10},
        {-10,   0,   5,  0,  0,   0,   0, -10},
        {-20, -10, -10, -5, -5, -10, -10, -20}
	},
	{ // KING  (strategie defensiva)
		{-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-20, -30, -30, -40, -40, -30, -30, -20},
        {-10, -20, -20, -20, -20, -20, -20, -10},
        { 20,  20,   0,   0,   0,   0,  20,  20},
        { 20,  30,  10,   0,   0,  10,  30,  20}
	},
	{ // KING2 (startegie ofensiva)
		{-50, -40, -30, -20, -20, -30, -40, -50},
        {-30, -20, -10,   0,   0, -10, -20, -30},
        {-30, -10,  20,  30,  30,  20, -10, -30},
        {-30, -10,  30,  40,  40,  30, -10, -30},
        {-30, -10,  30,  40,  40,  30, -10, -30},
        {-30, -10,  20,  30,  30,  20, -10, -30},
        {-30, -30,   0,   0,   0,   0, -30, -30},
        {-50, -30, -30, -30, -30, -30, -30, -50}
	},
	{ // PAWN
		{ 0,  0,   0,   0,   0,   0,  0,  0},
        {50, 50,  50,  50,  50,  50, 50, 50},
        {10, 10,  20,  30,  30,  20, 10, 10},
        { 5,  5,  10,  25,  25,  10,  5,  5},
        { 0,  0,   0,  20,  20,   0,  0,  0},
        { 5, -5, -10,   0,   0, -10, -5,  5},
        { 5, 10,  10, -20, -20,  10, 10,  5},
        { 0,  0,   0,   0,   0,   0,  0,  0}
	}
};

/* structura ce descrie o mutare prin piesa mutata, spatiul de pa care
se muta si spatiul unde ajunge prin mutare */
typedef struct {
	int pieceType;
	int firstSquare;
	int secondSquare;
	bool priority;
} Move;

const Move initMove {
	1, 0, 0
};

/* in pieces se stocheaza tipul piesei pe fiecare spatiu de pe tabla
in whitePieces si blackPieces se stocheaza spatiul de pe tabla
unde se afla piesa respectiva
whitePieces[ROOK_POSITION] va fi spatiul de pe tabla unde se afla
prima tura, whitePIECES[ROOK_POSITION + 1] pentru a doua tura,
urmand KNIGHT_POSITION, BISHOP_POSITION etc
whitePawns si blackPawns din etapa trecuta sunt inclusi aici
usedCastling indica daca s-a utilizat castling in partida curenta
rookMoved si kingMoved sunt utilizate la validarea posibilitatii
de aplicare a miscarii castling, daca rook sau king au fost
mutate, mutarea nu se poate executa
enPassant indica daca exista un pion in trecere, iar enPassantSquare
indica spatiul pe care se poate deplasa un pion pentru a lua pionul
advers in trecere */
typedef struct {
	int pieces[BOARD_SIZE];
	int whitePieces[16];
	int blackPieces[16];
	bool usedCastling[2];
	bool rookMoved[4];
	bool kingMoved[2];
	bool pawnChecking;
	bool enPassant[2];
	int enPassantSquare[2];
	std::vector<Move> movesHistory;
} Board;

void debugPrint(Board*, int);

Board* initializeBoard();
void pawnTakesPawn(Board*, char, int);
void movePiece(Board*, char*);
char* nextMove(Board*, COLOR);
char* applyMove(Board*, Move, bool);
void convertMoveToSquares(char*, int*, int*);
char* convertSquaresToMove(int, int);
void convertSqToPosWhite(int, int&, int&);
void convertSqToPosBlack(int, int&, int&);
COLOR otherColor(COLOR);
int countPieces(Board*);
void addHistory(Board*, Move);
bool inHistory(Board*, Move);
void error(int);
int eval (Board*, COLOR, int);
bool orderPriority(Move, Move);
std::pair<int, Move> minimax(Board*, COLOR, int, int, int, int);
std::pair<int, Move> quiescenceSearch(Board*, COLOR, int, int, int);
void removeSigInt();
int getColor(int);
std::vector<Move> getAvailableMoves(Board*, COLOR, bool);
std::vector<Move> getValidMoves(Board*, COLOR);
std::vector<Move> getAvailableCaptures(Board*, COLOR);
std::vector<Move> getValidCaptures(Board*, COLOR);
bool inCheck(Board*, COLOR);
bool isCheckMate(Board*, std::vector<Move>, COLOR);
void checkPromotion(Board* board);
bool checkValidCastling(Board*, Move, COLOR);
