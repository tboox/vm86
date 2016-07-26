#include "idc.idc"

static dump(addr, file)
{
	// find tail
	auto ee;
	for (ee = NextHead(addr, BADADDR); ee != BADADDR; ee = NextHead(ee, BADADDR))
	{
		if (Name(ee) != "") break;
	}
	if (ee == BADADDR) return ;

	// make file
	GenerateFile(OFILE_ASM, file, addr, ee, 0);

	// walk
	auto ea;
	for (ea = addr; ea != ee; ea = NextHead(ea, BADADDR))
	{
		auto line = GetDisasm(ea);
		auto p = strstr(line, "dd offset ");
		if (p != -1) 
		{
			auto soff = substr(line, 10, strlen(line));
			auto ploc = LocByName(soff);
			Message("%s: %x\n", soff, ploc);
			if (ploc != BADADDR) 
			{
				// flags
				auto flags = GetFlags(ploc);
//				Message("FF_COMM: %d\n", !(flags & FF_COMM));

				// dump it
				if ((isData(flags) || isUnknown(flags)) && !(flags & FF_COMM) && Name(ploc) != "") 
				{
					// clear flags
					SetFlags(ploc, flags | FF_COMM);

					// trace
//					Message("FF_COMM: %d\n", !(GetFlags(ploc) & FF_COMM));

					// dump
					dump(ploc, file);
				}
			}
		}
	}
}
static main()
{
	// flags
	auto flags = GetFlags(here);

	// is data?
	if (!isData(flags) && !isUnknown(flags))
	{
		Message("export: no data\n");
		return ;
	}

	// has name?
	auto name = Name(here);
	if (name == "")
	{
		Message("export: no name\n");
		return ;
	}

	// init filename
	auto filename = AskFile(1, "*.asm", "asm file");
	if (filename == 0)
	{
		Warning("export: no file!\n");
		return;
	}

	// init file
	auto file = fopen(filename, "w");
	if (file == 0)
	{
		Warning("export: open file %s failed\n", filename);
		return;
	}

	// dump
	dump(here, file);

	// exit file
	fclose(file);

	// ok
	Message("export: data: ok\n");
}
