# nzux

WIP operating system for x86 that I'm making for fun

Will probably not be done with this for like ages

Not practical at all

## Building
- Build docker image
- Run docker image
- in container run `make all`
- In another terminal run `qemu-system-i386 -cdrom dist/i686/nzux.iso -s`

## Debugging
- Use bochs, the config is in `dist/i686/bochsrc.bxrc`