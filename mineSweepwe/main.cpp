
#include <iomanip>
#include <ctime>
#include <string>
#include <fstream>
#include<iostream>

using namespace std;

ifstream fin;
ofstream fout;

//declaring constant board sizes along with amount of mines, depending on difficulty
const int BOARD_SIZE = 13;

const int EASY_SIZE = 5;
const int MEDIUM_SIZE = 7;
const int HARD_SIZE = 13;

const int EASY_MINES = 5;
const int  MEDIUM_MINES = 17;
const int HARD_MINES = 25;

//defining colors
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m" 
#define BLUE_COLOR "\033[34m" 
#define GREEN_COLOR "\033[32m"
#define CYAN_COLOR  "\033[36m"
#define MAGENTA_COLOR "\033[35m"

//function declaration
void Board_Printing(char array[BOARD_SIZE][BOARD_SIZE], int size);
void place_mines_and_bonus(char array[BOARD_SIZE][BOARD_SIZE], int size, int mines_count);
void score_display(int score, string username);
int bomb_distance(char arr[BOARD_SIZE][BOARD_SIZE], int row, int col);
void show_cell(char array[BOARD_SIZE][BOARD_SIZE], int row, int col);
void show_mines(char array[BOARD_SIZE][BOARD_SIZE], int size);
void show_bonus(char array[BOARD_SIZE][BOARD_SIZE], int size);
void main_game_play(char array[BOARD_SIZE][BOARD_SIZE], int size, int mines_count, string username);
void welcome(string username);
int difficulty_selection();
int TakeCurrentUser(const string  username);
void ScoreUpDater(const string  username, int new_score);
int mine_selection(int BOARD_SIZE_CHOICE);
void leaderboard(string userWithHightestScores[], int highest[]);
void TopScorers();
void playAgain();


int main() {



    srand(time(0));
    char array[BOARD_SIZE][BOARD_SIZE];
    string username;

    cout << "You can either enter your Roll number or name\n";
    cout << RED_COLOR << "Warning " << RESET_COLOR << "Please note that any username or roll number you input will be shown on the display.\n        There is no restriction.\n";

    cout << "Enter your username OR rollNumber: ";
    getline(cin, username);

    //initializing array with question mark symbols
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            array[i][j] = '?';
        }
    }

    //board and mine size depends on user choice
    welcome(username);
    int BOARD_SIZE_CHOICE = difficulty_selection();
    int MINES_COUNT = mine_selection(BOARD_SIZE_CHOICE);
    place_mines_and_bonus(array, BOARD_SIZE_CHOICE, MINES_COUNT);
    main_game_play(array, BOARD_SIZE_CHOICE, MINES_COUNT, username);
    playAgain();

    return 0;
}

void welcome(string username) {

    cout << "+-------------------------------------------+" << endl;
    cout << "|                 Welcome                   |" << endl;
    cout << "+-------------------------------------------+" << endl;
    cout << "| Welcome to Minesweeper:  " << setw(17) << left << username << "|" << endl;
    cout << right;
    cout << "+-------------------------------------------+" << endl;

    cout << "\n" << "Difficulty levels \n Easy \n Medium \n Hard \n\n";
    cout << RED_COLOR << "You can not enter spaces before or after difficulty choice\n" << RESET_COLOR;

    cout << GREEN_COLOR << "Instructions:\n" << RESET_COLOR;
    cout << GREEN_COLOR << "First you will enter row number and then column number\n" << RESET_COLOR;
    cout << GREEN_COLOR << "There is a bonus for you in the game you would have to guess it\n" << RESET_COLOR;
    cout << GREEN_COLOR << "There is 5 mines in easy mode 7 in medium mode and 25 in hard mode\n" << RESET_COLOR;
    cout << GREEN_COLOR << "There is a 45 seconds timer in easy mode\n60 secs in medium mode and\n85 seconds in hard mode\n\n" << RESET_COLOR;
}

