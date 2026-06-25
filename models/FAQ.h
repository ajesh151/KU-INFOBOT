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
    Faq(const QString& question,const QString& answer);

    QString getQuestion() const;
    QString getAnswer() const;

    void setQuestion(const QString& question);
    void setAnswer(const QString& answer);
};

#endif