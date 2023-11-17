#ifndef LUNARRED_H
#define LUNARRED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LunarRed header file
// contains definitions for certain global LunarRed variables
// Copyright 2019-2023 Kai D. Gonzalez

#define LUNARRED_VERSION "1.0.0"
#define LUNARRED_VERSION_LONG "LunarRed v" LUNARRED_VERSION

// external definitions for other bytecode interpreters
#define LUNARRED_OPENLUD_BYTE_LIMIT 0x100 // limit for OpenLUD bytecode size
#define LUNARRED_NEXFUSE_BYTE_LIMIT LUNARRED_OPENLUD_BYTE_LIMIT // NexFUSE follows OpenLUD

// memory increment factor
#define LUNARRED_MEMORY_INCREMENT 50

// type
#define byte char

#endif
