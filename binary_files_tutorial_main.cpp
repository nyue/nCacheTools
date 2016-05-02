#include <stdio.h>
#include <stdint.h>
#ifdef WIN32
#include <Winsock2.h>
#else // WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // WIN32

int main()
{
    {
        FILE *fp = fopen("data.bin","wb");
        if (fp)
        {
//        	int32_t value_int32 = htonl(4);
//        	fwrite(&value_int32,sizeof(value_int32),1,fp);

        	double value_double = 0.0;
        	uint64_t value_uint64;
#ifdef __linux__
        	value_uint64 = htole64(uint64_t(value_double));
#else
        	value_uint64 = ntohll(uint64_t(value_double));
#endif // __linux__
        	// fwrite(&value_uint64,sizeof(value_uint64),1,fp);
        	fwrite(&value_double,sizeof(value_double),1,fp);
        	fclose(fp);
        }
    }
    {
    }

    return 0;
}
