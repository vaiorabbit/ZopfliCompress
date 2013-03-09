SRCS = \
    $(CURDIR)/zopfli/blocksplitter.c \
    $(CURDIR)/zopfli/cache.c \
    $(CURDIR)/zopfli/deflate.c \
    $(CURDIR)/zopfli/gzip_container.c \
    $(CURDIR)/zopfli/hash.c \
    $(CURDIR)/zopfli/katajainen.c \
    $(CURDIR)/zopfli/lz77.c \
    $(CURDIR)/zopfli/squeeze.c \
    $(CURDIR)/zopfli/tree.c \
    $(CURDIR)/zopfli/util.c \
    $(CURDIR)/zopfli/zlib_container.c \
    $(CURDIR)/zopfli/zopfli_lib.c

OBJS = $(SRCS:.c=.o)

TARGET = $(CURDIR)/zopfli/libzopfli.a

.c.o:
	clang++ -c -O2 -W -Wall -Wextra -std=c++11 -stdlib=libc++ -pedantic -o $@ $<
#	gcc -c -O2 -W -Wall -Wextra -ansi -pedantic -o $@ $<

$(TARGET): $(OBJS)
	ar rsv $@ $^

clean:
	rm $(OBJS) $(TARGET)
