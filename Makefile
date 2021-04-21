#* ************************************************************************** *#
#*                                                                            *#
#*                                    :::::::         :::          :::::::    *#
#*   Makefile                       :+:    :+:       :+:         :+:    :+:   *#
#*                                 +:+      +:+     +:+         +:+           *#
#*                                +#+       +#+  +#+#+#+       +#+            *#
#*                               +#+       +#+    +#+         +#+   #+#+#+    *#
#*   dtg_lib                    #+#      #+#     #+#    #+    #+#     +#+     *#
#*                             ##########         ######       ########       *#
#*                                                                            *#
#* ************************************************************************** *#
#*
all: compile
#*
#* ************************************************************************** *#
#*                             Executable name                                *#
#* ************************************************************************** *#
#*
TARGET_NAME		=dtg_lib
DEBUG_TARGET_NAME	=deb_dtg_lib
#*
#* ************************************************************************** *#
#*                             Directories                                    *#
#* ************************************************************************** *#
#*
OBJS_DIR	=Objects
SRCS_DIR	=Sources
TARGET_DIR	=bin
INCLUDE_DIR	=Includes
DEBUG_DIR	=bin
#*
#* ************************************************************************** *#
#*                             Compilation variables                          *#
#* ************************************************************************** *#
#*
SRCS_EXT	=.cpp
OBJS_EXT	=.o
HEADER_EXT	=.hpp
CC		=g++-10 -std=c++14 -o2
CFLAGS		=-Wextra -Wall -Werror -Wno-parentheses #-Wno-error=parentheses
DEBUG_FLAGS	=-g3
DEBUG_PROGRAM	=gdb
#*
#* ************************************************************************** *#
#*                              Additional commands                           *#
#* ************************************************************************** *#
#*

#*
#* ************************************************************************** *#
#*                             Do not edit below                              *#
#* ************************************************************************** *#
#*

SRCS = $(shell find $(SRCS_DIR) -type f -name '*$(SRCS_EXT)')
OBJS = $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(SRCS:$(SRCS_EXT)=$(OBJS_EXT)))
DIRS = $(SRCS_DIR) $(TARGET_DIR) $(OBJS_DIR)

INCLUDES	= $(addprefix -I,$(sort $(dir $(shell find $(SRCS_DIR) -type f -name *$(HEADER_EXT)))))\
			-I$(INCLUDE_DIR)

TARGET		= $(TARGET_DIR)/$(TARGET_NAME)

DEBUG_TARGET	= $(DEBUG_DIR)/$(DEBUG_TARGET_NAME)

DIRS = $(OBJS_DIR) $(SRCS_DIR) $(COMPILE_DIR) #$(INCLUDE_DIR)


debug: $(DEBUG_TARGET)

de: debug

drun: debug
	$(DEBUG_PROGRAM) $(DEBUG_DIR)/$(DEBUG_TARGET_NAME)

compile: ${OBJS} ${TARGET}

$(TARGET): $(TARGET_DIR) ${OBJS}
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET)

$(DEBUG_TARGET): $(DEBUG_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) $(SRCS) -o $(DEBUG_TARGET)
	
$(OBJS_DIR)/%$(OBJS_EXT): $(SRCS_DIR)/%$(SRCS_EXT)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET_DIR):
	@mkdir -p $(TARGET_DIR)
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
$(SRCS_DIR):
	@mkdir -p $(SRCS_DIR)
$(DEBUG_DIR):
	@mkdir -p $(DEBUG_DIR)

dirs:
	@mkdir -p $(DIRS)


clean:
	rm -rf ${TARGET_DIR}

clear_console:
	clear
	clear

cl: clear_console

clall: clear_console all

fclean: clean
	rm -rf ${COMPILE_DIR}
re: fclean all

remake: re

run: ${TARGET}
	./$(TARGET)
rerun: re run
test:
	@echo '	SRCS: $(SRCS)'
	@echo '	OBJS: $(OBJS)'
	@echo '	INCLUDES: $(INCLUDES)'

.PHONY: all cl clall clean clear_console compile de debug dirs drun fclean re remake rerun run test $(ADDPHONY)
