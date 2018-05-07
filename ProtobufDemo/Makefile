#
# build protobuf
#
PROTOC = /usr/local/bin/protoc
PROTOC_OPTS = -I./protos
OUT_DIR = ./source
PROTOB_DIR = ./protos
SRCS = ${wildcard $(PROTOB_DIR)/*.proto}
OBJS = ${patsubst %.proto, $(OUT_DIR)/%.pb.cc, $(SRCS)}
OBJS += ${patsubst %.proto, $(OUT_DIR)/%.pb.h, $(SRCS)}


all:$(OBJS)
	@echo "#"
	@echo "# make OK."
	@echo "#"


# Build rule for the protocol
$(OUT_DIR)/%.pb.cc $(OUT_DIR)/%.pb.h: %.proto
	$(PROTOC) $(PROTOC_OPTS) --cpp_out=$(OUT_DIR) $^

clean:
	@rm -rf ./source/*.pb.cc ./source/*.pb.h

