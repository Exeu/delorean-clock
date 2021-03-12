#include <SegmentRow.h>

void SegmentRow::start(int backlight) {
    this->displayDate->begin();
    this->displayDate->setBacklight(backlight);

    this->displayYear->begin();
    this->displayYear->setBacklight(backlight);

    this->displayTime->begin();
    this->displayYear->setBacklight(backlight);
};

void SegmentRow::updateRow(char date[], int year, uint8_t hour, uint8_t minute) {
    this->displayDate->print(date);
    this->displayYear->printNumber(year);
    this->displayTime->printTime(hour, minute, false);
}