#include "mainwindow.h"
#include<QWidget>
#include<QPushButton>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>

#include<iostream>
#include<limits>
#include<random>
#include<vector>
#include<array>
#include<algorithm>

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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Tower Blaster GUI Client");

    deck deck;

    QWidget *centralWidget = new QWidget(this);
    
    QVBoxLayout *master_layout = new QVBoxLayout(this);
    QHBoxLayout *towers = new QHBoxLayout(this);

    QLabel *toptext = new QLabel(this);
    toptext->setText("Tower Blaster 1v1");
    toptext->setAlignment(Qt::AlignCenter);
    toptext->setStyleSheet("font-weight: bold; font-size: 16px;");
    master_layout->addWidget(toptext);

    array<QPushButton*,10> buttons1;
    array<QPushButton*,10> buttons2;

    QVBoxLayout *tower1 = new QVBoxLayout(this);
    QVBoxLayout *tower2 = new QVBoxLayout(this);

    for (int i = 0; i < 10; ++i) {
        QPushButton *button1 = new QPushButton(this);
        button1->setText(to_string(deck.draw_rand_card()).c_str());
        buttons1[i]=button1;
        tower1->addWidget(button1);

        QPushButton *button2 = new QPushButton(this);
        button2->setText(to_string(deck.draw_rand_card()).c_str());
        buttons2[i]=button2;
        tower2->addWidget(button2);
    }

    int drawn_card = deck.draw_rand_card();

    QLabel *drawn_card_label = new QLabel(this);
    drawn_card_label->setText(("Drawn Card: " + to_string(drawn_card)).c_str());
    drawn_card_label->setAlignment(Qt::AlignCenter);
    drawn_card_label->setStyleSheet("font-weight: bold; font-size: 14px;");

    QPushButton *newcard = new QPushButton(this);
    newcard->setText("Draw New Card");

    towers->addLayout(tower1);
    towers->addLayout(tower2);

    master_layout->addLayout(towers);
    master_layout->addWidget(drawn_card_label);
    master_layout->addWidget(newcard);

    centralWidget->setLayout(master_layout);

    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}
