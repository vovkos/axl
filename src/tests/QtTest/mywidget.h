#pragma once

#define HEXEDIT_H

class MyWidget : public QLabel
{
    Q_OBJECT
 
public:
 
    enum Effect {
        Plain = 0,
        Sunken,
        Raised
    };
 
    MyWidget(QWidget* parent = 0);
 
    /* The action takes place here. */
    void paintEvent(QPaintEvent *event); 
 
    /* Getter and setter for the effect property. */
    Effect effect() const
    { 
		return effect_; 
	}
  
    /* After setting the property we have to repaint the widget. */
	
public:
    void setEffect(Effect effect)
    { 
		effect_ = effect; 
		repaint(); 
	}

private:
 
    /* Helper method. */
    void drawTextEffect(QPainter* painter, QPoint offset);
 
    Effect effect_;
};
