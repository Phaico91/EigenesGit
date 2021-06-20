#ifndef TURNWHEEL_H
#define TURNWHEEL_H

typedef struct Turningwheel Turningwheel;

Turningwheel *turningwheel_new(unsigned int numofwheels,
                               const unsigned int *sizes);

const unsigned int *turningwheel_next(Turningwheel *tw);

void turningwheel_delete(Turningwheel *tw);

#endif
