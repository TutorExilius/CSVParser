/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		12.10.2018
Last update:		11.06.2020

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/

#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>

#include "csvparser.h"

#include <fstream>

int main()
{
	CSVParser csvParser{ ';' };
	csvParser.parse( R"(C:\Users\exi\Desktop\screenshots_and_csv\anonym\ano_2019__4_clusters.csv)" );
	auto grouped{ csvParser.groupByColumn("Cluster")};

	auto count_grouped{ csvParser.countedGroupsByColumn( "Cluster" ) };

	std::ofstream out{ R"(C:\Users\twitch\Desktop\OUT.csv)" };

	if( !out )
		std::wcout << "nope";

	out << csvParser.getCSVOutput();

	out.close();

	system( "PAUSE" );
	return 0;
}
