#ifndef INFORMATIONMANAGER_H
#define INFORMATIONMANAGER_H

#include <QString>

class InformationManager
{
public:
    virtual ~InformationManager() = default;

    virtual QString findAnswer(const QString& query) const = 0;
};

#endif // INFORMATIONMANAGER_H