#include "module.h"

void startup(int *player1, int*player_1, int board[10][10], struct Pion pawns[24])
{
    *player1 = 12;
    *player_1 = 12;
    for(int i = 0; i < 10; i+=1)
    {
        board[0][i] = -100;
        board[9][i] = -100;
        board[i][0] = -100;
        board[i][9] = -100;
    }
    for(int i = 1; i < 9; i+=1)
    {
        for(int j = 1; j < 9; j+=1)
        {
            board[i][j] = 100;
        }
    }
    board[1][1] = -12;
    board[1][3] = -11;
    board[1][5] = -10;
    board[1][7] = -9;
    board[2][2] = -8;
    board[2][4] = -7;
    board[2][6] = -6;
    board[2][8] = -5;
    board[3][1] = -4;
    board[3][3] = -3;
    board[3][5] = -2;
    board[3][7] = -1;

    board[8][8] = 11;
    board[8][6] = 10;
    board[8][4] = 9;
    board[8][2] = 8;
    board[7][7] = 7;
    board[7][5] = 6;
    board[7][3] = 5;
    board[7][1] = 4;
    board[6][8] = 3;
    board[6][6] = 2;
    board[6][4] = 1;
    board[6][2] = 0;


    pawns[20].x = 2; pawns[20].y = 8; pawns[20].state = 1; pawns[20].attack = 0; pawns[20].player = 1;
    pawns[21].x = 4; pawns[21].y = 8; pawns[21].state = 1; pawns[21].attack = 0; pawns[21].player = 1;
    pawns[22].x = 6; pawns[22].y = 8; pawns[22].state = 1; pawns[22].attack = 0; pawns[22].player = 1;
    pawns[23].x = 8; pawns[23].y = 8; pawns[23].state = 1; pawns[23].attack = 0; pawns[23].player = 1;

    pawns[16].x = 1; pawns[16].y = 7; pawns[16].state = 1; pawns[16].attack = 0; pawns[16].player = 1;
    pawns[17].x = 3; pawns[17].y = 7; pawns[17].state = 1; pawns[17].attack = 0; pawns[17].player = 1;
    pawns[18].x = 5; pawns[18].y = 7; pawns[18].state = 1; pawns[18].attack = 0; pawns[18].player = 1;
    pawns[19].x = 7; pawns[19].y = 7; pawns[19].state = 1; pawns[19].attack = 0; pawns[19].player = 1;

    pawns[12].x = 2; pawns[12].y = 6; pawns[12].state = 1; pawns[12].attack = 0; pawns[12].player = 1;
    pawns[13].x = 4; pawns[13].y = 6; pawns[13].state = 1; pawns[13].attack = 0; pawns[13].player = 1;
    pawns[14].x = 6; pawns[14].y = 6; pawns[14].state = 1; pawns[14].attack = 0; pawns[14].player = 1;
    pawns[15].x = 8; pawns[15].y = 6; pawns[15].state = 1; pawns[15].attack = 0; pawns[15].player = 1;


    pawns[0].x = 1; pawns[0].y = 1; pawns[0].state = 1; pawns[0].attack = 0; pawns[0].player = -1;
    pawns[1].x = 3; pawns[1].y = 1; pawns[1].state = 1; pawns[1].attack = 0; pawns[1].player = -1;
    pawns[2].x = 5; pawns[2].y = 1; pawns[2].state = 1; pawns[2].attack = 0; pawns[2].player = -1;
    pawns[3].x = 7; pawns[3].y = 1; pawns[3].state = 1; pawns[3].attack = 0; pawns[3].player = -1;

    pawns[4].x = 2; pawns[4].y = 2; pawns[4].state = 1; pawns[4].attack = 0; pawns[4].player = -1;
    pawns[5].x = 4; pawns[5].y = 2; pawns[5].state = 1; pawns[5].attack = 0; pawns[5].player = -1;
    pawns[6].x = 6; pawns[6].y = 2; pawns[6].state = 1; pawns[6].attack = 0; pawns[6].player = -1;
    pawns[7].x = 8; pawns[7].y = 2; pawns[7].state = 1; pawns[7].attack = 0; pawns[7].player = -1;

    pawns[8].x = 1; pawns[8].y = 3; pawns[8].state = 1; pawns[8].attack = 0; pawns[8].player = -1;
    pawns[9].x = 3; pawns[9].y = 3; pawns[9].state = 1; pawns[9].attack = 0; pawns[9].player = -1;
    pawns[10].x = 5; pawns[10].y = 3; pawns[10].state = 1; pawns[10].attack = 0; pawns[10].player = -1;
    pawns[11].x = 7; pawns[11].y = 3; pawns[11].state = 1; pawns[11].attack = 0; pawns[11].player = -1;

}

