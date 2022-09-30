#include <stdio.h>
#include <stdlib.h>
#include <math.h> //abs
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "module.h"

using namespace std;

int main()
{
    // inicjalizacja allegro
    int window_x = 1200, window_y = 960;
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();

    // zmienne allegro
    ALLEGRO_EVENT event;
    ALLEGRO_DISPLAY* display = al_create_display(window_x, window_y);
    ALLEGRO_EVENT_QUEUE* ev_queue = al_create_event_queue();
    ALLEGRO_FONT* machine_font = al_load_ttf_font("Hack-Regular.ttf", 32,0);
    ALLEGRO_FONT* title_font = al_load_ttf_font("LittleLordFontleroyNF.ttf", 90,0);
    ALLEGRO_FONT* title_small_font = al_load_ttf_font("LittleLordFontleroyNF.ttf", 60,0);
    ALLEGRO_TIMER* timer = al_create_timer(1.0/60);
    ALLEGRO_BITMAP * light_p = NULL;
    ALLEGRO_BITMAP * dark_p = NULL;
    ALLEGRO_BITMAP * light_q = NULL;
    ALLEGRO_BITMAP * dark_q = NULL;
    ALLEGRO_COLOR dark = al_map_rgb(102, 0, 0);
    ALLEGRO_COLOR light = al_map_rgb(255, 255, 153);
    ALLEGRO_COLOR background = al_map_rgb(0, 102, 0);
    ALLEGRO_COLOR gold = al_map_rgb(179, 179, 0);
    ALLEGRO_COLOR new_pos = al_map_rgb(0, 255, 0);
    ALLEGRO_COLOR sel_pos = al_map_rgb(255, 102, 0);

    // sterowniki i zdarzenia
    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(ev_queue, al_get_keyboard_event_source());
    al_register_event_source(ev_queue, al_get_mouse_event_source());
    al_register_event_source(ev_queue, al_get_display_event_source(display));
    al_register_event_source(ev_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    //grafiki
    light_p = al_load_bitmap("light.png");
    assert(light_p != NULL);
    light_q = al_load_bitmap("light_queen.png");
    assert(light_q != NULL);
    dark_p = al_load_bitmap("dark.png");
    assert(dark_p != NULL);
    dark_q = al_load_bitmap("dark_queen.png");
    assert(dark_q != NULL);

    // zmienne główne
    struct Pion pawns[24];
    int board[10][10];
    int player1 = 12;
    int player_1 = 12;

    // inne zmienne
    bool running = true; // kontrola pracy gry
    int slot_size = 90; // rozmiar pola szachownicy
    int start_x=window_x/2-4*slot_size, start_y=120; // wpółrzędne początku szachownicy
    float m_x = 0, m_y = 0; // współrzędne myszy
    int counter = 0; // licznik do usuwania pionków, które nie wykonały bicia
    int round = 1; // runda, wskazanie gracza
    int sel_x = -1, sel_y = -1; // wybrane pole
    int move_x = -1, move_y =-1; // pole docelowego ruchu
    int dir_x,  dir_y; // kierunek przemieszczenia damki
    int fin = 0, fin_inner = 0; // zmienne kończące wewnętrzne pętle, fin dla pojedynczej rundy, inner dla wielokrotnego bicia
    int id; // indeks przesuwanego pionka
    int cont = 0; // wykonywanie ponownego bicia
    int game_state = 0; // stan gry
    int tmp; // tymczasowa, głównie do wyznaczania docelowego pola przy biciu damką.
    FILE *registry; // plik logu

    registry = fopen("log.txt", "a");
    fputs("\n-------------------\n", registry);
    fputs("NEW GAME\n", registry);
    fputs("-------------------\n", registry);

    startup(&player1, &player_1, board, pawns);

    while(running)
    {
        al_clear_to_color(background);
        al_draw_text(title_font, gold, window_x/2-100, 10, 0, "Checker");
        switch(game_state) // rozpoznawanie stanu gry
        {
            case 0:
            {
                fprintf(registry, "Round of Player %d\n",round > 0 ? 1: 2);
                fin = 0;
                while(!fin)
                {

                    fin_inner = 0;
                    if(can_i_move(round, pawns, board) == 0) // zakończenie w wyniku braku ruchu
                    {
                        fprintf(registry, "You cannot make a move.\n");
                        fprintf(registry, "Winner: Player %d", round < 0 ? 1:2);
                        round*=-1;
                        fin = 1;
                        game_state = 1;
                    }
                    else{

                        al_wait_for_event(ev_queue, &event);
                        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // zamkięcie okna
                        {
                            fin = 1;
                            running = false;
                        }
                        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) // odczyt stanu myszy
                        {
                            m_x = event.mouse.x;
                            m_y = event.mouse.y;
                            if(sel_x == -1) // wybór pionka
                            {
                                if(m_x > start_x && m_x < start_x+8*slot_size)
                                {
                                    sel_x = (m_x-start_x)/slot_size;
                                }
                                else{
                                    sel_x = -1;
                                }
                                if (m_y > start_y && m_y < start_y+8*slot_size)
                                    {sel_y = (m_y - start_y)/slot_size;}
                                else{
                                    sel_y = -1;
                                }
                            }
                            else{ // wybór pola docelowego
                                if(m_x > start_x && m_x < start_x+8*slot_size)
                                {move_x = (m_x-start_x)/slot_size;}
                                else{
                                    move_x = -1;
                                }
                                if (m_y > start_y && m_y < start_y+8*slot_size)
                                    {move_y = (m_y - start_y)/slot_size;}
                                else{
                                    move_y = -1;
                                }
                            }
                            if(cont == 1) // zakończenie rundy
                            {
                                if(m_x > window_x - 200 && m_x < window_x - 10)
                                {
                                    if(m_y > window_y/2 && m_y < window_y/2+90)
                                    {
                                        cont = 0;
                                        fin = 1;
                                        fin_inner = 1;
                                        counter = 0;
                                    }
                                }
                            }
                            if(m_x > window_x - 200 && m_x < window_x - 10)
                            {
                                if(m_y > window_y/2-200 && m_y < window_y/2-110)
                                {
                                    fin = 1;
                                    fin_inner = 1;
                                    game_state = 3;
                                    break; // surrender
                                }
                            }
                            if(m_x > window_x - 200 && m_x < window_x - 10)
                            {
                                if(m_y > window_y/2 -100&& m_y < window_y/2-10)
                                {
                                    fin = 1;
                                    fin_inner = 1;
                                    game_state = 2;
                                    break;//tie
                                }
                            }


                        }
                        if(event.type == ALLEGRO_EVENT_TIMER)
                        {
                            // rysowanie szachownicy i napisów
                            al_clear_to_color(background);
                            al_draw_text(title_font, gold, window_x/2-100, 10, 0, "Checker");
                            al_draw_text(title_small_font, gold, window_x - 200, start_y, 0, "Player:");
                            if(round > 0)
                            {
                                al_draw_text(title_small_font, gold, window_x - 200, start_y+70, 0, "White");
                            }
                            else{
                                al_draw_text(title_small_font, gold, window_x - 200, start_y+70, 0, "Black");
                            }
                            for(int i = 1; i < 9; i+=1)
                            {
                                for(int j = 1; j < 9; j+=1)
                                {
                                    if((i+j)%2 == 0)
                                    {
                                        al_draw_filled_rectangle(start_x+slot_size*(i-1),start_y+slot_size*(j-1),start_x+slot_size*i, start_y+slot_size*j, dark);
                                    }
                                    else{
                                        al_draw_filled_rectangle(start_x+slot_size*(i-1),start_y+slot_size*(j-1),start_x+slot_size*i, start_y+slot_size*j, light);
                                    }
                                    if(board[j][i] <100)
                                    {
                                        if(pawns[board[j][i]+12].player == 1)
                                        {
                                            if(pawns[board[j][i]+12].state == 1)
                                            {
                                                al_draw_bitmap(light_p, start_x+slot_size*(i-1),start_y+slot_size*(j-1), 0);
                                            }
                                            else{
                                                al_draw_bitmap(light_q, start_x+slot_size*(i-1),start_y+slot_size*(j-1), 0);
                                            }
                                        }
                                        else{
                                            if(pawns[board[j][i]+12].state == 1)
                                            {
                                                al_draw_bitmap(dark_p, start_x+slot_size*(i-1),start_y+slot_size*(j-1), 0);
                                            }
                                            else{
                                                al_draw_bitmap(dark_q, start_x+slot_size*(i-1),start_y+slot_size*(j-1), 0);
                                            }
                                        }
                                    }

                                    // przyciski
                                    al_draw_filled_rectangle(window_x - 200, start_y+200, window_x - 10, start_y+290, gold);
                                    al_draw_text(machine_font, dark, window_x - 190, start_y+230, 0, "Surrender");
                                    al_draw_filled_rectangle(window_x - 200, start_y+300, window_x - 10, start_y+390, gold);
                                    al_draw_text(machine_font, dark, window_x - 150, start_y+330, 0, "Tie");
                                    if(cont == 1)
                                    {
                                        al_draw_filled_rectangle(window_x - 200, start_y+400, window_x - 10, start_y+490, gold);
                                        al_draw_text(machine_font, dark, window_x - 190, start_y+430, 0, "END ROUND");
                                    }

                                    // zaznaczenie wybranego piona
                                    if(sel_x != -1 && sel_y != -1)
                                    {
                                        if (pawns[board[sel_y+1][sel_x+1]+12].player == round)
                                        {
                                            al_draw_rectangle(start_x+slot_size*sel_x, start_y+slot_size*sel_y, start_x+slot_size*(sel_x+1), start_y+slot_size*(sel_y+1), sel_pos, 4);
                                            id = board[sel_y+1][sel_x+1]+12;
                                        }
                                        else{
                                            sel_x = -1; sel_y = -1;
                                        }
                                    }

                                    if(move_x != -1 && move_y != -1)
                                    {
                                        if(cont == 0 && pawns[board[move_y+1][move_x+1]+12].player == round) // zmiana wybranego piona
                                        {
                                            sel_x = move_x; sel_y = move_y;
                                            id = board[sel_y+1][sel_x+1]+12;
                                            move_x = -1; move_y = -1;
                                        }
                                        else{
                                            al_draw_rectangle(start_x+slot_size*move_x, start_y+slot_size*move_y, start_x+slot_size*(move_x+1), start_y+slot_size*(move_y+1), new_pos, 4);
                                            if(!fin_inner)
                                            {
                                                if(cont == 1) // gdy kontynuacja bicia
                                                {
                                                    attac_clear(pawns);
                                                    forced_attacks(board[move_y][move_x]+12, pawns, board);
                                                }
                                                if(pawns[id].state == 1) // pion zwykły
                                                {
                                                   if (board[move_y+1][move_x+1] == 100) // przesunięcie
                                                    {
                                                        if(cont == 0 && abs(sel_x-move_x) == 1 && sel_y - move_y == round)
                                                        {
                                                            board[move_y+1][move_x+1] = board[sel_y+1][sel_x+1];
                                                            board[sel_y+1][sel_x+1] = 100;
                                                            pawns[id].x = move_x+1;
                                                            pawns[id].y = move_y+1;

                                                            fprintf(registry, "Player %d: move pawn: %d %d to: %d %d\n", round>0?1:2, sel_x+1,sel_y+1,move_x+1,move_y+1);
                                                            sel_x = move_x;
                                                            sel_y = move_y;
                                                            move_x = move_y = -1;
                                                            fin = 1;
                                                            fin_inner = 1;
                                                        }
                                                        else if(abs(sel_x-move_x) == 2 && abs(sel_y - move_y) == 2 &&
                                                                pawns[board[abs((sel_y + move_y+2)/2)][abs((sel_x + move_x+2)/2)]+12].player == round*(-1)) // bicie
                                                        {
                                                            board[move_y+1][move_x+1] = board[sel_y+1][sel_x+1];
                                                            board[sel_y+1][sel_x+1] = 100;
                                                            pawns[id].x = move_x+1;
                                                            pawns[id].y = move_y+1;
                                                            pawns[id].attack = 0;
                                                            id = board[abs((sel_y + move_y+2)/2)][abs((sel_x + move_x+2)/2)]+12;
                                                            fprintf(registry, "Player %d: move pawn: %d %d to: %d %d\n", round>0?1:2, sel_x+1,sel_y+1,move_x+1,move_y+1);
                                                            fprintf(registry, "Pawn: %d %d has been taken.\n", abs((sel_x + move_x)/2)+1, abs((sel_y + move_y)/2)+1);

                                                            pawns[id].x= -1;
                                                            pawns[id].y= -1;
                                                            pawns[id].state= -1;
                                                            board[abs((sel_y + move_y+2)/2)][abs((sel_x + move_x+2)/2)] = 100;

                                                            if(round == 1)// odjęcie zbitych pionów, kotrola końca gry
                                                            {
                                                                player_1 -=1;
                                                                printf("White: You've taken the pawn. %d pawns left.\n", player_1);
                                                                if(player_1 == 0)
                                                                {
                                                                    fputs("Winner: Player 1", registry);
                                                                    game_state = 1;
                                                                    fin = 1;
                                                                    fin_inner = 1;
                                                                }
                                                            }
                                                            else{
                                                                player1 -=1;
                                                                printf("Black: You've taken the pawn. %d pawns left.\n", player1);
                                                                if(player1 == 0)
                                                                {
                                                                    fputs("Winner: Player 2", registry);
                                                                    game_state = 1;
                                                                    fin = 1;
                                                                    fin_inner = 1;
                                                                }
                                                            }
                                                            attac_clear(pawns);
                                                            sel_x = move_x;
                                                            sel_y = move_y;
                                                            move_x = move_y = -1;
                                                            cont = 1;
                                                        }

                                                    }

                                                }
                                                else if(pawns[id].state == 2) // damka
                                                {
                                                    if(board[move_y+1][move_x+1] == 100)
                                                    {
                                                        tmp = abs(sel_x - move_x);
                                                        dir_x = (sel_x - move_x)/tmp;
                                                        tmp = abs(sel_y - move_y);
                                                        dir_y = (sel_y - move_y)/tmp;//kierunek ruchu
                                                        if(tmp -abs(sel_x-move_x) == 0)//czy diagonala
                                                        {
                                                            int m = 1;
                                                            while(m < tmp && board[sel_y+1-m*dir_y][sel_x+1-m*dir_x] == 100)
                                                            {
                                                                m+=1;
                                                            }//przesun dopoki puste pola
                                                            if(cont == 0 && m == tmp)//jezeli !kontynuacja bicia i pole docelowe
                                                            {
                                                                board[move_y+1][move_x+1] = board[sel_y+1][sel_x+1];//przestaw
                                                                board[sel_y+1][sel_x+1] = 100;
                                                                pawns[id].x = move_x+1;
                                                                pawns[id].y = move_y+1;
                                                                fprintf(registry, "Player %d: move pawn: %d %d to: %d %d\n", round>0?1:2, sel_x+1,sel_y+1,move_x+1,move_y+1);
                                                                sel_x = move_x;
                                                                sel_y = move_y;
                                                                move_x = move_y = -1;

                                                                fin = 1;
                                                                fin_inner = 1;
                                                            }
                                                            else{//bicie
                                                                int new_id = board[sel_y+1-m*dir_y][sel_x+1-m*dir_x]+12;//pierwsze zajete pole
                                                                if(pawns[new_id].player == round*(-1))//jezeli jest tam pionek przeciwnika
                                                                {
                                                                    new_id = m;//new_id = ile nalezy przesunac
                                                                    m+=1;
                                                                    while(m < tmp && board[sel_y+1-m*dir_y][sel_x+1-m*dir_x] == 100)//przesuwaj dopoki pola sa wolne i nie osiagnieto celu
                                                                    {
                                                                        m+=1;
                                                                    }
                                                                    if(m == tmp)//jezeli osiagnieto cel
                                                                    {
                                                                        board[move_y+1][move_x+1] = board[sel_y+1][sel_x+1];//przestaw
                                                                        board[sel_y+1][sel_x+1] = 100;
                                                                        pawns[id].x = move_x+1;
                                                                        pawns[id].y = move_y+1;
                                                                        pawns[id].attack = 0;

                                                                        fprintf(registry, "Player %d: move pawn: %d %d to: %d %d\n", round>0?1:2, sel_x+1,sel_y+1,move_x+1,move_y+1);
                                                                        fprintf(registry, "Pawn: %d %d has been taken.\n", sel_x+1-new_id*dir_x, sel_y+1-new_id*dir_y);

                                                                        tmp = new_id;//usun zbity pionek
                                                                        new_id = board[sel_y+1-new_id*dir_y][sel_x+1-new_id*dir_x]+12;
                                                                        board[sel_y+1-tmp*dir_y][sel_x+1-tmp*dir_x] = 100;
                                                                        pawns[new_id].x= -1;
                                                                        pawns[new_id].y= -1;
                                                                        pawns[new_id].state= -1;

                                                                        if(round == 1)//zmniejsz ilosc pionkow, jezeli wszystkie zbite zakoncz
                                                                        {
                                                                            player_1 -=1;
                                                                            printf("White: You've taken the pawn. %d pawns left.\n", player_1);
                                                                            if(player_1 == 0)
                                                                            {
                                                                                fputs("Winner: Player 1", registry);
                                                                                printf("End of the game. Player %d wins.\nCoded by Karol Wesolowski\n", round);
                                                                                fin = 1;
                                                                                game_state = 1;
                                                                            }
                                                                        }
                                                                        else{
                                                                            player1 -=1;
                                                                            printf("Black: You've taken the pawn. %d pawns left.\n", player1);
                                                                            if(player1 == 0)
                                                                            {
                                                                                fputs("Winner: Player 2", registry);
                                                                                fin = 1;
                                                                                game_state = 1;
                                                                            }
                                                                        }
                                                                        attac_clear(pawns);
                                                                        sel_x = move_x;
                                                                        sel_y = move_y;
                                                                        move_x = move_y = -1;
                                                                        cont = 1;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            al_flip_display();
                        }
                    }
                }
                counter = 0;
                for(int i = 0; i < 24; i+=1)
                {
                    if(pawns[i].attack == 1)// usuwanie pionów, jeśli nie wykonano bicia
                    {
                        printf("Pawn %d, %d lost\n", pawns[i].x,pawns[i].y);
                        board[pawns[i].y][pawns[i].x] = 100;
                        pawns[i].x = -1; pawns[i].y = -1; pawns[i].state = -1; pawns[i].attack = 0;
                        counter+=1;
                    }
                    if(pawns[i].y == (int)(4.5 - round*3.5) && pawns[i].player == round) // ustawianie damek
                    {
                        pawns[i].state = 2;
                    }
                }
                if (counter > 0)
                {
                    printf("Player %d: %d  pawns lost due to not making a forced attack.\n", round>0?1:2, counter);
                    fprintf(registry, "\nPlayer %d: %d  pawns lost due to not making a forced attack.\n", round>0?1:2, counter);
                }
                if(round == 1)
                {
                    player1 -=counter;
                    if(player1 == 0)
                    {
                        fputs("Winner: Player 2", registry);
                        running = false;
                    }
                }
                else{
                    player_1 -=counter;
                    if(player_1 == 0)
                    {
                        fputs("Winner: Player 1", registry);
                        running = false;
                    }
                }
                attac_clear(pawns);
                counter = 0;
                round *=-1;
                fputs("\nRound is finished.\n", registry);
                break;
            }
            case 1:
            {
                al_draw_textf(title_font, gold, window_x/2-400, 500, 0, "End of the game. Player %d wins.", round < 0 ? 1:2);
                break;
            }
            case 2:
            {
                al_draw_textf(title_font, gold, window_x/2-220, 500, 0, "End of the game. Tie.");
                break;
            }
            case 3:
            {
                al_draw_textf(title_font, gold, window_x/2-400, 500, 0, "End of the game. Player %d surrenders.", round < 0 ? 1:2);
                break;
            }
        }

        if(game_state != 0) // wznowienie gry
        {
            m_x = -1;
            m_y = -1;
            al_draw_filled_rectangle(window_x/2-100, window_y/2-100, window_x/2+100, window_y/2, gold);
            al_draw_text(machine_font, dark, window_x/2-95, window_y/2-70, 0, "Play again");
            al_wait_for_event(ev_queue, &event);
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                running = false;
            }
            if(event.type == ALLEGRO_EVENT_TIMER)
            {
                al_flip_display();
            }
            if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                m_x = event.mouse.x; m_y = event.mouse.y;
            }
            if(m_x > window_x/2-100 && m_x < window_x/2+100)
            {
                if(m_y > window_y/2-100 && m_y < window_y/2)
                {
                    startup(&player1, &player_1, board, pawns);
                    game_state = 0;
                    round = 1;
                    counter = 0;
                    sel_x = -1, sel_y = -1;
                    move_x = -1, move_y =-1;
                    fin = 0, fin_inner = 0;
                    cont = 0;
                    printf("restart\n");
                }
            }
        }
    }
    // czyszczenie pamięci
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_destroy_timer(timer);
    al_destroy_bitmap(light_p);
    al_destroy_bitmap(light_q);
    al_destroy_bitmap(dark_q);
    al_destroy_bitmap(dark_p);
    fprintf(registry,"Game has been stopped\nWinner: %s",player1 > player_1 ? "Player 1":(player1 == player_1? "TIE":"Player 2"));
    fclose(registry);

    return 0;
}
