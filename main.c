#include <stdio.h>
#include <syslog.h>
#include <stdarg.h>
#include <mysql.h>

#include "spi-dev.h"

MYSQL *conn;

/*
 * log_level: LOG_INFO, LOG_WARN, LOG_ERROR, LOG_NOTICE, LOG_DEBUG
 */
void log_message(int log_level, const char *szformat, ...) {

	char buffer[1024];
	va_list args;
	va_start (args, szformat);
	vsnprintf (buffer, 1023, szformat, args);

	//setlogmask (LOG_UPTO (LOG_NOTICE));

	openlog ("temperaturemonitor", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

	syslog (log_level, buffer);

	va_end (args);
	closelog ();
}

int open_db() {
	const char *server = "localhost";
	const char *user = "dbouser";
	const char *password = "81988Dbd!"; /* set me first */
	const char *database = "servicedb";

	conn = mysql_init(NULL);

	/* Connect to database */
	if (!mysql_real_connect(conn, server,
		user, password, database, 0, NULL, 0)) {

		log_message(LOG_CRIT,"%s", mysql_error(conn));
		return -1;
	}

	return 0;
}

void close_db() {
	mysql_close(conn);
}

double read_temperature() {

	double scale = 3.3/4096;
	double voltage, celsius;
	unsigned char data[2] =  {0x00, 0x00};
	int value;

	SpiOpenPort(0,0,1000000);
	SpiWriteAndRead(0, data, 2);
	SpiWriteAndRead(0, data, 2);
	SpiClosePort(0);

	value = (((int)data[0] << 8) + (int)data[1]) & 0x0FFF;
	voltage = value * scale;
	celsius = (voltage * 100) - 50;

	return celsius;
}

void insert_reading(double temperature) {
	char sql[1024];

	snprintf(sql, 1023, "INSERT INTO temperatures VALUES(NULL,%f,NOW())", temperature);
	if (mysql_query(conn, sql)) {
		log_message(LOG_ERR, "%s", mysql_error(conn));
	}
}

int main(int argc, char* argv[]) {

	double celsius = read_temperature();
	double farenheit = ((celsius * 9)/5) + 32;

	if(!open_db()) {
		insert_reading(celsius);
		close_db();
	}

	log_message(LOG_INFO, "temperature read: %.2f\260F, %.2f\260C", farenheit, celsius);
}
