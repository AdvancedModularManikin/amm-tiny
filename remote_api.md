# Description

This document explains the inner workings of the remote API, including information about creating new endpoints.

Each endpoint is listed in `source/controllers/`, one per endpoint, with a switch table in `spi_chunks_slave.cpp` responsible for dispatching. Whenever a valid SPI packet is received, the chunk parser calls the dispatcher for each chunk valid chunk within it. The only invalid chunks are `{0}` and `{1}`. The dispatcher calls an appropriate handler, which currently means a big switch statement.

To add an endpoint, modify this switch statement, modify the header in `spi_proto` with the endpoint numbers, and for convenience create any desired `remote_*` functions.

Currently all endpoints are synchronous, as all they do is (at the end of a few function calls) read a variable or do some (simple) IO. There is no technical obstacle to creating asynchronous calls, but the linux remote implementation is blocking.
