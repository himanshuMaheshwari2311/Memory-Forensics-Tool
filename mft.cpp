#define mainfunc

#include <fstream>
#include <cstring>
#include <iostream>
#include <string.h>
#include <ctime>

#include "handlers/process_handler.cpp"
#include "handlers/dll_object_handler.cpp"
#include "handlers/phandle_handler.cpp"
#include "handlers/service_handler.cpp"
#include "handlers/registry_handler.cpp"
#include "handlers/kernel_module_handler.cpp"
#include "handlers/network_handler.cpp"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return -1;
    }
    char *fname = argv[1];
    profile prf;
    if (strcmp(argv[2], "10") == 0)
    {
        prf.init_as_win10();
    }
    else
    {
        prf.init_as_win7();
    }

    ifstream ifile;
    ifile.open(fname, ios::in | ios::binary);
    if (!ifile)
    {
        cout << "Error in opening file..!!";
        return -1;
    }
    process_handler ph;
    dll_object_handler dh;
    phandle_handler hh;
    service_handler sh;
    registry_handler rh;
    kernel_module_handler kh;
    network_handler nh;

    cout << "Generating Registry List" << endl;
    vector<registry> registry_list = rh.get_registry_list(ifile, prf);
    cout << "Generating Process List" << endl;
    vector<process> process_list = ph.get_process_list(ifile, prf);
    cout << "Generating Dll List" << endl;
    vector<dll_object> dll_object_list = dh.get_dll_object_list(ifile, prf, ph);
    cout << "Generating Phandle List" << endl;
    vector<phandle> phandle_list = hh.get_phandle_list(ifile, prf, ph);
    cout << "Generating Service List" << endl;
    vector<service> service_list = sh.get_service_list(ifile, prf);
    cout << "Generating Kernel List" << endl;
    vector<kernel_module> kernel_list = kh.get_kernel_list(ifile, prf);
    cout<< "Generating Network List" << endl;
    vector<network> network_list = nh.get_network_list(ifile, prf);

    string json = "";

    json += "{ ";

    json += "\"os_name\" : ";
    if (prf.type == 7)
    {
        json += "\"win7\", ";
    }
    else
    {
        json += "\"win10\", ";
    }

    json += "\"file_name\" : ";
    json += "\"";
    json += fname;
    json += "\"";
    json += ", ";

    json += "\"case_overview\": ";
    json += "\"\", ";

    json += "\"case_acquisition\": ";
    json += "\"\", ";

    json += "\"case_findings\": ";
    json += "\"\", ";

    json += "\"case_conclusion\": ";
    json += "\"\", ";

    json += "\"artifacts\" : ";
    json += "[ ";

    json += ph.get_info() + ", ";
    json += dh.get_info() + ", ";
    json += hh.get_info() + ", ";
    json += sh.get_info() + ", ";
    json += rh.get_info() + ", ";
    json += kh.get_info() + ", ";
    json += nh.get_info();
    
    json += "] ";

    json += "}";
    
    replace(json.begin(), json.end(), '\\', '/');
    cout << json << endl;

    ofstream ofile;

    string case_name;
    int len = 0;
    for (int i = 0; fname[i]; i++)
    {
        if (fname[i] == '/' || fname[i] == '\\')
        {
            case_name = "";
            i++;
        }
        case_name += fname[i];
    }
    time_t rawtime;
    time(&rawtime);
    case_name += "_";
    case_name += ctime(&rawtime);
    case_name[case_name.length() - 1] = 'x';
    case_name += ".json";
    replace(case_name.begin(), case_name.end(), ':', '-');
    replace(case_name.begin(), case_name.end(), ' ', '-');

    string out_file_name;
    out_file_name += "../data/json/";
    out_file_name += case_name;
    cout << out_file_name << endl;
    ofile.open(out_file_name, ios::out | ios::binary);
    ofile << json;

    cout << "Json Write Complete" << endl;

    return 0;
}