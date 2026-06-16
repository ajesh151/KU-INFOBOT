#ifndef FAQ_H
#define FAQ_H

#include <QString>

class Faq
{
private:
    QString question;
    QString answer;

public:
    Faq();
    Faq(QString question, QString answer);

    QString getQuestion() const;
    QString getAnswer() const;

    void setQuestion(QString question);
    void setAnswer(QString answer);
};

#endif