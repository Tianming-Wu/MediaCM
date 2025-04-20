#include "videoui.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MediaCM_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QUrl videoUrl;

    qsizetype cpos = 1;
    QStringList arg = app.arguments();
    while(cpos < argc) {
        if(arg[cpos] == "-f" || arg[cpos] == "--file") {
            if(cpos + 1 < argc) {
                if(videoUrl.isEmpty())
                    videoUrl = QUrl::fromLocalFile(arg[cpos+1]);
                else return 1;
                cpos += 2;
            } else return 2;
        } else if(arg[cpos] == "-u" || arg[cpos] == "--url") {
            if(cpos + 1 < argc) {
                if(videoUrl.isEmpty())
                    videoUrl = arg[cpos+1];
                else return 3;
                cpos += 2;
            } else return 4;
        } else return 5;
    }

    VideoUI ui;
    ui.show();

    if(videoUrl.isEmpty()) videoUrl = QUrl::fromLocalFile("/home/tianming/Videos/test.mp4");
    ui.loadVideo(videoUrl);

    return app.exec();
}
