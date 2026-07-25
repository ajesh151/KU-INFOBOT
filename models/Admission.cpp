#include "Admission.h"

Admission::Admission()
{
}

Admission::Admission(
    const QString& question,
    const QString& answer)
    : question(question),
      answer(answer)
{
}

QString Admission::getQuestion() const
{
    return question;
}

QString Admission::getAnswer() const
{
    return answer;
}
