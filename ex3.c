/* Name: ahmad abu bkr
ID: 213109192
   Assignment: ex3*/
#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

#define BORDER '|'

int board[ROWS][COLS * 2 + 1];
int current_player_type;
int player1, player2;
int final_round = ROWS * COLS;

void design_board(int board[ROWS][COLS * 2 + 1]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS * 2 + 1; col++) {
            board[row][col] = BORDER;
            board[row][col + 1] = EMPTY;
            col++;
        }
    }
}

void print_board() {
    printf("\n");
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS * 2 + 1; col++) {
            printf("%c", board[row][col]);
        }
        printf("\n");
    }
    for (int col = 0; col < COLS; col++) {
        printf(" %d", (col + 1) % 10);
    }
    printf("\n\n");
}

int get_player_type(int player_number) {
    char playertype;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", player_number);
        int scanned = scanf(" %c", &playertype);
        if (scanned != 1) {
             while (getchar() != '\n');
             continue;
        }
        if (playertype == 'c' || playertype == 'C') return COMPUTER;
        if (playertype == 'h' || playertype == 'H') return HUMAN;
        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n');
    }
}

void GameBody() {
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
}

int sim_move(int col_idx, int token, int target_len) {
    int r = -1;
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col_idx] == EMPTY) { r = i; break; }
    }
    if (r == -1) return 0;

    board[r][col_idx] = token;

    int dr[] = {0, 1, 1, 1};
    int dc[] = {2, 0, 2, -2};

    for (int d = 0; d < 4; d++) {
        int count = 1;
        for (int i = 1; i < target_len; i++) {
            int nr = r + dr[d] * i;
            int nc = col_idx + dc[d] * i;
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS*2+1 && board[nr][nc] == token) count++;
            else break;
        }
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

int ai_play(int round) {
    int my_token = (round % 2 == 0) ? TOKEN_P1 : TOKEN_P2;
    int opp_token = (round % 2 == 0) ? TOKEN_P2 : TOKEN_P1;

    int order[COLS];
    for(int i=0; i<COLS; i++) order[i] = i+1;

    for(int i=0; i<COLS-1; i++) {
        for(int j=0; j<COLS-i-1; j++) {
             int c1 = order[j];
             int c2 = order[j+1];
             int d1 = (2*c1 - (COLS+1)); if (d1 < 0) d1 = -d1;
             int d2 = (2*c2 - (COLS+1)); if (d2 < 0) d2 = -d2;
             if (d1 > d2 || (d1 == d2 && c1 > c2)) {
                 int temp = order[j];
                 order[j] = order[j+1];
                 order[j+1] = temp;
             }
        }
    }

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

void human_play(int round) {
    int choice_column = 0;
    int col_index;

    while (1) {
        printf("Enter column (1-%d): ", COLS);
        if (scanf("%d", &choice_column) != 1) {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (choice_column < 1 || choice_column > COLS) {
            printf("Invalid column. Choose between 1 and %d.\n", COLS);
            continue;
        }
        col_index = (choice_column - 1) * 2 + 1;
        if (board[0][col_index] != EMPTY) {
            printf("Column %d is full. Choose another column.\n", choice_column);
            continue;
        }
        break;
    }

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

int victory_checker() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 1; c < COLS * 2 + 1; c += 2) {
            int token = board[r][c];
            if (token == EMPTY) continue;
            if (c + 6 < COLS * 2 + 1 &&
                board[r][c+2] == token &&
                board[r][c+4] == token &&
                board[r][c+6] == token) {
                return (token == TOKEN_P1) ? HUMAN : COMPUTER;
            }
        }
    }

    for (int r = 0; r <= ROWS - 4; r++) {
        for (int c = 1; c < COLS * 2 + 1; c += 2) {
            int token = board[r][c];
            if (token == EMPTY) continue;
            if (board[r+1][c] == token &&
                board[r+2][c] == token &&
                board[r+3][c] == token) {
                return (token == TOKEN_P1) ? HUMAN : COMPUTER;
            }
        }
    }

    for (int r = 0; r <= ROWS - 4; r++) {
        for (int c = 1; c < COLS * 2 + 1; c += 2) {
            int token = board[r][c];
            if (token == EMPTY) continue;
            if (c + 6 < COLS * 2 + 1 &&
                board[r+1][c+2] == token &&
                board[r+2][c+4] == token &&
                board[r+3][c+6] == token) {
                return (token == TOKEN_P1) ? HUMAN : COMPUTER;
            }
        }
    }

    for (int r = 0; r <= ROWS - 4; r++) {
        for (int c = 1; c < COLS * 2 + 1; c += 2) {
            int token = board[r][c];
            if (token == EMPTY) continue;
            if (c - 6 >= 0 &&
                board[r+1][c-2] == token &&
                board[r+2][c-4] == token &&
                board[r+3][c-6] == token) {
                return (token == TOKEN_P1) ? HUMAN : COMPUTER;
            }
        }
    }
    return 0;
}

int main() {
    GameBody();
    player1 = get_player_type(1);
    player2 = get_player_type(2);
    design_board(board);
    print_board();

    for (int round = 0; round < ROWS * COLS; round++) {
        current_player_type = (round % 2 == 0) ? player1 : player2;

        if(round % 2 == 0) printf("Player 1 (X) turn.\n");
        else printf("Player 2 (O) turn.\n");

        if (current_player_type == COMPUTER) {
            ai_play(round);
        } else {
            human_play(round);
        }
        print_board();

        int winner = victory_checker();
        if (winner == HUMAN) {
            printf("Player 1 (X) wins!\n");
            return 0;
        }
        if (winner == COMPUTER) {
            printf("Player 2 (O) wins!\n");
            return 0;
        }
    }

    printf("Board full and no winner. It's a tie!\n");
    return 0;
}