#include <stdio.h>
#ifndef ROWS
#define ROWS 4
#endif

#ifndef COLS
#define COLS 4
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
//////////////////////////////////////
///      The Game design
void GameBody(){
printf("Connect Four (%d rows x %d cols)\n" , ROWS, COLS);
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

    // 3. Check all 4 directions
    int dr[] = {0, 1, 1, 1};
    int dc[] = {2, 0, 2, -2}; // Visual board steps

    for (int d = 0; d < 4; d++) {
        int count = 1;
        // Positive direction
        for (int i = 1; i < target_len; i++) {
            int nr = r + dr[d] * i;
            int nc = col_idx + dc[d] * i;
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS*2+1 && board[nr][nc] == token) count++;
            else break;
        }
        // Negative direction
        for (int i = 1; i < target_len; i++) {
            int nr = r - dr[d] * i;
            int nc = col_idx - dc[d] * i;
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS*2+1 && board[nr][nc] == token) count++;
            else break;
        }
        if (count >= target_len) {
            board[r][col_idx] = EMPTY;
            return 1;
        }
    }

    board[r][col_idx] = EMPTY;
    return 0;
}

////////////////////////////////////////
///  AI Logic
int ai_play(int round) {
    int my_token = (round % 2 == 0) ? TOKEN_P1 : TOKEN_P2;
    int opp_token = (round % 2 == 0) ? TOKEN_P2 : TOKEN_P1;

    // PRE-CALCULATE ORDER (Center -> Outwards)
    int order[COLS];
    for(int i=0; i<COLS; i++) order[i] = i+1;

    // Bubble sort columns by distance from center
    for(int i=0; i<COLS-1; i++) {
        for(int j=0; j<COLS-i-1; j++) {
             int c1 = order[j];
             int c2 = order[j+1];

             // Distance logic
             int d1 = (2*c1 - (COLS+1)); if (d1 < 0) d1 = -d1;
             int d2 = (2*c2 - (COLS+1)); if (d2 < 0) d2 = -d2;

             // If dist is equal, prefer smaller (left) column
             if (d1 > d2 || (d1 == d2 && c1 > c2)) {
                 int temp = order[j];
                 order[j] = order[j+1];
                 order[j+1] = temp;
             }
        }
    }

    // --- Priority 1: Win Now ---
    for (int i = 0; i < COLS; i++) {
        int c = order[i];
        int idx = (c - 1) * 2 + 1;
        if (sim_move(idx, my_token, 4)) {
            for (int r = ROWS - 1; r >= 0; r--) {
                if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
            }
            printf("Computer chose column %d\n", c);
            return 0;
        }
    }

    // --- Priority 2: Block Opponent ---
    for (int i = 0; i < COLS; i++) {
        int c = order[i];
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
    for (int i = 0; i < COLS; i++) {
        int c = order[i];
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
    for (int i = 0; i < COLS; i++) {
        int c = order[i];
        int idx = (c - 1) * 2 + 1;
        if (sim_move(idx, opp_token, 3)) {
            for (int r = ROWS - 1; r >= 0; r--) {
                if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
            }
            printf("Computer chose column %d\n", c);
            return 0;
        }
    }

    // --- Priority 5: Pick First Valid Column (in Center Order) ---
    for (int i = 0; i < COLS; i++) {
        int c = order[i];
        int idx = (c - 1) * 2 + 1;
        if (board[0][idx] == EMPTY) {
            for (int r = ROWS - 1; r >= 0; r--) {
                if (board[r][idx] == EMPTY) { board[r][idx] = my_token; break; }
            }
            printf("Computer chose column %d\n", c);
            return 0;
        }
    }
    return 0;
}
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
    printf("");

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
    // 2. Vertical Check (|)
    for (int r = 0; r <= ROWS - 4; r++) {
        for (int c = 1; c < COLS * 2 + 1; c += 2) {
            int token = board[r][c];
            if (token == EMPTY) continue;
            // Check 3 slots down
            if (board[r+1][c] == token &&
                board[r+2][c] == token &&
                board[r+3][c] == token) {
                return (token == TOKEN_P1) ? HUMAN : COMPUTER;
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
            ai_play(round);
        } else {
            human_play(round);
        }
        print_board();
        printf("\n") ;
        if (victory_checker()==1) {
            printf("Player 1 (X) wins!");
            break ;
        }
        if (victory_checker()==2) {
            printf("Player 2 (O) wins!");
            break;
        }


    }


}