#include "ResponseGenerator.h"
#include "../managers/CourseManager.h"
#include "../managers/RoutineManager.h"
#include "../managers/FaqManager.h"
#include "../managers/AdmissionManager.h"
#include "../models/Course.h"
#include "../models/Routine.h"
#include "../utils/CourseCodeUtils.h"

#include <QRegularExpression>
#include <QStringList>
#include <algorithm>



ResponseGenerator::ResponseGenerator(
    CourseManager* courseManager,
    RoutineManager* routineManager,
    FaqManager* faqManager,
    AdmissionManager* admissionManager)
    :
    courseManager(courseManager),
    routineManager(routineManager),
    faqManager(faqManager),
    admissionManager(admissionManager)
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

    // Pure delegation — CourseManager owns code extraction, name matching
    // (via TextMatcher), and its own WebCrawler fallback.
    case Intent::COURSE_INFO:
        return courseManager->findAnswer(userInput);

    // The ONE piece of real logic left in ResponseGenerator, and it's here
    // deliberately: RoutineManager must never call CourseManager, so if the
    // query names a course by NAME rather than by code, resolving that name
    // has to happen at the routing layer, not inside either manager.
    // Everything else (field parsing, searching, formatting) lives in
    // RoutineManager::findAnswer() now.
    case Intent::ROUTINE_QUERY:
    {
        QString resolvedCourseCode;

        if(CourseCodeUtils::extractCourseCode(userInput).isEmpty())
        {
            QString searchText =
                RoutineManager::stripRoutineNoiseWords(userInput);

            std::vector<Course> matches =
                courseManager->findByName(searchText);

            if(matches.size() == 1)
            {
                resolvedCourseCode = matches.front().getCode();
            }
            else if(matches.size() > 1)
            {
                QString response =
                    "Multiple courses matched your query:\n\n";

                for(const Course &course : matches)
                {
                    response += QString("%1 - %2\n")
                    .arg(course.getCode())
                        .arg(course.getName());
                }

                response += "\nPlease specify the exact course.";

                return response;
            }
            // matches.empty(): no course name found either — that's fine,
            // RoutineManager can still answer using program/year/semester
            // alone, or ask for more detail if it can't.
        }

        return routineManager->findAnswer(userInput, resolvedCourseCode);
    }

    // Pure delegation — FaqManager owns searching, scoring, small-talk data,
    // and its own WebCrawler fallback.
    case Intent::FAQ_QUERY:
        return faqManager->findAnswer(userInput);

    // Pure delegation — AdmissionManager owns searching, scoring, and its
    // own WebCrawler fallback.
    case Intent::ADMISSION_QUERY:
        return admissionManager->findAnswer(userInput);

    case Intent::UNKNOWN:
    default:
        return "Sorry, I could not understand your question.";
    }
}