#ifndef DTLIB_ELF_H
#define DTLIB_ELF_H

#include <stdint.h>

void load_elf(std::string path, void * buffer, uint32_t size) {
  FILE * fd = fopen(path.c_str(), "rb");
  if(fd == NULL) {
    printf("Couldn't find elf file at path %s\n", path.c_str());
    return;
  }

  // Check elf magic
  uint8_t magic = 0;
  fread(&magic, 1, 1, fd);
  if(magic != 0x7F) {
    printf("The file %s is not an elf binary\n", path.c_str());
    return;
  }

  // Read elf header and find elf program header
  uint32_t ph_offset = 0;
  fseek(fd, 28, SEEK_SET);
  fread(&ph_offset, 4, 1, fd);

  // Find the number of program segments
  uint16_t ph_entry_size = 0;
  uint16_t ph_num_entries = 0;
  fseek(fd, 42, SEEK_SET);
  fread(&ph_entry_size, 2, 1, fd);
  fseek(fd, 44, SEEK_SET);
  fread(&ph_num_entries, 2, 1, fd);

  // For each segment, copy the data to the provided buffer
  for(int i = 0; i < ph_num_entries; i++) {
    uint32_t p_type = 0;
    fseek(fd, ph_offset + ph_entry_size * i, SEEK_SET);
    fread(&p_type, 4, 1, fd);

    uint32_t p_offset = 0;
    fseek(fd, ph_offset + ph_entry_size * i + 4, SEEK_SET);
    fread(&p_offset, 4, 1, fd);

    uint32_t p_vaddr = 0; 
    fseek(fd, ph_offset + ph_entry_size * i + 8, SEEK_SET);
    fread(&p_vaddr, 4, 1, fd);

    uint32_t p_filesz = 0;
    fseek(fd, ph_offset + ph_entry_size * i + 16, SEEK_SET);
    fread(&p_filesz, 4, 1, fd);

    uint32_t p_memsz = 0;
    fseek(fd, ph_offset + ph_entry_size * i + 20, SEEK_SET);
    fread(&p_memsz, 4, 1, fd);
    
    // If it is a LOAD segment
    if(p_type == 1) {
      // Check if bigger than the memory size
      if(p_vaddr + p_memsz > size) {
        printf("Memory overflow with segment %d when loading binary %s\n  Memory end address : %08x, Segment end address: %08x\n\n", i, path.c_str(), size-1, p_vaddr + p_memsz - 1);
        return;
      }

      // Copy the segment data
      fseek(fd, p_offset, SEEK_SET);
      fread(buffer + p_vaddr, 1, p_filesz, fd);
    }
  } 

  fclose(fd);
}

void hexdump_buffer(uint32_t size, uint32_t max_size) {
  if(size > max_size) {
    size = max_size;
  }

  char ascii[17];
  int i, j;
  ascii[16] = '\0';
  printf("%08x: ", 0);
  for (i = 0; i < size; ++i) {
    printf("%02X", ((unsigned char*)buffer)[i]);
    if(i % 2 == 1) {
      printf(" ");
    }
    if (((unsigned char*)buffer)[i] >= ' ' && ((unsigned char*)buffer)[i] <= '~') {
      ascii[i % 16] = ((unsigned char*)buffer)[i];
    } else {
      ascii[i % 16] = '.';
    }
    if ((i+1) % 8 == 0 || i+1 == size) {
      if ((i+1) % 16 == 0) {
        printf(" %s \n", ascii);
        if(i+1 < size) {
          printf("%08x: ", i);
        }
      } else if (i+1 == size) {
        ascii[(i+1) % 16] = '\0';
        if ((i+1) % 16 <= 8) {
          printf(" ");
        }
        for (j = (i+1) % 16; j < 16; ++j) {
          printf("   ");
        }
        printf(" %s \n", ascii);
      }
    }
  }
}

#endif
