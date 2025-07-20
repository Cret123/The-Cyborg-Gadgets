# The Watch Extensions

This repository contains a series of programs designed for a customizable multitool watch extension. 
The watches are split into various generations:

- The single Generation 0 gadget that was built is a simple AA battery powered multitool using crocodile clips rather than switches to create a simple multitool gadget. 
While not really a watch, it did provide the inspiration needed for this project and is therefore counted as the first watch extension.
- Generation 1 featured a simple parallel circuit on a circuit board, introducing switches and using a LiPo battery for size. 
- Generation 2 still retained the same circuit, however was built with wires rather than a prototype board. 
This enhanced its lifespan from roughly 3 weeks to more than 2 years, as well as making assembly easier. 
- Currently in Generation 3, watches use an arduino to transform an everyday watch into a smart, 
multifunctional spy gadget, with the surprising side effect of saving the amount of power used.
- Generation 4 watches will still have the same user experience as Gen 3 watches, however will use a more powerful processor and have fully customised boards,
allowing for more compact circuitry.
- Finally, 5th Generation watches will use ESP32-C3's rather than arduinos, thereby drastically improving capabilities while still retaining its small size.

## Usage

1. Clone or download this repository.
2. Install the Arduino IDE.
3. Open the program.
4. Connect your hardware according to the pins at the top of the file.
5. Install required libraries via the Arduino Library Manager.
6. Upload the sketch to an Arduino Pro Mini.
7. Stick the arduino to a watch. Initially done by gluing magnets to the watch and arduino, to make it detachable.