#include "Penalty.h"
#include "Graphics.h"
#include <sqlite3.h> // Downloaded and installed the sqlite3 library
#include <vector>
#include <array>
#include <conio.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>


using nlohmann::json;

json Teams;
json Countries;


//Global Variables
int correct = -1; // used to know if the answer given was correct or not.
int counter = 0; // used to verify whose player turns is it.
int foul = -1; // used to check whether a user has commited a foul or not.
int checker = 0; // used to find number of questions asked when game goes to penalties. 

void setFontSize(int FontSize);

void setFontSize(int FontSize)
{
    CONSOLE_FONT_INFOEX info = { 0 };
    info.cbSize = sizeof(info);
    info.dwFontSize.Y = FontSize; // leave X as zero
    info.FontWeight = FW_EXTRABOLD;
    //info.FaceName = 
    //info.FaceName = L"Lucida Console";
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

//function which displays the menu
void menu();



vector<string> User_answers1; // vector to store all answers for user 1
vector<string> User_answers2; // vector to store all answers for user 2

//Declaring vectors for questions class
vector<string> ques;
vector<string> answ;
vector<string> corr;

//Declaring vectors to store teamIDs and TeamNames
vector<string> teamID;
vector<string> teamName;

//Decalting vectors to store penalties scores
vector<string> User1_vector;
vector<string> User2_vector;



//functions which retieves all the tables from the database
static int OutputQuestions(const char* s);
//int static OutputTeams(const char* s2);

//Functions which retrieve data and store the data in vectors
static int callbackQues(void* NotUsed, int argc, char** argv, char** azColName);

int static callbackTeams(void* NotUsed, int argc2, char** argv2, char** azColName2);

////having global class instances
Player user1;
Player user2;

//void CountDown();

int main()

{
    system("Color 1b");


    Countries = R"(
  {

    "Spain": ["La Liga","2nd Liga"],

    "England": ["Premier League","Championship"],

    "Italy": ["Series A","Series B"]
  }
)"_json;

    Teams = R"(
  {

    "La Liga": ["FCB","Real Madrid","Ath Madrid"],

    "2nd Liga": ["Cadiz", "Zarragoza", "Almeria"],

    "Premier League": ["Arsenal","Man U","Chelsea","Liverpool"],

    "Championship": ["Leeds U", "Birmingham City","Bristol City"],

    "Series A": ["Juventus","AC Milan","Inter Milan"],

    "Series B" : ["Benevento","Crotone","Frosione"]
  }
)"_json;

    setFontSize(20);
    PlaySound(TEXT("Background_Music.wav"), NULL, SND_LOOP | SND_ASYNC);
    // adding values so the vector is not empty
    User_answers1.push_back("S");
    User_answers2.push_back("S");

    // directing an address where the questions.db file is saved.
    const char* dir = "QUESTIONS.db";
    /*const char* dir2 = "c:\\Users\\Tushar\\Desktop\\Tables\\TEAMS.db";
    sqlite3* DB;*/

    //shows the menu
    menu();

    //stores names in each of the player's class
    cout << "Player 1 Enter your name :" << "  " << endl;
    //system("Color 6e");
    //system("Color 5");
    user1.GetName();
    cout << "Player 2 Enter your name :" << "  " << endl;
    //system("Color 9a");
    user2.GetName();



    system("cls");
    user1.DisplayingCountries();
    system("cls");
    user1.SelectingLeagues();
    system("cls");
    user1.SelectingTeam();

    system("cls");
    user2.DisplayingCountries();
    system("cls");
    user2.SelectingLeagues();
    system("cls");
    user2.SelectingTeam();
    system("cls");


    //function to display all teams on the screen
    //ShowTeams();

    //both functions are to store the selected teams to their required class instance
    //user1.SelectingTeams();
    //user2.SelectingTeams();


    cout << "\n";
    cout << "\n";





    //Calling function to retrieve tables from database file.
    OutputQuestions(dir);

    //creating instances of class Question
    Question q[25];

    //creating new variable to store the number of questions in the database
    int x = 0;
    //looping through the tables and storing them in 3 different vectors questions, answers and correctans respectively.
    for (int k = 0; k != (ques.size() - 10); k++) {

        q[k].question = ques[k];
        q[k].answers = answ[k];
        q[k].correctans = corr[k];

        x = x + 1;
    }

    int r = ((ques.size() - x) - 1); //number of questions remaining in the list

    //cout << user1.Team << "  WILL KICK OFF..." << "\n";
    //HERE a whistle kick off sound can go

    //make x times of instances of the calling method  "askQuestion" from the class Questions
    int z = 0;
    for (int i = 0; i < x; i++) {
        q[i].askQuestion();

        // if there is a foul then show the same question to the next player

        if (foul == 0) {
            q[z].askQuestion();
            foul = -1;
            z = z + 1;
        }
        else {
            z = z + 1;
            continue;
        }
    }


    cout << "FINAL RESULT:" << endl;
    user1.ShowScore();
    user2.ShowScore();
    cin.get();

    string input;
    int penalties = -1;

    if (user1.Score == user2.Score) {
        //cout << r << endl;
        cout << "Press P for penalties" << endl;
        cout << "Press D to keep it as a draw" << endl;
        cin >> input;

        if (input == "P" || input == "p") {
            system("cls");
            cout << "PENALTIES" << endl;
            penalties = 0;

        }
        if (input == "D" || input == "d") {
            cout << "Thanks for playing...." << endl;
        }
    }

    if (penalties == 0) {
        int correct = -1; // used to know if the answer given was correct or not.
        int counter = 0; // used to verify whose player turns is it.



        Penalties p[25];

        //creating new variable to store the number of questions in the database
        int b = 0;
        int w = 0;
        //looping through the tables and storing them in 3 different vectors questions, answers and correctans respectively.
        for (int w = 0; w != ques.size(); w++) {

            p[w].question = ques[w];
            p[w].answers = answ[w];
            p[w].correctans = corr[w];

            b = b + 1;
        }


        while (r < ques.size()) {

            // checks which player won by advantage. If there is any player then stop asking questions and show the final score.
            if (checker % 2 == 0 & user1.advantage == 0 & user2.advantage == -1) {
                break;
            }
            else if (checker % 2 == 0 & user1.advantage == -1 & user2.advantage == 0) {
                break;
            }
            else {
                p[r].AskQuestionPenaltyVersion();
                r++;
            }

        }

        cout << "AFTER PENALTIES:" << endl;
        cout << user1.Team << "    " << user1.Score << " " << "(" << user1.PenScore << ")" << endl;
        cout << user2.Team << "    " << user2.Score << " " << "(" << user2.PenScore << ")" << endl;
        cout << "" << endl;
        cout << "" << endl;
        cout << "                   Thanks for playing!" << endl;
        cin.get();

    }
    return 0;
}

