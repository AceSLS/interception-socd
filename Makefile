CXX := clang++
CPPFLAGS := -O3 -march=native -flto=full

INSTALLDIR := /opt/interception

build:
	$(CXX) $(CPPFLAGS) main.cpp -o interception-socd

clean:
	rm -v interception-socd

install: build
	sudo mkdir -p "$(INSTALLDIR)"
	sudo cp -v interception-socd "$(INSTALLDIR)"

.PHONY: build clean