int forced_attacks(int i, struct Pion pawns[24], int board[10][10])
{
    int x = pawns[i].x; int y = pawns[i].y;
    int player = pawns[i].player;

        if(pawns[i].state == 1)
        {
            if(pawns[board[y+1][x+1]+12].player == player*(-1))
            {
                if(board[y+2][x+2] == 100)
                {
                    pawns[i].attack = 1;
                }
            }
            if(pawns[board[y-1][x+1]+12].player == player*(-1))
            {
                if(board[y-2][x+2] == 100)
                {
                    pawns[i].attack = 1;
                }
            }
            if(pawns[board[y-1][x-1]+12].player == player*(-1))
            {
                if(board[y-2][x-2] == 100)
                {
                    pawns[i].attack = 1;
                }
            }
            if(pawns[board[y+1][x-1]+12].player == player*(-1))
            {
                if(board[y+2][x-2] == 100)
                {
                    pawns[i].attack = 1;
                }
            }
        }
        else if(pawns[i].state == 2)
        {
            int j = 1;
            while(board[y+j][x+j] == 100)
            {
                j+=1;
            }
            if (pawns[board[y+j][x+j]+12].player == player*(-1))
            {
                if (board[y+j+1][x+j+1] == 100)
                {
                    pawns[i].attack = 1;
                }
            }
            j = 1;
            while(board[y+j][x-j] == 100 )
            {
                j+=1;
            }
            if (pawns[board[y+j][x-j]+12].player == player*(-1))
            {
                if (board[y+j+1][x-j-1] == 100)
                {
                    pawns[i].attack = 1;
                }
            }
            j = 1;
            while(board[y-j][x+j] == 100)
            {
                j+=1;
            }
            if (board[y-j][x+j] != -100)
            {
                if (pawns[board[y-j][x+j]+12].player == player*(-1))
                {
                    if (board[y-j-1][x+j+1] == 100)
                    {
                        pawns[i].attack = 1;
                    }
                }
            }

            j = 1;
            while(board[y-j][x-j] == 100)
            {
                j+=1;
            }
            if(board[y-j][x-j] != -100)
            {
                if (pawns[board[y-j][x-j]+12].player == player*(-1))
                {
                    if (board[y-j-1][x-j-1] == 100)
                    {
                        pawns[i].attack = 1;
                    }
                }
            }

        }
    return pawns[i].attack;
}

int posible_move(int i, struct Pion pawns[24], int board[10][10])
{
    int x = pawns[i].x;
    int y = pawns[i].y;
    int r = pawns[i].player;
    if (pawns[i].state == 1)
    {
        if (board[y-r][x+1] == 100)
        {
            return 1;
        }
        if(board[y-r][x-1] == 100)
        {
            return 1;
        }

    }
    else if(pawns[i].state == 2)
    {
        if(board[y+1][x+1] == 100)
        {
            return 1;
        }
        if(board[y+1][x-1] == 100)
        {
            return 1;
        }
        if(board[y-1][x+1] == 100)
        {
            return 1;
        }
        if(board[y-1][x-1] == 100)
        {
            return 1;
        }
    }
    return 0;
}

int can_i_move(int round, struct Pion pawns[24], int board[10][10])
{
    int av = 0;
    int tmp;
    if(round>0)
    {
        for(int i = 12 ; i< 24; i +=1)
        {
            tmp = forced_attacks(i, pawns, board);
            av = av || tmp;
        }
    }
    else{
        for(int i = 0 ; i< 12; i +=1)
        {
            tmp = forced_attacks(i, pawns, board);
            av = av || tmp;
        }
    }
    if(av == 0)
    {
        if(round>0)
        {
            for(int i = 12 ; i< 24; i +=1)
            {
                av = av || posible_move(i, pawns, board);
            }
        }
        else{
            for(int i = 0 ; i< 12; i +=1)
            {
                av = av || posible_move(i, pawns, board);
            }
        }
    }
    return av;
}

void attac_clear(struct Pion pawns[24])
{
    for(int i = 0; i < 24; i+=1)
    {
        pawns[i].attack = 0;
    }
}
