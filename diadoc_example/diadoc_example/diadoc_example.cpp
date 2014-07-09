// diadoc_example.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream> 
#include <fstream>
#include <string>
#include <stdio.h>
#include "curl/curl.h"
#include <cstddef> 
#pragma comment(lib,"curllib.lib")
//---------------------------------------------------------------------------

using namespace std;

static char errorBuffer[CURL_ERROR_SIZE];
static string buffer;
const char * file = "D:\\Documents and Settings\\kuzmin\\Мои документы\\Visual Studio 2010\\Projects\\test\\Debug\\tmp.xml";

string send_message(char * token);
string get_organisation(char * token);
string authorization();
string execute_curl(CURL *curl, struct curl_slist *headerlist);
static int writer(char *data, size_t size, size_t nmemb, string *buffer);

int _tmain(int argc, _TCHAR* argv[])
{
	//==========авторизация с получением токена и преобразование токена в массив char
	string token = authorization();
	const char * c_token = token.c_str();
	char n_token[139];
	strcpy(n_token, c_token); 
	//===============================================================================




	//string uuu = get_organisation(n_token);
	//cout << uuu << endl;
	
	string uuu = send_message(n_token);
	cout << uuu << endl;

	//================запись в файл===================================================
	/*ofstream fout(file, ios::out | ios::app);
	if (!fout.is_open())
	{
		cout << "error" << endl;
		exit(EXIT_FAILURE);
	}
	fout << uuu <<  endl;
	fout.close();*/
	//=================================================================================
    getchar();
	return 0;
}



string send_message(char * token)
{
	CURL *curl;
	struct curl_slist *headerlist=NULL;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
    curl = curl_easy_init();
    if(curl)
        { 
			char head[] = "Authorization: DiadocAuth ddauth_api_client_id=comtec-e628f938-1133-449a-96d4-a105a0181e9a, ddauth_token=";
			char auth_token[246];
			strcpy(auth_token, head);
			strcat(auth_token, token);
			headerlist = curl_slist_append(headerlist, "POST https://diadoc-api.kontur.ru/Authenticate HTTP/1.1");
			headerlist = curl_slist_append(headerlist, auth_token);
			curl_easy_setopt(curl, CURLOPT_POST, true);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "FromBoxId=d7fe98d55f0d4fc196400a97f058a055@diadoc.ru");
			curl_easy_setopt(curl, CURLOPT_URL, "https://diadoc-api.kontur.ru//V3/PostMessage");
			string ttt = execute_curl(curl, headerlist);
			return ttt;
        }
	curl_easy_cleanup(curl);
}

string get_organisation(char * token)
{
	CURL *curl;
	struct curl_slist *headerlist=NULL;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
    curl = curl_easy_init();
    if(curl)
        { 
			char head[] = "Authorization: DiadocAuth ddauth_api_client_id=comtec-e628f938-1133-449a-96d4-a105a0181e9a, ddauth_token=";
			char auth_token[246];
			strcpy(auth_token, head);
			strcat(auth_token, token);
			headerlist = curl_slist_append(headerlist, "POST https://diadoc-api.kontur.ru/Authenticate HTTP/1.1");
			headerlist = curl_slist_append(headerlist, auth_token);
			curl_easy_setopt(curl, CURLOPT_URL, "https://diadoc-api.kontur.ru/GetOrganization?inn=7733012017&outputFormat=xml");
			//curl_easy_setopt(curl, CURLOPT_URL, "https://diadoc-api.kontur.ru//GetCounteragents?myOrgId=7264b16c-6ed4-4c79-9079-c8c3e791d6e9");
			string ttt = execute_curl(curl, headerlist);
			return ttt;
        }
	curl_easy_cleanup(curl);
}

string authorization()
{
	CURL *curl;
	struct curl_slist *headerlist=NULL;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
    curl = curl_easy_init();
    if(curl)
        {   
			headerlist = curl_slist_append(headerlist, "POST https://diadoc-api.kontur.ru/Authenticate HTTP/1.1");
			headerlist = curl_slist_append(headerlist, "Authorization: DiadocAuth ddauth_api_client_id=comtec-e628f938-1133-449a-96d4-a105a0181e9a");
			curl_easy_setopt(curl, CURLOPT_POST, true);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
			curl_easy_setopt(curl, CURLOPT_URL, "https://diadoc-api.kontur.ru/Authenticate?login=kuzmin@comtec.ru&password=Comtec1");
			string execute = execute_curl(curl, headerlist);
			return execute;
        }
	curl_easy_cleanup(curl);
}

string execute_curl(CURL *curl, struct curl_slist *headerlist)
{
	string request;
	CURLcode res;
	headerlist = curl_slist_append(headerlist, "Connection: Keep-Alive");
	headerlist = curl_slist_append(headerlist, "Host: diadoc-api.kontur.ru");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
	//вывести подробный ответ от сервера
	curl_easy_setopt(curl, CURLOPT_VERBOSE, true);
	curl_easy_setopt(curl, CURLOPT_HEADER, true);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	res = curl_easy_perform(curl);
	if (res == CURLE_OK) {      
		int k, n;
		for (int i = 0; i < int(buffer.length()); i++) 
		{
			if (buffer[i] == '\n' && buffer[i+2] == '\n') 
			{
				k = i+3;
				n = k-1;
			}
		}
		for (k; k < int(buffer.length()); k++) 
		{
			request += buffer[k];
		}
	}
	else  
	{
		request = errorBuffer; 
	}
	return request;
}

static int writer(char *data, size_t size, size_t nmemb, string *buffer)
{
  //переменная - результат, по умолчанию нулевая
  int result = 0;
  //проверяем буфер
  if (buffer != NULL)
  {
    //добавляем к буферу строки из data, в количестве nmemb
    buffer->append(data, size * nmemb);
    //вычисляем объем принятых данных
    result = size * nmemb;
  }
  //вовзращаем результат
  return result;
}


