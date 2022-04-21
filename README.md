Funciones y macros de propósito general, enfocadas a desarrollo de aplicaciones embebidas específicamente para microcontroladores PIC de 8 bits, pudiendo ser adaptadas a otros microcontroladores según se necesite.

# Scripts de prueba mediante CMake

## Windows: 
* Agregar archivo CMakeLists.txt con los archivos requeridos, nombre de proyecto y compilador adecuados (ver documentación).
* Crear carpeta ==build/== para que cmake genere todos los archivos necesarios ahí.
### En caso de usar MinGW:
* CMake se configura con el comando ```cmake -S . -B .\build\ -G "MinGW Makefiles"```
* finalmente, se ejecuta en la carpeta ==build/== el comando ```mingw32-make```