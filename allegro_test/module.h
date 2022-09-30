#if !defined (__MODULE_H)
#define __MODULE_H

struct Pion{
    int x;
    int y;
    int state; // 1 = standard; 2 = queen; -1 = destroyed
    int attack; // 0 = no forced attacks; 1 = there is at least one forced attack
    int player;
};

int forced_attacks(int i, struct Pion pawns[24], int board[10][10]);
void startup(int *player1, int*player_1, int board[10][10], struct Pion pawns[24]);
int posible_move(int i, struct Pion pawns[24], int board[10][10]);
int can_i_move(int round, struct Pion pawns[24], int board[10][10]);
void attac_clear(struct Pion pawns[24]);
#endif
