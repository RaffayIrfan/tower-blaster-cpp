#include "mainwindow.h"
#include<QWidget>
#include<QPushButton>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QString>


#include<iostream>
#include<limits>
#include<random>
#include<vector>
#include<array>
#include<algorithm>

using namespace std;

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

    QWidget *centralWidget = new QWidget(this);
    
    QVBoxLayout *master_layout = new QVBoxLayout(this);
    QHBoxLayout *towers = new QHBoxLayout(this);

    QLabel *toptext = new QLabel(this);
    toptext->setText("Tower Blaster 1v1");
    toptext->setAlignment(Qt::AlignCenter);
    toptext->setStyleSheet("font-weight: bold; font-size: 16px;");
    master_layout->addWidget(toptext);

    QVBoxLayout *tower1 = new QVBoxLayout(this);
    QVBoxLayout *tower2 = new QVBoxLayout(this);
    
    drawn_card = deck.draw_rand_card();

    drawn_card_label->setText(("Drawn Card: " + to_string(drawn_card)).c_str());
    drawn_card_label->setAlignment(Qt::AlignCenter);
    drawn_card_label->setStyleSheet("font-weight: bold; font-size: 14px;");

    QPushButton *newcard = new QPushButton(this);
    newcard->setText("Draw New Card");

    for (int i = 0; i < 10; ++i) {
        QPushButton *button1 = new QPushButton(this);
        button1->setText(to_string(deck.draw_rand_card()).c_str());
        buttons1[i]=button1;
        tower1->addWidget(button1);
        button1->setProperty("button_index", i); 

        connect(button1,SIGNAL(clicked()), this, SLOT(On_tower_clicked()));

        QPushButton *button2 = new QPushButton(this);
        button2->setText(to_string(deck.draw_rand_card()).c_str());
        buttons2[i]=button2;
        tower2->addWidget(button2);
        button2->setProperty("button_index", i);

        connect(button2,SIGNAL(clicked()), this, SLOT(On_tower_clicked()));
    }

    connect(newcard, SIGNAL(clicked()), this, SLOT(On_draw_clicked()));

    towers->addLayout(tower1);
    towers->addLayout(tower2);

    master_layout->addLayout(towers);
    master_layout->addWidget(drawn_card_label);
    master_layout->addWidget(newcard);

    centralWidget->setLayout(master_layout);

    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::On_tower_clicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    int button_index = button->property("button_index").toInt();
     
    int temp = drawn_card;
    drawn_card = button->text().toInt();
    button->setText(QString::number(temp));

    drawn_card_label->setText(("Drawn Card: " + to_string(drawn_card)).c_str());
}

void MainWindow::On_draw_clicked() {
    deck.discard_card(drawn_card);
    drawn_card = deck.draw_rand_card();
    drawn_card_label->setText(("Drawn Card: " + to_string(drawn_card)).c_str());
}
