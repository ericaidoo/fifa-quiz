#include "Player.h"


//function to get players names

void Player::GetName() {

    //storing player's name
    string name;
    getline(cin, name);
    cout << "\n";

    //making a class instance to store the name of 1st player
    Name = name;

}

//function to show the final result
void Player::ShowScore() {
    cout << "\n";
    cout << Team << "    " << Score << endl;
    cout << "\n";
    //cout << user2.Team << "   " << user2.Score << endl;
}

