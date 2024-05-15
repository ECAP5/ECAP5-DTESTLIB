instr_wb_slave
==============

The module ``instr_wb_slave`` is a wishbone slave with an instrumentation interface with the following features :

* Inject read response data
* Stall the interface on request

Interface
---------

.. list-table:: Control signals
  :header-rows: 1
  :width: 100%
  :widths: 10 10 10 70

  * - Name
    - Type
    - Width
    - Description

  * - clk_i
    - I
    - 1
    - Clock input.
  * - rst_i
    - I
    - 1
    - Hardware reset.

.. list-table:: Memory interface signals
  :header-rows: 1
  :width: 100%
  :widths: 10 10 10 70

  * - Name
    - Type
    - Width
    - Description

  * - wb_adr_i
    - I
    - 32
    - The address output array is used to pass binary address.
  * - wb_dat_o
    - O
    - 32
    - The data input array is used to pass binary data.
  * - wb_dat_i
    - I
    - 32
    - The data output array is used to pass binary data.
  * - wb_sel_i
    - I
    - 4
    - The select output array indicates where valid data is expected on the wb_dat_i signal array during READ cycles, and where it is placed on the wb_dat_o signal array during WRITE cycles. Each individual select signal correlates to one of four active bytes on the 32-bit data port.
  * - wb_we_i
    - I
    - 1
    - The write enable output indicates whether the current local bus cycle is a READ or WRITE cycle. This signal is negated during READ cycles and is asserted during WRITE cycles.
  * - wb_stb_i
    - I
    - 1
    - The strobe output indicates a valid data transfer cycle. It is used to qualify various other signals on the interface.
  * - wb_ack_o
    - O
    - 1
    - The acknowledge input, when asserted, indicates the normal termination of a bus cycle.
  * - wb_cyc_i
    - I
    - 1
    - The cycle output, when asserted, indicates that a valid bus cycle is in progress. This signal is asserted for the duration of all bus cycles.
  * - wb_stall_o
    - O
    - 1
    - The pipeline stall input indicates that current slave is not able to accept the transfer in the transaction queue.

.. list-table:: Instrumentation interface signals
  :header-rows: 1
  :width: 100%
  :widths: 10 10 10 70

  * - Name
    - Type
    - Width
    - Description

  * - stall_request_i
    - I
    - 1
    - Stalls the memory interface
  * - injected_data_i
    - I
    - 1
    - Injected read response data
