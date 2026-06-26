#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load data

    courseManager.loadCourses("data/courses.txt");
    routineManager.loadRoutines("data/routines.txt");
    faqManager.loadFaqs("data/faq.txt");
    admissionManager.loadData("data/admissions.txt");
    admissionManager.loadData("data/entrance.txt");
    admissionManager.loadData("data/fees.txt");
    // Create chatbot

    chatbot = new ChatBot(
        &courseManager,
        &routineManager,
        &faqManager,
        &admissionManager
        );

    // Send button

    connect(ui->btnSend,
            &QPushButton::clicked,
            this,
            &MainWindow::sendMessage);

    // Press Enter to send

    connect(ui->txtMessage,
            &QLineEdit::returnPressed,
            this,
            &MainWindow::sendMessage);

    // Welcome message

    addMessage(
        "Hello! I am KU-InfoBot. How can I help you today?",
        false
        );
}

MainWindow::~MainWindow()
{
    delete chatbot;
    delete ui;
}

void MainWindow::sendMessage()
{
    QString userMessage =
        ui->txtMessage->text().trimmed();

    if(userMessage.isEmpty())
    {
        return;
    }

    // Display user message

    addMessage(userMessage, true);

    // Generate response

    QString botResponse =
        chatbot->getResponse(userMessage);

    // Display bot response

    addMessage(botResponse, false);

    // Clear input field

    ui->txtMessage->clear();
}

void MainWindow::addMessage(const QString& text, bool isUser)
{
    // Chat bubble
    QLabel *messageLabel = new QLabel(text);
    messageLabel->setWordWrap(true);
    messageLabel->setMaximumWidth(700);

    if(isUser)
    {
        messageLabel->setStyleSheet(
            "QLabel{"
            "background:#0057D8;"
            "color:white;"
            "border-radius:15px;"
            "padding:12px;"
            "font-size:14px;"
            "}"
            );
    }
    else
    {
        messageLabel->setStyleSheet(
            "QLabel{"
            "background:#F2F2F2;"
            "color:black;"
            "border-radius:15px;"
            "padding:12px;"
            "font-size:14px;"
            "}"
            );
    }

    // Avatar
    QLabel *avatar = new QLabel;
    avatar->setFixedSize(45,45);

    if(isUser)
    {
        avatar->setPixmap(
            QPixmap("resources/icons/user.png")
                .scaled(45,45,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation)
            );
    }
    else
    {
        avatar->setPixmap(
            QPixmap("resources/icons/bot.png")
                .scaled(45,45,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation)
            );
    }

    QWidget *container = new QWidget;

    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(15,8,15,8);
    layout->setSpacing(10);

    if(isUser)
    {
        layout->addStretch();
        layout->addWidget(messageLabel);
        layout->addWidget(avatar);
    }
    else
    {
        layout->addWidget(avatar);
        layout->addWidget(messageLabel);
        layout->addStretch();
    }
    layout->setAlignment(avatar, Qt::AlignTop);
    layout->setAlignment(messageLabel, Qt::AlignTop);
    ui->verticalLayout_Chat->insertWidget(
        ui->verticalLayout_Chat->count()-1,
        container);

    QScrollBar *scrollBar =
        ui->scrollAreaChat->verticalScrollBar();

    scrollBar->setValue(scrollBar->maximum());
}