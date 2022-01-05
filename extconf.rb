require 'mkmf'
find_library("i2c", nil)
create_makefile("Smbus")