int mine_selection(int BOARD_SIZE_CHOICE) {
    int MINES_COUNT;

    if (BOARD_SIZE_CHOICE == EASY_SIZE) {
        MINES_COUNT = EASY_MINES;
    }
    else if (BOARD_SIZE_CHOICE == MEDIUM_SIZE) {
        MINES_COUNT = MEDIUM_MINES;
    }
    else if (BOARD_SIZE_CHOICE == HARD_SIZE) {
        MINES_COUNT = HARD_MINES;
    }
    else {
        cout << "Incorrect choice enter valid choice: ";
        return 1;
    }

    return MINES_COUNT;

}

int difficulty_selection() {
    string choice;

    int BOARD_SIZE_CHOICE;
    int MINES_COUNT;

    cout << "\n " << "Enter 1 for Easy \n Enter 2 for Medium \n Enter 3 for Hard \n ";
    cout << "Enter: ";

    getline(cin, choice);

    if (choice == "1") {
        BOARD_SIZE_CHOICE = EASY_SIZE;
        MINES_COUNT = EASY_MINES;
    }
    else if (choice == "2") {
        BOARD_SIZE_CHOICE = MEDIUM_SIZE;
        MINES_COUNT = MEDIUM_MINES;
    }
    else if (choice == "3") {
        BOARD_SIZE_CHOICE = HARD_SIZE;
        MINES_COUNT = HARD_MINES;
    }
    else {
        cout << "Incorrect choice enter valid choice";
        return difficulty_selection();
    }

    return BOARD_SIZE_CHOICE;
}

void Board_Printing(char array[BOARD_SIZE][BOARD_SIZE], int size) {

    cout << setw(4) << setfill(' ') << " ";
    for (int j = 0; j < size; j++) {
        cout << setw(2) << j << "  ";
    }
    cout << endl;


    cout << setw(4) << setfill(' ') << " ";
    for (int j = 0; j < size; j++) {
        cout << "----";
    }
    cout << "-" << endl;


    for (int i = 0; i < size; i++) {
        cout << i << " |";
        for (int j = 0; j < size; j++) {
            if (array[i][j] != '?' && array[i][j] != ' ' && array[i][j] != '*' && array[i][j] != 'b') {

                cout << BLUE_COLOR << setw(2) << array[i][j] << " " << RESET_COLOR << "|";
            }
            else if (array[i][j] == ' ') {
                cout << GREEN_COLOR << " - " << RESET_COLOR << "|";
            }
            else {
                cout << " ? |";
            }
        }
        cout << endl;


        cout << setw(4) << setfill(' ') << " ";
        for (int j = 0; j < size; j++) {
            cout << "----";
        }
        cout << "-" << endl;

    }





    system("pause");
}

void place_mines_and_bonus(char array[BOARD_SIZE][BOARD_SIZE], int size, int mines_count) {

    srand(time(0));

    int mines = 0, bonus = 0, x, y;
    int bonus_count = 1;
    //looop to place mines and bonus
    while (mines < mines_count) {
        x = rand() % size;
        y = rand() % size;
        if (array[x][y] != '*') {
            array[x][y] = '*';
            mines++;
        }
    }


    while (bonus < bonus_count) {
        x = rand() % size;
        y = rand() % size;
        if (array[x][y] != 'b' && array[x][y] != '*') {
            array[x][y] = 'b';
            bonus++;
        }

    }

}

void score_display(int score, string username) {


    cout << "+----------------------------+" << endl;
    cout << "            Your Score        " << endl;
    cout << "+----------------------------+" << endl;
    cout << "  " << setw(5) << left << username << ": " << left << setw(8) << score << "pts" << endl;
    cout << "+----------------------------+" << endl;

}

void leaderboard(string userWithHightestScores[], int highest[]) {
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    cout << "+--------------------------+" << endl;
    cout << "         leader board       " << endl;
    cout << "+--------------------------+" << endl;
    cout << "  " << setw(5) << left << "1. " << userWithHightestScores[0] << ": " << highest[0] << endl << right;
    cout << "  " << setw(5) << left << "2. " << userWithHightestScores[1] << ": " << highest[1] << endl << right;
    cout << "  " << setw(5) << left << "3. " << userWithHightestScores[2] << ": " << highest[2] << endl << right;
    cout << "+--------------------------+" << endl;
}

