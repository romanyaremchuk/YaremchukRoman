#************************************#
# Makefile                           #
# Solution IOS-DU2 : Child Care      #
# Author: Yaremchuk Roman (xyarem00) #
# Date: 28.04.2017                   #
#************************************#

TARGET = proj2
CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
SRCS = proj2.c shmsem.c mylib.c adultgen.c childgen.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)
$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) -pthread -lrt

.c.o:
		$(CC) $(CFLAGS)  -c $< -o $@

clean:
		rm -rf $(TARGET) $(OBJS) $(TARGET).out
