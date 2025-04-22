#ifndef WIDGET_H
#define WIDGET_H

// Widget.h (ujistěte se, že obsahuje toto)

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput> // <-- Přidejte toto
#include <QUrl>
#include <QDateTime>
#include <QDebug>
#include <QKeyEvent> // Pokud používáte keyPressEvent
#include <QTimerEvent> // Pokud používáte timerEvent
#include <QTime> // Pro QTime v timerEvent
#include <QCoreApplication> // Pro qApp->exit()
#include <QMessageBox> // Pokud používáte MessageBox (zakomentováno, ale může být potřeba)
// Zahrňte UI hlavičku
#include "ui_widget.h"
#include "dialog.h" // Pokud používáte vlastní Dialog

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void timerEvent(QTimerEvent *event) override; // Přesunuto pod protected
    void keyPressEvent(QKeyEvent *event) override; // Přesunuto pod protected

private slots:
    void resetsettings();
    void changeRate(); // Pokud tuto funkci používáte
    void handleBackgroundLoop(QMediaPlayer::PlaybackState state); // <-- Nový slot
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_2_valueChanged(int arg1);
    void on_spinBox_3_valueChanged(int arg1);
    void on_btbgplay_clicked(); // Předpokládá btbgplay pro stop
    void on_pushButton_4_clicked(); // Předpokládá pushButton_4 pro play
    void on_btopen_clicked();
    void openFileNameReady(QString fileName); // Pouze pokud používáte AndroidFileDialog nebo podobné asynchronní metody
    void on_btmute_clicked();
    void on_dial_valueChanged(int value); // Předpokládá Dial pro hlasitost music
    void on_dial_2_valueChanged(int value); // Předpokládá Dial pro hlasitost music2
    void on_bt_seek_right_clicked(); // Předpokládá tlačítko pro seek

private:
    Ui::Widget *ui;
    bool paused;
    QMediaPlayer *music; // Pro kliknutí metronomu
    QAudioOutput *musicAudioOutput; // <-- Audio výstup pro metronom (nové)
    QMediaPlayer *music2; // Pro zvuk smyčky pozadí
    QAudioOutput *music2AudioOutput; // <-- Audio výstup pro smyčku (nové)
    // QMediaPlaylist *bgplaylist; // <-- Tuto řádku odstraňte!

    int ticks;
    QDateTime mStartTime;
    int timerspeed;
    int timerId;
    int bpc; // Beats per cycle?
    int bpcplayed;
    int bpccomplete; // Cycles complete?
    bool ticksmute = false; // <-- Podle použití v timerEvent, předpokládám člen třídy
    int fullticks = 0; // <-- Podle použití v timerEvent, předpokládám člen třídy
void createToolBar();
    // qreal bgrate; // Pokud používáte pro music2->setPlaybackRate

    QTime n; // <-- Podle použití v timerEvent, předpokládám člen třídy
};

#endif // WIDGET_H
