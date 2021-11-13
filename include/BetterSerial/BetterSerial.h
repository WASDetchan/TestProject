//
// Serial better parsing and sending library
//

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <QueueArray/QueueArray.h>

#ifndef BETTER_SERIAL_H
#define BETTER_SERIAL_H

enum SerialType{
    NOT_DEFINED,
    HARDWARE,
    SOFTWARE,
};


class BetterSerial{
public:
    BetterSerial(HardwareSerial* serial, int baudRate);
    BetterSerial(SoftwareSerial* serial, int baudRate);

    void setDivider(char divider);
    void setTerminator(char terminator);
    void setMaxWordSize(int size);

    bool wordAvailable();
    bool lineAvailable();

    bool getWord(char* output_array);
    void nextLine();

    void tick();

private:
    bool serialAvailable();

    char serialRead();

    HardwareSerial* HWSerial{};
    SoftwareSerial* SWSerial{};
    SerialType type = NOT_DEFINED;
    QueueArray<QueueArray<QueueArray<char>>> commands = {};
    QueueArray<QueueArray<char>> saving_line;
    QueueArray<QueueArray<char>> operating_line = {};
    QueueArray<char> saving_word;
    QueueArray<char> operating_word = {};
    char divider = ' ', terminator = '\n';
    int maxSize = 32;
};

#endif //BETTER_SERIAL_H
