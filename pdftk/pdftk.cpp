#include <iostream>
#include "pdf.h"
#include <stdio.h>

const wchar_t* options[] =  { L"-s", L"--split", L"-m", L"--merge" };




bool						g_bSplitDoc;//splitdoc or merge
std::wstring				g_inp;//input
std::wstring				g_out;//output

std::wstring				g_opt_data;//input2 --- merge...
std::pair<size_t, size_t>	g_range;//bgnidx,count



#define TEST_PDF_INPUT L"C:/Users/ziaud/Desktop/test/test.pdf"
#define TEST_PDF_OUTPUT L"C:/Users/ziaud/Desktop/test/testout.pdf"

#define TEST_PDF_INPUT_MERGE L"C:/Users/ziaud/Desktop/test/testmrg.pdf"

#define TEST_PDF_GC_OUTPUT L"C:/Users/ziaud/Desktop/test/testgcout.pdf"

void print_help()
{
	printf("Usage: pdftk [input] [output] [options]\n");
	printf("Options:\n");
	printf("\t-s,--split <range>\t\tSplit the document. (<range>=BgnPgNum,EndPgNum)\n");
	printf("\t-m,--merge <input2>\t\tMerge <input> <input2>\n");
}

int run()
{
	try 
	{
		if (g_bSplitDoc)
		{
			pdf_split(g_out.c_str(), g_inp.c_str(), g_range.first, g_range.second);
		}
		else
		{
			pdf_merge(g_out.c_str(), g_inp.c_str(), g_opt_data.c_str());
		}
		


		//pdf_gc(g_out.c_str(), g_out.c_str());


		return S_OK;
	}
	catch (PoDoFo::PdfError & e) 
	{
		std::cerr << "Error: An error " << e.GetError() << " ocurred." << std::endl;
		e.PrintErrorMsg();
		return e.GetError();
	}
	catch (std::runtime_error & re) 
	{
		std::cerr << "Error: An error " << re.what() << " ocurred." << std::endl;
		return -1;
	}
}

#pragma region commandline parse
bool file_exist(const wchar_t* file)
{
	FILE* f = _wfopen(file, L"r");
	if (f)
	{
		fclose(f);
		return true;
	}
	fwprintf(stderr, L"File: %s doesn't exist...\n", file);
	return false;
}
bool try_parse_int(const char* str, long* result)
{
	char* pEnd;
	*result = strtol(str, &pEnd, 10);
	return !(*pEnd);
}
bool try_parse_int(const wchar_t* str, long* result)
{

	wchar_t* pEnd;
	*result = wcstol(str, &pEnd, 10);
	return !(*pEnd);
}
bool parse_range(const wchar_t* rng)
{
	if (wcslen(rng) >= 3)
	{
		wchar_t* p = (wchar_t*)wcsstr(rng, L",");

		if (p)
		{
			*p = L'\0';
			long _1, _2;
			if (try_parse_int(rng, &_1) && try_parse_int(++p, &_2))
			{
				g_range.first = _1;
				g_range.second = _2;
				return true;
			}
		}
	}
	perror("Failed to parse range...\n");
	return false;
}
bool parse_commandline(int argc, wchar_t* argv[])
{
	if (argc != 5)
		return false;

	//wprintf(L"path:%s\n", argv[0]);
	g_inp.assign(argv[1]);
	g_out.assign(argv[2]);

	const wchar_t* opt = argv[3];

	g_opt_data.assign(argv[4]);

	if (wcsicmp(opt, options[0]) == 0 || wcsicmp(opt, options[1]) == 0)
	{
		g_bSplitDoc = true;
	}
	else if (wcsicmp(opt, options[2]) == 0 || wcsicmp(opt, options[3]) == 0)
	{
		g_bSplitDoc = false;
	}
	else
	{
		fwprintf(stderr, L"cannout find opt: %s\n", opt);
		return false;
	}
	return
		(g_bSplitDoc ? parse_range(g_opt_data.c_str()) : file_exist(g_opt_data.c_str())) && file_exist(g_inp.c_str());// && file_exist(g_out.c_str());
}
#pragma endregion

//C:\Users\ziaud\Desktop\test\test.pdf C:\Users\ziaud\Desktop\test\testoutrel.pdf -s 200,208
//C:\Users\ziaud\Desktop\test\test.pdf C:\Users\ziaud\Desktop\test\testoutrel.pdf -m C:\Users\ziaud\Desktop\test\testmrg.pdf
int wmain(int argc, wchar_t* argv[])
{
#ifdef NDEBUG
	PoDoFo::PdfError::EnableLogging(false);
	PoDoFo::PdfError::EnableDebug(false);
	if (parse_commandline(argc, argv))
	{
		int code = run();
		if (code == 0)
		{
			printf("Sucessfully done!\n");
		}
		else
		{
			printf("Failed! (error_code:%d)\n", code);
		}
		return code;
	}
	else
	{
		print_help();
		return -2;
	}
#else


	g_bSplitDoc = true;
	g_inp.assign(TEST_PDF_INPUT);
	g_out.assign(TEST_PDF_OUTPUT);

	
	
	
	if (g_bSplitDoc)
	{
		g_opt_data.assign(L"200,208");
		parse_range(g_opt_data.c_str());
	}
	else
	{
		g_opt_data.assign(TEST_PDF_INPUT_MERGE);
	}


	return run();
#endif

}