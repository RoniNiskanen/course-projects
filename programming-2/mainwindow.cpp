#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <QKeyEvent>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Tetris");
    ui->gameOverFrame->setVisible(false);
    ui->newGameButton->setFocusPolicy(Qt::NoFocus);

    scene = new QGraphicsScene(this);
    sceneNextPiece = new QGraphicsScene(this);
    sceneHeldPiece = new QGraphicsScene(this);

    ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene);
    ui->graphicsViewNext->setScene(sceneNextPiece);
    ui->graphicsHold->setScene(sceneHeldPiece);

    scene->setSceneRect(21, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    int seed = time(0);
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, 6);

    connect(&timerMovePieces, &QTimer::timeout, this, &MainWindow::movePieces);
    connect(&timerTimePlayed, &QTimer::timeout, this, &MainWindow::updateSecondsPlayed);
    connect(&timerAnimations, &QTimer::timeout, this, &MainWindow::pointsAddedAnimation);
    pointsReceived = 0;
    resetGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_S) {
        if (pieceMovable(currentPieceParts, "down")){
        movePiece(currentPieceParts, "down");
        pointsReceived += 1;
        updatePointsAndLevel();
        return;
        }
    }
    else if(event->key() == Qt::Key_D) {
        if (pieceMovable(currentPieceParts, "right")){
        movePiece(currentPieceParts, "right");
        return;
        }
    }
    else if(event->key() == Qt::Key_Space) {
        while (pieceMovable(currentPieceParts, "down") == true){
                pointsReceived += 2;
                updatePointsAndLevel();
                movePiece(currentPieceParts, "down");
        }
         movePieces();
         return;
    }
    else if(event->key() == Qt::Key_A) {
        if (pieceMovable(currentPieceParts, "left")){
        movePiece(currentPieceParts, "left");
        return;
    }
    }
    else if(event->key() == Qt::Key_R) {
        resetGame();
        return;
    }
    else if(event->key() == Qt::Key_F) {
        holdPiece();
        return;
    }
    else if(event->key() == Qt::Key_W) {
        rotatePiece();
        return;
    }
}

void MainWindow::movePieces()
{
    if (gameRunning){
        updatePointsAndLevel();
        if (pieceMovable(currentPieceParts, "down") == true) {
            movePiece(currentPieceParts, "down");
            } else {
            for (unsigned int i=0; i<currentPieceParts.size(); i++ ) {
                int CoordX_ = currentPieceParts[i]->x() / 20;
                int CoordY_ = currentPieceParts[i]->y() / 20;
                tetrisRows[CoordY_][CoordX_] = currentPieceParts[i]->data(colorNumber).toInt();
                nextPieceParts.clear();
                sceneNextPiece->clear();
            }
            updateBoard();
            currentPieceParts.clear();
            deleteFullRows();
            nextPiece = upcomingPiece;
            choosePiece(upcomingPiece);
            showNextPiece(upcomingPiece);
            addNextPieceToBoard(nextPiece);
            alreadyStoredPiece = false;
        }
    } else {
        ui->gameOverFrame->setVisible(true);
    }
}

void MainWindow::movePiece(std::vector<QGraphicsRectItem *>, std::string direction)
{
    if (gameRunning) {
    if (direction == "down"){
    for(unsigned int i=0; i<currentPieceParts.size(); i++) {
        currentPieceParts[i]->moveBy(0,20);
}
        }
    else if (direction == "left"){
    for(unsigned int i=0; i<currentPieceParts.size(); i++) {
        currentPieceParts[i]->moveBy(-20,0);
}
        }
    else if (direction == "right"){
    for(unsigned int i=0; i<currentPieceParts.size(); i++) {
        currentPieceParts[i]->moveBy(20,0);
}
        }
    }
}

