
#include <iostream>
#include <iomanip>
#include "quaternion.h"

using namespace std;

void
Quaternion::debug_print() const
{
	cout << "< Quaternion >" << endl
		 << setprecision( 6 )
		 << setiosflags( ios::fixed )
		 << "[x]" << setw(10) << x << " "
		 << "[y]" << setw(10) << y << " "
		 << "[z]" << setw(10) << z << " "
		 << "[w]" << setw(10) << w << endl;	
}
