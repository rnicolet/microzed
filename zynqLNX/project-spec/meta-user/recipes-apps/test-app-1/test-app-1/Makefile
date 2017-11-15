APP = test-app-1

# Add any other object files to this list below
APP_OBJS = test-app-1.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)
