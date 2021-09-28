 #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "define.h"
#include "drivers.h"
#include "firebase.h"
#include <QMainWindow>
#include <QPushButton>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>


#ifdef SIMULATION_PC
    #include <QTextEdit>
#endif

namespace Ui {
  class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);
  public slots:
    //Q_SCRIPTABLE QString open();
  private slots:
    void handleDigitButton();
    void handleClearButton();
    void handleEnterButton();
    void checkInput();
    void openLock();
    void negativeMessage();
  private:
    //void checkPassword();
    Firebase m_firebase;
    Drivers m_drivers;
    QLineEdit * m_display;
    QPushButton * m_digitButton[10];
    QPushButton * m_clearButton;
    QPushButton * m_enterButton;
    QLabel * m_statusLabel1;
    QLabel * m_statusLabel2;
    QString m_status;
    QString m_typedNumber="";
    QString m_password="";
    QTimer * m_timer;

    //debug only
    QTimer * m_timer2;

#ifdef SIMULATION_PC
    QTextEdit * m_debugTextEdit;
#endif

};
#endif // MAINWINDOW_H