int bomb_distance(char arr[BOARD_SIZE][BOARD_SIZE], int row, int col) {
    int count = 0;
    //iterates thru 3x3 grid
    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = col - 1; j <= col + 1; ++j) {
            //checking if within bounds of gameboard and if bomb is in cell
            if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && arr[i][j] == '*') {
                count++;
            }
        }
    }
    return count;
}

void show_mines(char array[BOARD_SIZE][BOARD_SIZE], int size) {
    //displaying game board first
    cout << setw(4) << setfill(' ') << " ";
    for (int j = 0; j < size; j++) {
        cout << setw(6) << j;
    }
    cout << endl;


    cout << setw(4) << setfill(' ') << " ";
    for (int j = 0; j < size; j++) {
        cout << "------";
    }
    cout << "--" << endl;


    for (int i = 0; i < size; i++) {
        cout << i << "    " << " |";
        for (int j = 0; j < size; j++) {
            if (array[i][j] == '*') {
                //display mine in red
                cout << RED_COLOR << setw(3) << array[i][j] << " " << RESET_COLOR << " |";
            }
            else if (array[i][j] == ' ') {

                cout << GREEN_COLOR << "  - " << RESET_COLOR << " |";
            }
            else if (array[i][j] == 'b') {

                cout << MAGENTA_COLOR << "  b " << RESET_COLOR << " |";
            }
            else if (array[i][j] == '?') {

                cout << CYAN_COLOR << "  ? " << RESET_COLOR << " |";
            }
            else {
                cout << BLUE_COLOR << setw(3) << array[i][j] << " " << RESET_COLOR << " |";
            }
        }
        cout << endl;


        cout << setw(4) << setfill(' ') << " ";
        for (int j = 0; j < size; j++) {
            cout << "------";
        }
        cout << "--" << endl;
    }

    system("pause");
}

void show_bonus(char array[BOARD_SIZE][BOARD_SIZE], int size) {

    cout << setw(4) << setfill(' ') << " ";
    for (int j = 0; j < size; j++) {
        cout << setw(2) << j << "  ";
    }
    cout << endl;


    cout << setw(4) << setfill(' ') << " ";
    for (int j = 0; j < size; j++) {
        cout << "----";
    }
    cout << "-" << endl;


    for (int i = 0; i < size; i++) {
        cout << i << " |";
        for (int j = 0; j < size; j++) {
            if (array[i][j] != '?' && array[i][j] != ' ' && array[i][j] != '*' && array[i][j] != 'b') {

                cout << BLUE_COLOR << setw(2) << array[i][j] << " " << RESET_COLOR << "|";
            }
            else if (array[i][j] == 'b') {
                cout << MAGENTA_COLOR << setw(2) << array[i][j] << " " << RESET_COLOR << "|";
            }
            else if (array[i][j] == ' ') {
                cout << GREEN_COLOR << " - " << RESET_COLOR << "|";
            }
            else {
                cout << " ? |";
            }
        }
        cout << endl;


        cout << setw(4) << setfill(' ') << " ";
        for (int j = 0; j < size; j++) {
            cout << "----";
        }
        cout << "-" << endl;

    }

    system("pause");
}

void show_cell(char array[BOARD_SIZE][BOARD_SIZE], int row, int col) {
    //if cell is already revealed do nothing 
    if (array[row][col] != '?')
    {
        return;
    }
    //if a  mine , it gets revealed
    if (array[row][col] == '*')
    {
        array[row][col] = '*';
        return;
    }
    //if bonus, reveal it
    if (array[row][col] == 'b')
    {
        array[row][col] = 'b';
        return;
    }

    else {
        int adjacent = bomb_distance(array, row, col);
        if (adjacent > 0) {

            array[row][col] = '0' + adjacent;
        }
        else {
            //if there are no adjacent mines, reveal surrounding repeatedly
            array[row][col] = ' ';
            for (int i = row - 1; i <= row + 1; ++i) {
                for (int j = col - 1; j <= col + 1; ++j) {
                    if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE) {
                        show_cell(array, i, j);
                    }
                }
            }
        }
    }
}