//Functions to Retrieve  Questions from database
int static OutputQuestions(const char* s) {
    sqlite3* DB;

    int exit = sqlite3_open(s, &DB);
    int i = 0;
    string sql = "SELECT * FROM QUESTIONS;";

    sqlite3_exec(DB, sql.c_str(), callbackQues, NULL, NULL);

    return 0;
}
int static callbackQues(void* NotUsed, int argc, char** argv, char** azColName) {

    ques.push_back(argv[1]);
    answ.push_back(argv[2]);
    corr.push_back(argv[3]);




    return 0;
}

//Functions to Retrieve Teams from database
//int static OutputTeams(const char* s2) {
//    sqlite3* DB;
//
//    int exit = sqlite3_open(s2, &DB);
//    int i = 0;
//    string sql = "SELECT * FROM TEAMS;";
//
//    sqlite3_exec(DB, sql.c_str(), callbackTeams, NULL, NULL);
//
//    return 0;
//}
//int static callbackTeams(void* NotUsed, int argc2, char** argv2, char** azColName2) {
//
//    teamID.push_back(argv2[0]);
//    teamName.push_back(argv2[1]);
//
//    return 0;
//}

//function to display all teams on screen
//void ShowTeams() {
//    const char* dir2 = "c:\\Users\\Tushar\\Desktop\\Tables\\TEAMS.db";
//    sqlite3* DB;
//
//    OutputTeams(dir2);
//    // int t = -1; // to find out the number of questions
//    for (int i = 0; i < teamID.size(); i++) {
//
//
//        cout << teamID[i] << ".  " << teamName[i] << endl;
//    }
//}
//function to select teams 
//void Player::SelectingTeams() {
//
//    string PlayerChoice;
//
//    //stores the choice of the player
//    cout << Name << "     " << "Please select a team" << endl;
//    cin >> PlayerChoice;
//    cout << "\n";
//
//    //boolean expression used when team is found
//    bool match = false;
//    int k = 0;
//
//    //loop through the vector teamID to see if there is a match found
//    while (k < teamID.size() && match == false) {
//        if (PlayerChoice == teamID[k]) {
//            Team = teamName[k];
//            match = true;
//            //cout << user_team << endl;
//        }
//        else {
//            k++;
//
//        }
//    }
//    // if team does not exist on the list then output the following meesage and call same function.
//    if (match == false) {
//        cout << "team not found, please try again" << endl;
//        SelectingTeams();
//    }
//}