void MainWindow::addNextPieceToBoard(std::vector<std::vector<int>> new_piece) {

    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    QLinearGradient gradientP(QPointF(1,1),QPointF(10,10));
    gradientP.setColorAt(0, Qt::white);

    for (unsigned int i = 0; i<new_piece.size();i++ ){
        int posX = new_piece[i][0];
        int posY = new_piece[i][1];
        int color_number = new_piece[i][2];
        int second_form = new_piece[i][3];

        if (tetrisRows[posY/20][posX/20] != 0){
                gameRunning = false;
            }

        if (color_number == 1) { gradientP.setColorAt(1,Qt::red); }
        else if (color_number == 2) {gradientP.setColorAt(1,Qt::blue); }
        else if (color_number == 3) {gradientP.setColorAt(1,Qt::cyan); }
        else if (color_number == 4) {gradientP.setColorAt(1,Qt::yellow); }
        else if (color_number == 5) {gradientP.setColorAt(1,Qt::green); }
        else if (color_number == 6) {gradientP.setColorAt(1,Qt::magenta); }
        else if (color_number == 7) {gradientP.setColorAt(1,Qt::darkCyan); }

        QBrush pieceBrush(gradientP);
        pieceBrush.setStyle(Qt::LinearGradientPattern);

        square = scene->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, blackPen, pieceBrush);
        square->setPos(posX,posY);
        square->setData(secondForm, second_form);
        square->setData(colorNumber, color_number);
        currentPieceParts.push_back(square);
    }
}

void MainWindow::showNextPiece(std::vector<std::vector<int>> new_piece){
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    QLinearGradient gradientNP(QPointF(1,1),QPointF(10,10));
    gradientNP.setColorAt(0, Qt::white);

    for (unsigned int i = 0; i<new_piece.size();i++ ){
        int posX = new_piece[i][0];
        int posY = new_piece[i][1];
        int color_number = new_piece[i][2];

        if (color_number == 1) { gradientNP.setColorAt(1,Qt::red); }
        else if (color_number == 2) {gradientNP.setColorAt(1,Qt::blue); }
        else if (color_number == 3) {gradientNP.setColorAt(1,Qt::cyan); }
        else if (color_number == 4) {gradientNP.setColorAt(1,Qt::yellow); }
        else if (color_number == 5) {gradientNP.setColorAt(1,Qt::green); }
        else if (color_number == 6) {gradientNP.setColorAt(1,Qt::magenta); }
        else if (color_number == 7) {gradientNP.setColorAt(1,Qt::darkCyan); }

        QBrush pieceBrush(gradientNP);
        pieceBrush.setStyle(Qt::LinearGradientPattern);

        nextSquare = sceneNextPiece->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, blackPen, pieceBrush);
        nextSquare->setPos(posX,posY);
        nextSquare->setData(colorNumber, color_number);
        nextPieceParts.push_back(nextSquare);
    }
}

void MainWindow::showHeldPiece(std::vector<std::vector<int>> hold_piece){
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    QLinearGradient gradientHP(QPointF(1,1),QPointF(10,10));
    gradientHP.setColorAt(0, Qt::white);

    for (unsigned int i = 0; i<hold_piece.size();i++ ){
        int posX = hold_piece[i][0];
        int posY = hold_piece[i][1];
        int color_number = hold_piece[i][2];

        if (color_number == 1) { gradientHP.setColorAt(1,Qt::red); }
        else if (color_number == 2) {gradientHP.setColorAt(1,Qt::blue); }
        else if (color_number == 3) {gradientHP.setColorAt(1,Qt::cyan); }
        else if (color_number == 4) {gradientHP.setColorAt(1,Qt::yellow); }
        else if (color_number == 5) {gradientHP.setColorAt(1,Qt::green); }
        else if (color_number == 6) {gradientHP.setColorAt(1,Qt::magenta); }
        else if (color_number == 7) {gradientHP.setColorAt(1,Qt::darkCyan); }

        QBrush pieceBrush(gradientHP);
        pieceBrush.setStyle(Qt::LinearGradientPattern);

        heldSquare = sceneHeldPiece->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, blackPen, pieceBrush);
        heldSquare->setPos(posX,posY);
        heldSquare->setData(colorNumber, color_number);
        heldPieceParts.push_back(heldSquare);
    }
}

