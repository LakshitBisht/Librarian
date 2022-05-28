QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addbooks.cpp \
    admin_accountdetails.cpp \
    admin_dashboard.cpp \
    admin_messages.cpp \
    adminapplication.cpp \
    bookdetails.cpp \
    forgot_password.cpp \
    issuedbooks.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    member_dashboard.cpp \
    member_messages.cpp \
    memberadmindetails.cpp \
    registrationlog.cpp \
    signup_admin.cpp \
    signup_member.cpp

HEADERS += \
    addbooks.h \
    admin_accountdetails.h \
    admin_dashboard.h \
    admin_messages.h \
    adminapplication.h \
    bookdetails.h \
    database.h \
    forgot_password.h \
    issuedbooks.h \
    login.h \
    mainwindow.h \
    member_dashboard.h \
    member_messages.h \
    memberadmindetails.h \
    registrationlog.h \
    signup_admin.h \
    signup_member.h

FORMS += \
    addbooks.ui \
    admin_accountdetails.ui \
    admin_dashboard.ui \
    admin_messages.ui \
    adminapplication.ui \
    bookdetails.ui \
    forgot_password.ui \
    issuedbooks.ui \
    login.ui \
    mainwindow.ui \
    member_dashboard.ui \
    member_messages.ui \
    memberadmindetails.ui \
    registrationlog.ui \
    signup_admin.ui \
    signup_member.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
