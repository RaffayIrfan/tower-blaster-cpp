#include "mainwindow.h"

#include<QWidget>
#include<QPushButton>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QString>
#include<QMessageBox>
#include<QStackedWidget>

#include<iostream>
#include<limits>
#include<random>
#include<vector>
#include<array>
#include<algorithm>

using namespace std;

bool checkwin(array<QPushButton *, 10> tower) {
    array<int,10> button_value;
    for (int i = 0; i <10; i++) {
        button_value[i] = tower[i]->text().toInt();
    }
    for (int i = 0; i < 9; i++) {
        if (button_value[i] > button_value[i + 1]) {
            return false;
        }
    }
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Tower Blaster GUI Client");

    main_menu();
    player1v1();
    with_ai();

    stackedWidget->addWidget(mainmenu);
    stackedWidget->addWidget(player1v1loc);
    stackedWidget->addWidget(With_ai);

    stackedWidget->setCurrentWidget(mainmenu);

    setCentralWidget(stackedWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::main_menu(){

    QLabel *title = new QLabel(mainmenu);
    title->setText("Tower Blaster");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; font-size: 24px;");
    main_menulay->addWidget(title);

    QPushButton *p1v1 = new QPushButton(mainmenu);
    p1v1->setText("1v1 Player Mode");
    main_menulay->addWidget(p1v1);

    QPushButton *aibut = new QPushButton(mainmenu);
    aibut->setText("With computer Mode");
    main_menulay->addWidget(aibut);

    //QMessageBox::about(this, "About Qt Menu Example",
                           //"This is a simple application demonstrating how to create "
                           //"menus, actions, and connect them to slots in Qt Widgets (C++).");

    connect(p1v1,SIGNAL(clicked()), this, SLOT(p1v1_click()));
    connect(aibut,SIGNAL(clicked()), this, SLOT(ai()));

    mainmenu->setLayout(main_menulay);
}

void MainWindow::player1v1(){
    QHBoxLayout *towers = new QHBoxLayout();

    QLabel *toptext = new QLabel(player1v1loc);
    toptext->setText("Tower Blaster 1v1");
    toptext->setAlignment(Qt::AlignCenter);
    toptext->setStyleSheet("font-weight: bold; font-size: 16px;");
    master_layout->addWidget(toptext);

    QVBoxLayout *tower1 = new QVBoxLayout();
    QVBoxLayout *tower2 = new QVBoxLayout();
    
    drawn_card = deck.draw_rand_card();

    drawn_card_label->setText(("Drawn Card: " + to_string(drawn_card)).c_str());
    drawn_card_label->setAlignment(Qt::AlignCenter);
    drawn_card_label->setStyleSheet("font-weight: bold; font-size: 14px;");

    newcard->setText("Draw New Card");
    discardcard->setText("Discard move");
    player_move->setText("Player 1's Move");
    player_move->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < 10; ++i) {
        QPushButton *button1 = new QPushButton(player1v1loc);
        button1->setText(to_string(deck.draw_rand_card()).c_str());
        buttons1[i]=button1;
        tower1->addWidget(button1);
        button1->setProperty("button_index", i); 

        connect(button1,SIGNAL(clicked()), this, SLOT(On_tower_clicked()));

        QPushButton *button2 = new QPushButton(player1v1loc);
        button2->setText(to_string(deck.draw_rand_card()).c_str());
        buttons2[i]=button2;
        tower2->addWidget(button2);
        button2->setEnabled(false);
        button2->setProperty("button_index", i);

        connect(button2,SIGNAL(clicked()), this, SLOT(On_tower_clicked()));
    }
    restart->setText("Restart Game");
    menubutton->setText("Main Menu");

    connect(newcard, SIGNAL(clicked()), this, SLOT(On_draw_clicked()));
    connect(discardcard, SIGNAL(clicked()), this, SLOT(On_discard_clicked()));
    connect(restart, SIGNAL(clicked()), this, SLOT(On_restart_clicked()));
    connect(menubutton, SIGNAL(clicked()), this, SLOT(On_mainmenu_clicked()));
    
    towers->addLayout(tower1);
    towers->addLayout(tower2);

    master_layout->addLayout(towers);
    master_layout->addWidget(player_move);
    master_layout->addWidget(drawn_card_label);
    master_layout->addWidget(newcard);
    master_layout->addWidget(discardcard);
    master_layout->addWidget(restart);
    master_layout->addWidget(menubutton);

    player1v1loc->setLayout(master_layout);
}

void MainWindow::with_ai(){
    

    With_ai->setLayout(master_ailayout);
}

//Here are the slot implementations
void MainWindow::On_tower_clicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    int button_index = button->property("button_index").toInt();

    newcard->setEnabled(true);

    for(int i = 0; i < 10; i++) {
        if(flipflop) {
            buttons1[i]->setEnabled(false);
            buttons2[i]->setEnabled(true);
            player_move->setText("Player 2's Move");
        }
        if(!flipflop) {
            buttons2[i]->setEnabled(false);
            buttons1[i]->setEnabled(true);
            player_move->setText("Player 1's Move");
        }
    }
     
    flipflop = !flipflop;

    int temp = drawn_card;
    drawn_card = button->text().toInt();
    button->setText(QString::number(temp));

    drawn_card_label->setText(("Drawn Card: " + to_string(drawn_card)).c_str());

    if (checkwin(buttons1)) {
        drawn_card_label->setText("Player 1 Wins!");
        for(int i = 0; i < 10; i++) {
            buttons1[i]->setEnabled(false);
            buttons2[i]->setEnabled(false);
            discardcard->setEnabled(false);
        }
    } 
    else if (checkwin(buttons2)) {
        drawn_card_label->setText("Player 2 Wins!");
        for(int i = 0; i < 10; i++) {
            buttons1[i]->setEnabled(false);
            buttons2[i]->setEnabled(false);
            discardcard->setEnabled(false);
        }
    }
}

