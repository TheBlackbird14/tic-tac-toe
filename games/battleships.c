#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


#include "../libs/strcap.h"
//#include "../libs/cs50.h"
#include "../libs/colors.h"

#define hitship "\033[31;47m"
#define missed "\033[37;44m"

//player data
#define FIELD_SIZE 10
typedef struct
{
    int id;

    char sea[FIELD_SIZE][FIELD_SIZE];

    //for stats
    int shots;
    int hits;
    int misses;
} player;

void initialize(void);
void place(void);
void placeOnBoard(player user, int row, int col, char dir, int length);
void instructions(void);

//functions to be included with cglGeneral.h
void clear(void);
void waitfor(char input);
void quit(void);

void print(player you, int mode);
int shoot(int user, int row, int col);

int checkPlacement(int user, int shipnr, char *coords, char dir);
int checkInput(char *input);

int checkWinner(void);




char coords[FIELD_SIZE][FIELD_SIZE] = {{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}, {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'}};


//space for global variables

//ships
#define MAX_SHIPS 5

int shipsLengths[MAX_SHIPS] = {2, 3, 3, 4, 5};

//players
player p1 = {1};
player p2 = {2};


int main(void)
{
    place();
    return 0;
}

void initialize(void)
{
    for (int i = 0; i < FIELD_SIZE; i++)
    {
        for (int j = 0; j < FIELD_SIZE; j++)
        {
            p1.sea[i][j] = ' ';
            p2.sea[i][j] = ' ';
        }
    }
}

//print the board during the game
void print(player p, int mode ) // mode 0 = place; 1 = in game; 2 = display winner
{
    char you[10][10];
    char enemy[10][10];

    if (p.id == 1)
    {
        for (int i = 0; i < FIELD_SIZE; i++)
        {
            for (int j = 0; j < FIELD_SIZE; j++)
            {
                you[i][j] = p1.sea[i][j];
                enemy[i][j] = p2.sea[i][j];
            }
        }
    }
    else if (p.id == 2)
    {
        for (int i = 0; i < FIELD_SIZE; i++)
        {
            for (int j = 0; j < FIELD_SIZE; j++)
            {
                you[i][j] = p2.sea[i][j];
                enemy[i][j] = p1.sea[i][j];
            }
        }
    }
    else 
    {
        printf("invalid use of function print\n");
    }

    //print own sea
    printf("Your Sea: \n\n ");
    for (int i = 0; i < 10; i++) // write nums on the cols of sea
    {
        printf(" | %c", coords[0][i]);
    }

    printf(" |\n  ");

    for (int i = 0; i < 10; i++)
    {
        printf("----");
    }

    printf("-\n");
    
    for (int i = 0; i < 10; i++)
    {
        printf("%c", coords[1][i]); //print the row letters
        for (int j = 0; j < 10; j++)
        {
            //normal sea (nothing)
            if (you[i][j] == ' ')
            {
                printf(" | %s %s", bgBlue, off);
            }
            //ship placed on tile
            else if (you[i][j] == 'X')
            {
                printf(" | %s %s", bgWhite, off);
            }
            //shot fires but missed
            else if (you[i][j] == 'O')
            {
                printf(" | %sO%s", missed, off);
            }
            //shot fired and hit ship
            else if (you[i][j] == 'H')
            {
                printf(" | %sX%s", hitship, off);
            }
            else 
            {
                printf("?");
            }
            
        }
        
        printf(" |\n  ");
        
        for (int i = 0; i < 10; i++)
	    {
	        printf("----");
	    }
	    
        printf("-\n");
    }
    printf("\n\n");


    //print enemy sea
    if (mode != 0)
    {    
        printf("Enemy Sea: \n\n ");

        for (int i = 0; i < 10; i++)
        {
            printf(" | %c", coords[0][i]);
        }

        printf(" |\n  ");

        for (int i = 0; i < 10; i++)
        {
            printf("----");
        }

        printf("-\n");

        for (int i = 0; i < 10; i++)
        {
            printf("%c", coords[1][i]);
            for (int j = 0; j < 10; j++)
            {
                //normsal sea (nothing)
                if (enemy[i][j] == ' ')
                {
                    printf(" | %s %s", bgBlue, off);
                }
                //shot fires but missed
                else if (enemy[i][j] == 'O')
                {
                    printf(" | %sO%s", missed, off);
                }
                //shot fired and hit ship
                else if (enemy[i][j] == 'H')
                {
                    printf(" | %sX%s", hitship, off);
                }
                //ship placed on tile
                else if (mode == 2)
                {
                    if (enemy[i][j] == 'X')
                    {
                        printf(" | %s %s", bgWhite, off);
                    }
                }
                else 
                {
                    printf("?");
                }
            }

            printf(" |\n  ");

            for (int i = 0; i < 10; i++)
            {
                printf("----");
            }

            printf("-\n");
        }
        printf("\n\n");
    }

}

void place(void)
{
    clear();
    printf("ENTER to Place Ships for Player 1");
    waitfor('\n');

    for (int i = 0; i < MAX_SHIPS; i++)
    {
        clear();
        print(p1, 0);

        char input[3];
        int c;

        do
        {
            printf("Place Ship %d with length %d> ", i + 1, shipsLengths[i]);
            scanf("%2s", input);
            while ((c = getchar()) != '\n' && c != EOF);  // clear input buffer

            if (checkInput(input) != 0)
            {
                printf("Invalid Coordinate entered, try again\n");
            }
        }
        while (checkInput(input) != 0);

        //printf("%s\n", input);
        //waitfor('\n');

        char dir;
        c = 0;

        do
        {
            printf("Direction (u, d, l, r)> ");
            scanf("%c", &dir);

            while ((c = getchar()) != '\n' && c != EOF);  // clear input buffer

            if (toupper(dir) == 'Q')
            {
                quit();
            }
        }
        while (toupper(dir) != 'U' && toupper(dir) != 'D' && toupper(dir) != 'L' && toupper(dir) != 'R');

        int row = toupper(input[0]) - 65;
        int col = input[1] - 48;

        placeOnBoard(p1, row, col, dir, shipsLengths[i]);

    }

    clear();
    printf("ENTER to Place Ships for Player 2");
    waitfor('\n');

    for (int i = 0; i < MAX_SHIPS; i++)
    {
        clear();
        print(p2, 0);

        char input[3];
        int c;

        do
        {
            printf("Place Ship %d with length %d> ", i + 1, shipsLengths[i]);
            scanf("%2s", input);
            while ((c = getchar()) != '\n' && c != EOF);  // clear input buffer

            if (checkInput(input) != 0)
            {
                printf("Invalid Coordinate entered, try again\n");
            }
        }
        while (checkInput(input) != 0);

        //printf("%s\n", input);
        //waitfor('\n');

        char dir;
        c = 0;

        do
        {
            printf("Direction (u, d, l, r)> ");
            scanf("%c", &dir);

            while ((c = getchar()) != '\n' && c != EOF);  // clear input buffer

            if (toupper(dir) == 'Q')
            {
                quit();
            }
            else if (toupper(dir) != 'U' && toupper(dir) != 'D' && toupper(dir) != 'L' && toupper(dir) != 'R')
            {
                printf("\"%c\" is not a valid direction\n", dir);
            }

        }
        while (toupper(dir) != 'U' && toupper(dir) != 'D' && toupper(dir) != 'L' && toupper(dir) != 'R');

        int row = toupper(input[0]) - 65;
        int col = input[1] - 48;

        placeOnBoard(p2, row, col, toupper(dir), shipsLengths[i]);

    }
}

void placeOnBoard(player user, int row, int col, char dir, int length)
{
    switch (dir)
    {
    case 'U':
        for (int i = 0, c = row; i < length; i++, c--)
        {
            user.sea[c][col] = 'X';
        }
        break;

    case 'D':
        for (int i = 0, c = row; i < length; i++, c++)
        {
            user.sea[c][col] = 'X';
        }
        break;

    case 'L':
        for (int i = 0, c = col; i < length; i++, c--)
        {
            user.sea[row][c] = 'X';
        }
        break;

    case 'R':
        for (int i = 0, c = col; i < length; i++, c++)
        {
            user.sea[row][c] = 'X';
        }
        break;
    
    default:
        printf("invalid use of function placeOnBoard");
        break;
    }
}

//checks if the  entered input is a valid coord
int checkInput(char *input) //return 0 when is ok, 1 when not
{
    if (strcmp(strup(input), "QUIT") == 0 || strcmp(strup(input), "EXIT") == 0 || strcmp(strup(input), "Q") == 0)
    {
        quit();
    }

    bool validLet = false;
    bool validNum = false;

    for (int i = 0; i < FIELD_SIZE; i++)
    {
        if (toupper(input[0]) == coords[1][i])
        {
            validLet = true;
            break;
        }
    }
    
    for (int i = 0; i < FIELD_SIZE; i++)
    {
        if (toupper(input[1]) == coords[0][i])
        {
            validNum = true;
            break;
        }
    }

    if (validLet == true && validNum == true)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

int checkPlacement(int user, int shipnr, char *coords, char dir)
{

    return 0;
}


    //clear the screen
void clear(void)
{
    printf("\e[1;1H\e[2J");
}

//wait for an input (eg '\n')
void waitfor(char input)
{
    while (getchar() != input);
}

//print the instructions
void instructions(void)
{
    printf("%sWelcome to %sBattleships!%s\n", red, bgWhite, off);
    printf("In this game you shoot at eachothers ships and try to sink all your enemies ships first.\n");
    printf("But first, you have to %splace%s your ships at the beginning of the game.\n", cyan, off);
    printf("To do so, enter the %scoordinate%s at which the ships should start, and then the %sdirection%s it should point to (%su%sp, %sd%sown, %sl%seft, %sr%sight)\n", yellow, off, yellow, off, uWhite, off, uWhite, off, uWhite, off, uWhite, off);
    printf("%sPlayer 1%s starts. have fun!\n\n", green, off);
    printf("Press %sENTER%s to start", bRed, off);
    
    waitfor('\n');
}

//quit the program
void quit(void)
{
    clear();
    printf("Until Next Time!\n");
    exit(2);
}