void MainWindow::setupBoard(){
    tetrisRows.clear();
    for (int i=0; i<24; i++){
        tetrisRows.push_back(TETRIS_ROW_EMPTY);
    }
   tetrisRows.push_back(TETRIS_ROW_SOLID);
   updateBoard();
}

void MainWindow::updateBoard(){

    QPen boardPen(Qt::black);
    boardPen.setWidth(2);
    int board_color_number = 1;

    for (int i=1; i<13; i++){
        for (int j=0;j<24; j++){
            if (tetrisRows[j][i] != 0){
                board_color_number = tetrisRows[j][i];

                QLinearGradient gradientB(QPointF(i*20,j*20),QPointF(i*20+10,j*20+10));
                gradientB.setColorAt(0, Qt::white);

                if (board_color_number == 1 ) {gradientB.setColorAt(1,Qt::red); }
                else if (board_color_number == 2) {gradientB.setColorAt(1,Qt::blue); }
                else if (board_color_number == 3) {gradientB.setColorAt(1,Qt::cyan); }
                else if (board_color_number == 4) {gradientB.setColorAt(1,Qt::yellow); }
                else if (board_color_number == 5) {gradientB.setColorAt(1,Qt::green); }
                else if (board_color_number == 6) {gradientB.setColorAt(1,Qt::magenta); }
                else if (board_color_number == 7) {gradientB.setColorAt(1,Qt::darkCyan); }
                else if (board_color_number == 0) {gradientB.setColorAt(1,Qt::color0); }

               QBrush boardPieceBrush(gradientB);
               boardPieceBrush.setStyle(Qt::LinearGradientPattern);
               scene -> addRect(i*20, j*20, SQUARE_SIDE, SQUARE_SIDE, boardPen, boardPieceBrush);
            }
        }
    }
}

bool MainWindow::squareMovable(QGraphicsRectItem* square, std::string direction){
    int CoordX_ = square->x() / 20;
    int CoordY_ = square->y() / 20;
    if (direction == "down") {
        if (tetrisRows[CoordY_+1][CoordX_] == 0){
            return true;
        } else {
            return false;
        }
    } else if (direction == "left"){
        if (tetrisRows[CoordY_][CoordX_-1] == 0){
            return true;
        } else {
            return false;
        }
    } else if (direction == "right") {
        if (tetrisRows[CoordY_][CoordX_+1] == 0){
            return true;
        } else {
            return false;
        }
    }
    else return false;
}

bool MainWindow::pieceMovable(std::vector<QGraphicsRectItem*> squares, std::string direction){

    for (unsigned int i=0; i < squares.size(); i++) {
        if (squareMovable(squares[i], direction) == false){
            return false;
        }
    }
    return true;
}

void MainWindow::choosePiece(std::vector<std::vector<int>>& piece){
    int tempInt = distr(randomEng);
    if (tempInt == 0){
        piece = HORIZONTAL_PIECE;
    }
    else if (tempInt == 1){
        piece = LEFT_CORNER_PIECE;
    }
    else if (tempInt == 2){
        piece = RIGHT_CORNER_PIECE;
    }
    else if (tempInt == 3){
        piece = SQUARE_PIECE;
    }
    else if (tempInt == 4){
        piece = PYRAMID_PIECE;
    }
    else if (tempInt == 5){
        piece = STEP_UP_LEFT_PIECE;
    }
    else if (tempInt == 6){
        piece = STEP_UP_RIGHT_PIECE;
    }
}

