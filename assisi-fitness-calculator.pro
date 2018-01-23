TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG = opencv
LIBS += -lboost_program_options

SOURCES += main.cpp \
    evolutionary-parameters.cpp \
    analysis-parameters.cpp \
    image.cpp \
    histogram.cpp \
    fitness-function.cpp \
    csv.cpp

HEADERS += \
    evolutionary-parameters.hpp \
    util.hpp \
    analysis-parameters.hpp \
    image.hpp \
    histogram.hpp \
    fitness-function.hpp \
    csv.hpp
