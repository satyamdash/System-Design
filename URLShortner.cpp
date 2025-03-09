#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <random>

using namespace std;

class URLShortener {
private:
    unordered_map<string, string> shortToLong;
    unordered_map<string, string> longToShort;
    const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int counter = 100000; // Start counter to generate unique IDs

    string base62Encode(int num) {
        string shortURL;
        while (num) {
            shortURL.push_back(chars[num % 62]);
            num /= 62;
        }
        while (shortURL.size() < 6) shortURL.push_back('a'); // Ensure 6 chars
        reverse(shortURL.begin(), shortURL.end());
        return shortURL;
    }

public:
    string shortenURL(const string& longURL) {
        if (longToShort.find(longURL) != longToShort.end()) {
            return longToShort[longURL];
        }
        
        string shortURL = base62Encode(counter++);
        shortToLong[shortURL] = longURL;
        longToShort[longURL] = shortURL;
        
        return shortURL;
    }

    string getLongURL(const string& shortURL) {
        if (shortToLong.find(shortURL) != shortToLong.end()) {
            return shortToLong[shortURL];
        }
        return "Error: Short URL not found";
    }
};

int main() {
    URLShortener urlShortener;
    
    string longURL1 = "https://www.example.com/some/very/long/url";
    string longURL2 = "https://www.anotherexample.com/page1";
    
    string shortURL1 = urlShortener.shortenURL(longURL1);
    string shortURL2 = urlShortener.shortenURL(longURL2);
    
    cout << "Shortened URL for " << longURL1 << " -> " << shortURL1 << endl;
    cout << "Shortened URL for " << longURL2 << " -> " << shortURL2 << endl;
    
    cout << "Redirecting " << shortURL1 << " -> " << urlShortener.getLongURL(shortURL1) << endl;
    cout << "Redirecting " << shortURL2 << " -> " << urlShortener.getLongURL(shortURL2) << endl;
    
    return 0;
}
