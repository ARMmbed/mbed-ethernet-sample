## yotta Target Description Using GCC to Compile for Freescale Kinetis K64 Chips

This is a [yotta](https://github.com/ARMmbed/yotta) target description for
compiling for Freescale's Kinetis K64 family of microcontrollers. Currently
only the Mk64Fn1M0Vll12 is supported. You should not normally use this target
description directly, but instead use a more specific description of a target
board.

This target description inherits from the generic
[mbed-gcc](https://github.com/ARMmbed/target-mbed-gcc) target description,
which provides most of the information about how to run the compiler.

**To build for the FRDM K64F development board, use the
[frdm-k64f-gcc](https://github.com/ARMmbed/target-frdm-k64f-gcc) target
description** (which inherits from this one). The frdm-k64f target description
also provides a good starting point to copy if you are creating a new target
description for your own board using a K64F microcontroller.


