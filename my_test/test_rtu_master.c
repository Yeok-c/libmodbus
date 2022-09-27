#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <errno.h>
#include "modbus.h"

#include <errno.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define PORT_NAME "/dev/ttyUSB1"

int main(int argc, char *argv[])
{
    int ret;
    uint16_t table[3];
    modbus_t *mb;
    char port[20];  
    printf("argc = %d, argv[1] = %s\n", argc, argv[1]);
    if(argc == 2)
        strcpy(port, argv[1]);
    else 
        strcpy(port, PORT_NAME);
    printf("libmodbus modbu-rtu master demo: %s, 9600, N, 8, 1\n", port);

    mb = modbus_new_rtu(port, 9600, 'N', 8, 1);

    modbus_set_debug(mb, TRUE);

    if (mb == NULL)
    {
        modbus_free(mb);
        printf("new rtu failed: %s\n", modbus_strerror(errno));
        return 0;
    }

    modbus_set_slave(mb, 1);
    ret = modbus_connect(mb);

    if(ret == -1)
    {
        modbus_close(mb);
        modbus_free(mb);
        printf("connect failed: %s\n", modbus_strerror(errno));
        return 0;
    }
    while(1)
    {
        // Read 3001 
        ret = modbus_read_registers(mb, 0x0BB9, 1, table);
        if(ret == 3)
            printf("read success : 0x%02x 0x%02x 0x%02x \n", table[0], table[1], table[2]);
        else
        {
            printf("read error: %s\n", modbus_strerror(errno));
            break;
        }

        // for(int i = 0; i < 3; i++)
        //     table[i] += 1;

        // ret = modbus_write_registers(mb, 0x0F, 3, table);
        // if(ret == 3)
        //     printf("write success: 0x%02x 0x%02x 0x%02x \n", table[0], table[1], table[2]);
        // else
        // {
        //     printf("write error: %s\n", modbus_strerror(errno));
        //     break;
        // }
        // Sleep(1000);
        sleep(1000);
    }

    modbus_close(mb);
    modbus_free(mb);
    system("pause");
    return 0;
}