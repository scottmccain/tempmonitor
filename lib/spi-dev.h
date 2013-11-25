int SpiOpenPort (int spi_device, int mode, int speed);
int SpiClosePort (int spi_device);
int SpiWriteAndRead (int spi_device, unsigned char *data, int length);
