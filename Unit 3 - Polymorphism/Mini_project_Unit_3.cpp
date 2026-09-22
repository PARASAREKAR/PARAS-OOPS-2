#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// Abstract Base Class
class Media {
protected:
    string title;
    double sizeMB;

public:
    Media(const string& title, double sizeMB)
        : title(title), sizeMB(sizeMB) {}

    // Pure virtual functions forcing polymorphism in derived classes
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void showDetails() const = 0;

    virtual ~Media() = default; // Virtual destructor for proper memory cleanup
};

// Derived Class 1: Audio
class Audio : public Media {
private:
    string artist;
    int durationSec;

public:
    Audio(const string& title, double sizeMB, const string& artist, int durationSec)
        : Media(title, sizeMB), artist(artist), durationSec(durationSec) {}

    void play() override {
        cout << "[Audio Playing] ♪ " << title << " by " << artist << " ♪\n";
    }

    void pause() override {
        cout << "[Audio Paused] " << title << "\n";
    }

    void stop() override {
        cout << "[Audio Stopped] " << title << "\n";
    }

    void showDetails() const override {
        cout << "Type: Audio | Title: " << title 
             << " | Artist: " << artist 
             << " | Duration: " << durationSec << "s"
             << " | Size: " << sizeMB << " MB\n";
    }
};

// Derived Class 2: Video
class Video : public Media {
private:
    string resolution;
    int durationSec;

public:
    Video(const string& title, double sizeMB, const string& resolution, int durationSec)
        : Media(title, sizeMB), resolution(resolution), durationSec(durationSec) {}

    void play() override {
        cout << "[Video Playing] ▶ " << title << " (" << resolution << ")\n";
    }

    void pause() override {
        cout << "[Video Paused] ❚❚ " << title << "\n";
    }

    void stop() override {
        cout << "[Video Stopped] ◼ " << title << "\n";
    }

    void showDetails() const override {
        cout << "Type: Video | Title: " << title 
             << " | Resolution: " << resolution 
             << " | Duration: " << durationSec << "s"
             << " | Size: " << sizeMB << " MB\n";
    }
};

// Derived Class 3: Image
class Image : public Media {
private:
    string resolution;

public:
    Image(const string& title, double sizeMB, const string& resolution)
        : Media(title, sizeMB), resolution(resolution) {}

    void play() override {
        cout << "[Displaying Image] 🖼 " << title << " (" << resolution << ")\n";
    }

    void pause() override {
        cout << "[Image] Freeze display frame for: " << title << "\n";
    }

    void stop() override {
        cout << "[Closing Image] " << title << "\n";
    }

    void showDetails() const override {
        cout << "Type: Image | Title: " << title 
             << " | Dimensions: " << resolution 
             << " | Size: " << sizeMB << " MB\n";
    }
};

// Main Media Playlist Manager
int main() {
    // Collection of base-class pointers (unique_ptr for automatic memory management)
    vector<unique_ptr<Media>> playlist;

    // Populating media items polymorphically
    playlist.push_back(make_unique<Audio>("Bohemian Rhapsody", 8.5, "Queen", 354));
    playlist.push_back(make_unique<Video>("Inception Trailer", 120.0, "1080p", 148));
    playlist.push_back(make_unique<Image>("Sunset Beach", 4.2, "3840x2160"));

    cout << "================ MEDIA PLAYLIST DETAILS ================\n";
    for (const auto& media : playlist) {
        media->showDetails();
    }

    cout << "\n================ DEMO PLAYBACK CONTROLS ================\n";
    for (const auto& media : playlist) {
        media->play();
        media->pause();
        media->stop();
        cout << "--------------------------------------------------------\n";
    }

    return 0;
}