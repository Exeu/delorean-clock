#include <SevenSegmentExtended.h>


class SegmentRow {
    private:
        SevenSegmentExtended *displayDate;
        SevenSegmentExtended *displayYear;
        SevenSegmentExtended *displayTime;
    public:
        SegmentRow(
            SevenSegmentExtended &dispDate, 
            SevenSegmentExtended &dispYear, 
            SevenSegmentExtended &dispTime): 
                displayDate(&dispDate), 
                displayYear(&dispYear), 
                displayTime(&dispTime) { 

        };
        void start(int backlight);
        void updateRow(char date[], int year, int hour, int minute);
};