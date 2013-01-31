#include "stdafx.h"
#include "axl_pg_Parser.h"
#include "axl_pg_Generator.h"
#include "axl_pg_Config.h"

using namespace axl;
using namespace pg;

//.............................................................................

void 
PrintUsage ()
{
	printf (
		"till -- tibbo LL (k) parser generator\n"
		"usage: till <grammar_file> [options]\n"
		"    -?, -h, -H        print this usage and exit\n"
		"    -o <output_file>  generate <output_file> (multiple allowed)\n"
		"    -O <output_dir>   set output directory to <output_dir>\n"
		"    -f <frame_file>   use LUA frame <frame_file> (multiple allowed)\n" 
		"    -F <frame_dir>    add frame file directory <frame_dir> (multiple allowed)\n"
		"    -I <import_dir>   add import file directory <import_dir> (multiple allowed)\n"
		"    -t <trace_file>   write debug information into <trace_file>\n"
		"    -v                verbose mode\n"
		);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int _tmain(int argc, _TCHAR* argv[])
{
	bool Result;

	err::CParseErrorProviderA::Register ();

	CConfig Config;

	// analyze command line

	const tchar_t* pSrcFileName = NULL;
	const tchar_t* pTraceFileName = NULL;
	rtl::CStdArrayListT <CTarget> TargetList;

	size_t OutputCount = 0;
	size_t FrameCount = 0;

	CTarget* pTarget;
	rtl::CString String;

	for (int i = 1; i < argc; i++)
	{
		if (argv [i][0] == '-' || argv [i][0] == '/') // switch
		{
			switch (argv [i][1])
			{
			case '?': case 'h': case 'H':
				PrintUsage ();
				return 0;

			case 'o':
				pTarget = TargetList.Get (OutputCount++);
				pTarget->m_FileName = argv [i][2] ? &argv [i][2] : ++i < argc ? argv [i] : NULL;
				break;

			case 'O':
				Config.m_OutputDir = argv [i][2] ? &argv [i][2] : ++i < argc ? argv [i] : NULL;
				break;

			case 'f':
				pTarget = TargetList.Get (FrameCount++);
				pTarget->m_FrameFileName = argv [i][2] ? &argv [i][2] : ++i < argc ? argv [i] : NULL;
				break;

			case 'F':
				String = argv [i][2] ? &argv [i][2] : ++i < argc ? argv [i] : NULL;
				Config.m_FrameDirList.InsertTail (String);
				break;

			case 'I':
				String = argv [i][2] ? &argv [i][2] : ++i < argc ? argv [i] : NULL;
				Config.m_ImportDirList.InsertTail (String);
				break;

			case 't':
				pTraceFileName = argv [i][2] ? &argv [i][2] : ++i < argc ? argv [i] : NULL;
				break;

			case 'v':
				Config.m_Flags |= EConfigFlag_Verbose;
				break;
			}			
		}
		else
		{
			pSrcFileName = argv [i];
		}
	}

	if (!pSrcFileName)
	{
		PrintUsage ();
		return 1;
	}

	rtl::CString SrcFilePath = io::GetFullFilePath (pSrcFileName);
	if (SrcFilePath.IsEmpty ())
	{
		printf ("Cannot get full file path of '%s': %s\n", pSrcFileName, err::GetError ()->GetDescription ());
		return 2;
	}

	if (OutputCount != FrameCount)
	{
		printf ("output-file-count vs frame-file-count mismatch\n");
		return 3;
	}

	//if (pTraceFileName)
	//	stdout = fopen (pTraceFileName, "rwt");

	CModule Module;
	CParser Parser;

	Result = Parser.ParseFile (&Module, &Config, SrcFilePath);
	if (!Result)
	{
		printf ("%s\n", err::GetError ()->GetDescription ());
		return 4;
	}

	if (!Module.m_ImportList.IsEmpty ()) 
	{
		rtl::CStringHashTable FilePathSet;
		FilePathSet.Goto (SrcFilePath);

		rtl::CBoxIteratorT <rtl::CString> Import = Module.m_ImportList.GetHead ();
		for (; Import; Import++)
		{
			rtl::CString ImportFilePath = *Import;
			if (FilePathSet.Find (ImportFilePath))
				continue;

			Result = Parser.ParseFile (&Module, &Config, ImportFilePath);
			if (!Result)
			{
				printf ("%s\n", err::GetError ()->GetDescription ());
				return 5;
			}

			FilePathSet.Goto (ImportFilePath);
		}
	}

	Result = Module.Build (&Config);
	if (!Result)
	{
		printf ("%s\n", err::GetError ()->GetDescription ());
		return 6;
	}

	if (Config.m_Flags & EConfigFlag_Verbose)	
		Module.Trace ();

	CGenerator Generator;	
	Generator.Prepare (&Module);
	Generator.m_pConfig = &Config;

	if (!TargetList.IsEmpty ())
	{
		Result = Generator.GenerateList (TargetList.GetHead ());
		if (!Result)
		{
			printf ("%s\n", err::GetError ()->GetDescription ());
			return 8;
		}
	}

	return 0;
}

//.............................................................................
