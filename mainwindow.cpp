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

    QHBoxLayout *towers = new QHBoxLayout();
    QLabel *toptext = new QLabel(With_ai);
    toptext->setText("Tower Blaster With AI"); 
    toptext->setAlignment(Qt::AlignCenter);
    toptext->setStyleSheet("font-weight: bold; font-size: 16px;");

    QVBoxLayout *tower1 = new QVBoxLayout();
    QVBoxLayout *tower2 = new QVBoxLayout();

    ai_drawncard = deckai.draw_rand_card();
    ai_drawn_card_label->setText(("Drawn Card: " + to_string(ai_drawncard)).c_str());
    ai_drawn_card_label->setAlignment(Qt::AlignCenter);
    ai_drawn_card_label->setStyleSheet("font-weight: bold; font-size:14px;");

    ai_newcard->setText("Draw New Card");
    ai_discardcard->setText("Discard move");
    ai_move->setText("");
    ai_restart->setText("Restart Game");

    for (int i = 0; i < 10; ++i) {
        QPushButton *button1 = new QPushButton(player1v1loc);
        button1->setText(to_string(deckai.draw_rand_card()).c_str());
        ai_buttons1[i]=button1;
        tower1->addWidget(button1);
        button1->setProperty("button_index", i); 

        connect(button1,SIGNAL(clicked()), this, SLOT(ai_take_turn()));

        QPushButton *button2 = new QPushButton(player1v1loc);
        button2->setText(to_string(deckai.draw_rand_card()).c_str());
        ai_buttons2[i]=button2;
        button2->setEnabled(false);
        tower2->addWidget(button2);
        button2->setProperty("button_index", i);

    }

    mode->setText("Switch to hard Mode");

    ai_menubutton->setText("Main Menu");
    ai_move->setText("");

    connect(ai_newcard, SIGNAL(clicked()), this, SLOT(On_draw_clicked_ai()));
    connect(ai_discardcard, SIGNAL(clicked()), this, SLOT(On_discard_clicked_ai()));
    connect(ai_restart, SIGNAL(clicked()), this, SLOT(On_restart_clicked_ai()));
    connect(ai_menubutton, SIGNAL(clicked()), this, SLOT(On_mainmenu_clicked()));
    connect(mode, SIGNAL(clicked()), this, SLOT(hard_mode_switch()));


    towers->addLayout(tower1);
    towers->addLayout(tower2);

    master_ailayout->addWidget(toptext);
    master_ailayout->addLayout(towers);
    master_ailayout->addWidget(ai_drawn_card_label);
    master_ailayout->addWidget(ai_move);
    master_ailayout->addWidget(ai_newcard);
    master_ailayout->addWidget(ai_discardcard);
    master_ailayout->addWidget(mode);
    master_ailayout->addWidget(ai_restart);
    master_ailayout->addWidget(ai_menubutton);

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

void MainWindow::On_draw_clicked_ai() {
    deckai.discard_card(ai_drawncard);
    ai_drawncard = deckai.draw_rand_card();
    ai_drawn_card_label->setText(("Drawn Card: " + to_string(ai_drawncard)).c_str());
    ai_newcard->setEnabled(false);
}

int score_fit(int card, int low, int high) {
    if (card > low && card < high) {
        return 100 - abs((high + low)/2 - card);  // best fit in center
    }
    // penalty if outside the ideal interval
    return -abs(card - (high + low)/2);
}


int MainWindow::calculate_inversions(array<QPushButton*, 10> tower) {
    array<int,10> vals;
    for(int i=0; i<10; i++) vals[i] = tower[i]->text().toInt();
    int inv = 0;
    for(int i=0; i<10; i++) {
        for(int j=i+1; j<10; j++) {
            if(vals[i] > vals[j]) inv++;
        }
    }
    return inv;
}

