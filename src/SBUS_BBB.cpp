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
#include "blacklib/BlackTime.h"
//#include "blacklib/BlackCore.h"

int main() {

	//start a timer to measure runtime of various functions.
	 BlackLib::BlackTime mytime;   //Timer from the BlackLib library
	 BlackLib::BlackTimeElapsed et; //Counts the elapsed time from the timer
	 mytime.start();

	cout << "!!!Hello World!!2?!" << endl; // prints !!!Hello World!!?!

	//create 2 UART serial connections. Wire them together
	//According to the pinout (http://beagleboard.org/support/bone101), UART1_TXD = P9_24, UART1_RXD = P9_26. UART2_TXD = P9_21, UART2_RXD = P9_22
	//  EDIT: UART2 is disabled because I am using it for SPI (required funny configuration).
	//	Instead of UART2, use UART4. UART4_RXD = P9_11, UART4_TXD = P9_13
	//Connect the RX to TX. Wiring goes like this:
	//				(UART4_TXD)	P9_13 -> P9_26 (UART1_RXD)
	//				(UART4_RXD)	P9_11 -> P9_24 (UART1_TXD)
	BlackLib::BlackUART  myUart(BlackLib::UART1,
	                                       BlackLib::Baud9600,
	                                       BlackLib::ParityEven,
	                                       BlackLib::StopOne,
	                                       BlackLib::Char8 );
	BlackLib::BlackUART myUart2(BlackLib::UART4,
											BlackLib::Baud9600,
											BlackLib::ParityEven,
											BlackLib::StopOne,
											BlackLib::Char8 );
	//opend the UART connections
	myUart.open( BlackLib::ReadWrite | BlackLib::NonBlock);
	myUart2.open( BlackLib::ReadWrite | BlackLib::NonBlock);

	cout << "UART2 status: " <<  myUart2.isOpen() << endl;

	//write a test message
	string testMessage = "Test UART message";
	//flush the send and receive buffers
	myUart2.flush( BlackLib::bothDirection );
	myUart.flush( BlackLib::bothDirection );
	//write the message to UART1
	cout << myUart.write(testMessage);
	usleep(5000); //wait 50 ms. You have to wait for the message to be sent. at 9600 baud, sends about 1 char per ms.


	cout << "Message sent: " << testMessage << endl;
	//cout << "message received: " <<  myUart2.read() << endl;
	//read from the UART buffer. For implementation, it is best to put this in a separate thread because of the long waits between data.
	char readBuffer[testMessage.length()];
	char buf[1]; //single buffer
	int i = 0;
	while(i < testMessage.length()){
		//myUart2.read(char* buf, size) takes 8 microseconds if no message, 10 microseconds if message.
		if(myUart2.read(buf, sizeof(buf))){
			readBuffer[i] = buf[0];
			i ++;
		}else{
			usleep(500); //wait 0.5 ms if nothing is available to read
		}
	}
	cout << "Message received: " <<  readBuffer << endl;


	//close the UART connecctions
	myUart.close();
	myUart2.close();

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
