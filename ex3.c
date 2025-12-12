#include <stdio.h>
#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

#define BORDER '|'

//////////////////////////////////
///      Global Variables
int board[ROWS][COLS * 2 + 1];
int current_player_type ;
int player1 , player2 ;
int final_round=ROWS*COLS ;
///////////////////////////////////
///






///////////////////////////////////////
///    board designing tool
void design_board(int board[ROWS][COLS*2+1]) {



//filling the array logic
for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS*2+1; col++) {
        board[row][col] = BORDER;
        board[row][col+1] = EMPTY;
        col++;
    }
}



}
////////////////////////////////////////
///


////////////////////////////////////////
///     PrintBoard
void print_board() {
    //printing the board
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS*2+1; col++) {
            printf("%c", board[row][col]);
        }
        printf("\n");

    }
    for (int col = 0; col < COLS ;col++) {
        printf(" %d" ,col+1);
    }

}









////////////////////////////////////////
///        Player Type Handler
int get_player_type(int player_number) {
    char playertype ;
    printf("Choose type for player %d: h - human, c - computer:" ,player_number );
    while (1) {
        scanf(" %c", &playertype);
        if (playertype =='c' || playertype =='C')   return COMPUTER;

        if (playertype =='h' || playertype =='H')   return HUMAN;

            printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n');
    }

/*Although the code isn't optimal
 *im trying to match the assignment input outputs expectations,
 *so I didn't optimize it more,
 *although I would've preferred a function that would have an array of chars
 *to check each char individually and check smartly , but for the given assignment ill settle for that
 */
}
////////////////////////////////////////
////








//////////////////////////////////////
///      The Game design
void GameBody(){
printf("Connect Four (%d rows x %d cols)\n" , ROWS, COLS);
//printf("Choose type for player 1: h - human, c - computer:" );
//    get_player_type();
//printf("Choose type for player 2: h - human, c - computer:" );
//    get_player_type();


}
////////////////////////////////////////
///
int sim_move(int col_idx, int token, int target_len) {
    // 1. Find the row where the token would land (Gravity)
    int r = -1;
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col_idx] == EMPTY) {
            r = i;
            break;
        }
    }
    if (r == -1) return 0; // Column is full

    // 2. Place the token temporarily
    board[r][col_idx] = token;

    // 3. Check for the sequence
    int count = 0;

    // Check Horizontal (-)
    int c_start = col_idx;
    while(c_start > 1 && board[r][c_start-2] == token) c_start -= 2; // Go far left
    for (int i=0; i < target_len; i++) {
        if (c_start + i*2 < COLS*2+1 && board[r][c_start + i*2] == token) count++;
        else count = 0;
        if (count == target_len) { board[r][col_idx] = EMPTY; return 1; }
    }

    // Check Vertical (|)
    count = 0;
    // We only need to look down because we just placed the top piece
    for (int i = 0; i < target_len; i++) {
        if (r + i < ROWS && board[r+i][col_idx] == token) count++;
        else break;
    }
    if (count == target_len) { board[r][col_idx] = EMPTY; return 1; }
    board[r][col_idx] = EMPTY;
    return 0;
}



