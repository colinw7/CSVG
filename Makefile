all:
	cd src; make
	cd test; make
	cd qtest; make

clean:
	cd src; make clean
	cd test; make clean
	cd qtest; make clean
