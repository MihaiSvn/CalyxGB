# Hardware Wiring Guide

This directory contains the physical pin configurations, C drivers, and Fritzing schematics (`schematics/`) for the handheld console. 

The hardware architecture is designed specifically for the **Raspberry Pi 3 Model A+**, utilizing its 40-pin GPIO header.

**Components**

* **4-Pin Tactile Buttons (Input)**
  * **Breadboard Placement:** Always mount the buttons across the center dividing trench of the breadboard to prevent internal rails from shorting the pins.
  * **Pin Connection (Diagonal Rule):** Always wire the **diagonally opposite legs**. Connect one leg to the assigned GPIO pin, and the diagonally opposite leg directly to the Ground (GND) rail.
  * **Circuit Logic:** Active-low logic. Do not connect to 3.3V or 5V power pins.

**Wire Color Conventions**
* **Grey:** Grounding wires (GND)
* **Red:** Power Button Signal (GPIO 3 wake-up signal only, NOT a voltage supply)

**GPIO Pin Mapping (BCM Numbering)**

**System Controls**
* Power Button: GPIO 3

**D-Pad**
* Up Arrow: GPIO 20
* Down Arrow: GPIO 16
* Left Arrow: GPIO 21
* Right Arrow: GPIO 12

**Action & Menu Buttons**
* Button B (Left): GPIO 13
* Button A (Right): GPIO 6

* Select Button (Left): GPIO 26
* Start Button (Right): GPIO 19

*Note: The hardware pinout documented here must always stay synchronized with the macro definitions in `include/console_config.h`.*


## Visual Reference

<!-- TO DO LA FINAL SA ADAUG BREADBOARD -->
<!-- ![Fritzing Breadboard Schematic](schematics/exports/breadboard_schematic.png)
*Figure 1: Complete breadboard wiring schematic.* -->

<img src="schematics/exports/pinout_reference.png" alt="Raspberry Pi 3 A+ Pinout" width="250">
<br>
<em>Figure 1: Raspberry Pi 3 Model A+ BCM Pinout Reference.</em>