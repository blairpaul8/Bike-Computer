// HelloWidget.cpp

#include "HelloWidget.hpp"

void HelloWidget::draw(Canvas& c)
{
    c.writeString(kGreetingStringX, kGreetingStringY,
                    "Well hello there ELI!\nHow is your day going?");

    // Call update hr and print to string
    // updateHeartRate();
    // char text[48];

    // snprintf(text, sizeof(text),
    //     "Test Second string\nHeartRate: %" PRIu16, heartRate);

    c.writeString(kSecondStringX, kSecondStringY,
                    "Test Second string\nHeartRate: 0"); // replace with text var

}



// Add other functions that are needed here
// For example if this was drawing heart rate field
// you might want a function to get the heart rate
//
// void updateHeartRate() {
//     hr_ = client->getHeartRate();
// }