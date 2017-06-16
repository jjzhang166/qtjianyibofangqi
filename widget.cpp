#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QDebug>
#include<QMediaMetaData>
#include<QMessageBox>
#include<QTextEdit>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    musicPlayer=new QMediaPlayer(this);
    musicList=new QMediaPlaylist;
    ui->horizontalSlider_2->setValue(99);       //初始化声音值为当前最大值
    musicPlayer->setPlaylist(musicList);        //播放器列表加入到播放器中
    ui->tableWidget->setStyleSheet("selection-background-color:#bfefff");//设置选中项的背景色
    timer=new QTimer(this);
   connect(timer,SIGNAL(timeout()),this,SLOT(setPosition()));
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getCi(int)));
    connect(timer,SIGNAL(timeout()),this,SLOT(setCi()));

}





Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_7_clicked()
{
    ui->tableWidget->setColumnCount(2); //设置列数
    QStringList list;                   //更改列的名称
    list<<"歌曲名"<<"格式";
    ui->tableWidget->setHorizontalHeaderLabels(list);

    QStringList filelist= QFileDialog::getOpenFileNames(this,
    "file open","D:\mp3",
    "Mp3 File(*.mp3;*.mp4;*.flac;*.wav);;All Files(*)");
    for(int i=0;i<filelist.size();i++){
        QString path=filelist.at(i);         //文件路径
        qDebug()<<path;
        if(!path.isEmpty()){
            musicList->addMedia(QUrl::fromLocalFile(path)); //加入到播放器列表
            QString fileName=path.split("/").last();        //文件名
           int rowNum=ui->tableWidget->rowCount();         //插入行号
            qDebug()<<rowNum;
ui->tableWidget->insertRow(rowNum);             //加入到歌曲列表
ui->tableWidget->setItem(rowNum,0,new QTableWidgetItem(fileName.split(".").front()));
ui->tableWidget->setItem(rowNum,1,new QTableWidgetItem(fileName.split(".").last()));
qDebug()<<fileName.split(".").front()<<fileName.split(".").last()<<path;
        }
    }

}

void Widget::on_palybutton_clicked()
{
    if(flag==0){        //第一次单击播放键
        timer->start(1000);
        musicList->setCurrentIndex(ui->tableWidget->currentRow());
        songNum=musicList->currentIndex();
        ui->palybutton->setIcon(QIcon("D://mp3_src/pause.png"));
        musicPlayer->play();
        flag=1;
    }else {
        if(songNum==ui->tableWidget->currentRow()){     //还是同一首歌时
            if(flag%2==1){
                ui->palybutton->setIcon(QIcon("D:/mp3_src/play.png"));
                musicPlayer->pause();
                flag+=1;
            }else {
                ui->palybutton->setIcon(QIcon("D:/mp3_src/pause.png"));
                musicPlayer->play();
                flag+=1;
            }
        }else {                 //更换歌曲时
            timer->start(1000);
            musicPlayer->stop();
            musicList->setCurrentIndex(ui->tableWidget->currentRow());
            songNum=musicList->currentIndex();
            ui->palybutton->setIcon(QIcon("D:/mp3_src/pause.png"));
            musicPlayer->play();
            flag=1;
        }
    }
}

void Widget::on_tableWidget_cellDoubleClicked(int row, int column)
{


musicList->setCurrentIndex(row);
musicPlayer->play();
songNum=musicList->currentIndex();
ui->palybutton->setIcon(QIcon("D:/mp3_src/pause.png"));
flag=1;
}

void Widget::on_next_clicked()
{
    int currentIndex=musicList->currentIndex();
        if(++currentIndex==musicList->mediaCount()){
            currentIndex=0;
        }
        musicList->setCurrentIndex(currentIndex);
}


void Widget::on_up_clicked()
{

musicList->previous();
musicPlayer->play();
}
QString Widget::getTime(qint64 position)  {         //通过歌曲滑动位置，返回当前位置时间
    int i=musicPlayer->duration();
    int m=position/60000;
    int s=(position-m*60000)/1000;
    qDebug()<<"position"<<position<<"m:"<<m<<"s"<<s;
    QString sm,ss;
    if(m<10){
        sm="0"+QString::number(m);
    }else {
        sm=""+QString::number(m);
    }if(s<10){
        ss=":0"+QString::number(s);
    }else {
        ss=":"+QString::number(s);
    }
   qDebug()<<sm+ss;
    return sm+ss;
}
void Widget::setPosition()      //更改音乐滑动条位置
{
    qint64 position=musicPlayer->position();
    ui->horizontalSlider_2->setMaximum(musicPlayer->duration());
    ui->horizontalSlider_2->setSliderPosition(position);

    ui->time_label_1->setText(getTime(position));
    ui->time_label_2->setText(getTime(musicPlayer->duration()));
    //qDebug()<<position<<musicPlayer->duration();

}




void Widget::on_horizontalSlider_valueChanged(int value)
{
     musicPlayer->setVolume(value);

}

void Widget::on_horizontalSlider_2_valueChanged(int value)
{
    qint64 currentPosition=musicPlayer->position();
       //当滑块的值(是绝对值)改变一次性超多2000时,我们认为有人拖动滑块,要快进!(定时器的刷新滑块1s在1013)
           if(qAbs(value-currentPosition)>2000&&(ui->horizontalSlider->value()!=0))
           {
               musicPlayer->pause();
               timer->stop();
               musicPlayer->setPosition(value);
               timer->start();
               musicPlayer->play();
           }

}


/*void Widget::getCi(int z){                  //歌词显示
    if(ui->checkBox->isChecked()==true)
       {
           QString str=ui->tableWidget->item(z,0)->text();
           qDebug()<<str;
           QFile file("D:/MP3/"+str+".lrc");
           if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
           {
                ui->lab->setText("当前目录下未找到歌词文件");
           }
           else
           {
               QTextStream in(&file);
             for(int i=0;i<6;i++){
                   QString result1=in.readLine();
                   qDebug()<<"1:"+result1.split("]").front().split("[").last();
                   qDebug()<<"2:"+result1.split("]").last();
              }
               QString result=in.readAll();
               list=result.split("\n");
               qDebug()<<list.at(5);
               ui->lab->setText(result);

           }
       }
}*/
void Widget::getCi(int z)
{
    if(ui->checkBox->isChecked()==true)
    {
        QString str=ui->tableWidget->item(z,0)->text();
        qDebug()<<str;
        QFile file("D:/mp3/"+str+".lrc");
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
           ui->lab->setText("当前目录下未找到歌词文件");
        }
        else
        {
            QTextStream in(&file);
            QString result=in.readAll();
            QTextEdit *edit=new QTextEdit();

            ui->lab->setText(result);
            edit->show();
        }
    }
}
void Widget::setCi(){
    QString currentTime=getTime(musicPlayer->position());
    QString songTime;
    qDebug()<<"当前时间:"+currentTime;
    for(int i=0;i<list.length();i++){
        songTime=list.at(i).split("]").front().split("[").last().split(".").front();
        if(currentTime.compare(songTime)==0){
            ui->lab->setText(list.at(i).split("]").last());
            qDebug()<<"歌词时间:"+list.at(i).split("]").front().split("[").last().split(".").front();
            qDebug()<<list.at(i).split("]").last();
        }
    }
}

