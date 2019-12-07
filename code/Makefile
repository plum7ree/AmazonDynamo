CFLAGS  =
LFLAGS  =
CC      = g++ --std=c++11
RM      = /bin/rm -rf
CXXFLAGS = --std=c++11

####### grpc, protobuf, borrowed from https://github.com/grpc/grpc/blob/master/examples/cpp/route_guide/Makefile
PROTOC = protoc
PROTOS_PATH = ./service
HASH_PATH = ./hash
TYPE_PATH = ./type
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -pthread\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
#################################################



TESTS = test_app
SRC = test_app.cpp


all: $(TESTS)

manager: $(PROTOS_PATH)/message.pb.o $(PROTOS_PATH)/message.grpc.pb.o $(PROTOS_PATH)/managerService.o $(HASH_PATH)/consistent_hash.o $(TYPE_PATH)/helper.o manager.o
	$(CC) $^ $(LDFLAGS) -o $@

storage: $(PROTOS_PATH)/message.pb.o $(PROTOS_PATH)/message.grpc.pb.o $(PROTOS_PATH)/storageService.o $(TYPE_PATH)/helper.o storage.o
	$(CC) $^ $(LDFLAGS) -o $@

client: $(PROTOS_PATH)/message.pb.o $(PROTOS_PATH)/message.grpc.pb.o $(PROTOS_PATH)/clientService.o $(TYPE_PATH)/helper.o client.o
	$(CC) $^ $(LDFLAGS) -o client

test_app :  $(PROTOS_PATH)/message.pb.o $(PROTOS_PATH)/message.grpc.pb.o $(PROTOS_PATH)/managerService.o $(PROTOS_PATH)/storageService.o $(HASH_PATH)/consistent_hash.o $(TYPE_PATH)/helper.o $(PROTOS_PATH)/clientService.o client.o test_app.o
	$(CC) $^ $(LDFLAGS) -o test_app

con_hash: hash/consistent_hash.cpp
	$(CC) -Wall hash/consistent_hash.cpp -o conhash

############# grpc, protobuf  #############
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(PROTOS_PATH) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=$(PROTOS_PATH) $<
########################################################

clean:
	$(RM) *.o $(TESTS) conhash service/*.o hash/*.o type/*.o manager storage client
