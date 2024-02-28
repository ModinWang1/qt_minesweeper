/*
Author: Modin Wang  

Date: 06/02/24
*/

#include <QApplication>
#include <QtGui>
#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <iostream>
#include <random>
#include <vector>
#include <MyButton.h>
#include <QMessageBox>
#include <MainWindow.h>

using namespace std;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), buttonGrid(new QGridLayout), remainingCells(30 * 16 - 99), gameGoingOn(true) {


    setLayout(buttonGrid);
    initializeGrid();
    placeMines();

}

void MainWindow::placeMines() {
    // randomly generate 99 bomb positions
    int numberOfMines = 99;
    while (numberOfMines > 0) {
        int row = rand() % 16;
        int col = rand() % 30;

        // set cell to be mine
        if (!grid[row][col].isMine) {
            grid[row][col].isMine = true;
            --numberOfMines;
        }
    }

    // loop through every cell to find the number of adjacent mines 
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLUMNS; ++col) {

            //if it isnt a mine we want to give it a number
            if (!grid[row][col].isMine) {
                int adjacentMines = 0; 

                // loop through all the neighbours
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        int nr = row + dr;
                        int nc = col + dc;
                        if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLUMNS && grid[nr][nc].isMine) {
                            ++adjacentMines;
                        }
                    }
                }
                grid[row][col].adjacentMines = adjacentMines;
            }
        }
    }
    

}


void MainWindow::initializeGrid() {

    // Loop over grid and add buttons
    grid.resize(16);
    for (int i = 0; i < ROWS; i++) {
        grid[i].resize(32);
        for(int j = 0; j < COLUMNS; j++) {

            //Create button
            MyButton *button = new MyButton(this);

            button->setGeometry(QRect(QPoint(j * 30, i * 30), QSize(30,30)));
            connect(button, &MyButton::leftClicked, [this, i, j]() { handleLeftClick(i, j); });
            connect(button, &MyButton::rightClicked, [this, i, j]() {handleRightClick(i, j); });

            // Add button to grid
            buttonGrid->addWidget(button, i, j);
        }

    }


}

void MainWindow::handleLeftClick(int row, int col) {


        
        switch (grid[row][col].state) {
            
            // if cell is in hidden state invoke reveal
            case CellState::Hidden:
                revealCell(row, col);
                break;

            // if cell state is flagged then do nothing
            case CellState::Flagged:
                break;

            case CellState::QuestionMark:
                break;
                
            default:
                break;
    }


}

void MainWindow::handleRightClick(int row, int col) {

    MyButton *button = qobject_cast<MyButton*>(buttonGrid->itemAtPosition(row, col)->widget());
    
        // right click logic
        switch (grid[row][col].state) {

            // if a cell is hidden then change the icon to a mine to flag it
            case CellState::Hidden:
                button->setIcon(QIcon("mine_flag.png"));
                grid[row][col].state = CellState::Flagged;
                break;

            // if its flagged then change it to a null icon and set to question mark
            case CellState::Flagged:
                button->setIcon(QIcon());
                button->setText("?");
                grid[row][col].state = CellState::QuestionMark;
                break;
            
            // if question mark then set to no text (default)
            case CellState::QuestionMark:
                button->setText("");
                grid[row][col].state = CellState::Hidden;
                break;
            default:
                break;
        }
}

// Function for revealing a cell, if bomb then lose, if not reveal neighbours 
void MainWindow::revealCell(int row, int col) {

    MyButton *button = qobject_cast<MyButton*>(buttonGrid->itemAtPosition(row, col)->widget());
    button->setDisabled(true);

    if (grid[row][col].isMine) {
        button->setIcon(QIcon("bomb_explode.png"));
        gameOver(false);
        return;
    }

    // already revealed stop recursion
    if (grid[row][col].state == CellState::Revealed) {
        return; 
    }

    grid[row][col].state = CellState::Revealed;
    --remainingCells;

    // recursively reveal cells
    if (grid[row][col].adjacentMines > 0) {
        button->setText(QString::number(grid[row][col].adjacentMines));
    } else {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = row + dr;
                int nc = col + dc;
                if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLUMNS &&
                    // only reveal when not mine
                    grid[nr][nc].state == CellState::Hidden && !grid[nr][nc].isMine) {
                    revealCell(nr, nc);
                }
            }
        }
    }

    checkWin();
}

void MainWindow::gameOver(bool win) {
    QMessageBox msgBox;
    if (win) {
        msgBox.setText("Congratulations! You win!");
        gameGoingOn = false;
    } else {
        msgBox.setText("You hit a mine. Game Over!");
    }
    msgBox.exec();

    //set game on going to false so stops checking for win condition
    gameGoingOn = false;
}

// helper to check if the game is done
void MainWindow::checkWin() {
    if (gameGoingOn && remainingCells == 0) {
        gameOver(true);
    }
}