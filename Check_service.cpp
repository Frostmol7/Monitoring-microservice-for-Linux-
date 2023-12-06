#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstring>

const std::string botToken = "66800881360:AAFjtK2tfBREbZmyscQTaXbDxuNlE7LqK2U"; //My bot without any settings
const long long chatId = /*Your chat Id*/;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up){
  return size * nmemb;
}

void sendTelegramMessage(const std::string& message){
  CURL* curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  if (curl){
    std::stringstream url;
    url << "https://api.telegram.org/bot" << botToken << "/sendMessage?chat_id" << chatId << "&text" << message;

    curl_easy_setopt(curl, CURLOPT, url.str().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

    res = curl_easy_setopt(curl);

    if (res != CURL_OK){
      std::cerr << "Error sending message: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}

bool checkStatus(){
  FILE*pipe = popen("systemctl -l | grep PROCESS_NAME", "r"); //You need to change the PROCESS_NAME to the process you need
  if (!pipe){
    std::cerr << "Error executing command" << std::endl;
    return 1;
  }
  char buffer[128];
  std::string result = "";
  while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    result += buffer;
  pclose(pipe);
  if (result.find("running") != std::string::npos)
    return true;
  else
    return false;
}

int main() {
  while (true) //for infinity loop
  {
    bool isRunning = checkStatus();
    if (!isRunning)
      sendTelegramMessage("Process NAME not found!"); //change NAME 
    std::this_thread::sleep_for(std::chrono::second(300)); //300 seconds = 5 minutes 
  }
  return 0;
}
