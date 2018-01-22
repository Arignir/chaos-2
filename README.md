ChaOS is an SMP-aware kernel that we are doing as a part of our studies at Epitech.

# Build Dependencies
* `make`
* `gcc` or `clang` (latest version, ideally)
* `grub-mkrescue` and `libisoburn` (usually packed with other binaries as `grub`)
* `mtools`
* `dialogs`
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

First, tune as you wish the kernel's configuration with
```bash
make config
```

Then, build the kernel:
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
- [X] Kernel placed in high-addresses
- [X] Boot
  - [X] Multiboot
  - [X] Paging setup
  - [X] GDT setup
  - [X] IDT setup
  - [X] TSS setup
  - [X] SMP setup
- [ ] Basic drivers
  - [X] VGA
  - [ ] Serial
  - [ ] PS/2 Keyboard
- [X] Memory
  - [X] Physical Memory Management
  - [X] Virtual Memory Management (`mmap()`, `munmap()` etc.)
  - [X] Virtual segments of memory
  - [X] Kernel heap (`kalloc()`, `kfree()`, `krealloc()`)
- [X] ELF Program execution (`exec()`)
- [X] Syscall interface and userspace (ring 3)
- [ ] Multi process / threads
  - [X] Scheduling
  - [X] User Threads
  - [ ] Kernel threads
  - [ ] Processes (mix of `clone()`, `exec()` and `exit()`)
- [X] Initrd loading, reading and writing
- [ ] Device abstraction
- [ ] Filesystem
  - [ ] Directory listing (`opendir()`, `readdir()`, `closedir()`)
  - [ ] Creating and removing files/directories (`mkfile()`, `mkdir()`, `remove()`)
  - [ ] File basic IO operations (`open()`, `read()`, `write()`, `close()`)
  - [ ] File advanced IO operations (`pipe()`, `dup()`)
  - [ ] File informations (`stat()`)
- [ ] User space programs (init, tty, shell, basic binaries such as `echo`, `ls`, `rm`, `mkdir` etc.)
- [ ] Virtual filesystems (`/proc`, `/dev`)

# :rocket: Wanna participate?

Fork me!
