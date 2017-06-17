CXXFLAGS += -std=c++1z -Wall -Wextra -pedantic
opt: CXXFLAGS += -ffunction-sections -fdata-sections -flto -Ofast -march=native
prof: CXXFLAGS += -g -Ofast -march=native
debug: CXXFLAGS += -O0 -g

LDFLAGS += -fuse-ld=gold
opt: LDFLAGS += -s -Wl,--gc-sections -flto -Ofast

LDLIBS = -lpthread
