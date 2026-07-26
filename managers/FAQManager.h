#ifndef FAQMANAGER_H
#define FAQMANAGER_H

#include <QString>
#include <vector>

#include "../models/FAQ.h"

class WebCrawler; // forward declaration only — full include lives in the .cpp

// Owns the FAQ question-answer database and answers queries using the
// shared TextMatcher (exact -> phrase -> word -> keyword). Falls back to
// WebCrawler itself when there's no local match, so ResponseGenerator never
// has to know that fallback exists.
//
// Note: general chatbot small-talk ("what's your name?", "thank you", "how
// are you?") lives here as ordinary FAQ data entries rather than as special
// cases in ResponseGenerator — see data/faq.txt.
class FaqManager: public InformationManager
{
public:
    // webCrawler may be nullptr (e.g. in unit tests) — findAnswer simply
    // skips the crawler fallback in that case.
    explicit FaqManager(WebCrawler* webCrawler = nullptr);

    bool loadFaqs(const QString& filename);

    std::vector<Faq> getAllFaqs() const;

    // Finds the best-matching answer via TextMatcher's four-tier search.
    // Falls back to WebCrawler (if provided) when nothing matches locally,
    // and only then returns the final "not found" message.
    QString findAnswer(const QString& question) const override;

    std::vector<Faq> searchFaqs(const QString& keyword) const;

private:
    std::vector<Faq> faqs;
    WebCrawler* webCrawler;
};

#endif // FAQMANAGER_H