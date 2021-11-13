#include <Arduino.h>
#include <QueueArray/QueueArray.h>
#include "BetterSerial.h"

BetterSerial::BetterSerial(HardwareSerial* serial, int baud) {
    HWSerial = serial;
    if(!HWSerial->availableForWrite()){
        HWSerial->begin(baud);
    }
    type = HARDWARE;
}

BetterSerial::BetterSerial(SoftwareSerial *serial, int baud) {
    SWSerial = serial;
    if(!SWSerial->availableForWrite()){
        SWSerial->begin(baud);
    }
    type = SOFTWARE;
}

void BetterSerial::setDivider(char new_divider) {
    divider = new_divider;
}

void BetterSerial::setTerminator(char new_terminator) {
    terminator = new_terminator;
}

void BetterSerial::setMaxWordSize(int size) {
    maxSize = size;
}

bool BetterSerial::serialAvailable() {
    if (type == SOFTWARE){
        return SWSerial->available();
    }
    if(type == HARDWARE){
        return HWSerial->available();
    }
    return false;
}

char BetterSerial::serialRead() {
    if (type == SOFTWARE){
        return (char)SWSerial->read();
    }
    if(type == HARDWARE){
        return (char)HWSerial->read();
    }
    return 0;
}

bool BetterSerial::wordAvailable() {
    return !operating_line.isEmpty();
}

bool BetterSerial::lineAvailable() {
    return !commands.isEmpty();
}

bool BetterSerial::getWord(char* output_array) {
    int length = 0;
    if(wordAvailable()){
        return false;
    }
    operating_word = operating_line.dequeue();
    while(!operating_word.isEmpty() && length < maxSize){
        output_array[length] = operating_word.dequeue();
    }
    return true;
}

void BetterSerial::nextLine() {
    if(lineAvailable()){
        operating_line = commands.dequeue();
    }
}

void BetterSerial::tick() {
    char buffer_char;
    while(serialAvailable()){
        buffer_char = serialRead();
        if (!(buffer_char == terminator || buffer_char == divider)){
            saving_word.enqueue(buffer_char);
        }
        else{
            saving_word.enqueue('\0');
            saving_line.enqueue(saving_word);
            if (buffer_char == terminator){
                commands.enqueue(saving_line);
            }
        }
    }
}