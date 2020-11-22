#include <curl/curl.h>
#include <string.h>

#include "client.h"

#define EXIT_SUCCESS 0

static char* _message;
static char* _subject;
static char* _to;
static char* _from;
static char* _sender;
static char* _host;
static char* _password;

static void init_fields(
  char* host,
  char* password,
  char* from,
  char* to,
  char* sender,
  char* subject,
  char* message)
{
  _message  = message;
  _subject  = subject;
  _to       = to;
  _from     = from;
  _sender   = sender;
  _host     = host;
  _password = password;
}

struct upload_status {
  int lines_read;
};

static u_int32_t payload_source(void *ptr, u_int32_t size, u_int32_t nmemb, void *userp)
{
  const char* query[] = {
    "To:     <", _to,     ">\r\n",
    "From:   <", _from,   ">",
    "        (", _sender, ")\r\n",
    "Subject: ", _subject, " \r\b",
    "\r\n",   /* splitter between subject and message */
    _message, " \r\n",
    NULL
  };

  struct upload_status* upload_ctx = (struct upload_status*)userp;
  const  char*          data;

  if ((size  == 0  )  ||
      (nmemb == 0  )  ||
     ((size * nmemb)   < 1))
  {
    return EXIT_SUCCESS;
  }

  data = query[upload_ctx->lines_read];
  if (data) {
    u_int32_t length = strlen(data);
    memcpy(ptr, data, length);
    upload_ctx->lines_read++;
    return length;
  }
  return EXIT_SUCCESS;
}

static void smtp_setopts(void* curl_handle, struct curl_slist* recipients, struct upload_status* upload_ctx) {
  curl_easy_setopt(curl_handle, CURLOPT_USERNAME,      _from);
  curl_easy_setopt(curl_handle, CURLOPT_PASSWORD,      _password);
  curl_easy_setopt(curl_handle, CURLOPT_URL,           _host);
  curl_easy_setopt(curl_handle, CURLOPT_USE_SSL,       (long)CURLUSESSL_ALL);
  curl_easy_setopt(curl_handle, CURLOPT_MAIL_FROM,     _from);
  curl_easy_setopt(curl_handle, CURLOPT_MAIL_RCPT,     recipients);
  curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION,  payload_source);
  curl_easy_setopt(curl_handle, CURLOPT_READDATA,      &upload_ctx);
  curl_easy_setopt(curl_handle, CURLOPT_UPLOAD,        1L);
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE,       1L);
}

void sendmail(
  char* host,
  char* password,
  char* from,
  char* to,
  char* sender,
  char* subject,
  char* message)
{
  init_fields(host, password, from, to, sender, subject, message);
  CURL*                curl_handle;
  CURLcode             res = CURLE_OK;
  struct upload_status upload_ctx;
  struct curl_slist*   recipients = NULL;
  upload_ctx.lines_read = 0;
  curl_handle = curl_easy_init();
  if (curl_handle) {
    recipients = curl_slist_append(recipients, to);
    smtp_setopts(curl_handle, recipients, &upload_ctx);
    res = curl_easy_perform(curl_handle);
    if(res != CURLE_OK) {
      fprintf(
        stderr,
        "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
    }
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl_handle);
    curl_easy_reset(curl_handle);
  }
}
