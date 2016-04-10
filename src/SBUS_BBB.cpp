//============================================================================
// Name        : SBUS_BBB.cpp
// Author      : Mark VanderMeulen
// Version     :
// Copyright   : You can use this code. I am not responsible for anything it does! Use responsibly!
// Description : Test a connection with the X8R SBUS. This first needs a serial inverter.
// 				 Second, SBUS operates on 3.3V
//				 Third, SBUS uses 100K baud (might be 98K) inverted UART with 25 byte transmission. Each channel is 11 bits. Also start and end byte.
//						Also 2 stop its and even parity. Transmitted big endian, but each byte is little endian.
// useful source: https://developer.mbed.org/users/Digixx/notebook/futaba-s-bus-controlled-by-mbed/
//	Testing:
//		1. Enable two UART ports
//		2. Connect the two UARTs together, and test sending stuff between them
//============================================================================

#include <iostream>
using namespace std;

#include "blacklib/BlackLib.h"
#include "blacklib/BlackUART.h"

int main() {

	cout << "!!!Hello World!!2?!" << endl; // prints !!!Hello World!!?!

	BlackLib::BlackUART  myUart(BlackLib::UART1,
	                                       BlackLib::Baud9600,
	                                       BlackLib::ParityEven,
	                                       BlackLib::StopOne,
	                                       BlackLib::Char8 );
	BlackLib::BlackUART myUart2(BlackLib::UART2,
											BlackLib::Baud9600,
											BlackLib::ParityEven,
											BlackLib::StopOne,
											BlackLib::Char8 );

	cout << "LED Flash Start" << endl;

	BlackLib::BlackGPIO led1(BlackLib::GPIO_44, BlackLib::output);
	for(int i=0; i<2; i++){
		led1.setValue(BlackLib::high);
		usleep(500000);
		led1.setValue(BlackLib::low);
		usleep(1000000);
	}
	cout << "LED Flash End" << endl;
	return 0;
}
