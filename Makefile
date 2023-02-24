TFD=dependencies/tinyfiledialogs.o

imgui: 

all: $(TFD) imgui


.PHONY: clean
clean:
	rm -rf * .o
