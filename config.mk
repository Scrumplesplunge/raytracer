CXXFLAGS += -std=c++1z -Wall -Wextra -pedantic
opt: CXXFLAGS += -ffunction-sections -fdata-sections -flto -O3
debug: CXXFLAGS += -O0 -g

LDFLAGS += -fuse-ld=gold
opt: LDFLAGS += -s -Wl,--gc-sections -flto -O3

LDLIBS = -lpthread
