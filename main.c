#include <stdio.h>
#include <string.h>

#include "client.h"

int main(int argc, char** argv) {
  /*
  Host is installed by default,
  but you can change it by
  passing --host argument.
  */
  static char* host = "smtps://smtp.gmail.com:465";
  static char* password;
  static char* from;
  static char* to;
  static char* subject;
  static char* content;
  static char* sender;
  if (argc == 1) {
    printf("Please use arguments, more info:\n\tMail --help\n");
    return 0;
  }
  if (strcmp(argv[1], "--help") == 0) {
    printf("Usage:      Mail\n"
           "            --from     <your email>@gmail.com\n"
           "            --to       <your receiver>@gmail.com\n"
           "            --password <your password>\n"
           "            --subject  <Title>\n"
           "            --text     <message>\n"
           "            --sender   <Your name>\n"
           "(optional)  --host     <SMTP host>\n\n"
           "NOTE: default host - smtps://smtp.gmail.com:465, you can always change it."
           "NOTE: if you have an error like:\n  -- "
           "SmtpCmdResp: 535-5.7.8 Username and Password not accepted,\n\n"
           "you should log in into your Gmail account and in your browser go to\n"
           "https://www.google.com/settings/security/lesssecureapps\n"
           "and TURN ON Access for less secure apps.\n");
    return 0;
  }
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "--password") == 0) {
      password = argv[i+1];
    }
    else if (strcmp(argv[i], "--from") == 0) {
      from = argv[i+1];
    }
    else if (strcmp(argv[i], "--to") == 0) {
      to = argv[i+1];
    }
    else if (strcmp(argv[i], "--subject") == 0) {
      subject = argv[i+1];
    }
    else if (strcmp(argv[i], "--text") == 0) {
      content  = argv[i+1];
    }
    else if (strcmp(argv[i], "--sender") == 0) {
      sender  = argv[i+1];
    }
    else if (strcmp(argv[i], "--host") == 0) {
      host = argv[i+1];
    } else if ((argc != 13) && (argc != 15)) {
      fprintf(
        stderr,
        "Wrong arguments were given. Please use --help option.\n");
      return -1;
    }
  }
  sendmail(
    /* Host           */ host,
    /* User password  */ password,
    /* User Gmail     */ from,
    /* Receiver Gmail */ to,
    /* Sender         */ sender,
    /* Subject        */ subject,
    /* Content        */ content
  );
  return 0;
}
