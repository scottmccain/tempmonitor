#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>


int spi_cs0_fd;				//file descriptor for the SPI device
int spi_cs1_fd;				//file descriptor for the SPI device
unsigned char spi_mode;
unsigned char spi_bitsPerWord;
unsigned int spi_speed;




//***********************************
//***********************************
//********** SPI OPEN PORT **********
//***********************************
//***********************************
//spi_device	0=CS0, 1=CS1
int SpiOpenPort (int spi_device, int mode, int speed)
{
	int status_value = -1;
    int *spi_cs_fd;



    //----- SET SPI MODE -----
    //SPI_MODE_0 (0,0) 	CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
    //SPI_MODE_1 (0,1) 	CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
    //SPI_MODE_2 (1,0) 	CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
    //SPI_MODE_3 (1,1) 	CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
	switch(mode) {
		case 0:
		spi_mode = SPI_MODE_0;
		break;

		case 1:
		spi_mode = SPI_MODE_1;
		break;

		case 2:
		spi_mode = SPI_MODE_2;
		break;

		case 3:
		spi_mode = SPI_MODE_3;
		break;

		default:
		spi_mode = SPI_MODE_0;
		break;
	}


    //----- SET BITS PER WORD -----
    spi_bitsPerWord = 8;

    //----- SET SPI BUS SPEED -----
    spi_speed = speed; //1000000;		//1000000 = 1MHz (1uS per bit) 


    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;


    if (spi_device)
    	*spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
    else
    	*spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);

    if (*spi_cs_fd < 0)
    	return -1;

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status_value < 0)
    	return -2;

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status_value < 0)
    	return -3;

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    	return -4;

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    	return -5;

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    	return -6;

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    	return -7;

    return(status_value);
}



//************************************
//************************************
//********** SPI CLOSE PORT **********
//************************************
//************************************
int SpiClosePort (int spi_device)
{
    int *spi_cs_fd;

    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;


    return close(*spi_cs_fd);
}



//*******************************************
//*******************************************
//********** SPI WRITE & READ DATA **********
//*******************************************
//*******************************************
//data		Bytes to write.  Contents is overwritten with bytes read.
int SpiWriteAndRead (int spi_device, unsigned char *data, int length)
{
	struct spi_ioc_transfer spi[length];
	int i = 0;
    int *spi_cs_fd;

    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;

	//one spi transfer for each byte
	for (i = 0 ; i < length ; i++)
	{
		spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
		spi[i].len           = sizeof(*(data + i)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = spi_speed ;
		spi[i].bits_per_word = spi_bitsPerWord ;
		spi[i].cs_change = 0;
	}

	return ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(length), &spi) ;
}


