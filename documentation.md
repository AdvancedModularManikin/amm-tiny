# AMM tiny remote API documentation

## Control flow

A message is received over the SPI protocol, possibly having been retransmitted.
The callback `ammtinycb` is called, passing control to `spi_chunk_overall`.
`spi_chunk_overall` calls `spi_msg_chunks` with the `chunk_dispatcher_slave` chunk callback.
`chunk_dispatcher_slave` reads the type field of each chunk and calls the appropriate handler.
This is where new types should be added.
The handlers are in `source/controllers/`.

## To add a chunk type

1. Add definitions to `spi_proto/src/spi_remote_api.h` and `spi_proto/src/spi_chunk_defines.h` for the chunk id and the chunk formats.
Because it is a submodule, this must be done separately.
2. Add files in `source/controllers/` to handle your chunks.
3. Add a case to the switch in `chunk_dispatcher_slave` in `spi_chunks_slave.cpp`.
4. Add functions to `spi_proto/src/spi_proto_master.cpp` and `spi_proto/src/spi_remote_host.c` and possibly their header files.
