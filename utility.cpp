#include "definitions.h"

using namespace std;


/* converteste indecsi ai matricii board->pieces in indecsi ai matricii
de valori corespunzatoare pozitiilor pieselor albe */
void convertSqToPosWhite(int square, int& x, int& y)
{
	x = square / 10 - 2;
	y = square % 10 - 1;
}

/* converteste indecsi ai matricii board->pieces in indecsi ai matricii
de valori corespunzatoare pozitiilor pieselor negre */
void convertSqToPosBlack(int square, int& x, int& y)
{
	x = 7 - (square / 10 - 2);
	y = square % 10 - 1;
}

COLOR otherColor(COLOR last_color)
{
	if (last_color == WHITE)
	{
		return BLACK;
	}

	return WHITE;
}

// returneaza culoarea piesei
int getColor(int piece)
{
	if (piece > wK && piece <= bK)
	{
		return BLACK;
	}
	else if (piece < bP && piece > EMPTY)
	{
		return WHITE;
	}

	return -1;
}

// returneaza numarul total de piese de pe tabla
int countPieces(Board* board)
{
    int count = 0;

    for (int i = 0; i < 16; i++)
    {
        if (board->whitePieces[i] != -1)
        {
            count++;
        }

        if (board->blackPieces[i] != -1)
        {
            count++;
        }
    }

    return count;
}

// adauga o mtare in in vectorul de mutari
void addHistory(Board* board, Move move)
{
    board->movesHistory.push_back(move);

    if (board->movesHistory.size() > 4)
    {
      board->movesHistory.erase(board->movesHistory.begin());
    }
}

/* se verifica daca o mutare s-a efectuat deja anterior 
de mai mult de doua ori */
bool inHistory(Board* board, Move move)
{
    int count = 0;

    for (int i = 0; i < board->movesHistory.size(); i++)
    {
      if (move.pieceType == board->movesHistory[i].pieceType
        && (move.secondSquare == board->movesHistory[i].firstSquare
        || move.secondSquare == board->movesHistory[i].secondSquare))
      {
        count++;

        if (count < 2)
        {
          continue;
        }
        
        return true;
      }
    }

    return false;
}

void debugPrint(Board* board, int color)
{
  // Se afiseaza un vector cu pozitiile pieselor albe pe tabla de sah
  for (int i = 0; i < 16; i++)
  {
    printf("%d ", board->whitePieces[i]);
    fflush(stdout);
  }

  printf("\n");
  fflush(stdout);

  // Se afiseaza un vector cu pozitiile pieselor negre pe tabla de sah
  for (int i = 0; i < 16; i++)
  {
    printf("%d ", board->blackPieces[i]);
    fflush(stdout);
  }

  printf("\n");
  fflush(stdout);

    // Se afiseaza tabla de sah sub forma unei matrici 8x8
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int piece = board->pieces[i*10 + j];
            
            switch (piece)
            {
              case wP:
                printf("wP ");
                fflush(stdout);
                break;
              case wR:
                printf("wR ");
                fflush(stdout);
                break;
              case wN:
                printf("wN ");
                fflush(stdout);
                break;
              case wB:
                printf("wB ");
                fflush(stdout);
                break;
              case wQ:
                printf("wQ ");
                fflush(stdout);
                break;
              case wK:
                printf("wK ");
                fflush(stdout);
                break;
              case bP:
                printf("bP ");
                fflush(stdout);
                break;
              case bR:
                printf("bR ");
                fflush(stdout);
                break;
              case bN:
                printf("bN ");
                fflush(stdout);
                break;
              case bB:
                printf("bB ");
                fflush(stdout);
                break;
              case bQ:
                printf("bQ ");
                fflush(stdout);
                break;
              case bK:
                printf("bK ");
                fflush(stdout);
                break;
              case EMPTY_SQUARE:
                printf("-- ");
              default:
                break;

            }
        }

        printf("\n");
        fflush(stdout);
    }

    printf("DONE\n");
    fflush(stdout);
}

void error(int code)
{
  printf("ERROR NR: %d\n", code);
  fflush(stdout);
  exit(1);
}