void MainWindow::On_draw_clicked() {
    deck.discard_card(drawn_card);
    drawn_card = deck.draw_rand_card();
    drawn_card_label->setText(("Drawn Card: " + to_string(drawn_card)).c_str());
    newcard->setEnabled(false);
}

void MainWindow::On_discard_clicked() {
    newcard->setEnabled(true);

    for(int i = 0; i < 10; i++) {
        if(flipflop) {
            player_move->setText("Player 2's Move");
            buttons1[i]->setEnabled(false);
            buttons2[i]->setEnabled(true);
        }
        if(!flipflop) {
            player_move->setText("Player 1's Move");
            buttons2[i]->setEnabled(false);
            buttons1[i]->setEnabled(true);
        }
    }
     
    flipflop = !flipflop;
}

void MainWindow::On_restart_clicked() {
    deck.refill();
    drawn_card = deck.draw_rand_card();
    drawn_card_label->setText(("Drawn Card: " + to_string(drawn_card)).c_str());
    player_move->setText("Player 1's Move");
    newcard->setEnabled(true);
    discardcard->setEnabled(true);

    for (int i = 0; i < 10; ++i) {
        buttons1[i]->setText(to_string(deck.draw_rand_card()).c_str());
        buttons1[i]->setEnabled(true);
        buttons2[i]->setText(to_string(deck.draw_rand_card()).c_str());
        buttons2[i]->setEnabled(false);
    }

    flipflop = true;
}

void MainWindow::On_mainmenu_clicked() {
    On_restart_clicked();
    stackedWidget->setCurrentWidget(mainmenu);
}  

void MainWindow::ai() {
    stackedWidget->setCurrentWidget(With_ai);
    QMessageBox::information(this, "Info", "AI Mode is under construction.");
}

void MainWindow::p1v1_click() {
    stackedWidget->setCurrentWidget(player1v1loc);
}