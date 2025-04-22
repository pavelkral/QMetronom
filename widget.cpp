#include <QDebug>
#include "widget.h"
#include "ui_widget.h"
#include "dialog.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QDateTime>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Smetronom");
    paused = true;

    music = new QMediaPlayer(this);
    musicAudioOutput = new QAudioOutput(this);
    music->setAudioOutput(musicAudioOutput);
    music->setSource(QUrl("qrc:/samples/clap.wav"));

    ticks = 0;
    mStartTime = QDateTime::currentDateTime();
    timerspeed = 500;
    timerId = 0;

    qDebug() << "App running:";

    ui->maincounter->setStyleSheet("QLabel { background-color : green;margin-right:5px; color : black; border-radius:10px ;}");
    ui->lcdNumber_2->setPalette(Qt::black);

    music2 = new QMediaPlayer(this);
    music2AudioOutput = new QAudioOutput(this);
    music2->setAudioOutput(music2AudioOutput);
    music2->setSource(QUrl("qrc:/samples/loop.wav"));

    connect(music2, &QMediaPlayer::playbackStateChanged,
            this, &Widget::handleBackgroundLoop);

    bpc = 3;
    bpcplayed = 0;
    bpccomplete = 0;

    ticks = 0;
    bpcplayed = 0;
    bpccomplete = 0;


    ui->spinBox_2->setValue(120);
    ui->spinBox->setValue(bpccomplete);
    ui->spinBox_3->setValue(bpc);

    ticksmute = false;
    fullticks = 0;

    connect(ui->pushButton_3, &QPushButton::clicked, this, &Widget::resetsettings);

    connect(ui->spinBox_2, &QSpinBox::valueChanged, this, &Widget::on_spinBox_2_valueChanged);
    connect(ui->spinBox_3, &QSpinBox::valueChanged, this, &Widget::on_spinBox_3_valueChanged);
    connect(ui->dial, &QDial::valueChanged, this, &Widget::on_dial_valueChanged);
    connect(ui->dial_2, &QDial::valueChanged, this, &Widget::on_dial_2_valueChanged);

    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::on_pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Widget::on_pushButton_2_clicked);
    connect(ui->btbgplay, &QPushButton::clicked, this, &Widget::on_btbgplay_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Widget::on_pushButton_4_clicked);
    connect(ui->btopen, &QPushButton::clicked, this, &Widget::on_btopen_clicked);
    connect(ui->btmute, &QPushButton::clicked, this, &Widget::on_btmute_clicked);
    connect(ui->bt_seek_right, &QPushButton::clicked, this, &Widget::on_bt_seek_right_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::createToolBar()
{

}

void Widget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId)
    {
        n = QTime::currentTime();

        if(!paused){

            if(bpcplayed == bpc){
                ui->maincounter->setStyleSheet("QLabel { background-color : red;margin-right:5px; color : white;border-radius:10px; }");
                ui->maincounter->setText("1");

                music->setSource(QUrl("qrc:/samples/kick.wav"));

                bpcplayed = 0;
                ticks = 0;
                bpccomplete++;
                ui->spinBox->setValue(bpccomplete);
            }
            else{
                ui->maincounter->setStyleSheet("QLabel { background-color : green; margin-right:5px;color : black;border-radius:10px ; }");
                ui->maincounter->setText(QString("%1").arg(bpcplayed + 1));

                music->setSource(QUrl("qrc:/samples/clap.wav"));
            }

            if(ticksmute == false)
            {
                music->setPosition(0);
                music->play();
            }

            qint64 ms = mStartTime.msecsTo(QDateTime::currentDateTime());
            int h = ms / 1000 / 60 / 60;
            int m = (ms / 1000 / 60) % 60;
            int s = (ms / 1000) % 60;
            ms = ms % 1000;

            QString diff = QString("%1:%2:%3").arg(h, 2, 10, QChar('0')).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
            ui->lcdNumber_2->display(diff);

            bpcplayed++;
            ticks++;
            fullticks++;

        }
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_P:
    {
        qDebug() << "press P" ;
        if(paused){
            timerId = startTimer(timerspeed);
            paused = false;

        }
        else{
            killTimer(timerId);
            timerId = 0;
            paused = true;
        }
    }
    break;
    case Qt::Key_Escape:
    {
        qApp->exit();
    }
    break;
    default:
        QWidget::keyPressEvent(event);
    }
}


void Widget::on_pushButton_clicked()
{
    if(paused){
        paused = false;
        mStartTime = QDateTime::currentDateTime();
        ticks = 0;
        bpcplayed = 0;
        bpccomplete = 0;

        ui->maincounter->setText("0");
        ui->spinBox->setValue(0);
        ui->lcdNumber_2->display("00:00:00");

        if (timerId != 0) {
            killTimer(timerId);
        }
        timerId = startTimer(timerspeed);

        if (music2->playbackState() != QMediaPlayer::PlayingState) {
            music2->play();
        }

    }
    else{
    }
}

void Widget::on_pushButton_2_clicked()
{
    if(!paused){
        if (timerId != 0) {
            killTimer(timerId);
            timerId = 0;
        }
        paused = true;

        music2->stop();

        ui->maincounter->setText("0");
        ui->spinBox->setValue(0);
        ui->lcdNumber_2->display("00:00:00");
        ui->maincounter->setStyleSheet("QLabel { background-color : green; margin-right:5px;color : black;border-radius:10px ; }");

        ticks = 0;
        bpcplayed = 0;
        bpccomplete = 0;
        fullticks = 0;
    }
    else{
    }
}

