#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>

#include<QWidget>
#include<QPushButton>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QStackedWidget>

#include<iostream>
#include<limits>
#include<random>
#include<vector>
#include<array>
#include<algorithm>

using namespace std;

class deck{
    private:
        vector<int> cards;
    public:
        deck(){
            for(int i=1; i<=50; i++){
                cards.push_back(i);
            }
        }

        std::mt19937& getRng() {
            static std::random_device rd;
            static std::mt19937 rng(rd());
            return rng;
        }

        int randomInt(int low, int high) {
            std::uniform_int_distribution<int> dist(low, high);
            return dist(getRng());
        }

        int draw_rand_card(){
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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   
    deck deck; 
    int drawn_card;
    bool flipflop = true;

    std::array<QPushButton*, 10> buttons1;
    std::array<QPushButton*, 10> buttons2;

    QStackedWidget *stackedWidget = new QStackedWidget(this);

    QLabel* drawn_card_label= new QLabel(this); 
    QLabel* player_move= new QLabel(this); 
    QPushButton *newcard = new QPushButton(this);
    QPushButton *discardcard = new QPushButton(this);
    QPushButton *restart = new QPushButton(this);
    QPushButton *menubutton = new QPushButton(this);

    QWidget *mainmenu = new QWidget(this);
    QVBoxLayout *main_menulay = new QVBoxLayout(mainmenu);

    QWidget *player1v1loc = new QWidget(this);
    QVBoxLayout *master_layout = new QVBoxLayout(player1v1loc);

    QWidget *With_ai = new QWidget(this);
    QVBoxLayout *master_ailayout = new QVBoxLayout(With_ai);

    void player1v1();
    void main_menu();
    void with_ai();

public slots:
    void On_tower_clicked();
    void On_draw_clicked();
    void p1v1_click();
    void ai();
    void On_discard_clicked();
    void On_restart_clicked();
    void On_mainmenu_clicked();
};

#endif // MAINWINDOW_H
