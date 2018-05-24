
struct waiting_chunk {
	uint8_t buf[SPI_MSG_PAYLOAD_LEN];
	//buf[0] is len
	char ready_to_pack; // and therefore not empty. char because bool isn't in C
};

unsigned int unknown_chunk_type_msg_count;
int
chunk_dispatcher(uint8_t *buf, size_t len);
int
spi_msg_chunks(uint8_t *buf, size_t len, int (*chunk_handler)(uint8_t *b, size_t len));
int
send_chunk(uint8_t *buf, size_t len);
int
chunk_packer(struct waiting_chunk *chunks, size_t numchunk,
	uint8_t *buf, size_t len);
