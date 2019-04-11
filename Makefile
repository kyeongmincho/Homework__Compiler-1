OBJS = lex.yy.o token.o
TARGET = in2post

.SUFFIXES : .c .o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)

lex.yy.o : lex.yy.c token.h
token.o : token.c token.h list.h