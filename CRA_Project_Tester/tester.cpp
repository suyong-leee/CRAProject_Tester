#include "tester.h"
#include<string>
#include<stdexcept>
using namespace std;
void SSDTest_FullWriteAndReadCompare::run(string command1, string command2)
{
	//• 0 ~4번 LBA까지 Write 명령어를 수행한다.
	//• 0 ~4번 LBA까지 ReadCompare 수행
	//• 5 ~9번 LBA까지 다른 값으로 Write 명령어를 수행한다.
	//• 5 ~9번 LBA까지 ReadCompare 수행
	//• 10 ~14번 LBA까지 다른 값으로 Write 명령어를 수행한다.
	//• 10 ~14번 LBA까지 ReadCompare 수행
	//• 위와 같은 규칙으로 전체 영역에 대해 Full Write + Read Compare를 수행한다.
	string writeBuffer[100];
	string readBuffer;
	string buffer = "0x11111111";
	for (int i = 0; i < 100; i += 5)
	{
		for (int j = i; j < i + 5; j++) {
			write->run(to_string(j),buffer);
			writeBuffer[j] = buffer;
		}
		for (int j = i; j < i + 5; j++) {
			readBuffer = read->read(to_string(j));
			if (writeBuffer[j] == readBuffer) {
				continue;
			}
			else {
				throw exception("Compare Failed\n");
			}
		}
	}

}

void SSDTest_PartialLBAWrite::run(string param1, string param2)
{
}
