#include "ResponseGenerator.h"
#include <QRegularExpression>
ResponseGenerator::ResponseGenerator(
    CourseManager* courseManager,
    RoutineManager* routineManager/*,
    FaqManager* faqManager,
    AdmissionManager* admissionManager*/)
    : courseManager(courseManager),
    routineManager(routineManager)/*,
    faqManager(faqManager),
    admissionManager(admissionManager)*/
{
}

QString ResponseGenerator::generateResponse(
    Intent intent,
    const QString& userInput) const
{
    switch(intent)
    {
    case Intent::GREETING:
        return "Hello! How can I help you today?";

    case Intent::GOODBYE:
        return "Goodbye! Have a great day.";

    case Intent::COURSE_INFO:
    {
        QString upperInput = userInput.toUpper();

        QStringList words = upperInput.split(
            QRegularExpression("\\s+"),
            Qt::SkipEmptyParts);

        for(QString word : words)
        {
            word.remove('?');
            word.remove('.');
            word.remove(',');
            word.remove(':');
            word.remove(';');

            Course course =
                courseManager->findByCode(word);

            if(!course.getCode().isEmpty())
            {
                return QString(
                           "Course Code: %1\n"
                           "Course Name: %2\n"
                           "Credits: %3")
                    .arg(course.getCode())
                    .arg(course.getName())
                    .arg(course.getCredits());
            }
        }

        return "Course not found.";
    }
    case Intent::ROUTINE_QUERY:
    {
        return "Routine information found. "
               "Please specify program, year, semester and section.";
    }

    // case Intent::FAQ_QUERY:
    // {
    //     QString answer = faqManager.findAnswer(userInput);

    //     return answer;
    // }

    // case Intent::ADMISSION_QUERY:
    // {
    //     return admissionManager->findInfo(userInput);
    // }

    case Intent::UNKNOWN:
    default:
        return "Sorry, I could not understand your question.";
    }
}