void MainWindow::On_discard_clicked_ai() {
    if (mode_selected == false) {
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

            int s = score_fit(ai_drawncard, low, high);

            if (s > bestScore) {
                bestScore = s;
                bestPos = i;
            }
        }

        if (bestScore<0)
        {

            deckai.discard_card(ai_drawncard);
            ai_drawncard = deckai.draw_rand_card();
            ai_drawn_card_label->setText(("Ai Draws Card: " + to_string(ai_drawncard)).c_str());
            // Score how card fits each position
            for (int i = 0; i < 10; i++) {
                int low = ranges[i].first;
                int high = ranges[i].second;

                int s = score_fit(ai_drawncard, low, high);

                if (s > bestScore) {
                    bestScore = s;
                    bestPos = i;
                }
            }
        }
        if (bestScore>0) {
            cout << "AI places card at position " << bestPos << "\n";
            int num=ai_buttons2[bestPos]->text().toInt();
            ai_buttons2[bestPos]->setText(QString::number(ai_drawncard));
            ai_drawncard=num;
            ai_drawn_card_label->setText(("Drawn Card: " + to_string(ai_drawncard)).c_str());
        }
        
        ai_move->setText(("AI places card at position " + to_string(bestPos)).c_str());
    }

    //hard mode
    if(mode_selected==true){
        int current_inv = calculate_inversions(ai_buttons2);
        int min_inv = INT_MAX;
        int best_pos = -1;

        for(int i = 0; i < 10; i++) {
            array<int,10> tower_vals;
            for(int j=0; j<10; j++) tower_vals[j] = ai_buttons2[j]->text().toInt();
            tower_vals[i] = ai_drawncard;
            int inv = 0;
            for(int a=0; a<10; a++) {
                for(int b=a+1; b<10; b++) {
                    if(tower_vals[a] > tower_vals[b]) inv++;
                }
            }
            if(inv < min_inv || (inv == min_inv && rand() % 2 == 0)) {
                min_inv = inv;
                best_pos = i;
            }
        }

        if(min_inv < current_inv) {
            // place
            int num = ai_buttons2[best_pos]->text().toInt();
            ai_buttons2[best_pos]->setText(QString::number(ai_drawncard));
            ai_drawncard = num;
            ai_drawn_card_label->setText(("Drawn Card: " + to_string(ai_drawncard)).c_str());
            ai_move->setText(("AI places card at position " + to_string(best_pos)).c_str());
        } else {
            // discard
            deckai.discard_card(ai_drawncard);
            ai_drawncard = deckai.draw_rand_card();
            ai_drawn_card_label->setText(("AI Draws Card: " + to_string(ai_drawncard)).c_str());
            ai_move->setText("AI discards and draws new card");

            // now try to place the new card
            current_inv = calculate_inversions(ai_buttons2);
            min_inv = INT_MAX;
            best_pos = -1;
            for(int i = 0; i < 10; i++) {
                array<int,10> tower_vals;
                for(int j=0; j<10; j++) tower_vals[j] = ai_buttons2[j]->text().toInt();
                tower_vals[i] = ai_drawncard;
                int inv = 0;
                for(int a=0; a<10; a++) {
                    for(int b=a+1; b<10; b++) {
                        if(tower_vals[a] > tower_vals[b]) inv++;
                    }
                }
                if(inv < min_inv || (inv == min_inv && rand() % 2 == 0)) {
                    min_inv = inv;
                    best_pos = i;
                }
            }

            // place the new card
            // still not better, place anyway in the best pos
            int num = ai_buttons2[best_pos]->text().toInt();
            ai_buttons2[best_pos]->setText(QString::number(ai_drawncard));
            ai_drawncard = num;
            ai_drawn_card_label->setText(("Drawn Card: " + to_string(ai_drawncard)).c_str());
            ai_move->setText(("AI places new card at position " + to_string(best_pos)).c_str());
            
        }
    }

    ai_newcard->setEnabled(true);
}

void MainWindow::On_restart_clicked_ai() {
    deckai.refill();
    ai_drawncard = deckai.draw_rand_card();
    ai_drawn_card_label->setText(("Drawn Card: " + to_string(ai_drawncard)).c_str());
    ai_newcard->setEnabled(true);
    ai_discardcard->setEnabled(true);

    for (int i = 0; i < 10; ++i) {
        ai_buttons1[i]->setText(to_string(deckai.draw_rand_card()).c_str());
        ai_buttons1[i]->setEnabled(true);
        ai_buttons2[i]->setText(to_string(deckai.draw_rand_card()).c_str());
        ai_buttons2[i]->setEnabled(false);
    }

}

void MainWindow::ai_take_turn() {

    QPushButton *button = qobject_cast<QPushButton*>(sender());

    int button_index = button->property("button_index").toInt();

    newcard->setEnabled(true);

    int temp = ai_drawncard;
    ai_drawncard = button->text().toInt();
    button->setText(QString::number(temp));

    ai_drawn_card_label->setText(("Drawn Card: " + to_string(ai_drawncard)).c_str());

    if (checkwin(ai_buttons1)) {
        ai_drawn_card_label->setText("Player 1 Wins!");
        for(int i = 0; i < 10; i++) {
            ai_buttons1[i]->setEnabled(false);
            ai_buttons2[i]->setEnabled(false);
            ai_discardcard->setEnabled(false);
        }
    } 
    else if (checkwin(ai_buttons2)) {
        ai_drawn_card_label->setText("AI Wins!");
        for(int i = 0; i < 10; i++) {
            ai_buttons1[i]->setEnabled(false);
            ai_buttons2[i]->setEnabled(false);
            ai_discardcard->setEnabled(false);
        }
    }
    else {
        On_discard_clicked_ai();
    }
}

void MainWindow::hard_mode_switch() {
    mode_selected = !mode_selected;
    if (mode_selected) {
        mode->setText("Switch to easy Mode");
        QMessageBox::information(this, "Mode Switch", "Hard Mode Activated: AI will minimize inversions.");
    } else {
        mode->setText("Switch to hard Mode");
        QMessageBox::information(this, "Mode Switch", "Easy Mode Activated: AI will use range fitting.");
    }
}

void MainWindow::On_mainmenu_clicked() {
    On_restart_clicked();
    On_restart_clicked_ai();
    stackedWidget->setCurrentWidget(mainmenu);
}  

void MainWindow::ai() {
    stackedWidget->setCurrentWidget(With_ai);
    //QMessageBox::information(this, "Info", "AI Mode is under construction.");
}

void MainWindow::p1v1_click() {
    stackedWidget->setCurrentWidget(player1v1loc);
}