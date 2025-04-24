#ifndef WIDGET_H
#define WIDGET_H

// Widget.h (ujistěte se, že obsahuje toto)

#include <QWidget>
#include <QMediaPlayer>


#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include "ui_widget.h"
#include "dialog.h"

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
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void resetsettings();
    void changeRate();
    void handleBackgroundLoop(QMediaPlayer::PlaybackState state);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_2_valueChanged(int arg1);
    void on_spinBox_3_valueChanged(int arg1);
    void on_btbgplay_clicked();
    void on_pushButton_4_clicked();
    void on_btopen_clicked();
    void openFileNameReady(QString fileName);
    void on_btmute_clicked();
    void on_dial_valueChanged(int value);
    void on_dial_2_valueChanged(int value);
    void on_bt_seek_right_clicked();

private:
    Ui::Widget *ui;
    bool paused;
    QMediaPlayer *music;
    QAudioOutput *musicAudioOutput;
    QMediaPlayer *music2;
    QAudioOutput *music2AudioOutput;

    int ticks;
    QDateTime mStartTime;
    int timerspeed;
    int timerId;
    int bpc;
    int bpcplayed;
    int bpccomplete;
    bool ticksmute = false;
    int fullticks = 0;
    void createToolBar();

    QTime n;
};

#endif // WIDGET_H
