#pragma once
#include <podofo/podofo.h>


void pdf_gc(const wchar_t* input, const wchar_t* output)
{
	PoDoFo::PdfVecObjects objects;
	PoDoFo::PdfParser     parser(&objects);
	objects.SetAutoDelete(true);


	wprintf(L"--> Cleaning up file: %s\n", input);
	parser.ParseFile(input, false);

	PoDoFo::PdfWriter writer(&parser);
	writer.SetPdfVersion(parser.GetPdfVersion());
	/*
	if (parser.GetEncrypted())
	{
		writer.SetEncrypted(*(parser.GetEncrypt()));
	}
	*/
	wprintf(L"--> Saving cleaned file: %s\n", output);
	writer.Write(output);
}

void pdf_split(const wchar_t* output, const wchar_t* input, size_t fromPgNum, size_t toPgNum)
{
	wprintf(L"--> Reading input file: %s\n", input);
	PoDoFo::PdfMemDocument doc(input);

	PoDoFo::PdfMemDocument doc_output;// (input);


	wprintf(L"--> Inserting pages from %d to %d\n", fromPgNum, toPgNum);
	

	size_t count = (toPgNum -fromPgNum) +1;
	size_t start_idx = --fromPgNum;
	


	doc_output.InsertPages(doc, start_idx, count);

	wprintf(L"--> Writing output file: %s\n", output);
	doc_output.Write(output);
}
void pdf_merge(const wchar_t* output, const wchar_t* input, const wchar_t* input2)
{
	wprintf(L"--> Reading file: %s\n", input);
	PoDoFo::PdfMemDocument doc1(input);
	wprintf(L"--> Reading file: %s\n", input2);
	PoDoFo::PdfMemDocument doc2(input2);


	printf("--> Appending %i pages on a document with %i pages.\n", doc2.GetPageCount(), doc1.GetPageCount());
	doc1.Append(input2);


	//doc1.SetPageMode(ePdfPageModeUseBookmarks);
	//doc1.SetHideToolbar();
	//doc1.SetPageLayout(ePdfPageLayoutTwoColumnLeft);

	wprintf(L"--> Writing file: %s\n", output);
	doc1.Write(output);
}