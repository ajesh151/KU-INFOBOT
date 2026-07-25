#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "chatbot/ChatBot.h"
#include "managers/CourseManager.h"
#include "managers/RoutineManager.h"
#include "managers/FAQManager.h"
#include "managers/AdmissionManager.h"
#include "crawler/WebCrawler.h"
#include "managers/CurriculumManager.h"
#include "utils/CourseResolver.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMessage();

private:
    Ui::MainWindow *ui;

    ChatBot* chatbot;

    CourseManager courseManager;
    CourseResolver courseResolver;
    RoutineManager routineManager;
    FaqManager faqManager;
    AdmissionManager admissionManager;
    CurriculumManager curriculumManager;
    WebCrawler webCrawler;

    void addMessage(const QString& text, bool isUser);
};

#endif // MAINWINDOW_H