int TakeCurrentUser(string username) {
    ifstream fin("scores.txt");
    string name;
    int score;

    while (fin >> name >> score) {
        if (name == username) {
            fin.close();
            return score;
        }
        else {
            score = 0;
            return score;
        }
    }

    fin.close();
    return 0;
}

void  ScoreUpDater(string username, int new_score) {
    ifstream fin("scores.txt");
    ofstream fout("temp.txt");
    ofstream fileout("scores.txt", ios::app);

    string name;
    int score;
    bool UserMilGia = false;

    while (fin >> name >> score) {
        if (name == username) {
            UserMilGia = true;
            if (new_score > score) {
                fout << username << " " << new_score << endl;
            }
            else {
                fout << name << " " << score << endl;
            }
        }
        else {
            fout << name << " " << score << endl;

        }
    }

    if (!UserMilGia) {
        fout << username << " " << new_score << endl;
        fileout << username << " " << new_score << endl;
    }




    fin.close();
    fout.close();
    fileout.close();
}

void main_game_play(char array[BOARD_SIZE][BOARD_SIZE], int size, int mines_count, string username) {

    fout.open("scores.txt", ios::app);


    if (fout.is_open()) {

        int score = 0;
        time_t startTime = time(0);
        int current_score = TakeCurrentUser(username);

        if (size == 5) {
            cout << MAGENTA_COLOR << "You have 20 sec to complete the game\nYour timer will start when you will enter the first input\nBe ready!\n\n" << RESET_COLOR;
        }
        else if (size == 7) {
            cout << MAGENTA_COLOR << "You have 40 sec to complete the game\nYour timer will start when you will enter the first input\nBe ready!\n\n" << RESET_COLOR;
        }
        else {
            cout << MAGENTA_COLOR << "You have 60 sec to complete the game\nYour timer will start when you will enter the first input\nBe ready!\n\n" << RESET_COLOR;
        }

        while (true) {

            char  ROW, COL;
            int temp_size = size;
            cout << "Enter the row number and column number (0-" << size - 1 << "): ";
            cin >> ROW >> COL;

            int row = ROW - '0';
            int col = COL - '0';

            if (startTime == 0) {
                time(&startTime);
            }

            if (row < 0 || row >= size || col < 0 || col >= size) {
                cout << "Invalid input! Please enter valid row and column numbers.\n";
                continue;
            }

            if (array[row][col] == '*') {

                cout << "Game Over! You clicked on a mine." << endl;
                array[row][col] = '*';
                show_mines(array, size);
                score_display(score, username);

                if (score >= current_score) {
                    ScoreUpDater(username, score);
                }

                fout.close();

                break;
            }
            else if (array[row][col] == 'b') {
                cout << "Congratulations! You Got a Bonus\nYou got addition of 50 points" << endl;
                score += 50;
                show_bonus(array, size);
                array[row][col] = ' ';
                continue;
            }
            else if (array[row][col] != '?' && array[row][col] != 'b') {
                cout << "Cell already revealed. Choose another cell." << endl;
                continue;
            }
            else {
                if (size == 5) {
                    score += 15;

                }
                else if (size == 7) {
                    score += 10;

                }
                else {
                    score += 5;

                }

                int adjacent = bomb_distance(array, row, col);
                if (adjacent > 0) {
                    array[row][col] = '0' + adjacent;
                }
                else {
                    show_cell(array, row, col);
                }

                Board_Printing(array, size);

                //logic for board clearing (for winning)

                int unrevealed;
                if (size == 5)
                {
                    unrevealed = 25;
                }
                else if (size == 7)
                {
                    unrevealed = 49;
                }
                else if (size == 13)
                {
                    unrevealed = 169;
                }

                for (int i = 0; i < size; ++i) {
                    for (int j = 0; j < size; ++j) {
                        if (array[i][j] == '?') {
                            unrevealed--;
                        }
                    }
                }

                if (size == 5 && unrevealed == 5) {
                    cout << "Congratulations! You've cleared the board." << endl;
                    if (score >= current_score) {
                        ScoreUpDater(username, score); // Update the score in the file
                    }
                    fout.close();
                    score_display(score, username);
                    return;
                }
                if (size == 7 && unrevealed == 17) {
                    cout << "Congratulations! You've cleared the board." << endl;
                    cout << "Your final score is " << score << " points." << endl;
                    if (score >= current_score) {
                        ScoreUpDater(username, score); // Update the score in the file
                    }
                    score_display(score, username);
                    return;
                }
                if (size == 13 && unrevealed == 25) {
                    cout << "Congratulations! You've cleared the board." << endl;
                    cout << "Your final score is " << score << " points." << endl;
                    if (score >= current_score) {
                        ScoreUpDater(username, score); // Update the score in the file
                    }
                    score_display(score, username);
                    return;
                }


            }

            time_t currentTime;
            time(&currentTime);
            double elapsedTime = difftime(currentTime, startTime);

            if (size == 5) {
                if (elapsedTime >= 40) {
                    cout << "Time's up! Game Over." << endl;

                    if (score >= current_score) {
                        ScoreUpDater(username, score); // Update the score in the file
                    }
                    fout.close();
                    score_display(score, username);
                    break;
                }
            }

            else if (size == 7) {
                if (elapsedTime >= 60) {
                    cout << "Time's up! Game Over." << endl;

                    if (score >= current_score) {
                        ScoreUpDater(username, score); // Update the score in the file
                    }
                    fout.close();
                    score_display(score, username);
                    break;
                }
            }

            else {
                if (elapsedTime >= 80) {
                    cout << "Time's up! Game Over." << endl;

                    if (score >= current_score) {
                        ScoreUpDater(username, score); // Update the score in the file
                    }

                    fout.close();
                    score_display(score, username);
                    break;
                }
            }

        }


    }


    else {
        cout << "error\n";
    }

    TopScorers();
}

