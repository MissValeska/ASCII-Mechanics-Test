#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <termios.h>

void user_movement(void);
void monster_AI(void);

int i = 0;
int d = 0;
int turn = 0;

struct termios old_tio, new_tio;

struct chr_pos {
    int hp;
    int z;
    int y;
};

void pos_filler(struct chr_pos *tmp, int type) {
    if(type == 0) {
        tmp->hp = 20;
        tmp->z = 5;
        tmp->y = 10;
    }
    if(type == 1) {
        tmp->hp = rand() % 35;
        tmp->z = rand() % 10;
        tmp->y = rand() % 20;
    }
}

struct chr_pos character;
struct chr_pos monster;

int main(void) {
    
    srand((unsigned int)time((time_t*)0));
    
    pos_filler(&character, 0);
    pos_filler(&monster, 1);
    
    tcgetattr(STDIN_FILENO,&old_tio);
    new_tio=old_tio;
    
    new_tio.c_lflag &=(~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
    
    while(1) {
        printf("|");
        i++;
        if(d == character.z && i == character.y) {
            printf("@");
            if(i < 20) {
                i++;
            }
        }
        if(d == monster.z && i == monster.y) {
            printf("M");
            if(i < 20) {
                i++;
            }
        }
        if(i == 20) {
            printf("\n");
            d++;
            i = 0;
            if(d == 10) {
                d = 0;
                turn++;
                
                if(character.z == monster.z && character.y == monster.y) {
                    if(rand() % 2 == 0) {
                        character.hp -= rand() % 5;
                    }
                    if(rand() % 2 == 0) {
                        monster.hp -= rand() % 5;
                    }
                }
                if(character.hp < 0) {
                    character.hp = 0;
                }
                if(monster.hp < 0) {
                    monster.hp = 0;
                }
                
                printf("turn: %i character hp: %i monster hp: %i\n", turn, character.hp, monster.hp);
                user_movement();
                monster_AI();
                system("clear");
            }
        }
    }
}

void user_movement(void) {
    char request;
    request = getchar();
    fflush(stdin);
    
    if(request == 'w') {
        character.z--;
    }
    
    if(request == 'a') {
        character.y--;
    }
    
    if(request == 's') {
        character.z++;
    }
    
    if(request == 'd') {
        character.y++;
    }
    if(request == 'q') {
        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
        exit(0);
    }
}

void monster_AI(void) {
    
    int seek_z;
    int seek_y;
    int z_dir;
    int y_dir;
    
    /* Difference in Position between character and monster */
    if(monster.z > character.z) {
        z_dir = 0;
        seek_z = monster.z - character.z;
    }
    if(monster.z < character.z) {
        z_dir = 1;
        seek_z = character.z - monster.z;
    }
    if(monster.z == character.z) {
        z_dir = 2;
    }
    if(monster.y > character.y) {
        y_dir = 0;
        seek_y = monster.y - character.y;
    }
    if(monster.y < character.y) {
        y_dir = 1;
        seek_y = character.y - monster.y;
    }
    if(monster.y == character.y) {
        y_dir = 2;
    }
    
    /* Moving monster to character */
    
    if(z_dir == 0) {
        monster.z--;
        seek_z--;
    }
    if(z_dir == 1) {
        monster.z++;
        seek_z--;
    }
    if(y_dir == 0) {
        monster.y--;
        seek_y--;
    }
    if(y_dir == 1) {
        monster.y++;
        seek_y--;
    }
    
    if(rand() % 2 == 0) {
    /* Random movement */
    
        int x = rand() % 4;
    
        if(x == 0) {
            monster.z--;
        }
    
        if(x == 1) {
            monster.y--;
        }
    
        if(x == 2) {
            monster.z++;
        }
    
        if(x == 3) {
            monster.y++;
        }
    }
}