#ifndef PROGRAMCODEUTILS_H
#define PROGRAMCODEUTILS_H

#include <QString>

// Single source of truth for turning free text into the short program
// code stored in routines.txt (e.g. "CE", "CS", "BIT", "ME").
//
// This exists because SynonymMapper expands an abbreviation like "ce" to
// its FULL name ("computer engineering") upstream of RoutineManager — so
// by the time RoutineManager sees the query, the short code itself may no
// longer be present at all. extractProgramCode() checks for the short
// code first (covers callers that bypass SynonymMapper, e.g. tests), then
// falls back to recognizing the expanded full name.
//
// IMPORTANT: the full-name mappings here mirror the program-related phrase
// canonicals in data/synonyms.txt. If you add or rename a program there,
// mirror the change here too — this is the same knowledge expressed
// twice out of necessity (one is human-edited synonym data for general
// text matching, the other is typed lookup code for structured routine
// data), and the two can drift if only one is updated.
namespace ProgramCodeUtils
{
// Finds the short program code referenced anywhere in `text` —
// matching either the short code itself or its expanded full name —
// or an empty QString if none is found.
QString extractProgramCode(const QString& text);
}

#endif // PROGRAMCODEUTILS_H