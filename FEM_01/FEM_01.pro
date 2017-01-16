QT += core
QT -= gui

CONFIG += c++11

TARGET = FEM_01
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += \
       -lboost_system\

SOURCES += main.cpp \
    mesh.cpp \
    element.cpp \
    node.cpp \
    algorithm_method.cpp

HEADERS += \
    mesh.hpp \
    element.hpp \
    node.hpp \
    algorithm_method.hpp

DISTFILES += \
    docs/FEM_01.pdf \
    docs/functional minimizing.xlsx \
    docs/report.docx \
    data/mesh_3.txt \
    data/mesh_5.txt
