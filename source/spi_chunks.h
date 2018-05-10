
struct waiting_chunk {
	uint8_t buf[SPI_MSG_PAYLOAD_LEN];
	//buf[0] is len
	bool ready_to_pack; // and therefore not empty
};

unsigned int unknown_chunk_type_msg_count;
int
chunk_dispatcher(uint8_t *buf, size_t len);
int
spi_msg_chunks(uint8_t *buf, size_t len, int (*chunk_handler)(uint8_t *b, size_t len));
int
spi_chunk_overall(uint8_t *buf, size_t len);
