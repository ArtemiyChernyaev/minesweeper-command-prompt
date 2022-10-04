#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#include<windows.h>

#define _WIN64_WINNT 0x502 
/*
Necessary for using this function: GetConsoleWindow().
*/
#define STRING_BUFFER_LENGTH 10000 
/*
Used how length buffer for entering game parameters.
*/

int OpenCellsCount;
/*
Used to trigger the victory event.
If OpenCellsCount equal to the number of cells without mines, so the player won.
*/

void ShowPicture() 
{
    printf("\n __  __ ___ _   _ _____ ______        _______ _____ ____  _____ ____  \n");
    printf("|  \\/  |_ _| \\ | | ____/ ___\\ \\      / / ____| ____|  _ \\| ____|  _ \\ \n");
    printf("| |\\/| || ||  \\| |  _| \\___  \\ \\ /\\ / /|  _| |  _| | |_) |  _| | |_) |\n");
    printf("| |  | || || |\\  | |___ ___) |\\ V  V / | |___| |___|  __/| |___|  _ < \n");
    printf("|_|  |_|___|_| \\_|_____|____/  \\_/\\_/  |_____|_____|_|   |_____|_| \\_\\\n");
}

void SetGameParametres(int * Rows, int * Columns, int * MinesCount)
{
    char Buffer[STRING_BUFFER_LENGTH]; 
    /*
    Creating a buffer for the line entered by the user in order to process it 
    in the future.
    */
    int  IsSuitable;
    /*
    The variable responsible for the correctness of the data entered by the user.
    */

    /*
    Get the number of rows of the field.
    We execute until the user enters the correct values.
    */
    do { 
        system("cls"); // Clearing the console.  
        ShowPicture(); 
        printf("\nEnter number of rows(max 25): ");     

        IsSuitable = 1;
        scanf("%s",Buffer); // Reading the line entered by the user.

        /*
        Check that the entered string is a natural number.
        */
        for(int i = 0; i < strlen(Buffer); i++) { 
            if (!isdigit(Buffer[i])) {
                IsSuitable = 0;
                break;
            }
        }
        
        /*
        Check that the number is within the specified limits.
        */  
        if (IsSuitable) {
            if (atoi(Buffer) == 0 || atoi(Buffer) > 25) {
                IsSuitable = 0;
            } else {
                *Rows =  atoi(Buffer);
            }
        }
         
    } while (!IsSuitable); 
   
    /*
    Get the number of columns of the field.
    We execute until the user enters the correct values.
    */
    do {
        system("cls");
        ShowPicture();
        printf("\nEnter number of columns(max 25): ");
        
        IsSuitable = 1;
        scanf("%s",Buffer);

        for(int i = 0; i < strlen(Buffer); i++) {
            if (!isdigit(Buffer[i])) {
                IsSuitable = 0;
                break;
            }
        }

        if (IsSuitable) {
            if( atoi(Buffer) == 0 || atoi(Buffer) > 25) {
                IsSuitable = 0;
            } else {
                *Columns =  atoi(Buffer);
            }
        }
    } while (!IsSuitable); 

    /*
    Get the number of mines of the field.
    We execute until the user enters the correct values.
    */
    do{
        system("cls");
        ShowPicture();
        printf("\nEnter percent of mines(max 20): ");
        int Percent;

        IsSuitable = 1;
        scanf("%s",Buffer);

        for (int i = 0; i < strlen(Buffer); i++) {
            if (!isdigit(Buffer[i])) {
                IsSuitable = 0;
            }
        }
        
        if (IsSuitable) {
            if (atoi(Buffer) > 20) {
                IsSuitable = 0;
            } else {
                Percent = atoi(Buffer);
                *MinesCount = (*Rows) * (*Columns) * atoi(Buffer) / 100;
            }
        }
    } while (!IsSuitable);
}

