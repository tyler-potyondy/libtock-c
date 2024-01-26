The platform abstraction layer implementation.

Statuses:
- rng/entropy: Done porting
- `misc`: should be possible to test openthread even without implementing this. None of the functions seem *necessary*
- `openthread-system`: similar to `misc`, but waiting on Tyler to finish other stuff since his drivers and whatever the kernel design is for radio drivers will need to be added to libtock-c first.
- `radio`: **a lot of work**
- `flash`: **a lot of work?**
- `alarm-milli`: in progress
