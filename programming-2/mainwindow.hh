#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <random>
#include <QTimer>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_newGameButton_pressed();
    void on_quitButton_clicked();

private:
    Ui::MainWindow* ui;

    /* Graafiset ikkunat, joissa palat näytetään. */
    QGraphicsScene* scene;
    QGraphicsScene* sceneNextPiece;
    QGraphicsScene* sceneHeldPiece;

    /* Yksittäisten tetrispalojen ruutujen näyttämiseen käytettävät elementit. */
    QGraphicsRectItem* square;
    QGraphicsRectItem* nextSquare;
    QGraphicsRectItem* heldSquare;

    /* Ajastimet palojen siirämiseen, kuluneen ajan laskemiseen
     * ja animaatioiden toteuttamiseen. */
    QTimer timerMovePieces;
    QTimer timerTimePlayed;
    QTimer timerAnimations;

    /* Satunnaislukugeneraattori seuraavan palan valitsemista varten. */
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    /* Peli-ikkunan koko. */
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240;

    /* Peli-ikkunan sijainti pääikkunassa. */
    const int LEFT_MARGIN = 140;
    const int TOP_MARGIN = 30;

    /* Yksittäisen tetrispalan komponentin koko. */
    const int SQUARE_SIDE = 20;

    /* Ruutujen määrä pelilaudalla pystysuunnassa. */
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;

    /* Ruutujen määrä pelilaudalla vaakasuunnassa. */
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    const std::vector<int> TETRIS_ROW_SOLID = {8,8,8,8,8,8,8,8,8,8,8,8,8,8};
    const std::vector<int> TETRIS_ROW_EMPTY = {8,0,0,0,0,0,0,0,0,0,0,0,0,8};

    /* Tetrispalojen komponenttien koordinaatit. Palan ensimmäinen vektori
     * sisältää myös tiedon siitä, mihin palaan täytyy vaihtaa palaa kiertäessä. */
    const std::vector<std::vector<int>> HORIZONTAL_PIECE = {{100,0,1,1},{120,0,1},       // 0
                                                      {140,0,1},{160,0,1}};

    const std::vector<std::vector<int>> VERTICAL_PIECE = {{140,0,1,0},{140,20,1},        // 1
                                                      {140,40,1},{140,60,1}};

    const std::vector<std::vector<int>> LEFT_CORNER_PIECE = {{100,0,2,3},{120,0,2},      // 2
                                                      {140,0,2},{100,20,2}};

    const std::vector<std::vector<int>> LEFT_CORNER_PIECE_2 = {{120,0,2,4},{120,-20,2},   // 3
                                                      {120,20,2},{100,-20,2}};

    const std::vector<std::vector<int>> LEFT_CORNER_PIECE_3 = {{140,0,2,5},{100,20,2},   // 4
                                                      {140,20,2},{120,20,2}};

    const std::vector<std::vector<int>> LEFT_CORNER_PIECE_4 = {{120,0,2,2},{120,20,2},   // 5
                                                      {120,-20,2},{140,20,2}};

    const std::vector<std::vector<int>> RIGHT_CORNER_PIECE = {{140,0,3,7},{100,0,3},   // 6
                                                      {120,0,3},{140,20,3}};

    const std::vector<std::vector<int>> RIGHT_CORNER_PIECE_2 = {{120,0,3,8},{120,-20,3},   // 7
                                                      {120,20,3},{100,20,3}};

    const std::vector<std::vector<int>> RIGHT_CORNER_PIECE_3 = {{100,0,3,9},{100,20,3},   // 8
                                                      {120,20,3},{140,20,3}};

    const std::vector<std::vector<int>> RIGHT_CORNER_PIECE_4 = {{120,0,3,6},{140,-20,3},   // 9
                                                      {120,-20,3},{120,20,3}};

    const std::vector<std::vector<int>> SQUARE_PIECE = {{120,0,4,10},{100,0,4},           // 10
                                                      {100,20,4},{120,20,4}};

    const std::vector<std::vector<int>> STEP_UP_RIGHT_PIECE = {{120,0,5,12},{100,20,5},   // 11
                                                      {120,20,5},{140,0,5}};

    const std::vector<std::vector<int>> STEP_UP_RIGHT_PIECE_2 = {{120,0,5,13},{120,-20,5}, // 12
                                                      {140,0,5},{140,20,5}};

    const std::vector<std::vector<int>> STEP_UP_RIGHT_PIECE_3 = {{120,0,5,12},{100,20,5},  // 13
                                                      {120,20,5},{140,0,5}};

    const std::vector<std::vector<int>> STEP_UP_RIGHT_PIECE_4 = {{120,0,5,13},{120,-20,5}, // 14
                                                      {140,0,5},{140,20,5}};

    const std::vector<std::vector<int>> PYRAMID_PIECE = {{120,0,6,16},{100,20,6},        // 15
                                                      {120,20,6},{140,20,6}};

    const std::vector<std::vector<int>> PYRAMID_PIECE_2 = {{120,0,6,17},{100,-20,6},       // 16
                                                      {100,0,6},{100,20,6}};

    const std::vector<std::vector<int>> PYRAMID_PIECE_3 = {{120,0,6,18},{100,-20,6},       // 17
                                                      {120,-20,6},{140,-20,6}};

    const std::vector<std::vector<int>> PYRAMID_PIECE_4 = {{120,0,6,15},{140,-20,6},       // 18
                                                      {140,0,6},{140,20,6}};

    const std::vector<std::vector<int>> STEP_UP_LEFT_PIECE = {{120,0,7,20},{100,0,7},    // 19
                                                      {120,20,7},{140,20,7}};

    const std::vector<std::vector<int>> STEP_UP_LEFT_PIECE_2 = {{120,0,7,21},{120,-20,7},  // 20
                                                      {100,0,7},{100,20,7}};

    const std::vector<std::vector<int>> STEP_UP_LEFT_PIECE_3 = {{120,0,7,20},{100,0,7},    // 21
                                                      {120,20,7},{140,20,7}};

    const std::vector<std::vector<int>> STEP_UP_LEFT_PIECE_4 = {{120,0,7,21},{120,-20,7},  // 22
                                                      {100,0,7},{100,20,7}};

    const std::vector< std::vector<std::vector<int>>> ALL_PIECES = {HORIZONTAL_PIECE, VERTICAL_PIECE,
    LEFT_CORNER_PIECE, LEFT_CORNER_PIECE_2, LEFT_CORNER_PIECE_3, LEFT_CORNER_PIECE_4,
    RIGHT_CORNER_PIECE, RIGHT_CORNER_PIECE_2, RIGHT_CORNER_PIECE_3, RIGHT_CORNER_PIECE_4,
    SQUARE_PIECE,
    STEP_UP_RIGHT_PIECE, STEP_UP_RIGHT_PIECE_2, STEP_UP_RIGHT_PIECE_3, STEP_UP_RIGHT_PIECE_4,
    PYRAMID_PIECE, PYRAMID_PIECE_2, PYRAMID_PIECE_3, PYRAMID_PIECE_4,
    STEP_UP_LEFT_PIECE, STEP_UP_LEFT_PIECE_2, STEP_UP_LEFT_PIECE_3, STEP_UP_LEFT_PIECE_4};

    /* Funktio resetoi pelin alkutilaan nollaamalla tarvittavat vektorit
     * ja numeroarvot. Kun peli käynnistetään, sama funktio alustaa tarvittavat
     * vektorit ja muuttujat. */
    void resetGame();

    /* Funktio alustaa laudan pelin alussa, ja kun lauta resetoidaan.
     * Laudalta näkymättömissä on pohjalla ja molemmilla sivuilla reunat,
     * joiden avulla estetään palan siirtyminen ulos sivusta tai pohjasta.
     * Tähän käytetään vakiovektoreita TETRIS_ROWS_EMPTY ja TETRIS_ROWS_SOLID. */
    void setupBoard();

    /* Funktio piirtää matriisin tetrisRows mukaisesti palat laudalle. Palan väri saadaan
     * vektoriin tallennetusta numeroarvosta. */
    void updateBoard();

    /* Funktio vastaa täysien rivien poistamisesta matriisin tetrisRows perusteella.
     * Funktio myös antaa pisteitä poistettujen rivien perusteella. */
    void deleteFullRows();

    /* Funktio päivittää pelilaudalle kuluneen ajan. */
    void updateSecondsPlayed();

    /* Funktio päivittää pelaajan pisteet, ja nostaa niiden perusteella
     * pelin vaikeusastetta. */
    void updatePointsAndLevel();

    /* Funktio näyttää animaation kun pelaaja saa pisteitä täysistä riveistä.
     * Saadut pisteet näytetään tekstinä, joka kohoaa laudalla ylöspäin. */
    void pointsAddedAnimation();

    /* Funktio vastaa tulostaulun päivittämisestä, ja tulosten tallentamisesta
     * tiedostoon "hiscores.txt". Jos tiedostoa ei ole, se luodaan. Ohjelma toimii
     * vain jos tiedostoa ei ole, tai se on oikeaa muotoa, eli sisältää 5 lukuarvoa
     * omilla riveillään.*/
    void maintainHiscores();

    /* Funktio "kääntää" palan vaihtamalla sen palaan, joka
     * on muodoltaan alkuperäinen pala käännettynä myötäpäivään. */
    void rotatePiece();

    /* Funktio siirtää nykyisen putoavan palan säilöön, ja ottaa putoavaksi
     * palaksi seuraavan palan. Palaa ei voi heti ottaa pidosta takaisin, vaan
     * vasta kun seuraava pala on pudonnut.*/
    void holdPiece();

    /* Funktio näyttää säilössä olevan palan tälle varatussa kentässä. */
    void showHeldPiece(const std::vector<std::vector<int>>);

    /* Funktio näyttää seuraavana laudalle lisättävän palan tälle varatussa kentässä. */
    void showNextPiece(const std::vector<std::vector<int>>);

    /* Funktio valitsee seuraavaksi pudotettavan palan valmiiksi määriteltyjen
     * palojen joukosta. */
    void choosePiece(std::vector<std::vector<int>>&);

    /* Funktio vastaa uusimman palan lisäämisestä pelilaudalle. Palojen neliöiden
     * sijainnit ja värit funktio saa mainwindow.hh tiedostossa määritellyistä vektoreista.
     * Funktio myös lopettaa pelin, jos uutta palaa ei voida asettaa laudalle. */
    void addNextPieceToBoard(std::vector<std::vector<int>>);

    /* Funktio tarkistaa, onko yksittäisen neliön siirtäminen mahdollista. Funktio
     * pieceMovable käyttää tätä funktiota. */
    bool squareMovable(QGraphicsRectItem*, std::string);

    /* Funktio tarkistaa, onko palaa mahdollista liikuttaa halutulla tavalla. Funktiota kutsuttaessa
     * haluttu suunta annetaan. */
    bool pieceMovable(std::vector<QGraphicsRectItem*>, std::string);

    /* Funktio vastaa yksittäisen palan liikkeestä. Liikkuminen oikealle ja vasemmalle
     * on mahdollista näppäinkomennoilla. Funktio tarkistaa pieceMoveable-funktiota käyttäen,
     * onko palan siirtäminen mahdollista. */
    void movePiece(std::vector<QGraphicsRectItem*>, std::string);

    /* Funktio movePieces vastaa palojen automaattisesta siirtymisestä alaspäin
     * käyttäen ajastinta timerMovePieces. Ajastimen intervallia muutetaan muuttujan pieceSpeed
     * avulla, kun pelin vaikeusaste kasvaa, mikä saa palat putoamaan nopeammin. */
    void movePieces();

    std::vector<std::vector<int>> nextPiece;
    std::vector<std::vector<int>> upcomingPiece;
    std::vector<std::vector<int>> heldPiece;
    std::vector<std::vector<int>> tempPiece;
    std::vector<std::vector<int>> tetrisRows;
    std::vector<QGraphicsRectItem*> currentPieceParts;
    std::vector<QGraphicsRectItem*> nextPieceParts;
    std::vector<QGraphicsRectItem*> heldPieceParts;

    int CoordX;
    int CoordY;
    int colorNumber;
    int secondForm;
    int secondsPlayed;
    int pointsReceived;
    int pointsOld;
    int pieceSpeed;
    int level;
    int animationLoops;
    bool gameRunning;
    bool alreadyStoredPiece;
};
#endif // MAINWINDOW_HH
