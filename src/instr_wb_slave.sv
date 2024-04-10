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

module instr_wb_slave (
  input   logic        clk_i,
  input   logic        rst_i,

  //=================================
  //    Wishbone interface 
  
  input   logic[31:0]  wb_adr_i,
  output  logic[31:0]  wb_dat_o,
  input   logic[31:0]  wb_dat_i,
  input   logic        wb_we_o,
  input   logic[3:0]   wb_sel_o,
  input   logic        wb_stb_i,
  output  logic        wb_ack_o,
  input   logic        wb_cyc_i,
  output  logic        wb_stall_o,

  //=================================
  //    Instrumentation interface
  
  input   logic        stall_request_i,
  input   logic[31:0]  injected_data_i,
  output  logic[31:0]  sniffed_data_o
);

enum logic[1:0] {
  IDLE,
  STALL, 
  RESPONSE
} state_d, state_q;

always_comb begin
  state_d = state_q;

  case(state_q)
    IDLE: begin
      if(wb_stb_i && wb_cyc_o) begin
        if(stall_request_i) begin
          state_d = STALL;
        end else begin
          state_d = RESPONSE;
        end
      end
      STALL: begin
        if(!stall_request_i) begin
          state_d = RESPONSE;
        end
      end
      RESPONSE: begin
        state_d = IDLE;
      end
    end
  endcase
end

always_ff @(posedge clk_i) begin
  if(rst_i) begin
    state_q <= IDLE;
  end else begin
    state_q  <=  state_d;
  end
end

endmodule // instr_wb_slave