void Widget::on_spinBox_2_valueChanged(int arg1)
{
    bool wasRunning = !paused;
    if (wasRunning) {
        if (timerId != 0) {
            killTimer(timerId);
            timerId = 0;
        }
        paused = true;
    }

    double arg = static_cast<double>(arg1);
    if (arg > 0) {
        double beatsPerSecond = arg / 60.0;
        timerspeed = static_cast<int>(1000.0 / beatsPerSecond);
        qDebug() << "BPM:" << arg1 << "Timerspeed:" << timerspeed;
    } else {
        timerspeed = 1000;
        qDebug() << "BPM set to 0, using default timerspeed:" << timerspeed;
    }


    mStartTime = QDateTime::currentDateTime();
    ticks = 0;
    bpcplayed = 0;
    bpccomplete = 0;
    fullticks = 0;

    ui->maincounter->setText("0");
    ui->spinBox->setValue(0);
    ui->lcdNumber_2->display("00:00:00");
    ui->maincounter->setStyleSheet("QLabel { background-color : green; margin-right:5px;color : black;border-radius:10px ; }");


    if (wasRunning) {
        paused = false;
        timerId = startTimer(timerspeed);
    }
}

void Widget::resetsettings()
{
    if (!paused) {
        if (timerId != 0) {
            killTimer(timerId);
            timerId = 0;
        }
        paused = true;
    }

    mStartTime = QDateTime::currentDateTime();
    ticks = 0;
    bpcplayed = 0;
    bpccomplete = 0;
    bpc = 3;
    fullticks = 0;
    timerspeed = 500;

    ui->maincounter->setText("0");
    ui->spinBox->setValue(0);
    ui->lcdNumber_2->display("00:00:00");
    ui->spinBox_2->setValue(120);
    ui->spinBox_3->setValue(bpc);

    qDebug() << "reset settings" ;

}

void Widget::on_spinBox_3_valueChanged(int arg1)
{
    bool wasRunning = !paused;
    if (wasRunning) {
        if (timerId != 0) {
            killTimer(timerId);
            timerId = 0;
        }
        paused = true;
    }

    bpc = arg1;
    if (bpc <= 0) bpc = 1;

    ticks = 0;
    bpcplayed = 0;
    bpccomplete = 0;
    fullticks = 0;

    ui->maincounter->setText("0");
    ui->spinBox->setValue(0);
    ui->lcdNumber_2->display("00:00:00");
    ui->maincounter->setStyleSheet("QLabel { background-color : green; margin-right:5px;color : black;border-radius:10px ; }");

    mStartTime = QDateTime::currentDateTime();

    qDebug() << "bpc changed to" << bpc ;

    if (wasRunning) {
        paused = false;
        timerId = startTimer(timerspeed);
    }
}

void Widget::changeRate()
{
    qreal rate = 1.2;
    qDebug() << "changeRate called, rate:" << rate;

}

void Widget::on_btbgplay_clicked()
{
    qDebug() << "Background music stop clicked";
    music2->stop();
}

void Widget::on_pushButton_4_clicked()
{
    qDebug() << "Background music play clicked";
    if (music2->playbackState() != QMediaPlayer::PlayingState) {
        music2->play();
    }

}

void Widget::on_btopen_clicked()
{
    qDebug() << "Open file clicked";
    Dialog filedialog;

    if ( filedialog.exec() == QDialog::Accepted )
    {
        QString path1 = filedialog.path;
        qDebug() << "Selected file:" << path1;

        QUrl fileUrl = QUrl::fromLocalFile(path1);

        if (fileUrl.isValid()) {
            music2->stop();
            music2->setSource(fileUrl);
        } else {
            qDebug() << "Invalid file URL:" << path1;
            QMessageBox::warning(this, "Chyba", "Neplatná cesta k souboru.");
        }
    }
    else{
        qDebug() << "File selection cancelled";
    }
}

void Widget::openFileNameReady(QString fileName)
{
    if (!fileName.isNull()) {
        qDebug() << "FileName (from async dialog): " << fileName;
        QUrl fileUrl = QUrl::fromLocalFile(fileName);
        if (fileUrl.isValid()) {
            music2->stop();
            music2->setSource(fileUrl);
        } else {
            qDebug() << "Invalid file URL from async dialog:" << fileName;
            QMessageBox::warning(this, "Chyba", "Neplatná cesta k souboru z asynchronního dialogu.");
        }

    } else {
        qDebug() << "User did not choose file (async dialog)";
    }
}

void Widget::on_btmute_clicked()
{
    ticksmute = !ticksmute;
    qDebug() << "Ticks mute:" << ticksmute;

}

void Widget::on_dial_valueChanged(int value)
{
    if (musicAudioOutput) {
        musicAudioOutput->setVolume(static_cast<qreal>(value) / 100.0);
        qDebug() << "Metronome volume set to:" << value;
    }
}

void Widget::on_dial_2_valueChanged(int value)
{
    if (music2AudioOutput) {
        music2AudioOutput->setVolume(static_cast<qreal>(value) / 100.0);
        qDebug() << "Background volume set to:" << value;
    }
}

void Widget::on_bt_seek_right_clicked()
{
    qDebug() << "Seek right clicked";
    if (music2->isSeekable()) {
        quint64 currentPos = music2->position();
        music2->setPosition(currentPos + 1000);
        qDebug() << "Seeked to:" << music2->position();
    } else {
        qDebug() << "Background player is not seekable.";
    }
}

void Widget::handleBackgroundLoop(QMediaPlayer::PlaybackState state)
{
    qDebug() << "Background player state changed:" << state;
    if (state == QMediaPlayer::StoppedState) {
        if (music2->mediaStatus() == QMediaPlayer::EndOfMedia) {
            qDebug() << "Background music reached end, restarting loop";
            music2->play();
        } else {
            qDebug() << "Background music stopped for other reason, not looping.";
        }
    }
}
