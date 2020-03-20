#ifndef _QUESTIONCLASS_
#define _QUESTIONCLASS_
#include "Match.h"
#include "Player.h"



class Question : public Match, public Player
{
public:

    void askQuestion();
    //method used to validate the answer given by the user
    void QuesValidation(string correctanswer);

    string question;
    string answers;
    string correctans;

};

#endif // !_QUESTIONCLASS_

