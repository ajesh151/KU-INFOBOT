#include "CurriculumManager.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

bool CurriculumManager::loadCurriculum(const QString& filename)
{
    curriculum.clear();

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if(line.isEmpty())
            continue;

        QStringList p = line.split('|');

        if(p.size()!=6)
            continue;

        curriculum.emplace_back(
            p[0].toUpper(),
            p[1].toInt(),
            p[2].toInt(),
            p[3],
            p[4],
            p[5].toInt()
            );
    }

    return true;
}

QString CurriculumManager::extractProgram(const QString& text) const
{
    QString t=text.toUpper();

    if(t.contains("COMPUTER ENGINEERING"))
        return "CE";

    if(t.contains("CE"))
        return "CE";

    if(t.contains("COMPUTER SCIENCE"))
        return "CS";

    if(t.contains("CS"))
        return "CS";

    if(t.contains("CIVIL"))
        return "CIVIL";

    return "";
}

int CurriculumManager::extractYear(const QString& text) const
{
    QString t=text.toLower();

    if(t.contains("first year")) return 1;
    if(t.contains("second year")) return 2;
    if(t.contains("third year")) return 3;
    if(t.contains("fourth year")) return 4;

    QRegularExpression r("(\\d)");

    auto m=r.match(t);

    if(m.hasMatch())
        return m.captured(1).toInt();

    return -1;
}

int CurriculumManager::extractSemester(const QString& text) const
{
    QString t=text.toLower();

    if(t.contains("first semester")) return 1;
    if(t.contains("second semester")) return 2;

    QRegularExpression r("semester\\s*(\\d)");

    auto m=r.match(t);

    if(m.hasMatch())
        return m.captured(1).toInt();

    return -1;
}

QString CurriculumManager::findCurriculum(const QString& query) const
{
    QString program=extractProgram(query);

    int year=extractYear(query);

    int semester=extractSemester(query);

    if(program.isEmpty())
        return "Please specify the program.";

    QString result;

    result+="Curriculum\n";
    result+="Program : "+program+"\n";

    if(year!=-1)
        result+="Year : "+QString::number(year)+"\n";

    if(semester!=-1)
        result+="Semester : "+QString::number(semester)+"\n";

    result+="\n";

    int total=0;

    for(const Curriculum& c : curriculum)
    {
        if(c.getProgram()!=program)
            continue;

        if(year!=-1 && c.getYear()!=year)
            continue;

        if(semester!=-1 && c.getSemester()!=semester)
            continue;

        result += QString("%1\t%2\t(%3)\n")
                      .arg(c.getCode(),-10)
                      .arg(c.getName(),-45)
                      .arg(c.getCredit());

        total += c.getCredit();
    }

    if(total==0)
        return "Curriculum not found.";

    result+="\nTotal Credits : "+QString::number(total);

    return result;
}