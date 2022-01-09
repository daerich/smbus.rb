# ISC License
#
# Copyright (c) 2022 Erich Ericson
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

require "Smbus"

module Const
  I2C_SLAVE = 1795
  DEV_ID = 0x5C
  WHO_AM_I = 0x0F
  REG1 = 0x20
  REG2 = 0x21
  TEMP_L = 0x2B
  TEMP_H = 0x2C
end

class File
  include Smbus
end

begin
  i2c = File.open("/dev/i2c-1","r+")
  raise(IOError,"ioctl failed") unless i2c.ioctl(Const::I2C_SLAVE, Const::DEV_ID) >= 0
  i2c.read_data(Const::WHO_AM_I)
  i2c.write_data(Const::REG1, 0x00)
  i2c.write_data(Const::REG1, 0x84)
  i2c.write_data(Const::REG2, 0x01)
  sleep 0.4 until i2c.read_data(Const::REG2) == 0;
  temp_l = i2c.read_data(Const::TEMP_L)
  temp_h = i2c.read_data(Const::TEMP_H)
  temp = (temp_h << 8 | temp_l)
  # Overflow calculation/Simulation
  temp = (((temp + 2 ** 15) % 2 ** 16) - 2**15) if temp > 32767
  puts((42.5 + (temp / 480.0)).truncate(2))
  i2c.write_data(Const::REG1, 0x00)
rescue => error
  $stderr.puts "Reading of temperature failed at #{`date`} \
  Reason: #{error.message}"
  exit(1)
ensure
  i2c.close if defined? i2c.close
end
exit(0)
