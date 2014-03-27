CC = g++

TARGET = spider

COMPILEFLAGS = -lopenal -lalut -lglut -lGL -lGLU

$(TARGET): spider.o
	$(CC) -o $(TARGET) spider.o $(COMPILEFLAGS)

clean:
	rm -f $(TARGET) spider.o
