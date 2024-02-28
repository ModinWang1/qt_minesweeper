/*
Author: Modin Wang

Date: 06/02/24

*/


#include <QPushButton>
#include <QMainWindow>
#include <QGridLayout>
#include <QMessageBox>


using namespace std;

class MainWindow : public QMainWindow {


     Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);



    
private:
    enum class CellState { Hidden, Revealed, Flagged, QuestionMark };

    struct Cell {
        int adjacentMines;
        bool isMine;
        CellState state;

        Cell() : adjacentMines(0), isMine(false), state(CellState::Hidden) {}
    };
    QGridLayout *buttonGrid;
    vector<vector<Cell>> grid;

    int remainingCells;
    bool gameGoingOn;
    static const int ROWS = 16;
    static const int COLUMNS = 30;
    static const int BUTTON_SIZE = 30;
    
    

    void initializeGrid();
    void placeMines();
    void revealCell(int, int);
    void checkWin();
    void gameOver(bool);

    private slots:
    void handleLeftClick(int, int);
    void handleRightClick(int, int);



};


