#include "definitions.h"

using namespace std;


int main()
{
	char input[STR_BUFF];
    char output[STR_BUFF];
    char previous_input[STR_BUFF] = "-1";
    Board* board;

    XBOARD_MODE xboard = UNCONNECTED;
    FORCE_MODE force = INACTIVE;
    COLOR player_color = WHITE;

    removeSigInt();

    while (scanf("%s", input))
    {
    	// Tratarea comenzii xboard
    	if (strncmp(input, "xboard", 6) == 0 && xboard == UNCONNECTED) {
    		xboard = CONNECTED;

    		printf("Engine is CONNECTED\n");
    		fflush(stdout);
    	}

        // Tratarea comenzii computer
        if (strncmp(input, "computer", 8) == 0) {
            printf("Playing against computer.\n");
            fflush(stdout);
        }

    	// Tratarea comenzii new
    	if (strncmp(input, "new", 3) == 0) {
    		board = initializeBoard();
    		force = INACTIVE;
    		player_color = WHITE;

    		printf("New board\n");
    		fflush(stdout);
    	}

        // Tratarea comenzii speciale force
        if (strncmp(previous_input, "hard", 4) == 0 && strncmp(input, "force", 5) == 0) {
            printf("Receved level, post, hard and force\n");
            fflush(stdout);
        }

    	// Tratarea comenzii force
    	if (strncmp(previous_input, "exit", 4) != 0 && strncmp(previous_input, "hard", 4) != 0 
            && strncmp(input, "force", 5) == 0) {
    		force = ACTIVE;

    		printf("Force mode ON\n");
    		fflush(stdout);
    	}

    	// Tratarea comenzii go ; engine iese din force mode
    	if (strncmp(previous_input, "exit", 4) == 0 && strncmp(input, "force", 5) == 0) {
    		force == INACTIVE;

    		printf("Force mode OFF\n");
    		fflush(stdout);
    	}

    	// Tratarea comenzii black
    	if (strncmp(previous_input, "black", 5) == 0 && strncmp(input, "time", 4) == 0) {
    		scanf("%s", input);		// citeste timpul pentru engine
    		scanf("%s %s", previous_input, input);	// citeste timpul pentru adversar
    		scanf("%s", input);		// citeste "white"
    		scanf("%s", input);		// citeste "go"

    		force == INACTIVE;
    		player_color = BLACK;

            // se genereaza multimea mutarilor posibile
            vector<Move> moves = getValidMoves(board, otherColor(player_color));

            // daca nu mai exista mutari posibile, partida se incheie
            if (moves.size() == 0)
            {
            	printf("resign\n");
            	fflush(stdout);

            	continue;
            }
            
            // se apeleaza algoritmul minimax de tip Alpha-Beta si se executa muatrea obtinuta
            pair<int, Move> bestPair = minimax(board, otherColor(player_color), DEPTH, -INT_MAX, INT_MAX, 30);
            strcpy(output, applyMove(board, bestPair.second, true));

            if (DEBUG_MODE == 1) 
            {
                debugPrint(board, BLACK);
            }

            // se trimite mutarea catre xboard
    		printf("%s\n", output);
	    	fflush(stdout);

            // se verifica daca vreun pion s-a schimbat cu alta piesa
            checkPromotion(board);
    	}

    	// Tratarea comenzii white
    	if (strncmp(previous_input, "white", 5) == 0 && strncmp(input, "time", 4) == 0) {
    		scanf("%s", input);		// citeste timpul pentru engine
    		scanf("%s %s", previous_input, input);	// citeste timpul pentru adversar
    		scanf("%s", input);		// citeste "black"
    		scanf("%s", input);		// citeste "go"

    		force == INACTIVE;
    		player_color = WHITE;

            // se genereaza multimea mutarilor posibile
            vector<Move> moves = getValidMoves(board, otherColor(player_color));

            // daca nu mai exista mutari posibile, partida se incheie
            if (moves.size() == 0)
            {
            	printf("resign\n");
            	fflush(stdout);

            	continue;
            }

            // se apeleaza algoritmul minimax de tip Alpha-Beta si se executa muatrea obtinuta
            pair<int, Move> bestPair = minimax(board, otherColor(player_color), DEPTH, -INT_MAX, INT_MAX, 30);
            strcpy(output, applyMove(board, bestPair.second, true));

            if (DEBUG_MODE == 1) 
            {
                debugPrint(board, BLACK);
            }

            // se trimite mutarea catre xboard
    		printf("%s\n", output);
	    	fflush(stdout);

            // se verifica daca vreun pion s-a schimbat cu alta piesa
            checkPromotion(board);
    	}

    	/* Tratarea inputului de forma "e2e4", specific mutarii primite
        de la xboard */
    	if (input[0] >= 'a' && input[0] <= 'h' && input[1] >= '1' && input[1] <= '8') {
    		
            // se executa mutarea primita de la xboard
    		movePiece(board, input);

    		if (force == INACTIVE) {
                COLOR color = (COLOR)((player_color + 1) % 2);

                // se genereaza multimea mutarilor posibile
                vector<Move> moves = getValidMoves(board, color);

                // daca nu mai exista mutari posibile, partida se incheie
                if (moves.size() == 0)
                {
                	printf("resign\n");
                	fflush(stdout);

                	continue;
                }

                pair<int, Move> bestPair;

                int pieces = countPieces(board);

                if (pieces > PIECES_THRESHOLD)
                {
                    bestPair = minimax(board, color, DEPTH, -INT_MAX, INT_MAX, pieces);                    
                }
                else
                {
                    bestPair = minimax(board, color, DEPTH + 1, -INT_MAX, INT_MAX, pieces);
                }

                // se executa mutarea obtinuta de la algoritmul minimax
                strcpy(output, applyMove(board, bestPair.second, true));
                addHistory(board, bestPair.second);

                // se trimite mutarea catre xboard
	    		printf("%s\n", output);
	    		fflush(stdout);
			}

            if (DEBUG_MODE == 1) 
            {
                debugPrint(board, BLACK);
            }

            // se verifica daca vreun pion s-a schimbat cu alta piesa
            checkPromotion(board);
    	}

    	strcpy(previous_input, input);
    }

	return 0;
}
