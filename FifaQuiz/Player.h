#ifndef _PLAYERCLASS_
#define _PLAYERCLASS_

#include <string>
using namespace std;
#include<iostream>
class Player
{
public:
    // variables to store the teams
    string SelectedCount;
    string SelectedLeague;
    string SelectedTeam;

    // all methods display different : countries, leagues and teams
    void DisplayingCountries();
    void SelectingLeagues();
    void  SelectingTeam();

    //function to getname of users
    void GetName();
    // function to show score
    void ShowScore();


    int Score;
    string Team;
    //new score variable to store the scores when match goes to penalties
    int PenScore;
    // this variable will  be used to for the penalties in Sudden DEATH"
    int advantage = -1;

    //constructor
    Player() {

        Score = 0;
        PenScore = 0;
    }
private:
    string Name;
};

#endif // !_PLAYERCLASS_

