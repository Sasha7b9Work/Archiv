#pragma once

class String {
public:
    String();
    String(const char *_s);
    String(int _value);
    ~String();
    
    void Clear();
    int Length();

    String &operator+= (const char *_s);
    String &operator+= (const String &_string);
    const char *c_str() const;
private:
    char *data;
    int length;
};
