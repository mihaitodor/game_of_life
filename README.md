# Game of Life
Conway's Game of Life GUI implementation using the Nana C++ library

![preview](res/GoL.png?raw=true)

## Built using Microsoft Visual Studio Community 2015 

## Dependencies
* [Nana C++ Library](http://www.nanapro.org/)
* [Google Test](https://github.com/google/googletest)

## TODO
* Add architecture diagram
* Figure out why increasing the game grid size to 58x58 or above prevents the UI from rendering
* Investigate if a simpler control can be used for the game cells instead of `nana/gui/widgets/label.hpp`
* Leverage CMake to manage the build process
* Test on Linux and OS X

Please feel free to open bugs and submit pull requests.