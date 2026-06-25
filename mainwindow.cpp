#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load data

    courseManager.loadCourses("data/courses.txt");
    routineManager.loadRoutines("data/routines.txt");
    faqManager.loadFaqs("data/faq.txt");
    // admissionManager.loadAdmissions("data/file.txt");
    // Create chatbot

    chatbot = new ChatBot(
        &courseManager,
        &routineManager,
        &faqManager
        // &admissionManager
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

void MainWindow::addMessage(
    const QString& text,
    bool isUser)
{
    QLabel* messageLabel =
        new QLabel(text);

    messageLabel->setWordWrap(true);

    messageLabel->setMaximumWidth(600);

    if(isUser)
    {
        messageLabel->setStyleSheet(
            "background:#0057D8;"
            "color:white;"
            "border-radius:10px;"
            "padding:10px;"
            );
    }
    else
    {
        messageLabel->setStyleSheet(
            "background:#F0F0F0;"
            "color:black;"
            "border-radius:10px;"
            "padding:10px;"
            );
    }

    QWidget* container = new QWidget;

    QHBoxLayout* layout =
        new QHBoxLayout(container);

    layout->setContentsMargins(
        10, 5, 10, 5);

    if(isUser)
    {
        layout->addStretch();
        layout->addWidget(messageLabel);
    }
    else
    {
        layout->addWidget(messageLabel);
        layout->addStretch();
    }

    // Insert before spacer

    ui->verticalLayout_Chat->insertWidget(
        ui->verticalLayout_Chat->count() - 1,
        container
        );

    // Auto-scroll

    QScrollBar* scrollBar =
        ui->scrollAreaChat->verticalScrollBar();

    scrollBar->setValue(
        scrollBar->maximum()
        );
}