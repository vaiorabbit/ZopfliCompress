SRCS = \
    Main.cpp \
    Option.cpp \
    Convert.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = ZopfliCompress

.cpp.o:
	clang++ -c -O2 -W -Wall -Wextra -std=c++11 -stdlib=libc++ -pedantic -I ../external/zopfli -o $@ $<
#	g++ -c -O2 -W -Wall -Wextra -ansi -pedantic -I ../external/zopfli -o $@ $<

$(TARGET): $(OBJS) ../external/zopfli/libzopfli.a
	clang++ -L ../external/zopfli -lzopfli -stdlib=libc++ -o $@ $^
#	g++ -L ../external/zopfli -lzopfli -o $@ $^
#	cp $(TARGET) ../out

clean:
	rm $(OBJS) $(TARGET)