char ** GameFieldInit(int Rows, int Columns)
/*
Allocate memory for the game field 2d array and fill it with symbols with the 
number 254 (square).
*/
{
    char ** GameField = (char **)malloc(Rows * sizeof(char *));

    for (int i = 0; i < Rows; i++) {
        GameField[i] = (char *)malloc(Columns * sizeof(char));
    }

    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            GameField[i][j] = 254;
        }
    }
    return GameField;
}

int ** ValuesFieldInit(int Rows, int Columns, int BombsCount, 
                       int Row, int Column)
//Allocate memory for the values field 2d array and fill it with mines.
{
    int **ValuesField = (int **)malloc(Rows * sizeof(int *));

    for(int i = 0; i < Rows; i++) {
        ValuesField[i] = (int *)malloc(Columns * sizeof(int));
    }

    for(int i = 0; i < Rows; i++){
        for(int j = 0; j < Columns; j++){
            ValuesField[i][j] = 0;
        }
    }

    int k = BombsCount;

    while(k > 0) {
        int i = rand()%Rows;
        int j = rand()%Columns;

        /*
        We will spawn mines, provided that the bomb cannot be located 
        next to the starting cell.
        */
        if (ValuesField[i][j] >= 0 
            && !((i==Row && j==Column)
                ||(i==Row-1 && j==Column)
                ||(i==Row && j==Column-1)
                ||(i==Row-1 && j==Column-1)
                ||(i==Row+1 && j==Column)
                ||(i==Row && j==Column+1)
                ||(i==Row+1 && j==Column+1)
                ||(i==Row+1 && j==Column-1)
                ||(i==Row-1 && j==Column+1))) {

            ValuesField[i][j] = -9;

            if (i > 0)
                ValuesField[i - 1][j] = ValuesField[i - 1][j] + 1;
            if (j > 0)
                ValuesField[i][j - 1] = ValuesField[i][j - 1] + 1;
            if (i < Rows - 1)
                ValuesField[i + 1][j] = ValuesField[i + 1][j] + 1;
            if (j < Columns - 1)
                ValuesField[i][j + 1] = ValuesField[i][j + 1] + 1;
            if ((i > 0) && (j > 0))
                ValuesField[i - 1][j - 1] = ValuesField[i - 1][j - 1] + 1;
            if ((i > 0) && (j < Columns - 1))
                ValuesField[i - 1][j + 1] = ValuesField[i - 1][j + 1] + 1;
            if ((i < Rows - 1) && (j > 0))
                ValuesField[i + 1][j - 1] = ValuesField[i + 1][j - 1] + 1;
            if ((i < Rows - 1) && (j < Columns - 1))
                ValuesField[i + 1][j + 1] = ValuesField[i + 1][j + 1] + 1;
            k--;
        }
    }
    return ValuesField;
}

int ** IsOpenFieldInit(int Rows, int Columns)
/*
Allocate memory for the 2d array containing the states (open or closed) 
of each cell and fill it with mines.
*/
{
    int **IsOpenField = (int **)malloc(Rows * sizeof(int *));

    for (int i = 0; i < Rows; i++) {
        IsOpenField[i] = (int *)malloc(Columns * sizeof(int));
    }

    for(int i = 0; i < Rows; i++){
        for(int j = 0; j < Columns; j++){
            IsOpenField[i][j] = 0;
        }
    }

    return IsOpenField;
}

void GameFieldPrint(char **GameField, int Rows, int Columns, int MinesCount)
// Output to the console of the game field.
{
    system("cls");

    printf(" ");
    for (int j = 0; j < 2*Columns + 5; j++) {         
            printf("_");
    }
    printf("\n");

    printf("|");
    for (int j = 0; j < 2*Columns + 5; j++) {         
            printf(" ");
    }
    printf("|\n");

    for (int i = 0; i < Rows; i++) {
        printf("|   ");
        for (int j = 0; j < Columns; j++) {         
            printf("%c ", GameField[i][j]);
        }
        printf("  |\n");
    }  

    printf("|");
    for (int j = 0; j < 2*Columns + 5; j++) {         
            printf("_");
    }
    printf("|\n\n");

    printf("Mines count = %i\n", MinesCount);

    printf("Set or remove flag   Ctrl + LMB\n");
    printf("Open cell                   LMB\n");
}

