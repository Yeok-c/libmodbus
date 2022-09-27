#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <errno.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "modbus.h"

#define PORT_NAME "/dev/ttySP2"

int main(int argc, char *argv[])
{
    int ret = 0;
    uint8_t device = 1;
    uint8_t *query;
    modbus_t *mb;
    modbus_mapping_t *mb_mapping;
    char port[20];
    printf("argc = %d, argv[1] = %s\n", argc, argv[1]);
    if(argc == 2)
        strcpy(port, argv[1]);
    else 
        strcpy(port, PORT_NAME);

    printf("libmodbus modbu-rtu slave demo: %s, 115200, N, 8, 1\n", port);

    mb = modbus_new_rtu(port, 115200, 'N', 8, 1);

    if (mb == NULL)
    {
        modbus_free(mb);
        printf("new rtu failed: %s\n", modbus_strerror(errno));
        return 0;
    }

    //register: 15/16/17
    mb_mapping = modbus_mapping_new_start_address(0, 0, 0, 0, 15, 3, 0, 0);
    if(mb_mapping == NULL)
    {
        modbus_free(mb);
        printf("new mapping failed: %s\n", modbus_strerror(errno));
        return 0;
    }

    //保持寄存器数据
    mb_mapping->tab_registers[0] = 0x1001;
    mb_mapping->tab_registers[1] = 0x1002;
    mb_mapping->tab_registers[2] = 0x1003;

    modbus_set_slave(mb, device);
    ret = modbus_connect(mb);

    if(ret == -1)
    {
        modbus_free(mb);
        printf("connect failed: %s\n", modbus_strerror(errno));
        return 0;
    }
    printf("create modbus slave success\n");
    while(1)
    {
        do {
            ret = modbus_receive(mb, query);    //轮询串口数据，
        } while (ret == 0);

        if(ret > 0) //接收到的报文长度
        {
            printf("len=%02d: ", ret);
            for(int idx = 0; idx < ret; idx++)
            {
                printf(" %02x", query[idx]);
            }
            printf("\n");

            modbus_reply(mb, query, ret, mb_mapping);
        }
        else
        {
            printf("quit the loop: %s", modbus_strerror(errno));
            modbus_mapping_free(mb_mapping);
            break;
        }
    }

    modbus_close(mb);
    modbus_free(mb);
    return 0;
}