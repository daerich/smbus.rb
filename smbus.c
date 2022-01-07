/*
* ISC License
*
* Copyright (c) 2022 Erich Ericson
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
* REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
* AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
* INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
* OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include "ruby.h"
#include <linux/i2c.h>
#include <i2c/smbus.h>

static VALUE read_byte_data(VALUE self, VALUE cmd)
{
	int ret = 0;
	int fd = 0;

	Check_Type(cmd, T_FIXNUM);
	fd = NUM2INT(rb_funcall(self, rb_intern("fileno"), 0));
	ret = i2c_smbus_read_byte_data(fd, FIX2INT(cmd));
	if (ret < 0) {
		rb_raise(rb_eIOError, "SMBUS Read operation failed!\
Got Code: %d", ret);
		return Qnil;
	} else {
		return INT2FIX(ret);
	}
}

static VALUE write_byte_data(VALUE self, VALUE reg, VALUE bit)
{	
	int ret = 0;
	int fd = 0;

	Check_Type(reg, T_FIXNUM);
	Check_Type(bit, T_FIXNUM);
	fd = NUM2INT(rb_funcall(self, rb_intern("fileno"), 0));
	ret = i2c_smbus_write_byte_data(fd, FIX2INT(reg), FIX2INT(bit));
	if (ret) {
		rb_raise(rb_eIOError, "SMBUS Write operation failed!\
Got Code: %d", ret);
		return INT2FIX(ret);
	} else {
		return INT2FIX(ret);
	}
}

void Init_Smbus(void)
{
	VALUE mod_smbus = rb_define_module("Smbus");
	rb_define_method(mod_smbus, "read_data",read_byte_data, 1);
	rb_define_method(mod_smbus, "write_data", write_byte_data, 2);
}