void IntArrayFree(int **Array, int Rows)
// Clearing the memory allocated for the memory int 2d array.
{
    for(int i = 0; i < Rows; i++) {
        free(Array[i]);
    }
    free(Array);
}

void CharArrayFree(char **Array, int Rows)
// Clearing the memory allocated for the memory char 2d array.
{
    for(int i = 0; i < Rows; i++) {
        free(Array[i]);
    }
    free(Array);
}

void OpenCell(char **GameField, int **ValuesField, int **IsOpenField, 
              int Row, int Column, int Rows, int Columns)
// Opening a cell of the game field in a row(Row) and in a column(Column).
{
    if (IsOpenField[Row][Column] == 0) {
        IsOpenField[Row][Column] = 1;
        OpenCellsCount++;

        /*
        If the cell is empty and there are no mines nearby, then open all 
        neighboring cells.
        */
        if (ValuesField[Row][Column] == 0) {
            GameField[Row][Column] = ' ';
            
            if (Row > 0)
                OpenCell(GameField, ValuesField, IsOpenField, 
                         Row - 1, Column, Rows, Columns);
            if (Column > 0)
                OpenCell(GameField, ValuesField, IsOpenField, 
                         Row, Column - 1, Rows, Columns);
            if (Row < Rows - 1)
                OpenCell(GameField, ValuesField, IsOpenField, 
                         Row + 1, Column, Rows, Columns);
            if (Column < Columns - 1)
                OpenCell(GameField, ValuesField, IsOpenField, 
                         Row, Column + 1, Rows, Columns);
            if ((Row > 0) && (Column > 0))
                OpenCell(GameField, ValuesField, IsOpenField, 
                         Row - 1, Column - 1, Rows, Columns);
            if ((Row > 0) && (Column < Columns - 1))
                OpenCell(GameField, ValuesField, IsOpenField, 
                         Row - 1, Column + 1, Rows, Columns);
            if ((Row < Rows - 1) && (Column > 0))
               OpenCell(GameField, ValuesField, IsOpenField, 
                        Row + 1, Column - 1, Rows, Columns);
            if ((Row < Rows - 1) && (Column < Columns - 1))
               OpenCell(GameField, ValuesField, IsOpenField, 
                        Row + 1, Column + 1, Rows, Columns);
        } else {
            GameField[Row][Column] = ValuesField[Row][Column] + '0';
        }
    }
}

void OpenAlCells(char **GameField, int **ValuesField, int **IsOpenField, 
                 int Rows, int Columns)
// Opening all cells of the game field.
{
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            if (IsOpenField[i][j] == 0) {
                if (ValuesField[i][j] < 0) {
                    GameField[i][j] = 15;
                } else {
                    if(ValuesField[i][j] == 0) {
                        GameField[i][j] = ' ';
                    } else {
                        GameField[i][j] = ValuesField[i][j] + '0';
                    }
                }
            }
        }
    }
}

void SetOrRemoveFlag(char **GameField, int **IsOpenField, int Row, int Column)
// Set or remove flag(the symbol with the number 16) to mark suspected mines.
{
    if (IsOpenField[Row][Column] == 0 && (int)GameField[Row][Column] == -2) {
            GameField[Row][Column] = 16;
            return;
    }

    if (IsOpenField[Row][Column] == 0 && GameField[Row][Column] == 16) {
            GameField[Row][Column] = 254;
            return;
    }
}

void Loss(char **GameField, int **ValuesField, int **IsOpenField, 
          int Rows, int Columns, int MinesCount)
