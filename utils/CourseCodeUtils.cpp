#include "CourseCodeUtils.h"

#include <QRegularExpression>

QString CourseCodeUtils::extractCourseCode(const QString& text)
{
    static const QRegularExpression pattern("[A-Za-z]{3,5}\\d{3}[A-Za-z]*");

    auto match = pattern.match(text);

    if(match.hasMatch())
    {
        return match.captured(0).toUpper();
    }

    return QString();
}