#include <iostream>
#include <thread>
#include <chrono>
#include "ping.h"

int main()
{
	std::cout << "Hello from main!" << std::endl;
	while (true)
	{
		int i = Ping::Ping("192.168.1.5", 2000);

		std::string result = (i == 0) ? "passed" : "failed";

		std::cout << result << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

}