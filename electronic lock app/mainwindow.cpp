#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
     Qt::WindowFlags flags;
     flags =  Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint;
     this ->setWindowFlags(flags);

     QFont font;
     m_display = new QLineEdit("");
     m_display->setReadOnly(true);
     m_display->setAlignment(Qt::AlignRight);
     font = m_display->font();
     font.setPointSize(45);
     m_display->setFont(font);
     m_display->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
     m_display->setMinimumSize(200,50);


     for (int i=0;i<10;i++){
        m_digitButton[i] = new QPushButton(QString::number(i), this);
        font = m_digitButton[i]->font();
        font.setPointSize(45);
        m_digitButton[i]->setFont(font);
        m_digitButton[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_digitButton[i]->setMinimumSize(200,50);
        connect(m_digitButton[i], &QPushButton::released, this, &MainWindow::handleDigitButton);
    }

    m_clearButton= new QPushButton( "Clear", this );
    font = m_clearButton->font();
    font.setPointSize(45);
    m_clearButton->setFont(font);
    m_clearButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_clearButton->setMinimumSize(200,50);
    connect( m_clearButton, &QPushButton::released, this, &MainWindow::handleClearButton );

    m_enterButton= new QPushButton( "Enter", this );
    font = m_enterButton->font();
    font.setPointSize(45);
    m_enterButton->setFont(font);
    m_enterButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_enterButton->setMinimumSize(200,50);
    connect( m_enterButton, &QPushButton::released, this, &MainWindow::handleEnterButton);

    m_statusLabel1 = new QLabel("Status",this);
    font = m_statusLabel1->font();
    font.setPointSize(45);
    m_statusLabel1->setFont(font);

    m_statusLabel2 = new QLabel(m_drivers.getDoorStatus(),this);
    font = m_statusLabel2->font();
    font.setPointSize(45);
    m_statusLabel2 ->setFont(font);

    m_timer = new QTimer(this);
    connect( m_timer, &QTimer::timeout, this, &MainWindow::checkInput );
    m_timer->start( INPUT_READ_PERIOD_MILI_SEC );

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(m_display, 0, 0,1,3);
    layout->addWidget(m_digitButton[7], 1, 0);
    layout->addWidget(m_digitButton[8], 1, 1);
    layout->addWidget(m_digitButton[9], 1, 2);
    layout->addWidget(m_digitButton[4], 2, 0);
    layout->addWidget(m_digitButton[5], 2, 1);
    layout->addWidget(m_digitButton[6], 2, 2);
    layout->addWidget(m_digitButton[1], 3, 0);
    layout->addWidget(m_digitButton[2], 3, 1);
    layout->addWidget(m_digitButton[3], 3, 2);
    layout->addWidget(m_clearButton, 4, 0);
    layout->addWidget(m_digitButton[ 0], 4, 1);
    layout->addWidget(m_enterButton, 4, 2);
    layout->addWidget(m_statusLabel1, 5, 0);
    layout->addWidget(m_statusLabel2, 5, 1,1,2);

   setLayout(layout);

   connect( &m_firebase, &Firebase::openRequest, this, &MainWindow::openLock );
   setWindowTitle(tr("Electonic lock"));
}

void MainWindow::handleDigitButton()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if ( m_typedNumber.length() < MAX_TEXT_LENGTH )
    {
        m_typedNumber=m_typedNumber + clickedButton->text();
    }

    m_display->setText(m_typedNumber);
}

void MainWindow::handleClearButton()
{
    m_typedNumber = QString("");
    m_display->setText(m_typedNumber);
}

void MainWindow::handleEnterButton()
{
    if ( m_typedNumber != QString("") )
    {
        if ( m_typedNumber == QString( CLOSE_APP_PASS ) )
        {
            qApp->quit();
        }

        m_firebase.checkPassword( m_typedNumber );

        m_typedNumber = QString("");
        m_display->setText( m_typedNumber );
    }
}

void MainWindow::checkInput()
{
    QString temp_status = m_drivers.getDoorStatus();
    if ( temp_status != m_status )
    {
        m_status = temp_status;
        m_statusLabel2->setText(m_status);
        m_firebase.storeEvent(EVENT_DOOR_STATUS, m_status);
    }
}

void MainWindow::openLock()
{
   m_drivers.openLock();
   m_firebase.storeEvent(EVENT_OPEN_REQUEST, OPEN_CONFIRMATION );
}
