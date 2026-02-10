#include <iostream>
#include <string>
#include "ConsoleApp.h"

using namespace std;

// Se mantiene el main limpio gracias a la division de capas
int main() {
	ConsoleApp app; //Toda la logica de la aplicacion esta encapsulada en la clase ConsoleApp
    app.run();
    return 0;

}