void MainWindow::resetGame()
{
    maintainHiscores();
    currentPieceParts.clear();
    nextPieceParts.clear();
    heldPieceParts.clear();
    heldPiece.clear();
    scene->clear();
    sceneNextPiece->clear();
    sceneHeldPiece->clear();
    setupBoard();

    choosePiece(nextPiece);
    choosePiece(upcomingPiece);

    secondsPlayed = 0;
    pointsReceived = 0;
    pointsOld = 0;
    level = 1;
    pieceSpeed = 1000;
    alreadyStoredPiece = false;

    ui->lcdNumberSec->display(secondsPlayed);
    ui->lcdNumberPoints->display(pointsReceived);
    ui->lcdNumberLevel->display(level);
    gameRunning = true;

    timerMovePieces.start(pieceSpeed);
    timerAnimations.start(20);
    timerTimePlayed.start(1000);

    addNextPieceToBoard(nextPiece);
    showNextPiece(upcomingPiece);
    ui->gameOverFrame->setVisible(false);
}

void MainWindow::deleteFullRows()
{
    int rows_deleted = 0;
    unsigned int highestRowDeleted = 24; // 24 is lowest
    QString pointsText = "";
    for (unsigned int i = 1; i < tetrisRows.size()-1; i++) {
        bool isFilled = true;
        for (int j = 1; j < 13; j++) {
            if (tetrisRows[i][j] == 0) {
                isFilled = false;
            }
        }
        if (isFilled) { // Rivi i täynnä
            if(highestRowDeleted > i){highestRowDeleted = i;}
            tetrisRows.erase(tetrisRows.begin()+i);
            std::vector<std::vector<int>>::iterator it;
            it = tetrisRows.begin();
            tetrisRows.insert(it, TETRIS_ROW_EMPTY);
            scene->clear();
            updateBoard();
            rows_deleted++;
        }
    }


    if(rows_deleted == 1){pointsReceived += 100; pointsText = "+100";}
    else if(rows_deleted == 2){pointsReceived += 300; pointsText = "+300";}
    else if(rows_deleted == 3){pointsReceived += 500; pointsText = "+500";}
    else if(rows_deleted == 4){pointsReceived += 800; pointsText = "+800";}

    if(rows_deleted != 0){
    ui->pointsAddedLabel->setText(pointsText);
    ui->pointsAddedLabel->setGeometry(240,490-20*(24-highestRowDeleted),100,100);
    ui->pointsAddedLabel->setVisible(true);
    updatePointsAndLevel();
    animationLoops = 0;
    }
}

void MainWindow::pointsAddedAnimation(){
    int tempY = ui->pointsAddedLabel->y();
    if(ui->pointsAddedLabel->isVisible()){
            animationLoops++;
            if(tempY > 10){
                ui->pointsAddedLabel->setGeometry(240,tempY-1,100,100);
                //QFont f("Arial", fontSize_, QFont::Bold);
                //ui->pointsAddedLabel->setFont(f);
            }
            if(animationLoops > 100){
                ui->pointsAddedLabel->setVisible(false);
                animationLoops = 0;
            }
    }
}

void MainWindow::updateSecondsPlayed(){
    if(gameRunning){
    ++secondsPlayed;
    ui->lcdNumberSec->display(secondsPlayed);
    }
}

void MainWindow::updatePointsAndLevel(){
    if(pointsOld <= (pointsReceived-1000) && level < 10){
        level += 1;
        pieceSpeed -= 100;
        timerMovePieces.setInterval(pieceSpeed);
        pointsOld = pointsReceived;
    }
    ui->lcdNumberPoints->display(pointsReceived);
    ui->lcdNumberLevel->display(level);
}

