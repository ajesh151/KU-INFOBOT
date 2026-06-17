#ifndef FAQMANAGER_H
#define FAQMANAGER_H

#include <vector>
#include <QString>

#include "../models/Faq.h"

class FaqManager
{
private:
    std::vector<Faq> faqs;

public:
    FaqManager();

    bool loadFaqs(const QString& filename);

    std::vector<Faq> getAllFaqs() const;

    QString findAnswer(const QString& question) const;

    std::vector<Faq> searchFaqs(const QString& keyword) const;
};

#endif // FAQMANAGER_H