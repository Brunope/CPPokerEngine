/**
  * HandRanks.S - compiles to an object file containing the contents
  * of HandRanks.dat, once linked, the file's contents can be accessed
  * via the symbol HandRanks_start. This way, the statically built library
  * is standalone, and I don't have to worry about resource paths at runtime.
  *
  * When compiling pass the flag -D__HandRanks_file_path='"FPATH"', see
  * Makefile rule for HandRanks.o
  */

        .global HandRanks_start
        .align 4
HandRanks_start:
        .incbin __HandRanks_file_path

        .global HandRanks_end
HandRanks_end:
