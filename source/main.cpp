#include <xlnt/xlnt.hpp>
#include <excel_adapter.h>
#include <parser.h>


int main(int argc, char *argv[])
{
    xlnt::workbook wb;
    std::vector<std::string> args;
    
    for(int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    
        if(args.size() < 1)
        {
            std::cout << "usage: " << argv[0] << " xlsx_file [sheet_to_print]" << std::endl;
            return 1;
        }

        wb.load(args.front());
        args.erase(args.begin());
    
    if(args.empty())
    {
	for(auto ws : wb)
	{
	    std::cout << ws.title() << " ";
	}

	std::cout << std::endl;

	return 0;
    }

    xlnt::worksheet ws = wb.sheet_by_title(args.front());

    ExcelAdapter adapter(ws);
    Parser parser(adapter);
    parser.Process();

    return 0;
}
