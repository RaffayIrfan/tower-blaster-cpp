#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    deck deck; 
    int drawn_card; 
    QLabel* drawn_card_label= new QLabel(this); 
    
    std::array<QPushButton*, 10> buttons1;
    std::array<QPushButton*, 10> buttons2;

public slots:
    void On_tower_clicked();
    void On_draw_clicked();
    //void On_discard_clicked();
};

#endif // MAINWINDOW_H
