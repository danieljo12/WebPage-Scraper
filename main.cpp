#include <iostream>
#include <string>
#include <vector>

#define CURL_STATICLIB
#include <curl.h>

/*
* This function gets called by libcurl as soon as there is data received that needs to be saved.
* The size of the data pointed to by char* data is size multiplied with size_t nmemb, it will not be zero terminated.
* Return the number of bytes actually taken care of.
* If that amount differs from the amount passed to your function, it'll signal an error to the library.
* This will exit the transferand return CURLE_WRITE_ERROR.
*/
static int writer(char* data, size_t size, size_t nmemb, std::string* buffer) {
	unsigned int result = 0;
	if(buffer != NULL) {
		buffer->append(data, size * nmemb);
		result = size * nmemb;
	}
	return result;
}

std::string buffer;
std::vector<std::string> websiteData;

void getData() {
	int httpCode(0);
	const std::string url("https://www.dreamincode.net/forums/topic/78802-martyr2s-mega-project-ideas-list/");

	CURL* curl = curl_easy_init();

	// Set remote URL.
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	// Follow HTTP redirects if necessary.
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	// Hook up data handling function.
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

	// Hook up data container (will be passed as the last parameter to the
	// callback handling function).  Can be any pointer type, since it will
	// internally be passed as a void pointer.
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

	// Run our HTTP GET command, capture the HTTP response code, and clean up.
	curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
	curl_easy_cleanup(curl);

	if(httpCode == 200) {
		std::cout << "\nGot successful response from " << url << std::endl;

		while(true) {
			std::size_t found = buffer.find("bbc"); // looks for the class bbc and finds its pos
			if(found != std::string::npos) {

				websiteData.push_back(buffer.substr(found, 40));
				buffer.erase(found, 40);
			}
			else {
				break;
			}
		}
	}
	else {
		std::cout << "Couldn't GET from " << url << " - exiting" << std::endl;
		return;
	}
}

int main() {
	getData();



	for(auto i : websiteData) {
		std::cout << "\n\nfound\n\n";
		std::cout << i;
	}

}