void Player::SelectingTeam() {
    system("Color 5F");
    cout << "" << endl;

    //Displying all teams
    for (auto it = Teams[SelectedLeague].begin(); it != Teams[SelectedLeague].end(); ++it)
    {
        std::cout << it.value() << std::endl;

    }
    cout << "" << endl;

    //Validation check
    cout << Name << "     " << "SELECT TEAM" << endl;
    cout << "" << endl;
    //cout << " Team" << endl;
    getline(cin, SelectedTeam);

    bool match = false;
    for (auto it = Teams[SelectedLeague].begin(); it != Teams[SelectedLeague].end(); ++it)
    {
        cout << "" << endl;
        if (it.value() == SelectedTeam) {
            match = true;
            Team = SelectedTeam;

            break;
        }
    }

    if (match == false) {
        cout << "Team not found, please try again" << endl;
        SelectingTeam();
        cout << "" << endl;
    }

}

void Player::SelectingLeagues() {
    system("Color 4A");
    cout << "" << endl;
    cout << "" << endl;
    //Displaying all leagues
    for (auto it = Countries[SelectedCount].begin(); it != Countries[SelectedCount].end(); ++it)
    {
        std::cout << it.value() << std::endl;
    }
    cout << "" << endl;

    //Validation check
    cout << Name << "     " << "SELECT LEAGUE" << endl;
    cout << "" << endl;
    //cout << "select a League" << endl;
    getline(cin, SelectedLeague);

    bool match = false;

    for (auto it = Countries[SelectedCount].begin(); it != Countries[SelectedCount].end(); ++it)
        /* for (auto it: Countries[])*/
    {

        if (it.value() == SelectedLeague) {
            match = true;
            break;
        }
    }

    if (match == false) {
        cout << "League not found, please try again" << endl;
        SelectingLeagues();
        cout << "" << endl;
    }
}


void Player::DisplayingCountries()
{
    system("Color 0F");

    cout << "" << endl;

    for (auto it = Countries.begin(); it != Countries.end(); ++it)
    {

        std::cout << it.key() << std::endl;
    }
    cout << "" << endl;
    //Validation check

    cout << Name << "     " << "SELECT COUNTRY" << endl;
    cout << "" << endl;
    //cout << "select a country" << endl;
    getline(cin, SelectedCount);

    bool match = false;

    for (auto it = Countries.begin(); it != Countries.end(); ++it)
    {

        if (it.key() == SelectedCount) {
            match = true;
            //cout << "team found!" << endl;
            break;
        }
    }

    if (match == false) {
        cout << "Country not found, please try again" << endl;
        DisplayingCountries();

    }
}

//function which display questions on screen
void Question::askQuestion()
{

    //PlaySound(TEXT("Background_Music.wav"), NULL, SND_LOOP | SND_ASYNC);/// add stadium sound
    system("Color 1b");
    system("cls");
    system("Color 1b");
    //Linked();
    //CountDown();
    //Live ScoreBoard
    cout << user1.Team << " | " << user1.Score << " | " << user2.Score << " | " << user2.Team << " " << endl;

    turn(); // call the turn fuctions to check whose turn is it

    // display the question on screen
    cout << "\n";
    cout << "               " << question << "\n";
    cout << "               " << answers << "\n";
    cout << "\n";

    //CountDown();
    // call the QuestionValidation function to see if the user types the correct choices A to D. 
    QuesValidation(correctans);
    //Incremenet counter by one when wuestion is answered by the player

    counter = counter + 1;

    //Save the score of the player each time they answer a question
    savingscores();
    // saving all outputs in an array for each player so could be used to find whether the player has commited a foul.
    SavingAnswers();

};


