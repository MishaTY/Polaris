#include <stdint.h>
#include <stddef.h>
#include "../video/video.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/pit.h"
#include "../mm/pmm.h"
#include "../mm/vmm.h"
#include "../serial/serial.h"
#include <stivale2.h>
#include "die.h"

extern void init_gdt(void);

static uint8_t stack[4096];
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    // All tags need to begin with an identifier and a pointer to the next tag.
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = 0,
    .tags = (uintptr_t)&framebuffer_hdr_tag
};

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {

        if (current_tag == NULL) {
            return NULL;
        }

        if (current_tag->identifier == id) {
            return current_tag;
        }

        current_tag = (void *)current_tag->next;
    }
}

void _start(struct stivale2_struct *stivale2_struct) {
    stivale2_struct = (void *)stivale2_struct + MEM_PHYS_OFFSET;
    init_gdt();
    struct stivale2_struct_tag_framebuffer *fb_str_tag;
    fb_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    video_init(fb_str_tag);
    struct stivale2_struct_tag_memmap *memmap_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    pmm_init((void *)memmap_tag->memmap, memmap_tag->entries);
    vmm_init((void *)memmap_tag->memmap, memmap_tag->entries);
    if (fb_str_tag == NULL) {
        for (;;)
            __asm__("hlt");
    }
    serial_install();
    clear_screen(0x000000);
    kprint("Did the GDT work?\n");
    kprint("Did the PMM work?\n");
    kprint("Did the VMM work?\n");
    write_serial("Did Serial work?\n");
    isr_install();
    asm volatile("sti");
    kprint("Did the ISR load?\n");
    set_pit_freq(1000);
    kprint("Did the timer load?\n");
    //hex_to_ascii_upper(return_highest_page(), x);
    //kprint("Highest Page Value: ");
    //kprint(x);
    for (;;)
        __asm__("hlt");
}
