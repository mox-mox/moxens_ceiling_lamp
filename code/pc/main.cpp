#include <iostream>
#include <SerialStream.h>
#include <math.h>



void print_help(std::string progname)
{
	std::cout<<"Usage: "<<progname<<" /dev/yourtty <command> <data...>"<<std::endl;
}





int main(int argc, char** argv) //args: /dev/tty* command 
{
	if(argc<1)
	{
		std::cerr<<"Error: No serial port was specified"<<std::endl;
		print_help(argv[0]);
	}


	LibSerial::SerialStream my_serial_port(argv[1]);
// Open the serial port for communication.
	my_serial_port.Open(const BaudRate      baudRate    = BAUD_DEFAULT,
		     const CharacterSize charSize    = CHAR_SIZE_DEFAULT,
		     const Parity        parityType  = PARITY_DEFAULT,
		     const StopBits      stopBits    = STOP_BITS_DEFAULT,
		     const FlowControl   flowControl = FLOW_CONTROL_DEFAULT)
		throw(AlreadyOpen,
		      OpenFailed,
		      UnsupportedBaudRate,
		      std::invalid_argument);







// Use 8 bit wide characters.
	my_serial_port.SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);

//5.4. Setting the Number of Stop Bits


// Use one stop bit.
	my_serial_port.SetNumOfStopBits(1);

//5.5. Setting the Parity Type


// Use odd parity during serial communication.
	my_serial_port.SetParity(LibSerial::SerialStreamBuf::PARITY_ODD);

//5.6. Setting the Flow-Control Type


// Use hardware flow-control.
	my_serial_port.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_HARD);

//5.7. Reading Characters

	//Characters can be read from the serial port using standard iostream ">>" operator.For example :


// Read one character from the serial port.
	char next_char;
	my_serial_port >> next_char;
// You can also read other types of values from the serial port in a similar fashion.
	int data_size;
	my_serial_port >> data_size;

	//All other methods of standard C++ iostream objects are available too.For example, one can read characters from the serial port using the get() method :


// Read one byte from the serial port.
	char next_byte;
	my_serial_port.get(next_byte);

//5.8. Writing Characters


// Write a single character to the serial port.
	my_serial_port << 'U';
// You can write a whole string.
	my_serial_port << "Hello, Serial Port." << std::endl;
// In fact, you can pretty much write any type of object that
// is supported by a "<<" operator.
	//double radius = 2.0;
	double area = M_PI * 2.0 * 2.0;
	my_serial_port << area << std::endl;

//5.9. Reading Blocks of Data


// Read a whole array of data from the serial port.
	const int BUFFER_SIZE = 256;
	char input_buffer[BUFFER_SIZE];
//
	my_serial_port.read(input_buffer,
	                      BUFFER_SIZE);

//5.10. Writing Blocks of Data


// Write an array of data from the serial port.
	//const int BUFFER_SIZE = 256;
	char output_buffer[BUFFER_SIZE];
	for(int i=0; i < BUFFER_SIZE; ++i)
	{
		output_buffer[i] = i;
	}
	my_serial_port.write(output_buffer,
	                       BUFFER_SIZE);

//5.11. Closing the Serial Port


	my_serial_port.Close();
}
