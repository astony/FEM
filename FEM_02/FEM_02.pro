QT += core
QT -= gui

CONFIG += c++11

TARGET = FEM_02
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    algorithm_method.cpp \
    element.cpp \
    mesh.cpp \
    node.cpp

HEADERS += \
    algorithm_method.hpp \
    element.hpp \
    mesh.hpp \
    node.hpp

DISTFILES += \
    docs/FEM_02.pdf \
    data/mesh.csv \
    docs/report.docx


