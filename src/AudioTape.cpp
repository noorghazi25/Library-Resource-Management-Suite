// src/AudioTape.cpp

#include "AudioTape.h"

// Default constructor implementation
AudioTape::AudioTape() {
    std::strcpy(atno, "");
    std::strcpy(title, "");
    std::strcpy(artist, "");
    std::strcpy(format, "");
    duration = 0.0;
}

// Display audio tape details
void AudioTape::showAudioTape() const {
    std::cout << "\nAudioTape Number: " << atno;
    std::cout << "\nTitle: " << title;
    std::cout << "\nArtist: " << artist;
    std::cout << "\nFormat: " << format;
    std::cout << "\nDuration: " << duration << " minutes";
}

// Modify existing audio tape details
void AudioTape::modifyAudioTape() {
    std::cout << "\nModify AudioTape Title: ";
    std::cin.ignore(); // To ignore any leftover newline character
    std::cin.getline(title, 100);
    
    std::cout << "Modify Artist's Name: ";
    std::cin.getline(artist, 50);
    
    std::cout << "Modify Format: ";
    std::cin.getline(format, 20);
    
    std::cout << "Modify Duration (in minutes): ";
    std::cin >> duration;
}

// Display audio tape information in a formatted way
void AudioTape::report() const {
    std::cout << std::left << std::setw(10) << atno
              << std::setw(30) << title
              << std::setw(25) << artist
              << std::setw(15) << format
              << std::setw(10) << duration << std::endl;
}

// Overload the output operator
std::ostream& operator<<(std::ostream &os, const AudioTape &audiotape) {
    os << "AudioTape No: " << audiotape.atno
       << ", Title: " << audiotape.title
       << ", Artist: " << audiotape.artist
       << ", Format: " << audiotape.format
       << ", Duration: " << audiotape.duration << " minutes";
    return os;
}
