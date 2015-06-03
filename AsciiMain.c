#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <termios.h>

void user_movement(void);
void monster_AI(void);
void weather_sim(int season_change);

int i = 0;
int d = 0;
int turn = 0;
int tmp_turn = 0;
int temp = 70;
int wind_speed;
int wind_dir;
int season;

struct termios old_tio, new_tio;

struct chr_pos {
    int hp;
    int z;
    int y;
    int death;
    int c;
};

void pos_filler(struct chr_pos *tmp, int type) {
    tmp->death = 0;
    tmp->c = 0;
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
    
    /* The season should probably be set to winter by default */
    season = rand() % 4;
    
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
                if(character.z == 0) {
                    character.z = 10;
                    monster.z -= 10;
                }
                if(character.z == -1) {
                    character.z = 10;
                    monster.z += 10;
                }
                if(character.y == 19) {
                    character.y = 1;
                    monster.y -= 20;
                }
                if(character.y == 0) {
                    character.y = 19;
                    monster.y += 20;
                }
                if(character.death == 0 || monster.death == 0) {
                    if(character.z == monster.z && character.y == monster.y) {
                        if(rand() % 2 == 0) {
                        character.hp -= rand() % 5;
                        }
                        if(rand() % 2 == 0) {
                        monster.hp -= rand() % 5;
                        }
                    }
                }
                if(character.hp < 0) {
                    character.hp = 0;
                    character.death = 1;
                    if(character.c == 0) {
                        printf("You died!\n");
                        character.c++;
                        break;
                    }
                }
                if(monster.hp < 0) {
                    monster.hp = 0;
                    monster.death = 1;
                    if(monster.c == 0) {
                        printf("You survived!\n");
                        monster.c++;
                        break;
                    }
                }
                
                if(turn - tmp_turn == 100) {
                    tmp_turn = turn;
                    weather_sim(1);
                }
                else {
                    weather_sim(0);
                }
                
                printf("turn: %i character hp: %i monster hp: %i character z: %i character y: %i\n", turn, character.hp, monster.hp, character.z, character.y);
                user_movement();
                monster_AI();
                system("clear");
            }
        }
    }
    return 0;
}

void user_movement(void) {
    if(character.death != 1) {
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
}

void monster_AI(void) {
    if(monster.death != 1) {
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
}

void weather_sim(int season_change) {
    
    if(rand() % 5 == 0) {
        temp += rand() % 10;
    }
    if(rand() % 5 == 1) {
        temp -= rand() % 10;
    }
    
    if(rand() % 5 == 0) {
        wind_speed += rand() % 3;
    }
    if(rand() % 5 == 1) {
        wind_speed -= rand() % 3;
    }
    
    if(rand() % 5 == 0) {
        wind_dir += rand() % 9;
    }
    if(rand() % 5 == 1) {
        wind_dir -= rand() % 9;
    }
    
    if(season_change == 1) {
        season++;
        if(season >= 5) {
            season = 1;
        }
    }
}

/*int gravity_attraction() {
    
    G * M1 * m2 / dist2;
    
}*/

int physics_sim(int mass, int acceleration) {
    
    int force;
    
    force=mass*acceleration;
    
    return force;
}