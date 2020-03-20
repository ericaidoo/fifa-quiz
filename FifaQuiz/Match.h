#ifndef _MATCHCLASS_
#define _MATCHCLASS_



using namespace std;

class Match
{
public:
    void turn();
    //function which stores the score for each player
    void savingscores();
    //function which stores ONLY penalty scores
    void savingPenScores();
    //function which saves all the answers in a vector for each player
    void SavingAnswers();

    //function which stores answers when match gos to penalties.
    void SavingAnsPenalties();


};

#endif // !_MATCHCLASS_

