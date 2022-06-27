-------------------------------------------------------------------------------
-- Title      : Frequency Divider
-- Project    : General IP
-------------------------------------------------------------------------------
-- Author     : Markus Pfaff, Hagenberg/A, Copyright (c) 2003-2005
-- RevCtrl    : $Id: FreqDivider-Rtl-a.vhd 22 2011-09-20 21:51:00Z mroland $
-- UsrNames   : pfaff - Markus Pfaff
-------------------------------------------------------------------------------
-- Description : 
-- Description for synthesis.
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.Global.all;

entity FreqDivider is
  
  generic (
    gDivideBy : natural := 4);

  port (
    -- Sequential logic inside this unit
    iClk         : in std_ulogic;
    inResetAsync : in std_ulogic;

    -- Strobe with the above given cycle time
    oDivdClk : out std_ulogic);

begin

  assert (gDivideBy >= 4)
    report "Mp: Divider must be at least 4."
    severity error;

  assert (gDivideBy rem 2 = 0)
    report "Mp: Divider must be an even number."
    severity error;


end FreqDivider;

architecture Rtl of FreqDivider is

  constant cClkCycPerHalfDivdClkPeriod : natural := gDivideBy/2;

  constant cClkCounterBinDigitCount : natural :=
    LogDualis(cClkCycPerHalfDivdClkPeriod);

  -- The register whe use for counting the iClk cycles.
  signal ClkCounter : unsigned(cClkCounterBinDigitCount-1 downto 0);

  signal DivdClk : std_ulogic;

begin  -- architecture Rtl

  -- Count the number of Clk cycles from strobe pulse to strobe pulse.
  GenDivdClk : process (iClk, inResetAsync) is
  begin  -- process GenStrobe

    -- Asynchronous reset
    if inResetAsync = cnActivated then

      -- We begin counting at 0.
      ClkCounter <= to_unsigned(0, ClkCounter'length);
      -- oDivdClk shall be sourced directly by a register output to ease time budgeting
      -- when several units are connected with each other.
      DivdClk    <= '0';

      -- Rising clk edge
    elsif iClk'event and iClk = '1' then

      -- We count from 0 to cClkCycPerStrobeCyc-1.
      if ClkCounter = cClkCycPerHalfDivdClkPeriod-1 then
        ClkCounter <= (others => '0');
        DivdClk    <= not(DivdClk);
      else
        ClkCounter <= ClkCounter+1;
      end if;

    end if;
  end process GenDivdClk;

  oDivdClk <= DivdClk;

end architecture Rtl;
