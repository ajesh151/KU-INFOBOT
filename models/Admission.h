models/admisiion.h


#ifndef ADMISSION_H
#define ADMISSION_H

#include <QString>

class Admission
{
private:
    QString question;
    QString answer;

public:
    Admission();

    Admission(
        const QString& question,
        const QString& answer);

    QString getQuestion() const;
    QString getAnswer() const;

    void setQuestion(const QString& question);
    void setAnswer(const QString& answer);
};

#endif // ADMISSION_H