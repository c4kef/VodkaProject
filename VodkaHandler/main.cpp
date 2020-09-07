/* VodkaHandler Author C4ke#0002
*  Ты думаешь это магазин с водярой? 
*  Я скажу так, ди нахер
*/

#include <chrono>
#include <thread>
#include <iostream>
using namespace std::chrono_literals;

static uintptr_t typeReq = 151515;//0x3058
static uintptr_t memAddress = 161616;//0x3048
static uintptr_t memSize = 171717;//0x3050
static uintptr_t retAnswer = 181818;//0x3038
static float fRetAnswer = 202020;//0x3040
static uintptr_t pid = 191919;//0x3060

int main(const int argc, char** argv)
{
	std::cout << &typeReq;
	std::cout << &memAddress;
	std::cout << &memSize;
	std::cout << &retAnswer;
	std::cout << &pid;
	std::cout << &fRetAnswer;
	while (true)
		std::this_thread::sleep_for(1s);
}