// Function for the event - loss.
{
    OpenAlCells(GameField, ValuesField, IsOpenField, Rows, Columns);

    GameFieldPrint(GameField, Rows, Columns, MinesCount);

    CharArrayFree(GameField, Rows);
    IntArrayFree(ValuesField, Rows);
    IntArrayFree(IsOpenField, Rows);

    printf("\nYOU LOSE");
}

void Win(char **GameField, int **ValuesField, int **IsOpenField, 
         int Rows, int Columns, int MinesCount)
// Function for the event - win.
{
    OpenAlCells(GameField, ValuesField, IsOpenField, Rows, Columns);

    GameFieldPrint(GameField, Rows, Columns, MinesCount);

    CharArrayFree(GameField, Rows);
    IntArrayFree(ValuesField, Rows);
    IntArrayFree(IsOpenField, Rows);

    printf("\nYOU WIN!!!");
}

void GameStart(){
    int Rows;
    int Columns;
    int Row;
    int Column;
    int MinesCount;
    OpenCellsCount = 0;   

    /* 
    If the function is successful, a handle for the specified device is returned.
    STD_OUTPUT_HANDLE - Standard output device. Initially, this is the input buffer 
    of the console.
    */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    /*
    Returns value is a handle to the window used by the console associated with 
    the calling process or NULL if there is no such associated console.
    */
    HWND hwnd = GetConsoleWindow();


    /*
    A pointer to a POINT structure that receives the screen coordinates of the cursor.
    */
    LPPOINT pPnt;
    pPnt = malloc(sizeof(*pPnt));

    /*
    In the variable will "fontInfo" we will record the parameters of the current 
    font, from which we will then find out the size of the console cell.
    */
    CONSOLE_FONT_INFO fontInfo;

    /*
    Setting the structure "cfi" with the parameters of the console font 
    that we will use. 
    The font size of 12x26 is necessary for the correct operation of clicking 
    on the cell of the playing field.
    */
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 12;                  
    cfi.dwFontSize.Y = 26;                  
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(hOut, 0, &cfi); // Set console font parametres. 

    ShowPicture(); // Show large inscription "minesweepr".
    printf("\nPress any key to start game...\n");
    getch(); // Start the game after pressing any button on the keyboard.

    SetGameParametres(&Rows, &Columns, &MinesCount); 

    char **GameField = GameFieldInit(Rows, Columns);
    int **IsOpenField = IsOpenFieldInit(Rows, Columns);

    GameFieldPrint(GameField, Rows, Columns, MinesCount);

    /*
    To start the game, you need to get a starting cell, then place it 
    on the mines. 
    This is necessary so that the user does not hit a mine with the first click.
    */
    while (1) {
        SetCurrentConsoleFontEx(hOut, 0, &cfi); 

        /*
        If the left mouse button is pressed and the left ctrl is not pressed, 
        the selected cell opens.
        */
        if (GetKeyState(VK_LBUTTON) < 0 && GetKeyState(VK_LCONTROL) >= 0) {   
            /*
            We write the coordinates of the cursor position in "pPnt".  
            */
            GetCursorPos(pPnt); 

            /*
            Calculate the coordinates of the mouse relative to the console 
            window "hwnd".
            */
            ScreenToClient(hwnd, pPnt);  

            /*
            Get console font parametres, to find out the row and column of 
            the selected cell.
            */ 
            GetCurrentConsoleFont(hOut, 0, &fontInfo);

            /*
            Find out the row and column of the selected cell.
            */ 
            Row = pPnt[0].y/fontInfo.dwFontSize.Y - 2;
            Column =   pPnt[0].x/(2*fontInfo.dwFontSize.X) - 2;

            /*
            If such a cell exists then we exit the cycle.
            */ 
            if (Row < Rows && Column < Columns && Row >= 0 && Column >= 0) {
                break;
            } else {
                Sleep(100);
            }
        }
        /*
        If the left mouse button is pressed and the left ctrl is pressed, 
        a flag is placed on the selected cell.
        */
        if (GetKeyState(VK_LBUTTON) < 0 && GetKeyState(VK_LCONTROL) < 0 ) { 
            GetCursorPos(pPnt);    
            ScreenToClient(hwnd, pPnt);   
            GetCurrentConsoleFont(hOut, 0, &fontInfo);

            Row = pPnt[0].y/fontInfo.dwFontSize.Y - 2;
            Column =  pPnt[0].x/(2*fontInfo.dwFontSize.X) - 2;

            if (Row < Rows && Column < Columns && Row >= 0 && Column >= 0) {
                SetOrRemoveFlag(GameField, IsOpenField, Row, Column); 
                GameFieldPrint(GameField, Rows, Columns, MinesCount);       
            }

            Sleep(100);
        }
    }
    
    srand(time(NULL)); // Set new params for the pseudorandom number generator.

    /*
    Spawn mines on game field.
    */
    int **ValuesField = ValuesFieldInit(Rows, Columns, MinesCount, Row, Column);
    
    /*
    Open start cell.
    */
    OpenCell(GameField, ValuesField, IsOpenField,Row, Column, Rows, Columns);  
    GameFieldPrint(GameField, Rows, Columns, MinesCount);
    
    while (1){
        SetCurrentConsoleFontEx(hOut, 0, &cfi);

        if (GetKeyState(VK_LBUTTON) < 0 && GetKeyState(VK_LCONTROL) >= 0) {   
            GetCursorPos(pPnt);    
            ScreenToClient(hwnd, pPnt);   
            GetCurrentConsoleFont(hOut, 0, &fontInfo);

            Row = pPnt[0].y/fontInfo.dwFontSize.Y - 2;
            Column =  pPnt[0].x/(2*fontInfo.dwFontSize.X) - 2;

            if (Row < Rows && Column < Columns && Row >= 0 && Column >= 0) {
                
                /*
                If we hit a mine, we lost.
                */
                if (ValuesField[Row][Column] < 0) {
                    Loss(GameField, ValuesField, IsOpenField, Rows, Columns, MinesCount);
                    break;
                }
                else {
                    OpenCell(GameField, ValuesField, IsOpenField, 
                             Row, Column, Rows, Columns);
                    GameFieldPrint(GameField, Rows, Columns, MinesCount);
                }               
            }

            /*
            If we opened all cells without mine, we win.
            */
            if (OpenCellsCount == Rows*Columns - MinesCount) {
                    Win(GameField, ValuesField, IsOpenField, Rows, Columns, MinesCount);
                    break;
            }
            Sleep(100);
        }  

        if(GetKeyState(VK_LBUTTON) < 0 && GetKeyState(VK_LCONTROL) < 0 ) { 
            GetCursorPos(pPnt);    
            ScreenToClient(hwnd, pPnt);   
            GetCurrentConsoleFont(hOut, 0, &fontInfo);

            Row = pPnt[0].y/fontInfo.dwFontSize.Y - 2;
            Column =  pPnt[0].x/(2*fontInfo.dwFontSize.X) - 2;

            if (Row < Rows && Column < Columns && Row >= 0 && Column >= 0) {
                SetOrRemoveFlag(GameField, IsOpenField, Row, Column);        
            }

            GameFieldPrint(GameField, Rows, Columns, MinesCount);
            Sleep(100);
        }
    }
}

void main() {
    SetConsoleTitle("MINESWEEPER");

    /* 
    If the function is successful, a handle for the specified device is returned.
    STD_INPUT_HANDLE - Standard input device. Initially, this is the input buffer 
    of the console.
    */
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    
    /* 
    We disable the possibility for the user to stop the output of the program
    to the console by clicking the left mouse button and switch the console to 
    input mode, otherwise, when clicking on the cell, the console will switch 
    to input mode and the program will work incorrectly.
    */
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS); 
    
    while(1){
        GameStart();
    }
}




