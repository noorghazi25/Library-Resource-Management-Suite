// include/AudioTape.h

#ifndef AUDIOTAPE_H
#define AUDIOTAPE_H

#include <iostream>
#include <iomanip>
#include <cstring>

class AudioTape {
public:
    char atno[6];        // AudioTape Number (e.g., "AT001")
    char title[100];     // AudioTape Title
    char artist[50];     // Artist Name
    char format[20];     // Format (e.g., "Cassette", "CD")
    double duration;     // Duration in minutes

    // Default constructor initializes members to default values
    AudioTape();

    // Function to display audio tape details
    void showAudioTape() const;

    // Function to modify existing audio tape details
    void modifyAudioTape();

    // Function to display audio tape information in a formatted way
    void report() const;

    // Overload the output operator for easy display
    friend std::ostream& operator<<(std::ostream &os, const AudioTape &audiotape);
};

#endif // AUDIOTAPE_H
