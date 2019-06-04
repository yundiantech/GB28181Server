#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdio.h>
#include <list>

enum MessageType
{
    MessageType_Register = 0,
    MessageType_KeepAlive,
    MessageType_Catalog,
    MessageType_CallAnswer,
    MessageType_CallFailed
};


#endif // TYPES_H
