# AUTOSAR-Full-Layered-Project

This is a Fully layered AUTOSAR embedded system project which is designed to work on TM4C123GH6PM microcontroller. The project is written in C language and uses CCS. The project is divided into several modules, each module is responsible for a specific task. The modules are:

* **MCU**: This module is responsible for the initialization of the microcontroller and its peripherals. The module is responsible for the following tasks:
	+ Initializing the microcontroller clock.
	+ Initializing the microcontroller peripherals.
	+ Initializing the microcontroller pins.
* **PORT**: This module is responsible for the digital input/output operations. The module is responsible for the following tasks:
	+ Initializing the port pins.
	+ Writing data to the port pins.
	+ Reading data from the port pins.
* **DIO**: This module is responsible for the digital input/output operations. The module is responsible for the following tasks:
	+ Initializing the digital pins.
	+ Writing data to the digital pins.
	+ Reading data from the digital pins.
* **GPT**: This module is responsible for the timer operations. The module is responsible for the following tasks:
	+ Initializing the timer.
	+ Starting the timer.
	+ Stopping the timer.
	+ Getting the timer value.
* **BUTTON**: This module is responsible for the button operations. The module is responsible for the following tasks:
	+ Initializing the button pin.
	+ Reading the button state.
* **LED**: This module is responsible for the LED operations. The module is responsible for the following tasks:
	+ Initializing the LED pin.
	+ Writing the LED state.
* **APP**: This module is responsible for the application logic. The module is responsible for the following tasks:
	+ Initializing the application.
	+ Executing the application logic.

The project uses a layered architecture, each layer is responsible for a specific task. The layers are:

* **MCAL**: This layer is responsible for the microcontroller abstraction layer. The layer is responsible for the following tasks:
	+ Initializing the microcontroller.
	+ Initializing the microcontroller peripherals.
* **ECUAL**: This layer is responsible for the electrical control unit abstraction layer. The layer is responsible for the following tasks:
	+ Initializing the electrical control unit.
	+ Initializing the electrical control unit peripherals.
* **OS**: This layer is responsible for the operating system. The layer is responsible for the following tasks:
	+ Initializing the operating system.
	+ Managing the tasks.
	+ Managing the resources.
* **APP**: This layer is responsible for the application logic. The layer is responsible for the following tasks:
	+ Initializing the application.
	+ Executing the application logic.

The project is designed to be highly configurable, the user can configure the project by changing the values of the macros in the configuration files. The project is also designed to be highly portable, the user can easily port the project to any other microcontroller.

The project is tested on TM4C123GH6PM microcontroller and it is working correctly.