void MainWindow::rotatePiece(){
    bool rotatable = true;

    int offsetX = currentPieceParts[0]->x()-120;
    int offsetY = currentPieceParts[0]->y();
    int secondFormTemp = currentPieceParts[0]->data(secondForm).toInt();

    std::vector<std::vector<int>> rotatedPiece = ALL_PIECES[secondFormTemp];

    for (unsigned int i = 0; i < rotatedPiece.size(); i++) {
        int newCoordX = (rotatedPiece[i][0] + offsetX) / 20;
        int newCoordY = (rotatedPiece[i][1] + offsetY) / 20;

        if(newCoordY < 0 || newCoordY > 24 || newCoordX < 0 or newCoordX > 12)
            rotatable = false;
        if(rotatable){
            if(tetrisRows[newCoordY][newCoordX] != 0){
                rotatable = false;
            }
        }
        if (rotatable){
            rotatedPiece[i][0] += offsetX;
            rotatedPiece[i][1] += offsetY;
        }
    }
    if (rotatable) {
        currentPieceParts.clear();
        scene->clear();
        updateBoard();
        addNextPieceToBoard(rotatedPiece);
    }
}

void MainWindow::holdPiece(){
    if(alreadyStoredPiece == false && gameRunning){
        currentPieceParts.clear();
        nextPieceParts.clear();
        heldPieceParts.clear();
        sceneNextPiece->clear();
        sceneHeldPiece->clear();
        scene->clear();
        updateBoard();

        if(heldPiece.empty()){
            heldPiece = nextPiece;
            nextPiece = upcomingPiece;
            choosePiece(upcomingPiece);
            alreadyStoredPiece = true;
        }
        else{
            tempPiece = nextPiece;
            nextPiece = heldPiece;
            heldPiece = tempPiece;
            alreadyStoredPiece = true;
        }
        showNextPiece(upcomingPiece);
        showHeldPiece(heldPiece);
        addNextPieceToBoard(nextPiece);
    }
}

void MainWindow::maintainHiscores(){
    std::string line;
    std::vector<int> hiscoreVector;
    std::vector<std::string> hiscoreTextVector;
    bool hiscoreUpdated = false;

    if(std::ifstream("hiscores.txt")){
        std::ifstream input("hiscores.txt");
        while(std::getline(input,line)){
            if(line.size() > 0){
                hiscoreVector.push_back(stoi(line));
            }
        }
    } else {
            hiscoreVector.push_back(pointsReceived);
            for(int i = 0; i < 5; i++){
                hiscoreVector.push_back(0);
            }
        }
    if(pointsReceived > hiscoreVector[4]){
        hiscoreVector[4] = pointsReceived;
        hiscoreUpdated = true;
    }

    if(hiscoreUpdated){
        std::sort(hiscoreVector.begin(),hiscoreVector.end(),std::greater<int>());
    }
        QString temp;
        hiscoreTextVector.push_back(std::to_string(hiscoreVector[0]));
        temp = QString::fromStdString(hiscoreTextVector[0]);
        this->ui->hiscoreLabel_1->setText(temp);
        hiscoreTextVector.push_back(std::to_string(hiscoreVector[1]));
        temp = QString::fromStdString(hiscoreTextVector[1]);
        this->ui->hiscoreLabel_2->setText(temp);
        hiscoreTextVector.push_back(std::to_string(hiscoreVector[2]));
        temp = QString::fromStdString(hiscoreTextVector[2]);
        this->ui->hiscoreLabel_3->setText(temp);
        hiscoreTextVector.push_back(std::to_string(hiscoreVector[3]));
        temp = QString::fromStdString(hiscoreTextVector[3]);
        this->ui->hiscoreLabel_4->setText(temp);
       hiscoreTextVector.push_back(std::to_string(hiscoreVector[4]));
        temp = QString::fromStdString(hiscoreTextVector[4]);
        this->ui->hiscoreLabel_5->setText(temp);

    std::ofstream output("hiscores.txt");
    std::ostream_iterator<std::string> output_iterator(output, "\n");
    std::copy(hiscoreTextVector.begin(), hiscoreTextVector.end(), output_iterator);
    output.close();
}

void MainWindow::on_newGameButton_pressed()
{
        resetGame();
}

void MainWindow::on_quitButton_clicked()
{
    MainWindow::close();
}
