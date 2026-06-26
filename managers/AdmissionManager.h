managers/admisiionmanager.h

#ifndef ADMISSIONMANAGER_H
#define ADMISSIONMANAGER_H

#include <vector>
#include <QString>

#include "../models/Admission.h"

class AdmissionManager
{
private:
    std::vector<Admission> admissions;

public:
    AdmissionManager();

    bool loadData(const QString& filename);

    std::vector<Admission> getAllData() const;

    QString findAnswer(const QString& question) const;

    std::vector<Admission> searchData(const QString& keyword) const;
};

#endif // ADMISSIONMANAGER_H