////////////////////////////////////////
///  AI Logic
int ai_play(){
int my_token = TOKEN_P2;   // Computer is 'O'
    int opp_token = TOKEN_P1;  // Human is 'X'

    // We check all priorities in order.
    // As soon as we find a move, we make it and return.

    // --- Priority 1: Win Now (4 in a row) ---
    for (int c = 1; c <= COLS; c++) {
        int idx = (c - 1) * 2 + 1;
        if (sim_move(idx, my_token, 4)) {
            for (int r = ROWS - 1; r >= 0; r--) {
                if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
            }
            printf("Computer chose column %d\n", c);
            return 0;
        }
    }

    // --- Priority 2: Block Opponent Win (Prevent 4 in a row) ---
    for (int c = 1; c <= COLS; c++) {
        int idx = (c - 1) * 2 + 1;
        if (sim_move(idx, opp_token, 4)) {
            for (int r = ROWS - 1; r >= 0; r--) {
                if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
            }
            printf("Computer chose column %d\n", c);
            return 0;
        }
    }

    // --- Priority 3: Create Sequence of 3 ---
    for (int c = 1; c <= COLS; c++) {
        int idx = (c - 1) * 2 + 1;
        if (sim_move(idx, my_token, 3)) {
            for (int r = ROWS - 1; r >= 0; r--) {
                if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
            }
            printf("Computer chose column %d\n", c);
            return 0;
        }
    }

    // --- Priority 4: Block Opponent Sequence of 3 ---
    for (int c = 1; c <= COLS; c++) {
        int idx = (c - 1) * 2 + 1;
        if (sim_move(idx, opp_token, 3)) {
            for (int r = ROWS - 1; r >= 0; r--) {
                if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
            }
            printf("Computer chose column %d\n", c);
            return 0;
        }
    }

    // --- Priority 5: Pick Center (Ordering Rule) ---
    // We scan columns from the center outwards: 4, 3, 5, 2, 6...
    // Center column index
    int center = (COLS + 1) / 2;

    // Check Center First
    int idx = (center - 1) * 2 + 1;
    if (board[0][idx] == EMPTY) { // If top row is empty, column is valid
         for (int r = ROWS - 1; r >= 0; r--) {
             if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
         }
         printf("Computer chose column %d\n", center);
         return 0;
    }

    // Expand outwards (offset 1, 2, 3...)
    for (int dist = 1; dist < COLS; dist++) {
        // Left side first (center - dist)
        int left = center - dist;
        if (left >= 1) {
            idx = (left - 1) * 2 + 1;
            if (board[0][idx] == EMPTY) {
                for (int r = ROWS - 1; r >= 0; r--) {
                    if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
                }
                printf("Computer chose column %d\n", left);
                return 0;
            }
        }
        // Right side second (center + dist)
        int right = center + dist;
        if (right <= COLS) {
            idx = (right - 1) * 2 + 1;
            if (board[0][idx] == EMPTY) {
                for (int r = ROWS - 1; r >= 0; r--) {
                    if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
                }
                printf("Computer chose column %d\n", right);
                return 0;
            }
        }
    }
    return 0;




}
////////////////////////////////////////
///



///////////////////////////////////////
///    Human Logic
void human_play(int round) {
    int choice_column = 0;
    int col_index;

    while (1) {
        printf("Enter column (1-%d): ", COLS);
        if (scanf("%d", &choice_column) != 1) {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n'); // flush buffer
            continue;
        }
        while (getchar() != '\n'); // flush buffer

        if (choice_column < 1 || choice_column > COLS) {
            printf("Invalid column. Choose between 1 and %d.\n", COLS);
            continue;
        }

        col_index = (choice_column - 1) * 2 + 1; // map to board
        if (board[0][col_index] != EMPTY) {
            printf("Column %d is full. Choose another.\n", choice_column);
            continue;
        }

        break; // valid choice
    }

    // Place token
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col_index] == EMPTY) {
            if (round % 2 == 0)
                board[i][col_index] = TOKEN_P1;
            else
                board[i][col_index] = TOKEN_P2;
            break;
        }
    }
}