void Match::savingscores() {

    int turn = counter % 2;

    //saving score if player is odd
    if ((correct == 0) && (turn != 0)) {

        user1.Score = user1.Score + 1;


    }
    //saving score for the even player
    if ((correct == 0) && (turn == 0)) {
        user2.Score = user2.Score + 1;

    }

}

void Match::savingPenScores() {
    int turn = counter % 2;

    if ((correct == 0) && (turn != 0)) {

        user1.PenScore = user1.PenScore + 1;
        //user1.PlayerScore = Score1;

    }

    if ((correct == 0) && (turn == 0)) {
        user2.PenScore = user2.PenScore + 1;
        //user2.PlayerScore = Score2;

    }
}
Graphics goal;
Graphics missed;
Graphics PlayerFoul;



void Match::SavingAnswers() {
    int turn = counter % 2;

    // this if statement means user 1 got the right answer
    if ((correct == 0) && (turn != 0)) {


        User_answers1.push_back("C"); //push C to the user1 vector

        std::cout << "\n";

        goal.score();

        std::cout << "\n";

    }

    // this if statement means user 2 got the right answer
    if ((correct == 0) && (turn == 0)) {

        User_answers2.push_back("C");

        std::cout << "\n";
        goal.score();
        std::cout << "\n";


    }

    //this if statement is used to see if the user1 got the wrong answer
    if ((correct == -1) && (turn != 0)) {

        User_answers1.push_back("I");

        //compare the last item against the second last item in the vector
        int i = User_answers1.size();
        if (User_answers1[i - 1] == "I" && User_answers1[i - 2] == "I") {
            User_answers2.push_back("F");
            User_answers1.push_back("R");
            foul = 0;
            std::cout << "\n";
            PlayerFoul.foul();
            std::cout << "\n";
        }
        else {
            foul = -1;
            std::cout << "\n";
            missed.missed();
            std::cout << "\n";
        }
    }

    // this if statement is used to see if the user1 got the wrong answer
    if ((correct == -1) && (turn == 0)) {

        User_answers2.push_back("I");

        //compare the last item against the second last item in the vector
        int i = User_answers2.size();
        if (User_answers2[i - 1] == "I" && User_answers2[i - 2] == "I") {
            User_answers1.push_back("F");
            User_answers2.push_back("R"); // reset the vector
            foul = 0; // change foul to 0 (true)
            std::cout << "\n";
            PlayerFoul.foul(); // display foul graphics
            std::cout << "\n";
        }
        else {
            foul = -1;
            std::cout << "\n";
            missed.missed();
            std::cout << "\n";
        }
    }



}

void Match::turn() {

    //decalre a turn variable
    int turn = counter % 2;

    /*counter increments each type a new question is asked. holds a initial value of 0.
    player1 always odd and player2 always even*/

    //check whether the player is even or odd
    if (turn == 0) {

        cout << "\n";
        cout << "Is " << user1.Team << "'s turn..." << endl;
        cout << "\n";
    }
    else {
        cout << "\n";
        cout << "Is " << user2.Team << "'turn..." << endl;
        cout << "\n";
    }
}




