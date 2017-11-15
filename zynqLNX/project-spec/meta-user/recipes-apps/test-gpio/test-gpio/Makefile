APP = test-gpio

# Add any other object files to this list below
# Also add new source code cpp and header file to test-gpio.bb 
APP_OBJS := get-options.o
APP_OBJS += test-gpio.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

clean:
	-rm -f $(APP) *.elf *.gdb *.o	
