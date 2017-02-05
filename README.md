# Makesmith-joystick
Control a MakesmithCNC machine with a simple analog joystick.

The MakesmithCNC machine was a Kickstarter project in 2014 to make an extremely
low-cost desktop CNC router, using a laser-cut plywood structure and a closed-loop
servo-driven motion system controlled by an Arduino Mega 2560 equipped with a custom
driver board. After building the kit, I concluded that it wasn't going to be practical
as a CNC router, and set it aside. In early 2017, it occurred to me that the machine
would be usable as a platform for an inexpensive USB microscope. For that application,
none of the CNC control features were needed, just a simple interactive joystick to
allow the operator to position the microscope with respect to the work.

## Getting Started

Connect a two-axis analog joystick with an integrated pushbutton (I used Adafruit
product 512) to +5V, GND, A13 (X), A14 (Y), and A15 (button) on the MakesmithCNC's
Arduino board, as shown here.

![Wiring Photo](/images/wiring.png)

Program the Arduino with this sketch. Now the machine is under joystick control.
Press the button on the joystick to enable X/Y control of the machine. Press the button
again to enable Z axis (focus) control.

If you're using a USB microscope with the machine, mount it where the spindle motor
would go, and connect it via USB according to the microscope's instructions.

### Prerequisites

You'll need:
* a MakesmithCNC desktop CNC router, assembled and working
* a computer with the Arduino development environment on it
* a 2-axis analog joystick with a button, like Adafruit #512
* some wire

## Author

* **Paul Williamson** - *Initial work* - [MustBeArt](https://github.com/MustBeArt)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Bar Smith and Tom Beckett created the MakesmithCNC
