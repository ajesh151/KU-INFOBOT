#ifndef ADMISSIONMANAGER_H
#define ADMISSIONMANAGER_H

#include <QString>
#include <QStringList>
#include <vector>

#include "../models/Admission.h"

class WebCrawler; // forward declaration only — full include lives in the .cpp

// Owns the merged admission/fees/entrance question-answer database and
// answers queries using the shared TextMatcher (exact -> phrase -> word ->
// keyword). Falls back to WebCrawler itself when there's no local match, so
// ResponseGenerator never has to know that fallback exists.
class AdmissionManager
{
public:
    // webCrawler may be nullptr (e.g. in unit tests) — findAnswer simply
    // skips the crawler fallback in that case.
    explicit AdmissionManager(WebCrawler* webCrawler = nullptr);

    // Loads one file in "question|answer" format into the database.
    // Deliberately ADDITIVE — it does not clear existing entries — so
    // admissions.txt, fees.txt, and entrance.txt can all be merged into a
    // single unified database by calling this once per file. Prefer
    // loadAllSources() when loading all of them at once.
    bool loadData(const QString& filename);

    // Convenience wrapper: loads several admission-format files into one
    // unified database in a single call, e.g.:
    //   loadAllSources({"data/admissions.txt", "data/fees.txt", "data/entrance.txt"});
    bool loadAllSources(const QStringList& filenames);

    std::vector<Admission> getAllData() const;

    // Finds the best-matching answer via TextMatcher's four-tier search.
    // Falls back to WebCrawler (if provided) when nothing matches locally,
    // and only then returns the final "not found" message.
    QString findAnswer(const QString& question) const;

    std::vector<Admission> searchData(const QString& keyword) const;

private:
    std::vector<Admission> admissions;
    WebCrawler* webCrawler;
};

#endif // ADMISSIONMANAGER_H