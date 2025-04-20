#include "themecm.h"

#include <QEvent>
#include <QFile>
#include <QPalette>
#include <QRegularExpression>

ThemeCM::ThemeCM(QWidget *parent)
    : QWidget(parent)
{
    this->setVisible(false);
    QFile ss(":/style/ui/base.qss");
    ss.open(QFile::ReadOnly);
    stylesheet = ss.readAll();
    ss.close();
}

QString ThemeCM::generate()
{
    QString newstyle = stylesheet;
    QColor windowbg = palette().color(QPalette::Window);
    QColor text = palette().color(QPalette::Text);

    // QRegularExpression reg_bg(R"\${WINDOWBG:(\d3)}");

    // QRegularExpressionMatch match;
    // try {
    //     match = reg_bg.match(newstyle);
    // } catch(...) {
    //     qDebug() << "exception thrown by QRegularExpression::match()";
    // }
    // try {
    //     if(match.hasMatch()) {

    //     }
    // } catch(...) {
    //     qDebug() << "exception thrown by QRegularExpressionMatch::hasMatch()";
    // }

    windowbg.setAlpha(120);
    newstyle.replace("${WINDOWBG:120}", windowbg.name(QColor::HexArgb));

    // qDebug() << newstyle;

    return newstyle;
}

bool ThemeCM::event(QEvent *e)
{
    if (e->type() == QEvent::ApplicationPaletteChange) {
        emit ThemeChanged();
    }
    return QObject::event(e);
}
