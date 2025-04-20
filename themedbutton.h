#pragma once

#include <QObject>
#include <QPushButton>
#include <QApplication>

class ThemedButton : public QPushButton
{
    Q_OBJECT
public:
    ThemedButton(QWidget *parent = nullptr);
    void setIcon(const QIcon &icon);

public slots:
    void setIconSize(const QSize &size);

protected:
    bool event(QEvent *e) override;

private slots:
    void updateColor();

private:
    QIcon m_baseIcon;
};
