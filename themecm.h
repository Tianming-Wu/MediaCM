#pragma once

#include <QWidget>

class ThemeCM : public QWidget
{
    Q_OBJECT
public:
    explicit ThemeCM(QWidget *parent = nullptr);

    QString generate();

protected:
    bool event(QEvent *e) override;

signals:
    void ThemeChanged();

private:
    QString stylesheet;
};
