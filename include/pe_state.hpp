/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pe_state.hpp
 * Author: parfait
 *
 * Created on 3 octobre 2018, 14:34
 */

#pragma once
#include "types.hpp"
#include "slab.hpp"
#include "compiler.hpp"
#include "queue.hpp"
#include "console.hpp"
#include "regs.hpp"
#include "stdio.hpp"

class Pe_state {
    
public:
    enum Type {
        PE_STATE_DEFAULT              = 0,
        PE_STATE_RESOLVE_COWFAULT     = 1,
        PE_STATE_PLACE_PHYS0          = 2,
    };
    
private:
    friend class Queue<Pe_state>;
    friend class Pe;
    static Slab_cache cache;    
    static Queue<Pe_state> pe_states, log_pe_states;
    
    static size_t number;
    mword rax = 0, rbx = 0, rcx = 0, rdx = 0, rbp = 0, rdi = 0, rsi = 0, rsp = 0, rip = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r13 = 0, r14 = 0, r15 = 0;
    mword rsp_content = 0, rip_content = 0;
    uint64 retirement_counter = 0;
    uint8 run_no = 123; 
    mword int_no = 0;
    bool is_vcpu = false;
    size_t numero = 0;
    mword instruction = 0;
    mword attr = 0; 
    mword sub_reason = 0;
    mword diff_reason = 0;
    Type type = PE_STATE_DEFAULT;
    size_t count = 0, page_twin_index = 0;
    int missmatch_addr = 0;
    mword page_addr = 0, page_twin_addr = 0, page_addr_placed = 0, page_twin_addr_placed = 0;
    Paddr phys0 = 0, phys1 = 0, phys2 = 0, phys0_placed = 0, phys1_placed = 0, phys2_placed = 0;
    
    Pe_state* prev;
    Pe_state* next;
    
public:
    
    /**
     * 
     * @param ec
     * @param pd
     * @param rip_value
     * @param counter
     */
    Pe_state(Exc_regs*, uint64, uint8, mword, bool = false); 
    Pe_state(Cpu_regs*, uint64, uint8, mword, bool = false); 
    Pe_state &operator = (Pe_state const &);
    Pe_state(size_t, int, mword, Paddr, Paddr, Paddr, mword, mword);
    Pe_state(mword, Paddr, Paddr, Paddr, mword);
    ALWAYS_INLINE
    static inline void *operator new (size_t, Quota &quota) { return cache.alloc(quota); }

    ALWAYS_INLINE
    static inline void destroy (Pe_state *obj, Quota &quota) { obj->~Pe_state(); cache.free (obj, quota); }
    
    Pe_state(const Pe_state& orig);    
    ~Pe_state();
    
    ALWAYS_INLINE
    static inline void operator delete (void *ptr, Quota &quota) {
        Pe_state* pe_state = static_cast<Pe_state*> (ptr);
        pe_state->~Pe_state();
        cache.free (ptr, quota);
    }

    void print(){
//        trace(0, "%d, A %010lx B %010lx C %010lx D %010lx S %010lx D %010lx B %010lx S %010lx I %010lx R8 %010lx %010lx %010lx %010lx %010lx "
//        "%010lx %010lx %010lx, %0#12llx, %ld:%ld %lx", run_no, rax, rbx, rcx, rdx, rbp, rdi, rsi, rsp, rip, r8, r9, 
//                r10, r11, r12, r13, r14, r15, retirement_counter, int_no, sub_reason, diff_reason);
        switch(type){
            case PE_STATE_RESOLVE_COWFAULT:
                trace(0, "  count %lu MM %x c %lx %lx %lx %lx ce %lx  index %lu", count, missmatch_addr, 
                    page_addr, phys0, phys1, phys2, page_twin_addr, page_twin_index);
                break;
            case PE_STATE_PLACE_PHYS0:
                trace(0, "Placing c %lx %lx %lx %lx ce %lx", page_addr_placed, phys0_placed, 
                        phys1_placed, phys2_placed, page_twin_addr_placed);
                break;
            case PE_STATE_DEFAULT:
                break;
        }
    }

    static size_t get_number(){
        return number;
    }
    
    static void add_pe_state(Pe_state*);
    
    static void set_current_pe_sub_reason(mword);
    
    static void set_current_pe_diff_reason(mword);
    
    static void free_recorded_pe_state();
    static void free_recorded_log_pe_state();
    
    static void dump();
    static void dump_log();
};