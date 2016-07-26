#include "idc.idc"

static main()
{
	auto funcname = GetFunctionName(here);
	Message("export: func: %s: ..\n", funcname);
	if (funcname == "")
	{
		Warning("export: no function");
		return;
	}

	auto filename = AskFile(1, "*.asm", "asm file");
	if (filename == 0)
	{
		Warning("export: no file!\n");
		return;
	}

	auto file = fopen(filename, "w");
	if (file == 0)
	{
		Warning("export: open file %s failed\n", filename);
		return;
	}

	auto be = FirstFuncFchunk(here);
	auto ee = FindFuncEnd(here);
	GenerateFile(OFILE_ASM, file, be, ee, 0);
	fclose(file);

	Message("export: func: %s: ok\n", funcname);
}
