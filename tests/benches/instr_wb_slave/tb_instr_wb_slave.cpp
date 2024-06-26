/*           __        _
 *  ________/ /  ___ _(_)__  ___
 * / __/ __/ _ \/ _ `/ / _ \/ -_)
 * \__/\__/_//_/\_,_/_/_//_/\__/
 * 
 * Copyright (C) Clément Chaine
 * This file is part of ECAP5-DTESTLIB <https://github.com/ecap5/ECAP5-DTESTLIB>
 *
 * ECAP5-DTESTLIB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ECAP5-DTESTLIB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ECAP5-DTESTLIB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <svdpi.h>

#include "Vtb_instr_wb_slave.h"
#include "testbench.h"

enum CondId {
  COND_wishbone,
  __CondIdEnd
};

enum TestcaseId {
  T_NO_STALL_READ =  0,
  T_NO_STALL_WRITE = 1
};

class TB_Instr_wb_slave : public Testbench<Vtb_instr_wb_slave> {
public:
  void reset() {
    this->_nop();
    this->core->rst_i = 1;
    for(int i = 0; i < 5; i++) {
      this->tick();
    }
    this->core->rst_i = 0;

    Testbench<Vtb_instr_wb_slave>::reset();
  }
  
  void _nop() {
  }
};

void tb_instr_wb_slave_no_stall_read(TB_Instr_wb_slave * tb) {
  Vtb_instr_wb_slave * core = tb->core;
  core->testcase = T_NO_STALL_READ;

  // The following actions are performed in this test :
  //    tick 0. Set inputs for read no stall

  //=================================
  //      Tick (0)
  
  tb->reset();
  
  //`````````````````````````````````
  //      Set inputs
  
  uint32_t addr = rand();
  core->wb_adr_i = addr;
  core->wb_we_i = 0;
  core->wb_sel_i = 0xF;
  core->wb_stb_i = 1;
  core->wb_cyc_i = 1;

  core->stall_request_i = 0;
  core->injected_data_i = rand();

  //`````````````````````````````````
  //      Checks 
  
  tb->check(COND_wishbone, (core->wb_ack_o == 0)); 

  //=================================
  //      Tick (1)
  
  tb->tick();

  //`````````````````````````````````
  //      Set inputs
  
  core->wb_adr_i = 0;
  core->wb_we_i = 0;
  core->wb_sel_i = 0;
  core->wb_stb_i = 0;

  //`````````````````````````````````
  //      Checks 
  
  tb->check(COND_wishbone, (core->wb_ack_o == 1)); 

  //=================================
  //      Tick (2)
  
  tb->tick();

  //`````````````````````````````````
  //      Checks 
  
  tb->check(COND_wishbone, (core->wb_ack_o == 0)); 

  //`````````````````````````````````
  //      Set inputs
  
  core->wb_cyc_i = 0;

  //=================================
  //      Tick (3)
  
  tb->tick();

  //`````````````````````````````````
  //      Formal Checks 
  
  CHECK("tb_instr_wb_slave.no_stall_read.01",
      tb->conditions[COND_wishbone],
      "Failed to implement the wishbone protocol", tb->err_cycles[COND_wishbone]);
}

void tb_instr_wb_slave_no_stall_write(TB_Instr_wb_slave * tb) {
  Vtb_instr_wb_slave * core = tb->core;
  core->testcase = T_NO_STALL_WRITE;
  
  // The following actions are performed in this test :
  //    tick 0. Set inputs for write no stall

  //=================================
  //      Tick (0)
  
  tb->reset();
  
  //`````````````````````````````````
  //      Set inputs
  
  uint32_t addr = rand();
  uint32_t data = rand();
  core->wb_adr_i = addr;
  core->wb_dat_i = data;
  core->wb_we_i = 1;
  core->wb_sel_i = 0xF;
  core->wb_stb_i = 1;
  core->wb_cyc_i = 1;

  core->stall_request_i = 0;
  core->injected_data_i = rand();

  //`````````````````````````````````
  //      Checks 
  
  tb->check(COND_wishbone, (core->wb_ack_o == 0)); 

  //=================================
  //      Tick (1)
  
  tb->tick();

  //`````````````````````````````````
  //      Set inputs
  
  core->wb_adr_i = 0;
  core->wb_dat_i = 0;
  core->wb_we_i = 0;
  core->wb_sel_i = 0;
  core->wb_stb_i = 0;

  //`````````````````````````````````
  //      Checks 
  
  tb->check(COND_wishbone, (core->wb_ack_o == 1)); 

  //=================================
  //      Tick (2)
  
  tb->tick();

  //`````````````````````````````````
  //      Checks 
  
  tb->check(COND_wishbone, (core->wb_ack_o == 0)); 

  //`````````````````````````````````
  //      Set inputs
  
  core->wb_cyc_i = 0;

  //=================================
  //      Tick (3)
  
  tb->tick();

  //`````````````````````````````````
  //      Formal Checks 
  
  CHECK("tb_instr_wb_slave.no_stall_write.01",
      tb->conditions[COND_wishbone],
      "Failed to implement the wishbone protocol", tb->err_cycles[COND_wishbone]);
}

int main(int argc, char ** argv, char ** env) {
  srand(time(NULL));
  Verilated::traceEverOn(true);

  bool verbose = parse_verbose(argc, argv);

  TB_Instr_wb_slave * tb = new TB_Instr_wb_slave;
  tb->open_trace("waves/instr_wb_slave.vcd");
  tb->open_testdata("testdata/instr_wb_slave.csv");
  tb->set_debug_log(verbose);
  tb->init_conditions(__CondIdEnd);

  /************************************************************/

  tb_instr_wb_slave_no_stall_read(tb);
  tb_instr_wb_slave_no_stall_write(tb);

  /************************************************************/

  printf("[INSTR_WB_SLAVE]: ");
  if(tb->success) {
    printf("Done\n");
  } else {
    printf("Failed\n");
  }

  delete tb;
  exit(EXIT_SUCCESS);
}
