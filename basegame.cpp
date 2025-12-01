#include<iostream>
#include<limits>
#include<random>
#include<vector>
#include<array>
#include<algorithm>
#include"nlohmann/json.hpp"

using namespace std;

std::mt19937& getRng() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    return rng;
}

int randomInt(int low, int high) {
    std::uniform_int_distribution<int> dist(low, high);
    return dist(getRng());
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

class deck{
    private:
        vector<int> cards;
    public:
        deck(){
            for(int i=1; i<=50; i++){
                cards.push_back(i);
            }
        }

        int draw_rand_card(){
            if(cards.empty()){
                cout << "Deck is empty!\n";
                return -1;
            }
            int rand_card_index = randomInt(0, cards.size()-1);
            int card = cards[rand_card_index];
            cards.erase(cards.begin() + rand_card_index);
            return card;
        }

        void refill(){
            cards.clear();
            for(int i=1; i<=50; i++){
                cards.push_back(i);
            }
        }
        
        void print(){
            cout << endl;
            for(int card : cards){
                cout << card << " ";
            }
        }

        void discard_card(int card){
            cards.push_back(card);
        }
};
    


bool checkwin(array<int, 10> tower) {
    for (int i = 0; i < 9; i++) {
        if (tower[i] > tower[i + 1]) {
            return false;
        }
    }
    return true;
}

void print_towers(array<int,10> tower1,array<int,10> tower2) {
    cout << "Player 1 Tower: ";
    for(int i=0; i<10; i++){
        cout << tower1[i] << " ";   
    }
    cout << endl;

    cout << "Player 2 Tower: ";
    for(int i=0; i<10; i++){
        cout << tower2[i] << " ";   
    }
    cout << endl;
}

void human_move(array<int,10> &tower, int &card, deck &deck,int player_num) {

    cout << "Player "<<player_num<<"'s turn:\n";
    cout << "Drawn card: " << card << "\n";
    cout << "Enter position to replace (1-10) or 11 to get random number from deck: \n";

    int pos;
    bool discard = false;

    // Input validation loop
    while (true) {
        cin >> pos;
        if (!cin || pos < 1 || pos > 11) {
            cout << "Invalid position. Please enter a number between 1 and 11: ";
            clearInput();
            continue;
        }
        else {
            break; // Valid input
        }
    }

    if(pos == 11){
        deck.discard_card(card);
        card = deck.draw_rand_card();
        cout << "New drawn card: " << card << "\n";
        cout << "Enter position to replace (1-10) or discard turn with 0: \n";

        // Input validation loop
        while (true) {  
            cin >> pos;
            if (!cin || pos < 0 || pos > 10) {
                cout << "Invalid position. Please enter a number between 1 and 10 or discard with 0: ";
                clearInput();
                continue;
            }
            else {
                break; // Valid input
            }
        }
    }
    if(pos != 0){
        swap(tower[pos - 1], card);
    }
}

//score functon arranges numbers but in localized format 
//first attempt on building ai with basic scoring
int towerScore(const array<int,10>& t) {
    int score = 0;

    for (int i = 0; i < 9; i++) {
        // Reward ascending pairs strongly
        if (t[i] < t[i+1]) {
            score += 20;
            // bonus for being close
            score += max(0, 10 - abs(t[i] - t[i+1]));
        }
        else {
            // Penalize descending drops
            score -= 15;
            score -= abs(t[i] - t[i+1]); 
        }
    }

    return score;
}

//takes ranges into consideration and calculates score of the 
//moving position by evaluating if the card lies in the ranges
int score_fit(int card, int low, int high) {
    if (card > low && card < high) {
        return 100 - abs((high + low)/2 - card);  // best fit in center
    }
    // penalty if outside the ideal interval
    return -abs(card - (high + low)/2);
}

void ai_move(array<int,10> &tower, int &card, deck &deck) {
    cout << "AI's turn :\n";
    cout << "Drawn card: " << card << "\n";

    //hardcoded ranges better idea than sorted ranges 
    //which can be abrupt and give placement on already sorted locality
    vector<pair<int,int>> ranges = {{1,5},{5,10},{11,15},{16,20},{21,25},
                {26,30},{31,35},{36,40},{41,45},{46,50}};

    int bestPos = -1;
    int bestScore = -1;

    // Score how card fits each position
    for (int i = 0; i < 10; i++) {
        int low = ranges[i].first;
        int high = ranges[i].second;

        int s = score_fit(card, low, high);

        if (s > bestScore) {
            bestScore = s;
            bestPos = i;
        }
    }

    if (bestScore<0)
    {
        deck.discard_card(card);
        card = deck.draw_rand_card();
        cout << "No improving move; AI draws new card: " << card << "\n";
        // Score how card fits each position
        for (int i = 0; i < 10; i++) {
            int low = ranges[i].first;
            int high = ranges[i].second;

            int s = score_fit(card, low, high);

            if (s > bestScore) {
                bestScore = s;
                bestPos = i;
            }
        }
    }

    if (bestScore>0)
    {
        cout << "AI places card at position " << bestPos << "\n";
        swap(tower[bestPos], card);
    }
    else{
        cout << "AI discards turn:" << "\n";
    }
}

void game(){
    
    cout << "Welcome to the Base Game!\n";
    cout << "This is TowerBlaster Base game\n";
    cout << "Arrange the numbers in ascending order!\n";
    cout<< "With Ai or not? 1 for Ai 0 for 1v1\n";
    bool vs_ai;
    cin >> vs_ai;
    
    deck deck;
    array<int, 10> tower1;
    array<int, 10> tower2;

    for(int i=0; i<10; i++){
        tower1[i] = deck.draw_rand_card();
        tower2[i] = deck.draw_rand_card();
    }

    int drawn_card = deck.draw_rand_card();

    while(true){
        print_towers(tower1, tower2);
    
        human_move(tower1, drawn_card, deck, 1);
        if(checkwin(tower1)){
            cout << "Player 1 wins!\n";
            print_towers(tower1, tower2);
            break;
        }

        print_towers(tower1, tower2);
        if(vs_ai == false){
            human_move(tower2, drawn_card, deck, 2);
        }
        else{
            ai_move(tower2, drawn_card, deck);
        }
        
        if(checkwin(tower2)){
            cout << "Player 2 wins!\n";
            print_towers(tower1, tower2);
            break;
        }
    }
    
}

int main(){

    game();
    return 0;
}