#!/bin/sh

MYSQL_LFLAGS=$(mysql_config --libs)
export MYSQL_LFLAGS

MYSQL_CFLAGS=$(mysql_config --cflags)
export MYSQL_CFLAGS
