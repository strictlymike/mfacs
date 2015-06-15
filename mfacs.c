// File: mfacs.c
// Desc: Quick and dirty utility that uses the MapFileAndCheckSum() function to
// load program image and calculate checksum of a binary. It's mostly a wrapper
// for this API:
//
// 	MapFileAndCheckSum() function:
// 	https://msdn.microsoft.com/en-us/library/windows/desktop/ms680355%28v=vs.85
// 	%29.aspx

#include <windows.h>
#include <Imagehlp.h>
#include <stdio.h>

#pragma comment(lib, "Imagehlp.lib")

int main(int argc, char **argv)
{
	DWORD hs;
	DWORD cs;
	DWORD Ret;

	if (argc != 2)
	{
		return Usage(1, argv[0]);
	}

	if (!strcmp(argv[1], "/?") ||
		!strcmp(argv[1], "-h") ||
		!strcmp(argv[1], "--help")
	   )
	{
		return Usage(0, argv[0]);
	}

	Ret = MapFileAndCheckSum(argv[1], &hs, &cs);

	switch (Ret)
	{
		case CHECKSUM_SUCCESS:
			Ret = (hs == cs)? 0: 1;
			printf(
				"%s: Header (0x%08x) %s Computed (0x%08x)\n",
				Ret? "ERROR": "Okay", hs, Ret? "!=": "==", cs
			   );
			break;
		case CHECKSUM_MAP_FAILURE:
			fprintf(stderr, "Could not map the file\n");
			break;
		case CHECKSUM_MAPVIEW_FAILURE:
			fprintf(stderr, "Could not map a view of the file\n");
			break;
		case CHECKSUM_OPEN_FAILURE:
			fprintf(stderr, "Could not open the file\n");
			break;
		case CHECKSUM_UNICODE_FAILURE:
			fprintf(stderr, "Could not convert the file name to Unicode\n");
			break;
		default:
			fprintf(stderr, "Unknown error %d, GLE=%d\n", Ret, GetLastError());
	}

	return Ret;
}

int Usage(int Err, char *Progname)
{
	fprintf(
		Err? stderr: stdout,
		"Map file and check sum\n"
		"Usage: %s filename\n",
		Progname
	   );

	return Err;
}
