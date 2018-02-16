##
## EPITECH PROJECT, 2018
## PSU_2017_nmobjdump
## File description:
## Makefile
##

#
# OUTPUT
#

NAME_NM		=	my_nm
NAME_OBJDUMP	=	my_objdump

#
# SOURCES
#

MAINSRC_NM	=	my_nm.c
MAINSRC_OBJDUMP	=	my_objdump.c

SRC		=	elf_file.c					\
			elf_section.c					\
			elf_strtab.c					\
			elf_symtab.c

#
# HEADERS
#

INC		=	nmobjdump.h

#
# SYSTEM AND USER LIBS
#

LIB		=
USERLIB		=

#
# PATHS
#

SRCDIR		=	src
INCDIR		=	include
OBJDIR		=	obj
SUBDIRS		=
USERLIBDIR	=

#
# UNIT TESTS
#

TESTNAME	=

TESTSRC		=
TESTLIB		=

TESTSRCDIR 	=
TESTOBJDIR	=

#
# COMPILATION AND TOOLS
#

CC		=	gcc

CFLAGS		=	-c -W -Wall -Wextra -I$(INCDIR)
LDFLAGS		=	$(addprefix -L$(USERLIBDIR)/lib, $(USERLIB)) $(addprefix -l, $(LIB) $(USERLIB))

TESTCFLAGS	=	-c -W -Wall -Wextra -ftest-coverage -fprofile-arcs -I$(INCDIR)
TESTLDFLAGS	=	--coverage $(addprefix -L$(USERLIBDIR)/lib, $(USERLIB)) $(addprefix -l, $(LIB) $(USERLIB) $(TESTLIB))

MAKE		=	make
MKDIR		=	mkdir
RM		=	rm -rf

#
# OBJECTS
#

OBJ_NM		=	$(MAINSRC_NM:.c=.o)
OBJ_OBJDUMP	=	$(MAINSRC_OBJDUMP:.c=.o)
OBJ		=	$(SRC:.c=.o)
TESTOBJ		=	$(TESTSRC:.c=.o)

#
# TARGETS
#

all: $(NAME_NM) $(NAME_OBJDUMP) $(TESTNAME)

$(NAME_NM): $(addprefix $(OBJDIR)/, $(OBJ_NM) $(OBJ))
	$(CC) $^ $(LDFLAGS) -o $@

$(NAME_OBJDUMP): $(addprefix $(OBJDIR)/, $(OBJ_OBJDUMP) $(OBJ))
	$(CC) $^ $(LDFLAGS) -o $@

$(TESTNAME): $(addprefix $(TESTOBJDIR)/, $(OBJ) $(TESTOBJ))
	$(CC) $^ $(TESTLDFLAGS) -o $@

$(addprefix $(OBJDIR)/, $(OBJ_NM) $(OBJ_OBJDUMP) $(OBJ)): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(addprefix $(INCDIR)/, $(INC)) | $(OBJDIR) $(addprefix $(OBJDIR)/,$(SUBDIRS))
	$(CC) $< $(CFLAGS) -o $@

$(addprefix $(TESTOBJDIR)/, $(OBJ_NM) $(OBJ_OBJDUMP) $(OBJ)): $(TESTOBJDIR)/%.o: $(SRCDIR)/%.c $(addprefix $(INCDIR)/, $(INC)) | $(TESTOBJDIR) $(addprefix $(TESTOBJDIR)/,$(SUBDIRS))
	$(CC) $< $(TESTCFLAGS) -o $@

$(addprefix $(TESTOBJDIR)/, $(TESTOBJ)): $(TESTOBJDIR)/%.o: $(TESTSRCDIR)/%.c | $(TESTOBJDIR)
	$(CC) $< $(TESTCFLAGS) -o $@

$(OBJDIR) $(addprefix $(OBJDIR)/,$(SUBDIRS)) $(TESTOBJDIR) $(addprefix $(TESTOBJDIR)/,$(SUBDIRS)):
	$(MKDIR) $@

tests_run: $(TESTNAME)
	./$<

clean:
	$(RM) $(OBJDIR) $(TESTOBJDIR)

fclean: clean
	$(RM) $(NAME_NM) $(NAME_OBJDUMP) $(TESTNAME)

re: fclean all

.PHONY:	all tests_run clean fclean re
