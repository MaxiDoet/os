#include <debug.hpp>
#include <drivers/screen.hpp>
#include <stdarg.h>

SerialPort debugPort(0);

void debugInit(SerialPort serialPort)
{
    debugPort = serialPort;
}

void debugPrint(char* format, ...)
{
    va_list args;

	va_start(args, format);

	char c;
    long n;
    int i=0;
	char* s;
	char cc;

	int base;
	char* prefix;

    while(format[i] != '\0') {
		c=format[i];

        if(c == '%') {
        	i++;
            c=format[i];

            switch(c) {
				case '%':
					debugPort.writeString("%");
					screenPrint("%");
					break;

				case 'c':
					cc = va_arg(args, int);
					debugPort.writeChar(cc);
					break;

                case 'd':
					prefix="";
					base=10;
                    goto print_num;

				case 's':
					s = va_arg(args, char *);
					debugPort.writeString(s);
					screenPrint(s);
					break;

				case 'x':
					prefix="";
					base=16;
                    goto print_num;

				print_num:
					debugPort.writeString(prefix);
					screenPrint(prefix);

					n = va_arg(args, long);

					if (n==0) {
						debugPort.writeString("0");
						screenPrint("0");
					}

                    static char buf[32] = {0};

                    int j = 30;

                    for(; n && j ; --j, n /= base)
						buf[j] = "0123456789abcdef"[n % base];
						debugPort.writeString(&buf[j+1]);
						screenPrint(&buf[j+1]);
						
					break;
			}
        } else {
            debugPort.writeChar(c);
			screenPrintChar(c);
		}

        i++;
	}

	va_end(args);
}