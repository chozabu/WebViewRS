!include("../Common/retroshare_plugin.pri"): error("Could not include file ../Common/retroshare_plugin.pri")

greaterThan(QT_MAJOR_VERSION, 4) {
	# Qt 5
	QT += widgets
	QT += webkitwidgets
}

QT += webkit
QT += network
exists($$[QMAKE_MKSPECS]/features/mobility.prf) {
  CONFIG += mobility
} else {
  QT += multimedia
}
CONFIG += qt uic qrc resources
MOBILITY = multimedia

DEPENDPATH += ../../retroshare-gui/src/temp/ui ../../libretroshare/src
INCLUDEPATH += ../../retroshare-gui/src/temp/ui ../../libretroshare/src

#################################### Windows #####################################

linux-* {
	INCLUDEPATH += /usr/include
	LIBS += $$system(pkg-config --libs opencv)
}

win32 {
	LIBS_DIR = $$PWD/../../../libs
	LIBS += -L"$$LIBS_DIR/lib/opencv"

	OPENCV_VERSION = 249
	LIBS += -lopencv_core$$OPENCV_VERSION -lopencv_highgui$$OPENCV_VERSION -lopencv_imgproc$$OPENCV_VERSION -llibjpeg -llibtiff -llibpng -llibjasper -lIlmImf -lole32 -loleaut32 -luuid -lavicap32 -lavifil32 -lvfw32 -lz
}

QMAKE_CXXFLAGS *= -Wall

SOURCES = WebViewPlugin.cpp               \
          services/p3WebView.cc           \
          services/rsWebViewItems.cc \
    gui/WebViewMainpage.cpp \
    gui/WebViewNotify.cpp \
    gui/webenginepagepanel.cpp \
    gui/webviewrs.cpp \
    gui/webtabcontents.cpp \
    gui/webbridgers.cpp \
    gui/filedownloader.cpp

HEADERS = WebViewPlugin.h                 \
          services/p3WebView.h            \
          services/rsWebViewItems.h       \
          interface/rsWebView.h \
    gui/WebViewMainpage.h \
    gui/WebViewNotify.h \
    gui/webenginepagepanel.h \
    gui/webviewrs.h \
    gui/webtabcontents.h \
    gui/webbridgers.h \
    gui/filedownloader.h

#FORMS   = gui/AudioInputConfig.ui

TARGET = WebView

RESOURCES = gui/WebView_images.qrc \
    html_rc.qrc


LIBS += -lspeex -lspeexdsp

FORMS += \
    gui/WebViewMainpage.ui \
    gui/webenginepagepanel.ui \
    gui/webtabcontents.ui
