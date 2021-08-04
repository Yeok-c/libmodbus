# modbus_set_compliant

## Name

modbus_set_compliant - set compliant flag of the context

## Synopsis

```c
int modbus_set_compliant(modbus_t *ctx, int flag);
```

## Description

The *modbus_set_compliant()* function shall set the compliant flag of the
*modbus_t* context by using the argument `flag`. By default, the boolean flag is
set to `TRUE`. When the `flag` value is set to `FALSE`, different non-standard
behaviors to the protocol are allowed:

- slave adresses between 247 and 255 are allowed.

## Return value

The function shall return 0 if successful. Otherwise it shall return -1 and set
errno.
