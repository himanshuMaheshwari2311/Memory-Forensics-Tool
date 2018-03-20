#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "../objects/process.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"

using namespace std;

class process_handler
{
    private:
        vector<process> process_list;

    public:
		void generate_processes(ifstream &ifile, profile prf)
		{
            uint64_t addr_val = 0;
            char found_pattern[8];
            char p_name[16];
            cout<<setw(16)<<"Address"<<setw(16)<<"PID"<<setw(16)<<"PPID"<<setw(16)<<"Name"<<endl;
            while(ifile.eof()==0)
	        {
                ifile.read(found_pattern, 8);
                addr_val+=8;
                if(utility_function::compare_array(prf.process_signature, found_pattern, 8))
                {
                    process proc;
                    cout<<setw(16)<<hex<<addr_val - 8;
                    proc.physical_offset = addr_val - 8;

                    ifile.ignore(prf.process_offsets[0]);
                    addr_val += prf.process_offsets[0];

                    ifile.read(reinterpret_cast<char *>(&proc.pid), sizeof(proc.pid));
                    addr_val += 8;
                    cout<<setw(16)<<dec<<proc.pid;

                    ifile.ignore(prf.process_offsets[1]);
                    addr_val += prf.process_offsets[1];
                    
                    ifile.read(reinterpret_cast<char *>(&proc.ppid), sizeof(proc.ppid));
                    addr_val += 8;
                    cout<<setw(16)<<proc.ppid;

                    ifile.ignore(prf.process_offsets[2]);
                    addr_val += prf.process_offsets[2];

                    ifile.read(p_name, 16);
                    addr_val += 16;
                    proc.name = p_name;
                    cout<<setw(16)<<proc.name<<endl;

                    ifile.ignore(prf.process_offsets[3]);

                    process_list.push_back(proc);

                }
                else
                {
                    addr_val += 8;
                }

            }
			
		}
        void print_processes()
        {

        }
        
};

int main(void)
{
    process_handler ph;
    ifstream ifile;
	profile prf(7);
    char fname[] = "../data/samples/win764.vmem";
    
    ifile.open(fname, ios::in | ios::binary);
    if(!ifile)
	{
		cout<<"Error in opening file..!!";
	}	
	cout<<"File opened..";
	cout<<"\n";
    
    ph.generate_processes(ifile, prf);
    ph.print_processes();

}