/////////////////////////////////////////
///
int victory_checker() {
    //1. Horizontal Check (-)
    int counter=0 ;
for (int row = 0; row < ROWS; row++) {
    counter=0;//cleaning the counter
for (int col = 1; col < COLS*2+1; col=col+2) {
if (board[row][col] == TOKEN_P1) {
    counter++;
    if (counter == 4) {
        return HUMAN;
    }
    if ((board[row][col+2] == TOKEN_P2)||(board[row][col+2] == EMPTY)) {
        counter=0;
    }
}

}


}
    printf("\n");

    for (int row = 0; row < ROWS; row++) {
        counter=0 ; //cleaning the counter
        for (int col = 1; col < COLS*2+1; col=col+2) {
            if (board[row][col] == TOKEN_P2) {
                counter++;
                if (counter == 4) {
                    return COMPUTER ;
                }
                if ((board[row][col+2] == TOKEN_P1)||(board[row][col+2] == EMPTY)) {
                    counter=0;
                }
            }

        }


    }
    //3. Diagonal Down-Right (\)
    int col=1 ;
    counter=0 ;
    for (int row2 = 0; row2 < ROWS; row2++) {
        for (int row = row2; row < ROWS; row++) {
            if  (board[row][col]==TOKEN_P1) {
                counter++;
            }else {
                counter=0;
            }
            if (counter == 4) {
                return HUMAN;
            }
            col=col+2 ;
        }
        counter=0;
        col=1 ;
    }
col=1;
    counter=0 ;
    for (int row2 = 0; row2 < ROWS; row2++) {
        for (int row = row2; row < ROWS; row++) {
            if  (board[row][col]==TOKEN_P2) {
                counter++;
            }else {
                counter=0;
            }
            if (counter == 4) {
                return COMPUTER;
            }
            col=col+2 ;
        }
        counter=0;
        col=1 ;
    }

/////4. Diagonal Down-Left (/)
    counter=0 ;
    col=COLS*2-1;
    for (int row2 = 0; row2 < ROWS; row2++) {
        for (int row = row2; row < ROWS; row++) {
            if  (board[row][col]==TOKEN_P1) {
                counter++;
            } else {
                counter=0;
            }
            if (counter == 4) {
                return HUMAN;
            }
            col=col-2 ;
        }
        counter=0;
        col=COLS*2-1 ;
    }
    col=COLS*2-1;
    counter=0 ;
    for (int row2 = 0; row2 < ROWS; row2++) {
        for (int row =row2; row < ROWS; row++) {
            if  (board[row][col]==TOKEN_P2) {
                counter++;
            }else {
                counter=0;
            }
            if (counter == 4) {
                return COMPUTER;
            }
            col=col-2 ;
        }
        counter=0;
        col=COLS*2-1 ;
    }
///2. Vertical Check (|)
counter=0;
for (col = 1; col < COLS*2+1; col=col+2) {
  for (int row2 = 0; row2 < ROWS; row2++) {
      if  (board[row2][col]==TOKEN_P1) {
          counter++;
      }
      if (counter == 4) {
          return HUMAN;
      }
      if  ((board[row2+1][col] == TOKEN_P2)||(board[row2][col] == EMPTY)) {
          counter=0;
      }

  }

}

    counter=0;
    for (col = 1; col < COLS*2+1; col=col+2) {
        for (int row2 = 0; row2 < ROWS; row2++) {
            if  (board[row2][col]==TOKEN_P2) {
                counter++;
            }
            if (counter == 4) {
                return COMPUTER;
            }
            if  ((board[row2+1][col] == TOKEN_P1)||(board[row2][col] == EMPTY)) {
                counter=0;
            }

        }

    }



}






int main() {
    GameBody();
    player1=get_player_type(1);
    player2=get_player_type(2);
    design_board(board);
    print_board();
    printf("\n") ;


    for (int round=0;round<ROWS*COLS+1;round++) {
if (round==final_round) {
    printf("Board full and no winner. It's a tie!");
    break;
}

        current_player_type = (round % 2 == 0) ? player1 : player2;
                              (round % 2 == 0) ? printf("Player 1 (X) turn.\n") : printf("Player 2 (O) turn.\n");
        if (current_player_type == COMPUTER) {
            ai_play();
        } else {
            human_play(round);
        }
        print_board();
        printf("\n");

        if (victory_checker()==1) {
            printf("Player 1 (X) wins!");
            break ;
        }
        if (victory_checker()==2) {
            printf("Player 2 (O) wins!");
            break;
        }
        printf("\n") ;


    }


}