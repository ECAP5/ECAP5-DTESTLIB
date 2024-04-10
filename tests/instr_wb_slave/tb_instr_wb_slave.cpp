/*           __        _
 *  ________/ /  ___ _(_)__  ___
 * / __/ __/ _ \/ _ `/ / _ \/ -_)
 * \__/\__/_//_/\_,_/_/_//_/\__/
 * 
 * Copyright (C) Clément Chaine
 * This file is part of ECAP5-DTLIB <https://github.com/cchaine/ECAP5-DTLIB>
 *
 * ECAP5-DTLIB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ECAP5-DTLIB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ECAP5-DTLIB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <svdpi.h>

#include "Vtb_instr_wb_slave.h"
#include "testbench.h"
#include "Vtb_instr_wb_slave_ecap5_dproc_pkg.h"

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

enum CondId {
  __CondIdEnd
};

void tb_instr_wb_slave_no_stall_read(TB_Instr_wb_slave * tb) {
  Vtb_instr_wb_slave * core = tb->core;
  core->testcase = 1;

  // The following actions are performed in this test :
  //    tick 0. Set inputs for read no stall

  //=================================
  //      Tick (0)
  
  tb->reset();
  
  //`````````````````````````````````
  //      Set inputs
  
  core->stall_request_i = 0;
  uint32_t addr = rand();
  core->wb_adr_i = addr;
  core->wb_we_i = 0;
  core->wb_sel_i = 0xF;
  core->wb_stb_i = 1;
  core->wb_cyc_i = 1;

  //=================================
  //      Tick (1)
  
  tb->tick();

  //`````````````````````````````````
  //      Checks 
  
  

  //`````````````````````````````````
  //      Formal Checks 
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

  tb_instr_wb_slave_stall_read(tb);
  tb_instr_wb_slave_stall_write(tb);

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
