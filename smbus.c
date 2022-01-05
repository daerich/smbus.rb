#include <stdio.h>
#include <stdlib.h>
#include "ruby.h"
#include <linux/i2c.h>
#include <i2c/smbus.h>

static VALUE read_byte_data(VALUE self, VALUE fd, VALUE cmd)
{
	int ret = 0;

	Check_Type(fd, T_FIXNUM);
	Check_Type(cmd, T_FIXNUM);
	ret = i2c_smbus_read_byte_data(FIX2INT(fd), FIX2INT(cmd));
	if (ret < 0) {
		rb_raise(rb_eIOError, "SMBUS Read operation failed!");
		return Qnil;
	} else {
		return INT2FIX(ret);
	}
}

static VALUE write_byte_data(VALUE self, VALUE fd, VALUE reg, VALUE bit)
{	
	int ret = 0;

	Check_Type(fd, T_FIXNUM);
	Check_Type(reg, T_FIXNUM);
	Check_Type(bit, T_FIXNUM);
	ret = i2c_smbus_write_byte_data(FIX2INT(fd), FIX2INT(reg), FIX2INT(bit));
	if (!ret) {
		rb_raise(rb_eIOError, "SMBUS Write operation failed!");
		return Qnil;
	} else {
		return INT2FIX(ret);
	}
}

void Init_Smbus(void)
{
	VALUE mod_smbus = rb_define_module("Smbus");
	rb_define_module_function(mod_smbus, "read_data",read_byte_data, 2);
	rb_define_module_function(mod_smbus, "write_data", write_byte_data, 3);
}
