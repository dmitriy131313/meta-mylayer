#include "UART_drv.h"

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 

UART::UART(const std::string& filename)
{
	_fd = open(filename.c_str(), O_RDWR|O_NOCTTY|O_NDELAY);
	if(_fd < 0)
	{
		perror("Can't open the serial port\n");
		//TODO Throw exeption
	}
	if(fcntl(_fd, F_SETFL, 0)<0)
	{
		printf("Fcntl failed\n");
		//TODO Throw exeption 
	}
	else
	{
		printf("Fcntl=%d\n", fcntl(_fd,F_SETFL,0));
	}
	if(isatty(STDIN_FILENO)==0)
	{
		printf("Standard input is not a terminal device\n");
		//TODO Throw exeption 
	}
	else
	{
		printf("Isatty success\n");
	}
	printf("_fd->open=%d\n", _fd);
}

UART::~UART()
{
	int rc = close(_fd);
	if (rc < 0) 
	{
		printf("%s: failed to close UART device\r\n", __func__);
		//TODO Throw exeption 
	}
	else
	{
		printf("%s: UART close OK\r\n", __func__);
	}
}

int UART::setAttribs(SPEED speed, FLOW_CTRL flow_ctrl, DATA_BITS databits, STOP_BITS stopbits, PARITY parity)
{
	// int i;
	// int status;
	struct termios options;
	if(tcgetattr(_fd, &options)!=0)
	{
		printf("Setup UART\n");
		return -1;
	}

	// cfsetispeed(&options, static_cast<speed_t>(speed));
	// cfsetospeed(&options, static_cast<speed_t>(speed));
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);


	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	// options.c_iflag &= ~IGNBRK;         // disable break processing
	options.c_iflag &= ~(IXON | IXOFF | IXANY);

	options.c_cflag |= CLOCAL;
	options.c_cflag |= CREAD;

	switch(flow_ctrl)
	{
		case FLOW_CTRL::NO:
			options.c_cflag &= ~CRTSCTS;
			break;
		case FLOW_CTRL::HW:
			options.c_cflag |= CRTSCTS;
			break;
		case FLOW_CTRL::SW:
			options.c_cflag |= IXON | IXOFF | IXANY;
			break;
	}

	options.c_cflag &= ~CSIZE; // Mask other flag bits
	// options.c_cflag |= static_cast<speed_t>(databits);
	options.c_cflag |= static_cast<unsigned int>(databits);

	switch(parity)
	{  
		case PARITY::NO:
			options.c_cflag &= ~PARENB;
			options.c_iflag &= ~INPCK;    
			break;
		case PARITY::ODD:
			options.c_cflag |= (PARODD | PARENB);
			options.c_iflag |= INPCK;             
			break;
		case PARITY::EVEN:
			options.c_cflag |= PARENB;       
			options.c_cflag &= ~PARODD;       
			options.c_iflag |= INPCK;       
			break;
		case PARITY::SPACE:
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:  
			printf("Unsupported parity\n");   
			return -1;
	}

	//Set stop bit
	switch (stopbits)
	{  
		case STOP_BITS::_1:
			options.c_cflag &= ~CSTOPB;
			break;
		case STOP_BITS::_2:
			options.c_cflag |= CSTOPB;
			break;
		default:   
			printf("Unsupported stop bits\n");
		return -1;
	}

    //Modify output mode, RAW data mode
	options.c_oflag &= ~OPOST;

	//set the minimum waiting time and minimum receiving bytes before unblocking
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 255;

	tcflush(_fd,TCIFLUSH);
	//active the configuration
	if(tcsetattr(_fd, TCSANOW, &options) != 0)
	{
		printf("com set error!\n");  
		return -1;
	}

	printf("Attribs set OK\n");
	return 0;
}

int UART::read(char* data, unsigned int len)
{
	// int num_bytes = ::read(_fd, data, len);
	// printf("readed bytes: %d\r\n",num_bytes);
	// return num_bytes;

    int nb_read_bytes = 0;
    while(nb_read_bytes < (int)len)
    {
        int num_bytes = ::read(_fd, data + nb_read_bytes, len - nb_read_bytes);
		if (num_bytes < 0)
		{
			printf("%s: failed to read uart data\r\n", __func__);
			return num_bytes;
		}
        nb_read_bytes += num_bytes;
		//printf("readed bytes: %d;   rest len: %d\r\n",num_bytes, len - nb_read_bytes);
    }
    return nb_read_bytes;
}

int UART::write(char* data, unsigned int len)
{
	int wr_len = ::write(_fd, data, len);
	if(wr_len == (int)len)
	{
		return wr_len;
	}     
	else
	{
		tcflush(_fd, TCOFLUSH);
		return -1;
	}
}