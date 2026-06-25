#include "Faq.h"

Faq::Faq()
{
}

Faq::Faq(const QString &question,
         const QString &answer)
    : question(question),
    answer(answer)
{
}

QString Faq::getQuestion() const
{
    return question;
}

QString Faq::getAnswer() const
{
    return answer;
}

void Faq::setQuestion( const QString& question)
{
    this->question = question;
}

void Faq::setAnswer( const QString& answer)
{
    this->answer = answer;
}