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
	CSVParser csvParser{ L';' };
	csvParser.parse( LR"(C:\Users\exi\Desktop\screenshots_and_csv\anonym\ano_2019__4_clusters.csv)" );
	auto grouped{ csvParser.groupByColumn(L"Cluster")};

	std::unordered_map<std::wstring, int> groups_count;

	for( const auto &group : grouped )
	{
		groups_count[group.first] += group.second.size();
	}

	std::wofstream out{ LR"(C:\Users\twitch\Desktop\OUT.csv)" };

	if( !out )
		std::wcout << L"nope";

	out << csvParser.getCSVOutput();

	out.close();

	system( "PAUSE" );
	return 0;
}
