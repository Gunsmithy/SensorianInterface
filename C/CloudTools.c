/**
 * @file CloudTools.c
 * @author Dylan Kauling
 * @date 24 June 2016
 * @brief Extra tools related to Cloud integration for Sensorian projects
 */

#include "CloudTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/**
 * @brief Defines the struct in which the request response containing the public IP is stored
 */
struct MemoryStruct {
    char *memory;
    size_t size;
};

/**
 * @brief Stores the results of the call to icanhazip.com in memory to be retrieved later for the IP
 */
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

/**
 * @brief Get the public IP address of the Raspberry Pi by requesting it from icanhazip.com
 * @param buf A string array of chars buffer in which to store the public IP, should be size 16
 * @return none
 */
void cloud_get_public_ip(char *buf){
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* will be grown as needed by realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://icanhazip.com");
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        /* some servers don't like requests that are made without a user-agent
           field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        else {
            /*
             * Now, our chunk.memory points to a memory block that is chunk.size
             * bytes big and contains the remote file.
             *
             * Do something nice with it!
             */
            //printf("%s\n",chunk.memory);
            //printf("BLAH");
            strncpy(buf, chunk.memory, 16);
            buf[strcspn(buf, "\r\n")] = 0; //Removes new line characters from the end
        }

        /* always cleanup */
        curl_easy_cleanup(curl);

        free(chunk.memory);

        /* we're done with libcurl, so clean it up */
        curl_global_cleanup();
    }
}

/**
 * @brief Sends an HTTP GET request to the IFTTT.com Maker Channel to trigger a recipe
 * @param key A string array of chars that contains the user's IFTTT API Key
 * @param event A string array of chars that contains the recipe trigger's event name
 * @param timeout A long in seconds of how long to wait on the command before timing out
 * @return none
 */
void cloud_ifttt_trigger(char *key, char *event, long timeout)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    char ifttt_url[128];
    strcpy(ifttt_url, "https://maker.ifttt.com/trigger/");
    strcat(ifttt_url, event);
    strcat(ifttt_url, "/with/key/");
    strcat(ifttt_url, key);

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ifttt_url);

        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}