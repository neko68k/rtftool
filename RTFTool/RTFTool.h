#ifndef __RTFTOOL_H__
#define __RTFTOOL_H__

int openFile(char *inFN);
int openFile(unsigned char *data, unsigned long size);
void openFile(unsigned char *data, unsigned long size, unsigned char exp);

#endif