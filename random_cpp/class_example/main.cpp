/*
Here is a test of your code. Feel free to play with it but there's
no need to edit this file. Remember, you're only trying to make your code
compile.
*/
#include <Car.h>

int main()
{
    Car car;
    
    // try to drive 10 times
    for (int i = 0; i < 10; i++) {
        bool didDrive = car.drive();
        if (!didDrive) {
            // car is broken! must fix it
            car.fix();
        }
    }
    
    return 0;
}
