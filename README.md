# KU InfoBot

A desktop-based, rule-based chatbot developed using **C++** and the **Qt Framework** to provide information about **Kathmandu University**. The chatbot assists users by answering queries related to courses, curricula, admissions, fees, class routines, entrance examinations, and frequently asked questions through an interactive graphical user interface.

---

## Features

- Rule-based chatbot architecture
- Interactive Qt-based graphical user interface
- Typo correction using the Levenshtein Distance algorithm
- Synonym and abbreviation mapping
- Intent recognition using keyword-based rules
- Course and curriculum information retrieval
- Class routine retrieval
- Admission, fee, and entrance examination information
- Frequently Asked Questions (FAQ) support
- WebCrawler fallback for unavailable local information
- Modular Object-Oriented Programming (OOP) design
- Fast retrieval using structured text-based knowledge files

---

## System Architecture

The chatbot follows a sequential processing pipeline:

```
User Input
     │
     ▼
TypoCorrector
     │
     ▼
SynonymMapper
     │
     ▼
IntentRecognizer
     │
     ├──────── Unknown Intent ────────► WebCrawler
     │                                     │
     ▼                                     ▼
ResponseGenerator                    Web Response
     │
     ▼
Select Appropriate Manager
     │
     ├── CourseManager
     ├── RoutineManager
     ├── AdmissionManager
     └── FaqManager
     │
     ├──────── No Match ─────────────► WebCrawler
     │                                     │
     ▼                                     ▼
 Local Response                     Web Response
           │
           ▼
     Display Response
```

---



## Information Base

The chatbot stores information in structured text files.

| File | Description |
|------|-------------|
| courses.txt | Courses information |
| curriculum.txt | Curriculum information |
| routines.txt | Class routine information |
| admissions.txt | Admission requirements and procedures |
| fees.txt | Tuition fee information |
| entrance.txt | Entrance examination information |
| faq.txt | Frequently asked questions |
| synonyms.txt | Synonyms and abbreviations |
| typo_dictionary.txt | Dictionary used for typo correction |

---

## Technologies Used

| Technology | Purpose |
|------------|---------|
| C++17 | Core programming language |
| Qt 6.11.1 | GUI framework |
| Qt Widgets | Desktop user interface |
| CMake | Build system |
| MinGW GCC | Compiler |
| Git | Version control |
| GitHub | Source code hosting |

---

## Object-Oriented Design

The project follows Object-Oriented Programming principles.

- Encapsulation
- Abstraction
- Inheritance (where applicable)
- Polymorphism (where applicable)
- Modular class design

Major classes include:

- ChatBot
- TypoCorrector
- SynonymMapper
- IntentRecognizer
- ResponseGenerator
- CourseManager
- RoutineManager
- AdmissionManager
- FaqManager
- WebCrawler

---

## Building the Project

### Prerequisites

- C++17 Compiler
- Qt 6.11.1
- CMake 3.16 or later
- MinGW GCC

### Clone the Repository

```bash
git clone https://github.com/ajesh151/KU_INFOBOT.git
cd KU_INFOBOT
```

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run

```bash
./KU_INFOBOT
```

On Windows:

```bash
KU_INFOBOT.exe
```

---


## Error Handling

The chatbot handles:

- Misspelled words
- Abbreviations
- Synonymous terms
- Unknown intents
- Missing local information
- Invalid queries

When no local information is available, the chatbot automatically searches the official Kathmandu University website. If no relevant information is found, it informs the user that the requested information is unavailable.

---

## Future Improvements

- Voice-based interaction
- Multilingual support
- Database integration
- Semantic search
- Personalized responses
- Expanded university knowledge base
- Mobile application support

---

## Testing

The application has been tested for:

- Course retrieval
- Curriculum retrieval
- Admission information
- Fee information
- Routine retrieval
- FAQ retrieval
- Typo correction
- Synonym mapping
- Unknown query handling
- WebCrawler fallback

---


## License

This project was developed as an academic project for educational purposes.

---

## Authors

**Ajesh Pokhrel** \
**Prayag Parajuli** \
**Smriti Niroula** \
**Aislinn Paudel** \
**Prashis Prasai** 

Department of Computer Science and Engineering  
Kathmandu University

---

## Acknowledgements

The authors would like to thank our Supervisor, Mr. Sushil Nepal and the Department of Computer Science and Engineering, Kathmandu University, for providing guidance and resources throughout the development of this project. Appreciation is also extended to the developers of the Qt Framework and the open-source software community for the tools and libraries used during implementation.