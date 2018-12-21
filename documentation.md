# AMM tiny remote API documentation

This document explains the inner workings of the remote API, including information about creating new endpoints.

Currently all endpoints are synchronous, as all they do is (at the end of a few function calls) read a variable or do some (simple) IO.
There is no technical obstacle to creating asynchronous calls, but the linux remote implementation is blocking.

## Control flow

The unit of control is a length-prefixed chunk.
Lengths are one byte.
The only invalid chunks are `{0}` and `{1}`.

Each endpoint is listed in [source/controllers/], one per endpoint, with a switch table in [spi_chunks_slave.cpp] responsible for dispatching.
Whenever a valid spi packet is received, the chunk parser calls the dispatcher for each chunk valid chunk within it.
The dispatcher calls an appropriate handler, which currently means a big switch statement.

A message is received over the spi protocol, possibly having been retransmitted.
The callback `ammtinycb` in [amm-tiny.cpp] is called, passing control to `spi_chunk_overall` in [spi_chunks_slave.cpp].
`spi_chunk_overall` calls `spi_msg_chunks` in [spi_chunks.cpp] with the `chunk_dispatcher_slave` chunk callback.
`chunk_dispatcher_slave` in [spi_chunks_slave.cpp] reads the type field of each chunk and calls the appropriate handler.
The handlers are in [source/controllers/].

## To add a chunk type (aka an endpoint)

1. Add definitions to [spi_proto/src/spi_remote_api.h] and [spi_proto/src/spi_chunk_defines.h] for the chunk id and the chunk formats.
Because it is a submodule, this must be done separately.
2. Add files in [source/controllers/] to handle your chunks.
3. Add a case to the switch in `chunk_dispatcher_slave` in [spi_chunks_slave.cpp].
4. Add any desired `remote_*` functions to [spi_proto/src/spi_proto_master.cpp] and [spi_proto/src/spi_remote_host.c] and possibly their header files.

[amm-tiny.cpp]: source/amm-tiny.cpp
[source/controllers/]: source/controllers/
[spi_chunks.cpp]: spi_proto/src/spi_chunks.cpp
[spi_chunks_slave.cpp]: source/spi_chunks_slave.cpp
[spi_proto/src/spi_chunk_defines.h]: spi_proto/src/spi_chunk_defines.h
[spi_proto/src/spi_proto_master.cpp]: spi_proto/src/spi_proto_master.cpp
[spi_proto/src/spi_remote_api.h]: spi_proto/src/spi_remote_api.h
[spi_proto/src/spi_remote_host.c]: spi_proto/src/spi_remote_host.c
