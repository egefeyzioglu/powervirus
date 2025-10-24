all: powervirus

powervirus: powervirus.c
	$(CC) $(CFLAGS) powervirus.c -o powervirus

clean:
	rm -f powervirus

.PHONY: clean