void menu() {
    //Program Title designed with an ASCII art generator.
    //Link: http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
    //Changing Font Colors of the System

    //char d = cin.get();

    system("Color 1b");

    std::cout << R"(
                       /$$      /$$           /$$                                                  
                      | $$  /$ | $$          | $$                                                  
                      | $$ /$$$| $$  /$$$$$$ | $$  /$$$$$$$  /$$$$$$  /$$$$$$/$$$$   /$$$$$$       
                      | $$/$$ $$ $$ /$$__  $$| $$ /$$_____/ /$$__  $$| $$_  $$_  $$ /$$__  $$      
                      | $$$$_  $$$$| $$$$$$$$| $$| $$      | $$  \ $$| $$ \ $$ \ $$| $$$$$$$$      
                      | $$$/ \  $$$| $$_____/| $$| $$      | $$  | $$| $$ | $$ | $$| $$_____/      
                      | $$/   \  $$|  $$$$$$$| $$|  $$$$$$$|  $$$$$$/| $$ | $$ | $$|  $$$$$$$      
                      |__/     \__/ \_______/|__/ \_______/ \______/ |__/ |__/ |__/ \_______/      
                                               /$$$$$$$$                                           
                                              |__  $$__/                                           
                                                 | $$  /$$$$$$                                     
                                                 | $$ /$$__  $$                                    
                                                 | $$| $$  \ $$                                    
                                                 | $$| $$  | $$                                    
                       /$$$$$$$$ /$$  /$$$$$$    | $$|  $$$$$$/$$$$$$            /$$               
                      | $$_____/|__/ /$$__  $$   |__/ \______/$$__  $$          |__/               
                      | $$       /$$| $$  \__//$$$$$$       | $$  \ $$ /$$   /$$ /$$ /$$$$$$$$     
                      | $$$$$   | $$| $$$$   |____  $$      | $$  | $$| $$  | $$| $$|____ /$$/     
                      | $$__/   | $$| $$_/    /$$$$$$$      | $$  | $$| $$  | $$| $$   /$$$$/      
                      | $$      | $$| $$     /$$__  $$      | $$/$$ $$| $$  | $$| $$  /$$__/       
                      | $$      | $$| $$    |  $$$$$$$      |  $$$$$$/|  $$$$$$/| $$ /$$$$$$$$     
                      |__/      |__/|__/     \_______/       \____ $$$ \______/ |__/|________/     
                                                                  \__/                                                                             
                                                               ___
                                             o__        o__   |   |\
                                            /|          /\    |   |X\
                                            / > o        <\   |   |XX\		                                                    
)";


    //Startup message
    std::cout << "Press Enter to start...\n";
    std::cin.get();


}

void Question::QuesValidation(string correctanswer) {

    string guess; //variable that will store the answer of the player

        //User enters their answer.
    std::cout << "               " << "What is your answer?" << "\n";
    std::cin >> guess;

    //declared an array with choices available
    string options[8] = { "a","b", "c", "d", "A", "B", "C", "D" };

    int i = 0;
    int u = -1;//boolean expression used to end the following loop

    //looping through each element in the array to find out if the user's choice matches with one of the array elements
    while (i < 8 && u == -1) {
        if (guess == options[i]) {
            u = 0;
            break;
        }
        else {
            i++;

        }
    }
    if (u == -1) {
        cout << "Choice not found, please try again" << endl;
        QuesValidation(correctanswer);

    }
    string s;
    char upperans = toupper(correctans[0]);
    s.push_back(upperans);

    //change correct to 0 if answer is correct else change correct to -1.
    if (guess == correctans || guess == s) {
        correct = 0;
    }
    else {
        correct = -1;
    }
}

void Penalties::AskQuestionPenaltyVersion() {


    system("cls");
    //Linked();

    //Live ScoreBoard
    cout << user1.Team << " | " << user1.PenScore << " | " << user2.PenScore << " | " << user2.Team << " " << endl;

    turn(); // call the turn fuctions to check whose turn is it

    // display the question on screen
    cout << "\n";
    cout << question << "\n";
    cout << answers << "\n";
    cout << "\n";

    //CountDown();
    // call the QuestionValidation function to see if the user types the correct choices A to D. 
    QuesValidation(correctans);
    //Incremenet counter by one when wuestion is answered by the player

    counter = counter + 1;

    //Save the score of the player each time they answer a question
    savingPenScores();
    // saving all outputs in an array for each player so could be used to find whether the player has commited a foul.
    SavingAnsPenalties();


}

void Match::SavingAnsPenalties() {
    int turn = counter % 2;


    // this if statement means user 1 got the right answer
    if ((correct == 0) && (turn != 0)) {


        User_answers1.push_back("C"); //push C to the user1 vector
        std::cout << "\n";
        goal.score();


        user1.advantage = 0;
        checker = checker + 1;
    }
    // this if statement means user 2 got the right answer
    if ((correct == 0) && (turn == 0)) {

        User_answers2.push_back("C");
        std::cout << "\n";
        goal.score();


        user2.advantage = 0;
        checker = checker + 1;
    }

    //this if statement is used to see if the user1 got the wrong answer
    if ((correct == -1) && (turn != 0)) {

        User_answers1.push_back("I");
        std::cout << "\n";
        missed.missed();

        user1.advantage = -1;
        checker = checker + 1;
    }

    // this if statement is used to see if the user2 got the wrong answer
    if ((correct == -1) && (turn == 0)) {

        User_answers2.push_back("I");
        std::cout << "\n";
        missed.missed();

        user2.advantage = -1;
        checker = checker + 1;
    }
}







