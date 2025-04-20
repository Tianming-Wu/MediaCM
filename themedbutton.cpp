#include "themedbutton.h"

#include <QPalette>
#include <QEvent>
#include <QPainter>

ThemedButton::ThemedButton(QWidget *parent)
    : QPushButton(parent)
{
}

void ThemedButton::setIcon(const QIcon &icon)
{
    m_baseIcon = icon;
    updateColor();
}

void ThemedButton::setIconSize(const QSize &size)
{
    QPushButton::setIconSize(size);
    updateColor();
}

bool ThemedButton::event(QEvent *e)
{
    if (e->type() == QEvent::ApplicationPaletteChange) {
        updateColor();
    }
    return QPushButton::event(e);
}

void ThemedButton::updateColor()
{
    if (m_baseIcon.isNull()) return;

    QColor targetColor = palette().color(QPalette::Text);

    QSize pixmapSize = iconSize().isEmpty() ? QSize(16, 16) : iconSize();

    QPixmap basePixmap = m_baseIcon.pixmap(pixmapSize);
    QPixmap coloredPixmap(pixmapSize);
    coloredPixmap.fill(Qt::transparent);

    QPainter painter(&coloredPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, basePixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(coloredPixmap.rect(), targetColor);
    painter.end();

    // 更新按钮图标
    QPushButton::setIcon(QIcon(coloredPixmap));
}
