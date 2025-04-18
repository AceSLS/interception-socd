CXX := clang++
CPPFLAGS := -O3 -march=native -flto=full

INSTALLDIR := /opt/interception

interception-socd:
	$(CXX) $(CPPFLAGS) main.cpp -o interception-socd

install: interception-socd
	sudo mkdir -p "$(INSTALLDIR)"
	sudo cp -v interception-socd "$(INSTALLDIR)"
