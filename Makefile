all:
	$(MAKE) -f Makefile.1xx SCEkxploit
	$(MAKE) -f Makefile.3xx

1xx:
	$(MAKE) -f Makefile.1xx SCEkxploit

3xx:
	$(MAKE) -f Makefile.3xx

clean:
	$(MAKE) -f Makefile.1xx clean
	$(MAKE) -f Makefile.3xx clean
