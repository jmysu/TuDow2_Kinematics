#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTimer>
#include "gl_viewer.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Viewer *viewer;
    void updateGroupboxKtitle();

    float x,y,z;
    QTimer *timer, *timerDemo;

private:
    Ui::MainWindow *ui;


private slots:
    //void on_horizontalSlider1_valueChanged(int value);
    //void on_horizontalSlider2_valueChanged(int value);
    //void on_horizontalSlider3_valueChanged(int value);
    void on_horizontalSlider1_sliderMoved(int position);
    void on_horizontalSlider2_sliderMoved(int position);
    void on_horizontalSlider3_sliderMoved(int position);

    void on_radioButtonFK_toggled(bool checked);
    void on_radioButtonIK_toggled(bool checked);


    void on_checkBoxDemo_clicked();
};

#endif // MAINWINDOW_H
