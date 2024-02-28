#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MyButton(QWidget *parent = nullptr);

signals:
    void leftClicked();
    void rightClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // MYBUTTON_H