#pragma once

#define ERR(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)
#define LOG(fmt, ...) //fprintf(stdout, fmt "\n", ##__VA_ARGS__)