void TopScorers() {
    fin.open("temp.txt", ios::in);

    if (fin.is_open()) {

        string user;
        int fscore;
        int highest[3] = { 0 , 0 , 0 };
        string userWithHightestScores[3] = { "No user" , "No user" , "No user" };

        while (fin >> user >> fscore) {

            if (fscore > highest[0]) {
                highest[2] = highest[1];
                userWithHightestScores[2] = userWithHightestScores[1];

                highest[1] = highest[0];
                userWithHightestScores[1] = userWithHightestScores[0];

                highest[0] = fscore;
                userWithHightestScores[0] = user;
            }
            else if (fscore > highest[1]) {
                highest[2] = highest[1];
                userWithHightestScores[2] = userWithHightestScores[1];

                highest[1] = fscore;
                userWithHightestScores[1] = user;
            }
            else if (fscore > highest[2]) {
                highest[2] = fscore;
                userWithHightestScores[2] = user;
            }
        }


        leaderboard(userWithHightestScores, highest);

        fin.close();
    }



    else {
        cout << "error\n";
    }
}

void playAgain()
{
    string  Rchoice;

    while (true)
    {
        cout << "\nDo you want to play again? \n" << endl;
        cout << CYAN_COLOR << "Enter y for YES any other input will be considerd as NO" << RESET_COLOR << endl;
        cin >> Rchoice;

        cin.ignore(10000, '\n');

        if (Rchoice == "y")
        {
            srand(time(0));
            char array[BOARD_SIZE][BOARD_SIZE];
            string username;


            //initializing array with question mark symbols
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    array[i][j] = '?';
                }
            }

            //board and mine size depends on user choice
            int BOARD_SIZE_CHOICE = difficulty_selection();
            int MINES_COUNT = mine_selection(BOARD_SIZE_CHOICE);
            place_mines_and_bonus(array, BOARD_SIZE_CHOICE, MINES_COUNT);
            main_game_play(array, BOARD_SIZE_CHOICE, MINES_COUNT, username);

        }
        else
        {
            TopScorers();
            break;
        }
    }
}