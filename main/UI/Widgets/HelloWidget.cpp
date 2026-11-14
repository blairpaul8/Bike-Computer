// HelloWidget.cpp

#include "HelloWidget.hpp"

void HelloWidget::draw(Canvas& c)
{
    c.writeString(kGreetingStringX, kGreetingStringY,
                    "Well hello there ELI!\nHow is your day going?");


    c.writeString(kSecondStringX, kSecondStringY,
                    "Test Second string\nHeartRate: 0"); // replace with text var

}