#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    QString getTime(qint64 position);
    ~Widget();
private slots:



   void on_pushButton_7_clicked();
   void on_palybutton_clicked();

   void on_tableWidget_cellDoubleClicked(int row, int column);

   void on_next_clicked();

   void on_up_clicked();


   void setPosition();

   void setCi();

   void on_horizontalSlider_valueChanged(int value);

   void on_horizontalSlider_2_valueChanged(int value);



   void getCi(int z);



private:
    Ui::Widget *ui;

    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicList;

       QTimer *timer;
       int flag = 0;
       int songNum;
       QStringList list;
};

#endif // WIDGET_H
