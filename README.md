ChaOS is a fun project that i'm doing for my studies at Epitech.

# Build Dependencies
* `make`
* `gcc` or `clang` (last version preferred)
* `grub-mkrescue` and `libisoburn` (generally packed with other binaries as `grub`)
* `mtools`
* `qemu` (cpu emulator) *optional*

If you are using `apt-get` as your package manager (`Debian`, `Ubuntu` etc.), you can use this command to install all dependencies:
```bash
apt-get install qemu grub-pc-bin xorriso mtools
```

If you are using `pacman` as your package manager (`ArchLinux`, `Manjaro` etc.), you can use this command:
```bash
pacman -Sy qemu grub libisoburn mtools
```

If you are using `portage` as your package manager (`Gentoo`), you can use this command instead:
```bash
emerge --ask sys-boot/libisoburn sys-fs/dosfstools sys-fs/mtools
```

If you are using an other package manager, well... Good luck! :p

# Building an iso

First, edit the configuration that you want with
```bash
make config
```

Then, build the kernel by running
```bash
make kernel
```

To build a complete iso with grub installed (suitable for USB flash drives or virtual machines), run
```bash
make iso
```

# Running with QEMU

If you want to run ChaOS through QEMU even if it's boring & useless right now, run
```bash
make run
```

# Roadmap

- [X] Kernel options
- [X] Kernel architecture
- [ ] High-address Kernel
- [ ] Boot
  - [X] Multiboot
  - [ ] Paging setup
  - [X] GDT setup
  - [ ] IDT setup
  - [ ] TSS setup
  - [ ] SMP setup
- [ ] Pc drivers
  - [X] VGA
  - [ ] Serial
- [ ] Memory
  - [ ] PMM
  - [ ] VMM
  - [ ] Kernel heap
  - [ ] User heap
  - [ ] User stacks / mmap
- [ ] Multi process / threads
  - [ ] Scheduling
  - [ ] Kernel threads
  - [ ] Processes (`fork()` and `exit()`)
- [ ] Syscall interface
- [ ] Userspace (ring3)
- [ ] Filesystem (`open()`, `read()`, `write()`, `close()`, `unlink`, `opendir()`, `readdir()`, `closedir()`, `pipe()`, `dup()`, `dup2()`)
- [ ] ELF parsing and loading (`execve()`)
- [ ] User space programs (init, shell, basic binaries such as `echo`, `ls`, `rm` etc.)

# :rocket: Wanna participate?

Fork me!
