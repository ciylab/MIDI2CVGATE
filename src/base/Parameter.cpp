#include "Parameter.h"

Parameter::Parameter(char name[8], 
                int min, int max, 
                int defaultValue, 
                Parameter::Type type) {
    Parameter::copy(name, this->name, 8);
    this->min = min;
    this->max = max;
    this->value = defaultValue;
    this->defaultValue = defaultValue;
    this->type = type;
}

void Parameter::copy(char src[8], char dest[8], int length) {
    int i = 0;
    while(i < length && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    while(i < length) {
        dest[i] = ' ';
        i++;
    }
}

