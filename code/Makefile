CFLAGS  =
LFLAGS  =
CC      = g++ -std=c++11
RM      = /bin/rm -rf

####### grpc, protobuf, borrowed from https://github.com/grpc/grpc/blob/master/examples/cpp/route_guide/Makefile
PROTOC = protoc
PROTOS_PATH = .
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

############# grpc, protobuf  #############
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<
########################################################
manager: message.pb.o message.grpc.pb.o manager.o
	$(CC) $^ $(LDFLAGS) -o $@

storage: storage.cpp
	$(CC) -Wall storage.cpp -o storage

client: message.pb.o message.grpc.pb.o client.o
	$(CC) $^ $(LDFLAGS) -o $@

test_app :  message.pb.o message.grpc.pb.o client.o test_app.o
	$(CC) $^ $(LDFLAGS) -o test_app

clean:
	$(RM) *.o $(TESTS)
